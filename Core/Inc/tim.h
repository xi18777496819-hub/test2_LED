/**
  ******************************************************************************
  * @file    tim.h
  * @brief   TIM2 timer configuration header
  *
  *          TIM2 generates a 0.5 s period interrupt for the LED blink task.
  *          Clock: 72 MHz → ÷7200 → 10 kHz; Period: 5000 → 0.5 s.
  ******************************************************************************
  */

#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;

/**
  * @brief  Blink flag set by TIM2 ISR every 0.5 s.
  * @note   Defined in main.c. Cleared by main loop after consuming.
  */
extern volatile uint8_t tim_flag;

/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Initialize TIM2 for 0.5 s periodic interrupt.
  */
void MX_TIM2_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */
