/**
  ******************************************************************************
  * @file    led.h
  * @brief   LED控制函数头文件
  *
  *          在HAL GPIO之上封装高层LED操作接口。
  *          板载LED位于PC13，低电平点亮（高电平熄灭）。
  ******************************************************************************
  */

#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件引用 ----------------------------------------------------------------*/
#include "main.h"

/* 导出函数声明 --------------------------------------------------------------*/

/**
  * @brief  点亮LED。
  * @note   PC13低电平有效，写RESET拉低引脚→LED亮。
  */
void LED_On(void);

/**
  * @brief  熄灭LED。
  * @note   写SET拉高引脚→LED灭。
  */
void LED_Off(void);

/**
  * @brief  翻转LED状态。
  */
void LED_Toggle(void);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H__ */
