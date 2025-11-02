/**
 * Copyright Statement:
 * This software and related documentation (ESWIN SOFTWARE) are protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * Beijing ESWIN Computing Technology Co., Ltd.(ESWIN)and/or its licensors.
 * Without the prior written permission of ESWIN and/or its licensors, any reproduction, modification,
 * use or disclosure Software, and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * Copyright ©[2023] [Beijing ESWIN Computing Technology Co., Ltd.]. All rights reserved.
 *
 * @file motor_control.h
 * @brief 电机控制底层函数头文件
 * @author abu (abu@eswincomputing.com)
 * @date 2023-02-06
 *
 * @details
 * 麦克纳姆轮小车电机底层控制函数声明
 */

#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

#include "sdk_project_config.h"
// 使用pins_driver.h中的类型定义而不是stdint.h
#include "pins_driver.h"

// 定义电机1控制引脚（右后轮）
#define MOTOR1_STBY_PORT PORTB
#define MOTOR1_STBY_PIN  11U
#define MOTOR1_AIN1_PORT PORTB
#define MOTOR1_AIN1_PIN  13U
#define MOTOR1_AIN2_PORT PORTB
#define MOTOR1_AIN2_PIN  14U

// 定义电机2控制引脚（右前轮）
#define MOTOR2_STBY_PORT PORTB
#define MOTOR2_STBY_PIN  0U
#define MOTOR2_AIN1_PORT PORTB
#define MOTOR2_AIN1_PIN  1U
#define MOTOR2_AIN2_PORT PORTB
#define MOTOR2_AIN2_PIN  2U

// 定义电机3控制引脚（左前轮）
#define MOTOR3_STBY_PORT PORTB
#define MOTOR3_STBY_PIN  4U
#define MOTOR3_AIN1_PORT PORTB
#define MOTOR3_AIN1_PIN  5U
#define MOTOR3_AIN2_PORT PORTB
#define MOTOR3_AIN2_PIN  7U

// 定义电机4控制引脚（左后轮）
#define MOTOR4_STBY_PORT PORTB
#define MOTOR4_STBY_PIN  8U
#define MOTOR4_AIN1_PORT PORTB
#define MOTOR4_AIN1_PIN  25U
#define MOTOR4_AIN2_PORT PORTB
#define MOTOR4_AIN2_PIN  26U

// 定义电机速度
#define MOTOR_DEFAULT_DUTY 0x8000
#define MOTOR_SPEED_HIGH   0xC000  // 约75%占空比，高速
#define MOTOR_SPEED_MED    0x8000  // 约50%占空比，中速
#define MOTOR_SPEED_LOW    0x4000  // 约25%占空比，低速

// 定义电机方向
#define FORWARD 0
#define BACKWARD 1

/**
 * @brief 初始化所有PWM通道
 */
void InitAllPWM(void);

/**
 * @brief 设置电机1（右后轮）方向和启动
 * @param direction 方向：0-前进，1-后退
 */
void SetMotor1Direction(uint8_t direction);

/**
 * @brief 设置电机2（右前轮）方向和启动
 * @param direction 方向：0-前进，1-后退
 */
void SetMotor2Direction(uint8_t direction);

/**
 * @brief 设置电机3（左前轮）方向和启动
 * @param direction 方向：0-前进，1-后退
 */
void SetMotor3Direction(uint8_t direction);

/**
 * @brief 设置电机4（左后轮）方向和启动
 * @param direction 方向：0-前进，1-后退
 */
void SetMotor4Direction(uint8_t direction);

/**
 * @brief 设置电机1速度
 * @param duty PWM占空比，范围0-0xFFFF
 */
void SetMotor1Speed(uint16_t duty);

/**
 * @brief 设置电机2速度
 * @param duty PWM占空比，范围0-0xFFFF
 */
void SetMotor2Speed(uint16_t duty);

/**
 * @brief 设置电机3速度
 * @param duty PWM占空比，范围0-0xFFFF
 */
void SetMotor3Speed(uint16_t duty);

/**
 * @brief 设置电机4速度
 * @param duty PWM占空比，范围0-0xFFFF
 */
void SetMotor4Speed(uint16_t duty);

/**
 * @brief 停止所有电机
 */
void StopAllMotors(void);

/**
 * @brief 直行功能
 * 所有电机向前转动，速度一致
 */
void MoveStraight(void);

/**
 * @brief 左转功能
 * 左侧电机减速或反转，右侧电机正向转动
 */
void TurnLeft(void);

/**
 * @brief 右转功能
 * 右侧电机减速或反转，左侧电机正向转动
 */
void TurnRight(void);

#endif /* __MOTOR_CONTROL_H__ */ 