/**
 * @file h30.c
 * @brief H30 惯性姿态模块驱动实现
 * @details 提供 I2C 通信、欧拉角读取、角速度读取与积分航向估计功能
 */
#include "h30.h"
#include "sdk_project_config.h"
#include "board_delay.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OSIF_WAIT_FOREVER 0xFFFFFFFFU
#define DELAY_MS(ms) simple_delay_ms(ms)

// H30 I2C 定义（注意：STM32示例的 0x6A 可能为8位地址，对应7位为 0x35）
#define H30_I2C_ADDR_7BIT_PREFERRED  (0x35)   // 推测值：示例0x6A(8位) -> 0x35(7位)
#define H30_I2C_ADDR_PRIMARY         (0x6A)
#define H30_I2C_ADDR_ALTERNATE       (0x6B)
#define H30_REG_GYRO_BASE            (0x20)   // 连续12字节: X/Y/Z，每轴4字节int
#define H30_GYRO_DATA_LEN_BYTES      (12)
#define H30_DATA_SCALE_NOT_MAG       (0.000001f) // 数据缩放因子 1e-6

// H30 INT 数据就绪引脚：PORTD, pin 4
#define H30_INT_PORT                 (PORTD)
#define H30_INT_PIN                  (4U)

static i2c_master_state_t s_i2c0MasterState;
static bool s_h30_inited = false;
static float s_yaw_deg = 0.0f;
static const float DEAD_ZONE_DPS = 0.15f;  // 死区阈值：0.15°/s
static uint8_t s_h30_i2c_addr = H30_I2C_ADDR_PRIMARY;

static void h30_set_addr_value(uint8_t addr)
{
	I2C_DRV_MasterSetSlaveAddr(INST_I2C_0, addr, false);
}

static void h30_set_addr(void)
{
	h30_set_addr_value(s_h30_i2c_addr);
}

static bool h30_wait_data_ready_level(uint32_t timeout_ms, bool wait_high)
{
	uint32_t waited = 0;
	while (waited < timeout_ms) {
		uint32_t port_val = PINS_DRV_ReadPins(H30_INT_PORT);
		bool is_high = (port_val & (1UL << H30_INT_PIN)) != 0;
		if ((wait_high && is_high) || (!wait_high && !is_high)) {
			return true;
		}
		DELAY_MS(1);
		waited += 1;
	}
	return false;
}

static bool h30_wait_data_ready_any(uint32_t timeout_ms)
{
	// 先尝试等待高电平，失败再尝试低电平
	if (h30_wait_data_ready_level(timeout_ms, true)) return true;
	return h30_wait_data_ready_level(timeout_ms, false);
}

static bool h30_read_gyro_block_once(uint8_t *data /*12字节*/, bool send_stop_after_reg)
{
	uint8_t reg = H30_REG_GYRO_BASE;
	h30_set_addr();
	status_t st;
	st = I2C_DRV_MasterSendDataBlocking(INST_I2C_0, &reg, 1, send_stop_after_reg /*sendStop*/, OSIF_WAIT_FOREVER);
	if (st != STATUS_SUCCESS) return false;
	st = I2C_DRV_MasterReceiveDataBlocking(INST_I2C_0, data, H30_GYRO_DATA_LEN_BYTES, true /*sendStop*/, OSIF_WAIT_FOREVER);
	return (st == STATUS_SUCCESS);
}

static bool h30_read_gyro_block(uint8_t *data /*12字节*/)
{
	// 方式1：写寄存器后不发送STOP（重复起始）
	if (h30_read_gyro_block_once(data, false)) return true;
	// 短延时再试
	DELAY_MS(2);
	// 方式2：写寄存器后发送STOP，再单独读
	return h30_read_gyro_block_once(data, true);
}

// 小端解析4字节为int32
static int32_t read_le_i32(const uint8_t *p)
{
	return (int32_t)((uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24));
}

static void h30_scan_i2c_bus(void)
{
	printf("I2C0 扫描开始...\r\n");
	int found = 0;
	for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
		uint8_t dummy = 0x00;
		I2C_DRV_MasterSetSlaveAddr(INST_I2C_0, addr, false);
		status_t st = I2C_DRV_MasterSendDataBlocking(INST_I2C_0, &dummy, 1, true, 5);
		if (st == STATUS_SUCCESS) {
			printf("发现I2C设备: 0x%02X\r\n", addr);
			found++;
		}
	}
	if (found == 0) {
		printf("I2C0 扫描未发现任何设备\r\n");
	}
	// 还原当前地址
	h30_set_addr();
}

bool H30_Init(void)
{
	if (s_h30_inited) return true;
	/* I2C0 初始化（若已初始化，多次调用也安全） */
	I2C_DRV_MasterInit(INST_I2C_0, &g_stI2c0MasterUserConfig0, &s_i2c0MasterState);
	// 配置 INT 引脚为输入
	PINS_DRV_WritePinDirection(H30_INT_PORT, H30_INT_PIN, GPIO_INPUT_DIRECTION);
	
	// 若扫描曾显示 0x35，则优先尝试 0x35
	s_h30_i2c_addr = H30_I2C_ADDR_7BIT_PREFERRED;
	uint8_t probe[H30_GYRO_DATA_LEN_BYTES];
	if (!h30_read_gyro_block(probe)) {
		printf("H30 0x35(7位) 读失败，尝试0x6A...\r\n");
		s_h30_i2c_addr = H30_I2C_ADDR_PRIMARY;
		if (!h30_read_gyro_block(probe)) {
			printf("H30 0x6A 读失败，尝试0x6B...\r\n");
			s_h30_i2c_addr = H30_I2C_ADDR_ALTERNATE;
			if (!h30_read_gyro_block(probe)) {
				printf("H30 在 0x35/0x6A/0x6B 均读失败\r\n");
				// 扫描总线，帮助定位接线/模式问题
				h30_scan_i2c_bus();
				return false;
			}
		}
	}
	// 通过探测
	s_h30_inited = true;
	printf("H30 使用I2C地址 0x%02X\r\n", s_h30_i2c_addr);
	return true;
}

bool H30_ReadGzDps(float *gz_dps_out)
{
	if (!gz_dps_out) return false;
	// 优先等待INT，但不作为硬性条件
	( void )h30_wait_data_ready_any(5);
	// 数据就绪后稍作延时，满足器件保持时间
	DELAY_MS(1);
	uint8_t raw[H30_GYRO_DATA_LEN_BYTES];
	if (!h30_read_gyro_block(raw)) return false;
	// 数据顺序: X[0..3], Y[4..7], Z[8..11]，单位通过系数换算
	int32_t gz_i32 = read_le_i32(&raw[8]);
	float gz_dps = ((float)gz_i32) * H30_DATA_SCALE_NOT_MAG; // 转为dps
	*gz_dps_out = gz_dps;
	return true;
}

bool H30_ReadEuler(float *pitch_deg, float *roll_deg, float *yaw_deg)
{
	uint8_t reg = 0x40; // I2C_EULER_REG_ADDR
	h30_set_addr();
	status_t st = I2C_DRV_MasterSendDataBlocking(INST_I2C_0, &reg, 1, false, OSIF_WAIT_FOREVER);
	if (st != STATUS_SUCCESS) return false;
	uint8_t buf[12];
	st = I2C_DRV_MasterReceiveDataBlocking(INST_I2C_0, buf, 12, true, OSIF_WAIT_FOREVER);
	if (st != STATUS_SUCCESS) return false;
	int32_t p_i = read_le_i32(&buf[0]);
	int32_t r_i = read_le_i32(&buf[4]);
	int32_t y_i = read_le_i32(&buf[8]);
	float p = ((float)p_i) * H30_DATA_SCALE_NOT_MAG;
	float r = ((float)r_i) * H30_DATA_SCALE_NOT_MAG;
	float y = ((float)y_i) * H30_DATA_SCALE_NOT_MAG;
	if (pitch_deg) *pitch_deg = p;
	if (roll_deg)  *roll_deg  = r;
	if (yaw_deg)   *yaw_deg   = y;
	return true;
}

void H30_ResetYaw(void)
{
	s_yaw_deg = 0.0f;
}

void H30_UpdateYaw(uint32_t dt_ms)
{
	float gz_dps;
	if (!H30_ReadGzDps(&gz_dps)) return;
	
	// 直接减去固定的零偏值，防止角度漂移
	// 根据观察到的数据，零偏约为 5.5°/s（与监控显示一致，如需可调整）
	const float FIXED_BIAS_DPS = 5.5f;
	float gz_corr = gz_dps - FIXED_BIAS_DPS;
	
	// 死区处理：如果角速度很小，认为是静止状态
	if (fabsf(gz_corr) < DEAD_ZONE_DPS) {
		// 在死区内，不进行角度积分，防止累积误差
		gz_corr = 0.0f;
	}
	
	s_yaw_deg += gz_corr * ((float)dt_ms / 1000.0f);
}

float H30_GetYawDeg(void)
{
	return s_yaw_deg;
}

float H30_GetGyroZBias(void)
{
	// 零偏补偿已禁用，始终返回 0
	return 0.0f;
} 

bool H30_CaptureYawOrigin(uint8_t samples, uint16_t delay_ms_between_samples)
{
	if (samples == 0) samples = 10;
	float sum_yaw = 0.0f;
	uint8_t ok = 0;
	for (uint8_t i = 0; i < samples; i++) {
		float p, r, y;
		if (H30_ReadEuler(&p, &r, &y)) {
			sum_yaw += y;
			ok++;
		}
		DELAY_MS(delay_ms_between_samples);
	}
	if (ok == 0) return false;
	float avg_yaw = sum_yaw / (float)ok;
	// 将内部积分航向角清零，并将后续相对角度以当前为零点
	s_yaw_deg = 0.0f;
	// 同时将固定零偏调到更小（可选：不改）
	return true;
} 