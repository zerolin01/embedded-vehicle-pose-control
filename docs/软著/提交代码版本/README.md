本目录用于生成“提交代码版本”（精简版，仅包含实际使用到的模块与示例主流程），不改动原工程。

包含模块（基于 `src/main.c` 的 `nb()` 实际调用）：
- H30 姿态来源：`board/h30.c|h`
- 运动与航向保持（直行/转弯/避障）：`board/my_move.c|h`
- 四轮 520 电机驱动：`board/dc_motor_control.c|h`、`board/motor_control.c|h`
- 舵机执行器：`board/servo_control.c|h`、`board/servo2_control.c|h`
- 超声波避障：`board/hcsr04.c|h`
- 基础延时：`board/board_delay.c|h`
- 示例主流程：`src/main.c`（`nb()`）

排除模块：视觉接口与姿态二维码矫正（如 `board/pose.*`、`board/uarttouart.*`）、未被 `nb()` 调用的通用姿态控制（如 `board/motion_controller.*`）等。

使用方法：
1. 执行导出脚本，复制上述文件到 `docs/软著/提交代码版本/code/`，保持相对路径结构
2. 将 `code/` 目录作为提交附件或与“源代码选编”一起打包

命令（PowerShell）：
```powershell
./export_submit_code.ps1
```

