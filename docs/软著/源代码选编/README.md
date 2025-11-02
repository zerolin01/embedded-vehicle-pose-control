本目录用于存放“源代码选编”材料。

使用方法：
- 优先运行同目录下的 `build_selected_code.ps1` 生成 `selected_code.txt` 合并稿（含文件分隔与路径）
- 或者逐文件打印 `文件清单.txt` 中列举的源码，确保：
  - 等宽字体、固定行宽（不自动换行）
  - 页眉含软件名/版本；页脚含页码/文件路径
  - 保留缩进和空行

建议选编范围（原创核心）：
- `board/h30.c|h`（H30 惯性姿态模块接入与欧拉角/角速度接口）
- `board/my_move.c|h`（直行航向保持、定角转弯、避障）
- `board/dc_motor_control.*`、`board/motor_control.*`（四轮电机驱动）
- `board/servo_control.*`、`board/servo2_control.*`（舵机控制）
- `board/hcsr04.c|h`（超声波避障）
- `board/board_delay.c|h`（延时工具）
- `src/main.c`（初始化与 `nb()` 任务流程）

