#include "power.h"
#include "pins_arduino.h"

const PinMap PinMap_ADC[] = {
    {PB_0, ADC1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 9, 0)}, // ADC1_IN9
    {NC, NP, 0}
};

// power issue. run then upload Error: checksum mismatch - attempting binary compare 

Power::Power() : _voltage(0), _batteryLevel(0) {}

void Power::begin() {
    pinMode(PB_0, INPUT_ANALOG);
    analogReadResolution(12); // Set ADC resolution to 12 bits
}

float Power::readVoltage() {
    uint32_t adcValue = analogRead(PB_0);
    // Convert ADC value to voltage (3.0V is the VDDA voltage)
    // Multiply by 2 because of voltage divider
    _voltage = (adcValue * 3.0f * 2.0f) / 4095.0f;
    return _voltage;
}

uint8_t Power::getBatteryLevel() {
    float voltage = readVoltage();
    
    // (3.0V is the VDDA voltage) 
    if (voltage >= 4.2f) {
        _batteryLevel = 100;
    } else if (voltage >= 3.9f) {
        _batteryLevel = 75;
    } else if (voltage >= 3.6f) {
        _batteryLevel = 50;
    } else if (voltage >= 3.3f) {
        _batteryLevel = 25;
    } else {
        _batteryLevel = 0;
    }
    
    return _batteryLevel;
}

bool Power::isLowBattery() {
    return (_voltage < 3.3f); // Consider low battery below 3.3V
}
