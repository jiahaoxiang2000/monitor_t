/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED2_SER_Pin | LED2_RCLK_Pin | LED2_SCLK_Pin | DIG_SER_Pin | DIG_RCLK_Pin | DIG_SCLK_Pin | LED1_RCLK_Pin | LED1_SCLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_SER_Pin | LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = LED2_SER_Pin | LED2_RCLK_Pin | LED2_SCLK_Pin | DIG_SER_Pin | DIG_RCLK_Pin | DIG_SCLK_Pin | LED1_RCLK_Pin | LED1_SCLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LED1_SER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LED1_SER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = WAKE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(WAKE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/* USER CODE BEGIN 2 */

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == WAKE_Pin)
  {
    // HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    device_paramter.KeyStatus = KEY_SHAKE_STATE;
    HAL_ResumeTick();
  }
}

// 0~9，+ the dot
uint16_t sgh_value[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80};

/**
 * @brief Sends data to SN74HC595 shift registers.
 * @param sn_num: The number of the shift register (SN_LED1, SN_LED2, or SN_DIG).
 * @param sendValue: The data to be sent to the shift register.
 * @retval None
 */
void SN74HC595_Send_Data(unsigned char sn_num, unsigned int sendValue)
{
  uint8_t i = 0;
  if (sn_num == SN_LED1)
  {
    for (i = 0; i < 8; i++)
    {
      if (((sendValue << i) & 0x80) != 0)
      {
        HAL_GPIO_WritePin(LED1_SER_GPIO_Port, LED1_SER_Pin, GPIO_PIN_SET); // 高电平
      }
      else
      {
        HAL_GPIO_WritePin(LED1_SER_GPIO_Port, LED1_SER_Pin, GPIO_PIN_RESET); // 低电平
      }
      HAL_GPIO_WritePin(LED1_SCLK_GPIO_Port, LED1_SCLK_Pin, GPIO_PIN_RESET); // 产生一个SCLK上�?�沿
      HAL_GPIO_WritePin(LED1_SCLK_GPIO_Port, LED1_SCLK_Pin, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(LED1_RCLK_GPIO_Port, LED1_RCLK_Pin, GPIO_PIN_RESET); // 产生一个RCLK上�?�沿
    HAL_GPIO_WritePin(LED1_RCLK_GPIO_Port, LED1_RCLK_Pin, GPIO_PIN_SET);
  }
  else if (sn_num == SN_LED2)
  {
    for (i = 0; i < 8; i++)
    {
      if (((sendValue << i) & 0x80) != 0)
      {
        HAL_GPIO_WritePin(LED2_SER_GPIO_Port, LED2_SER_Pin, GPIO_PIN_SET);
      }
      else
      {
        HAL_GPIO_WritePin(LED2_SER_GPIO_Port, LED2_SER_Pin, GPIO_PIN_RESET);
      }
      HAL_GPIO_WritePin(LED2_SCLK_GPIO_Port, LED2_SCLK_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED2_SCLK_GPIO_Port, LED2_SCLK_Pin, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(LED2_RCLK_GPIO_Port, LED2_RCLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED2_RCLK_GPIO_Port, LED2_RCLK_Pin, GPIO_PIN_SET);
  }
  else if (sn_num == SN_DIG)
  {
    for (i = 0; i < 8; i++)
    {
      if (((sendValue << i) & 0x80) != 0)
      {
        HAL_GPIO_WritePin(DIG_SER_GPIO_Port, DIG_SER_Pin, GPIO_PIN_SET);
      }
      else
      {
        HAL_GPIO_WritePin(DIG_SER_GPIO_Port, DIG_SER_Pin, GPIO_PIN_RESET);
      }
      HAL_GPIO_WritePin(DIG_SCLK_GPIO_Port, DIG_SCLK_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(DIG_SCLK_GPIO_Port, DIG_SCLK_Pin, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(DIG_RCLK_GPIO_Port, DIG_RCLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DIG_RCLK_GPIO_Port, DIG_RCLK_Pin, GPIO_PIN_SET);
  }
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

/**
 * @brief Displays a number on a 7-segment LED display.
 *
 * This function takes in the row, column, and value parameters to determine
 * which 7-segment LED display to update and what number to display on it.
 *
 * @param row The row of the LED display (1 or 2).
 * @param column The column of the LED display (1, 2, or 3).
 * @param value The value to be displayed on the LED display (0-9).
 */
void ShowNum(uint8_t row, uint8_t column, uint8_t value)
{
  if (row == 1)
  {
    switch (column)
    {
    case 1:
      SN74HC595_Send_Data(SN_DIG, 0xFE);
      SN74HC595_Send_Data(SN_LED1, sgh_value[value]);
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
    SysCtlDelay(1000);
    SN74HC595_Send_Data(SN_LED2, 0x00);
  }
}

/* USER CODE END 2 */
