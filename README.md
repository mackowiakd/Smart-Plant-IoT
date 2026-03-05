> 🎓 **Academic Project**: This project was developed for the *Microprocessor and Embedded Systems* course during the 5th semester of Computer Science at the **Silesian University of Technology** (Politechnika Śląska).

# Smart Plant Care IoT System 🌱

An end-to-end Internet of Things (IoT) solution for monitoring environmental parameters and autonomously managing plant hydration. The project spans custom hardware design (PCB), edge device firmware (ESP32-C3), and a full-stack web dashboard.

## 🌟 Key Features
* **Smart Autonomous Watering:** Activates a water pump based on custom-calibrated soil moisture data (`AIR_VALUE` & `WATER_VALUE`). The logic includes safety checks, such as preventing watering during low temperatures to protect the plant roots.
* **Extreme Power Optimization:** Heavily utilizes the ESP32 **Deep Sleep** mode. The system wakes up periodically, performs measurements, communicates with the server, and goes back to sleep, allowing for long-term operation on a standard 5V powerbank.
* **Fail-Safe Offline Mode (Fallback AP):** If the configured WiFi network is lost, the device automatically spawns its own Access Point (e.g., "Roslina SOS"). It serves a lightweight local HTML page (`WebPage.h`) directly to a smartphone, allowing the user to view telemetry without internet access.
* **Hybrid Timekeeping:** Combines internet time synchronization (NTP) with a battery-backed hardware Real-Time Clock (DS3231) to maintain time context and scheduling during power losses or offline periods.
* **Full-Stack SPA Architecture:** Telemetry is pushed via HTTP to a custom .NET backend, which seamlessly serves a modern React-based user dashboard.

## 🛠️ System Architecture & Tech Stack

### 1. Edge Device (Firmware & Hardware)
* **Microcontroller:** Seeed Studio XIAO ESP32-C3.
* **Sensors & Actuators:** * **BME280** (Temperature, Humidity, Atmospheric Pressure) via I2C.
  * **BH1750** (Light intensity/Lux) via I2C.
  * **DS3231** (Hardware RTC) via I2C.
  * **Capacitive Soil Moisture Sensor v1.2** via ADC.
  * 5V Water Pump controlled via MOSFET/Relay.
* **Firmware:** C++, PlatformIO, custom sleep/wake routines.

### 2. Web Application (Backend & Frontend)
* **Architecture:** Single Page Application (SPA) integration.
* **Backend:** C#, ASP.NET Core REST API for telemetry data ingestion and aggregation.
* **Frontend:** React.js, Vite, and Tailwind CSS for responsive data visualization.

## 📸 Project Gallery


![smart_plant_hardware](https://github.com/user-attachments/assets/93fefc6c-8d88-4d65-a838-92d87e0e0d09)

  **Figure 1: Custom PCB & Hardware Assembly.** *The physical implementation of the project. I designed the schematic and routed the custom PCB using **KiCad** to seamlessly integrate the XIAO ESP32-C3 with various modules: an I2C environment sensor array (including BH1750), a battery-backed RTC, and the power delivery circuitry. The entire system is optimized to run autonomously on a standard 5V powerbank utilizing Deep Sleep cycles.*
<img width="1244" height="779" alt="image" src="https://github.com/user-attachments/assets/37a24afd-0508-4783-898e-a38cfc5661e0" />



  **Figure 1: Custom PCB & Hardware Assembly.** Showing the XIAO ESP32-C3 integration with environmental sensors and the watering unit running on powerbank supply.
<img width="719" height="681" alt="image" src="https://github.com/user-attachments/assets/2d4ea02c-d76a-47d6-8b8a-af10fbc866f6" />
 <img width="719" height="697" alt="image" src="https://github.com/user-attachments/assets/65641eec-899a-41da-b56c-126238572afb" />


https://github.com/user-attachments/assets/34abda69-28c0-408e-982a-c36de282d4bd


  **Figure 2: React Dashboard.** Visualizing real-time telemetry data (humidity, temperature, pressure, light) received from the .NET REST API.

 


https://github.com/user-attachments/assets/e3be91f5-163f-4e76-9010-1a1898133729
  **Figure 3: Emergency Offline Mode.** The lightweight captive portal/HTML page served directly by the ESP32 when external WiFi is unavailable.


## 🚀
