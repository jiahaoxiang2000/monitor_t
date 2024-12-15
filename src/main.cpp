#include <Arduino.h>
#include <Wire.h>
#include "pins_arduino.h"
#include "helpers.h"
#include "led.h"

struct DeviceParameter {
  uint16_t Temp;
  uint16_t Humi;
  uint8_t sleepStatus;
  uint8_t KeyStatus;
} device_parameter;


void setup() {
 pinMode(LED_BLINK, OUTPUT);
 initPins();
 cleanLED();
    setupLEDTimer();
    enableDisplay(true);
    setDisplayNumber(1, 1, 1);
    setDisplayNumber(1, 2, 1);
    setDisplayNumber(2, 1, 1);
 digitalWrite(LED_BLINK, LOW);
}

void loop() {
 digitalWrite(LED_BLINK, !digitalRead(LED_BLINK));
    delay(1000);

}