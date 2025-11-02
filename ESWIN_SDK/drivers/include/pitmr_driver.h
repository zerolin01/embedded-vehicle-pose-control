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
 * @file  pitmr_driver.h
 * @brief PITMR driver header file
 * 
 * @note There are two PITMR modules,PITMR0 containing 4 channels,PITMR1 containing 10 channels.
 * @note Except that PITMR1 has the same general functions as PITMR0, PITMR1’s 0 ~3 channels are directly connected to PDMA's
 *       0~3 channels.  PITMR1's 4 ~9 channels are directly connected to can's 0 ~5 channels.
 * @note channels 0 ~ 3 of PITMR1 can be used to periodically trigger dma channel 0 ~ 3 channels, and PITMR1 channels 4 ~ 9 
 *       can be used to add time stamps to channels 0 ~ 5 of CAN modules. 
 * 
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PITMR_DRIVER_H__
#define __PITMR_DRIVER_H__

#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup PITMR_Driver PITMR Driver
 * @{
 */

/**
 * @brief Max mask of PITMR0, 4 channels
 */
#define PITMR0_MAX_MASK (0xFU)

/**
 * @brief Max mask of PITMR1, 10 channels
 */
#define PITMR1_MAX_MASK (0x3FFU)

/**
 * @brief Max period in count of all operation mode except for dual 16 bit periodic counter mode.
 */
#define MAX_PERIOD_COUNT (0xFFFFFFFFU)

/**
 * @brief Max period in count of dual 16 bit periodic counter mode.
 */

#define MAX_PERIOD_COUNT_IN_DUAL_16BIT_MODE (0x1FFFEU)

/**
 *
 * @brief Max count of 16 bit.
 */
#define MAX_PERIOD_COUNT_16_BIT (0xFFFFU)

/**
 * @brief Definition for the PITMR channel callback function.
 */
typedef void (*pitmr_callback_t)(void *parameter);

/**
 * @brief Mode options available for the PITMR timer.
 */
typedef enum {
    PITMR_PERIODIC_COUNTER      = 0x00U, /**< 32-bit Periodic Counter.        */
    PITMR_DUAL_PERIODIC_COUNTER = 0x01U, /**< Dual 16-bit Periodic Counter.   */
    PITMR_TRIGGER_ACCUMULATOR   = 0x02U, /**< 32-bit Trigger Accumulator.     */
    PITMR_INPUT_CAPTURE         = 0x03U  /**< 32-bit Trigger Input Capture.   */
} pitmr_timer_modes_t;

/**
 * @brief Trigger source options.
 */
typedef enum {
    PITMR_TRIGGER_SOURCE_EXTERNAL = 0x00U, /**< Use external trigger.  */
    PITMR_TRIGGER_SOURCE_INTERNAL = 0x01U  /**< Use internal trigger.  */
} pitmr_trigger_source_t;

/**
 * @brief Unit options for PITMR period.
 */
typedef enum {
    PITMR_PERIOD_UNITS_COUNTS       = 0x00U, /**< Period value unit is count. */
    PITMR_PERIOD_UNITS_MICROSECONDS = 0x01U  /**< Period value unit is microsecond. */
} pitmr_period_units_t;

/**
 * @brief PITMR configuration structure.
 */
typedef struct {
    bool enableRunInDebug; /**< True: Timer channels continue to run in debug mode.
                              False: Timer channels stop in debug mode. \D{false} */
    bool enableRunInDoze;  /**< True: Timer channels continue to run in doze mode.
                              False: Timer channels stop in doze mode. \D{false} */
} pitmr_user_config_t;

/**
 * @brief Structure to configure the channel timer
 */
typedef struct {

    pitmr_timer_modes_t timerMode;    /**< Operation mode of timer channel. \D{PITMR_PERIODIC_COUNTER} */
    pitmr_period_units_t periodUnits; /**< Timer period value units. \D{PITMR_PERIOD_UNITS_MICROSECONDS} */
    uint32_t period;                  /**< Period of timer channel. \D{10000U} */

    /** Selects between internal and external trigger sources. \D{PITMR_TRIGGER_SOURCE_EXTERNAL} */
    pitmr_trigger_source_t triggerSource;

    uint32_t triggerSelect;     /**< Selects one trigger from the internal trigger sources
                                    this field makes sense if trigger source is internal. \D{0} */
    bool enableReloadOnTrigger; /**< True: Timer channel will reload on selected trigger.
                                    False: Timer channel will not reload on selected trigger. \D{false} */
    bool enableStopOnInterrupt; /**< True: Timer will stop after timeout.
                                    False: Timer channel does not stop after timeout. \D{false} */
    bool enableStartOnTrigger;  /**< True: Timer channel starts to decrement when rising edge on selected trigger is detected.
                                    False: Timer starts to decrement immediately based on restart condition. \D{false} */
    bool chainChannel;          /**< Channel chaining enable. \D{false} */

    bool isInterruptEnabled; /**< Timer channel interrupt generation enable. \D{true} */

    pitmr_callback_t callBack; /**< Callback function pointer for the PITMR */

    void *parameter; /**< PITMR callBack parameter */

} pitmr_user_channel_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Interrupt registration function.
 * 
 * @param  instance   PITMR  instance.      
 * @param  channel    PITMR  channel.    
 * @param  callback   Callback function.          
 * @param  parameter  The argument to the callback function.      
 * @return ** status_t 
 *  *      STATUS_SUCCESS  if succeed.
 *         STATUS_ERRORPAR if error in input parameters.
 */
status_t PITMR_DRV_InstallCallback(uint32_t instance, uint32_t channel, pitmr_callback_t callback, void *parameter);

/**
 * @brief Get the default PITMR configuration.
 * 
 * @param  config  The configuration structure.        
 * @return ** status_t 
 *            STATUS_SUCCESS if get success.
 *            STATUS_ERRORPAR if function parameter config error.
 */
status_t PITMR_DRV_GetDefaultConfig(pitmr_user_config_t *const config);

/**
 * @brief Get the default PITMR channel configuration.
 * 
 * @param  config  The channel configuration structure.        
 * @return ** status_t 
 *            STATUS_SUCCESS if get success.
 *            STATUS_ERRORPAR if function parameter config error.
 */
status_t PITMR_DRV_GetDefaultChanConfig(pitmr_user_channel_config_t *const config);

/**
 * @brief  Initializes the PITMR module.
 * 
 * This function reset and enable PITMR module, configure PITMR status in Debug and DOZE mode. 
 * This configuration structure affects all timer channels. This function should be called before calling any other PITMR driver function.
 * 
 * @param  instance     PITMR instance(0 ~ 1).      
 * @param  userConfig   Pointer to PITMR configuration structure.    
 * @return ** status_t 
 *         STATUS_SUCCESS  if PITMR initialize succeed.
 *         STATUS_ERRORPAR if PITMR instance out of range or userConfig is NULL.
 *         STATUS_ERROR    if PITMR initialize failed. 
 *         STATUS_ERRORCLK if fetch clock error.
 * 
 */
status_t PITMR_DRV_Init(uint32_t instance, const pitmr_user_config_t *userConfig);

/**
 * @brief De-Initializes the PITMR module.
 * 
 * @param  instance PITMR instance(0 ~ 1).        
 * @return ** status_t 
 *            STATUS_SUCCESS if get success.
 *            STATUS_ERRORPAR if function parameter instance error.
 */
status_t PITMR_DRV_Deinit(uint32_t instance);

/**
 * @brief Initializes the PITMR channel.
 *
 * Initializes PITMR channel.
 * This function initializes the PITMR timers by using a channel, this function
 * configures timer channel chaining, timer channel mode, timer channel period,
 * interrupt generation, trigger source, trigger select, reload on trigger,
 * stop on interrupt and start on trigger.
 * The timer channel number and its configuration structure shall be passed as arguments.
 * Timer channels do not start counting by default after calling this function.
 * The function PITMR_DRV_StartTimerChannels must be called to start the timer channel counting.
 * In order to re-configures the period, call the PITMR_DRV_SetTimerPeriodByUs or
 * PITMR_DRV_SetTimerPeriodByCount.
 *
 * @param  instance PITMR module instance number
 * @param  channel  Timer channel number
 * @param  userChannelConfig Pointer to PITMR channel configuration structure
 * @return status_t
 *         STATUS_SUCCESS if Operation was successful.
 *         STATUS_ERROR   if The channel 0 is chained.
 *         STATUS_ERROR   if input period is invalid.
 */
status_t PITMR_DRV_InitChannel(uint32_t instance, uint32_t channel,
                               const pitmr_user_channel_config_t *userChannelConfig);

/**
 * @brief  Starts the timer channel counting.
 *
 * This function allows starting timer channels simultaneously .
 * After calling this function, timer channels are going operate depend on mode and
 * control bits which controls timer channel start, reload and restart.
 *
 * -with mask = 0x01u means channel 0 
 * -with mask = 0x02u means channel 1 
 * -with mask = 0x03u means channel 0 and 1 
 * -with mask = 0x0Fu means channel 0, 1 , 2 and 3.
 * PITMR0 channels = 4 
 * PITMR1 channels = 10 
 * 
 * @param  instance PITMR module instance number
 * @param  mask     Timer channels starting mask that decides which channels
 */
status_t PITMR_DRV_StartTimerChannels(uint32_t instance, uint32_t mask);

/**
 * @brief  Stop PITMR channel counting.
 * 
 * This function allows stop timer channels simultaneously from counting.
 * Timer channels reload their periods respectively after the next time
 * they call the PITMR_DRV_StartTimerChannels. Note that: In 32-bit Trigger Accumulator
 * mode, the counter will load on the first trigger rising edge.
 * 
 * -with mask = 0x01u means channel 0 
 * -with mask = 0x02u means channel 1 
 * -with mask = 0x03u means channel 0 and 1 
 * -with mask = 0x0Fu means channel 0, 1 , 2 and 3.
 * PITMR0 channels = 4 
 * PITMR1 channels = 10 
 * 
 * @param  instance  PITMR module instance number,(0 ~1).    
 * @param  mask      PITMR  channel mask, PITMR0(0x0 ~ 0xF), PITMR1(0x0 ~ 0x3FF).  
 * @return ** status_t 
 *          STATUS_SUCCESS if PITMR stop succeed.
 *          STATUS_ERRORPAR if PITMR instance or PITMR channel mask error.
 */
status_t PITMR_DRV_StopTimerChannels(uint32_t instance, uint32_t mask);

/**
 * @brief  Set PITMR channel period in microseconds.
 * 
 * This function sets the timer channel period in microseconds
 * when timer channel mode is 32 bit periodic or dual 16 bit counter mode.
 * The period range depends on the frequency of the PITMR functional clock and
 * operation mode of timer channel.
 * If the required period is out of range, use the suitable mode if applicable.
 * This function is only valid for one single channel.
 * 
 * @param  instance PITMR module instance number,(0 ~ 1).    
 * @param  channel  PITMR channel number, PITMR0(0 ~3), PITMR1(0 ~9).     
 * @param  periodUs PITMR channel period in microseconds.      
 * @return ** status_t 
 *          STATUS_SUCCESS if Input period of timer channel is valid.
 *          STATUS_ERROR if Input period of timer channel is invalid.
 *          STATUS_ERRORPAR if PITMR instance or PITMR channel number error.
 */
status_t PITMR_DRV_SetTimerPeriodByUs(uint32_t instance, uint32_t channel, uint32_t periodUs);

/**
 * @brief Set the timer channel period in microseconds.
 * 
 * This function sets the timer channel period in microseconds
 * when timer channel mode is dual 16 bit periodic counter mode.
 * The period range depends on the frequency of the PITMR functional clock and
 * operation mode of timer channel.
 * If the required period is out of range, use the suitable mode if applicable.
 * This function is only valid for one single channel.
 * 
 * @param  instance    PITMR module instance number,(0 ~ 1).     
 * @param  channel     PITMR instance channel number, PITMR0(0 ~ 3), PITMR1(0 ~ 9).     
 * @param  periodHigh  Period of higher 16 bit in microseconds.   
 * @param  periodLow   Period of lower 16 bit in microseconds.     
 * @return ** status_t 
 *         STATUS_SUCCESS if function run succeed.
 *         STATUS_ERROR  if get PITMR clk error.
 *         STATUS_ERRORPAR if PITMR instance or PITMR channel number error.
 */
status_t PITMR_DRV_SetTimerPeriodInDual16ModeByUs(uint32_t instance, uint32_t channel, uint16_t periodHigh,
                                                  uint16_t periodLow);

/**
 * @brief  Gets the timer channel period in microseconds.
 *   
 * The value of a user-configured register.
 *
 * @param  instance  PITMR module instance number.
 * @param  channel   Timer channel number.
 * @return uint64_t Timer channel period in microseconds.
 */
uint64_t PITMR_DRV_GetTimerPeriodByUs(uint32_t instance, uint32_t channel);

/**
 * @brief  Gets the current timer channel counting value in microseconds.
 *
 * @param  instance PITMR module instance number.
 * @param  channel  Timer channel number.
 * @return uint64_t Current timer channel counting value in microseconds.
 */
uint64_t PITMR_DRV_GetCurrentTimerUs(uint32_t instance, uint32_t channel);

/**
 * @brief Set PITMR channel period in count unit.
 * 
 * This function sets the timer channel period in count unit.
 * The counter period of a running timer channel can be modified by first setting
 * a new load value, the value will be loaded after the timer channel expires.
 * To abort the current cycle and start a timer channel period with the new value,
 * the timer channel must be disabled and enabled again.
 * 
 * @param  instance    PITMR module instance number(0 ~ 1).      
 * @param  channel     PITMR channel num, PITMR0(0 ~ 3), PITMR1(0 ~ 9).  
 * @param  count       PITMR channel period in count unit.    
 * @return ** status_t 
 *         STATUS_SUCCESS if PITMR set succeed.
 *         STATUS_ERRORPAR if PITMR instance or PITMR channel error.
 */
status_t PITMR_DRV_SetTimerPeriodByCount(uint32_t instance, uint32_t channel, uint32_t count);

/**
 * @brief   Sets the timer channel period in count unit.
 * 
 * This function sets the timer channel period in count unit when timer channel
 * mode is dual 16 periodic counter mode.
 * The counter period of a running timer channel can be modified by first setting
 * a new load value, the value will be loaded after the timer channel expires.
 * To abort the current cycle and start a timer channel period with the new value,
 * the timer channel must be disabled and enabled again.
 * 
 * @param  instance    PITMR module instance number(0 ~ 1).     
 * @param  channel  PITMR channel number,PITMR0(0 ~3), PITMR1(0~9).   
 * @param  periodHigh  Period of higher 16 bit in count unit.
 * @param  periodLow   Period of lower 16 bit in count unit.    
 * @return ** status_t 
 *         STATUS_ERRORPAR if PITMR instance or PITMR channel number out of range.
 *         STATUS_SUCCESS  if PITMR run success.
 */
status_t PITMR_DRV_SetTimerPeriodInDual16ModeByCount(uint32_t instance, uint32_t channel, uint16_t periodHigh,
                                                     uint16_t periodLow);

/**
 * @brief  Gets the user-configured register value in count unit.
 *
 * @param  instance PITMR module instance number.
 * @param  channel  Timer channel number.
 * @return uint32_t Timer channel period in count unit.
 */
uint32_t PITMR_DRV_GetTimerPeriodByCount(uint32_t instance, uint32_t channel);

/**
 * @brief  Gets the current timer channel counting value in count.
 *
 * @param  instance  PITMR module instance number.
 * @param  channel   Timer channel number.
 * @return uint32_t Current timer channel counting value in count.
 */
uint32_t PITMR_DRV_GetCurrentTimerCount(uint32_t instance, uint32_t channel);

/**
 * @brief Enable PITMR channel interrupt.
 * 
 * -with mask = 0x01u means channel 0 
 * -with mask = 0x02u means channel 1 
 * -with mask = 0x03u means channel 0 and 1 
 * -with mask = 0x0Fu means channel 0, 1 , 2 and 3.
 * PITMR0 channels = 4 
 * PITMR1 channels = 10 
 * 
 * @param  instance  PITMR module instance number.     
 * @param  mask      PITMR channel mask,PITMR0(0x0 ~ 0xF),PITMR1(0x0 ~ 0x3FF).      
 * @return ** status_t 
 *         STATUS_ERRORPAR if PITMR channel mask out of range.
 *         STATUS_SUCCESS  if PITMR run success.
 */
status_t PITMR_DRV_EnableTimerChannelInterrupt(uint32_t instance, uint32_t mask);

/**
 * @brief Disable PITMR interrupt.
 * 
 * -with mask = 0x01u means channel 0 
 * -with mask = 0x02u means channel 1 
 * -with mask = 0x03u means channel 0 and 1 
 * -with mask = 0x0Fu means channel 0, 1 , 2 and 3.
 * PITMR0 channels = 4 
 * PITMR1 channels = 10 
 * 
 * @param  instance      PITMR module instance number   
 * @param  mask          PITMR channel mask,PITMR0(0x0 ~ 0xF),PITMR1(0x0 ~ 0x3FF)  
 * @return ** status_t 
 *         STATUS_ERRORPAR if PITMR channel mask out of range.
 *         STATUS_SUCCESS  if PITMR run success.
 */
status_t PITMR_DRV_DisableTimerChannelInterrupt(uint32_t instance, uint32_t mask);

/**
 * @brief   Gets the current interrupt flag of timer channels.
 *
 * In compare modes, the flag sets to 1 at the end of the timer period.
 * In capture modes, the flag sets to 1 when the trigger asserts.
 *
 * @param  instance PITMR module instance number.
 * @param  mask     The interrupt flag getting mask that decides which channels will
 * @return uint32_t
 *         Current the interrupt flag of timer channels
 */
uint32_t PITMR_DRV_GetInterruptFlagTimerChannels(uint32_t instance, uint32_t mask);

/**
 * @brief Clear interrupt flag of PITMR channel.
 * 
 * This function clear the interrupt flag of PITMR channels after their interrupt event occurred.
 * -with mask = 0x01u means channel 0 
 * -with mask = 0x02u means channel 1 
 * -with mask = 0x03u means  channel 0 and 1 
 * -with mask = 0x0Fu means  channel 0, 1 , 2 and 3.
 * PITMR0 channels = 4 
 * PITMR1 channels = 10 
 * 
 * @param  instance     PITMR module instance number.         
 * @param  mask         PITMR channel mask. PITMR0(0x0 ~ 0xF), PITMR1(0x0 ~ 0x3FF)  
 * @return ** status_t 
 *         STATUS_ERRORPAR if PITMR channel mask out of range
 *         STATUS_SUCCESS  if PITMR run success.
 */
status_t PITMR_DRV_ClearInterruptFlagTimerChannels(uint32_t instance, uint32_t mask);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group PITMR_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif
