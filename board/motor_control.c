/**
 * @file motor_control.c
 * @brief 电机控制函数实现
 * @author abu (abu@eswincomputing.com)
 * @date 2023-02-06
 *
 * @details
 * 麦克纳姆轮小车电机控制函数实现
 */

#include "motor_control.h"
#include "peripherals_pwm_multi_config.h"
#include "pins_driver.h"

/**
 * @brief 初始化所有PWM通道
 */
void InitAllPWM(void)
{
    // 初始化PWM2 - 电机1（右后轮）
    g_stPwm2Config0.duty = 0;  // 初始化时占空比为0，避免错误启动
    PWM_DRV_Init(INST_PWM_2, &g_stPwmState_2, &g_stPwm2Config0);
    PWM_DRV_Start(INST_PWM_2);  // 确保启动PWM
    
    // 初始化PWM0 - 电机2（右前轮）
    g_stPwm0Config0.duty = 0;
    PWM_DRV_Init(INST_PWM_0, &g_stPwmState_0, &g_stPwm0Config0);
    PWM_DRV_Start(INST_PWM_0);  // 确保启动PWM
    
    // 初始化PWM1 - 电机3（左前轮）
    g_stPwm1Config0.duty = 0;
    PWM_DRV_Init(INST_PWM_1, &g_stPwmState_1, &g_stPwm1Config0);
    PWM_DRV_Start(INST_PWM_1);  // 确保启动PWM
    
    // 初始化PWM3 - 电机4（左后轮）
    g_stPwm3Config0.duty = 0;
    PWM_DRV_Init(INST_PWM_3, &g_stPwmState_3, &g_stPwm3Config0);
    PWM_DRV_Start(INST_PWM_3);  // 确保启动PWM
}

/**
 * @brief 设置电机1（右后轮）方向和启动
 * @param direction 方向：0-前进，1-后退
 */
void SetMotor1Direction(uint8_t direction)
{
    // 设置TB6612 STBY引脚为1，使能驱动芯片
    PINS_DRV_WritePin(MOTOR1_STBY_PORT, MOTOR1_STBY_PIN, 1);
    
    if (direction == FORWARD) {
        // 设置电机方向 - 前进
        PINS_DRV_WritePin(MOTOR1_AIN1_PORT, MOTOR1_AIN1_PIN, 1);
        PINS_DRV_WritePin(MOTOR1_AIN2_PORT, MOTOR1_AIN2_PIN, 0);
    } else {
        // 设置电机方向 - 后退
        PINS_DRV_WritePin(MOTOR1_AIN1_PORT, MOTOR1_AIN1_PIN, 0);
        PINS_DRV_WritePin(MOTOR1_AIN2_PORT, MOTOR1_AIN2_PIN, 1);
    }
}

/**
 * @brief 设置电机2（右前轮）方向和启动
 * @param direction 方向：0-前进，1-后退
 */
void SetMotor2Direction(uint8_t direction)
{
    // 设置TB6612 STBY引脚为1，使能驱动芯片
    PINS_DRV_WritePin(MOTOR2_STBY_PORT, MOTOR2_STBY_PIN, 1);
    
    if (direction == FORWARD) {
        // 设置电机方向 - 前进
        PINS_DRV_WritePin(MOTOR2_AIN1_PORT, MOTOR2_AIN1_PIN, 0);
        PINS_DRV_WritePin(MOTOR2_AIN2_PORT, MOTOR2_AIN2_PIN, 1);
    } else {
        // 设置电机方向 - 后退
        PINS_DRV_WritePin(MOTOR2_AIN1_PORT, MOTOR2_AIN1_PIN, 1);
        PINS_DRV_WritePin(MOTOR2_AIN2_PORT, MOTOR2_AIN2_PIN, 0);
    }
}

/**
 * @brief 设置电机3（左前轮）方向和启动
 * @param direction 方向：0-前进，1-后退
 */
void SetMotor3Direction(uint8_t direction)
{
    // 设置TB6612 STBY引脚为1，使能驱动芯片
    PINS_DRV_WritePin(MOTOR3_STBY_PORT, MOTOR3_STBY_PIN, 1);
    
    if (direction == FORWARD) {
        // 设置电机方向 - 前进
        PINS_DRV_WritePin(MOTOR3_AIN1_PORT, MOTOR3_AIN1_PIN, 0);
        PINS_DRV_WritePin(MOTOR3_AIN2_PORT, MOTOR3_AIN2_PIN, 1);
    } else {
        // 设置电机方向 - 后退
        PINS_DRV_WritePin(MOTOR3_AIN1_PORT, MOTOR3_AIN1_PIN, 1);
        PINS_DRV_WritePin(MOTOR3_AIN2_PORT, MOTOR3_AIN2_PIN, 0);
    }
}

/**
 * @brief 设置电机4（左后轮）方向和启动
 * @param direction 方向：0-前进，1-后退
 */
void SetMotor4Direction(uint8_t direction)
{
    // 设置TB6612 STBY引脚为1，使能驱动芯片
    PINS_DRV_WritePin(MOTOR4_STBY_PORT, MOTOR4_STBY_PIN, 1);
    
    if (direction == FORWARD) {
        // 设置电机方向 - 前进
        PINS_DRV_WritePin(MOTOR4_AIN1_PORT, MOTOR4_AIN1_PIN, 1);
        PINS_DRV_WritePin(MOTOR4_AIN2_PORT, MOTOR4_AIN2_PIN, 0);
    } else {
        // 设置电机方向 - 后退
        PINS_DRV_WritePin(MOTOR4_AIN1_PORT, MOTOR4_AIN1_PIN, 0);
        PINS_DRV_WritePin(MOTOR4_AIN2_PORT, MOTOR4_AIN2_PIN, 1);
    }
}

/**
 * @brief 设置电机1速度
 * @param duty PWM占空比，范围0-0xFFFF
 */
void SetMotor1Speed(uint16_t duty)
{
    g_stPwm2Config0.duty = duty;
    PWM_DRV_Init(INST_PWM_2, &g_stPwmState_2, &g_stPwm2Config0);
    PWM_DRV_Start(INST_PWM_2);  // 确保更新占空比后重新启动PWM
}

/**
 * @brief 设置电机2速度
 * @param duty PWM占空比，范围0-0xFFFF
 */
void SetMotor2Speed(uint16_t duty)
{
    g_stPwm0Config0.duty = duty;
    PWM_DRV_Init(INST_PWM_0, &g_stPwmState_0, &g_stPwm0Config0);
    PWM_DRV_Start(INST_PWM_0);  // 确保更新占空比后重新启动PWM
}

/**
 * @brief 设置电机3速度
 * @param duty PWM占空比，范围0-0xFFFF
 */
void SetMotor3Speed(uint16_t duty)
{
    g_stPwm1Config0.duty = duty;
    PWM_DRV_Init(INST_PWM_1, &g_stPwmState_1, &g_stPwm1Config0);
    PWM_DRV_Start(INST_PWM_1);  // 确保更新占空比后重新启动PWM
}

/**
 * @brief 设置电机4速度
 * @param duty PWM占空比，范围0-0xFFFF
 */
void SetMotor4Speed(uint16_t duty)
{
    g_stPwm3Config0.duty = duty;
    PWM_DRV_Init(INST_PWM_3, &g_stPwmState_3, &g_stPwm3Config0);
    PWM_DRV_Start(INST_PWM_3);  // 确保更新占空比后重新启动PWM
}

/**
 * @brief 停止所有电机
 */
void StopAllMotors(void)
{
    // 停止所有PWM信号
    SetMotor1Speed(0);
    SetMotor2Speed(0);
    SetMotor3Speed(0);
    SetMotor4Speed(0);
    
    // 禁用所有电机驱动
    PINS_DRV_WritePin(MOTOR1_STBY_PORT, MOTOR1_STBY_PIN, 0);
    PINS_DRV_WritePin(MOTOR2_STBY_PORT, MOTOR2_STBY_PIN, 0);
    PINS_DRV_WritePin(MOTOR3_STBY_PORT, MOTOR3_STBY_PIN, 0);
    PINS_DRV_WritePin(MOTOR4_STBY_PORT, MOTOR4_STBY_PIN, 0);
}

/**
 * @brief 直行功能
 * 所有电机向前转动，速度一致
 */
void MoveStraight(void)
{
    // 设置所有电机方向为前进
    SetMotor1Direction(FORWARD);
    SetMotor2Direction(FORWARD);
    SetMotor3Direction(FORWARD);
    SetMotor4Direction(FORWARD);
    
    // 设置所有电机为中等速度
    SetMotor1Speed(MOTOR_SPEED_MED);
    SetMotor2Speed(MOTOR_SPEED_MED);
    SetMotor3Speed(MOTOR_SPEED_MED);
    SetMotor4Speed(MOTOR_SPEED_MED);
}

/**
 * @brief 左转功能
 * 左侧电机减速或反转，右侧电机正向转动
 */
void TurnLeft(void)
{
    // 右侧电机(1和2)前进
    SetMotor1Direction(FORWARD);
    SetMotor2Direction(FORWARD);
    SetMotor1Speed(MOTOR_SPEED_MED);
    SetMotor2Speed(MOTOR_SPEED_MED);
    
    // 左侧电机(3和4)后退
    SetMotor3Direction(BACKWARD);
    SetMotor4Direction(BACKWARD);
    SetMotor3Speed(MOTOR_SPEED_LOW);
    SetMotor4Speed(MOTOR_SPEED_LOW);
}

/**
 * @brief 右转功能
 * 右侧电机减速或反转，左侧电机正向转动
 */
void TurnRight(void)
{
    // 左侧电机(3和4)前进
    SetMotor3Direction(FORWARD);
    SetMotor4Direction(FORWARD);
    SetMotor3Speed(MOTOR_SPEED_MED);
    SetMotor4Speed(MOTOR_SPEED_MED);
    
    // 右侧电机(1和2)后退
    SetMotor1Direction(BACKWARD);
    SetMotor2Direction(BACKWARD);
    SetMotor1Speed(MOTOR_SPEED_LOW);
    SetMotor2Speed(MOTOR_SPEED_LOW);
} 
