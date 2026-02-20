#pragma once
#include <Arduino.h>

class WateringUnit {
private:
    int _pumpPin;
    int _sensorPin;
    
public:
    // Konstruktor przyjmuje numery pinów
    WateringUnit(int pumpPin, int sensorPin);

    void begin();
    
    // Odczyt surowy (0-4095)
    int readRaw();
    
    // Odczyt procentowy (0-100%) - wymaga kalibracji!
    int readPercent();
    
    // Sterowanie pompką
    void setPump(bool state);
};