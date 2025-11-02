/**
 * @file h30.h
 * @brief H30 惯性姿态模块驱动接口
 * @details 提供欧拉角、角速度读取与航向积分估计功能
 */
#ifndef __BOARD_H30_H__
#define __BOARD_H30_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 I2C 和 H30 惯性姿态模块
 * @return true 初始化成功；false 失败
 */
bool H30_Init(void);

// 读取当前 Z 轴角速度（单位：度/秒）。返回是否成功
bool H30_ReadGzDps(float *gz_dps_out);

// 读取 H30 欧拉角（pitch/roll/yaw，单位：度）。返回是否成功
bool H30_ReadEuler(float *pitch_deg, float *roll_deg, float *yaw_deg);

// 捕获当前 H30 航向角作为参考零点（平均若干样本），成功返回 true
bool H30_CaptureYawOrigin(uint8_t samples, uint16_t delay_ms_between_samples);

// 将内部累计航向角清零
void H30_ResetYaw(void);

// 基于当前陀螺数据积分更新航向角（dt_ms：积分步长，单位 ms）
void H30_UpdateYaw(uint32_t dt_ms);

// 获取内部累计航向角（单位：度）。右转为正，左转为负
float H30_GetYawDeg(void);

// 获取当前陀螺 Z 轴零偏值（单位：度/秒）
float H30_GetGyroZBias(void);

#ifdef __cplusplus
}
#endif

#endif // __BOARD_H30_H__ 