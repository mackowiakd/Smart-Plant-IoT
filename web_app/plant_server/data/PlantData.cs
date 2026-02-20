namespace plant_server.data
{
    public class PlantData
    {
        public DateTime Timestamp { get; set; } = DateTime.Now; // Oś X wykresu
        public float Moisture { get; set; }
        public float Lux { get; set; }
        public float Temperature { get; set; }
        public float Humidity { get; set; } // <--- NOWE
        public float Pressure { get; set; } // <--- NOWE
        public string Status { get; set; } // <--- DODAJ TĘ LINIĘ

    }
}
