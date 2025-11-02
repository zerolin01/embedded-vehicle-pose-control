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
 * @file pctmr_driver.h
 * @brief PCTMR driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PCTMR_DRIVER_H__
#define __PCTMR_DRIVER_H__

#include "status.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PCTMR_ENABLE (1U)

#define PCTMR_DISABLE (0U)

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup PCTMR_Driver PCTMR Driver
 * @{
 */

/**
 * @brief Definition for the PCTMR channel callback function.
 */
typedef void (*pctmr_callback_t)(void *parameter);

/**
 * @brief Pulse Counter Input selection.
 */
typedef enum {
    PCTMR_PINSELECT_TRGMUX = 0x00U, /**< Count pulses from TRGMUX trigger. */
    PCTMR_PINSELECT_ALT1   = 0x01U, /**< Count pulses from pin alternative 1. */
    PCTMR_PINSELECT_ALT2   = 0x02U, /**< Count pulses from pin alternative 2. */
    PCTMR_PINSELECT_ALT3   = 0x03U  /**< Count pulses from pin alternative 3. */
} pctmr_pinselect_t;

/**
 * @brief Pulse Counter input polarity.
 */
typedef enum {
    PCTMR_PINPOLARITY_RISING  = 0U, /**< Count pulse on rising edge. */
    PCTMR_PINPOLARITY_FALLING = 1U  /**< Count pulse on falling edge. */
} pctmr_pinpolarity_t;

/**
 * @brief Work Mode.
 */
typedef enum {
    PCTMR_WORKMODE_TIMER        = 0U, /**< Timer. */
    PCTMR_WORKMODE_PULSECOUNTER = 1U  /**< Pulse counter. */
} pctmr_workmode_t;

/**
 * @brief Prescaler Selection.
 */
typedef enum {
    PCTMR_PRESCALE_2                        = 0x00U, /**< TimerMode: prescaler 2, GlitchFilterMode:          invalid. */
    PCTMR_PRESCALE_4_GLITCHFILTER_2         = 0x01U, /**< TimerMode: prescaler 4, GlitchFilterMode:         2 clocks. */
    PCTMR_PRESCALE_8_GLITCHFILTER_4         = 0x02U, /**< TimerMode: prescaler 8, GlitchFilterMode:         4 clocks. */
    PCTMR_PRESCALE_16_GLITCHFILTER_8        = 0x03U, /**< TimerMode: prescaler 16, GlitchFilterMode:        8 clocks. */
    PCTMR_PRESCALE_32_GLITCHFILTER_16       = 0x04U, /**< TimerMode: prescaler 32, GlitchFilterMode:       16 clocks. */
    PCTMR_PRESCALE_64_GLITCHFILTER_32       = 0x05U, /**< TimerMode: prescaler 64, GlitchFilterMode:       32 clocks. */
    PCTMR_PRESCALE_128_GLITCHFILTER_64      = 0x06U, /**< TimerMode: prescaler 128, GlitchFilterMode:      64 clocks. */
    PCTMR_PRESCALE_256_GLITCHFILTER_128     = 0x07U, /**< TimerMode: prescaler 256, GlitchFilterMode:     128 clocks. */
    PCTMR_PRESCALE_512_GLITCHFILTER_256     = 0x08U, /**< TimerMode: prescaler 512, GlitchFilterMode:     256 clocks. */
    PCTMR_PRESCALE_1024_GLITCHFILTER_512    = 0x09U, /**< TimerMode: prescaler 1024, GlitchFilterMode:    512 clocks. */
    PCTMR_PRESCALE_2048_GLITCHFILTER_1024   = 0x0AU, /**< TimerMode: prescaler 2048, GlitchFilterMode:   1024 clocks. */
    PCTMR_PRESCALE_4096_GLITCHFILTER_2048   = 0x0BU, /**< TimerMode: prescaler 4096, GlitchFilterMode:   2048 clocks. */
    PCTMR_PRESCALE_8192_GLITCHFILTER_4096   = 0x0CU, /**< TimerMode: prescaler 8192, GlitchFilterMode:   4096 clocks. */
    PCTMR_PRESCALE_16384_GLITCHFILTER_8192  = 0x0DU, /**< TimerMode: prescaler 16384, GlitchFilterMode:  8192 clocks. */
    PCTMR_PRESCALE_32768_GLITCHFILTER_16384 = 0x0EU, /**< TimerMode: prescaler 32768, GlitchFilterMode: 16384 clocks. */
    PCTMR_PRESCALE_65536_GLITCHFILTER_32768 = 0x0FU, /**< TimerMode: prescaler 65536, GlitchFilterMode: 32768 clocks. */
} pctmr_prescaler_t;

/**
 * @brief Clock Source selection.
 */
typedef enum {
    PCTMR_CLOCKSOURCE_SROSC    = 0x00U, /**< Irclk(SROSC clock). */
    PCTMR_CLOCKSOURCE_1KHZ_LPO = 0x01U, /**< 1kHz LPO clock. */
    PCTMR_CLOCKSOURCE_AON      = 0x02U, /**< AON clock. */
    PCTMR_CLOCKSOURCE_ERCLK    = 0x03U, /**< Erclk clock. */
} pctmr_clocksource_t;

/**
 * @brief Defines the PCTMR counter units available for configuring or reading the timer compare value.
 */
typedef enum {
    PCTMR_COUNTER_UNITS_TICKS        = 0x00U,
    PCTMR_COUNTER_UNITS_MICROSECONDS = 0x01U,
} pctmr_counter_units_t;

/**
 * @brief Defines the configuration structure for PCTMR.
 */
typedef struct {
    /* General parameters. */
    bool dmaRequest;           /**< Enable/Disable PDMA requests. */
    bool interruptEnable;      /**< Enable/Disable Interrupt. */
    bool freeRun;              /**< Enable/Disable Free Running Mode. */
    pctmr_workmode_t workMode; /**< Time/Pulse Counter Mode. */

    /* Counter parameters. */
    pctmr_clocksource_t clockSelect;    /**< Clock selection for Timer/Glitch filter. */
    pctmr_prescaler_t prescaler;        /**< Prescaler Selection. */
    bool bypassPrescaler;               /**< Enable/Disable prescaler bypass. */
    uint32_t compareValue;              /**< Compare value. */
    pctmr_counter_units_t counterUnits; /**< Compare value units. */

    /* Pulse Counter specific parameters. */
    pctmr_pinselect_t pinSelect;     /**< Pin selection for Pulse-Counter. */
    pctmr_pinpolarity_t pinPolarity; /**< Pin Polarity for Pulse-Counter. */

    pctmr_callback_t callBack; /**< Callback function pointer for the PCTMR */
    void *parameter;           /**< PCTMR callBack parameter */

} pctmr_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Interrupt registration function.
 * 
 * @param  instance     PCTMR  instance.     
 * @param  callback     Callback function.   
 * @param  parameter    The argument to the callback function.     
 * @return ** status_t 
 *         STATUS_SUCCESS  if succeed.
 *         STATUS_ERRORPAR if error in input parameters.
 */
status_t PCTMR_DRV_InstallCallback(uint32_t instance, pctmr_callback_t callback, void *parameter);

/**
 * @brief  Initialize a configuration structure with default values.
 *
 * @param  config Pointer to the configuration structure to be initialized.
 */
void PCTMR_DRV_InitConfigStruct(pctmr_config_t *const config);

/**
 * @brief  Initialize a PCTMR instance with values from an input configuration structure.
 *
 * @param  instance PCTMR instance number.
 * @param  config   Pointer to the input configuration structure.
 */
void PCTMR_DRV_Init(const uint32_t instance, const pctmr_config_t *const config);

/**
 * @brief  Configure a PCTMR instance.
 *
 * @param  instance  PCTMR instance number.
 * @param  config    Pointer to the input configuration structure.
 */
status_t PCTMR_DRV_SetConfig(const uint32_t instance, const pctmr_config_t *const config);

/**
 * @brief Get the current configuration of a PCTMR instance.
 *
 * @param  instance PCTMR instance number.
 * @param  config   Pointer to the output configuration structure.
 */
void PCTMR_DRV_GetConfig(const uint32_t instance, pctmr_config_t *const config);

/**
 * @brief  De-initialize a PCTMR instance.
 *
 * @param  instance PCTMR instance number.
 */
void PCTMR_DRV_Deinit(const uint32_t instance);

/**
 * @brief  Set the compare value in counter tick units for a PCTMR instance.
 *
 * @param  instance PCTMR instance.
 * @param  compareValueByCount The compare value in counter ticks, to be written.
 * @return status_t
 *          STATUS_SUCCESS Completed successfully.
 *          STATUS_ERROR Cannot reconfigure compare value (TCF not set).
 *          STATUS_TIMEOUT Compare value greater then current counter value.
 */
status_t PCTMR_DRV_SetCompareValueByCount(const uint32_t instance, const uint16_t compareValueByCount);

/**
 * @brief  Get the compare value in counter tick units, of a PCTMR instance.
 *
 * @param  instance  PCTMR instance number.
 * @param  compareValueByCount Pointer to current compare value, in counter ticks.
 */
void PCTMR_DRV_GetCompareValueByCount(const uint32_t instance, uint16_t *const compareValueByCount);
/**
 * @brief  Set the compare value for Timer Mode in microseconds, for a PCTMR instance.
 *
 * @param  instance   PCTMR peripheral instance number.
 * @param  compareValueUs Compare value in microseconds.
 * @return status_t
 * STATUS_SUCCESS Completed successfully.
 * STATUS_ERROR Cannot reconfigure compare value.
 * STATUS_TIMEOUT Compare value greater then current counter value.
 */
status_t PCTMR_DRV_SetCompareValueByUs(const uint32_t instance, const uint32_t compareValueUs);

/**
 * @brief Get the compare value in microseconds, of a PCTMR instance.
 *
 * @param  instance  PCTMR instance number.
 * @param  compareValueUs Pointer to current compare value, in microseconds.
 */
status_t PCTMR_DRV_GetCompareValueByUs(const uint32_t instance, uint32_t *const compareValueUs);

/**
 * @brief   Get the current state of the Compare Flag of a PCTMR instance.
 *
 * @param  instance  PCTMR instance number.
 * @return bool The state of the Compare Flag.
 */
bool PCTMR_DRV_GetIntFlag(const uint32_t instance);

/**
 * @brief   Clear the Compare Flag of a PCTMR instance.
 *
 * @param  instance PCTMR instance number.
 */
void PCTMR_DRV_ClearIntFlag(const uint32_t instance);

/**
 * @brief  Get the run state of a PCTMR instance.
 *
 * @param  instance PCTMR instance number.
 * @return true    Timer/Counter started.
 * @return false   Timer/Counter stopped.
 */
bool PCTMR_DRV_IsRunning(const uint32_t instance);

/**
 * @brief  Enable/disable the PCTMR interrupt.
 *
 * @param  instance PCTMR instance number.
 * @param  enableInterrupt The new state of the PCTMR interrupt enable flag.
 */
void PCTMR_DRV_SetInterrupt(const uint32_t instance, const bool enableInterrupt);

/**
 * @brief  Get the current counter value in counter tick units.
 *
 * @param  instance  PCTMR instance number.
 * @return uint16_t The current counter value.
 */
uint16_t PCTMR_DRV_GetCounterValueByCount(const uint32_t instance);

/**
 * @brief Enable PCTMR
 *
 * @note bypass mode must be false when use PCTMR pulse mode
 *
 * @param  instance
 * @return ** status_t
 *         STATUS_ERRORCLK if get PCTMR frequency error.
 *         STATUS_ERRORPAR if use both bypass and pulse pctmr mode
 *         STATUS_SUCCESS  if function working correctly
 *
 */
status_t PCTMR_DRV_StartCounter(const uint32_t instance);

/**
 * @brief  Disable the PCTMR / Stop the counter.
 *
 * @param  instance PCTMR instance number.
 */
void PCTMR_DRV_StopCounter(const uint32_t instance);

/**
 * @brief  Set the Input Pin configuration for Pulse Counter mode.
 *
 * @param  instance     PCTMR instance number.
 * @param  pinSelect    PCTMR pin selection.
 * @param  pinPolarity  Polarity on which to increment counter (rising/falling edge).
 */
void PCTMR_DRV_SetPinConfiguration(const uint32_t instance, const pctmr_pinselect_t pinSelect,
                                   const pctmr_pinpolarity_t pinPolarity);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group PCTMR_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __PCTMR_DRIVER_H__ */
