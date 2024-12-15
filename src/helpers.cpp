#include <Arduino.h>
#include <Wire.h>
#include "pins_arduino.h"
#include "helpers.h"

// 0~9，+ the dot
uint16_t sgh_value[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80};

// Global variables for display control
volatile uint16_t updata_flag = 0;
volatile uint8_t sleep_flag = 0;
struct {
    int16_t Temp;
    int16_t Humi;
    uint8_t sleepStatus;
} device_paramter = {0};

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

/* 64Mhz clock，ulCount=1，function run 3 cycles，delay=3*1/64us */
void SysCtlDelay(unsigned long ulCount)
{
  __asm__ __volatile__(
      "1: sub %0, %0, #1\n"
      "bne 1b"
      : "=r"(ulCount)
      : "0"(ulCount));
}

void ShowNum(uint8_t row, uint8_t column, uint8_t value)
{
  if (row == 1)
  {
    switch (column)
    {
    case 1:
      SN74HC595_Send_Data(SN_DIG, 0xFE);
      SN74HC595_Send_Data(SN_LED1, sgh_value[value] );
      break;
    case 2:
      SN74HC595_Send_Data(SN_DIG, 0xFD);
      SN74HC595_Send_Data(SN_LED1, (sgh_value[value] | 0x80));
      break;
    case 3:
      SN74HC595_Send_Data(SN_DIG, 0xFB);
      SN74HC595_Send_Data(SN_LED1, sgh_value[value]);
      break;
    default:
      break;
    }
    // delayMicroseconds(50);  // Replace SysCtlDelay(1000)
    // dynamic scan show, need to delay 50us
    SysCtlDelay(1000); // 50us
    SN74HC595_Send_Data(SN_LED1, 0x00);
  }
  else
  {
    switch (column)
    {
    case 1:
      SN74HC595_Send_Data(SN_DIG, 0xF7);
      SN74HC595_Send_Data(SN_LED2, sgh_value[value]);
      break;
    case 2:
      SN74HC595_Send_Data(SN_DIG, 0xEF);
      SN74HC595_Send_Data(SN_LED2, (sgh_value[value] | 0x80));
      break;
    case 3:
      SN74HC595_Send_Data(SN_DIG, 0xDF);
      SN74HC595_Send_Data(SN_LED2, sgh_value[value]);
      break;
    default:
      break;
    }
    SysCtlDelay(1000); // 50us
    SN74HC595_Send_Data(SN_LED2, 0x00);
  }
}

void cleanLED() {
   // clean all shift registers data to zero, show digial show clean.
  SN74HC595_Send_Data(SN_DIG, 0xff);
  SN74HC595_Send_Data(SN_LED1, 0x00);
  SN74HC595_Send_Data(SN_LED2, 0x00);
}

void initPins() {
    // LED1 pins
    pinMode(LED1_SER_PIN, OUTPUT);
    pinMode(LED1_SCLK_PIN, OUTPUT);
    pinMode(LED1_RCLK_PIN, OUTPUT);
    
    // LED2 pins
    pinMode(LED2_SER_PIN, OUTPUT);
    pinMode(LED2_SCLK_PIN, OUTPUT);
    pinMode(LED2_RCLK_PIN, OUTPUT);
    
    // Digital pins
    pinMode(DIG_SER_PIN, OUTPUT);
    pinMode(DIG_SCLK_PIN, OUTPUT);
    pinMode(DIG_RCLK_PIN, OUTPUT);

    // Initialize all pins to LOW
    digitalWrite(LED1_SER_PIN, LOW);
    digitalWrite(LED1_SCLK_PIN, LOW);
    digitalWrite(LED1_RCLK_PIN, LOW);
    digitalWrite(LED2_SER_PIN, LOW);
    digitalWrite(LED2_SCLK_PIN, LOW);
    digitalWrite(LED2_RCLK_PIN, LOW);
    digitalWrite(DIG_SER_PIN, LOW);
    digitalWrite(DIG_SCLK_PIN, LOW);
    digitalWrite(DIG_RCLK_PIN, LOW);
}
