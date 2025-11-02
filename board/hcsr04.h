/**
 * @file hcsr04.h
 * @author 林木@江南大学
 * @brief HC-SR04 超声波测距传感器驱动接口
 * @details 提供超声波测距与障碍物检测功能
 */

#ifndef HCSR04_H
#define HCSR04_H

#include "pins_driver.h"
#include "../ESWIN_SDK/platform/basic/include/basic_api.h"
#include <stdio.h>
#include <math.h>

// HC-SR04引脚定义
#define TRIG_PORT    PTA
#define TRIG_PIN     31U
#define ECHO_PORT    PTA
#define ECHO_PIN     30U

// 测距参数
#define SOUND_SPEED_20C     343.0f  // 20°C时的声速 (m/s)
#define TEMP_COEFFICIENT    0.6f    // 温度系数 (m/s/°C)
#define OBSTACLE_THRESHOLD  6.0f   // 障碍物检测阈值 (cm) - 修改为8cm

// 函数声明
void HCSR04_Init(void);
float HCSR04_MeasureDistance(void);
bool HCSR04_IsObstacleDetected(void);
float single_measure_distance_cm(void);
float calculate_sound_speed(float temperature);

#endif // HCSR04_H
