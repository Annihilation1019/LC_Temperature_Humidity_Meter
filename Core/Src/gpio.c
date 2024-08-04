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
// 0~9数码管显示编码，外加一个小数点
static uint16_t sgh_value[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80};
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
  HAL_GPIO_WritePin(GPIOA, LED2_SER_Pin|LED2_RCLK_Pin|LED2_SCLK_Pin|DIG_SER_Pin
                          |DIG_RCLK_Pin|DIG_SCLK_Pin|LED1_RCLK_Pin|LED1_SCLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_SER_GPIO_Port, LED1_SER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_TEST_GPIO_Port, LED_TEST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = LED2_SER_Pin|LED2_RCLK_Pin|LED2_SCLK_Pin|DIG_SER_Pin
                          |DIG_RCLK_Pin|DIG_SCLK_Pin|LED1_RCLK_Pin|LED1_SCLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = LED1_SER_Pin|LED_TEST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = WAKE_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(WAKE_KEY_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 2 */

/**
 * @brief SN74HC595发送数据
 *
 * @param sn_num 数码管编号以及位选择
 * @param sendValue 发送数码管数字编码
 */
void SN74HC595_Send_Data(unsigned char sn_num, unsigned int sendValue)
{
  uint8_t i = 0;
  if (sn_num == SN_LED1) // 第一个数码管
  {
    for (i = 0; i < 8; i++)
    {
      if (((sendValue << i) & 0x80) != 0) // 从高位开始发送
      {
        HAL_GPIO_WritePin(LED1_SER_GPIO_Port, LED1_SER_Pin, GPIO_PIN_SET); // 高电平
      }
      else
      {
        HAL_GPIO_WritePin(LED1_SER_GPIO_Port, LED1_SER_Pin, GPIO_PIN_RESET); // 低电平
      }
      HAL_GPIO_WritePin(LED1_SCLK_GPIO_Port, LED1_SCLK_Pin, GPIO_PIN_RESET); // 产生一个SCLK上升沿
      HAL_GPIO_WritePin(LED1_SCLK_GPIO_Port, LED1_SCLK_Pin, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(LED1_RCLK_GPIO_Port, LED1_RCLK_Pin, GPIO_PIN_RESET); // 产生一个RCLK上升沿
    HAL_GPIO_WritePin(LED1_RCLK_GPIO_Port, LED1_RCLK_Pin, GPIO_PIN_SET);
  }
  else if (sn_num == SN_LED2) // 第二个数码管
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
  else if (sn_num == SN_DIG) // 位选择
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

/* 64Mhz时钟时，当ulCount为1，函数耗时3个时钟，延时=3*1/64us */
__asm void SysCtlDelay(unsigned long ulCount)
{
  SUBS r0, #1;
  BNE SysCtlDelay;
  BX lr;
}

/**
 * @brief 数码管显示数字
 *
 * @param row 选择行 1或2
 * @param column 选择列 1、2、3
 * @param value 要显示的数字 0 ~ 9
 * @param type 显示类型（温湿度、电压值、日期）
 */
void ShowNum(uint8_t row, uint8_t column, uint8_t value, uint8_t type)
{
  if (row == 1)
  {
    switch (column)
    {
    case 1:                                  // 如果是第一排第一个
      if (type == TEMP_HUMI || type == TIME) // 如果是温湿度或时间显示
      {
        SN74HC595_Send_Data(SN_DIG, 0xFE);              // 位选
        SN74HC595_Send_Data(SN_LED1, sgh_value[value]); // 显示值对应16进制数
      }
      else if (type == VOL) // 如果是电压显示
      {
        SN74HC595_Send_Data(SN_DIG, 0xFE);                     // 位选
        SN74HC595_Send_Data(SN_LED1, sgh_value[value] | 0x80); // 显示值对应16进制数
      }
      break;
    case 2:
      if (type == TEMP_HUMI)
      {
        SN74HC595_Send_Data(SN_DIG, 0xFD);
        SN74HC595_Send_Data(SN_LED1, (sgh_value[value] | 0x80)); // 显示值加上小数点
      }
      else if (type == VOL || type == TIME)
      {
        SN74HC595_Send_Data(SN_DIG, 0xFD);
        SN74HC595_Send_Data(SN_LED1, sgh_value[value]); // 显示值对应16进制数
      }
      break;
    case 3:
      SN74HC595_Send_Data(SN_DIG, 0xFB);
      SN74HC595_Send_Data(SN_LED1, sgh_value[value]);
      break;
    default:
      break;
    }
    SysCtlDelay(1000);                  // 大致延时50us
    SN74HC595_Send_Data(SN_LED1, 0x00); // 消影，防止错位
  }
  else
  {
    switch (column)
    {
    case 1:
      if (type == TEMP_HUMI || type == TIME)
      {
        SN74HC595_Send_Data(SN_DIG, 0xF7);
        SN74HC595_Send_Data(SN_LED2, sgh_value[value]);
      }
      else if (type == VOL)
      {
        SN74HC595_Send_Data(SN_DIG, 0xF7);
        SN74HC595_Send_Data(SN_LED2, sgh_value[value] | 0x80);
      }
      break;
    case 2:
      if (type == TEMP_HUMI)
      {
        SN74HC595_Send_Data(SN_DIG, 0xEF);
        SN74HC595_Send_Data(SN_LED2, (sgh_value[value] | 0x80));
      }
      else if (type == VOL || type == TIME)
      {
        SN74HC595_Send_Data(SN_DIG, 0xEF);
        SN74HC595_Send_Data(SN_LED2, sgh_value[value]);
      }
      break;
    case 3:
      SN74HC595_Send_Data(SN_DIG, 0xDF);
      SN74HC595_Send_Data(SN_LED2, sgh_value[value]);
      break;
    default:
      break;
    }
    SysCtlDelay(1000); // 大致延时50us
    SN74HC595_Send_Data(SN_LED2, 0x00);
  }
}

/**
 * @brief 按键中断回调函数
 *
 * @param GPIO_Pin 按键引脚
 */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_5)
  {
    device_paramter.KeyStatus = KEY_SHAKE_STATE; // 设置按键状态为消抖状态
    HAL_ResumeTick();                            // 恢复滴答定时器以唤醒系统
  }
}
/* USER CODE END 2 */
