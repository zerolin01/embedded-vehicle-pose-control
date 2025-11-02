/**
 * @file hcsr04.c
 * @author 林木@江南大学
 * @brief HC-SR04 超声波测距传感器驱动实现
 * @details 提供超声波测距、障碍物检测与温度补偿功能
 */

#include "hcsr04.h"

/**
 * @brief 初始化HC-SR04超声波传感器
 */
void HCSR04_Init(void)
{
    // 引脚已在pins_driver.c中配置
    // TRIG设为输出，ECHO设为输入
    printf("HC-SR04 超声波传感器初始化完成\r\n");
}

/**
 * @brief 根据温度计算声速
 * @param temperature 温度(°C)
 * @return 声速(m/s)
 */
float calculate_sound_speed(float temperature)
{
    return SOUND_SPEED_20C + TEMP_COEFFICIENT * (temperature - 20.0f);
}

/**
 * @brief 单次测距
 * @return 距离(cm)，-1表示测量失败
 */
float single_measure_distance_cm(void)
{
    uint32_t timeout = 60000;
    uint32_t start_time, end_time, duration;

    // 确保ECHO为低电平
    while (PINS_DRV_ReadPins(ECHO_PORT) & (1 << ECHO_PIN)) {
        BASIC_DelayUs(1);
    }

    // 发送触发脉冲
    PINS_DRV_WritePin(TRIG_PORT, TRIG_PIN, 0);
    BASIC_DelayUs(2);
    PINS_DRV_WritePin(TRIG_PORT, TRIG_PIN, 1);
    BASIC_DelayUs(20);
    PINS_DRV_WritePin(TRIG_PORT, TRIG_PIN, 0);

    // 等待ECHO上升沿
    BASIC_DelayUs(100);
    timeout = 60000;
    while (!(PINS_DRV_ReadPins(ECHO_PORT) & (1 << ECHO_PIN))) {
        if (--timeout == 0) {
            return -1.0f;
        }
        BASIC_DelayUs(1);
    }

    // 记录开始时间并等待下降沿
    start_time = 0;
    timeout = 60000;
    while (PINS_DRV_ReadPins(ECHO_PORT) & (1 << ECHO_PIN)) {
        if (--timeout == 0) {
            return -1.0f;
        }
        BASIC_DelayUs(1);
        start_time++;
    }

    duration = start_time;
    
    // 计算距离 (声速343m/s，往返除以2)
    float distance = (duration * 343.0f * 0.000001f * 100.0f) / 2.0f;
    
    return distance;
}

/**
 * @brief 测量距离(多次测量取平均)
 * @return 距离(cm)
 */
float HCSR04_MeasureDistance(void)
{
    float distances[5];
    float sum = 0.0f;
    int valid_count = 0;

    // 连续测量5次
    for (int i = 0; i < 5; i++) {
        distances[i] = single_measure_distance_cm();
        
        if (distances[i] > 0 && distances[i] < 400.0f) {
            sum += distances[i];
            valid_count++;
        }
        
        BASIC_DelayUs(10000); // 10ms间隔
    }

    if (valid_count > 0) {
        return sum / valid_count;
    }
    
    return -1.0f; // 测量失败
}

/**
 * @brief 检测是否有障碍物
 * @return true-检测到障碍物，false-无障碍物
 */
bool HCSR04_IsObstacleDetected(void)
{
    float distance = HCSR04_MeasureDistance();
    
    if (distance > 0 && distance < OBSTACLE_THRESHOLD) {
        printf("checked，distence: %.1f cm\r\n", distance);
        return true;
    }
    
    return false;
}
