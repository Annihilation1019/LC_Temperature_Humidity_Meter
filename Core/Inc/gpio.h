/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
typedef enum
{
  SN_LED1,
  SN_LED2,
  SN_DIG
}SN_NUM; // 数码管选择位

typedef enum 
{
  KEY_NO_PRESS,
  KEY_SHAKE_STATE
}KEY_STATE; // 按键状态位

typedef enum
{
  TEMP_HUMI,
  VOL,
  TIME,
}SHOW_TYPE; // 显示类型

#define Open_Led()	 	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET)
#define Close_Led() 	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET)
#define Toggle_LED()	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7)

extern uint8_t key_state;
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void SN74HC595_Send_Data(unsigned char sn_num, unsigned int sendValue);
void ShowNum(uint8_t row, uint8_t column, uint8_t value,uint8_t type);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

