

#include <Arduino.h>
#include "Logic/plant entity/PlantManager.h" 

// --- KONFIGURACJA ---
#define PIN_SDA 6
#define PIN_SCL 7
#define PIN_PUMP 21     
#define PIN_MOIST 2     
#define PIN_LED 3       
#define PIN_BTN 4       

// Obiekty
PlantManager manager(new Indicator(PIN_LED, PIN_BTN), new WateringUnit(PIN_PUMP, PIN_MOIST), new RealTimeClock());

// Zmienne
bool interactiveMode = true; // ZAWSZE TRUE - bo zawsze chcemy wejść do loop
unsigned long lastActivityTime = 0;

void setup() {
    Serial.begin(115200);
    
    // Safety delay (żebyś mogła wgrać kod w razie pętli resetów)
    long start = millis();
    while(!Serial && (millis()-start < 2000)); 
    delay(2000); // Dajmy 2s na start, nie 10s (szkoda czasu na prezentacji)

    Serial.println("\n--- POBUDKA SYSTEMU ---");
    
    // 1. Inicjalizacja sprzętu (WiFi/AP)
    manager.begin(true); 

    // 2. Dlaczego wstaliśmy? (Tylko dla informacji w logach)
    esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();
    if (reason == ESP_SLEEP_WAKEUP_GPIO) {
        Serial.println(">> Powód: PRZYCISK.");
    } else {
        Serial.println(">> Powód: TIMER / RESET.");
    }

    // 3. PIERWSZY POMIAR (ZAWSZE!)
    // Wykonujemy go od razu, żeby mieć dane na start (dla WWW i C#)
    Serial.println("[Setup] Wykonuje pierwszy pomiar...");
    manager.runAutoCycle();

    // 4. PRZYGOTOWANIE DO PĘTLI
    lastActivityTime = millis();
    
    // UWAGA: Tu NIE MA manager.sleep()! 
    // Pozwalamy procesorowi wyjść z setup() i wpaść naturalnie do loop().
}

void loop() {
    
    // 1. Obsługa sieci (Server WWW / WiFi)
    manager.updateNetwork();
    
    // 2. Obsługa przycisku
    manager.handleUI();
    manager.handleAPVisuals(); 

    // 3. Reset licznika czasu przy aktywności (klikaniu)
    if (digitalRead(PIN_BTN) == LOW) {
        lastActivityTime = millis();
    }

    // 4. CYKLICZNE POMIARY W PĘTLI (co 10 sekund)
    // Dzięki temu na prezentacji wykresy w C# będą "żyły"
    
    if (millis() - manager.get_lastMeasure() > 10000) {
        manager.set_lastMeasure(millis());
       
        manager.runAutoCycle(); 
    }

    
    
    // 5. TIMEOUT (Inteligentne usypianie)
    // Jeśli AP (Awaria) -> 6 minut (30o tys ms)
    // Jeśli WiFi (Sukces) -> 5 minuty (180000 ms) - czas na pokazanie wykresów
    unsigned long timeout = manager.isAPmode() ? 300000 : 180000;

    if (millis() - lastActivityTime > timeout) {
        Serial.print("[LOOP] Czas aktywnosci minal (");
        Serial.print(timeout/1000);
        Serial.println("s). Ide spac.");
        
        // Dopiero tutaj idziemy spać!
        // Budzik na 3 minut (dla testów) lub 180 min (finalnie)
        manager.sleep(3); 
    }
    
    
    delay(20); 
    
}

