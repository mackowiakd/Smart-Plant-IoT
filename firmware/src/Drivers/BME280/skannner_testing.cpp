// #include <Arduino.h>
// #include <Wire.h>


// // Używamy numerów GPIO zamiast nazw D4/D5 dla pewności
// // Zgodnie z Twoim schematem: SDA=6, SCL=7
// #define MY_SDA 6
// #define MY_SCL 7

// void setup() {
//   Serial.begin(115200);
//   delay(2000); 
//   Serial.println("\n--- I2C SCANNER (GPIO 6 i 7) ---");

//   // Reset magistrali
//   Wire.end(); 
  
//   // Konfiguracja
//   // WAŻNE: Usunąłem pinMode(INPUT_PULLUP) - Wire robi to samo, ale lepiej!
//   Wire.setPins(MY_SDA, MY_SCL);
  
//   if (!Wire.begin()) {
//     Serial.println("Blad startu Wire!");
//     while(1); 
//   }

//   Serial.println("Start skanowania...");
// }

// void loop() {
//   byte error, address;
//   int nDevices = 0;

//   Serial.println("Skanowanie...");
// //int arr[] ={32, 35, 92, 104,118, 119};
//   for (address = 1; address < 127; address++) {
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
//     Serial.print(address);
//     Serial.print("/n");

//     if (error == 0) {
//       Serial.print("\n Znaleziono urzadzenie I2C pod adresem: 0x");
      
//       Serial.print(address,HEX);
      
//       // Podpowiedź co to może być:
//       if (address == 0x76 || address == 0x77) Serial.print(" (Prawdopodobnie BME280)");
//       if (address == 0x23 || address == 0x5C) Serial.print(" (Prawdopodobnie BH1750)");
      
//       Serial.println();
//       nDevices++;
//     } else if (error == 4) {
//       Serial.print("Nieznany blad przy adresie 0x");
      
      
//     }
//   }

//   if (nDevices == 0) {
//     Serial.println("Nie znaleziono zadnych urzadzen I2C\n");
//   } else {
//     Serial.println("Koniec skanowania\n");
//   }
  
//   delay(5000);
// }
