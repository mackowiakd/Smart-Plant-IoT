#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Typy zdarzeń przycisku
enum ButtonEvent {
    NONE = 0,
    SINGLE_CLICK,
    DOUBLE_CLICK,
    LONG_PRESS
};

class Indicator {
private:
    int _ledPin;
    int _buttonPin;
    Adafruit_NeoPixel* _pixels; 
    const int NUM_LEDS = 1; 

    // Zmienne do liczenia czasu przycisku
    unsigned long _btnPressTime = 0;
    bool _btnStateLast = true; 
    int _clickCount = 0;
    unsigned long _lastClickTime = 0;

public:
    Indicator(int ledPin, int buttonPin); // Konstruktor poprawiony (bez numPixels, bo stała 1)
    void begin();
    
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void off();
    
    // Ta funkcja robi całą magię
    ButtonEvent checkButton();
};