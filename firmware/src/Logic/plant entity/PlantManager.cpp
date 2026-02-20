#include "PlantManager.h"

PlantManager::PlantManager(  Indicator* i, WateringUnit* w, RealTimeClock* r) {
    //_light = l;
    _ui = i;
    _water = w;
    _rtc = r;
    _cloud = new CloudClient("Dominika", "5Oi920;4");
    _lastMeasurementTime = 0;
}



// ZMIANA: Dodajemy parametr do funkcji
void PlantManager::begin(bool useWiFi) {
  _ui->begin();
    
    // --- SEKWENCJA ROZGRZEWKOWA (BOOT ANIMATION) ---
    // To "obudzi" diodę i da Ci pewność, że działa, zanim ruszy WiFi.
    Serial.println("[System] Test LED...");
    
    _ui->setColor(252, 3, 194); delay(300); // rozwy
    _ui->setColor(252, 3, 194); delay(300); // fioletowy
    _ui->setColor(0, 0, 255); delay(300); // Niebieski
    
    // Finał: BIAŁY (Sygnał: "System startuje, myślę...")
    _ui->setColor(255, 255, 255); 
    // -----------------------------------------------

    if (_bme.begin(0x76, &Wire)) {
        Serial.println("[System] BME280 OK");
    } else {
        Serial.println("[System] BLAD: Nie wykryto BME280!");
    }

    // ZMIANA: Przekazujemy parametr 'useWiFi' do CloudClienta
    Serial.println("[System] Inicjalizacja Sieci...");
    _cloud->connect(useWiFi); 
   

   
    _water->begin();
    _rtc->begin(); 
}

void PlantManager::runAutoCycle() {
    measureAll();
    evaluateLogic();
    sendReport();
}

void PlantManager::measureAll() {
    Serial.println("[STAN] Pomiary...");
    
   // 1. Światło (Twoja metoda bezpośrednia)
    _currentData.lux = readLuxDirectly(); 
    
    // 2. Wilgotność Gleby (z czujnika pojemnościowego)
    _currentData.moisture = _water->readPercent(); 
    
    // 3. Klimat (z BME280)
    float t = _bme.readTemperature();
    float h = _bme.readHumidity();
    float p = _bme.readPressure();

    // ZABEZPIECZENIE PRZED "NAN" (Not a Number)
    // Jeśli czujnik nie działa, wpisz 0, żeby nie psuć JSON-a
    if (isnan(t)) t = 0.0;
    if (isnan(h)) h = 0.0;
    if (isnan(p)) p = 0.0; // Pamiętaj, że readPressure zwraca w Pa, a nie hPa
    else p = p / 100.0F;   // Przeliczamy na hPa

    _currentData.temperature = t;
    _currentData.humidity = h;
    _currentData.pressure = p;

    // Debug w konsoli
    Serial.printf("Sensory -> T: %.1fC | H_Air: %.1f%% | H_Soil: %d%% | L: %.0flx\n", 
                  _currentData.temperature, _currentData.humidity, 
                  _currentData.moisture, _currentData.lux);

};



// --- INDYWIDUALNA FUNKCJA ZARZĄDZANIA ŚWIATŁEM ---
float PlantManager::readLuxDirectly() {
    byte addresses[] = {0x5C, 0x23}; // Lista podejrzanych adresów (Twoje testy wskazały 0x5C!)
    
    for (byte addr : addresses) {
        // 1. Sprawdź czy czujnik żyje (Ping)
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            // Serial.printf("[I2C] Znaleziono BH1750 pod adresem 0x%X\n", addr);
            
            // 2. Obudź go i każ mierzyć (CMD: 0x01 PowerOn, 0x10 H-Res)
            Wire.beginTransmission(addr);
            Wire.write(0x01); // Power On
            Wire.endTransmission();
            
            Wire.beginTransmission(addr);
            Wire.write(0x10); // Mode H-Res
            Wire.endTransmission();
            
            // 3. Pobierz wynik
            int count = Wire.requestFrom((int)addr, 2);
            if (count == 2) {
                byte b1 = Wire.read();
                byte b2 = Wire.read();
                float val = ((b1 << 8) | b2) / 1.2;
                return val; // Sukces! Zwracamy wynik i wychodzimy.
            }
        }
    }
    
    Serial.println("[I2C] BLAD: Nie znaleziono czujnika swiatla (0x5C ani 0x23)!");
    return -1.0; // Porażka
}



void PlantManager::evaluateLogic() {
    // 1. Pobierz dane
    int currentHour = _rtc->getHour();
    int lastWatered = _rtc->getLastWateredHour();
    
    float temp = _currentData.temperature;
    float humAir = _currentData.humidity;
    float light = _currentData.lux;
    int soil = _currentData.moisture;

    Serial.println("\n--- ANALIZA WIELOKRYTERIALNA ---");

    // Reset statusu na czysty
    _currentData.status = "OK"; 
    
    bool requestWatering = false;
    bool criticalStatusSet = false; // Flaga: czy mamy ważny problem?

    // --- LOGIKA 1: BEZPIECZEŃSTWO TERMICZNE ---
    bool isTooCold = (temp < 10.0);
    // Jeśli zimno, blokujemy wszystko inne
    if (isTooCold) {
        _currentData.status = "CRITICAL: TOO COLD!";
        criticalStatusSet = true;
    }

    // --- LOGIKA 2: WILGOTNOŚĆ GLEBY (GŁÓWNA) ---
    // Decyzję podejmujemy tylko, jeśli nie jest za zimno
    if (!isTooCold && soil < DRY_THRESHOLD) {
        
        bool isNight = (currentHour >= 22 || currentHour < 6);
        bool justWatered = (lastWatered == currentHour);

        if (isNight) {
            _currentData.status = "DRY (Waiting: Night)";
            Serial.println(" -> Decyzja: SUCHO, ale NOC. Czekam.");
            criticalStatusSet = true; 
        } 
        else if (justWatered) {
            _currentData.status = "DRY (Waiting: Cooldown)";
            Serial.println(" -> Decyzja: SUCHO, ale niedawno podlano.");
            criticalStatusSet = true;
        } 
        else {
            requestWatering = true; // Warunki spełnione -> Podlewamy
        }
    }

    // --- WYKONANIE AKCJI (PODLEWANIE) ---
    if (requestWatering) {
        Serial.println(" -> Decyzja: PODLEWAM.");
        
        // Logika Heat Stress (dłuższe podlewanie w upały)
        bool isHeatStress = (temp > 28.0 && light > 5000);
        int duration = isHeatStress ? 4000 : 2000;
        
        if (isHeatStress) {
             _currentData.status = "WATERING (Heavy)";
             Serial.println(" -> HEAT STRESS: Zwiekszona dawka.");
        } else {
             _currentData.status = "WATERING";
        }

        _ui->setColor(0, 0, 255); 
        _water->setPump(true);
        delay(duration);
        _water->setPump(false);
        _ui->off();
        
        _currentData.watered = true;
        _rtc->setLastWateredHour(currentHour);
        
        criticalStatusSet = true; // Podlewanie to stan krytyczny/akcji
    } 

    // --- LOGIKA 3: DORADCA (DODATKOWA) ---
    // Porady dodajemy TYLKO, jeśli nie ma statusu krytycznego (np. nie podlewamy teraz)
    // LUB doklejamy je jako info dodatkowe.
    
    if (!criticalStatusSet) {
        // Jeśli wszystko inne jest OK, sprawdzamy "drobne" rzeczy
        if (humAir < 40.0) {
            _currentData.status = "ADVICE: Mist plant, air to dry"; // Suche powietrze
        }
        else if (light < 50 && currentHour > 10 && currentHour < 16) {
             _currentData.status = "ADVICE: Low light";
        }
    } 
    else {
        // Jeśli mamy status krytyczny (np. "DRY (Waiting: Night)"), 
        // to nie nadpisujemy go radą, ale możemy dodać w logach
        if (humAir < 40.0) {
            Serial.println(" -> (Info boczne: Powietrze tez jest suche)");
        }
    }
    
    Serial.printf("Status koncowy: %s\n", _currentData.status.c_str());
}

void PlantManager::sendReport() {
    String currentTime = _rtc->getTimeString();
    Serial.println("---------------- RAPORT ----------------");
    Serial.printf("Czas: %s | Status: %s\n", _rtc->getTimeString().c_str(), _currentData.status.c_str());
    
    //_cloud->connect(Wifi);

     // Jeśli po connect() nie jesteśmy w AP, to znaczy że WiFi się udało
   
    // Używamy nowej metody z 6 argumentami (wiadomość tekstowa na końcu)
    _cloud->sendData(
        _currentData.lux, 
        _currentData.moisture, 
        _currentData.temperature,
        _currentData.humidity,
        _currentData.pressure,
        _currentData.status, // <-- PRZESYŁAMY STATUS/PORADĘ
        currentTime
    );

     if (!_cloud->isAPMode()) {
        // SUKCES -> ZIELONY (na 1 sekundę)
        _ui->setColor(0, 255, 0);
        delay(1000);
        _ui->off(); // Gasimy, żeby oszczędzać prąd
    }
}



// --- NOWY TRYB PREZENTACYJNY (MANUAL) ---
void PlantManager::runManualMode() {
   
    // 1. Ustawiamy status MANUAL
    _currentData.watered = true;
    _currentData.status = "MANUAL WATERING"; 
    
    // KROK KRYTYCZNY: Wysyłamy to do C# NATYCHMIAST
    // Zanim zaczniemy mrugać i blokować czas, wysyłamy info w świat.
    sendReport();
    // 2. Niebieski start (Sygnał: Jestem gotowy)
    _ui->setColor(0, 0, 255); 
    
    // Bezpieczne czekanie 500ms
    unsigned long timer = millis();
    while(millis() - timer < 500) { 
        if(isAPmode()) _cloud->handleClient(); 
        delay(5); 
    }

    // 3. Pompa START
    _water->setPump(true);
    
    // Rozruch pompy (100ms)
    timer = millis();
    while(millis() - timer < 100) { delay(5); } // Tu krótki delay jest ok

    // Mruganie diodą podczas podlewania (żeby było widać akcję)
    for(int i=0; i<3; i++) {
        _ui->setColor(0, 0, 255); // Niebieski
        delay(250);
        _ui->setColor(0, 0, 0);   // Zgaś
        delay(250);
    }
    
    // 3. Stop
    _water->setPump(false);
    Serial.println("[DEMO] Pompka ON (Demonstracja done)");
   

    //4. powiadomienie o maulanym podlewaniu - automat nie włączy się ponownie w tej samej godzinie.
    int currentHour = _rtc->getHour();
    _rtc->setLastWateredHour(currentHour);
    
    
    Serial.printf("[Logic] Zaktualizowano czas ostatniego podlewania na godzine: %d\n", currentHour);
  
  
    sendReport(); 
    // -------------------------

    // 6. Sygnał końca (RÓŻOWY) - Potwierdzenie dla oka
    _ui->setColor(215, 3, 252);

   unsigned long startFreeze = millis();
    while (millis() - startFreeze < 15000) { 
        // To jest kluczowe: Pozwalamy serwerowi WWW działać
        _cloud->handleClient(); 
        delay(10);
    }
    _ui->off();
    // --- DODAJ TO TUTAJ ---
    // Resetujemy zegar pomiarów automatycznych. 
    // Dzięki temu po powrocie do loop() automat odczeka pełne 10s 
    // zanim nadpisze Twój piękny status "MANUAL WATERING".
    
    _lastMeasurementTime = millis(); // Użyj nazwy zmiennej, której używasz w loop() do odliczania czasu
}

void PlantManager::sleep(int minutes) {
    Serial.printf("Ide spac na %d minut... Zzz...\n", minutes);
    Serial.flush();
    unsigned long now = millis();
        if ((now / 500) % 2 == 0) {
            _ui->setColor(252, 198, 3); // Zolty
        } else {
            _ui->off();
        }
        
    // 1. Wyłącz wszystko przed snem
    _water->setPump(false);
    _ui->off(); 

    // 2. Budzik Czasowy (Timer)
    esp_sleep_enable_timer_wakeup(minutes * 60 * 1000000ULL);
    
    // 3. Budzik Przyciskiem (GPIO 4 / D2, stan LOW)
    esp_deep_sleep_enable_gpio_wakeup(1ULL << 4, ESP_GPIO_WAKEUP_GPIO_LOW); 

    // 4. Dobranoc
    esp_deep_sleep_start();
}

// Przekazujemy sterowanie do CloudClienta (żeby strona WWW działała)
void PlantManager::updateNetwork() {
    _cloud->handleClient();
}

// Obsługa interfejsu (Co robić jak klikniesz)
void PlantManager::handleUI() {
    ButtonEvent event = _ui->checkButton(); // Pytamy Indicator co się stało

    switch (event) {
        case DOUBLE_CLICK:
            Serial.println("[UI] Dwa kliki -> Podlewanie Manualne!");
            runManualMode(); 
            break;

        // Wewnątrz void PlantManager::handleUI()

        case LONG_PRESS:
            Serial.println("[UI] Long Press -> ZMIANA TRYBU SIECI (Soft Switch)");

            // 1. Sygnalizacja świetlna (FIOLETOWY - Zmiana)
            // Nie używamy restartu, więc dioda po prostu pomruga i wróci do pracy
            for(int i=0; i<3; i++) {
                _ui->setColor(252, 3, 252); delay(200); 
                _ui->off(); delay(200);
            }

            // 2. LOGIKA PRZEŁĄCZANIA (BEZ RESTARTOWANIA CPU)
            if (isAPmode()) {
                // --- Jesteśmy w AP -> Przechodzimy na WiFi ---
                Serial.println("[Switch] Wyłączam AP, próbuję łączyć z WiFi...");
                
                // Ważne: Sprzątamy po AP
                WiFi.softAPdisconnect(true); 
                delay(100);
                
                // Łączymy z WiFi (true = useWiFi)
                _cloud->connect(true); 
                
                // Jeśli się udało -> Dioda na zielono
                if (!_cloud->isAPMode()) {
                    _ui->setColor(0, 255, 0); delay(1000); _ui->off();
                } else {
                    // Jak się nie udało połączyć z WiFi, connect() sam przywróci AP
                    // więc nic nie musimy robić, najwyżej wróci do czerwonego mrugania
                }

            } else {
                // --- Jesteśmy w WiFi -> Przechodzimy na AP ---
                Serial.println("[Switch] Rozłączam WiFi, stawiam AP...");
                
                // Ważne: Sprzątamy po WiFi
                WiFi.disconnect(true);
                delay(100);
                
                // Stawiamy AP (false = useWiFi)
                _cloud->connect(false);
                
                // Dioda zacznie mrugać na czerwono w pętli loop() dzięki handleAPVisuals
            }
            break;
        }
}

void PlantManager::handleAPVisuals() {
    if (isAPmode()) {
        // Mruganie: 1 sekunda cyklu (500ms ON, 500ms OFF)
        unsigned long now = millis();
        if ((now / 500) % 2 == 0) {
            _ui->setColor(255, 0, 0); // CZERWONY
        } else {
            _ui->off();
        }
    }
}