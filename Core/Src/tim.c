/**
  ******************************************************************************
  * @file    tim.c
  * @brief   TIM2 timer setup — 0.5 s periodic interrupt for LED blinking.
  *
  *          TIM2 is clocked from APB1 timer clock = 72 MHz.
  *            Prescaler = 7200 - 1 = 7199   → 72 MHz / 7200 = 10 kHz
  *            Period    = 5000 - 1 = 4999   → 5000 / 10 kHz  = 0.5 s
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* Functions -----------------------------------------------------------------*/

/**
  * @brief  Configure TIM2 with a 0.5 s period.
  */
void MX_TIM2_Init(void)
{
  __HAL_RCC_TIM2_CLK_ENABLE();

  htim2.Instance               = TIM2;
  htim2.Init.Prescaler         = 7199;
  htim2.Init.Period            = 4999;
  htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  HAL_TIM_Base_Init(&htim2);

  /* Configure NVIC for TIM2 */
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/**
  * @brief  TIM2 MSP (MCU Support Package) — clock enable for the timer
  *         peripheral. Called by HAL_TIM_Base_Init().
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    __HAL_RCC_TIM2_CLK_ENABLE();
  }
}

/**
  * @brief  Timer period-elapsed callback.
  *         Called from TIM2_IRQHandler via HAL_TIM_IRQHandler.
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    tim_flag = 1;
  }
}
