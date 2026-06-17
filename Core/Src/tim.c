/**
  ******************************************************************************
  * @file    tim.c
  * @brief   TIM2定时器配置 — 0.5秒周期中断，驱动LED闪烁。
  *
  *          时钟参数：
  *            预分频器(PSC) = 7200 - 1 = 7199  →  72MHz ÷ 7200 = 10kHz计数频率
  *            自动重载(ARR) = 5000 - 1 = 4999  →  5000次 ÷ 10kHz = 0.5秒溢出
  *
  *          中断链路：
  *            TIM2溢出 → TIM2_IRQHandler → HAL_TIM_IRQHandler
  *            → HAL_TIM_PeriodElapsedCallback → 置位 tim_flag
  ******************************************************************************
  */

/* 头文件引用 ----------------------------------------------------------------*/
#include "tim.h"

/* 私有变量 ------------------------------------------------------------------*/

/** TIM2句柄，供 HAL 库和中断服务函数使用 */
TIM_HandleTypeDef htim2;

/* 函数实现 ------------------------------------------------------------------*/

/**
  * @brief  配置TIM2产生0.5秒周期中断。
  * @note   启用TIM2时钟 → 配置定时参数 → 调用HAL初始化 → 配置NVIC中断优先级。
  */
void MX_TIM2_Init(void)
{
  /* 使能TIM2外设时钟 */
  __HAL_RCC_TIM2_CLK_ENABLE();

  /* 配置TIM2基本定时参数 */
  htim2.Instance               = TIM2;                             /* TIM2外设              */
  htim2.Init.Prescaler         = 7199;                            /* 72MHz / 7200 = 10kHz  */
  htim2.Init.Period            = 4999;                            /* 5000次 → 0.5秒       */
  htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;              /* 向上计数              */
  htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          /* 时钟不分频            */
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  /* 立即更新ARR           */

  /* HAL库初始化TIM2（内部会调用 HAL_TIM_Base_MspInit） */
  HAL_TIM_Base_Init(&htim2);

  /* 配置NVIC：TIM2中断优先级0，使能中断 */
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/**
  * @brief  TIM2 MSP初始化（MCU Support Package）。
  * @note   由 HAL_TIM_Base_Init() 回调，负责使能外设时钟。
  * @param  htim 定时器句柄指针
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    __HAL_RCC_TIM2_CLK_ENABLE();
  }
}

/**
  * @brief  定时器溢出回调函数。
  * @note   由 HAL_TIM_IRQHandler 调用。每0.5秒触发一次，
  *          将 tim_flag 置1，通知主循环执行闪烁操作。
  * @param  htim 触发回调的定时器句柄指针
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    tim_flag = 1;   /* 通知主循环：0.5秒已到 */
  }
}
