// #include <Arduino.h>
// #include <Wire.h>
// #include "Drivers/RTC/RealTimeClock.h"

// // Definicje pinów I2C
// #define PIN_SDA 6
// #define PIN_SCL 7

// RealTimeClock zegar;

// void setup() {
//     Serial.begin(115200);
//     // Czekanie na USB
//     long start = millis();
//     while(!Serial && (millis()-start < 4000));
//     Serial.println("\n--- start ---");
//     delay(5000);
//     Serial.println("\n--- TEST ZEGARA DS3231 ---");

//     // Inicjalizacja I2C
//     Wire.begin(PIN_SDA, PIN_SCL);

//     if (zegar.begin()) {
//         Serial.println("RTC: ZNALEZIONY i gotowy!");
//     } else {
//         Serial.println("RTC: BLAD! Nie widze ukladu DS3231 (Adres 0x68).");
//         Serial.println("Sprawdz kable SDA/SCL i zasilanie.");
//         while(1); // Stop
//     }
// }

// void loop() {
//     // 1. Wyświetl godzinę
//     Serial.print("Godzina: ");
//     Serial.print(zegar.getTimeString());
    
//     // 2. Wyświetl temperaturę (bonus z DS3231)
//     Serial.print(" | Temp RTC: ");
//     Serial.print(zegar.getTemperature());
//     Serial.println(" C");

//     // 3. Test logiczny (np. czy jest noc?)
//     int h = zegar.getHour();
//     if (h >= 22 || h < 6) {
//         Serial.println(" -> Jest NOC (Ciiisza...)");
//     } else {
//         Serial.println(" -> Jest DZIEN (Mozna podlewac)");
//     }

//     Serial.println("-----------------------------");
//     delay(2000);
// }