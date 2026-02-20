#include "CloudClient.h"
#include "WebPage.h" // Tu importujemy nasz czysty HTML

CloudClient::CloudClient(const char* ssid, const char* pass) {
    _ssid = ssid;
    _pass = pass;
    _server = new WebServer(80);
    _dnsServer = new DNSServer(); 
}

// ZMIANA NAGŁÓWKA I LOGIKI
void CloudClient::connect(bool useWiFi) {
    
    // 1. Jeśli kazaliśmy NIE używać WiFi (force AP)
    if (!useWiFi) {
        Serial.println("[Network] WYMUSZONO tryb AP (pomijam szukanie WiFi).");
        setupAP();
        return; // Kończymy, nie dotykamy WiFi.begin()
    }

    // 2. Standardowa logika (jeśli useWiFi == true)
    Serial.print("[Network] Laczenie z WiFi: ");
    Serial.println(_ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 15) { 
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[Network] Polaczono! IP: " + WiFi.localIP().toString());
        _isAPMode = false;
    } else {
        Serial.println("\n[Network] BLAD. Przechodze w tryb AP.");
        setupAP();
    }
}

void CloudClient::setupAP() {
    _isAPMode = true;
    
    Serial.println("[AP] Czyszczenie konfiguracji WiFi...");

    // --- KROK 1: BRUTALNE ROZŁĄCZENIE ---
    // true = wyłącz radio WiFi i wyczyść zapisany SSID z pamięci
    WiFi.disconnect(true); 
    delay(100); 

    // --- KROK 2: TYLKO TRYB AP ---
    WiFi.mode(WIFI_AP);
    
    // --- KROK 3: WYŁĄCZ OSZCZĘDZANIE ENERGII (Kluczowe dla iPhone!) ---
    WiFi.setSleep(false); 

    // --- KROK 4: KONFIGURACJA SIECI ---
    // Używamy: Nazwa, Hasło, Kanał (1), Ukryty (0), Max połączeń (1)
    // Ustawienie Kanału 1 pomaga uniknąć skakania po częstotliwościach
    WiFi.softAP("Roslina_SOS", "12345678", 1, 0, 1); 

    delay(200); // Chwila na stabilizację

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("[AP] Siec utworzona. IP: ");
    Serial.println(myIP);

    // --- KROK 5: START USŁUG ---
    _dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    _dnsServer->start(53, "*", myIP);

    _server->on("/", [this]() {
        _server->send(200, "text/html", getHtmlPage());
    });
    
    _server->onNotFound([this]() {
        _server->send(200, "text/html", getHtmlPage());
    });
    
    _server->begin();
}

String CloudClient::getHtmlPage() {
    // Kopia szablonu z PROGMEM do Stringa
    String p = FPSTR(INDEX_HTML);
    
    // Podmiana placeholderów na wartości
    p.replace("%MOISTURE%", String(_lastMoisture));
    p.replace("%TEMP%", String(_lastTemp));
    p.replace("%HUM%", String(_lastHum, 1));   // np. 45.2 %
    p.replace("%PRESS%", String(_lastPress, 0));
    p.replace("%LUX%", String(_lastLux));
    p.replace("%STATUS%", _lastStatus);
    p.replace("%TIME%", _lastTime);
    
    return p;
}

void CloudClient::handleClient() {
    if (_isAPMode) {
        _dnsServer->processNextRequest();
        _server->handleClient();
    }
}

void CloudClient::sendData(float lux, int moisture, float temp, float humidity, float pressure, String message, String time) {
    // Zapisz dane lokalnie dla strony WWW
    _lastLux = lux;
    _lastMoisture = moisture;
    _lastTemp = temp;
    _lastHum = humidity;
    _lastPress = pressure;
    _lastStatus = message;
    _lastTime = time;
    
    delay(100);
    if (_isAPMode) return; // Jeśli AP, nie wysyłamy HTTP

   if (WiFi.status() != WL_CONNECTED) {
            connect(); 
        }
        
        if (WiFi.status() != WL_CONNECTED) return;

        HTTPClient http;
        http.begin(_serverUrl);
        http.addHeader("Content-Type", "application/json");

        // BUDOWANIE ROZSZERZONEGO JSONA
        String json = "{";
        json += "\"moisture\":" + String(moisture) + ",";
        json += "\"lux\":" + String(lux) + ",";
        json += "\"temperature\":" + String(temp) + ",";
        json += "\"humidity\":" + String(humidity) + ","; // Nowe
        json += "\"pressure\":" + String(pressure) + ",";      // Nowe
        json += "\"status\":\"" + message + "\""; 
        json += "}";

        Serial.println("[HTTP] Wysylam JSON: " + json);
        
        int httpCode = http.POST(json);
        
        if (httpCode > 0) {
            Serial.printf("[HTTP] Sukces: %d\n", httpCode);
        } else {
            Serial.printf("[HTTP] Blad: %s\n", http.errorToString(httpCode).c_str());
        }
        
        http.end();
}

bool CloudClient::isAPMode() {
    return _isAPMode;
}

