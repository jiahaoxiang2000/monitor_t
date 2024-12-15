#ifndef __HELPERS_H
#define __HELPERS_H

#include <Arduino.h>

#include "pins_arduino.h"





// Function declarations
void SN74HC595_Send_Data(uint8_t sn_num, uint8_t sendValue);
void ShowNum(uint8_t row, uint8_t column, uint8_t value);
void SHT40_Read_RHData(float *temperature, float *humidity);
void SysCtlDelay(unsigned long ulCount);
void cleanLED();
void initPins();

#endif // __HELPERS_H
