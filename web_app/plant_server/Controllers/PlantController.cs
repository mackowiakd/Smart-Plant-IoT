using Microsoft.AspNetCore.Mvc;
using plant_server.data; // Upewnij się, że namespace pasuje do Twojego modelu PlantData
using System.Text.Json;

namespace plant_server.Controllers
{
    [ApiController]
    [Route("api/plant")]
    public class PlantController : ControllerBase
    {
        // Ścieżka do pliku (będzie szukać w folderze, gdzie uruchamia się aplikacja)
        private const string FilePath = "plant_logs.json";

        // Pamięć podręczna
        private static List<PlantData> _history = new();

        // Konstruktor statyczny - uruchamia się RAZ przy starcie serwera
        static PlantController()
        {
            LoadDataFromFile();
        }

        // 1. Odbieranie danych z ESP32 (i zapis do pliku)
        [HttpPost]
        public IActionResult ReceiveData([FromBody] PlantData data)
        {
            data.Timestamp = DateTime.Now; // Nadpisujemy czas serwera

            _history.Add(data);

            // Opcjonalnie: Trzymaj tylko ostatnie 500 pomiarów, żeby plik nie urósł do gigabajtów
            if (_history.Count > 500)
            {
                _history.RemoveAt(0);
            }

            SaveDataToFile(); // <--- ZAPIS DO PLIKU

            Console.WriteLine($"[LOG] Wilg: {data.Moisture}%, Lux: {data.Lux}, Temp: {data.Temperature}, Hum: {data.Humidity}%, Pres: {data.Pressure} hPa");
            return Ok();
        }

        // 2. Wysyłanie historii do Reacta
        [HttpGet("history")]
        public IActionResult GetHistory()
        {
            return Ok(_history);
        }

        // 3. Aktualny stan
        [HttpGet("current")]
        public IActionResult GetCurrent()
        {
            var last = _history.LastOrDefault();
            if (last == null) return NoContent();
            return Ok(last);
        }

        // --- Metody pomocnicze do plików ---

        private static void LoadDataFromFile()
        {
            if (System.IO.File.Exists(FilePath))
            {
                try
                {
                    string json = System.IO.File.ReadAllText(FilePath);
                    var data = JsonSerializer.Deserialize<List<PlantData>>(json, new JsonSerializerOptions
                    {
                        PropertyNameCaseInsensitive = true
                    });
                    if (data != null)
                    {
                        _history = data;
                        Console.WriteLine($"[SYSTEM] Zaladowano {_history.Count} pomiarow z pliku.");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[BLAD] Nie udalo sie wczytac pliku: {ex.Message}");
                }
            }
            else
            {
                Console.WriteLine("[SYSTEM] Brak pliku z danymi. Startuje na czysto.");
            }
        }

        private static void SaveDataToFile()
        {
            try
            {
                // Formatowanie z wcięciami, żebyś mogła łatwo czytać plik w Notatniku
                var options = new JsonSerializerOptions { WriteIndented = true };
                string json = JsonSerializer.Serialize(_history, options);
                System.IO.File.WriteAllText(FilePath, json);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[BLAD] Nie udalo sie zapisac pliku: {ex.Message}");
            }
        }
    }
}