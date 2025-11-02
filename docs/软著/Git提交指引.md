# Git 提交指引 - 软著版本 V1.0

## 📋 提交前检查清单

- [x] 代码精简完成（已删除 pose/motion_controller/uarttouart/move/map）
- [x] H30 重命名完成（mpu6050 → h30）
- [x] 所有核心文件添加注释（作者：林木，欲得必行，文明人@江南大学）
- [x] .gitignore 已创建
- [x] README.md 已创建
- [x] 软著材料已准备（docs/软著/）

## 🚀 提交步骤（首次提交）

### 1. 初始化 Git 仓库（如果尚未初始化）

```bash
cd "E:\Test\软著\最终演示代码\Final"
git init
```

### 2. 配置 Git 用户信息

```bash
git config user.name "林木"
git config user.email "your-email@example.com"  # 替换为你的邮箱
```

### 3. 查看文件状态

```bash
git status
```

### 4. 添加所有文件到暂存区

```bash
# 添加所有文件（.gitignore 会自动排除不需要的文件）
git add .

# 或者分步添加（更安全）
git add board/ src/ docs/ ESWIN_SDK/
git add .gitignore README.md project.ect
git add Debug/makefile Debug/sources.mk Debug/objects.mk
```

### 5. 查看暂存区文件

```bash
git status
```

预期输出：
```
On branch main
Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
        new file:   .gitignore
        new file:   README.md
        new file:   board/h30.c
        new file:   board/h30.h
        new file:   board/my_move.c
        new file:   board/my_move.h
        ...
```

### 6. 提交到本地仓库

```bash
git commit -m "feat: 初始化软著版本 V1.0 - 姿态矫正控制系统

- 实现 H30 惯性姿态模块驱动（I2C 通信、欧拉角读取）
- 实现航向保持控制（PID 差速纠偏、振荡检测、大误差处理）
- 实现定角转弯（原地旋转、舵机联动）
- 实现超声波避障（HC-SR04、连续检测、去抖处理）
- 实现四轮电机控制（520 直流电机、麦克纳姆轮）
- 实现双舵机控制（GPIO 模拟 PWM）
- 删除冗余模块（pose/motion_controller/uarttouart/move/map）
- 添加完整注释（Doxygen 风格）
- 准备软著材料（说明书、源代码选编、材料清单）

作者: 林木，欲得必行，文明人@江南大学
版本: V1.0
日期: 2024-11-02"
```

### 7. 打版本标签

```bash
git tag -a v1.0 -m "软著提交版本：嵌入式移动小车姿态矫正控制软件 V1.0

功能特性:
- H30 惯性姿态模块驱动
- 航向保持与 PID 差速控制
- 定角转弯与舵机联动
- 超声波避障
- 四轮麦克纳姆电机控制
- 双舵机控制

作者: 林木，欲得必行，文明人@江南大学
提交日期: 2024-11-02"
```

### 8. 查看提交历史与标签

```bash
# 查看提交历史
git log --oneline

# 查看标签
git tag

# 查看标签详细信息
git show v1.0
```

## 🌐 推送到 GitHub

### 1. 在 GitHub 创建仓库

1. 访问 https://github.com
2. 点击右上角 "+" → "New repository"
3. 填写仓库信息：
   - Repository name: `embedded-vehicle-pose-control` (或你喜欢的名字)
   - Description: `嵌入式移动小车姿态矫正控制软件 V1.0 - 基于H30惯性姿态模块`
   - Public / Private: 根据需要选择
   - **不要**勾选 "Add a README file"（因为我们已经有了）
4. 点击 "Create repository"

### 2. 添加远程仓库

复制 GitHub 仓库 URL，然后执行：

```bash
# HTTPS 方式
git remote add origin https://github.com/your-username/embedded-vehicle-pose-control.git

# 或 SSH 方式（需要先配置 SSH 密钥）
git remote add origin git@github.com:your-username/embedded-vehicle-pose-control.git
```

### 3. 重命名主分支为 main（如果需要）

```bash
git branch -M main
```

### 4. 推送代码与标签

```bash
# 推送主分支
git push -u origin main

# 推送标签
git push origin v1.0

# 或者一次性推送所有标签
git push origin --tags
```

## 🔍 验证提交结果

### 1. 在 GitHub 上验证

访问你的 GitHub 仓库：
- 检查文件结构是否完整
- 检查 README.md 是否正确显示
- 检查 Releases 页面是否有 v1.0 标签

### 2. 下载 Release 用于软著提交

1. 访问 GitHub 仓库的 "Releases" 页面
2. 找到 v1.0 标签
3. 下载 Source code (zip) 或 Source code (tar.gz)
4. 解压验证文件完整性

## 📝 后续维护（可选）

### 创建软著专用分支

```bash
# 创建并切换到软著分支
git checkout -b software-copyright-v1.0

# 推送到远程
git push -u origin software-copyright-v1.0

# 切回主分支继续开发
git checkout main
```

### 如果需要修改

```bash
# 修改文件后
git add <修改的文件>
git commit -m "fix: 修复描述"

# 如果需要更新 v1.0 标签（谨慎操作）
git tag -d v1.0                    # 删除本地标签
git push origin :refs/tags/v1.0    # 删除远程标签
git tag -a v1.0 -m "更新说明"       # 重新打标签
git push origin v1.0               # 推送新标签
```

## 🎯 常见问题

### Q1: 提示文件过大

**解决方案**：
```bash
# 检查哪些文件过大
git ls-files -s | awk '{print $4, $2}' | sort -n -r | head -20

# 如果是编译产物，添加到 .gitignore 并移除
git rm --cached Debug/*.bin Debug/*.elf
git commit -m "chore: 移除编译产物"
```

### Q2: 推送失败（需要认证）

**解决方案**：
```bash
# 方式1：使用 Personal Access Token
# 1. GitHub Settings → Developer settings → Personal access tokens
# 2. 生成 token，勾选 repo 权限
# 3. 推送时使用 token 作为密码

# 方式2：配置 SSH 密钥（推荐）
ssh-keygen -t ed25519 -C "your-email@example.com"
# 将 ~/.ssh/id_ed25519.pub 添加到 GitHub SSH keys
```

### Q3: 想修改提交信息

**解决方案**：
```bash
# 修改最后一次提交信息
git commit --amend -m "新的提交信息"

# 如果已经推送，需要强制推送（谨慎）
git push -f origin main
```

## 📦 软著材料准备

### 生成源代码选编

```powershell
.\docs\软著\源代码选编\build_selected_code.ps1
```

### 导出提交代码版本

```powershell
.\docs\软著\提交代码版本\export_submit_code.ps1
```

### 软著提交材料清单

1. **软件说明书**：`docs/软著/软件说明书.md` → 转 PDF
2. **源代码选编**：`docs/软著/源代码选编/selected_code.txt` → 转 PDF
3. **权属证明**：
   - GitHub 仓库链接：`https://github.com/your-username/embedded-vehicle-pose-control`
   - Release v1.0 链接
   - Commit 时间戳
4. **其他材料**：按当地版权局要求准备

## ✅ 完成确认

- [ ] Git 仓库初始化完成
- [ ] 代码已提交到本地仓库
- [ ] 版本标签 v1.0 已创建
- [ ] 代码已推送到 GitHub
- [ ] Release v1.0 可下载
- [ ] 软著材料已准备
- [ ] README.md 正确显示

---

**提交日期**: 2024-11-02  
**版本**: V1.0  
**作者**: 林木，欲得必行，文明人@江南大学

