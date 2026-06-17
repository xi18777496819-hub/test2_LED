/**
  ******************************************************************************
  * @file    led.c
  * @brief   LED控制函数实现
  *
  *          对HAL GPIO操作进行封装，隐藏PC13 active-low硬件细节。
  *          上层业务代码只需调用 LED_On / LED_Off / LED_Toggle。
  ******************************************************************************
  */

/* 头文件引用 ----------------------------------------------------------------*/
#include "led.h"
#include "gpio.h"

/* 函数实现 ------------------------------------------------------------------*/

/**
  * @brief  点亮LED。
  * @note   PC13低电平有效 — 写RESET使引脚拉低灌电流，LED点亮。
  */
void LED_On(void)
{
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  熄灭LED。
  * @note   写SET使引脚拉高，停止灌电流，LED熄灭。
  */
void LED_Off(void)
{
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/**
  * @brief  翻转LED状态。
  * @note   底层调用 HAL_GPIO_TogglePin 自动翻转引脚电平。
  */
void LED_Toggle(void)
{
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}
