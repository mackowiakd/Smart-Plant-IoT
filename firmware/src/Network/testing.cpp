// #include <Arduino.h>
// #include <WiFi.h>
// #include <WebServer.h>

// WebServer server(80);

// void setup() {
//     Serial.begin(115200);
//     delay(1000);
//     Serial.println("\n--- TEST SAMEGO AP ---");

//     // 1. Czyścimy starą konfigurację
//     WiFi.disconnect(true);
//     WiFi.mode(WIFI_AP);

//     // 2. Prosta konfiguracja (BEZ softAPConfig - używamy domyślnego 192.168.4.1)
//     // Hasło musi mieć min. 8 znaków!
//     bool wynik = WiFi.softAP("Roslina_TEST", "12345678");

//     if (wynik) {
//         Serial.println("Siec 'Roslina_TEST' utworzona!");
//         Serial.print("Adres IP: ");
//         Serial.println(WiFi.softAPIP());
//     } else {
//         Serial.println("BLAD tworzenia sieci!");
//     }

//     server.on("/", []() {
//         server.send(200, "text/plain", "Czesc! To dziala!");
//     });
//     server.begin();
// }

// void loop() {
//     // To jest kluczowe - procesor musi ciągle "słuchać"
//     server.handleClient();
//     delay(2); // Dajemy oddech WiFi
// }