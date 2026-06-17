# Blue Pill (STM32F103C8T6) 硬件参考

> 来源: test2_LED | 2026-06-17

## LED (PC13)

- 低电平点亮 (active-low)，最大输出速度 2MHz
- `GPIO_PIN_RESET` = 亮, `GPIO_PIN_SET` = 灭

## SWD 调试

PA13=SWDIO, PA14=SWCLK, JTAG 已禁用

## 时钟 (HSE 8MHz → 72MHz)

| 时钟 | 频率 | 来源 |
|------|------|------|
| SYSCLK | 72MHz | PLL (HSE×9) |
| AHB | 72MHz | ÷1 |
| APB1 | 36MHz | ÷2 |
| APB2 | 72MHz | ÷1 |
| **TIM2** | **72MHz** | APB1×2 |

## 0.5秒定时器 (TIM2)

```
PSC = 7199 → 72MHz/7200 = 10kHz
ARR = 4999 → 5000/10kHz = 0.5s
```

## 通用公式

```
定时周期 = (PSC+1) × (ARR+1) / TIM_CLK
```
