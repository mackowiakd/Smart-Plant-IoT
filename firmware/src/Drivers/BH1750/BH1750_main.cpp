// #include <Arduino.h>
// #include "LightSensor.h" // Dołączamy naszą klasę

// // Definiujemy piny
// #define PIN_SDA 6
// #define PIN_SCL 7

// // Tworzymy obiekt naszego czujnika
// LightSensor mojCzujnik(PIN_SDA, PIN_SCL);

// void setup() {
//     Serial.begin(115200);
//     delay(2000);
//     // Czekamy na serial monitor
//     NewFunction();
//     Serial.println("Start systemu...");

//     // Uruchamiamy czujnik używając naszej metody
//     if (mojCzujnik.begin()) {
//         Serial.println("Czujnik BH1750 dziala!");
//     } else {
//         Serial.println("Blad czujnika!");
//         while(1); // Stop
//     }
// }

// void NewFunction()
// {
//     long startWait = millis();
//     while (!Serial && (millis() - startWait < 6000))
        
//         delay(1000);
// }

// void loop() {
//     // Czytamy wartość
//     float lux = mojCzujnik.readLux();
    
//     Serial.print("Swiatlo: ");
//     Serial.print(lux);
//     Serial.println(" lx");
    
//     delay(1000);
// }