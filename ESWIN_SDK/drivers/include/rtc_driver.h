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
 * @file rtc_driver.h
 * @brief RTC driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __RTC_DRIVER_H__
#define __RTC_DRIVER_H__

#include "status.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup RTC_Driver RTC Driver
 * @{
 */

#define RTC_ENABLE          (1U)
#define RTC_DISABLE         (0U)
#define SECONDS_IN_A_DAY    (86400UL)
#define SECONDS_IN_A_HOUR   (3600U)
#define SECONDS_IN_A_MIN    (60U)
#define MINS_IN_A_HOUR      (60U)
#define HOURS_IN_A_DAY      (24U)
#define DAYS_IN_A_YEAR      (365U)
#define DAYS_IN_A_LEAP_YEAR (366U)
#define YEAR_RANGE_START    (1970U)
#define YEAR_RANGE_END      (2099U)

/**
 * @brief Definition for the pdu channel callback function.
 */
typedef void (*rtc_callback_t)(void *parameter);

/**
 * @brief RTC clock select.
 */
typedef enum {
    RTC_CLK_SRC_OSC_32768HZ  = 0x00U, /**< RTC Prescaler increments using 32.768 KHz crystal. */
    RTC_CLK_SRC_OSC_32KHZ    = 0x01U, /**< RTC Prescaler increments using 32 KHz crystal. */
    RTC_CLK_SRC_OSC_38400HZ  = 0x02U, /**< RTC Prescaler increments using 38.4 KHz crystal. */
    RTC_CLK_SRC_OSC_32768HZ2 = 0x03U, /**< RTC Prescaler increments using 38.4 KHz crystal. */
} rtc_clk_select_t;

/**
 * @brief RTC IRQ TYPE.
 */
typedef enum {
    RTC_IRQ_ALARM     = 0x00U, /**< RTC irq alarm */
    RTC_IRQ_STOPWATCH = 0x01U, /**< RTC irq stopwatch. */
    RTC_IRQ_SAMPLEING = 0x02U, /**< RTC irq sampling */
} rtc_irq_type_t;

/**
 * @brief RTC Interrupt.
 */
typedef enum {
    RTC_INT_SW   = 0U,  /**< Stopwatch Interrupt. */
    RTC_INT_MIN  = 1U,  /**< Minute Interrupt. */
    RTC_INT_ALM  = 2U,  /**< Alarm Interrupt. */
    RTC_INT_DAY  = 3U,  /**< Day Interrupt. */
    RTC_INT_1HZ  = 4U,  /**< 1Hz Interrupt. */
    RTC_INT_HR   = 5U,  /**< Hour Interrupt. */
    RTC_INT_2HZ  = 7U,  /**< 2Hz Interrupt. */
    RTC_INT_SAM0 = 8U,  /**< sampling Interrupt. */
    RTC_INT_SAM1 = 9U,  /**< sampling Interrupt. */
    RTC_INT_SAM2 = 10U, /**< sampling Interrupt. */
    RTC_INT_SAM3 = 11U, /**< sampling Interrupt. */
    RTC_INT_SAM4 = 12U, /**< sampling Interrupt. */
    RTC_INT_SAM5 = 13U, /**< sampling Interrupt. */
    RTC_INT_SAM6 = 14U, /**< sampling Interrupt. */
    RTC_INT_SAM7 = 15U, /**< sampling Interrupt. */
    RTC_INT_AUTO = 16U, /**<  read register to get the interrupt. */
} rtc_int_t;

/**
 * @brief RTC Time Date structure.
 */
typedef struct {
    uint16_t year;    /**< Year. \D{1970} */
    uint16_t month;   /**< Month. \D{1} */
    uint16_t day;     /**< Day. \D{2} */
    uint16_t hour;    /**< Hour. \D{0} */
    uint16_t minutes; /**< Minutes. \D{0} */
    uint8_t seconds;  /**< Seconds. \D{0} */
} rtc_timedate_t;

/**
 * @brief RTC Initialization structure.
 */
typedef struct {
    rtc_clk_select_t clockSelect; /**< RTC Clock Select.  */
} rtc_init_config_t;

/**
 * @brief RTC alarm configuration.
 */
typedef struct {
    rtc_timedate_t alarmTime;                   /**< Alarm time. */
    bool alarmIntEnable;                        /**< Enable alarm interrupt. \D{false} */
    void (*alarmCallback)(void *callbackParam); /**< Pointer to the user callback method. \D{NULL} */
    void *callbackParams;                       /**< Pointer to the callback parameters. \D{NULL} */
} rtc_alarm_config_t;

/**
 * @brief RTC interrupt configuration.
 */
typedef struct {
    bool minIntEnable;                               /**< Minute Interrupt enable.   \D{false} */
    bool dayIntEnable;                               /**< Day Interrupt enable.      \D{false} */
    bool hz1IntEnable;                               /**< 1Hz Interrupt enable.      \D{false} */
    bool hourIntEnable;                              /**< Hour Interrupt enable.     \D{false} */
    bool hz2IntEnable;                               /**< 2Hz Interrupt enable.      \D{false} */
    bool sam0IntEnable;                              /**< sampling Interrupt enable. \D{false} */
    bool sam1IntEnable;                              /**< sampling Interrupt enable. \D{false} */
    bool sam2IntEnable;                              /**< sampling Interrupt enable. \D{false} */
    bool sam3IntEnable;                              /**< sampling Interrupt enable. \D{false} */
    bool sam4IntEnable;                              /**< sampling Interrupt enable. \D{false} */
    bool sam5IntEnable;                              /**< sampling Interrupt enable. \D{false} */
    bool sam6IntEnable;                              /**< sampling Interrupt enable. \D{false} */
    bool sam7IntEnable;                              /**< sampling Interrupt enable. \D{false} */
    void (*rtcSampingCallback)(void *callbackParam); /**< Pointer to the user callback method. \D{NULL} */
    void *callbackParams;                            /**< Pointer to the callback parameters.  \D{NULL} */
} rtc_samping_config_t;

/**
 * @brief RTC stopwatch Interrupt Configuration.
 */
typedef struct {
    uint8_t stopwatchVal;                              /**< Stopwatch val. \D{0x1} */
    bool stopwatchIntEnable;                           /**< Stopwatch Interrupt enable. \D{false} */
    void (*rtcStopwatchCallback)(void *callbackParam); /**< Pointer to the user callback method. \D{NULL} */
    void *callbackParams;                              /**< Pointer to the callback parameters. \D{NULL} */
} rtc_stopwatch_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Interrupt registration function.
 * 
 * @param  instance   RTC instance.      
 * @param  callback   Callback function.     
 * @param  parameter  The argument to the callback function.
 * @param  irqType    RTC irq type.
 * @return ** status_t 
 *         STATUS_SUCCESS  if succeed.
 *         STATUS_ERRORPAR if error in input parameters.
 */
status_t RTC_DRV_InstallCallback(uint32_t instance, rtc_callback_t callback, void *parameter, rtc_irq_type_t irqType);

/**
 * @brief This function initializes the RTC instance with the settings
 *        provided by the user via the rtcUserCfg parameter.
 *
 * @param  instance    The number of the RTC instance used.
 * @param  rtcUserCfg  Pointer to the user's configuration structure.
 * @return status_t STATUS_SUCCESS if the operation was successful.
 */
status_t RTC_DRV_Init(uint32_t instance, const rtc_init_config_t *const rtcUserCfg);

/**
 * @brief This function deinitialize the RTC instance.
 *
 * @param  instance The number of the RTC instance used.
 * @return status_t
 *         STATUS_SUCCESS if the operation was successful.
 *         STATUS_ERROR if Control register is locked.
 */
status_t RTC_DRV_Deinit(uint32_t instance);

/**
 * @brief This function will Get the default configuration values.
 *
 * @param  config  Pointer to the structure in which the configuration will be saved.
 */
void RTC_DRV_GetDefaultConfig(rtc_init_config_t *const config);

/**
 * @brief Start RTC Module.
 *
 * if you manually stop the RTC module,you need to call this function to restart.
 *
 * @param  instance The number of the RTC instance used
 */
void RTC_DRV_StartCounter(uint32_t instance);

/**
 * @brief Stop RTC instance counter.
 *
 * @param  instance The number of the RTC instance used.
 */
void RTC_DRV_StopCounter(uint32_t instance);

/**
 * @brief  Get current time and date from RTC instance.
 *
 * @param  instance The number of the RTC instance used.
 * @param  currentTime Get current time.
 */
void RTC_DRV_GetCurrentTimeDate(uint32_t instance, rtc_timedate_t *const currentTime);

/**
 * @brief Set time and date for RTC instance.
 *
 * @param  instance The number of the RTC instance used.
 * @param  time     Pointer to the variable in which the time is stored.
 * @return status_t
 *         STATUS_SUCCESS if the operation was successful.
 *         STATUS_ERROR if the time provided was invalid or if the counter was not stopped.
 */
status_t RTC_DRV_SetTimeDate(uint32_t instance, const rtc_timedate_t *const time);

/**
 * @brief  This method configures the alarm with the configuration from the alarmConfig parameter.
 *
 * @param  instance The number of the RTC instance used.
 * @param  alarmConfig  Pointer to the structure which holds the alarm configuration.
 * @return status_t
 *         STATUS_SUCCESS if the configuration is successful.
 *         STATUS_ERROR if the alarm time is invalid.
 */
status_t RTC_DRV_ConfigureAlarm(uint32_t instance, rtc_alarm_config_t *const alarmConfig);

/**
 * @brief Get alarm configuration for RTC instance.
 *
 * @param  instance The number of the RTC instance used.
 * @param  alarmConfig Pointer to the structure in which to store the alarm configuration.
 */
void RTC_DRV_GetAlarmConfig(uint32_t instance, rtc_alarm_config_t *alarmConfig);

/**
 * @brief Check if IRQ is pending.
 *
 * @param  instance The number of the RTC instance used.
 * @param  IRQType RTC irq type.
 * @return bool
 *         true: If the alarm has occurred.
 *         false: If alarm not occurred.
 */
bool RTC_DRV_IsIRQPending(uint32_t instance, rtc_int_t IRQType);

/**
 * @brief  This method configures the stopwatch with the
 *         configuration from the stopwatchConfig parameter.
 *
 * @param  instance  The number of the RTC instance used.
 * @param  stopwatchConfig  Pointer to the structure which holds the stopwatch configuration.
 * @return status_t STATUS_SUCCESS if the configuration is successful.
 */
status_t RTC_DRV_ConfigureStopwatch(uint32_t instance, rtc_stopwatch_config_t *const stopwatchConfig);

/**
 * @brief  Get Stopwatch configuration for RTC instance.
 *
 * @param  instance The number of the RTC instance used.
 * @param  stopwatchConfig Pointer to the structure in which to store the alarm configuration.
 */
void RTC_DRV_GetStopwatchConfig(uint32_t instance, rtc_stopwatch_config_t *stopwatchConfig);

/**
 * @brief This method configures the samping with the
 *        configuration from the sampingConfig parameter.
 *
 * @param  instance     The number of the RTC instance used.
 * @param  sampingConfig Pointer to the structure which holds the samping configuration.
 */
void RTC_DRV_ConfigureSamping(uint32_t instance, rtc_samping_config_t *const sampingConfig);

/**
 * @brief  Get samping configuration for RTC instance.
 *
 * @param  instance The number of the RTC instance used.
 * @param  sampingConfig  Pointer to the structure in which to store the samping configuration.
 */
void RTC_DRV_GetSampingConfig(uint32_t instance, rtc_samping_config_t *sampingConfig);

/**
 * @brief Convert days to rtc_timedate_t structure.
 * 
 * The day value that the we obtains from the register so we need to convert the value of register to the 
 * familiar year,month and day.
 * 
 * @param  timeDate   Pointer to the structure in which to store the result.
 * @param  days   Pointer to the days.
 */
void RTC_DRV_ConvertDaysToTimeDate(rtc_timedate_t *const timeDate, const uint32_t *days);

/**
 * @brief Convert rtc_timedate_t structure to seconds.
 *
 * Convert the year,month,day to the second value,can be called according to their needs. 
 * 
 * @param  timeDate  Pointer to the source struct.
 * @param  seconds   Pointer to the variable in which to store the result.
 */
void RTC_DRV_ConvertTimeDateToSeconds(const rtc_timedate_t *const timeDate, uint32_t *const seconds);

/**
 * @brief Convert rtc_timedate_t structure to days.
 *
 * Convert year,month and day to day value , which can be used for configuration register.
 * 
 * @param  timeDate Pointer to the source struct.
 * @param  days     Pointer to the variable in which to store the result.
 */
void RTC_DRV_ConvertTimeDateToDays(const rtc_timedate_t *const timeDate, uint32_t *const days);

/**
 * @brief Check if the current year is leap.
 *
 * @param  year  Year to check.
 * @return bool
 *         True If the year is leap.
 *         False If the year is not leap.
 */
bool RTC_DRV_IsYearLeap(uint16_t year);

/**
 * @brief  Check if the date time struct is configured properly.
 *
 * Check that the time is in the correct range,for example the seconds range 0 to 59.
 * @param  timeDate Structure to check to check.
 * @return bool
 *         True If the time date is in the correct format.
 *         False If the time date is not in the correct format.
 */
bool RTC_DRV_IsTimeDateCorrectFormat(const rtc_timedate_t *const timeDate);

/**
 * @brief Clear RTC interrupt.
 * 
 * @param  instance  The RTC instance.
 */
void RTC_DRV_ClearInterrupt(uint32_t instance);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group RTC_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif
