/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
 *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "gpio.h"
static uint16_t updata_flag;
static uint8_t sleep_flag;
/* USER CODE END 0 */

TIM_HandleTypeDef htim14;

/* TIM14 init function */
void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 64-1;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 40-1;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM14)
  {
  /* USER CODE BEGIN TIM14_MspInit 0 */

  /* USER CODE END TIM14_MspInit 0 */
    /* TIM14 clock enable */
    __HAL_RCC_TIM14_CLK_ENABLE();

    /* TIM14 interrupt Init */
    HAL_NVIC_SetPriority(TIM14_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM14_IRQn);
  /* USER CODE BEGIN TIM14_MspInit 1 */

  /* USER CODE END TIM14_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM14)
  {
  /* USER CODE BEGIN TIM14_MspDeInit 0 */

  /* USER CODE END TIM14_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM14_CLK_DISABLE();

    /* TIM14 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM14_IRQn);
  /* USER CODE BEGIN TIM14_MspDeInit 1 */

  /* USER CODE END TIM14_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM14)
  {
    // 40ms trigger the TIM14 interrupt
    HAL_TIM_Base_Stop_IT(&htim14);
    updata_flag++;
    if (updata_flag <= 5000)
    {
      ShowNum(1, 1, (device_paramter.Temp / 100));
      ShowNum(1, 2, (device_paramter.Temp / 10 % 10));
      ShowNum(1, 3, device_paramter.Temp % 10);
    }
    else if (updata_flag <= 10000)
    {
      ShowNum(2, 1, (device_paramter.Humi / 100));
      ShowNum(2, 2, (device_paramter.Humi / 10 % 10));
      ShowNum(2, 3, device_paramter.Humi % 10);
    }
    else
    {
      updata_flag = 0;
      sleep_flag++;
    }

    // run timer again
    __HAL_TIM_SetCounter(&htim14, 0);
    // if show the temperature and humidity three, then go the sleep status
    if (sleep_flag >= 3)
    {
      sleep_flag = 0;
      device_paramter.sleepStatus = 1;
      SN74HC595_Send_Data(SN_DIG, 0xFF);
      SN74HC595_Send_Data(SN_LED1, 0x00);
      SN74HC595_Send_Data(SN_LED2, 0x00);
    }
    else
    {
      // show twice, need to run the timer again by the interrupt
      HAL_TIM_Base_Start_IT(&htim14);
    }
  }
}
/* USER CODE END 1 */
