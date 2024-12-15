#include <Arduino.h>
#include <Wire.h>
#include "pins_arduino.h"
#include "helpers.h"
#include "led.h"
#include "SHT40.h"

SHT40 sht;
volatile bool isAwake = true;
unsigned long lastWakeTime = 0;
const unsigned long SLEEP_TIMEOUT = 10000; // 10 seconds in milliseconds

void wakeInterrupt() {
   enableDisplay(true);
    isAwake = true;
    lastWakeTime = millis();
}

void enterSleepMode() {
    enableDisplay(false);
    cleanLED(); // Turn off all displays
    digitalWrite(LED_BLINK, HIGH);
    delay(10);
    
    // Configure Sleep mode
   //  HAL_PWR_EnableSleepOnExit();
   //  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

}

void initSleepMode() {
    // Configure wake pin
    pinMode(WAKE_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeInterrupt, RISING);
}

void setup() {
    pinMode(LED_BLINK, OUTPUT);
    digitalWrite(LED_BLINK, HIGH);
    initPins();
    cleanLED();
    setupLEDTimer();
    enableDisplay(true);
    sht.begin();
    
    // Initial sleep mode
    initSleepMode();
}

void loop() {
    if (isAwake) {
        // Check if it's time to go back to sleep
        if (millis() - lastWakeTime >= SLEEP_TIMEOUT) {
            isAwake = false;
            enterSleepMode();
            return;
        }

        // Normal operation code
      //   digitalWrite(LED_BLINK, LOW);
        if (sht.readSample()) {
            float temp = sht.getTemperature();
            int wholeNumber = static_cast<int>(temp);
            int decimal = static_cast<int>((temp - wholeNumber) * 10);

            setDisplayNumber(1, 1, wholeNumber / 10);
            setDisplayNumber(1, 2, wholeNumber % 10);
            setDisplayNumber(1, 3, decimal);

            float humi = sht.getHumidity();
            wholeNumber = static_cast<int>(humi);
            decimal = static_cast<int>((humi - wholeNumber) * 10);

            setDisplayNumber(2, 1, wholeNumber / 10);
            setDisplayNumber(2, 2, wholeNumber % 10);
            setDisplayNumber(2, 3, decimal);
        }
        delay(1000);
    } else {
        // Device is sleeping, WFI instruction will be executed
        __WFI();
    }
}