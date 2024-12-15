#pragma once
#include <cstdint>
#include "HardwareTimer.h"

extern HardwareTimer *displayTimer;

void setupLEDTimer();
void setDisplayNumber(uint8_t row, uint8_t col, uint8_t number);
void enableDisplay(bool enable);
void updateTempHumi(int16_t temp, int16_t humi);
void display_update();  // Timer callback function
