#pragma once

#include <Arduino.h>

class SHT40 {
public:
    SHT40(uint8_t address = 0x44);
    bool begin();
    bool readSample();
    float getTemperature();
    float getHumidity();

private:
    uint8_t _address;
    float _temperature;
    float _humidity;
};
