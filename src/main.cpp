#include <Arduino.h>
#include <Wire.h>
#include "pins_arduino.h"
#include "helpers.h"
#include "led.h"
#include "SHT40.h"
struct DeviceParameter {
  uint16_t Temp;
  uint16_t Humi;
  uint8_t sleepStatus;
  uint8_t KeyStatus;
} device_parameter;

SHT40 sht;
void setup() {
 pinMode(LED_BLINK, OUTPUT);
 initPins();
 cleanLED();
setupLEDTimer();
enableDisplay(true);
   //  setDisplayNumber(1, 1, 1);
sht.begin();

 digitalWrite(LED_BLINK, LOW);
}

void loop() {
 digitalWrite(LED_BLINK, !digitalRead(LED_BLINK));
    delay(1000);

    if (sht.readSample()) {
   
      setDisplayNumber(1, 1, sht.getTemperature() / 100);
      setDisplayNumber(1, 2, (static_cast<int>(sht.getTemperature()) / 10) % 10);
      setDisplayNumber(1, 3, static_cast<int>(sht.getTemperature()) % 10);
    } else{
     setDisplayNumber(2, 1, 1);

    }

}