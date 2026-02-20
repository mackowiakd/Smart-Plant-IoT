
#include "LightSensor.h"

LightSensor::LightSensor(uint8_t addr, int sda, int scl) {
    _address = addr;
    // Ignorujemy sda/scl tutaj, bo Wire jest inicjowane globalnie w PlantManager
}

void LightSensor::sendCommand(byte cmd) {
    Wire.beginTransmission(_address);
    Wire.write(cmd);
    Wire.endTransmission();
}

bool LightSensor::begin() {
    // UWAGA: Nie robimy tutaj Wire.begin() ani Wire.end()!
    // Zakładamy, że PlantManager już uruchomił magistralę.
    
    // Tylko konfigurujemy czujnik (Twój kod):
    sendCommand(CMD_POWER_ON);
    delay(10); // Krótka przerwa dla stabilności
    sendCommand(CMD_MODE_H_RES);
    
    return true; 
}

float LightSensor::readLux() {
    // Pytamy o 2 bajty (Twój kod z pętli loop)
    int count = Wire.requestFrom((int)_address, 2);

    if (count == 2) {
        byte b1 = Wire.read();
        byte b2 = Wire.read();
        
        uint16_t val = ((b1 << 8) | b2);
        return val / 1.2; // Twoje przeliczenie
    } else {
        // Jeśli błąd, próbujemy go "kopnąć" ponownie (z Twojego kodu)
        // Serial.println("BH1750: Brak danych, restartuje pomiar...");
        sendCommand(CMD_POWER_ON);
        sendCommand(CMD_MODE_H_RES);
        return -1.0; // Kod błędu
    }
}
/*
#include "LightSensor.h"

LightSensor::LightSensor(byte address, int sda, int scl) {
    _address = address;
    _sdaPin = sda;
    _sclPin = scl;
}




void LightSensor::sendCommand(byte cmd) {
    Wire.beginTransmission(_address);
    Wire.write(cmd);
    Wire.endTransmission();
}

bool LightSensor::begin() {
    // Twoja sprawdzona sekwencja startowa
    Wire.end();
    Wire.setPins(_sdaPin, _sclPin);
    
    if (!Wire.begin()) {
        return false; // Błąd I2C
    }

    // Włączenie czujnika
    sendCommand(CMD_POWER_ON);
    sendCommand(CMD_MODE_H_RES);
    
    return true;
}





*/