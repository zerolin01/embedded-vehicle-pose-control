/**
 * @file board_delay.c
 * @author 林木@江南大学
 * @brief 基础延时函数实现
 * @details 提供毫秒级软件延时功能，用于简单的时序控制
 */

#include "board_delay.h"

/**
 * @brief 简单的毫秒级延时函数
 * @param ms 延时时长（毫秒）
 * @details 基于空循环实现的阻塞式延时，精度取决于系统时钟
 */
void simple_delay_ms(unsigned int ms) {
    while (ms--) {
        for (volatile unsigned int i = 0; i < 8000; ++i) {
            __asm__("nop");
        }
    }
} 