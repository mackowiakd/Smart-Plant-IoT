// #include <Arduino.h>
// #include <Adafruit_NeoPixel.h>

// #define PIN_LED 3    // Upewnij się, że to ten pin!
// #define NUM_LEDS 1

// Adafruit_NeoPixel pixels(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);

// void setup() {
//     Serial.begin(115200);
//     delay(2000);
//     Serial.println("START TESTU DIODY");
    
//     pixels.begin();
//     pixels.setBrightness(50);
// }

// void loop() {
//     // 1. Czerwony
//     Serial.println("Kolor: CZERWONY");
//     pixels.setPixelColor(0, pixels.Color(255, 0, 0));
//     pixels.show();
//     delay(2000);

//     // 2. Zielony
//     Serial.println("Kolor: ZIELONY");
//     pixels.setPixelColor(0, pixels.Color(0, 255, 0));
//     pixels.show();
//     delay(2000);

//     // 3. Niebieski
//     Serial.println("Kolor: NIEBIESKI");
//     pixels.setPixelColor(0, pixels.Color(0, 0, 255));
//     pixels.show();
//     delay(2000);
    
//     // 4. Wyłącz
//     Serial.println("Kolor: OFF");
//     pixels.clear();
//     pixels.show();
//     delay(2000);
// }

// // #include <Arduino.h>

// // #include "Indicator.h"

// // // --- KONFIGURACJA ---
// // #define PIN_SDA 6
// // #define PIN_SCL 7
// // #define BH1750_ADDR 0x23 // Zmień na 0x5C jeśli naprawiłaś PCB, 0x23 jeśli na kablach

// // #define PIN_LED 3     // D2
// // #define PIN_BTN 4     // D3

// // // --- OBIEKTY ---


// // Indicator panelSterowania(PIN_LED, PIN_BTN);

// // void setup() {
// //     Serial.begin(115200);
    
// //     // Czekamy na USB (standardowa procedura dla C3)
// //     long startWait = millis();
// //     while (!Serial && (millis() - startWait < 4000));
// //     delay(1000);

// //     Serial.println("\n--- START SYSTEMU (Wersja Obiektowa) ---");
// //     pinMode(PIN_BTN, INPUT_PULLUP);
// //     Serial.println("\n--- TEST PRZYCISKU ---");

// //     // 1. Uruchamiamy panel (guzik + led)
// //     panelSterowania.begin();

  
// // }

// // void loop() {
    
   
    
    

// //     // ... dalsza część Twojego setupu ...
// //     long start = millis();
// //     // 1. Obsługa przycisku i diody (musi być szybka)
// //     // panelSterowania.update();

    
// //     int stan = digitalRead(PIN_BTN);
    
// //     Serial.print("Stan pinu: ");
// //     Serial.print(stan);
    
// //     if (stan == 0) {
// //         Serial.println(" (LOW - WCIŚNIĘTY / ZWARTY)");
// //         panelSterowania.setColor(240, 5, 145);
         
// //     } else {
// //         Serial.println(" (HIGH - PUSZCZONY / OK)");
// //          panelSterowania.setColor(26, 44, 240);

// //     }
   
// // }

