// #include <Arduino.h>
// #include "WateringUnit.h"

// // Piny z Twojego schematu
// #define PIN_MOISTURE 2   // D0 (GPIO 2) - Analog IN
// #define PIN_PUMP     21  // D6 (GPIO 21) - Digital OUT

// WateringUnit doniczka(PIN_PUMP, PIN_MOISTURE);

// void setup() {
//     Serial.begin(115200);
//     long start = millis();
//     while(!Serial && (millis()-start < 4000));
    
//     Serial.println("\n--- TEST M5STACK WATERING UNIT (GPIO) ---");
    
//     doniczka.begin();
//     Serial.println("Piny skonfigurowane.");
// }

// void loop() {
//     // 1. Odczyt wilgotności
//     int raw = doniczka.readRaw();
//     Serial.print("Wilgotnosc powietrze RAW: ");
//     Serial.print(raw);
    
//     // 2. Test dotyku (interpretacja)
//     if (raw > 2500) Serial.print(" (SUCHO / POWIETRZE)");
//     else if (raw < 1500) Serial.print(" (MOKRO / WODA)");
//     else Serial.print(" (WILGOTNO)");
//     delay(10000);
//     Serial.println();

//     // 3. test pompki (co 5 sekund)
   
//     // static long lastPumpTime = 0;
//     // if (millis() - lastPumpTime > 5000) {
//     //     Serial.println(">>> TEST POMPKI (Bzzzt!) <<<");
//     //     doniczka.setPump(true);
//     //     delay(1000); // Tylko 200ms, żeby nie męczyć silnika na sucho
//     //     doniczka.setPump(false);
//     //     lastPumpTime = millis();
//     // }

//      raw = doniczka.readRaw();
//     Serial.print("Wilgotnosc woda RAW: ");
// }

