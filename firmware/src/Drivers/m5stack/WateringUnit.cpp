#include "WateringUnit.h"

// Kalibracja (ustalimy to testem!)
// Wartość "Na Sucho" (w powietrzu)
const int AIR_VALUE = 2550;   
// Wartość "W Wodzie"
const int WATER_VALUE = 2130; 

WateringUnit::WateringUnit(int pumpPin, int sensorPin) {
    _pumpPin = pumpPin;
    _sensorPin = sensorPin;
}

void WateringUnit::begin() {
    // Pompka to WYJŚCIE
    pinMode(_pumpPin, OUTPUT);
    digitalWrite(_pumpPin, LOW); // Wyłącz na start
    
    // Czujnik to WEJŚCIE
    pinMode(_sensorPin, INPUT);
}

int WateringUnit::readRaw() {
    return analogRead(_sensorPin);
}

int WateringUnit::readPercent() {
    int val = analogRead(_sensorPin);
    // Mapowanie odwrotne (im wyższa wartość, tym bardziej sucho)
    int percent = map(val, AIR_VALUE, WATER_VALUE, 0, 100);
    return constrain(percent, 0, 100);
}

void WateringUnit::setPump(bool state) {
    if (state) {
        digitalWrite(_pumpPin, HIGH); // Włącz
    } else {
        digitalWrite(_pumpPin, LOW);  // Wyłącz
    }
}