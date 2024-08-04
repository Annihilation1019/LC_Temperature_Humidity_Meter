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
/* 定义数据更新显示标志位和睡眠模式标志位 */
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
    HAL_NVIC_SetPriority(TIM14_IRQn, 2, 0);
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

// 将 BCD 格式转换为十进制格式
uint8_t BCDToDecimal(uint8_t bcd)
{
  return ((bcd >> 4) * 10) + (bcd & 0x0F);
}
/**
 * @brief  定时器中断回调函数
 * @param  htim 定时器句柄
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM14)
  {
    HAL_TIM_Base_Stop_IT(&htim14); // 停止定时器
    updata_flag++;
    if (updata_flag <= 5000) // 显示温度
    {
      ShowNum(1, 1, (device_paramter.Temp / 100), TEMP_HUMI);
      ShowNum(1, 2, (device_paramter.Temp / 10 % 10), TEMP_HUMI);
      ShowNum(1, 3, device_paramter.Temp % 10, TEMP_HUMI);
    }
    else if (updata_flag <= 10000) // 显示湿度
    {
      ShowNum(2, 1, (device_paramter.Humi / 100), TEMP_HUMI);
      ShowNum(2, 2, (device_paramter.Humi / 10 % 10), TEMP_HUMI);
      ShowNum(2, 3, device_paramter.Humi % 10, TEMP_HUMI);
    }
    else if (updata_flag <= 15000) // 显示电压
    {
      ShowNum(1, 1, (int)Get_VolValue(), VOL);
      ShowNum(1, 2, (int)(Get_VolValue() * 10) % 10, VOL);
      ShowNum(1, 3, (int)(Get_VolValue() * 100) % 10, VOL);
    }
    else if (updata_flag <= 20000) // 显示日期
    {
      /* 获取时间和日期 */
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

      // 将 BCD 格式转换为十进制格式
      uint8_t month = BCDToDecimal(sDate.Month);
      uint8_t date = BCDToDecimal(sDate.Date);

      ShowNum(1, 1, month / 10, TIME);
      ShowNum(1, 2, month % 10, TIME);

      ShowNum(2, 1, date / 10, TIME);
      ShowNum(2, 2, date % 10, TIME);
    }
    else if (updata_flag <= 25000) // 显示时间
    {
      /* 获取时间和日期 */
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

      // 将 BCD 格式转换为十进制格式
      uint8_t hours = BCDToDecimal(sTime.Hours);
      uint8_t minutes = BCDToDecimal(sTime.Minutes);

      ShowNum(1, 1, hours / 10, TIME);
      ShowNum(1, 2, hours % 10, TIME);

      ShowNum(2, 1, minutes / 10, TIME);
      ShowNum(2, 2, minutes % 10, TIME);
    }
    else
    {
      updata_flag = 0;
      sleep_flag++;
    }

    __HAL_TIM_SetCounter(&htim14, 0); // 重装载计数器
    if (sleep_flag >= 2)              // 显示两次数据后进入休眠
    {
      sleep_flag = 0;
      device_paramter.sleepStatus = 1;

      /* 清除显示 */
      SN74HC595_Send_Data(SN_DIG, 0xFF);
      SN74HC595_Send_Data(SN_LED1, 0x00);
      SN74HC595_Send_Data(SN_LED2, 0x00);
    }
    else
    {
      HAL_TIM_Base_Start_IT(&htim14); // 继续定时器
    }
  }
}
/* USER CODE END 1 */
