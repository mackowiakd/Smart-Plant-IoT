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
*(Podmień ścieżki w nawiasach na nazwy swoich zdjęć z folderu)*

* `![Hardware Setup](hardware/photo.jpg)`  
  **Figure 1: Custom PCB & Hardware Assembly.** Showing the XIAO ESP32-C3 integration with environmental sensors and the watering unit running on powerbank supply.
* `![Web Dashboard](web_app/dashboard.png)`  
  **Figure 2: React Dashboard.** Visualizing real-time telemetry data (humidity, temperature, pressure, light) received from the .NET REST API.
* `![Fallback AP](docs/fallback_ap.png)`  
  **Figure 3: Emergency Offline Mode.** The lightweight captive portal/HTML page served directly by the ESP32 when external WiFi is unavailable.

## 🚀