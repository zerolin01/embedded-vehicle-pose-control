/**
 * Copyright Statement:
 * This software and related documentation (ESWIN SOFTWARE) are protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * Beijing ESWIN Computing Technology Co., Ltd.(ESWIN)and/or its licensors.
 * Without the prior written permission of ESWIN and/or its licensors, any reproduction, modification,
 * use or disclosure Software, and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * Copyright ©[2023] [Beijing ESWIN Computing Technology Co., Ltd.]. All rights reserved.
 *
 * RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES THAT THE SOFTWARE
 * AND ITS DOCUMENTATIONS (ESWIN SOFTWARE) RECEIVED FROM ESWIN AND / OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. ESWIN EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON INFRINGEMENT.
 * NEITHER DOES ESWIN PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
 * WHICH MAY BE USED BY,INCORPORATED IN, OR SUPPLIED WITH THE ESWIN SOFTWARE,
 * AND RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ESWIN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file superio_hw_access.h
 * @brief SUPERIO driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERIO_HW_ACCESS_H__
#define __SUPERIO_HW_ACCESS_H__

#include "superio_register.h"
#include <stdbool.h>

/**
 * @brief Shift clock polarity options
 *
 */
typedef enum {
    SUPERIO_TIMER_POLARITY_POSEDGE = 0x00U, /**< Shift on positive edge of Shift clock */
    SUPERIO_TIMER_POLARITY_NEGEDGE = 0x01U, /**< Shift on negative edge of Shift clock */
} superio_timer_polarity_t;

/**
 * @brief Pin polarity options
 *
 */
typedef enum {
    SUPERIO_PIN_POLARITY_HIGH = 0x00U, /**< Pin is active high */
    SUPERIO_PIN_POLARITY_LOW  = 0x01U, /**< Pin is active low  */
} superio_pin_polarity_t;

/**
 * @brief Pin configuration options
 *
 */
typedef enum {
    SUPERIO_PIN_CONFIG_DISABLED     = 0x00U, /**< Shifter pin output disabled */
    SUPERIO_PIN_CONFIG_OPEN_DRAIN   = 0x01U, /**< Shifter pin open drain or bidirectional output enable */
    SUPERIO_PIN_CONFIG_BIDIR_OUTPUT = 0x02U, /**< Shifter pin bidirectional output data */
    SUPERIO_PIN_CONFIG_OUTPUT       = 0x03U, /**< Shifter pin output */
} superio_pin_config_t;

/**
 * @brief Shifter mode options
 *
 */
typedef enum {
    SUPERIO_SHIFTER_MODE_DISABLED         = 0x00U, /**< Shifter disabled */
    SUPERIO_SHIFTER_MODE_RECEIVE          = 0x01U, /**< Receive mode */
    SUPERIO_SHIFTER_MODE_TRANSMIT         = 0x02U, /**< Transmit mode */
    SUPERIO_SHIFTER_MODE_MATCH_STORE      = 0x04U, /**< Match Store mode */
    SUPERIO_SHIFTER_MODE_MATCH_CONTINUOUS = 0x05U, /**< Match Continuous mode */
} superio_shifter_mode_t;

/**
 * @brief Shifter input source options
 *
 */
typedef enum {
    SUPERIO_SHIFTER_SOURCE_PIN     = 0x00U, /**< Input source is selected pin */
    SUPERIO_SHIFTER_SOURCE_SHIFTER = 0x01U, /**< Input source is shifter N+1 output */
} superio_shifter_source_t;

/**
 * @brief Read/Write mode for shifter buffer
 *
 */
typedef enum {
    SUPERIO_SHIFTER_RW_MODE_NORMAL   = 0x00U, /**< Normal shifter buffer read/write */
    SUPERIO_SHIFTER_RW_MODE_BIT_SWAP = 0x01U, /**< Data is bit-swapped */
} superio_shifter_buffer_mode_t;

/**
 * @brief Trigger polarity
 *
 */
typedef enum {
    SUPERIO_TRIGGER_POLARITY_HIGH = 0x00U, /**< Trigger is active high */
    SUPERIO_TRIGGER_POLARITY_LOW  = 0x01U, /**< Trigger is active low  */
} superio_trigger_polarity_t;

/**
 * @brief Trigger sources
 *
 */
typedef enum {
    SUPERIO_TRIGGER_SOURCE_EXTERNAL = 0x00U, /**< External trigger selected */
    SUPERIO_TRIGGER_SOURCE_INTERNAL = 0x01U, /**< Internal trigger selected */
} superio_trigger_source_t;

/**
 * @brief Timer mode options
 *
 */
typedef enum {
    SUPERIO_TIMER_MODE_DISABLED  = 0x00U, /**< Timer Disabled. */
    SUPERIO_TIMER_MODE_8BIT_BAUD = 0x01U, /**< Dual 8-bit counters baud/bit mode. */
    SUPERIO_TIMER_MODE_8BIT_PWM  = 0x02U, /**< Dual 8-bit counters PWM mode. */
    SUPERIO_TIMER_MODE_16BIT     = 0x03U, /**< Single 16-bit counter mode. */
} superio_timer_mode_t;

/**
 * @brief Timer initial output options
 *
 */
typedef enum {
    SUPERIO_TIMER_INITOUT_ONE        = 0x00U, /**< Timer output is logic one when enabled, unaffected by timer reset. */
    SUPERIO_TIMER_INITOUT_ZERO       = 0x01U, /**< Timer output is logic zero when enabled, unaffected by timer reset. */
    SUPERIO_TIMER_INITOUT_ONE_RESET  = 0x02U, /**< Timer output is logic one when enabled and on timer reset. */
    SUPERIO_TIMER_INITOUT_ZERO_RESET = 0x03U, /**< Timer output is logic zero when enabled and on timer reset. */
} superio_timer_output_t;

/**
 * @brief Timer decrement options
 *
 */
typedef enum {
    SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR =
        0x00U, /**< Decrement counter on SuperIO clock, Shift clock equals Timer output. */
    SUPERIO_TIMER_DECREMENT_TRG_SHIFT_TMR =
        0x01U, /**< Decrement counter on Trigger input (both edges), Shift clock equals Timer output. */
    SUPERIO_TIMER_DECREMENT_PIN_SHIFT_PIN =
        0x02U, /**< Decrement counter on Pin input (both edges), Shift clock equals Pin input. */
    SUPERIO_TIMER_DECREMENT_TRG_SHIFT_TRG =
        0x03U, /**< Decrement counter on Trigger input (both edges), Shift clock equals Trigger input. */
} superio_timer_decrement_t;

/**
 * @brief Timer reset options
 *
 */
typedef enum {
    SUPERIO_TIMER_RESET_NEVER      = 0x00U, /**< Timer never reset. */
    SUPERIO_TIMER_RESET_PIN_OUT    = 0x02U, /**< Timer reset on Timer Pin equal to Timer Output. */
    SUPERIO_TIMER_RESET_TRG_OUT    = 0x03U, /**< Timer reset on Timer Trigger equal to Timer Output. */
    SUPERIO_TIMER_RESET_PIN_RISING = 0x04U, /**< Timer reset on Timer Pin rising edge. */
    SUPERIO_TIMER_RESET_TRG_RISING = 0x06U, /**< Timer reset on Trigger rising edge. */
    SUPERIO_TIMER_RESET_TRG_BOTH   = 0x07U, /**< Timer reset on Trigger rising or falling edge. */
} superio_timer_reset_t;

/**
 * @brief Timer disable options
 *
 */
typedef enum {
    SUPERIO_TIMER_DISABLE_NEVER           = 0x00U, /**< Timer never disabled. */
    SUPERIO_TIMER_DISABLE_TIM_DISABLE     = 0x01U, /**< Timer disabled on Timer N-1 disable. */
    SUPERIO_TIMER_DISABLE_TIM_CMP         = 0x02U, /**< Timer disabled on Timer compare. */
    SUPERIO_TIMER_DISABLE_TIM_CMP_TRG_LOW = 0x03U, /**< Timer disabled on Timer compare and Trigger Low. */
    SUPERIO_TIMER_DISABLE_PIN             = 0x04U, /**< Timer disabled on Pin rising or falling edge. */
    SUPERIO_TIMER_DISABLE_PIN_TRG_HIGH =
        0x05U,                         /**< Timer disabled on Pin rising or falling edge provided Trigger is high. */
    SUPERIO_TIMER_DISABLE_TRG = 0x06U, /**< Timer disabled on Trigger falling edge. */
} superio_timer_disable_t;

/**
 * @brief Timer enable options
 *
 */
typedef enum {
    SUPERIO_TIMER_ENABLE_ALWAYS               = 0x00U, /**< Timer always enabled. */
    SUPERIO_TIMER_ENABLE_TIM_ENABLE           = 0x01U, /**< Timer enabled on Timer N-1 enable. */
    SUPERIO_TIMER_ENABLE_TRG_HIGH             = 0x02U, /**< Timer enabled on Trigger high. */
    SUPERIO_TIMER_ENABLE_TRG_PIN_HIGH         = 0x03U, /**< Timer enabled on Trigger high and Pin high. */
    SUPERIO_TIMER_ENABLE_PIN_POSEDGE          = 0x04U, /**< Timer enabled on Pin rising edge. */
    SUPERIO_TIMER_ENABLE_PIN_POSEDGE_TRG_HIGH = 0x05U, /**< Timer enabled on Pin rising edge and Trigger high. */
    SUPERIO_TIMER_ENABLE_TRG_POSEDGE          = 0x06U, /**< Timer enabled on Trigger rising edge. */
    SUPERIO_TIMER_ENABLE_TRG_EDGE             = 0x07U, /**< Timer enabled on Trigger rising or falling edge. */
} superio_timer_enable_t;

/**
 * @brief Timer stop bit options
 *
 */
typedef enum {
    SUPERIO_TIMER_STOP_BIT_DISABLED    = 0x00U, /**< Stop bit disabled. */
    SUPERIO_TIMER_STOP_BIT_TIM_CMP     = 0x01U, /**< Stop bit is enabled on timer compare. */
    SUPERIO_TIMER_STOP_BIT_TIM_DIS     = 0x02U, /**< Stop bit is enabled on timer disable. */
    SUPERIO_TIMER_STOP_BIT_TIM_CMP_DIS = 0x03U, /**< Stop bit is enabled on timer compare and disable. */
} superio_timer_stop_t;

/**
 * @brief Timer stop bit options - for Transmit, Receive or Match Store modes only
 *
 */
typedef enum {
    SUPERIO_SHIFTER_STOP_BIT_DISABLED = 0x00U, /**< Stop bit disabled. */
    SUPERIO_SHIFTER_STOP_BIT_0        = 0x02U, /**< Transmit/expect stop bit value 0 */
    SUPERIO_SHIFTER_STOP_BIT_1        = 0x03U, /**< Transmit/expect stop bit value 1 */
} superio_shifter_stop_t;

/**
 * @brief Timer start bit options - for Transmit, Receive or Match Store modes only
 *
 */
typedef enum {
    SUPERIO_SHIFTER_START_BIT_DISABLED    = 0x00U, /**< Start bit disabled, transmitter loads data on enable */
    SUPERIO_SHIFTER_START_BIT_DISABLED_SH = 0x01U, /**< Start bit disabled, transmitter loads data on first shift */
    SUPERIO_SHIFTER_START_BIT_0           = 0x02U, /**< Transmit/expect start bit value 0 */
    SUPERIO_SHIFTER_START_BIT_1           = 0x03U, /**< Transmit/expect start bit value 1 */
} superio_shifter_start_t;

/**
 * @brief Timer start bit options
 *
 */
typedef enum {
    SUPERIO_TIMER_START_BIT_DISABLED = 0x00U, /**< Start bit disabled. */
    SUPERIO_TIMER_START_BIT_ENABLED  = 0x01U, /**< Start bit enabled. */
} superio_timer_start_t;

/**
 *  @brief      Timer Pin ouput states
 *  @details    This enumeration specifies timer pin out states SuperIO
 *
 */
typedef enum {
    SUPERIO_PWM_TIMER_PIN_OUTPUT_DISABLE = 0x00U, /**< Timer Pin output disabled */
    SUPERIO_PWM_TIMER_PIN_OUTPUT_ENABLE  = 0x03U, /**< Timer Pin Output mode */
} superio_pwmTimerPinType_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Get the number of shifters of the SuperIO module.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @return uint8_t the number of shifters.
 *
 */
static inline uint8_t SUPERIO_GetShifterNum(const superio_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->PARAM;
    tmp          = (tmp & SUPERIO_PARAM_SHIFTER_MASK) >> SUPERIO_PARAM_SHIFTER_SHIFT;
    return (uint8_t)tmp;
}

/**
 * @brief Enables of disables SuperIO functionality in debug mode.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param enable Whether to enable.
 *
 */
static inline void SUPERIO_SetDebugMode(superio_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->CTRL;
    regValue &= (uint32_t)(~(SUPERIO_CTRL_DBGE_MASK));
    regValue |= SUPERIO_CTRL_DBGE(enable);
    baseAddr->CTRL = (uint32_t)regValue;
}

/**
 * @brief Calling this function with enable parameter set to true resets all internal
 * master logic and registers, except the SuperIO Control Register. The reset state
 * persists until this function is called with enable parameter set to false.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param enable Whether to enable.
 *
 */
static inline void SUPERIO_SetSoftwareReset(superio_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->CTRL;
    regValue &= (uint32_t)(~(SUPERIO_CTRL_SWRST_MASK));
    regValue |= SUPERIO_CTRL_SWRST(enable);
    baseAddr->CTRL = (uint32_t)regValue;
}

/**
 * @brief Enables or disables the SuperIO module.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param enable Whether to enable.
 *
 */
static inline void SUPERIO_SetEnable(superio_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->CTRL;
    regValue &= (uint32_t)(~(SUPERIO_CTRL_FLEXEN_MASK));
    regValue |= SUPERIO_CTRL_FLEXEN(enable);
    baseAddr->CTRL = (uint32_t)regValue;
}

/**
 * @brief Returns the data read from all the SUPERIO pins. Only the lower
 * n bits are valid, where n is the number of pins returned by SUPERIO_GetPinNum().
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @return uint8_t The data read of all the SUPERIO pins.
 *
 */
static inline uint8_t SUPERIO_GetPinData(const superio_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->PIN;
    tmp          = (tmp & SUPERIO_PIN_PDI_MASK) >> SUPERIO_PIN_PDI_SHIFT;
    return (uint8_t)tmp;
}

/**
 * @brief This function returns the value of the status flag for the specified shifter.
 * The meaning of the status flag depends on the current mode.
 *  - Transmit mode: shifter buffer is empty and ready to accept more data
 *  - Receive mode: shifter buffer is full and received data can be read from it
 *  - Match Store mode: match occurred between shifter buffer and shifter
 *  - Match Continuous mode: current match result between shifter buffer and shifter
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @return The shifter status.
 *
 */
static inline bool SUPERIO_GetShifterStatus(const superio_type_t *baseAddr, uint8_t shifter)
{
    uint32_t regValue = (uint32_t)baseAddr->SHIFTSTAT;
    regValue          = (regValue >> shifter) & 1U;
    return (bool)regValue;
}

/**
 * @brief This function returns the value of the status flags for all shifters. Each bit in
 * the returned value specifies the status for one shifter, starting with  * shifter 0 from
 * least significant bit.
 * The meaning of the status flag depends on the current mode.
 *  - Transmit mode: shifter buffer is empty and ready to accept more data
 *  - Receive mode: shifter buffer is full and received data can be read from it
 *  - Match Store mode: match occurred between shifter buffer and shifter
 *  - Match Continuous mode: current match result between shifter buffer and shifter
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @return uint32_t All the shifter status.
 *
 */
static inline uint32_t SUPERIO_GetAllShifterStatus(const superio_type_t *baseAddr)
{
    return baseAddr->SHIFTSTAT;
}

/**
 * @brief This function clears the status flag for the specified shifter. This is possible in
 * all modes except Match Continuous mode.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 *
 */
static inline void SUPERIO_ClearShifterStatus(superio_type_t *baseAddr, uint8_t shifter)
{
    baseAddr->SHIFTSTAT = 1UL << shifter;
}

/**
 * @brief This function returns the value of the error status flag for the specified shifter.
 * The meaning of the error status flag depends on the current mode.
 *  - Transmit mode: shifter buffer was not written before it was transferred in the shifter (buffer overrun)
 *  - Receive mode: shifter buffer was not read before new data was transferred from the shifter (buffer underrun)
 *  - Match Store mode: match event occurred before the previous match data was read from shifter buffer (buffer
 * overrun)
 *  - Match Continuous mode: match occurred between shifter buffer and shifter
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @return The shifter error status.
 *
 */
static inline bool SUPERIO_GetShifterErrorStatus(const superio_type_t *baseAddr, uint8_t shifter)
{
    uint32_t regValue = (uint32_t)baseAddr->SHIFTERR;
    regValue          = (regValue >> shifter) & 1U;
    return (bool)regValue;
}

/**
 * @brief This function returns the value of the error status flags for all shifters. Each bit in
 * the returned value specifies the error status for one shifter, starting with
 * shifter 0 from least significant bit.
 * The meaning of the error status flag depends on the current mode.
 *  - Transmit mode: shifter buffer was not written before it was transferred in the shifter (buffer overrun)
 *  - Receive mode: shifter buffer was not read before new data was transferred from the shifter (buffer underrun)
 *  - Match Store mode: match event occurred before the previous match data was read from shifter buffer (buffer
 * overrun)
 *  - Match Continuous mode: match occurred between shifter buffer and shifter
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @return uint32_t All the shifter error status.
 *
 */
static inline uint32_t SUPERIO_GetAllShifterErrorStatus(const superio_type_t *baseAddr)
{
    return baseAddr->SHIFTERR;
}

/**
 * @brief This function clears the error status flag for the specified shifter.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 *
 */
static inline void SUPERIO_ClearShifterErrorStatus(superio_type_t *baseAddr, uint8_t shifter)
{
    baseAddr->SHIFTERR = 1UL << shifter;
}

/**
 * @brief This function returns the value of the status flag for the specified timer.
 * The meaning of the status flag depends on the current mode.
 *  - 8-bit counter mode: the timer status flag is set when the upper 8-bit counter equals zero
 * and decrements. This also causes the counter to reload with the value in the compare register.
 *  - 8-bit PWM mode: the upper 8-bit counter equals zero and decrements. This also causes the
 * counter to reload with the value in the compare register.
 *  - 16-bit counter mode: the 16-bit counter equals zero and decrements. This also causes the
 * counter to reload with the value in the compare register.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @return The timer status.
 *
 */
static inline bool SUPERIO_GetTimerStatus(const superio_type_t *baseAddr, uint8_t timer)
{
    uint32_t regValue = (uint32_t)baseAddr->TIMSTAT;
    regValue          = (regValue >> timer) & 1U;
    return (bool)regValue;
}

/**
 * @brief This function returns the value of the status flags for all timers. Each bit in
 * the returned value specifies the status for one timer, starting with
 * timer 0 from least significant bit.
 * The meaning of the status flag depends on the current mode.
 *  - 8-bit counter mode: the timer status flag is set when the upper 8-bit counter equals zero
 * and decrements. This also causes the counter to reload with the value in the compare register.
 *  - 8-bit PWM mode: the upper 8-bit counter equals zero and decrements. This also causes the
 * counter to reload with the value in the compare register.
 *  - 16-bit counter mode: the 16-bit counter equals zero and decrements. This also causes the
 * counter to reload with the value in the compare register.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @return uint32_t All the timer status.
 *
 */
static inline uint32_t SUPERIO_GetAllTimerStatus(const superio_type_t *baseAddr)
{
    return baseAddr->TIMSTAT;
}

/**
 * @brief This function clears the status flag for the specified timer.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 *
 */
static inline void SUPERIO_ClearTimerStatus(superio_type_t *baseAddr, uint8_t timer)
{
    baseAddr->TIMSTAT = 1UL << timer;
}

/**
 * @brief Returns the state of the interrupt for all shifters. Each bit in
 * the returned value specifies the interrupt state for one shifter, starting with
 * shifter 0 from least significant bit.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @return uint32_t All the shifter interrupt.
 *
 */
static inline uint32_t SUPERIO_GetAllShifterInterrupt(const superio_type_t *baseAddr)
{
    return baseAddr->SHIFTSIEN;
}

/**
 * @brief Enable or disable specified shifter interrupts. The interrupt mask must contain a
 * bit of 1 for each shifter who's interrupt must be enabled or disabled.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param interruptMask The interrupt mask.
 * @param enable Whether to enable.
 *
 */
static inline void SUPERIO_SetShifterInterrupt(superio_type_t *baseAddr, uint8_t interruptMask, bool enable)
{
    uint32_t tmp;

    tmp = baseAddr->SHIFTSIEN;

    if (enable == true) {
        tmp |= interruptMask;
    } else {
        tmp &= ~interruptMask;
    }
    baseAddr->SHIFTSIEN = tmp;
}

/**
 * @brief Returns the state of the error interrupt for all shifters. Each bit in
 * the returned value specifies the interrupt state for one shifter, starting with
 * shifter 0 from least significant bit.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @return uint32_t All the shifter error interrupt.
 *
 */
static inline uint32_t SUPERIO_GetAllShifterErrorInterrupt(const superio_type_t *baseAddr)
{
    return baseAddr->SHIFTEIEN;
}

/**
 * @brief Enable or disable specified shifter error interrupts. The interrupt mask must contain a
 * bit of 1 for each shifter who's error interrupt must be enabled or disabled.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param interruptMask The interrupt mask.
 * @param enable Whether to enable.
 *
 */
static inline void SUPERIO_SetShifterErrorInterrupt(superio_type_t *baseAddr, uint8_t interruptMask, bool enable)
{
    uint32_t tmp;

    tmp = baseAddr->SHIFTEIEN;

    if (enable == true) {
        tmp |= interruptMask;
    } else {
        tmp &= ~interruptMask;
    }
    baseAddr->SHIFTEIEN = tmp;
}

/**
 * @brief Returns the state of the interrupt for all timers. Each bit in
 * the returned value specifies the interrupt state for one timer, starting with
 * timer 0 from least significant bit.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @return uint32_t All the timer interrupt.
 *
 */
static inline uint32_t SUPERIO_GetAllTimerInterrupt(const superio_type_t *baseAddr)
{
    return baseAddr->TIMIEN;
}

/**
 * @brief Enable or disable specified timer interrupts. The interrupt mask must contain a
 * bit of 1 for each timer who's interrupt must be enabled or disabled.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param interruptMask The interrupt mask.
 * @param enable Whether to enable.
 *
 */
static inline void SUPERIO_SetTimerInterrupt(superio_type_t *baseAddr, uint8_t interruptMask, bool enable)
{
    uint32_t tmp;

    tmp = baseAddr->TIMIEN;

    if (enable == true) {
        tmp |= interruptMask;
    } else {
        tmp &= ~interruptMask;
    }
    baseAddr->TIMIEN = tmp;
}

/**
 * @brief Enable or disable specified shifter PDMA requests. The request mask must contain a
 * bit of 1 for each shifter who's PDMA requests must be enabled or disabled.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param requestMask The request mask.
 * @param enable Whether to enable.
 *
 */
static inline void SUPERIO_SetShifterDMARequest(superio_type_t *baseAddr, uint8_t requestMask, bool enable)
{
    uint32_t tmp;

    tmp = baseAddr->SHIFTSDEN;

    if (enable == true) {
        tmp |= requestMask;
    } else {
        tmp &= ~requestMask;
    }
    baseAddr->SHIFTSDEN = tmp;
}

/**
 * @brief This function assigns a timer to control the specified shifter, and also configures its polarity.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param timer The timer index.
 * @param polarity The polarity.
 *
 */
static inline void SUPERIO_SetShifterTimer(superio_type_t *baseAddr, uint8_t shifter, uint8_t timer,
                                           superio_timer_polarity_t polarity)
{
    uint32_t tmp = baseAddr->SHIFTCTL[shifter];
    tmp &= ~(SUPERIO_SHIFTCTL_TIMSEL_MASK);
    tmp |= SUPERIO_SHIFTCTL_TIMSEL(timer);
    tmp &= ~(SUPERIO_SHIFTCTL_TIMPOL_MASK);
    tmp |= SUPERIO_SHIFTCTL_TIMPOL(polarity);
    baseAddr->SHIFTCTL[shifter] = tmp;
}

/**
 * @brief This function assigns the specified pin to the specified shifter, and also sets
 * its polarity and configuration.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param pin The pin index.
 * @param polarity The polarity.
 * @param config The pin config.
 *
 */
static inline void SUPERIO_SetShifterPin(superio_type_t *baseAddr, uint8_t shifter, uint8_t pin,
                                         superio_pin_polarity_t polarity, superio_pin_config_t config)
{
    uint32_t tmp = baseAddr->SHIFTCTL[shifter];
    tmp &= ~(SUPERIO_SHIFTCTL_PINSEL_MASK);
    tmp |= SUPERIO_SHIFTCTL_PINSEL(pin);
    tmp &= ~(SUPERIO_SHIFTCTL_PINPOL_MASK);
    tmp |= SUPERIO_SHIFTCTL_PINPOL(polarity);
    tmp &= ~(SUPERIO_SHIFTCTL_PINCFG_MASK);
    tmp |= SUPERIO_SHIFTCTL_PINCFG(config);
    baseAddr->SHIFTCTL[shifter] = tmp;
}

/**
 * @brief This function configures the pin assigned to the specified specified shifter.
 * It does not change the other pin-related settings.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param config The pin config.
 *
 */
static inline void SUPERIO_SetShifterPinConfig(superio_type_t *baseAddr, uint8_t shifter, superio_pin_config_t config)
{
    uint32_t tmp = baseAddr->SHIFTCTL[shifter];
    tmp &= ~(SUPERIO_SHIFTCTL_PINCFG_MASK);
    tmp |= SUPERIO_SHIFTCTL_PINCFG(config);
    baseAddr->SHIFTCTL[shifter] = tmp;
}

/**
 * @brief This function configures the mode for the specified shifter.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param mode The shifter mode.
 *
 */
static inline void SUPERIO_SetShifterMode(superio_type_t *baseAddr, uint8_t shifter, superio_shifter_mode_t mode)
{
    uint32_t tmp = baseAddr->SHIFTCTL[shifter];
    tmp &= ~(SUPERIO_SHIFTCTL_SMOD_MASK);
    tmp |= SUPERIO_SHIFTCTL_SMOD(mode);
    baseAddr->SHIFTCTL[shifter] = tmp;
}

/**
 * @brief This function configures the control settings for the specified shifter: mode settings,
 * pin settings and timer settings.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param mode The shifter mode.
 * @param pin The pin index.
 * @param pinPolarity The pin polarity.
 * @param pinConfig The pin config.
 * @param timer The timer index.
 * @param timerPolarity The timer polarity.
 *
 */
static inline void SUPERIO_SetShifterControl(superio_type_t *baseAddr, uint8_t shifter, superio_shifter_mode_t mode,
                                             uint8_t pin, superio_pin_polarity_t pinPolarity,
                                             superio_pin_config_t pinConfig, uint8_t timer,
                                             superio_timer_polarity_t timerPolarity)
{
    baseAddr->SHIFTCTL[shifter] = SUPERIO_SHIFTCTL_SMOD(mode) | SUPERIO_SHIFTCTL_PINPOL(pinPolarity) |
                                  SUPERIO_SHIFTCTL_PINSEL(pin) | SUPERIO_SHIFTCTL_PINCFG(pinConfig) |
                                  SUPERIO_SHIFTCTL_TIMPOL(timerPolarity) | SUPERIO_SHIFTCTL_TIMSEL(timer);
}

/**
 * @brief This function configures the input source of the specified shifter.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param source The shifter source.
 *
 */
static inline void SUPERIO_SetShifterInputSource(superio_type_t *baseAddr, uint8_t shifter,
                                                 superio_shifter_source_t source)
{
    uint32_t tmp;
    tmp = baseAddr->SHIFTCFG[shifter];
    tmp &= ~(SUPERIO_SHIFTCFG_INSRC_MASK);
    tmp |= SUPERIO_SHIFTCFG_INSRC(source);
    baseAddr->SHIFTCFG[shifter] = tmp;
}

/**
 * @brief This function configures the sending or receiving of
 * a stop bit in Transmit, Receive or Match Store modes.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param stop The shifter stop.
 *
 */
static inline void SUPERIO_SetShifterStopBit(superio_type_t *baseAddr, uint8_t shifter, superio_shifter_stop_t stop)
{
    uint32_t tmp;
    tmp = baseAddr->SHIFTCFG[shifter];
    tmp &= ~(SUPERIO_SHIFTCFG_SSTOP_MASK);
    tmp |= SUPERIO_SHIFTCFG_SSTOP(stop);
    baseAddr->SHIFTCFG[shifter] = tmp;
}

/**
 * @brief This function configures the sending or receiving of
 * a start bit in Transmit, Receive or Match Store modes.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param start The shifter start.
 *
 */
static inline void SUPERIO_SetShifterStartBit(superio_type_t *baseAddr, uint8_t shifter, superio_shifter_start_t start)
{
    uint32_t tmp;
    tmp = baseAddr->SHIFTCFG[shifter];
    tmp &= ~(SUPERIO_SHIFTCFG_SSTART_MASK);
    tmp |= SUPERIO_SHIFTCFG_SSTART(start);
    baseAddr->SHIFTCFG[shifter] = tmp;
}

/**
 * @brief This function sets the following configurations for the specified shifter:
 * start bit, stop bit, input source
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param start The shifter start.
 * @param stop The shifter stop.
 * @param source The shifter source.
 *
 */
static inline void SUPERIO_SetShifterConfig(superio_type_t *baseAddr, uint8_t shifter, superio_shifter_start_t start,
                                            superio_shifter_stop_t stop, superio_shifter_source_t source)
{
    baseAddr->SHIFTCFG[shifter] =
        SUPERIO_SHIFTCFG_SSTART(start) | SUPERIO_SHIFTCFG_SSTOP(stop) | SUPERIO_SHIFTCFG_INSRC(source);
}

/**
 * @brief This function reads data from the specified shifter buffer. The data can be
 * read in any of the four ways allowed by the hardware - see description of type
 * superio_shifter_buffer_mode_t for details.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param mode The shifter buffer mode.
 * @return The data in shifter buffer.
 *
 */
static inline uint32_t SUPERIO_ReadShifterBuffer(const superio_type_t *baseAddr, uint8_t shifter,
                                                 superio_shifter_buffer_mode_t mode)
{
    uint32_t data;

    switch (mode) {
    case SUPERIO_SHIFTER_RW_MODE_NORMAL:
        data = baseAddr->SHIFTBUF[shifter];
        break;
    case SUPERIO_SHIFTER_RW_MODE_BIT_SWAP:
        data = baseAddr->SHIFTBUFBIS[shifter];
        break;
    default:
        /* bad read mode */
        data = 0U;
        break;
    }
    return data;
}

/**
 * @brief This function writes data in the specified shifter buffer. The data can be
 * written in any of the four ways allowed by the hardware - see description of type
 * superio_shifter_buffer_mode_t for details.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param shifter The shifter index.
 * @param value The data written into shifter buffer.
 * @param mode The shifter buffer mode.
 *
 */
static inline void SUPERIO_WriteShifterBuffer(superio_type_t *baseAddr, uint8_t shifter, uint32_t value,
                                              superio_shifter_buffer_mode_t mode)
{
    switch (mode) {
    case SUPERIO_SHIFTER_RW_MODE_NORMAL:
        baseAddr->SHIFTBUF[shifter] = value;
        break;
    case SUPERIO_SHIFTER_RW_MODE_BIT_SWAP:
        baseAddr->SHIFTBUFBIS[shifter] = value;
        break;
    default:
        /* bad write mode */
        break;
    }
}

/**
 * @brief This function configures the trigger for the specified timer, and also its
 * source (internal or external) and polarity settings.
 * For internal triggers, the selection is as follows:
 *  - 4*N - Pin 2*N input
 *  - 4*N+1 - Shifter N status flag
 *  - 4*N+2 - Pin 2*N+1 input
 *  - 4*N+3 - Timer N trigger output
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param trigger the trigger index.
 * @param polarity The trigger polarity.
 * @param source The trigger source.
 *
 */
static inline void SUPERIO_SetTimerTrigger(superio_type_t *baseAddr, uint8_t timer, uint8_t trigger,
                                           superio_trigger_polarity_t polarity, superio_trigger_source_t source)
{
    uint32_t tmp = baseAddr->TIMCTL[timer];
    tmp &= ~(SUPERIO_TIMCTL_TRGSEL_MASK);
    tmp |= SUPERIO_TIMCTL_TRGSEL(trigger);
    tmp &= ~(SUPERIO_TIMCTL_TRGPOL_MASK);
    tmp |= SUPERIO_TIMCTL_TRGPOL(polarity);
    tmp &= ~(SUPERIO_TIMCTL_TRGSRC_MASK);
    tmp |= SUPERIO_TIMCTL_TRGSRC(source);
    baseAddr->TIMCTL[timer] = tmp;
}

/**
 * @brief This function assigns the specified pin to the specified timer, and also sets
 * its polarity and configuration.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param pin The pin index.
 * @param polarity The pin polarity.
 * @param config The pin config.
 *
 */
static inline void SUPERIO_SetTimerPin(superio_type_t *baseAddr, uint8_t timer, uint8_t pin,
                                       superio_pin_polarity_t polarity, superio_pin_config_t config)
{
    uint32_t tmp = baseAddr->TIMCTL[timer];
    tmp &= ~(SUPERIO_TIMCTL_PINSEL_MASK);
    tmp |= SUPERIO_TIMCTL_PINSEL(pin);
    tmp &= ~(SUPERIO_TIMCTL_PINPOL_MASK);
    tmp |= SUPERIO_TIMCTL_PINPOL(polarity);
    tmp &= ~(SUPERIO_TIMCTL_PINCFG_MASK);
    tmp |= SUPERIO_TIMCTL_PINCFG(config);
    baseAddr->TIMCTL[timer] = tmp;
}

/**
 * @brief This function configures the mode for the specified timer.
 * In 8-bit counter mode, the lower 8-bits of the counter and compare register are used to
 * configure the baud rate of the timer shift clock and the upper 8-bits are used to configure
 * the shifter bit count.
 * In 8-bit PWM mode, the lower 8-bits of the counter and compare
 * register are used to configure the high period of the timer shift clock and the upper
 * 8-bits are used to configure the low period of the timer shift clock. The shifter bit count
 * is configured using another timer or external signal.
 * In 16-bit counter mode, the full 16-bits of the counter and compare register are used to
 * configure either the baud rate of the shift clock or the shifter bit count.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param mode The timer mode.
 *
 */
static inline void SUPERIO_SetTimerMode(superio_type_t *baseAddr, uint8_t timer, superio_timer_mode_t mode)
{
    uint32_t tmp = baseAddr->TIMCTL[timer];
    tmp &= ~(SUPERIO_TIMCTL_TIMOD_MASK);
    tmp |= SUPERIO_TIMCTL_TIMOD(mode);
    baseAddr->TIMCTL[timer] = tmp;
}

/**
 * @brief This function configures the control settings for the specified timer: mode settings,
 * pin settings and trigger settings.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param trigger The trigger index.
 * @param triggerPolarity The trigger polarity.
 * @param triggerSource The trigger source.
 * @param pin The pin index.
 * @param pinPolarity The pin polarity.
 * @param pinConfig The pin config.
 * @param mode The timer mode.
 *
 */
static inline void SUPERIO_SetTimerControl(superio_type_t *baseAddr, uint8_t timer, uint8_t trigger,
                                           superio_trigger_polarity_t triggerPolarity,
                                           superio_trigger_source_t triggerSource, uint8_t pin,
                                           superio_pin_polarity_t pinPolarity, superio_pin_config_t pinConfig,
                                           superio_timer_mode_t mode)
{
    baseAddr->TIMCTL[timer] = SUPERIO_TIMCTL_TIMOD(mode) | SUPERIO_TIMCTL_PINSEL(pin) |
                              SUPERIO_TIMCTL_PINPOL(pinPolarity) | SUPERIO_TIMCTL_PINCFG(pinConfig) |
                              SUPERIO_TIMCTL_TRGSRC(triggerSource) | SUPERIO_TIMCTL_TRGPOL(triggerPolarity) |
                              SUPERIO_TIMCTL_TRGSEL(trigger);
}

/**
 * @brief This function configures the initial output of the specified timer and whether it is
 * affected by the Timer reset.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param output The timer output.
 *
 */
static inline void SUPERIO_SetTimerInitialOutput(superio_type_t *baseAddr, uint8_t timer, superio_timer_output_t output)
{
    uint32_t tmp = baseAddr->TIMCFG[timer];
    tmp &= ~(SUPERIO_TIMCFG_TIMOUT_MASK);
    tmp |= SUPERIO_TIMCFG_TIMOUT(output);
    baseAddr->TIMCFG[timer] = tmp;
}

/**
 * @brief This function configures the decrement condition for the specified timer and the source
 * of the shift clock. See description of type superio_timer_decrement_t for a list of options.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param decrement The timer decrement.
 *
 */
static inline void SUPERIO_SetTimerDecrement(superio_type_t *baseAddr, uint8_t timer,
                                             superio_timer_decrement_t decrement)
{
    uint32_t tmp = baseAddr->TIMCFG[timer];
    tmp &= ~(SUPERIO_TIMCFG_TIMDEC_MASK);
    tmp |= SUPERIO_TIMCFG_TIMDEC(decrement);
    baseAddr->TIMCFG[timer] = tmp;
}

/**
 * @brief This function configures the conditions that cause the timer counter (and optionally
 * output) to be reset. In 8-bit counter mode, the timer reset will only reset the
 * lower 8-bits that configure the baud rate. In all other modes, the timer reset
 * will reset the full 16-bits of the counter.
 * See description of type superio_timer_decrement_t for a list of options.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param reset The timer reset.
 *
 */
static inline void SUPERIO_SetTimerReset(superio_type_t *baseAddr, uint8_t timer, superio_timer_reset_t reset)
{
    uint32_t tmp = baseAddr->TIMCFG[timer];
    tmp &= ~(SUPERIO_TIMCFG_TIMRST_MASK);
    tmp |= SUPERIO_TIMCFG_TIMRST(reset);
    baseAddr->TIMCFG[timer] = tmp;
}

/**
 * @brief This function configures the condition that cause the specified timer to be disabled.
 * See description of type superio_timer_disable_t for a list of options.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param disable The timer disable.
 *
 */
static inline void SUPERIO_SetTimerDisable(superio_type_t *baseAddr, uint8_t timer, superio_timer_disable_t disable)
{
    uint32_t tmp = baseAddr->TIMCFG[timer];
    tmp &= ~(SUPERIO_TIMCFG_TIMDIS_MASK);
    tmp |= SUPERIO_TIMCFG_TIMDIS(disable);
    baseAddr->TIMCFG[timer] = tmp;
}

/**
 * @brief This function configures the condition that cause the specified timer to be enabled
 * and start decrementing.
 * See description of type superio_timer_disable_t for a list of options.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param enable The timer enable.
 *
 */
static inline void SUPERIO_SetTimerEnable(superio_type_t *baseAddr, uint8_t timer, superio_timer_enable_t enable)
{
    uint32_t tmp = baseAddr->TIMCFG[timer];
    tmp &= ~(SUPERIO_TIMCFG_TIMENA_MASK);
    tmp |= SUPERIO_TIMCFG_TIMENA(enable);
    baseAddr->TIMCFG[timer] = tmp;
}

/**
 * @brief This function configures stop bit insertion for the specified timer.
 * The stop bit can be added on a timer compare (between each word) or on a timer disable.
 * When stop bit is enabled, configured shifters will output the contents of the stop bit
 * when the timer is disabled. When stop bit is enabled on timer disable, the timer remains
 * disabled until the next rising edge of the shift clock. If configured for both timer
 * compare and timer disable, only one stop bit is inserted on timer disable.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param stop The timer stop.
 *
 */
static inline void SUPERIO_SetTimerStop(superio_type_t *baseAddr, uint8_t timer, superio_timer_stop_t stop)
{
    uint32_t tmp = baseAddr->TIMCFG[timer];
    tmp &= ~(SUPERIO_TIMCFG_TSTOP_MASK);
    tmp |= SUPERIO_TIMCFG_TSTOP(stop);
    baseAddr->TIMCFG[timer] = tmp;
}

/**
 * @brief This function configures start bit insertion for the specified timer.
 * When start bit is enabled, configured shifters will output the contents of the start
 * bit when the timer is enabled and the timer counter will reload from the compare
 * register on the first rising edge of the shift clock.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param start The timer start.
 *
 */
static inline void SUPERIO_SetTimerStart(superio_type_t *baseAddr, uint8_t timer, superio_timer_start_t start)
{
    uint32_t regValue = (uint32_t)baseAddr->TIMCFG[timer];
    regValue &= (uint32_t)(~(SUPERIO_TIMCFG_TSTART_MASK));
    regValue |= SUPERIO_TIMCFG_TSTART(start);
    baseAddr->TIMCFG[timer] = (uint32_t)regValue;
}

/**
 * @brief This function sets the following configurations for the specified timer:
 * start bit, stop bit, enable condition, disable condition, reset condition,
 * decrement condition, initial output
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param start The timer start.
 * @param stop The timer stop.
 * @param enable The timer enable.
 * @param disable The timer disable.
 * @param reset The timer reset.
 * @param decrement The timer decrement.
 * @param output The timer output.
 *
 */
static inline void SUPERIO_SetTimerConfig(superio_type_t *baseAddr, uint8_t timer, superio_timer_start_t start,
                                          superio_timer_stop_t stop, superio_timer_enable_t enable,
                                          superio_timer_disable_t disable, superio_timer_reset_t reset,
                                          superio_timer_decrement_t decrement, superio_timer_output_t output)
{

    baseAddr->TIMCFG[timer] = SUPERIO_TIMCFG_TSTART(start) | SUPERIO_TIMCFG_TSTOP(stop) |
                              SUPERIO_TIMCFG_TIMENA(enable) | SUPERIO_TIMCFG_TIMDIS(disable) |
                              SUPERIO_TIMCFG_TIMRST(reset) | SUPERIO_TIMCFG_TIMDEC(decrement) |
                              SUPERIO_TIMCFG_TIMOUT(output);
}

/**
 * @brief This function returns the compare value currently set for the specified timer.
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @return The value of timer compare.
 *
 */
static inline uint16_t SUPERIO_GetTimerCompare(const superio_type_t *baseAddr, uint8_t timer)
{
    return (uint16_t)(baseAddr->TIMCMP[timer]);
}

/**
 * @brief This function configures the compare value for the specified timer.
 * The timer compare value is loaded into the timer counter when the timer is first enabled,
 * when the timer is reset and when the timer decrements down to zero.
 * In dual 8-bit counters baud/bit mode, the lower 8-bits configure the baud rate divider
 * and the upper 8-bits configure the number of bits in each word.
 * In dual 8-bit counters PWM mode, the lower 8-bits configure the high period of the output
 * and the upper 8-bits configure the low period.
 * In 16-bit counter mode, the compare value can be used to generate the baud rate divider
 * (if shift clock source is timer output) or the number of bits in each word (when the shift
 * clock source is a pin or trigger input).
 *
 * @param baseAddr The SuperIO Base address pointer.
 * @param timer The timer index.
 * @param value the value to set timer compare.
 *
 */
static inline void SUPERIO_SetTimerCompare(superio_type_t *baseAddr, uint8_t timer, uint16_t value)
{
    baseAddr->TIMCMP[timer] = (uint32_t)value;
}

/**
 * @brief This function initializes all the registers of the SuperIO module to
 * their reset value.
 *
 * @param baseAddr The SuperIO Base address pointer.
 *
 */
static inline void SUPERIO_Init(superio_type_t *baseAddr)
{
    /* Use software reset bit to reset the module */
    SUPERIO_SetSoftwareReset(baseAddr, true);
    /* Control register is not affected by software reset */
    baseAddr->CTRL = 0x0U;
}

/**
 *  @brief      Return logic state of the pin output
 *
 *  @param  Base The SuperIO Base address pointer
 *  @param  Pin The Pin id for the given instance
 *
 *  @return     Logic state of the selected pin
 *
 */
static inline bool SUPERIO_PWM_GetPinState(const superio_type_t *const Base, uint8_t Pin)
{
    return (bool)((((Base->PIN & SUPERIO_PIN_PDI_MASK) >> Pin) & 0x1U) == 0x1U) ? true : false;
}

/**
 *  @brief      Return if the flag event occurred on one timer
 *
 *  @param  Base    The SuperIO Base address pointer
 *  @param  Timer   The Timer id for the given instance
 *
 *  @return     Value of the flag for selected timer
 *
 */
static inline bool SUPERIO_PWM_GetTimerFlag(const superio_type_t *const Base, uint8_t Timer)
{
    return (bool)((((Base->TIMSTAT & SUPERIO_TIMSTAT_TSF_MASK) >> Timer) & 0x1U) == 0x1U) ? true : false;
}

/**
 *  @brief      Enable/Disable the pin out for the selected timer
 *
 *  @param  Base        The SuperIO Base address pointer
 *  @param  Timer       The Timer id for the given instance
 *  @param  PinMode     The Timer Pin output mode
 *
 */
static inline void SUPERIO_PWM_SetTimerPinOutput(superio_type_t *Base, uint8_t Timer, superio_pin_config_t PinMode)
{
    Base->TIMCTL[Timer] = (Base->TIMCTL[Timer] & ~SUPERIO_TIMCTL_PINCFG_MASK) | SUPERIO_TIMCTL_PINCFG(PinMode);
}

/**
 *  @brief      Select the pin that the selected timer will ouput the signal
 *
 *  @param  Base    The SuperIO Base address pointer
 *  @param  Timer   The Timer id for the given instance
 *  @param  Pin     The Pin id used to output pwm signal
 *
 */
static inline void SUPERIO_PWM_SetTimerPin(superio_type_t *Base, uint8_t Timer, uint8_t Pin)
{
    Base->TIMCTL[Timer] = (Base->TIMCTL[Timer] & ~SUPERIO_TIMCTL_PINSEL_MASK) | SUPERIO_TIMCTL_PINSEL(Pin);
}

/**
 *  @brief      Select the pin polarity
 *
 *  @param  Base        The SuperIO Base address pointer
 *  @param  Timer       The Timer id for the given instance
 *  @param  Polarity    The Pin polarity type
 *
 */
static inline void SUPERIO_PWM_SetTimerPinPolarity(superio_type_t *Base, uint8_t Timer, superio_pin_polarity_t Polarity)
{
    Base->TIMCTL[Timer] = (Base->TIMCTL[Timer] & ~SUPERIO_TIMCTL_PINPOL_MASK) | SUPERIO_TIMCTL_PINPOL(Polarity);
}

/**
 *  @brief      Set the upper 8 bit value for timer comparator
 *
 *  @param  Base    The SuperIO Base address pointer
 *  @param  Timer   The Timer id for the given instance
 *  @param  Value   The value of the upper 8 bits of timer comparator
 *
 */
static inline void SUPERIO_PWM_SetUpperValue(superio_type_t *Base, uint8_t Timer, uint8_t Value)
{
    Base->TIMCMP[Timer] =
        (Base->TIMCMP[Timer] & ~SUPERIO_PWM_TIMCMP_CMP_UPPER_MASK) | SUPERIO_PWM_TIMCMP_CMP_UPPER(Value);
}

/**
 *  @brief      Set the lower 8 bit value for timer comparator
 *
 *  @param  Base    The SuperIO Base address pointer
 *  @param  Timer   The Timer id for the given instance
 *  @param  Value   The value of the lower 8 bits of timer comparator
 *
 */
static inline void SUPERIO_PWM_SetLowerValue(superio_type_t *Base, uint8_t Timer, uint8_t Value)
{
    Base->TIMCMP[Timer] =
        (Base->TIMCMP[Timer] & ~SUPERIO_PWM_TIMCMP_CMP_LOWER_MASK) | SUPERIO_PWM_TIMCMP_CMP_LOWER(Value);
}

/**
 *  @brief      Get the upper 8 bit value for timer comparator
 *
 *  @param  Base     The SuperIO Base address pointer
 *  @param  Timer    The Timer id for the given instance
 *
 *  @return     The value of the upper 8 bits of timer comparator
 *
 */
static inline uint8_t SUPERIO_PWM_GetUpperValue(const superio_type_t *const Base, uint8_t Timer)
{
    return (uint8_t)(((uint32_t)Base->TIMCMP[Timer] & SUPERIO_PWM_TIMCMP_CMP_UPPER_MASK) >>
                     SUPERIO_PWM_TIMCMP_CMP_UPPER_SHIFT);
}

/**
 *  @brief      Get the lower 8 bit value for timer comparator
 *
 *  @param  Base     The SuperIO Base address pointer
 *  @param  Timer    The Timer id for the given instance
 *
 *  @return     The value of the lower 8 bits of timer comparator
 *
 */
static inline uint8_t SUPERIO_PWM_GetLowerValue(const superio_type_t *const Base, uint8_t Timer)
{
    return (uint8_t)(((uint32_t)Base->TIMCMP[Timer] & SUPERIO_PWM_TIMCMP_CMP_LOWER_MASK) >>
                     SUPERIO_PWM_TIMCMP_CMP_LOWER_SHIFT);
}

#if defined(__cplusplus)
}
#endif

#endif /* __SUPERIO_HW_ACCESS_H__ */