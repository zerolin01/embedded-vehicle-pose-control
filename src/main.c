/**
 * @file main.c
 * @author 林木，欲得必行，文明人@江南大学
 * @brief 嵌入式移动小车姿态矫正控制系统主程序
 * @details 基于 H30 惯性姿态模块的移动小车航向保持与自动避障系统
 *          实现直行航向保持、定角转弯、超声波避障与舵机联动控制
 */

// 引入必要的头文件
#include "../board/sdk_project_config.h"
#include "../board/h30.h"
#include "../board/board_delay.h"
#include "../board/hcsr04.h"
#include "../board/dc_motor_control.h"
#include "../board/servo_control.h"
#include "../board/servo2_control.h"
#include "../board/my_move.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// 主任务函数声明
void nb(void);

/**
 * @brief 系统初始化
 */
void system_init(void)
{
    CLOCK_SYS_Init(g_pstClockManConfigsArr[0]);
    // 初始化PDMA、PINMUX、UART2
    PDMA_DRV_Init(&g_stPdmaState0, &g_stPdma0UserConfig0, g_stPdma0ChnStateArray,
                  g_stPdma0ChannelConfigArray, PDMA_CHANNEL_CONFIG_COUNT);
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr);
    UART_DRV_Init(INST_UART_2, &g_stUartState_2, &g_stUart2UserConfig0);
    setLogPort(2);
    // I2C 初始化将在 H30_Init() 中进行

    printf("系统初始化完成!\r\n");

    // 舵机初始化（servo）
    printf("\r\n舵机初始化（servo）\r\n");
    servo_init();
    printf("servo 初始化完成\r\n");

    // 舵机初始化（servo2）
    printf("\r\n舵机初始化（servo2）\r\n");
    servo2_init();
    printf("servo2 初始化完成\r\n");

    // 初始化 H30 惯性姿态模块
    if (!H30_Init()) {
        printf("H30 初始化失败!\r\n");
        while (1) {
            printf("H30 初始化失败，请检查硬件连接!\r\n");
            simple_delay_ms(1000);
        }
    }
    
    // 初始化超声波
    HCSR04_Init();

    // 初始化电机控制
    DCMotor_Init();
    printf("电机控制初始化完成!\r\n");

    printf("H30 初始化成功!\r\n");
    printf("零偏补偿功能已禁用，直接使用原始角速度数据\r\n");
    
    // 等待系统稳定
    simple_delay_ms(1000);
}

/**
 * @brief nb 任务流程：基于 H30 的直行航向保持与转弯，集成超声避障
 */
void nb(void)
{
	// 1) 第一次直行：静止多次采样，设定初始目标（使用已有初始化+避障版本）
	MyMove_StraightInit();
	float first_target = MyMove_GetStraightTarget();
	printf("[nb] 第一次直行目标(采样均值)=%.2f°\r\n", first_target);
	MyMove_StraightHoldYawWithObstacleAvoidanceUseTarget(0.12f, 5500);

	// 2) 执行第一次右转90°（原有实现，带舵机）
	MyMove_TurnRight90WithServo(0.18f, 1.0f, 6000, 105);

	// 3) 第二次直行：目标 = 第一次目标 - 90°（右转后继续沿着绝对参考系方向行驶）
	float target_second = MyMove_NormalizeDeg(first_target - 90.0f);
	MyMove_SetStraightTarget(target_second);
	printf("[nb] 第二次直行目标(首目标-90)=%.2f°\r\n", target_second);
	MyMove_StraightHoldYawWithObstacleAvoidanceUseTarget(0.12f, 2000);

	// 4) 中间舵机动作
	simple_delay_ms(1000);
	servo_set_angle(105);
	
	// 5) 第三次直行：无拐弯，沿用第二次直行目标
	printf("[nb] 第三次直行目标(沿用第二次)=%.2f°\r\n", MyMove_GetStraightTarget());
	MyMove_StraightHoldYawWithObstacleAvoidanceUseTarget(0.12f, 2000);

	// 6) 第二次右转90°
	MyMove_TurnRight90WithServo(0.18f, 1.0f, 6000, 102);
	
	// 7) 第四次直行：目标 = 第一次目标 ±180°（等效 first_target - 180°）
	float target_fourth = MyMove_NormalizeDeg(first_target - 180.0f);
	MyMove_SetStraightTarget(target_fourth);
	printf("[nb] 第四次直行目标(首目标-180)=%.2f°\r\n", target_fourth);
	MyMove_StraightHoldYawWithObstacleAvoidanceUseTarget(0.12f, 5000);
	
	// 收尾舵机
	simple_delay_ms(1000);
	servo_set_angle(80);
}


/**
 * @brief 主函数
 */
int main(void)
{
	system_init();
	nb();
	return 0;
}

