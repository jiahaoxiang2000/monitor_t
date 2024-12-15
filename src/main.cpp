#include <Arduino.h>
#include <Wire.h>
#include "pins_arduino.h"

struct DeviceParameter {
  uint16_t Temp;
  uint16_t Humi;
  uint8_t sleepStatus;
  uint8_t KeyStatus;
} device_parameter;

const uint16_t sgh_value[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80};
volatile bool displayUpdate = false;


void SN74HC595_Send_Data(uint8_t sn_num, uint8_t sendValue);
void ShowNum(uint8_t row, uint8_t column, uint8_t value);
void SHT40_Read_RHData(float *temperature, float *humidity);

void setup() {
 pinMode(LED_BLINK, OUTPUT);
 digitalWrite(LED_BLINK, LOW);
}

void loop() {
 digitalWrite(LED_BLINK, !digitalRead(LED_BLINK));
    delay(1000);
}