/**
 * @file my_move.h
 * @author 林木@江南大学
 * @brief 高精度运动控制接口 - 基于 H30 姿态闭环
 * @details 提供直行航向保持、定角转弯、避障与差速纠偏功能
 */

#ifndef __MY_MOVE_H__
#define __MY_MOVE_H__

#include "RISCV_Typedefs.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 基础直行/拐弯/停止（基于最底层 motor_control 接口）
void MyMove_Forward(float32_t speed);
void MyMove_TurnLeft(float32_t speed);
void MyMove_TurnRight(float32_t speed);
void MyMove_Stop(void);

// 差速直行接口：base_speed 为基础速度，yaw_corr>0 向右纠偏（右轮更快/左轮更慢）
void MyMove_ForwardWithDiff(float32_t base_speed, float32_t yaw_corr);

// 基于方向角误差的差速直行：yaw_error_deg 为(参考-当前)的角度误差，Kp为比例增益
// 内部计算 yaw_corr = clamp(Kp * yaw_error_deg)
void MyMove_ForwardWithYaw(float32_t base_speed, float32_t yaw_error_deg, float32_t Kp);

// ========================
// 高精度直行/拐弯接口（使用 H30 欧拉航向 yaw）
// 速度标度统一为 0.0~1.0，角度(度)范围规范化至 [-180, 180]
// ========================

// 模式与方向常量
#define MY_MODE_STRAIGHT 0
#define MY_MODE_TURN     1
#define MY_TURN_LEFT     (+1)
#define MY_TURN_RIGHT    (-1)

// 直行PID参数设置（默认：KP=1.0, KI=0.1, KD=0.05，对应伪代码思路，可按机型调整）
void MyMove_SetStraightPID(float32_t Kp, float32_t Ki, float32_t Kd);
// 转向PID参数设置（默认：KP=2.0, KI=0.05, KD=0.1）
void MyMove_SetTurnPID(float32_t Kp, float32_t Ki, float32_t Kd);

// 初始化直行：以当前 H30 欧拉航向 yaw 作为目标航向
void MyMove_StraightInit(void);
// 执行直行闭环：内部读取 H30 yaw，基于 PID 差速纠偏，运行 duration_ms 毫秒
void MyMove_StraightHoldYaw(float32_t base_speed, uint32_t duration_ms);
// 执行直行闭环（带日志）：打印初始航向角与每次采纳的航向角
void MyMove_StraightHoldYawWithLog(float32_t base_speed, uint32_t duration_ms);
// 执行直行闭环（带日志+车轮速度）：打印初始航向、每次航向，以及4个轮子的速度
void MyMove_StraightHoldYawWithLogAndSpeeds(float32_t base_speed, uint32_t duration_ms);

// 执行直行闭环（带避障）：集成超声波避障，连续3次检测到障碍物距离小于5cm时停止
void MyMove_StraightHoldYawWithObstacleAvoidance(float32_t base_speed, uint32_t duration_ms);

// ========================
// 新增：外部设定/获取直行目标，以及使用当前目标的直行变体
// ========================
// 归一化角度到 [-180, 180]
float32_t MyMove_NormalizeDeg(float32_t a);
// 设置直行目标航向（不进行重新采样），并重置内部PID状态
void MyMove_SetStraightTarget(float32_t target_yaw_deg);
// 获取最近一次设定/初始化后的直行目标航向
float32_t MyMove_GetStraightTarget(void);
// 使用当前目标航向执行直行（带避障），不会重新采样初始航向
void MyMove_StraightHoldYawWithObstacleAvoidanceUseTarget(float32_t base_speed, uint32_t duration_ms);

// 初始化转向：direction=MY_TURN_LEFT/MY_TURN_RIGHT，目标为当前yaw±90°
void MyMove_TurnInit(int direction);
// 执行转向闭环：内部读取 H30 yaw，直到 |误差|<=stop_deg 或超时
void MyMove_TurnExecute(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms);
// 便捷：按相对角度转向（正=左转，负=右转）
void MyMove_TurnDelta(float32_t delta_deg, float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms);
// 便捷：左/右转90°
void MyMove_TurnLeft90(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms);
void MyMove_TurnRight90(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms);

// 结合舵机控制的转向函数：小车转向的同时舵机反向转动，保持物品相对地面静止
void MyMove_TurnRight90WithServo(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms, uint16_t servo_angle);

#ifdef __cplusplus
}
#endif

#endif // __MY_MOVE_H__
