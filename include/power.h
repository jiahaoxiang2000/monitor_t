#pragma once

#include <Arduino.h>

class Power {
public:
    Power();
    void begin();
    float readVoltage();
    uint8_t getBatteryLevel();
    bool isLowBattery();

private:
    float _voltage;
    uint8_t _batteryLevel;
};
