#include "peripherals_uart_5_config.h"

uart_state_t g_stUartState_5;

uart_user_config_t g_stUart5UserConfig0 = {
    .baudRate        = 115200,
    .parityMode      = UART_PARITY_DISABLED,
    .stopBitCount    = UART_ONE_STOP_BIT,
    .bitCountPerChar = UART_8_BITS_PER_CHAR,
    .transferType    = UART_USING_INTERRUPTS,  // 只能用中断
    .fifoType        = UART_FIFO_DEPTH_1,
    .rxDMAChannel    = 6,
    .txDMAChannel    = 7,
}; 