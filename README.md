# STM32F103C8T6 LED 闪烁测试项目 (TIM 定时器版)

## 项目概述

本项目基于 STM32F103C8T6 (Blue Pill) 开发板，使用 STM32CubeMX + HAL 库 + CMake + VSCode 开发环境，实现板载 LED (PC13) 以 0.5 秒频率精确闪烁。定时采用 TIM2 硬件定时器中断方式，代码按功能模块拆分。

## 硬件配置

### MCU 型号
- **主控芯片**: STM32F103C8T6
- **封装**: LQFP48
- **内核**: ARM Cortex-M3
- **主频**: 72MHz
- **Flash**: 128KB
- **RAM**: 20KB

### 引脚配置
| 引脚 | 功能 | 说明 |
|------|------|------|
| PC13 | LED | 板载 LED，低电平点亮 (active-low) |
| PA13 | SWDIO | SWD 调试数据线 |
| PA14 | SWCLK | SWD 调试时钟线 |
| PD0 | OSC_IN | 外部 8MHz 晶振输入 |
| PD1 | OSC_OUT | 外部 8MHz 晶振输出 |

### LED 控制逻辑
- **低电平点亮**: `GPIO_PIN_RESET` → LED 亮
- **高电平熄灭**: `GPIO_PIN_SET` → LED 灭
- PC13 属于备份域，最大输出速度仅 2MHz (GPIO_SPEED_FREQ_LOW)

## 软件架构

### 开发环境
- **IDE**: VSCode + CMake
- **编译器**: arm-none-eabi-gcc (GNU Tools for STM32 14.3.1)
- **HAL 库**: STM32Cube FW_F1 V1.8.7
- **配置工具**: STM32CubeMX 6.17.0
- **构建系统**: CMake + Ninja

### 项目结构
```
test2_LED/
├── Core/
│   ├── Inc/                       # 头文件
│   │   ├── main.h                 # 主头文件
│   │   ├── gpio.h                 # GPIO 配置声明 + LED 引脚宏定义
│   │   ├── led.h                  # LED 控制函数声明
│   │   ├── tim.h                  # TIM2 定时器声明
│   │   ├── stm32f1xx_it.h        # 中断声明
│   │   └── stm32f1xx_hal_conf.h  # HAL 库模块配置
│   └── Src/                       # 源代码
│       ├── main.c                 # 主程序（仅业务逻辑）
│       ├── gpio.c                 # GPIO 引脚初始化 (PC13)
│       ├── led.c                  # LED 控制函数实现（硬件抽象层）
│       ├── tim.c                  # TIM2 定时器配置 + 中断回调
│       ├── stm32f1xx_it.c        # 中断服务函数
│       ├── stm32f1xx_hal_msp.c   # MSP 初始化
│       ├── system_stm32f1xx.c     # 系统时钟配置
│       ├── sysmem.c               # 内存管理
│       └── syscalls.c             # 系统调用
├── Drivers/                       # HAL 驱动库
│   ├── CMSIS/                     # ARM CMSIS 核心
│   └── STM32F1xx_HAL_Driver/     # STM32F1 HAL 驱动
├── cmake/                         # CMake 配置
│   ├── gcc-arm-none-eabi.cmake    # 交叉编译工具链
│   └── stm32cubemx/               # CubeMX 生成的 CMake
│       └── CMakeLists.txt         # 源文件列表
├── skills/                        # 辅助技能
│   ├── README.md                  # 技能使用说明
│   └── stlink_flash_stm32.json   # ST-Link 烧录技能
├── .vscode/                       # VSCode 配置
│   ├── launch.json                # 调试启动配置
│   ├── tasks.json                 # 烧录任务（Ctrl+Shift+B）
│   └── settings.json              # CMake 设置
├── test2_LED.ioc                  # CubeMX 配置文件
├── CMakeLists.txt                 # 顶层 CMake
├── CMakePresets.json              # CMake 预设
├── CLAUDE.md                      # Claude Code 开发指南
└── README.md                      # 本说明文档
```

### 分层架构
```
main.c      ← 业务逻辑层：检测 tim_flag → LED_Toggle()
  │
  ├── led.c     ← 硬件抽象层：LED_On / LED_Off / LED_Toggle
  │     └── 调用 HAL_GPIO_WritePin / HAL_GPIO_TogglePin
  │
  ├── tim.c     ← 定时器层：TIM2 配置 (0.5s) + 中断回调置 flag
  │     └── HAL_TIM_PeriodElapsedCallback → tim_flag = 1
  │
  └── gpio.c    ← 硬件初始化层：PC13 推挽输出配置
```

## 功能说明

### TIM2 定时器参数

```
TIM2 挂载 APB1，APB1 = 36MHz，分频系数 ≠ 1
→ TIM2 时钟 = 36MHz × 2 = 72MHz

预分频器 (PSC) = 7200 - 1 = 7199  →  计数频率 = 72MHz / 7200 = 10kHz
自动重载 (ARR)  = 5000 - 1 = 4999  →  溢出周期 = 5000 / 10kHz = 0.5s
```

### 中断链路
```
TIM2 硬件计数器溢出 (0.5s)
  → TIM2_IRQHandler (stm32f1xx_it.c)
    → HAL_TIM_IRQHandler(&htim2)
      → HAL_TIM_PeriodElapsedCallback (tim.c)
        → tim_flag = 1
          → main.c while(1) 检测到 flag
            → LED_Toggle() → PC13 翻转 → LED 闪烁
```

### 主程序流程
1. **系统初始化**: HAL_Init → SystemClock_Config (72MHz) → MX_GPIO_Init → MX_TIM2_Init
2. **启动定时器**: HAL_TIM_Base_Start_IT(&htim2)
3. **主循环**: 检测 tim_flag → 清零 → LED_Toggle → 继续等待

## 时钟配置

| 时钟域 | 频率 | 来源 |
|--------|------|------|
| HSE | 8MHz | 外部晶振 |
| PLL | 72MHz | HSE × 9 |
| SYSCLK | 72MHz | PLLCLK |
| AHB (HCLK) | 72MHz | ÷1 |
| APB1 | 36MHz | ÷2 |
| APB2 | 72MHz | ÷1 |
| TIM2 | 72MHz | APB1 × 2 |

## 编译与烧录

### 编译
使用 VSCode CMake 扩展自动构建，输出文件在 `build/Debug/`。

**关键文件**:
- ELF 文件: `build/Debug/test2_LED.elf`
- MAP 文件: `build/Debug/test2_LED.map`

### 烧录（克隆版 ST-Link）

> ⚠️ **克隆版 ST-Link 用户必读**: 必须使用 `connectUnderReset` 模式，详见 `skills/` 文件夹。

**快捷方式**: 按 `Ctrl+Shift+B` 一键烧录，或执行：

```powershell
# 使用 STM32CubeProgrammer CLI（已内置在 STM32Cube 工具链中）
STM32_Programmer_CLI.exe -c port=SWD connectUnderReset -w build/Debug/test2_LED.elf -v -rst
```

参数说明:
- `port=SWD` — SWD 接口
- `connectUnderReset` — 复位下连接（**克隆版关键参数**）
- `-w test2_LED.elf` — 写入固件
- `-v` — 烧录后校验
- `-rst` — 烧录完复位运行

## 技能说明 (Skills)

项目 `skills/` 文件夹提供了 **ST-Link 烧录技能**，专门解决克隆版 ST-Link 的烧录问题。

详见 [skills/README.md](skills/README.md)。

支持的操作：
- 烧录固件 (flash)
- 擦除芯片 (erase)
- 校验固件 (verify)
- 复位 MCU (reset)
- 构建并烧录 (build_and_flash)
- 完全重烧 (full_flash)
- 读取芯片信息 (read_info)
- 检查连接状态 (status)

## 常见问题

### Q: 编译报 `stm32f1xx_hal_tim.h: No such file or directory`
A: CubeMX 未生成 TIM 驱动文件，需要从 Cube 仓库手动复制，详见 [CLAUDE.md](CLAUDE.md)。

### Q: 烧录报 `error 138: could not connect`
A: 克隆版 ST-Link 问题，使用 `connectUnderReset` 模式。详见 [skills/README.md](skills/README.md)。

### Q: LED 不闪烁
A: 检查以下几点：
1. 确认烧录成功后 MCU 已复位（-rst 参数）
2. 确认 PC13 引脚配置正确（推挽输出、低电平点亮）
3. 确认 TIM2 中断已启动（`HAL_TIM_Base_Start_IT`）

## 扩展指南

### 添加新外设
1. 使用 STM32CubeMX 打开 `test2_LED.ioc` 配置新外设
2. 重新生成代码（保留 USER CODE 区域内的自定义代码）
3. 如需新的 HAL 驱动文件，参考 [stm32-hal-missing-drivers 处理流程](CLAUDE.md#1-hal-驱动文件缺失)
4. 在 `cmake/stm32cubemx/CMakeLists.txt` 中添加新源文件

### 修改闪烁频率
修改 `Core/Src/tim.c` 中的 TIM2 参数：
```c
htim2.Init.Prescaler = 7199;   // 修改预分频器
htim2.Init.Period    = 4999;   // 修改自动重载值
// 周期 = (PSC+1) × (ARR+1) / 72MHz
```

### 添加新功能模块
1. 创建 `Core/Inc/新模块.h` 和 `Core/Src/新模块.c`
2. 在 `main.c` 中 `#include "新模块.h"`
3. 在 `cmake/stm32cubemx/CMakeLists.txt` 的 `MX_Application_Src` 中添加源文件

## 版本信息

- **创建日期**: 2026-06
- **HAL 库版本**: STM32Cube FW_F1 V1.8.7
- **CubeMX 版本**: 6.17.0
- **编译器**: arm-none-eabi-gcc 14.3.1

## 许可证

本项目遵循 STMicroelectronics BSD 3-Clause 许可证。
