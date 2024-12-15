#pragma once

#include "Arduino.h"

#define LED_BLINK PB7 // LED Blink Pin

// LED Matrix Control Pins
#define LED1_SER_PIN   PB2
#define LED1_RCLK_PIN  PA8
#define LED1_SCLK_PIN  PA9

#define LED2_SER_PIN   PA0
#define LED2_RCLK_PIN  PA1
#define LED2_SCLK_PIN  PA2

#define DIG_SER_PIN    PA3
#define DIG_RCLK_PIN   PA4
#define DIG_SCLK_PIN   PA5

// Other Control Pins
#define LED2_PIN       PB3
#define WAKE_PIN       PB5
#define BAT_ADC_PIN    PB1

// I2C Pins
#define I2C_SDA_PIN    PB9
#define I2C_SCL_PIN    PB8

// Constants
#define SHT40_ADDR     0x44
#define SN_LED1        1
#define SN_LED2        2
#define SN_DIG         3

#define KEY_NO_PRESS    0
#define KEY_SHAKE_STATE 1
