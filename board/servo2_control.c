/**
 * @file servo2_control.c
 * @brief 第二个SG90舵机控制实现 - 使用GPIO模拟PWM
 * @date 2024-07-21
 *
 * @details
 * 实现第二个SG90舵机控制的基本功能
 * 使用PORTC4 GPIO引脚通过软件模拟PWM控制舵机角度
 * 由于硬件PWM资源已用完，采用GPIO翻转高低电平的方式模拟PWM信号
 */

#include "servo2_control.h"

// 全局变量
static servo2_control_t g_servo2_control = {
    .current_angle = SERVO2_ANGLE_CENTER,
    .target_angle = SERVO2_ANGLE_CENTER,
    .current_pulse_us = SERVO2_CENTER_PULSE_US,
    .state = SERVO2_STATE_IDLE,
    .is_initialized = false
};

/**
 * @brief 精确延时函数(微秒级)
 * @param us 延时时间(微秒)
 */
void servo2_delay_us(uint32_t us) {
    // 使用系统提供的微秒延时函数
    BASIC_DelayUs(us);
}

/**
 * @brief 将角度转换为脉宽(微秒)
 * @param angle 舵机角度(0-180度)
 * @return 对应的脉宽值(微秒)
 */
uint32_t servo2_angle_to_pulse_us(uint16_t angle) {
    // 限制角度范围
    if (angle > SERVO2_ANGLE_MAX) {
        angle = SERVO2_ANGLE_MAX;
    }

    // 线性插值计算脉宽
    // 脉宽 = 最小脉宽 + (最大脉宽 - 最小脉宽) * 角度 / 180
    uint32_t pulse_us = SERVO2_MIN_PULSE_US +
                       ((SERVO2_MAX_PULSE_US - SERVO2_MIN_PULSE_US) * angle) / SERVO2_ANGLE_MAX;

    return pulse_us;
}

/**
 * @brief 发送单个PWM脉冲
 * @param pulse_width_us 脉宽(微秒)
 */
void servo2_send_pulse(uint32_t pulse_width_us) {
    // 设置GPIO为高电平
    PINS_DRV_WritePin(PORTC, 4U, 1);  // SERVO2_GPIO_PORT, SERVO2_GPIO_PIN

    // 保持高电平指定时间
    servo2_delay_us(pulse_width_us);

    // 设置GPIO为低电平
    PINS_DRV_WritePin(PORTC, 4U, 0);  // SERVO2_GPIO_PORT, SERVO2_GPIO_PIN

    // 计算剩余的低电平时间
    uint32_t low_time_us = SERVO2_PERIOD_US - pulse_width_us;  // SERVO2_PERIOD_US
    if (low_time_us > 0) {
        servo2_delay_us(low_time_us);
    }
}

/**
 * @brief 发送多个PWM周期
 * @param pulse_width_us 脉宽(微秒)
 * @param cycles 发送的周期数
 */
void servo2_send_pwm_cycles(uint32_t pulse_width_us, uint8_t cycles) {
    for (uint8_t i = 0; i < cycles; i++) {
        servo2_send_pulse(pulse_width_us);
    }
}

void servo2_hold_ms(uint32_t ms) {
    if (!g_servo2_control.is_initialized) {
        return;
    }
    uint32_t total_us = ms * 1000U;
    while (total_us >= SERVO2_PERIOD_US) {
        servo2_send_pulse(g_servo2_control.current_pulse_us);
        total_us -= SERVO2_PERIOD_US;
    }
    // 剩余不足一个周期的时间不再补偿，避免失真
}

void myservo_left90(void) {
	int i;
	for(i=0;i<=90;i+=30){
		servo2_set_angle(90-i);
		BASIC_DelayUs(400000);
	}
}

void myservo_right90(void) {
	int i;
	for(i=0;i<=90;i+=30){
		servo2_set_angle(90+i);
		BASIC_DelayUs(400000);
	}
}

/**
 * @brief 设置舵机角度
 * @param angle 舵机角度(0-180度)
 */
void servo2_set_angle(uint16_t angle) {
    if (!g_servo2_control.is_initialized) {
        return;
    }

    // 限制角度范围
    if (angle > SERVO2_ANGLE_MAX) {
        angle = SERVO2_ANGLE_MAX;
    }
    
    // 更新控制结构体
    g_servo2_control.target_angle = angle;
    g_servo2_control.current_pulse_us = servo2_angle_to_pulse_us(angle);
    g_servo2_control.state = SERVO2_STATE_MOVING;

    // 发送PWM信号
    servo2_send_pwm_cycles(g_servo2_control.current_pulse_us, SERVO2_PWM_CYCLES);

    // 更新当前角度
    g_servo2_control.current_angle = angle;
    g_servo2_control.state = SERVO2_STATE_IDLE;
}

/**
 * @brief 舵机左转90度 (从当前位置转到0度)
 */
void servo2_turn_left_90(void) {
    servo2_set_angle(SERVO2_ANGLE_MIN);
}

/**
 * @brief 舵机右转90度 (从当前位置转到180度)
 */
void servo2_turn_right_90(void) {
    servo2_set_angle(SERVO2_ANGLE_MAX);
}

/**
 * @brief 舵机转到中心位置 (90度)
 */
void servo2_turn_center(void) {
    servo2_set_angle(SERVO2_ANGLE_CENTER);
}

/**
 * @brief 顺时针旋转指定角度
 * @param degrees 旋转角度（0-180度）
 */
void servo2_rotate_clockwise(uint16_t degrees) {
    if (!g_servo2_control.is_initialized) {
        return;
    }
    
    uint16_t new_angle = g_servo2_control.current_angle + degrees;
    if (new_angle > SERVO2_ANGLE_MAX) {
        new_angle = SERVO2_ANGLE_MAX;
    }
    servo2_set_angle(new_angle);
}

/**
 * @brief 逆时针旋转指定角度
 * @param degrees 旋转角度（0-180度）
 */
void servo2_rotate_counterclockwise(uint16_t degrees) {
    if (!g_servo2_control.is_initialized) {
        return;
    }
    
    uint16_t new_angle = g_servo2_control.current_angle - degrees;
    if (new_angle < SERVO2_ANGLE_MIN) {
        new_angle = SERVO2_ANGLE_MIN;
    }
    servo2_set_angle(new_angle);
}

/**
 * @brief 相对角度旋转（正数顺时针，负数逆时针）
 * @param degrees 旋转角度（正数顺时针，负数逆时针）
 */
void servo2_rotate_relative(int16_t degrees) {
    if (degrees == 0) return;
    
    if (degrees > 0) {
        servo2_rotate_clockwise((uint16_t)degrees);
    } else {
        servo2_rotate_counterclockwise((uint16_t)(-degrees));
    }
}

/**
 * @brief 连续旋转控制（适用于SG90连续旋转舵机）
 * @param speed 旋转速度（-100到100，0为停止，正数顺时针，负数逆时针）
 */
void servo2_rotate_continuous(int16_t speed) {
    if (!g_servo2_control.is_initialized) {
        return;
    }
    
    uint32_t pulse_us;
    
    if (speed == 0) {
        // 停止
        pulse_us = SERVO2_CENTER_PULSE_US;  // 1.5ms
    } else if (speed > 0) {
        // 顺时针旋转（正数）
        // 速度范围：1-100，对应脉宽：1.6ms-2.5ms
        pulse_us = SERVO2_CENTER_PULSE_US + (speed * 10);  // 1.5ms + 速度*0.01ms
        if (pulse_us > SERVO2_MAX_PULSE_US) pulse_us = SERVO2_MAX_PULSE_US;
    } else {
        // 逆时针旋转（负数）
        // 速度范围：-1到-100，对应脉宽：1.4ms-0.5ms
        pulse_us = SERVO2_CENTER_PULSE_US + (speed * 10);  // 1.5ms + 速度*0.01ms
        if (pulse_us < SERVO2_MIN_PULSE_US) pulse_us = SERVO2_MIN_PULSE_US;
    }
    
    // 持续发送PWM信号
    servo2_send_pwm_cycles(pulse_us, SERVO2_PWM_CYCLES);
}

/**
 * @brief 基于时间的相对角度旋转（适用于连续旋转舵机）
 * @param degrees 旋转角度（正数顺时针，负数逆时针）
 */
void servo2_rotate_relative_time(int16_t degrees) {
    if (degrees == 0) return;
    
    // 根据角度计算旋转时间（可调整）
    uint32_t rotate_time_ms = abs(degrees) * 8;  // 8ms/度，可根据实际情况调整
    
    int16_t speed = (degrees > 0) ? 60 : -60;  // 中等速度，可根据实际情况调整
    
    // 开始旋转
    servo2_rotate_continuous(speed);
    
    // 等待指定时间
    servo2_delay_us(rotate_time_ms * 1000);  // 转换为微秒
    
    // 停止
    servo2_rotate_continuous(0);
}

/**
 * @brief 初始化第二个舵机
 */
void servo2_init(void) {
    // 初始化GPIO引脚为低电平
    PINS_DRV_WritePin(PORTC, 4U, 0);  // SERVO2_GPIO_PORT, SERVO2_GPIO_PIN
    
    // 初始化控制结构体
    g_servo2_control.current_angle = SERVO2_ANGLE_CENTER;
    g_servo2_control.target_angle = SERVO2_ANGLE_CENTER;
    g_servo2_control.current_pulse_us = SERVO2_CENTER_PULSE_US;
    g_servo2_control.state = SERVO2_STATE_IDLE;
    g_servo2_control.is_initialized = true;

    // 设置舵机到中心位置
    servo2_set_angle(SERVO2_ANGLE_CENTER);
}

/**
 * @brief 获取舵机当前角度
 * @return 当前角度值
 */
uint16_t servo2_get_current_angle(void) {
    return g_servo2_control.current_angle;
}

/**
 * @brief 获取舵机状态
 * @return 舵机状态
 */
servo2_state_t servo2_get_state(void) {
    return g_servo2_control.state;
}

/**
 * @brief 舵机演示函数 - 0度到180度往返运动
 */
void servo2_demo(void) {
    if (!g_servo2_control.is_initialized) {
        return;
    }
    
    // 0->180度
    for (uint16_t angle = SERVO2_ANGLE_MIN; angle <= SERVO2_ANGLE_MAX; angle += 10) {
        servo2_set_angle(angle);
        servo2_delay_us(200000); // 延时200ms
    }

    // 180->0度
    for (uint16_t angle = SERVO2_ANGLE_MAX; angle >= SERVO2_ANGLE_MIN; angle -= 10) {
        servo2_set_angle(angle);
        servo2_delay_us(200000); // 延时200ms

        // 防止无符号整数下溢
        if (angle == SERVO2_ANGLE_MIN) {
            break;
        }
    }
    
    // 回到中心位置
    servo2_turn_center();
}
