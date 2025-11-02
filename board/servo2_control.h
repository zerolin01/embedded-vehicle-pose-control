/**
 * @file servo2_control.h
 * @brief 第二个SG90舵机控制接口 - 使用GPIO模拟PWM
 * @author 林木@江南大学
 * @date 
 *
 * @details
 * 提供第二个SG90舵机控制的基本功能接口
 * 使用PORTC4 GPIO引脚通过软件模拟PWM控制舵机角度
 * 由于硬件PWM资源已用完，采用GPIO翻转高低电平的方式模拟PWM信号
 */

#ifndef __SERVO2_CONTROL_H__
#define __SERVO2_CONTROL_H__

#include "pins_driver.h"
#include "pin_config.h"
#include "basic_api.h"
#include <stdint.h>
#include <stdbool.h>

// 舵机PWM参数定义 (20ms周期，50Hz频率)
#define SERVO2_PERIOD_US 20000U         // 20ms周期
#define SERVO2_MIN_PULSE_US 500U        // 0.5ms最小脉宽 (0度)
#define SERVO2_MAX_PULSE_US 2500U       // 2.5ms最大脉宽 (180度)
#define SERVO2_CENTER_PULSE_US 1500U    // 1.5ms中心脉宽 (90度)

// 舵机角度定义
#define SERVO2_ANGLE_MIN 0              // 最小角度
#define SERVO2_ANGLE_MAX 180            // 最大角度
#define SERVO2_ANGLE_CENTER 90          // 中心角度

// 预定义角度的脉宽值
#define SERVO2_ANGLE_0_PULSE_US SERVO2_MIN_PULSE_US    // 0度对应脉宽
#define SERVO2_ANGLE_90_PULSE_US SERVO2_CENTER_PULSE_US  // 90度对应脉宽
#define SERVO2_ANGLE_180_PULSE_US SERVO2_MAX_PULSE_US // 180度对应脉宽

// 软件PWM控制参数
#define SERVO2_PWM_CYCLES 50            // 每次角度设置发送的PWM周期数（增强兼容性）
#define SERVO2_DELAY_STEP_US 10U        // 延时步长(微秒)

/**
 * @brief 舵机状态枚举
 */
typedef enum {
    SERVO2_STATE_IDLE = 0,      // 空闲状态
    SERVO2_STATE_MOVING,        // 运动状态
    SERVO2_STATE_ERROR          // 错误状态
} servo2_state_t;

/**
 * @brief 舵机控制结构体
 */
typedef struct {
    uint16_t current_angle;     // 当前角度
    uint16_t target_angle;      // 目标角度
    uint32_t current_pulse_us;  // 当前脉宽(微秒)
    servo2_state_t state;       // 舵机状态
    bool is_initialized;        // 是否已初始化
} servo2_control_t;

/**
 * @brief 将角度转换为脉宽(微秒)
 * @param angle 舵机角度(0-180度)
 * @return 对应的脉宽值(微秒)
 */
uint32_t servo2_angle_to_pulse_us(uint16_t angle);

/**
 * @brief 发送单个PWM脉冲
 * @param pulse_width_us 脉宽(微秒)
 */
void servo2_send_pulse(uint32_t pulse_width_us);

/**
 * @brief 发送多个PWM周期
 * @param pulse_width_us 脉宽(微秒)
 * @param cycles 发送的周期数
 */
void servo2_send_pwm_cycles(uint32_t pulse_width_us, uint8_t cycles);

/**
 * @brief 设置舵机角度
 * @param angle 舵机角度(0-180度)
 */
void servo2_set_angle(uint16_t angle);

/**
 * @brief 舵机左转90度 (从当前位置转到0度)
 */
void servo2_turn_left_90(void);

/**
 * @brief 舵机右转90度 (从当前位置转到180度)
 */
void servo2_turn_right_90(void);

/**
 * @brief 舵机转到中心位置 (90度)
 */
void servo2_turn_center(void);

/**
 * @brief 初始化第二个舵机
 */
void servo2_init(void);

/**
 * @brief 获取舵机当前角度
 * @return 当前角度值
 */
uint16_t servo2_get_current_angle(void);

/**
 * @brief 获取舵机状态
 * @return 舵机状态
 */
servo2_state_t servo2_get_state(void);

/**
 * @brief 舵机演示函数 - 0度到180度往返运动
 */
void servo2_demo(void);

/**
 * @brief 精确延时函数(微秒级)
 * @param us 延时时间(微秒)
 */
void servo2_delay_us(uint32_t us);

/**
 * @brief 在指定毫秒内持续保持当前角度的PWM输出
 * @param ms 持续时间(毫秒)
 */
void servo2_hold_ms(uint32_t ms);

/**
 * @brief 顺时针旋转指定角度
 * @param degrees 旋转角度（0-180度）
 */
void servo2_rotate_clockwise(uint16_t degrees);

/**
 * @brief 逆时针旋转指定角度
 * @param degrees 旋转角度（0-180度）
 */
void servo2_rotate_counterclockwise(uint16_t degrees);

/**
 * @brief 相对角度旋转（正数顺时针，负数逆时针）
 * @param degrees 旋转角度（正数顺时针，负数逆时针）
 */
void servo2_rotate_relative(int16_t degrees);

/**
 * @brief 连续旋转控制（适用于SG90连续旋转舵机）
 * @param speed 旋转速度（-100到100，0为停止，正数顺时针，负数逆时针）
 */
void servo2_rotate_continuous(int16_t speed);

/**
 * @brief 基于时间的相对角度旋转（适用于连续旋转舵机）
 * @param degrees 旋转角度（正数顺时针，负数逆时针）
 */
void servo2_rotate_relative_time(int16_t degrees);

#endif /* __SERVO2_CONTROL_H__ */
