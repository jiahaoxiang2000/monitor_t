#include <Arduino.h>
#include <Wire.h>
#include "pins_arduino.h"

void SN74HC595_Send_Data(uint8_t sn_num, uint8_t sendValue) {
  uint8_t serPin, sclkPin, rclkPin;
  
  switch(sn_num) {
    case SN_LED1:
      serPin = LED1_SER_PIN;
      sclkPin = LED1_SCLK_PIN;
      rclkPin = LED1_RCLK_PIN;
      break;
    case SN_LED2:
      serPin = LED2_SER_PIN;
      sclkPin = LED2_SCLK_PIN;
      rclkPin = LED2_RCLK_PIN;
      break;
    case SN_DIG:
      serPin = DIG_SER_PIN;
      sclkPin = DIG_SCLK_PIN;
      rclkPin = DIG_RCLK_PIN;
      break;
    default:
      return;
  }
  
  for (int i = 0; i < 8; i++) {
    digitalWrite(serPin, (sendValue << i) & 0x80 ? HIGH : LOW);
    digitalWrite(sclkPin, LOW);
    digitalWrite(sclkPin, HIGH);
  }
  digitalWrite(rclkPin, LOW);
  digitalWrite(rclkPin, HIGH);
}

void SHT40_Read_RHData(float *temperature, float *humidity) {
  Wire.beginTransmission(SHT40_ADDR);
  Wire.write(0xFD);
  Wire.endTransmission();
  delay(10);
  
  Wire.requestFrom(SHT40_ADDR, 6);
  uint32_t tempData = Wire.read() << 8 | Wire.read();
  Wire.read(); // CRC
  uint32_t humiData = Wire.read() << 8 | Wire.read();
  Wire.read(); // CRC
  
  *temperature = (tempData * 175.0f) / 65535.0f - 45;
  *humidity = (humiData * 125.0f) / 65535.0f - 6;
}
