/*
 * Copyright (c) 2013-2020 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ----------------------------------------------------------------------
 *
 * $Date:        19. July 2021
 * $Revision:    V0.0.0
 *
 * Project:      EMSIS-BASIC API
 * Title:        platform.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

/* #include "basic_common.h" */
#include <stdarg.h>
#include "eswin_sdk_soc.h"
#include "platform.h"
// #include "uart_driver.h"

/*--------------------------------------------------------------------------*/
/* Interrupt */
__WEAK __INTERRUPT void clic_mtip_handler(void);
__WEAK __INTERRUPT void clic_csip_handler(void);
__WEAK __INTERRUPT void clic_meip_handler(void);
__WEAK __INTERRUPT void clic_default_intexc_handler(void);
extern void _start(void);
int32_t log_uart_nr = -1;

#ifdef CONFIG_RV_CLIC_VEC_ADDR
__attribute__((aligned(4096), section(".timcode"))) uint32_t clic_vector_table[SOC_INT_MAX] = {
#if defined(__GNUC__)
    (uint32_t)_start, /* 0: Reserved, default handler for Flash download mode */
#else
    (uint32_t)clic_default_intexc_handler, /* 0: Reserved */
#endif
    (uint32_t)clic_default_intexc_handler, /* 1: Reserved */
    (uint32_t)clic_default_intexc_handler, /* 2: Reserved */
    (uint32_t)clic_default_intexc_handler, /* 3: Machine software interrupt */
    (uint32_t)clic_default_intexc_handler, /* 4: Reserved */
    (uint32_t)clic_default_intexc_handler, /* 5: Reserved */
    (uint32_t)clic_default_intexc_handler, /* 6: Reserved */
    (uint32_t)clic_mtip_handler,           /* 7: Machine timer interrupt */
    (uint32_t)clic_default_intexc_handler, /* 8: Reserved */
    (uint32_t)clic_default_intexc_handler, /* 9: Reserved */
    (uint32_t)clic_default_intexc_handler, /* 10: Reserved */
    (uint32_t)clic_meip_handler,           /* 11: Machine external interrupt */
    (uint32_t)clic_csip_handler,           /* 12: Clic Software Interrupt */
    (uint32_t)clic_default_intexc_handler, /* 13: Reserved */
    (uint32_t)clic_default_intexc_handler, /* 14: Reserved */
    (uint32_t)clic_default_intexc_handler, /* 15: Reserved */
    (uint32_t)clic_default_intexc_handler,
};
#endif

#ifdef CONFIG_BASIC_HPM
/*--------------------------------------------------------------------------*/
/* HPM */
/****************************************************************************
 * Name: basic_e320_hpm_exist
 *
 * Description:
 *   Judge whether HPM counter exists.
 *
 * Input Parameters:
 *   counter- Counter ID.
 *
 * Returned Value:
 *   0- Operation of HPM counter succeeded.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
static int basic_e320_hpm_exist(basic_hpm_counter_t counter)
{
    if (counter == BASIC_HPM_TIME)
        return -BASICEREGRW;

    if ((counter < BASIC_HPM_CYCLE) || (counter > BASIC_HPM_COUNTER31))
        return -BASICEREGRW;
    else
        return 0;
}

/****************************************************************************
 * Name: basic_e320_hpm_write
 *
 * Description:
 *   Judge whether HPM counter writable.
 *
 * Input Parameters:
 *   counter- Counter ID.
 *
 * Returned Value:
 *   0- Operation of HPM counter succeeded.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
static int basic_e320_hpm_writeable(basic_hpm_counter_t counter)
{
    if (counter == BASIC_HPM_TIME)
        return -BASICEREGRW;

    if ((counter < BASIC_HPM_CYCLE) || (counter > BASIC_HPM_COUNTER6))
        return -BASICEREGRW;
    else
        return 0;
}
#endif /* CONFIG_BASIC_HPM */

#ifdef CONFIG_BASIC_PMP
/*--------------------------------------------------------------------------*/
/* PMP */
/****************************************************************************
 * Name: basic_e320_get_pmp_counter
 *
 * Description:
 *   Function to get the pmp counter.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   The counter of pmp.
 *
 ****************************************************************************/
static uint32_t basic_e320_get_pmp_counter(void)
{
    return PMP_ENTRY_NUM;
}
#endif

/*--------------------------------------------------------------------------*/
/* Ticks */
/****************************************************************************
 * Name: basic_e320_get_timer_freq
 *
 * Description:
 *   Function to get the timer frequencey.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   The number of timer frequencey.
 *
 ****************************************************************************/
static uint32_t basic_e320_get_timer_freq(void)
{
    return SystemTimerClock;
}

/****************************************************************************
 * Name: basic_e320_set_tick
 *
 * Description:
 *   Set the number of ticks riggered by the next timer interrupt.
 *
 * Input Parameters:
 *   tick- The number of ticks to be set in timer.
 *
 * Returned Value:
 *   Number of ticks before setting.
 *
 ****************************************************************************/
static uint64_t basic_e320_set_tick(uint64_t tick)
{
    uint64_t tick_value;

    BASIC_ASSERT(tick);

    tick_value = SysTimer_GetLoadValue();
    SysTimer_SetCompareValue(tick);
    return tick_value;
}

/****************************************************************************
 * Name: basic_e320_set_tick_interval
 *
 * Description:
 *   Set the next timer interrupt trigger time to tick.
 *
 * Input Parameters:
 *   tick- Time interval to be set.
 *
 * Returned Value:
 *   Time interval from next timer interrupt trigger.
 *
 ****************************************************************************/
static uint64_t basic_e320_set_tick_interval(uint64_t tick)
{
    BASIC_ASSERT(tick);

    uint64_t cur_ticks    = SysTimer_GetLoadValue();
    uint64_t reload_ticks = tick + cur_ticks;

    if (__USUALLY(reload_ticks > cur_ticks)) {
        SysTimer_SetCompareValue(reload_ticks);
    } else {
        /*
         * When added the ticks value, then the MTIMERCMP < TIMER,
         * which means the MTIMERCMP is overflowed,
         * so we need to reset the counter to zero
         */
        SysTimer_SetLoadValue(0);
        SysTimer_SetCompareValue(tick);
    }

    return tick;
}

/*--------------------------------------------------------------------------*/

/****************************************************************************
 * Name: basic_e320_usart_read(uint8_t *readbuf, uint8_t num)
 *
 * Description:
 *   Get hardware hard ID.

 * Input Parameters:
 *   readbuf-The usart read buf.
 *   num-The size of buf.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
static void basic_e320_usart_read(uint8_t *readbuf, uint8_t num)
{
    if (log_uart_nr < 0 || num <= 0 || NULL == readbuf) {
        return;
    }
    struct {
        __I  uint32_t VERID;                             /**< Version ID Register, offset: 0x0 */
        __I  uint32_t PARAM;                             /**< Parameter Register, offset: 0x4 */
        __IO uint32_t GLOBAL;                            /**< UART Global Register, offset: 0x8 */
        __IO uint32_t PINCFG;                            /**< UART Pin Configuration Register, offset: 0xC */
        __IO uint32_t BAUD;                              /**< UART Baud Rate Register, offset: 0x10 */
        __IO uint32_t STAT;                              /**< UART Status Register, offset: 0x14 */
        __IO uint32_t CTRL;                              /**< UART Control Register, offset: 0x18 */
        __IO uint32_t DATA;                              /**< UART Data Register, offset: 0x1C */
        __IO uint32_t MATCH;                             /**< UART Match Address Register, offset: 0x20 */
        __IO uint32_t MODIR;                             /**< UART Modem IrDA Register, offset: 0x24 */
        __IO uint32_t FIFO;                              /**< UART FIFO Register, offset: 0x28 */
        __IO uint32_t WATER;                             /**< UART Watermark Register, offset: 0x2C */
    } *pUartBase = (void *)(UART_0_BASEADDR + (log_uart_nr)*UART_0_SIZE);
    /* Enable uart receiver */
    pUartBase->CTRL = pUartBase->CTRL | (1 << 18);
    /* Wait for the register write operation to complete */
    while ((pUartBase->CTRL & (1 << 18)) == 0U)
        ;

    while (num > 0) {
        while (!(pUartBase->STAT & (1 << 21)))
            ;
        *readbuf = (uint8_t)pUartBase->DATA;
        readbuf++;
        num--;

        /* Clear error */
        if (pUartBase->STAT & (0x0F << 16)) {
            pUartBase->STAT |= (0x0F << 16);
            break;
        }
    }

    /* Disable uart receiver */
    pUartBase->CTRL &= (~(1 << 18));
    /* Wait for the register write operation to complete */
    while ((pUartBase->CTRL & (1 << 18))) {
    }

    /* Read dummy to clear RDRF flag */
    num = pUartBase->DATA;

    // UART_DRV_ReceiveDataPolling(log_uart_nr, readbuf, num); // TODO:zhoulei, should not use driver api

}

/****************************************************************************
 * Name: basic_e320_usart_write(uint8_t *readbuf, uint8_t num)
 *
 * Description:
 *   Get hardware hard ID.

 * Input Parameters:
 *   writebuf-The usart write buf.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
static void basic_e320_usart_write(uint8_t *writebuf, uint8_t num)
{
    // if (log_uart_nr >= 0) {
    //     UART_DRV_SendDataPolling(log_uart_nr, writebuf, num);
    // }
    if (log_uart_nr < 0 || num <= 0 || NULL == writebuf) {
        return;
    }
    struct {
        __I  uint32_t VERID;                             /**< Version ID Register, offset: 0x0 */
        __I  uint32_t PARAM;                             /**< Parameter Register, offset: 0x4 */
        __IO uint32_t GLOBAL;                            /**< UART Global Register, offset: 0x8 */
        __IO uint32_t PINCFG;                            /**< UART Pin Configuration Register, offset: 0xC */
        __IO uint32_t BAUD;                              /**< UART Baud Rate Register, offset: 0x10 */
        __IO uint32_t STAT;                              /**< UART Status Register, offset: 0x14 */
        __IO uint32_t CTRL;                              /**< UART Control Register, offset: 0x18 */
        __IO uint32_t DATA;                              /**< UART Data Register, offset: 0x1C */
        __IO uint32_t MATCH;                             /**< UART Match Address Register, offset: 0x20 */
        __IO uint32_t MODIR;                             /**< UART Modem IrDA Register, offset: 0x24 */
        __IO uint32_t FIFO;                              /**< UART FIFO Register, offset: 0x28 */
        __IO uint32_t WATER;                             /**< UART Watermark Register, offset: 0x2C */
    } *pUartBase = (void *)(UART_0_BASEADDR + (log_uart_nr)*UART_0_SIZE);
    /* Enable uart receiver */
    pUartBase->CTRL = pUartBase->CTRL | (1 << 19);
    /* Wait for the register write operation to complete */
    while ((pUartBase->CTRL & (1 << 19)) == 0U)
        ;

    while (num > 0) {
        while (!(pUartBase->STAT & (1 << 23)))
            ;

        volatile uint8_t *dataRegBytes = (volatile uint8_t *)(&(pUartBase->DATA));
        dataRegBytes[0]                = *writebuf;
        writebuf++;
        num--;
    }

    /* Disable uart receiver */
    pUartBase->CTRL &= (~(1 << 19));
    /* Wait for the register write operation to complete */
    while ((pUartBase->CTRL & (1 << 19))) {
        pUartBase->CTRL &= (~(1 << 19));
    }
}

const struct basic_platform_operations_s platform_ops = {
#ifdef CONFIG_BASIC_HPM
    .hpm_exist     = basic_e320_hpm_exist,
    .hpm_writeable = basic_e320_hpm_writeable,
#endif
#ifdef CONFIG_BASIC_PMP
    .get_pmp_counter = basic_e320_get_pmp_counter,
#endif
    .get_timer_freq    = basic_e320_get_timer_freq,
    .set_tick          = basic_e320_set_tick,
    .set_tick_interval = basic_e320_set_tick_interval,
    .usart_read        = basic_e320_usart_read,
    .usart_write       = basic_e320_usart_write,
};

__WEAK int main(void)
{
}

#ifdef EMPS_SIMULATION
#define PRINT_ADDR 0x1C01F7F8
static char traceBuff[128];
int32_t trace(const char *fmt, ...)
{
    int32_t num;
    va_list list;
    va_start(list, fmt);
    num = vsprintf(traceBuff, fmt, list);
    va_end(list);
    traceBuff[num] = '\0';
    for (int32_t i = 0; i < strlen(traceBuff); i++) {
        (*((volatile unsigned char *)(PRINT_ADDR))) = traceBuff[i];
    }
}
#else
int32_t trace(const char *fmt, ...)
{
    return 0;
}
#endif
