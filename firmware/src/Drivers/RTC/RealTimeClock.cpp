#include "RealTimeClock.h"

// --- ZMIENNA W PAMIĘCI RTC (Niezniszczalna przez Deep Sleep) ---
// Jest poza klasą (globalna), ale używana tylko tutaj.
RTC_DATA_ATTR int _persistentLastWateredHour = -1;

RealTimeClock::RealTimeClock() {
    // Konstruktor pusty, inicjalizacja w begin()
}
bool RealTimeClock::begin() {
    if (!_rtc.begin(&Wire)) return false;
   // --- BEZWARUNKOWA SYNCHRONIZACJA (jeśli jest WiFi) ---
    Serial.println("[RTC] Proba synchronizacji czasu z Internetem...");

    // 1. Konfiguracja (3600 = UTC+1 dla Polski zimą)
    // Jeśli chcesz czas letni, można dać 7200 lub użyć bardziej zaawansowanej strefy
    configTime(3600, 3600, "pool.ntp.org"); 
    
    struct tm timeinfo;
    
    // 2. Próbujemy pobrać czas (czekamy max 4 sekundy)
    // Funkcja getLocalTime zwróci true tylko jeśli WiFi działa i serwer odpowiedział
    if (getLocalTime(&timeinfo, 4000)) {
        Serial.println("[RTC] SUKCES! Pobrano czas z sieci NTP.");
        
        // Nadpisujemy RTC (DS3231) idealnym czasem z sieci
        _rtc.adjust(DateTime(
            timeinfo.tm_year + 1900, 
            timeinfo.tm_mon + 1, 
            timeinfo.tm_mday,
            timeinfo.tm_hour, 
            timeinfo.tm_min, 
            timeinfo.tm_sec
        ));
    } else {
        Serial.println("[RTC] Nie udalo sie pobrac czasu (brak WiFi lub timeout).");
        Serial.println("      -> Dzialam na wewnetrznej baterii RTC.");
        
        // Tylko jeśli bateria padła całkowicie, ustawiamy datę kompilacji
        if (_rtc.lostPower()) {
             Serial.println("[RTC] AWARIA: Bateria pusta! Reset do czasu kompilacji.");
             _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
    }
    
    // Diagnostyka: Pokaż, która jest godzina po operacji
    Serial.print("[RTC] Aktualny czas systemowy: ");
    Serial.println(getTimeString());

    return true;
}
/*bool RealTimeClock::begin() {
    if (!_rtc.begin()) {
        return false; // Nie znaleziono układu
    }
     _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
     // Jeśli wyjęto baterię, resetujemy też pamięć podlewania!
        _persistentLastWateredHour = -1;

    // Sprawdź, czy zegar nie zgubił prądu
    if (_rtc.lostPower()) {
        Serial.println("RTC: Zegar stracil zasilanie! Ustawiam czas kompilacji...");
        // Magiczna linijka: ustawia datę i godzinę z momentu wgrania kodu na płytkę
       
    }

    return true;
}
*/

int RealTimeClock::getHour() {
    DateTime now = _rtc.now();
    return now.hour();
}

int RealTimeClock::getMinute() {
    DateTime now = _rtc.now();
    return now.minute();
}

String RealTimeClock::getTimeString() {
    DateTime now = _rtc.now();
    // Formatowanie HH:MM:SS
    sprintf(_buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    return String(_buffer);
}

float RealTimeClock::getTemperature() {
    return _rtc.getTemperature();
}

int RealTimeClock::getLastWateredHour() {
    return _persistentLastWateredHour;
}

void RealTimeClock::setLastWateredHour(int hour) {
    _persistentLastWateredHour = hour;
}