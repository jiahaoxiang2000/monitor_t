#include "SHT40.h"
#include "pins_arduino.h"
#include <Wire.h>

SHT40::SHT40(uint8_t address) : _address(address), _temperature(0), _humidity(0) {}

bool SHT40::begin() {
    Wire.setSDA(I2C_SDA_PIN);  // Set SDA to PB9
    Wire.setSCL(I2C_SCL_PIN);  // Set SCL to PB8
    Wire.begin();
    
    Wire.beginTransmission(_address);
    return Wire.endTransmission() == 0;
}

bool SHT40::readSample() {
    uint8_t readData[6];
    uint32_t tempData;
    
    // Send high precision measurement command
    Wire.beginTransmission(_address);
    Wire.write(0xFD);
    if (Wire.endTransmission() != 0) {
        return false;
    }

    delay(10);  // Wait for measurement

    if (Wire.requestFrom(_address, (uint8_t)6) != 6) {
        return false;
    }

    // Read all bytes
    for (int i = 0; i < 6; i++) {
        readData[i] = Wire.read();
    }

    // Convert temperature
    tempData = ((uint32_t)readData[0] << 8) | readData[1];
    _temperature = ((float)tempData * 175.0f) / 65535.0f - 45.0f;

    // Convert humidity
    tempData = ((uint32_t)readData[3] << 8) | readData[4];
    _humidity = ((float)tempData * 125.0f) / 65535.0f - 6.0f;

    return true;
}

float SHT40::getTemperature() {
    return _temperature;
}

float SHT40::getHumidity() {
    return _humidity;
}
