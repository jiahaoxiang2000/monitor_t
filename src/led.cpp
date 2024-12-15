#include <Arduino.h>
#include "helpers.h"
#include "HardwareTimer.h"

// Display control variables
static uint8_t current_row = 1;
static uint8_t current_col = 1;
static uint8_t display_data[2][3] = {{0}}; // [row][column]
static bool display_enabled = false;

// Hardware timer instance
HardwareTimer *displayTimer;

// Timer callback function
void display_update() {
    if (!display_enabled) return;
    
    // Display current digit
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            ShowNum(i+1, j+1, display_data[i][j]);
        }
    }
}

void setupLEDTimer() {
    // Configure TIM14 for 2ms interval (500Hz)
    displayTimer = new HardwareTimer(TIM14);
    displayTimer->setOverflow(4000, MICROSEC_FORMAT); // 40ms interval
    displayTimer->attachInterrupt(display_update);
    displayTimer->resume();
}

void setDisplayNumber(uint8_t row, uint8_t col, uint8_t number) {
    if (row < 1 || row > 2 || col < 1 || col > 3 || number > 9) return;
    display_data[row-1][col-1] = number;
}

void enableDisplay(bool enable) {
    display_enabled = enable;
    if (enable) {
        displayTimer->resume();
    } else {
        displayTimer->pause();
        cleanLED();
    }
}



// Example usage function for temperature and humidity display
void updateTempHumi(int16_t temp, int16_t humi) {
    // Update temperature digits (row 1)
    setDisplayNumber(1, 1, temp / 100);
    setDisplayNumber(1, 2, (temp / 10) % 10);
    setDisplayNumber(1, 3, temp % 10);
    
    // Update humidity digits (row 2)
    setDisplayNumber(2, 1, humi / 100);
    setDisplayNumber(2, 2, (humi / 10) % 10);
    setDisplayNumber(2, 3, humi % 10);
}
