#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <DNSServer.h>

class CloudClient {
private:
    const char* _ssid;
    const char* _pass;
    String _serverUrl = "http://192.168.137.1:5202/api/plant"; 
    
    bool _isAPMode = false;
    WebServer* _server;
    DNSServer* _dnsServer;

    // Dane do wyświetlenia na stronie
    float _lastLux = 0;
    int _lastMoisture = 0;
    float _lastTemp = 0;
    float _lastHum = 0;
    float _lastPress = 0;
    String _lastTime = "--:--";
    String _lastStatus = "Init";

    void setupAP();
    String getHtmlPage(); // Funkcja podmieniająca dane w HTML

public:
    CloudClient(const char* ssid, const char* pass);
    
    void connect(bool useWifi = true); // Próba połączenia
    void handleClient(); // Obsługa żądań WWW
    
    void sendData(float lux, int moisture, float temp, float humidity, float pressure, String message, String time);
    
    bool isAPMode(); // Getter dla managera
    
};


// #pragma once
// #include <WiFi.h>
// #include <HTTPClient.h>

// class CloudClient {
// private:
//     const char* _ssid ="Dominika";
//     const char* _pass ="5Oi920;4";
//     // TU WPISZ SWÓJ ADRES IP (np. 192.168.0.15) i PORT SERWERA C# (zazwyczaj 5000 lub 5001)
//     // UWAGA: Upewnij się, czy w C# masz http (5000) czy https (5001). ESP32 łatwiej łączy się po HTTP.
//     String _serverUrl = "http://192.168.137.1:5202/api/plant";
    
// public:
//     CloudClient(const char* ssid, const char* pass) {
//         _ssid = ssid;
//         _pass = pass;
//     }

//     bool connect() {
//         if (WiFi.status() == WL_CONNECTED) 
//             return 1;

//         Serial.print("[WiFi] Laczenie z: ");
//         Serial.println(_ssid);
        
//         WiFi.begin(_ssid, _pass);
        
//         int timeout = 0;
//         while (WiFi.status() != WL_CONNECTED && timeout < 20) { // 10 sekund
//             delay(500);
//             Serial.print(".");
//             timeout++;
//         }
        
//         if (WiFi.status() == WL_CONNECTED) {
//             Serial.println("\n[WiFi] Polaczone!");
//             Serial.print("IP: ");
//             Serial.println(WiFi.localIP());
//         } else {
//             Serial.println("\n[WiFi] Blad polaczenia.");
//             return 0;
//         }

//         return 1;
//     }

//     void sendData(float lux, int moisture, float temp, float humidity, float pressure, String message) {
//         if (WiFi.status() != WL_CONNECTED) {
//             connect(); 
//         }
        
//         if (WiFi.status() != WL_CONNECTED) return;

//         HTTPClient http;
//         http.begin(_serverUrl);
//         http.addHeader("Content-Type", "application/json");

//         // BUDOWANIE ROZSZERZONEGO JSONA
//         String json = "{";
//         json += "\"moisture\":" + String(moisture) + ",";
//         json += "\"lux\":" + String(lux) + ",";
//         json += "\"temperature\":" + String(temp) + ",";
//         json += "\"humidity\":" + String(humidity) + ","; // Nowe
//         json += "\"pressure\":" + String(pressure) + ",";      // Nowe
//         json += "\"status\":\"" + message + "\""; 
//         json += "}";

//         Serial.println("[HTTP] Wysylam JSON: " + json);
        
//         int httpCode = http.POST(json);
        
//         if (httpCode > 0) {
//             Serial.printf("[HTTP] Sukces: %d\n", httpCode);
//         } else {
//             Serial.printf("[HTTP] Blad: %s\n", http.errorToString(httpCode).c_str());
//         }
        
//         http.end();
//     }
// };