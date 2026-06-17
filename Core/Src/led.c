/**
  ******************************************************************************
  * @file    led.c
  * @brief   LED control functions
  *
  *          Wraps HAL GPIO operations for the onboard LED (PC13, active-low).
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "led.h"
#include "gpio.h"

/* Functions -----------------------------------------------------------------*/

/**
  * @brief  Turn the onboard LED on.
  * @note   PC13 is active-low — write RESET to sink current and light the LED.
  */
void LED_On(void)
{
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Turn the onboard LED off.
  * @note   Write SET to stop sinking current.
  */
void LED_Off(void)
{
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/**
  * @brief  Toggle the onboard LED state.
  */
void LED_Toggle(void)
{
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}
