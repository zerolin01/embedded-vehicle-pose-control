/**
 * @file dc_motor_control.h
 * @brief 直流电机高级控制函数头文件
 * @author Claude
 * @date 2024-05-25
 *
 * @details
 * 基于EAM20XX电机控制函数库的GM37-520直流电机控制接口
 * 实现基于编码器反馈的闭环速度控制
 */

#ifndef __DC_MOTOR_CONTROL_H__
#define __DC_MOTOR_CONTROL_H__

#include "motor_control.h"
#include "e_gflib.h"      // 通用功能函数库（包含PI控制器等）
#include "e_gdflib.h"     // 数字滤波器库
#include "e_mlib.h"       // 基础数学函数库
#include "RISCV_Typedefs.h" // 数据类型定义
#include "peripherals_supertmr_qd_1_config.h" // 正交编码器配置

// 定义编码器相关常量
#define MOTOR_ENCODER_PPR       11    // 电机编码器每转脉冲数（电机轴）
#define MOTOR_GEAR_RATIO        30    // 电机减速比例
#define ENCODER_COUNTS_PER_REV  (MOTOR_ENCODER_PPR * MOTOR_GEAR_RATIO) // 输出轴每转计数值

// 电机编码器通道定义
#define MOTOR2_ENCODER_INSTANCE 1  // 电机2使用的编码器实例
#define MOTOR3_ENCODER_INSTANCE 2  // 电机3使用的编码器实例

// 使用F32类型进行控制
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
// 速度PI控制器
extern E_GFLIB_CONTROLLER_PIAW_R_T_F32 g_stMotor1SpeedController;
extern E_GFLIB_CONTROLLER_PIAW_R_T_F32 g_stMotor2SpeedController;
extern E_GFLIB_CONTROLLER_PIAW_R_T_F32 g_stMotor3SpeedController;
extern E_GFLIB_CONTROLLER_PIAW_R_T_F32 g_stMotor4SpeedController;

// 低通滤波器（用于速度滤波）
extern E_GDFLIB_FILTER_IIR1_T_F32 g_stMotor1SpeedFilter;
extern E_GDFLIB_FILTER_IIR1_T_F32 g_stMotor2SpeedFilter;
extern E_GDFLIB_FILTER_IIR1_T_F32 g_stMotor3SpeedFilter;
extern E_GDFLIB_FILTER_IIR1_T_F32 g_stMotor4SpeedFilter;

// 电机速度和参考速度
extern float32_t g_af32MotorSpeeds[4];     // 实际速度（滤波后）
extern float32_t g_af32MotorSpeedRefs[4];  // 参考速度
extern float32_t g_af32RawMotorSpeeds[4];  // 原始速度（未滤波）

// 编码器计数值
extern uint16_t g_au16EncoderCounts[4];    // 当前编码器计数值
extern uint16_t g_au16LastEncoderCounts[4]; // 上次编码器计数值
#endif

// 函数声明
// 初始化函数
void DCMotor_Init(void);
void DCMotor_InitEncoders(void);

// 系统时间更新
void DCMotor_UpdateSystemTime(uint32_t deltaMs);

// 速度控制接口
void DCMotor_SetSpeedRef(uint8_t motorIndex, float32_t speedRef);
void DCMotor_UpdateEncoderCounts(void);
void DCMotor_CalculateSpeeds(void);
void DCMotor_SpeedControl(uint8_t motorIndex);
void DCMotor_UpdateAllMotors(void);
float32_t DCMotor_GetMotorSpeed(uint8_t motorIndex);

// 运动控制接口
void DCMotor_MoveForward(float32_t speed);
void DCMotor_MoveBackward(float32_t speed);
void DCMotor_TurnLeft(float32_t speed);
void DCMotor_TurnRight(float32_t speed);
void DCMotor_Stop(void);

#endif /* __DC_MOTOR_CONTROL_H__ */ 