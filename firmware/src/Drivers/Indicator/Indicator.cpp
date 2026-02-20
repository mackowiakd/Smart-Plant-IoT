#include "Indicator.h"

Indicator::Indicator(int ledPin, int buttonPin) {
    _ledPin = ledPin;
    _buttonPin = buttonPin;
    _pixels = new Adafruit_NeoPixel(NUM_LEDS, _ledPin, NEO_GRB + NEO_KHZ800);
}

void Indicator::begin() {
    _pixels->begin();
    delay(100);
    _pixels->setBrightness(75);
    _pixels->show();
    delay(100); 
    pinMode(_buttonPin, INPUT_PULLUP);
}

void Indicator::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _pixels->setPixelColor(0, _pixels->Color(r, g, b));
    _pixels->show();
}

void Indicator::off() {
    _pixels->clear();
    _pixels->show();
}

// --- LOGIKA KLIKNIĘĆ ---
ButtonEvent Indicator::checkButton() {
    bool currentState = digitalRead(_buttonPin); // LOW = wciśnięty
    unsigned long now = millis();
    ButtonEvent event = NONE;

    // 1. Wykrycie wciśnięcia (opadające zbocze)
    if (_btnStateLast == HIGH && currentState == LOW) {
        _btnPressTime = now;
    }

    // 2. Wykrycie puszczenia (narastające zbocze)
    if (_btnStateLast == LOW && currentState == HIGH) {
        unsigned long duration = now - _btnPressTime;

        if (duration > 3000) { // Przytrzymanie > 3s
            event = LONG_PRESS;
            _clickCount = 0;
        } 
        else if (duration > 50) { // Krótkie kliknięcie (>50ms to filtr drgań)
            _clickCount++;
            _lastClickTime = now;
        }
    }

    // 3. Sprawdzenie czy to koniec klikania (minęło 400ms od ostatniego kliku)
    if (_clickCount > 0 && (now - _lastClickTime > 400)) {
        if (_clickCount == 2) event = DOUBLE_CLICK;
        else if (_clickCount == 1) event = SINGLE_CLICK;
        
        _clickCount = 0; // Reset
    }

    _btnStateLast = currentState;
    return event;
}