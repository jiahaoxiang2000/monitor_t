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
   
      float temp = sht.getTemperature();
      int wholeNumber = static_cast<int>(temp);
      int decimal = static_cast<int>((temp - wholeNumber) * 10);  // Get first decimal place

      setDisplayNumber(1, 1, wholeNumber / 10);     // First digit of whole number
      setDisplayNumber(1, 2, wholeNumber % 10);     // Second digit of whole number
      setDisplayNumber(1, 3, decimal);              // Decimal place

      float humi = sht.getHumidity();
      wholeNumber = static_cast<int>(humi);
      decimal = static_cast<int>((humi - wholeNumber) * 10);  // Get first decimal place

      setDisplayNumber(2, 1, wholeNumber / 10);     // First digit of whole number
      setDisplayNumber(2, 2, wholeNumber % 10);     // Second digit of whole number
      setDisplayNumber(2, 3, decimal);              // Decimal place
    } 

}