# 克隆版 ST-Link 烧录方案

> 来源: test2_LED | 2026-06-17

## 问题

克隆版 ST-Link 在 VSCode 中烧录报错：
```
error 138: could not connect
```
Keil（取消 Debug Description → Enable）可以烧录，但 VSCode 不行。

## 解决方案

STM32CubeProgrammer CLI 加 `connectUnderReset` 参数，复位状态下连接绕过克隆检测：

```powershell
"C:\Users\Admin\AppData\Local\stm32cube\bundles\programmer\2.22.0+st.1\bin\STM32_Programmer_CLI.exe" -c port=SWD connectUnderReset -w build\Debug\<项目名>.elf -v -rst
```

工具是 STM32Cube VSCode 扩展自带的，无需额外安装。

## VSCode tasks.json（Ctrl+Shift+B 一键烧录）

```json
{
    "version": "2.0.0",
    "tasks": [{
        "label": "烧录 STM32",
        "type": "shell",
        "command": "C:\\Users\\Admin\\AppData\\Local\\stm32cube\\bundles\\programmer\\2.22.0+st.1\\bin\\STM32_Programmer_CLI.exe",
        "args": ["-c", "port=SWD", "connectUnderReset", "-w", "${workspaceFolder}\\build\\Debug\\<项目名>.elf", "-v", "-rst"],
        "group": { "kind": "build", "isDefault": true }
    }]
}
```

## 踩过的坑

- st-flash（开源工具）Windows 发布包不带 `libusb-1.0.dll`，64位/32位DLL不兼容
- 烧录前关闭 Keil，否则 ST-Link 被占用
- `connectUnderReset` 必须跟在 `-c port=SWD` 后面
