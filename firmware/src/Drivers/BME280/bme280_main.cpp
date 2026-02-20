
// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BME280.h>
// // Twoje piny I2C
// #define PIN_SDA 6
// #define PIN_SCL 7
// // Definicja adresu (0x76 to standard dla modułów HW-611)
// #define BME_ADDR 0x76

// Adafruit_BME280 bme; // Obiekt czujnika

// void setup() {
//   Serial.begin(115200);
  
//   // Opoznienie startowe
//   long startWait = millis();
//   while (!Serial && (millis() - startWait < 4000));
//     delay(1000);
//     Serial.println("\n--- TEST BME280 START ---");

//     // 1. Uruchomienie I2C na pinach XIAO (SDA=6, SCL=7)
//     Wire.begin(PIN_SDA, PIN_SCL);

//     // 2. Start czujnika
//     // Ważne: Biblioteka Adafruit domyślnie szuka pod 0x77.
//     // Musimy jej podać &Wire (magistralę)
//     if (!bme.begin(BME_ADDR, &Wire)) {
//         Serial.println("BLAD: Nie znaleziono czujnika BME280!");
//         Serial.println("Sprawdz kable i czy adres to na pewno 0x76.");
//         while (1); // Stop
//     }

//     Serial.println("BME280 znaleziony! Rozpoczynam pomiary...");
//     Serial.println("------------------------------------------------");
// }

// void loop() {
//     Serial.print("Temperatura: ");
//     Serial.print(bme.readTemperature());
//     Serial.println(" *C");

//     Serial.print("Wilgotnosc:  ");
//     Serial.print(bme.readHumidity());
//     Serial.println(" %");

//     Serial.print("Cisnienie:   ");
//     Serial.print(bme.readPressure() / 100.0F);
//     Serial.println(" hPa");

//     Serial.println("------------------------------------------------");
    
//     // Test: Czy to jednak BMP280?
//     if (bme.readHumidity() == 0.00) {
//         Serial.println("UWAGA: Wilgotnosc wynosi 0%. To prawdopodobnie tanszy czujnik BMP280!");
//     }

//     delay(2000);
// }
