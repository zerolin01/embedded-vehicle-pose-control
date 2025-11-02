/**
 * @file servo_control.c
 * @brief MG90S舵机控制实现 - 使用GPIO模拟PWM
 * @author 林木@江南大学
 * @date 
 *
 * @details
 * 使用GPIO翻转高低电平的方式模拟PWM信号，控制SG90舵机角度
 */

#include "servo_control.h"

// 控制状态
static struct {
    uint16_t current_angle;
    uint16_t target_angle;
    uint32_t current_pulse_us;
    bool is_initialized;
} g_servo_ctrl = {
    .current_angle = SERVO_ANGLE_CENTER,
    .target_angle = SERVO_ANGLE_CENTER,
    .current_pulse_us = SERVO_CENTER_PULSE_US,
    .is_initialized = false
};

uint32_t servo_angle_to_pulse_us(uint16_t angle) {
    if (angle > SERVO_ANGLE_MAX) {
        angle = SERVO_ANGLE_MAX;
    }
    uint32_t pulse_us = SERVO_MIN_PULSE_US +
                        ((SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US) * angle) / SERVO_ANGLE_MAX;
    return pulse_us;
}

void servo_send_pulse(uint32_t pulse_width_us) {
    PINS_DRV_WritePin(SERVO_GPIO_PORT, SERVO_GPIO_PIN, 1);  // 高电平
    BASIC_DelayUs(pulse_width_us);                          // 保持脉宽时间
    PINS_DRV_WritePin(SERVO_GPIO_PORT, SERVO_GPIO_PIN, 0);  // 低电平
    uint32_t low_time_us = SERVO_PERIOD_US - pulse_width_us;
    if (low_time_us > 0) {
        BASIC_DelayUs(low_time_us);                          // 保持低电平时间
    }
}

void servo_send_pwm_cycles(uint32_t pulse_width_us, uint8_t cycles) {
    for (uint8_t i = 0; i < cycles; i++) {
        servo_send_pulse(pulse_width_us);
    }
}

void servo_hold_ms(uint32_t ms) {
    if (!g_servo_ctrl.is_initialized) {
        return;
    }
    uint32_t total_us = ms * 1000U;
    while (total_us >= SERVO_PERIOD_US) {
        servo_send_pulse(g_servo_ctrl.current_pulse_us);
        total_us -= SERVO_PERIOD_US;
    }
}

void servo_set_angle(uint16_t angle) {
    if (!g_servo_ctrl.is_initialized) {
        return;
    }
    if (angle > SERVO_ANGLE_MAX) {
        angle = SERVO_ANGLE_MAX;
    }
    g_servo_ctrl.target_angle = angle;
    g_servo_ctrl.current_pulse_us = servo_angle_to_pulse_us(angle);
    servo_send_pwm_cycles(g_servo_ctrl.current_pulse_us, SERVO_PWM_CYCLES);
    g_servo_ctrl.current_angle = angle;
}

uint16_t servo_get_current_angle(void) {
    return g_servo_ctrl.current_angle;
}

void servo_init(void) {
    // 置低电平
    PINS_DRV_WritePin(SERVO_GPIO_PORT, SERVO_GPIO_PIN, 0);

    g_servo_ctrl.current_angle = SERVO_ANGLE_CENTER;
    g_servo_ctrl.target_angle = SERVO_ANGLE_CENTER;
    g_servo_ctrl.current_pulse_us = SERVO_CENTER_PULSE_US;
    g_servo_ctrl.is_initialized = true;

    // 上电设置到中心位置
    servo_set_angle(SERVO_ANGLE_CENTER);
}

void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 8000; ++i) {
        __asm__("nop");
    }
}

// 顺时针旋转指定角度
void servo_rotate_clockwise(uint16_t degrees) {
    uint16_t new_angle = g_servo_ctrl.current_angle + degrees;
    // 移除角度限制，允许超过180度
    servo_set_angle(new_angle);
}

// 逆时针旋转指定角度
void servo_rotate_counterclockwise(uint16_t degrees) {
    uint16_t new_angle = g_servo_ctrl.current_angle - degrees;
    if (new_angle < SERVO_ANGLE_MIN) {
        new_angle = SERVO_ANGLE_MIN;
    }
    servo_set_angle(new_angle);
}

// 连续旋转控制
void servo_rotate_continuous(int16_t speed) {
    if (!g_servo_ctrl.is_initialized) {
        return;
    }
    
    uint32_t pulse_us;
    
    if (speed == 0) {
        // 停止
        pulse_us = SERVO_CENTER_PULSE_US;  // 1.5ms
    } else if (speed > 0) {
        // 顺时针旋转（正数）
        // 速度范围：1-100，对应脉宽：1.6ms-2.5ms
        pulse_us = SERVO_CENTER_PULSE_US + (speed * 10);  // 1.5ms + 速度*0.01ms
        if (pulse_us > SERVO_MAX_PULSE_US) pulse_us = SERVO_MAX_PULSE_US;
    } else {
        // 逆时针旋转（负数）
        // 速度范围：-1到-100，对应脉宽：1.4ms-0.5ms
        pulse_us = SERVO_CENTER_PULSE_US + (speed * 10);  // 1.5ms + 速度*0.01ms
        if (pulse_us < SERVO_MIN_PULSE_US) pulse_us = SERVO_MIN_PULSE_US;
    }
    
    // 持续发送PWM信号
    servo_send_pwm_cycles(pulse_us, SERVO_PWM_CYCLES);
}

// 相对旋转（基于时间的连续旋转）
void servo_rotate_relative(int16_t degrees) {
    if (degrees == 0) return;
    
    // 根据角度计算旋转时间（可调整）
    uint32_t rotate_time_ms = abs(degrees) * 8;  // 8ms/度，更快
    
    int16_t speed = (degrees > 0) ? 60 : -60;  // 提高速度
    
    servo_rotate_continuous(speed);
    simple_delay_ms(rotate_time_ms);
    servo_rotate_continuous(0);
} 