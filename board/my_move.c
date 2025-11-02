/**
 * @file my_move.c
 * @author 林木@江南大学
 * @brief 高精度运动控制实现 - 基于 H30 姿态闭环
 * @details 实现直行航向保持、定角转弯、避障与 PID 差速纠偏功能
 */

#include "my_move.h"
#include "motor_control.h"
#include "h30.h"
#include "dc_motor_control.h"
#include "board_delay.h"
#include "servo2_control.h"
#include "hcsr04.h"
#include <math.h>

// ========================
// 内部状态与参数
// ========================
static float32_t s_straight_kp = 0.06f;
static float32_t s_straight_ki = 0.002f;
static float32_t s_straight_kd = 0.012f;

static float32_t s_turn_kp = 2.0f;
static float32_t s_turn_ki = 0.05f;
static float32_t s_turn_kd = 0.10f;

static float32_t s_target_yaw_deg = 0.0f;   // 直行目标或转向目标
static float32_t s_prev_err = 0.0f;
static float32_t s_integral = 0.0f;
// 记录上一次直行初始化时的航向
static float32_t s_last_straight_init_yaw = 0.0f;
static int s_has_last_straight_init = 0;

static float32_t normalize_deg(float32_t a)
{
	while (a > 180.0f) { a -= 360.0f; }
	while (a < -180.0f) { a += 360.0f; }
	return a;
}

float32_t MyMove_NormalizeDeg(float32_t a)
{
	return normalize_deg(a);
}

void MyMove_SetStraightTarget(float32_t target_yaw_deg)
{
	s_target_yaw_deg = normalize_deg(target_yaw_deg);
	// 重置PID状态，确保以新目标进入闭环
	s_prev_err = 0.0f;
	s_integral = 0.0f;
	// 记录为“最近一次直行参考”
	s_last_straight_init_yaw = s_target_yaw_deg;
	s_has_last_straight_init = 1;
}

float32_t MyMove_GetStraightTarget(void)
{
	return s_target_yaw_deg;
}

static float32_t clampf32(float32_t v, float32_t lo, float32_t hi)
{
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}

// 在使用前为带舵机的转向执行函数添加前置声明
static void MyMove_TurnExecuteGentleToTargetWithServo(float32_t base_turn_speed,
	float32_t stop_deg,
	uint32_t timeout_ms,
	float32_t target_yaw,
	uint16_t servo_angle);

void MyMove_SetStraightPID(float32_t Kp, float32_t Ki, float32_t Kd)
{
	if (Kp < 0) { Kp = 0; } if (Ki < 0) { Ki = 0; } if (Kd < 0) { Kd = 0; }
	s_straight_kp = Kp; s_straight_ki = Ki; s_straight_kd = Kd;
}

void MyMove_SetTurnPID(float32_t Kp, float32_t Ki, float32_t Kd)
{
	if (Kp < 0) { Kp = 0; } if (Ki < 0) { Ki = 0; } if (Kd < 0) { Kd = 0; }
	s_turn_kp = Kp; s_turn_ki = Ki; s_turn_kd = Kd;
}

// 初始化直行：将当前 H30 欧拉航向设为参考
void MyMove_StraightInit(void)
{
	float p, r, y;
	// 先停再采样，避免运动干扰
	MyMove_Stop();
	simple_delay_ms(120);
	// 多次采样求平均，获得更稳的初始航向
	float sum_y = 0.0f; int cnt = 0;
	for (int i = 0; i < 8; ++i) {
		if (H30_ReadEuler(&p, &r, &y)) { sum_y += y; cnt++; }
		simple_delay_ms(20);
	}
	if (cnt > 0) {
		float y_avg = sum_y / (float)cnt;
		s_target_yaw_deg = y_avg;
		// 记录直行参考航向
		s_last_straight_init_yaw = y_avg;
		s_has_last_straight_init = 1;
		// 重置PID状态
		s_prev_err = 0.0f;
		s_integral = 0.0f;
	}
}

// 执行直行闭环（带日志）：打印初始航向角与每次采纳的航向角
void MyMove_StraightHoldYawWithLog(float32_t base_speed, uint32_t duration_ms)
{
	float32_t bs = clampf32(base_speed, 0.0f, 1.0f);
	float p0, r0, y0;
	if (!H30_ReadEuler(&p0, &r0, &y0)) {
		MyMove_Stop();
		return;
	}
	s_target_yaw_deg = y0;
	// 记录直行参考航向
	s_last_straight_init_yaw = y0;
	s_has_last_straight_init = 1;
	s_prev_err = 0.0f;
	s_integral = 0.0f;
	printf("StraightInit: yaw0=%.2f°\r\n", y0);

	uint32_t now = 0;
	while (now < duration_ms) {
		float p, r, y;
		if (!H30_ReadEuler(&p, &r, &y)) {
			MyMove_Stop();
			return;
		}
		printf("StraightTick: yaw=%.2f°\r\n", y);
		float err = normalize_deg(s_target_yaw_deg - y);
		s_integral += err;
		if (s_integral > 1000.0f) { s_integral = 1000.0f; }
		if (s_integral < -1000.0f) { s_integral = -1000.0f; }
		float derr = err - s_prev_err;
		s_prev_err = err;
		float adjust = s_straight_kp * err + s_straight_ki * s_integral + s_straight_kd * derr;
		float yaw_corr = clampf32(adjust, -0.6f, 0.6f);
		MyMove_ForwardWithDiff(bs, yaw_corr);
		simple_delay_ms(100);
		now += 100;
	}
	MyMove_Stop();
}

// 执行直行闭环（带日志+车轮速度）：打印初始航向、每次航向，以及4个轮子的速度
void MyMove_StraightHoldYawWithLogAndSpeeds(float32_t base_speed, uint32_t duration_ms)
{
	float32_t bs = clampf32(base_speed, 0.0f, 1.0f);
	float p0, r0, y0;
	// 先停再采样，避免运动干扰
	MyMove_Stop();
	simple_delay_ms(120);
	// 多次采样求平均，获得更稳的初始航向
	float sum_y0 = 0.0f; int cnt0 = 0;
	for (int i = 0; i < 8; ++i) {
		if (H30_ReadEuler(&p0, &r0, &y0)) { sum_y0 += y0; cnt0++; }
		simple_delay_ms(20);
	}
	if (cnt0 == 0) {
		MyMove_Stop();
		return;
	}
	float y0_avg = sum_y0 / (float)cnt0;
	s_target_yaw_deg = y0_avg;
	// 记录直行参考航向
	s_last_straight_init_yaw = y0_avg;
	s_has_last_straight_init = 1;
	s_prev_err = 0.0f;
	s_integral = 0.0f;
	printf("StraightInit: yaw0=%.2f°\r\n", y0_avg);

	uint32_t now = 0;
	// 误差EMA与死区、输出斜率限制配置
	static float err_ema = 0.0f;
	const float ERR_EMA_ALPHA = 0.18f;     // 误差低通强度（更平滑）
	const float DEADBAND_DEG = 1.2f;       // 误差死区（度，更大更稳）
	static float prev_yaw_cmd = 0.0f;      // 上次差速指令
	const float SLEW_STEP = 0.04f;         // 每周期最大变化量（更慢更稳）
	// 复位跨次调用的静态状态，避免继承上一次的滤波/输出残留
	err_ema = 0.0f;
	prev_yaw_cmd = 0.0f;

	while (now < duration_ms) {
		float p, r, y;
		if (!H30_ReadEuler(&p, &r, &y)) {
			MyMove_Stop();
			return;
		}
		// 误差计算与滤波
		float err_raw = normalize_deg(s_target_yaw_deg - y);
		err_ema = (1.0f - ERR_EMA_ALPHA) * err_ema + ERR_EMA_ALPHA * err_raw;
		float err = err_ema;
		// 死区处理
		if (fabsf(err) < DEADBAND_DEG) { err = 0.0f; }

		// 与速度弱相关的差速限幅（更小上限以防一大一小交替）
		float rot_limit = fminf(0.15f, bs * 0.28f + 0.02f);

		// PD 计算
		float derr = err - s_prev_err;
		s_prev_err = err;
		float rot_pd = s_straight_kp * err + s_straight_kd * derr;

		// 抗积分饱和：仅在PD未接近饱和时积分；死区内缓慢泄放
		if (fabsf(rot_pd) < rot_limit * 0.9f && fabsf(err) > (DEADBAND_DEG * 0.5f)) {
			s_integral += s_straight_ki * err;
			if (s_integral > 0.10f) { s_integral = 0.10f; }
			if (s_integral < -0.10f) { s_integral = -0.10f; }
		} else {
			s_integral *= 0.98f;
		}

		float yaw_cmd = rot_pd + s_integral;
		// 限幅
		if (yaw_cmd > rot_limit) { yaw_cmd = rot_limit; }
		if (yaw_cmd < -rot_limit) { yaw_cmd = -rot_limit; }
		// 斜率限制
		float delta = yaw_cmd - prev_yaw_cmd;
		if (delta > SLEW_STEP) { yaw_cmd = prev_yaw_cmd + SLEW_STEP; }
		else if (delta < -SLEW_STEP) { yaw_cmd = prev_yaw_cmd - SLEW_STEP; }
		prev_yaw_cmd = yaw_cmd;

		float yaw_corr = yaw_cmd;
		// 先计算左右占空比（0.0~1.0），便于打印
		float32_t left  = clampf32(bs - yaw_corr, 0.0f, 1.0f);
		float32_t right = clampf32(bs + yaw_corr, 0.0f, 1.0f);
		MyMove_ForwardWithDiff(bs, yaw_corr);

		// 打印航向与四个轮子的指令占空比（百分比），作为无编码器时的速度近似
		float d1 = right * 100.0f; // M1右后
		float d2 = right * 100.0f; // M2右前
		float d3 = left  * 100.0f; // M3左前
		float d4 = left  * 100.0f; // M4左后
		printf("StraightTick: yaw=%.2f°, duty%%: M1=%.1f%% M2=%.1f%% M3=%.1f%% M4=%.1f%%\r\n",
		       y, d1, d2, d3, d4);

		simple_delay_ms(100);
		now += 100;
	}
	MyMove_Stop();
}

// 执行直行闭环：运行固定时长，周期约30ms（不打印日志）
void MyMove_StraightHoldYaw(float32_t base_speed, uint32_t duration_ms)
{
	float32_t bs = clampf32(base_speed, 0.0f, 1.0f);
	uint32_t now = 0;
	while (now < duration_ms) {
		float p, r, y;
		if (!H30_ReadEuler(&p, &r, &y)) {
			MyMove_Stop();
			return;
		}
		float err = normalize_deg(s_target_yaw_deg - y);
		s_integral += err;
		if (s_integral > 1000.0f) { s_integral = 1000.0f; }
		if (s_integral < -1000.0f) { s_integral = -1000.0f; }
		float derr = err - s_prev_err;
		s_prev_err = err;
		float adjust = s_straight_kp * err + s_straight_ki * s_integral + s_straight_kd * derr;
		float yaw_corr = clampf32(adjust, -0.6f, 0.6f);
		MyMove_ForwardWithDiff(bs, yaw_corr);
		for (volatile uint32_t d = 0; d < 100000; ++d) { }
		now += 30;
	}
	MyMove_Stop();
}

// 执行直行闭环（带避障）：集成超声波避障，连续3次检测到障碍物距离小于8cm时停车等待
void MyMove_StraightHoldYawWithObstacleAvoidance(float32_t base_speed, uint32_t duration_ms)
{
	float32_t bs = clampf32(base_speed, 0.0f, 1.0f);
	float p0, r0, y0;
	// 先停再采样，避免运动干扰
	MyMove_Stop();
	simple_delay_ms(120);
	// 多次采样求平均，获得更稳的初始航向
	float sum_y0 = 0.0f; int cnt0 = 0;
	for (int i = 0; i < 8; ++i) {
		if (H30_ReadEuler(&p0, &r0, &y0)) { sum_y0 += y0; cnt0++; }
		simple_delay_ms(20);
	}
	if (cnt0 == 0) {
		MyMove_Stop();
		return;
	}
	float y0_avg = sum_y0 / (float)cnt0;
	s_target_yaw_deg = y0_avg;
	// 记录直行参考航向
	s_last_straight_init_yaw = y0_avg;
	s_has_last_straight_init = 1;
	s_prev_err = 0.0f;
	s_integral = 0.0f;
	printf("StraightInit: yaw0=%.2f°\r\n", y0_avg);

	uint32_t start_time = 0;  // 记录开始时间
	uint32_t now = 0;         // 当前运行时间
	uint32_t obstacle_wait_time = 0;  // 累计等待障碍物的时间
	uint32_t actual_motion_time = 0;  // 实际运动时间（不包含等待时间）
	
	// 误差EMA与死区、输出斜率限制配置 - 优化参数减少扭动
	static float err_ema = 0.0f;
	const float ERR_EMA_ALPHA = 0.30f;     // 增强滤波，减少噪声影响
	const float DEADBAND_DEG = 2.0f;       // 增大死区，减少过度纠偏
	static float prev_yaw_cmd = 0.0f;      // 上次差速指令
	const float SLEW_STEP = 0.08f;         // 增大斜率限制，提高响应性
	
	// 大角度偏差检测与处理
	const float LARGE_ERROR_THRESHOLD = 6.0f;  // 降低阈值，更早干预
	const float ERROR_CHANGE_THRESHOLD = 8.0f; // 误差变化阈值
	static float prev_error = 0.0f;
	bool large_error_detected = false;
	uint32_t large_error_start_time = 0;
	const uint32_t LARGE_ERROR_RESET_MS = 3000; // 延长大误差处理时间
	
	// 振荡检测与抑制
	static float error_history[5] = {0};  // 误差历史记录
	static int history_index = 0;
	int oscillation_count = 0;
	const int OSCILLATION_THRESHOLD = 3;  // 连续3次误差符号变化认为振荡
	
	// 避障去抖参数 - 修改为8cm阈值
	int obs_hits = 0;
	const int OBS_HITS_THRESHOLD = 3;     // 连续3次命中才停车等待
	const uint32_t OBS_MIN_ENABLE_MS = 500; // 上电500ms后才响应避障
	bool is_waiting_for_obstacle = false;  // 是否正在等待障碍物消失
	uint32_t obstacle_wait_start = 0;      // 开始等待障碍物的时间
	
	// 复位跨次调用的静态状态，避免继承上一次的滤波/输出残留
	err_ema = 0.0f;
	prev_yaw_cmd = 0.0f;
	obs_hits = 0;
	prev_error = 0.0f;
	large_error_detected = false;
	history_index = 0;
	oscillation_count = 0;
	for (int i = 0; i < 5; i++) error_history[i] = 0;

	while (actual_motion_time < duration_ms) {  // 使用实际运动时间作为循环条件
		float p, r, y;
		if (!H30_ReadEuler(&p, &r, &y)) {
			MyMove_Stop();
			return;
		}
		
		// 避障检测（去抖）- 使用8cm阈值
		if (now > OBS_MIN_ENABLE_MS && HCSR04_IsObstacleDetected()) {
			obs_hits++;
			if (obs_hits >= OBS_HITS_THRESHOLD) {
				// 检测到障碍物，停车等待
				MyMove_Stop();
				if (!is_waiting_for_obstacle) {
					is_waiting_for_obstacle = true;
					obstacle_wait_start = now;
					printf("检测到障碍物（6cm内），停车等待！连续%d次检测到障碍物\r\n", obs_hits);
				}
			}
		} else {
			obs_hits = 0;
			// 如果之前正在等待障碍物，现在障碍物消失了
			if (is_waiting_for_obstacle) {
				uint32_t wait_duration = now - obstacle_wait_start;
				obstacle_wait_time += wait_duration;
				printf("障碍物消失，继续直行。本次等待时间: %dms, 累计等待时间: %dms\r\n", 
				       wait_duration, obstacle_wait_time);
				is_waiting_for_obstacle = false;
			}
		}
		
		// 如果正在等待障碍物消失，跳过运动控制，时间继续累加但不计入实际运动时间
		if (is_waiting_for_obstacle) {
			simple_delay_ms(100);
			now += 100;
			continue;
		}
		
		// 实际运动时间累加（不包含等待时间）
		actual_motion_time += 100;
		
		// 误差计算与滤波
		float err_raw = normalize_deg(s_target_yaw_deg - y);
		err_ema = (1.0f - ERR_EMA_ALPHA) * err_ema + ERR_EMA_ALPHA * err_raw;
		float err = err_ema;
		
		// 振荡检测
		error_history[history_index] = err;
		history_index = (history_index + 1) % 5;
		
		// 检查是否发生振荡（误差符号频繁变化）
		oscillation_count = 0;
		for (int i = 1; i < 5; i++) {
			if ((error_history[i] > 0) != (error_history[i-1] > 0)) {
				oscillation_count++;
			}
		}
		bool is_oscillating = (oscillation_count >= OSCILLATION_THRESHOLD);
		
		// 检测大角度偏差
		float error_change = fabsf(err - prev_error);
		if (fabsf(err) > LARGE_ERROR_THRESHOLD || error_change > ERROR_CHANGE_THRESHOLD) {
			if (!large_error_detected) {
				large_error_detected = true;
				large_error_start_time = now;
				// 重置积分项，避免积分饱和
				s_integral = 0.0f;
				printf("检测到大角度偏差: %.2f°，重置积分项\r\n", err);
			}
		}
		
		// 检查是否退出大误差状态
		if (large_error_detected && (now - large_error_start_time) > LARGE_ERROR_RESET_MS) {
			large_error_detected = false;
			printf("退出大误差处理状态\r\n");
		}
		
		prev_error = err;
		
		// 死区处理
		if (fabsf(err) < DEADBAND_DEG) { err = 0.0f; }

		// 与速度弱相关的差速限幅（更小上限以防一大一小交替）
		float rot_limit = fminf(0.10f, bs * 0.20f + 0.02f);  // 进一步减小最大纠偏幅度

		// PD 计算 - 根据误差大小和振荡状态使用不同策略
		float derr = err - s_prev_err;
		s_prev_err = err;
		
		float kp_effective, kd_effective, ki_effective;
		
		if (is_oscillating) {
			// 振荡时：大幅降低增益，增加阻尼，禁用积分
			kp_effective = s_straight_kp * 0.3f;  // 大幅降低比例增益
			kd_effective = s_straight_kd * 3.0f;  // 大幅增加微分增益
			ki_effective = 0.0f;                  // 禁用积分
			s_integral = 0.0f;                    // 清零积分项
			printf("振荡检测！使用振荡抑制模式: Kp=%.3f, Kd=%.3f, Ki=%.3f\r\n", kp_effective, kd_effective, ki_effective);
		} else if (large_error_detected) {
			// 大误差时：降低增益，增加阻尼，禁用积分
			kp_effective = s_straight_kp * 0.5f;  // 降低比例增益
			kd_effective = s_straight_kd * 2.0f;  // 增加微分增益
			ki_effective = 0.0f;                  // 禁用积分
			printf("大误差模式: Kp=%.3f, Kd=%.3f, Ki=%.3f\r\n", kp_effective, kd_effective, ki_effective);
		} else {
			// 正常误差时：使用标准参数
			kp_effective = (fabsf(err) < 2.0f) ? (s_straight_kp * 0.6f) : s_straight_kp;
			kd_effective = s_straight_kd;
			ki_effective = s_straight_ki;
		}
		
		float rot_pd = kp_effective * err + kd_effective * derr;

		// 积分项处理 - 更严格的积分控制
		if (!is_oscillating && !large_error_detected && fabsf(rot_pd) < rot_limit * 0.7f && fabsf(err) > (DEADBAND_DEG * 0.9f)) {
			s_integral += ki_effective * err;
			// 更严格的积分限幅
			if (s_integral > 0.05f) { s_integral = 0.05f; }
			if (s_integral < -0.05f) { s_integral = -0.05f; }
		} else {
			s_integral *= 0.90f;  // 更快的积分衰减
		}

		float yaw_cmd = rot_pd + s_integral;
		// 限幅
		if (yaw_cmd > rot_limit) { yaw_cmd = rot_limit; }
		if (yaw_cmd < -rot_limit) { yaw_cmd = -rot_limit; }
		// 斜率限制
		float delta = yaw_cmd - prev_yaw_cmd;
		if (delta > SLEW_STEP) { yaw_cmd = prev_yaw_cmd + SLEW_STEP; }
		else if (delta < -SLEW_STEP) { yaw_cmd = prev_yaw_cmd - SLEW_STEP; }
		prev_yaw_cmd = yaw_cmd;

		float yaw_corr = yaw_cmd;
		// 先计算左右占空比（0.0~1.0），便于打印
		float32_t left  = clampf32(bs - yaw_corr, 0.0f, 1.0f);
		float32_t right = clampf32(bs + yaw_corr, 0.0f, 1.0f);
		MyMove_ForwardWithDiff(bs, yaw_corr);

		// 打印航向与四个轮子的指令占空比（百分比），作为无编码器时的速度近似
		float d1 = right * 100.0f; // M1右后
		float d2 = right * 100.0f; // M2右前
		float d3 = left  * 100.0f; // M3左前
		float d4 = left  * 100.0f; // M4左后
		printf("StraightTick: yaw=%.2f°, duty%%: M1=%.1f%% M2=%.1f%% M3=%.1f%% M4=%.1f%%, 避障计数=%d, 等待状态=%s, 纠偏=%.3f, 大误差=%s, 振荡=%s, 实际运动=%dms, 累计等待=%dms\r\n",
		       y, d1, d2, d3, d4, obs_hits, is_waiting_for_obstacle ? "是" : "否", yaw_corr, large_error_detected ? "是" : "否", is_oscillating ? "是" : "否", actual_motion_time, obstacle_wait_time);

		simple_delay_ms(100);
		now += 100;
	}
	
	// 显示最终统计信息
	printf("直行完成！总时间: %dms, 实际运动时间: %dms, 累计等待时间: %dms\r\n", 
	       now, actual_motion_time, obstacle_wait_time);
	
	// ==================== 姿态矫正功能 ====================
	printf("\r\n=== 开始姿态矫正 ===\r\n");
	
	// 先停车，确保静止
	MyMove_Stop();
	simple_delay_ms(200);
	
	// 读取当前航向角
	float p_final, r_final, y_final;
	if (!H30_ReadEuler(&p_final, &r_final, &y_final)) {
		printf("姿态矫正失败：无法读取当前航向角\r\n");
		MyMove_Stop();
		return;
	}
	
	// 计算与初始航向角的差值
	float yaw_error = normalize_deg(s_target_yaw_deg - y_final);
	printf("初始航向: %.2f°, 当前航向: %.2f°, 误差: %.2f°\r\n", 
	       s_target_yaw_deg, y_final, yaw_error);
	
	// 如果误差大于1度，进行姿态矫正
	if (fabsf(yaw_error) > 1.0f) {
		printf("开始姿态矫正，目标误差: ≤1°\r\n");
		
		// 姿态矫正参数 - 非常温和的参数
		const float CORRECTION_SPEED = 0.08f;        // 很慢的矫正速度
		const float CORRECTION_KP = 0.8f;            // 较小的比例增益
		const float CORRECTION_DEADBAND = 0.8f;      // 较小的死区
		const uint32_t CORRECTION_TIMEOUT = 8000;    // 8秒超时
		const float MAX_CORRECTION_LIMIT = 0.06f;    // 最大纠偏幅度
		
		uint32_t correction_start = now;
		float prev_correction_err = yaw_error;
		float correction_integral = 0.0f;
		
		while (fabsf(yaw_error) > 1.0f && (now - correction_start) < CORRECTION_TIMEOUT) {
			// 读取当前航向
			if (!H30_ReadEuler(&p_final, &r_final, &y_final)) {
				printf("姿态矫正失败：无法读取航向角\r\n");
				break;
			}
			
			yaw_error = normalize_deg(s_target_yaw_deg - y_final);
			
			// 死区处理
			float correction_err = yaw_error;
			if (fabsf(correction_err) < CORRECTION_DEADBAND) {
				correction_err = 0.0f;
			}
			
			// 简单的P控制，不使用积分避免超调
			float correction_cmd = CORRECTION_KP * correction_err;
			
			// 限幅
			if (correction_cmd > MAX_CORRECTION_LIMIT) correction_cmd = MAX_CORRECTION_LIMIT;
			if (correction_cmd < -MAX_CORRECTION_LIMIT) correction_cmd = -MAX_CORRECTION_LIMIT;
			
			// 执行细微转向
			if (fabsf(correction_cmd) > 0.01f) {
				// 原地旋转
				if (correction_cmd > 0) {
					// 需要左转
					MyMove_TurnLeft(CORRECTION_SPEED * fabsf(correction_cmd) / MAX_CORRECTION_LIMIT);
				} else {
					// 需要右转
					MyMove_TurnRight(CORRECTION_SPEED * fabsf(correction_cmd) / MAX_CORRECTION_LIMIT);
				}
			} else {
				MyMove_Stop();
			}
			
			printf("姿态矫正: 当前=%.2f°, 目标=%.2f°, 误差=%.2f°, 纠偏=%.3f\r\n", 
			       y_final, s_target_yaw_deg, yaw_error, correction_cmd);
			
			simple_delay_ms(50);  // 50ms控制周期，更精细
			now += 50;
		}
		
		// 最终停车
		MyMove_Stop();
		simple_delay_ms(200);
		
		// 读取最终航向角
		if (H30_ReadEuler(&p_final, &r_final, &y_final)) {
			float final_error = normalize_deg(s_target_yaw_deg - y_final);
			printf("姿态矫正完成！最终航向: %.2f°, 最终误差: %.2f°\r\n", y_final, final_error);
			
			if (fabsf(final_error) <= 1.0f) {
				printf("✓ 姿态矫正成功，误差在1°以内\r\n");
			} else {
				printf("⚠ 姿态矫正未完全成功，误差: %.2f°\r\n", final_error);
			}
		}
	} else {
		printf("✓ 航向角误差已在1°以内，无需矫正\r\n");
	}
	
	printf("=== 姿态矫正结束 ===\r\n\r\n");
	
	MyMove_Stop();
}

// 新增：使用当前目标航向执行直行（带避障），不会重新采样初始航向
void MyMove_StraightHoldYawWithObstacleAvoidanceUseTarget(float32_t base_speed, uint32_t duration_ms)
{
	// 直接复用 MyMove_StraightHoldYawWithObstacleAvoidance 的主体，但省略“重新采样/设目标”部分。
	// 为减少重复代码，调用者先通过 MyMove_SetStraightTarget() 设定好目标，再进入核心循环。
	float32_t bs = clampf32(base_speed, 0.0f, 1.0f);
	printf("StraightUseTarget: targetYaw=%.2f°\r\n", s_target_yaw_deg);

	uint32_t now = 0;
	uint32_t obstacle_wait_time = 0;
	uint32_t actual_motion_time = 0;

	static float err_ema = 0.0f;
	const float ERR_EMA_ALPHA = 0.30f;
	const float DEADBAND_DEG = 2.0f;
	static float prev_yaw_cmd = 0.0f;
	const float SLEW_STEP = 0.08f;

	const float LARGE_ERROR_THRESHOLD = 6.0f;
	const float ERROR_CHANGE_THRESHOLD = 8.0f;
	static float prev_error = 0.0f;
	bool large_error_detected = false;
	uint32_t large_error_start_time = 0;
	const uint32_t LARGE_ERROR_RESET_MS = 3000;

	static float error_history[5] = {0};
	static int history_index = 0;
	int oscillation_count = 0;
	const int OSCILLATION_THRESHOLD = 3;

	int obs_hits = 0;
	const int OBS_HITS_THRESHOLD = 3;
	const uint32_t OBS_MIN_ENABLE_MS = 500;
	bool is_waiting_for_obstacle = false;
	uint32_t obstacle_wait_start = 0;

	// 重置跨次状态与PID内部状态（不改变目标）
	err_ema = 0.0f;
	prev_yaw_cmd = 0.0f;
	obs_hits = 0;
	prev_error = 0.0f;
	large_error_detected = false;
	history_index = 0;
	oscillation_count = 0;
	for (int i = 0; i < 5; i++) error_history[i] = 0;
	s_prev_err = 0.0f;
	s_integral = 0.0f;

	while (actual_motion_time < duration_ms) {
		float p, r, y;
		if (!H30_ReadEuler(&p, &r, &y)) {
			MyMove_Stop();
			return;
		}

		if (now > OBS_MIN_ENABLE_MS && HCSR04_IsObstacleDetected()) {
			obs_hits++;
			if (obs_hits >= OBS_HITS_THRESHOLD) {
				MyMove_Stop();
				if (!is_waiting_for_obstacle) {
					is_waiting_for_obstacle = true;
					obstacle_wait_start = now;
					printf("检测到障碍物（6cm内），停车等待！连续%d次检测到障碍物\r\n", obs_hits);
				}
			}
		} else {
			obs_hits = 0;
			if (is_waiting_for_obstacle) {
				uint32_t wait_duration = now - obstacle_wait_start;
				obstacle_wait_time += wait_duration;
				printf("障碍物消失，继续直行。本次等待时间: %dms, 累计等待时间: %dms\r\n", wait_duration, obstacle_wait_time);
				is_waiting_for_obstacle = false;
			}
		}

		if (is_waiting_for_obstacle) {
			simple_delay_ms(100);
			now += 100;
			continue;
		}

		actual_motion_time += 100;

		float err_raw = normalize_deg(s_target_yaw_deg - y);
		err_ema = (1.0f - ERR_EMA_ALPHA) * err_ema + ERR_EMA_ALPHA * err_raw;
		float err = err_ema;
		if (fabsf(err) < DEADBAND_DEG) { err = 0.0f; }

		float rot_limit = fminf(0.10f, bs * 0.20f + 0.02f);

		float derr = err - s_prev_err;
		s_prev_err = err;
		float rot_pd = s_straight_kp * err + s_straight_kd * derr;

		float kp_effective = s_straight_kp;
		float kd_effective = s_straight_kd;
		float ki_effective = s_straight_ki;

		if (fabsf(rot_pd) < rot_limit * 0.7f && fabsf(err) > (DEADBAND_DEG * 0.9f)) {
			s_integral += ki_effective * err;
			if (s_integral > 0.05f) { s_integral = 0.05f; }
			if (s_integral < -0.05f) { s_integral = -0.05f; }
		} else {
			s_integral *= 0.90f;
		}

		float yaw_cmd = kp_effective * err + kd_effective * derr + s_integral;
		if (yaw_cmd > rot_limit) { yaw_cmd = rot_limit; }
		if (yaw_cmd < -rot_limit) { yaw_cmd = -rot_limit; }
		float delta = yaw_cmd - prev_yaw_cmd;
		if (delta > SLEW_STEP) { yaw_cmd = prev_yaw_cmd + SLEW_STEP; }
		else if (delta < -SLEW_STEP) { yaw_cmd = prev_yaw_cmd - SLEW_STEP; }
		prev_yaw_cmd = yaw_cmd;

		float yaw_corr = yaw_cmd;
		MyMove_ForwardWithDiff(bs, yaw_corr);

		float32_t left  = clampf32(bs - yaw_corr, 0.0f, 1.0f);
		float32_t right = clampf32(bs + yaw_corr, 0.0f, 1.0f);
		printf("StraightUseTargetTick: yaw=%.2f°, duty%%: M1=%.1f%% M2=%.1f%% M3=%.1f%% M4=%.1f%%, 目标=%.2f°\r\n",
		       y, right*100.0f, right*100.0f, left*100.0f, left*100.0f, s_target_yaw_deg);

		simple_delay_ms(100);
		now += 100;
	}

	printf("直行结束（使用外部目标）。实际运动时间: %dms, 累计等待时间: %dms\r\n", actual_motion_time, obstacle_wait_time);
	MyMove_Stop();
}

// 初始化转向：目标为当前 yaw ± 90°
void MyMove_TurnInit(int direction)
{
	float p, r, y;
	if (H30_ReadEuler(&p, &r, &y)) {
		float delta = (direction >= 0) ? (+90.0f) : (-90.0f);
		// 如果是右转且已记录直行参考，则优先以该参考为目标
		if (direction < 0 && s_has_last_straight_init) {
			s_target_yaw_deg = normalize_deg(s_last_straight_init_yaw);
		} else {
			s_target_yaw_deg = normalize_deg(y + delta);
		}
		s_prev_err = 0.0f;
		s_integral = 0.0f;
	}
}

// 温和型转向执行：参考直行增强版的滤波/限幅/斜率/积分策略
static void MyMove_TurnExecuteGentleToTarget(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms, float32_t target_yaw)
{
	float32_t bs = clampf32(base_turn_speed, 0.0f, 1.0f);
	if (stop_deg < 0.5f) { stop_deg = 0.5f; }
	uint32_t elapsed = 0;
	// 误差EMA/死区/斜率限制参数（适度更保守）
	static float err_ema = 0.0f;
	const float ERR_EMA_ALPHA = 0.20f;
	const float DEADBAND_DEG = 1.2f;
	static float prev_yaw_cmd = 0.0f;
	const float SLEW_STEP = 0.035f;
	// 复用直行PID参数
	s_prev_err = 0.0f;
	s_integral = 0.0f;
	float last_y = 0.0f;
	// 直行后先停再拐弯
	MyMove_Stop();
	simple_delay_ms(120);
	// 复位跨次调用的静态状态
	err_ema = 0.0f;
	prev_yaw_cmd = 0.0f;

	while (elapsed < timeout_ms) {
		float p, r, y;
		if (!H30_ReadEuler(&p, &r, &y)) {
			MyMove_Stop();
			return;
		}
		last_y = y;
		float err_raw = normalize_deg(target_yaw - y);
		float aerr = (err_raw >= 0.0f) ? err_raw : -err_raw;
		if (aerr <= stop_deg) {
			printf("TurnDone: finalYaw=%.2f°, target=%.2f°, err=%.2f°\r\n", y, target_yaw, err_raw);
			MyMove_Stop();
			return;
		}
		printf("TurnTick: yaw=%.2f°, target=%.2f°, err=%.2f°\r\n", y, target_yaw, err_raw);
		// EMA + 死区
		err_ema = (1.0f - ERR_EMA_ALPHA) * err_ema + ERR_EMA_ALPHA * err_raw;
		float err = err_ema;
		if (fabsf(err) < DEADBAND_DEG) { err = 0.0f; }

		// 上限随速度：略高于直行，便于控制但仍克制
		float rot_limit = fminf(0.18f, bs * 0.32f + 0.03f);

		float derr = err - s_prev_err;
		s_prev_err = err;
		float rot_pd = s_straight_kp * err + s_straight_kd * derr;

		// 受控积分
		if (fabsf(rot_pd) < rot_limit * 0.85f && fabsf(err) > (DEADBAND_DEG * 0.5f)) {
			s_integral += s_straight_ki * err;
			if (s_integral > 0.12f) { s_integral = 0.12f; }
			if (s_integral < -0.12f) { s_integral = -0.12f; }
		} else {
			s_integral *= 0.98f;
		}

		float yaw_cmd = rot_pd + s_integral;
		if (yaw_cmd > rot_limit) { yaw_cmd = rot_limit; }
		if (yaw_cmd < -rot_limit) { yaw_cmd = -rot_limit; }
		float delta = yaw_cmd - prev_yaw_cmd;
		if (delta > SLEW_STEP) { yaw_cmd = prev_yaw_cmd + SLEW_STEP; }
		else if (delta < -SLEW_STEP) { yaw_cmd = prev_yaw_cmd - SLEW_STEP; }
		prev_yaw_cmd = yaw_cmd;

		// 原地旋转：左右轮反向
		float rot_base = fminf(0.35f, bs * 0.40f + 0.05f); // 基础旋转速度（温和）
		float scale = (rot_limit > 1e-6f) ? (fabsf(yaw_cmd) / rot_limit) : 0.0f;
		float rot_speed = clampf32(rot_base * scale, 0.12f, 0.5f);
		if (yaw_cmd > 0.0f) {
			// 左转
			MyMove_TurnLeft(rot_speed);
		} else if (yaw_cmd < 0.0f) {
			// 右转
			MyMove_TurnRight(rot_speed);
		} else {
			MyMove_Stop();
		}

		for (volatile uint32_t d = 0; d < 100000; ++d) { }
		elapsed += 30;
	}
	// 超时信息
	{
		float final_err = normalize_deg(target_yaw - last_y);
		printf("TurnTimeout: finalYaw=%.2f°, target=%.2f°, err=%.2f°\r\n", last_y, target_yaw, final_err);
	}
	MyMove_Stop();
}

// 执行转向：直到 |误差|<=stop_deg 或超时
void MyMove_TurnExecute(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms)
{
	// 使用温和型转向到 s_target_yaw_deg
	MyMove_TurnExecuteGentleToTarget(base_turn_speed, stop_deg, timeout_ms, s_target_yaw_deg);
}

void MyMove_TurnDelta(float32_t delta_deg, float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms)
{
	float p, r, y;
	if (!H30_ReadEuler(&p, &r, &y)) {
		MyMove_Stop();
		return;
	}
	s_target_yaw_deg = normalize_deg(y + delta_deg);
	s_prev_err = 0.0f;
	s_integral = 0.0f;
	// 直接以目标航向进行温和型执行
	MyMove_TurnExecuteGentleToTarget(base_turn_speed, stop_deg, timeout_ms, s_target_yaw_deg);
}

void MyMove_TurnLeft90(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms)
{
	MyMove_TurnInit(MY_TURN_LEFT);
	MyMove_TurnExecute(base_turn_speed, stop_deg, timeout_ms);
}

void MyMove_TurnRight90(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms)
{
	float p, r, y;
	if (!H30_ReadEuler(&p, &r, &y)) {
		MyMove_Stop();
		return;
	}
	float32_t target = normalize_deg(y - 90.0f);
	s_target_yaw_deg = target;
	s_prev_err = 0.0f;
	s_integral = 0.0f;
	MyMove_TurnExecuteGentleToTarget(base_turn_speed, stop_deg, timeout_ms, target);
}

// 基础动作与差速接口保持不变
void MyMove_Forward(float32_t speed)
{
	float32_t s = clampf32(speed, 0.0f, 1.0f);
	SetMotor1Direction(FORWARD); SetMotor1Speed((uint16_t)(s * 0xFFFF));
	SetMotor2Direction(FORWARD); SetMotor2Speed((uint16_t)(s * 0xFFFF));
	SetMotor3Direction(FORWARD); SetMotor3Speed((uint16_t)(s * 0xFFFF));
	SetMotor4Direction(FORWARD); SetMotor4Speed((uint16_t)(s * 0xFFFF));
}

void MyMove_TurnLeft(float32_t speed)
{
	float32_t s = clampf32(speed, 0.0f, 1.0f);
	SetMotor1Direction(FORWARD);  SetMotor1Speed((uint16_t)(s * 0xFFFF));
	SetMotor2Direction(FORWARD);  SetMotor2Speed((uint16_t)(s * 0xFFFF));
	SetMotor3Direction(BACKWARD); SetMotor3Speed((uint16_t)(s * 0xFFFF));
	SetMotor4Direction(BACKWARD); SetMotor4Speed((uint16_t)(s * 0xFFFF));
}

void MyMove_TurnRight(float32_t speed)
{
	float32_t s = clampf32(speed, 0.0f, 1.0f);
	SetMotor1Direction(BACKWARD); SetMotor1Speed((uint16_t)(s * 0xFFFF));
	SetMotor2Direction(BACKWARD); SetMotor2Speed((uint16_t)(s * 0xFFFF));
	SetMotor3Direction(FORWARD);  SetMotor3Speed((uint16_t)(s * 0xFFFF));
	SetMotor4Direction(FORWARD);  SetMotor4Speed((uint16_t)(s * 0xFFFF));
}

void MyMove_Stop(void)
{
	SetMotor1Speed(0);
	SetMotor2Speed(0);
	SetMotor3Speed(0);
	SetMotor4Speed(0);
}

void MyMove_ForwardWithDiff(float32_t base_speed, float32_t yaw_corr)
{
	float32_t bs = clampf32(base_speed, 0.0f, 1.0f);
	float32_t yc = clampf32(yaw_corr, -0.6f, 0.6f);
	float32_t left  = clampf32(bs - yc, 0.0f, 1.0f);
	float32_t right = clampf32(bs + yc, 0.0f, 1.0f);
	SetMotor1Direction(FORWARD); SetMotor1Speed((uint16_t)(right * 0xFFFF));
	SetMotor2Direction(FORWARD); SetMotor2Speed((uint16_t)(right * 0xFFFF));
	SetMotor3Direction(FORWARD); SetMotor3Speed((uint16_t)(left  * 0xFFFF));
	SetMotor4Direction(FORWARD); SetMotor4Speed((uint16_t)(left  * 0xFFFF));
}

void MyMove_ForwardWithYaw(float32_t base_speed, float32_t yaw_error_deg, float32_t Kp)
{
	float32_t kp = (Kp < 0.0f) ? 0.0f : Kp;
	float32_t yaw_corr = kp * yaw_error_deg;
	yaw_corr = clampf32(yaw_corr, -0.6f, 0.6f);
	MyMove_ForwardWithDiff(base_speed, yaw_corr);
}

// 结合舵机控制的右转90度：小车右转的同时舵机反向转动，保持物品相对地面静止
void MyMove_TurnRight90WithServo(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms, uint16_t servo_angle)
{
	float p, r, y;
	if (!H30_ReadEuler(&p, &r, &y)) {
		MyMove_Stop();
		return;
	}
	float32_t target = normalize_deg(y - 90.0f);
	s_target_yaw_deg = target;
	s_prev_err = 0.0f;
	s_integral = 0.0f;
	
	// 开始执行转向控制
	MyMove_TurnExecuteGentleToTargetWithServo(base_turn_speed, stop_deg, timeout_ms, target, servo_angle);
}

// 带舵机控制的转向执行函数
static void MyMove_TurnExecuteGentleToTargetWithServo(float32_t base_turn_speed, float32_t stop_deg, uint32_t timeout_ms, float32_t target_yaw, uint16_t servo_angle)
{
	float32_t bs = clampf32(base_turn_speed, 0.0f, 1.0f);
	if (stop_deg < 0.5f) { stop_deg = 0.5f; }
	uint32_t elapsed = 0;
	
	// 误差EMA/死区/斜率限制参数（适度更保守）
	static float err_ema = 0.0f;
	const float ERR_EMA_ALPHA = 0.20f;
	const float DEADBAND_DEG = 1.2f;
	static float prev_yaw_cmd = 0.0f;
	const float SLEW_STEP = 0.035f;
	
	// 复用直行PID参数
	s_prev_err = 0.0f;
	s_integral = 0.0f;
	float last_y = 0.0f;
	
	// 直行后先停再拐弯
	MyMove_Stop();
	simple_delay_ms(120);
	
	// 复位跨次调用的静态状态
	err_ema = 0.0f;
	prev_yaw_cmd = 0.0f;

	// 计算目标角度对应的脉宽（避免调用会阻塞的 servo2_set_angle）
	uint32_t servo_target_pulse = servo2_angle_to_pulse_us(servo_angle);

	while (elapsed < timeout_ms) {
		float p, r, y;
		if (!H30_ReadEuler(&p, &r, &y)) {
			MyMove_Stop();
			return;
		}
		last_y = y;
		float err_raw = normalize_deg(target_yaw - y);
		float aerr = (err_raw >= 0.0f) ? err_raw : -err_raw;
		if (aerr <= stop_deg) {
			printf("TurnDone: finalYaw=%.2f°, target=%.2f°, err=%.2f°\r\n", y, target_yaw, err_raw);
			MyMove_Stop();
			return;
		}
		printf("TurnTick: yaw=%.2f°, target=%.2f°, err=%.2f°\r\n", y, target_yaw, err_raw);
		
		// EMA + 死区
		err_ema = (1.0f - ERR_EMA_ALPHA) * err_ema + ERR_EMA_ALPHA * err_raw;
		float err = err_ema;
		if (fabsf(err) < DEADBAND_DEG) { err = 0.0f; }

		// 上限随速度：略高于直行，便于控制但仍克制
		float rot_limit = fminf(0.18f, bs * 0.32f + 0.03f);

		float derr = err - s_prev_err;
		s_prev_err = err;
		float rot_pd = s_straight_kp * err + s_straight_kd * derr;

		// 受控积分
		if (fabsf(rot_pd) < rot_limit * 0.85f && fabsf(err) > (DEADBAND_DEG * 0.5f)) {
			s_integral += s_straight_ki * err;
			if (s_integral > 0.12f) { s_integral = 0.12f; }
			if (s_integral < -0.12f) { s_integral = -0.12f; }
		} else {
			s_integral *= 0.98f;
		}

		float yaw_cmd = rot_pd + s_integral;
		if (yaw_cmd > rot_limit) { yaw_cmd = rot_limit; }
		if (yaw_cmd < -rot_limit) { yaw_cmd = -rot_limit; }
		float delta = yaw_cmd - prev_yaw_cmd;
		if (delta > SLEW_STEP) { yaw_cmd = prev_yaw_cmd + SLEW_STEP; }
		else if (delta < -SLEW_STEP) { yaw_cmd = prev_yaw_cmd - SLEW_STEP; }
		prev_yaw_cmd = yaw_cmd;

		// 原地旋转：左右轮反向
		float rot_base = fminf(0.35f, bs * 0.40f + 0.05f); // 基础旋转速度（温和）
		float scale = (rot_limit > 1e-6f) ? (fabsf(yaw_cmd) / rot_limit) : 0.0f;
		float rot_speed = clampf32(rot_base * scale, 0.12f, 0.5f);
		if (yaw_cmd > 0.0f) {
			// 左转
			MyMove_TurnLeft(rot_speed);
		} else if (yaw_cmd < 0.0f) {
			// 右转
			MyMove_TurnRight(rot_speed);
		} else {
			MyMove_Stop();
		}

		// 发送一个完整的舵机PWM周期，以保证舵机与电机每周期并行工作
		servo2_send_pulse(servo_target_pulse);
		elapsed += (SERVO2_PERIOD_US / 1000U); // 以舵机周期为时间基准
	}
	
	// 超时信息
	{
		float final_err = normalize_deg(target_yaw - last_y);
		printf("TurnTimeout: finalYaw=%.2f°, target=%.2f°, err=%.2f°\r\n", last_y, target_yaw, final_err);
	}
	MyMove_Stop();
}
