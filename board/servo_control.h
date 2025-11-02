/**
 * @file servo_control.h
 * @brief MG90S舵机控制接口 - 使用GPIO模拟PWM
 * @date 2025-09-11
 *
 * @details
 * 提供MG90S舵机控制的基本功能接口
 * 使用GPIO引脚通过软件模拟PWM控制舵机角度
 */

#ifndef __SERVO_CONTROL_H__
#define __SERVO_CONTROL_H__

#include "pins_driver.h"
#include "pin_config.h"
#include "basic_api.h"
#include <stdint.h>
#include <stdbool.h>

// 舵机PWM参数定义 (20ms周期，50Hz频率)
#define SERVO_PERIOD_US 20000U         // 20ms周期
#define SERVO_MIN_PULSE_US 500U        // 0.5ms最小脉宽 (0度)
#define SERVO_MAX_PULSE_US 2500U       // 2.5ms最大脉宽 (180度)
#define SERVO_CENTER_PULSE_US 1500U    // 1.5ms中心脉宽 (90度)

// 舵机角度定义
#define SERVO_ANGLE_MIN 0              // 最小角度
#define SERVO_ANGLE_MAX 180            // 最大角度
#define SERVO_ANGLE_CENTER 90          // 中心角度

// 软件PWM控制参数
#define SERVO_PWM_CYCLES 50            // 每次角度设置发送的PWM周期数（增强兼容性）
#define SERVO_DELAY_STEP_US 10U        // 延时步长(微秒)

// 选择用于servo的GPIO口与引脚（与原PWM4物理引脚一致：PORTD21）
#define SERVO_GPIO_PORT PORTD
#define SERVO_GPIO_PIN  21U

/**
 * @brief 将角度转换为脉宽(微秒)
 * @param angle 舵机角度(0-180度)
 * @return 对应的脉宽值(微秒)
 */
uint32_t servo_angle_to_pulse_us(uint16_t angle);

/**
 * @brief 发送单个PWM脉冲
 * @param pulse_width_us 脉宽(微秒)
 */
void servo_send_pulse(uint32_t pulse_width_us);

/**
 * @brief 发送多个PWM周期
 * @param pulse_width_us 脉宽(微秒)
 * @param cycles 发送的周期数
 */
void servo_send_pwm_cycles(uint32_t pulse_width_us, uint8_t cycles);

/**
 * @brief 设置舵机角度
 * @param angle 舵机角度(0-180度)
 */
void servo_set_angle(uint16_t angle);

/**
 * @brief 初始化舵机
 */
void servo_init(void);

/**
 * @brief 获取舵机当前角度
 * @return 当前角度值
 */
uint16_t servo_get_current_angle(void);

/**
 * @brief 在指定毫秒内持续保持当前角度的PWM输出
 * @param ms 持续时间(毫秒)
 */
void servo_hold_ms(uint32_t ms);

/**
 * @brief 简单延时函数（兼容旧接口）
 * @param ms 延时时间(毫秒)
 */
void delay_ms(uint32_t ms);

/**
 * @brief 顺时针旋转指定角度
 * @param degrees 旋转角度（0-180度）
 */
void servo_rotate_clockwise(uint16_t degrees);

/**
 * @brief 逆时针旋转指定角度
 * @param degrees 旋转角度（0-180度）
 */
void servo_rotate_counterclockwise(uint16_t degrees);

/**
 * @brief 相对角度旋转（正数顺时针，负数逆时针）
 * @param degrees 旋转角度（正数顺时针，负数逆时针）
 */
void servo_rotate_relative(int16_t degrees);

/**
 * @brief 连续旋转控制（适用于SG90连续旋转舵机）
 * @param speed 旋转速度（-100到100，0为停止，正数顺时针，负数逆时针）
 */
void servo_rotate_continuous(int16_t speed);

#endif /* __SERVO_CONTROL_H__ */ 