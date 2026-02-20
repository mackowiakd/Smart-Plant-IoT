
#pragma once
#include <Arduino.h>
#include <Wire.h>

class LightSensor {
private:
    uint8_t _address;
    
    // Stałe dla BH1750 (z Twojego działającego kodu)
    const byte CMD_POWER_ON = 0x01;
    const byte CMD_MODE_H_RES = 0x10;

    // Metoda pomocnicza do wysyłania komend
    void sendCommand(byte cmd);

public:
    // Konstruktor
    LightSensor(uint8_t addr, int sda, int scl);

    // Inicjalizacja (Wysyła tylko komendy startowe, nie resetuje Wire!)
    bool begin();

    // Odczyt (Twoja matematyka / 1.2)
    float readLux();
};

/*
#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
#pragma once
#include <Arduino.h>
#include <Wire.h>


class LightSensor {
private:
    byte _address;
    int _sdaPin;
    int _sclPin;
    
    // Stałe dla BH1750 (ukryte w klasie)
    const byte CMD_POWER_ON = 0x01;
    const byte CMD_RESET = 0x07;
    const byte CMD_MODE_H_RES = 0x10;

    void sendCommand(byte cmd);

public:
    // Konstruktor: przyjmuje adres i piny
    LightSensor(byte address, int sda, int scl);

    // Inicjalizacja magistrali i czujnika
    bool begin();

    // Odczyt wartości (zwraca -1.0 w przypadku błędu)
    float readLux();
};

#endif
*/