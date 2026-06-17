# STM32F103C8T6 (Blue Pill) 项目开发指南

## 本项目专属配置

### 环境
- **芯片**: STM32F103C8T6, LQFP48, 128KB Flash
- **工具链**: STM32CubeMX 6.17.0 + HAL FW_F1 V1.8.7 + GCC 14.3.1 + CMake+Ninja
- **调试器**: 克隆版 ST-Link (SWD: PA13=SWDIO, PA14=SWCLK)
- **时钟**: HSE 8MHz → PLL×9 → 72MHz

### 项目引脚
| 引脚 | 功能 |
|------|------|
| PC13 | LED (低电平点亮) |
| PA13 | SWDIO |
| PA14 | SWCLK |
| PD0 | OSC_IN (8MHz) |
| PD1 | OSC_OUT |

### 模块文件
| 文件 | 职责 |
|------|------|
| `Core/Src/gpio.c` | GPIO 初始化 (PC13 推挽输出) |
| `Core/Src/led.c` | LED 控制抽象层 |
| `Core/Src/tim.c` | TIM2 0.5s 定时器 + 中断回调 |
| `Core/Src/main.c` | 仅业务逻辑 |

### TIM2 参数 (0.5s)
```
PSC=7199, ARR=4999 → 72MHz/7200=10kHz, 5000/10kHz=0.5s
```

---

## 通用技能

以下问题有现成方案，详见 `skills/` 文件夹：

| 问题 | 查阅 |
|------|------|
| 克隆版 ST-Link 烧录报 error 138 | `skills/clone-stlink-flash.md` |
| HAL 驱动文件缺失编译报错 | `skills/hal-missing-drivers.md` |
| Blue Pill 硬件参数/时钟 | `skills/bluepill-hardware.md` |
| CMake 添加新源文件 | `skills/cmake-project-setup.md` |
| 项目总结/生成新技能 | `skills/_project_summary_skill.md` |

### 烧录快捷命令
```powershell
# Ctrl+Shift+B 一键烧录，或终端执行:
"C:\Users\Admin\AppData\Local\stm32cube\bundles\programmer\2.22.0+st.1\bin\STM32_Programmer_CLI.exe" -c port=SWD connectUnderReset -w build\Debug\test2_LED.elf -v -rst
```
