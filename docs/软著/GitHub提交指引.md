## GitHub 提交指引（软著精简版）

### 提交前准备
1. **版本标识**
   - 在仓库打上版本标签：`git tag -a v1.0 -m "软著提交版本 - 姿态矫正控制系统"`
   - 确保提交时间与软著材料中的"版本生成时间"一致

2. **检查精简结果**
   - 已删除冗余模块：`pose.*`、`motion_controller.*`、`uarttouart.*`、`move.*`、`map.*`
   - 保留核心：`mpu6050.*`（H30）、`my_move.*`、电机/舵机、避障、`main.c`
   - `src/main.c` 已精简至 119 行（仅 `system_init` + `nb` + `main`）

### .gitignore 建议（若尚未配置）
```gitignore
# 编译输出
Debug/
*.o
*.elf
*.bin
*.hex
*.map
*.lst

# IDE 配置（按需保留或忽略）
.vscode/
.idea/
*.code-workspace

# 临时文件
*.log
*.tmp
*.bak

# 软著生成物（可选：若不想提交合并稿）
docs/软著/源代码选编/selected_code.txt
docs/软著/提交代码版本/code/

# 系统文件
.DS_Store
Thumbs.db
```

### 提交命令示例
```bash
# 1. 添加精简后的文件
git add board/ src/ docs/

# 2. 提交精简版本
git commit -m "refactor: 精简项目至软著提交版本 V1.0

- 删除未使用的视觉接口与旧版运动控制模块
- 保留核心：H30姿态、my_move运动控制、电机/舵机、避障
- 简化 main.c 至 nb() 任务流程
- 更新软著材料（源代码选编清单、说明书）"

# 3. 打版本标签
git tag -a v1.0 -m "软著提交版本：嵌入式移动小车姿态矫正控制软件 V1.0"

# 4. 推送至 GitHub
git push origin main
git push origin v1.0
```

### README.md 建议内容（项目根目录）
```markdown
# 嵌入式移动小车姿态矫正控制软件 V1.0

基于 H30 惯性模块的移动小车姿态估计与航向保持控制系统。

## 功能特性
- H30 I2C 欧拉角/角速度采集与死区抑制
- 直行航向保持（PID 差速纠偏 + 振荡检测 + 大误差处理）
- 定角转弯（带舵机联动，保持物品相对地面静止）
- 超声波避障（6cm 阈值，连续 3 次命中停车等待）
- 四轮 520 电机驱动与双舵机控制

## 硬件平台
- 主控：EAM2011 开发板
- 姿态模块：H30（I2C，自适应地址探测 0x35/0x6A/0x6B）
- 电机：四轮 520 直流电机
- 执行器：2 路舵机（servo/servo2）
- 传感器：HC-SR04 超声波测距

## 快速开始
1. 硬件连接：按 `board/pin_config.h` 配置引脚
2. 编译：使用配套 SDK 编译工程
3. 运行：上电后执行 `nb()` 任务流程（直行+转弯+避障）

## 核心代码结构
```
board/
├── h30.c|h             # H30 惯性姿态模块接口
├── my_move.c|h         # 运动控制（直行/转弯/避障）
├── dc_motor_control.*  # 四轮电机驱动
├── motor_control.*     # 电机底层控制
├── servo_control.*     # 舵机1控制
├── servo2_control.*    # 舵机2控制
├── hcsr04.c|h          # 超声波避障
└── board_delay.c|h     # 延时工具
src/
└── main.c              # 系统初始化 + nb() 任务
```

## 软件著作权
- 版本：V1.0
- 说明：本仓库为软著提交版本，已移除未使用的视觉接口与旧版运动控制模块
- 材料：见 `docs/软著/`

## 许可证
（按你的实际需求添加，如 MIT / Apache-2.0 / 专有）
```

### 提交后验证
- 访问 GitHub 仓库，确认文件结构与精简版一致
- 检查 Release 页面是否显示 `v1.0` 标签
- 下载 Release 压缩包，用于软著附件提交

### 软著材料准备（基于 GitHub 版本）
1. **源代码选编**：运行 `docs/软著/源代码选编/build_selected_code.ps1` 生成 `selected_code.txt`
2. **软件说明书**：`docs/软著/软件说明书.md` 转 PDF
3. **权属证明**：GitHub 仓库链接 + Commit 时间戳 + Release 下载链接可作为辅助证据
4. **版本固化**：将 `v1.0` Release 的压缩包（.zip）存档，确保与提交材料一致

### 注意事项
- **版本号一致性**：软著申请表、说明书、代码头注释、Git 标签均应为 `V1.0`
- **时间戳**：Git commit 时间应与材料中"版本生成时间"一致
- **分支管理**（建议）：在 `main` 之外创建 `software-copyright-v1.0` 分支锁定提交版本，便于后续开发与维护

