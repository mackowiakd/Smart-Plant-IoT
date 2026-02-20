#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h> // Biblioteka Adafruit

class RealTimeClock {
private:
    RTC_DS3231 _rtc; // Obiekt biblioteki
    char _buffer[10]; // Bufor pomocniczy do napisów

public:
    RealTimeClock();

    bool begin();
    
    // Metody do odczytu czasu
    int getHour();
    int getMinute();
    
    // Zwraca ładny napis np. "14:30:05"
    String getTimeString();
    
    // Zwraca temperaturę (DS3231 ma termometr w gratisie!)
    float getTemperature();
    
    int getLastWateredHour();          // Odczytaj
    void setLastWateredHour(int hour); // Zapisz
};