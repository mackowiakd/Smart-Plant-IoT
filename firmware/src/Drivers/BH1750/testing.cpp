

// #include <Arduino.h>
// #include <Wire.h>

// // Ustawienia sprawdzone (z działającego skanera)
// #define PIN_SDA 6
// #define PIN_SCL 7
// #define BH1750_ADDR 0x5C
// #define PIN_FAKE 5



// // Komendy BH1750
// #define POWER_ON 0x01
// #define MODE_H_RES 0x10

// void setup() {
//   Serial.begin(115200);
  
//   // Opoznienie dla USB
//   long startWait = millis();
//   while (!Serial && (millis() - startWait < 4000));
//   delay(1000);

//   Serial.println("\n--- START HYBRYDOWY ---");

// //  // 1. KONFIGURACJA "SZTUCZNEJ MASY"
 
// //   pinMode(PIN_FAKE, OUTPUT);
// //   digitalWrite(PIN_FAKE, LOW); 
// //   Serial.println("Pin D3 (GPIO 5) ustawiony jako GND dla adresu.");
// //   delay(10); // Dajmy chwilę na ustabilizowanie napięcia
//   // 2. Start I2C
//   Wire.end(); 
//   Wire.setPins(PIN_SDA, PIN_SCL);
//   if (!Wire.begin()) {
//     Serial.println("BLAD KRYTYCZNY: Wire nie startuje.");
//     while(1);
//   }

//   // 2. PETLA "DOBIJANIA SIĘ" (To jest klucz do sukcesu!)
//   Serial.print("Szukam czujnika pod 0x5C...");
  
//   bool znaleziono = false;
//   for (int i = 0; i < 10; i++) { // Probujemy 10 razy
//     Wire.beginTransmission(BH1750_ADDR);
//     if (Wire.endTransmission() == 0) {
//       znaleziono = true;
//       Serial.println(" SUKCES! (Znaleziono)");
//       break; // Wychodzimy z pętli, bo działa
//     }
//     Serial.print("."); // Kropka oznacza kolejną próbę
//     delay(100); // Czekamy chwilę
//   }

//   if (!znaleziono) {
//     Serial.println("\nBLAD: Czujnik nie odpowiada po 10 probach.");
//     Serial.println("Ale skoro skaner dziala, to zignorujmy to i idzmy dalej!");
//     // Nie robimy while(1) - idziemy na żywioł!
//   }

//   // 3. Konfiguracja czujnika
//   Wire.beginTransmission(BH1750_ADDR);
//   Wire.write(POWER_ON);
//   Wire.endTransmission();
  
//   Wire.beginTransmission(BH1750_ADDR);
//   Wire.write(MODE_H_RES);
//   Wire.endTransmission();
// }

// void loop() {
//   // Pytamy o dane
//   int count = Wire.requestFrom(BH1750_ADDR, 2);

//   if (count == 2) {
//     byte buff[2];
//     buff[0] = Wire.read();
//     buff[1] = Wire.read();
    
//     uint16_t val = ((buff[0] << 8) | buff[1]);
//     float lux = val / 1.2;
    
//     Serial.print("Swiatlo: ");
//     Serial.print(lux);
//     Serial.println(" lx");
//   } else {
//     // Jeśli odczyt się nie uda, spróbujmy go "obudzić" ponownie
//     Serial.println("Brak danych... probuje zrestartowac pomiar...");
//     Wire.beginTransmission(BH1750_ADDR);
//     Wire.write(POWER_ON);
//     Wire.write(MODE_H_RES);
//     Wire.endTransmission();
//   }

//   delay(1000);
// }

// /*BUTTON + DIODA + BH1750
// #include <Arduino.h>
// #include "LightSensor.h"
// #include "button&diode/Indicator.h"

// // --- KONFIGURACJA ---
// #define PIN_SDA 6
// #define PIN_SCL 7
// #define BH1750_ADDR 0x23 // Zmień na 0x5C jeśli naprawiłaś PCB, 0x23 jeśli na kablach

// #define PIN_LED 2     // D0
// #define PIN_BTN 3     // D1

// // --- OBIEKTY ---
// // Tworzymy nasze klocki
// LightSensor czujnikSwiatla(BH1750_ADDR, PIN_SDA, PIN_SCL);
// Indicator panelSterowania(PIN_LED, PIN_BTN);

// void setup() {
//     Serial.begin(115200);
    
//     // Czekamy na USB (standardowa procedura dla C3)
//     long startWait = millis();
//     while (!Serial && (millis() - startWait < 4000));
//     delay(1000);

//     Serial.println("\n--- START SYSTEMU (Wersja Obiektowa) ---");

//     // 1. Uruchamiamy panel (guzik + led)
//     panelSterowania.begin();

//     // 2. Uruchamiamy czujnik
//     if (czujnikSwiatla.begin()) {
//         Serial.println("Czujnik swiatla: ZAINICJOWANY.");
//     } else {
//         Serial.println("Czujnik swiatla: BLAD I2C!");
//         // Nie robimy while(1), żeby guzik i tak działał!
//     }
// }

// void loop() {
//     // 1. Obsługa przycisku i diody (musi być szybka)
//     panelSterowania.update();

//     // 2. Odczyt światła co 1 sekundę (żeby nie zalewać konsoli)
//     static long ostatniPomiar = 0;
//     if (millis() - ostatniPomiar > 1000) {
//         float lux = czujnikSwiatla.readLux();
        
//         Serial.print("Swiatlo: ");
//         if (lux >= 0) {
//             Serial.print(lux);
//             Serial.println(" lx");
//         } else {
//             Serial.println("Blad odczytu / Reanimacja...");
//         }
        
//         ostatniPomiar = millis();
//     }
    
//     // Krótkie opóźnienie dla stabilności
//     delay(10);
// }

// */