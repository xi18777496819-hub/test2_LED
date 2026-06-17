# HAL 驱动文件缺失修复

> 来源: test2_LED | 2026-06-17

## 问题

CubeMX 只拷贝 `.ioc` 中配置的外设驱动。编译报错：
```
fatal error: stm32f1xx_hal_xxx.h: No such file or directory
```

## 修复步骤

**1. 复制驱动文件**（从 Cube 仓库到项目）

仓库路径: `C:\Users\Admin\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Drivers\`

```bash
# 复制 .c 和 .h 到项目对应目录
cp <仓库>/Src/stm32f1xx_hal_xxx.c   → Drivers/STM32F1xx_HAL_Driver/Src/
cp <仓库>/Inc/stm32f1xx_hal_xxx.h   → Drivers/STM32F1xx_HAL_Driver/Inc/
cp <仓库>/Inc/stm32f1xx_hal_xxx_ex.h → Drivers/STM32F1xx_HAL_Driver/Inc/  # 如果有
```

以 TIM 为例需复制 4 个文件（含 _ex.c 和 _ex.h）。

**2. 启用模块**

`Core/Inc/stm32f1xx_hal_conf.h` 中去掉注释：
```c
#define HAL_XXX_MODULE_ENABLED
```

**3. 更新 CMakeLists.txt**

`cmake/stm32cubemx/CMakeLists.txt` 中：
- `STM32_Drivers_Src`: 加 HAL 驱动 .c 文件
- `MX_Application_Src`: 加用户新增 .c 文件

## 踩过的坑

- 别漏了 `_ex.c` 扩展文件
- CMakeLists.txt 两处都要改
- CubeMX 重新生成不会自动添加这些文件
