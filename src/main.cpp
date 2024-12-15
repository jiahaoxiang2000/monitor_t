#include <Arduino.h>
#include <Wire.h>
#include "pins_arduino.h"
#include "helpers.h"

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
     SN74HC595_Send_Data(SN_DIG, 0xfb);
 ShowNum(1, 1, 6);

 ShowNum(1, 2, 6);
 ShowNum(1, 3, 6);
 digitalWrite(LED_BLINK, LOW);
}

void loop() {
 digitalWrite(LED_BLINK, !digitalRead(LED_BLINK));
    delay(1000);

}