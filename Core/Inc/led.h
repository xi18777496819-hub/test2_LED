/**
  ******************************************************************************
  * @file    led.h
  * @brief   LED control functions header
  *
  *          Provides high-level LED abstraction on top of GPIO HAL.
  *          The onboard LED on PC13 is active-low (RESET = ON, SET = OFF).
  ******************************************************************************
  */

#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Turn the onboard LED on.
  */
void LED_On(void);

/**
  * @brief  Turn the onboard LED off.
  */
void LED_Off(void);

/**
  * @brief  Toggle the onboard LED state.
  */
void LED_Toggle(void);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H__ */
