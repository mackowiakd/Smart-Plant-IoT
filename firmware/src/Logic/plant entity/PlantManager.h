#pragma once
#include <Arduino.h>
//#include "../../Drivers/BH1750/LightSensor.h"
#include "../../Drivers/m5stack/WateringUnit.h"
#include "../../Drivers/Indicator/Indicator.h"
#include "../../Drivers/RTC/RealTimeClock.h" 
#include "../../Network/CloudClient.h"
#include <Adafruit_Sensor.h>  // BME
#include <Adafruit_BME280.h>  // Główna biblioteka BME

struct PlantData {
    float lux = 0;
    int moisture = 0;
    float temperature = 0; // Z RTC -> BME teraz
    float humidity = 0;    //  Wilgotność powietrza (z BME)
    float pressure = 0;    //  Ciśnienie (z BME)
    bool watered = false;
    String status = "OK";
};

class PlantManager {
private:
    
    Indicator* _ui;
    WateringUnit* _water;
    RealTimeClock* _rtc; 
    Adafruit_BME280 _bme;
    CloudClient* _cloud;
    unsigned long _lastMeasurementTime;
    PlantData _currentData;
    const int DRY_THRESHOLD = 30;

    void measureAll();
    void evaluateLogic();
    void sendReport();
    float readLuxDirectly();
public:
    // Konstruktor przyjmuje wszystko
    PlantManager( Indicator* i, WateringUnit* w, RealTimeClock* r);
    

    void begin(bool useWifi=true);
    void runAutoCycle();  
    void runManualMode(); 
    void sleep(int minutes); 
    void handleUI();
    void updateNetwork();
    void handleAPVisuals();
    bool isAPmode(){
       return  _cloud->isAPMode();
    };

    unsigned long get_lastMeasure(){
        return _lastMeasurementTime;
    }
    void set_lastMeasure(unsigned long time){
        _lastMeasurementTime=time;
    }
};