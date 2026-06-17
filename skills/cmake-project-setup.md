# CMake STM32 项目配置

> 来源: test2_LED | 2026-06-17

## 添加新源文件

编辑 `cmake/stm32cubemx/CMakeLists.txt`：

**用户代码** → `MX_Application_Src`:
```cmake
${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/新模块.c
```

**HAL 驱动** → `STM32_Drivers_Src`:
```cmake
${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/.../stm32f1xx_hal_xxx.c
```

## 头文件

`Core/Inc/` 已在 `MX_Include_Dirs` 中，新增 .h 放这里即可。

## 注意

- CubeMX 重新生成可能覆盖 CMakeLists.txt 的源文件列表
- 编译输出: `build/Debug/<项目名>.elf`
