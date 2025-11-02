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
 * @file gtmr_driver.h
 * 
 * @brief GTMR module is an ADC mounted counter,used to time mark the ADC data.At the same time, the module can also be triggered as a common timer.
 * 
 * @note  1.GTMR is cycle timer,counting cycles from the user-set initial value to 0xFFFF_FFFF.
 * @note  2.Trigger set value must be greater than start value if user want to use it's trigger function.
 * @note  3.GTMR start to work only after it is triggered,it can be triggered by software or hardware(trgmux).
 * @note  4.GTMR interrupt type is overflow interrupt,an interrupt will occur after the time value reaches 0xFFFF_FFFF.
 * @note  5.GTMR frequency is fixed to 1Mhz.
 *
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __GTMR_DRIVER_H__
#define __GTMR_DRIVER_H__

#include "status.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup GTMR_Driver GTMR Driver
 * @{
 */

/**
 * @brief Definition for the gtmr channel callback function.
 */
typedef void (*gtmr_callback_t)(void *parameter);

/**
 * @brief GTMR trigger mode type.
 */
typedef enum {
    GTMR_TRIGGERMODE_HARD = 0U, /**< Hardware Trigger. */
    GTMR_TRIGGERMODE_SOFT = 1U, /**< Software Trigger. */
} gtmr_Triggermode_t;

/**
 * @brief Defines the configuration structure for GTMR.
 */
typedef struct {
    gtmr_Triggermode_t triggerMode; /**< Mode that GTMR is triggered,GTMR must be triggered to start count. \D{GTMR_TRIGGERMODE_SOFT} */
    bool interruptEnable;           /**< Enable/Disable Interrupt. \D{true} */
    bool continuousModeEnable;      /**< Enable Continuous trigger mode. \D{true} */
    uint32_t startValue;            /**< GTMR counter begin to count from this value. \D{0xFFFE0000} */
    uint32_t triggerValue;          /**< GTMR output a trigger signal when counter equals to TriggerValue. \D{0xFFFF2000} */
    gtmr_callback_t callBack;       /**< Callback function pointer for the GTMR */
    void *parameter;                /**< GTMR callBack parameter */
} gtmr_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Interrupt registration function.
 * 
 * @param  instance     GTMR instance.     
 * @param  callback     Callback function.   
 * @param  parameter    The argument to the callback function.     
 * @return ** status_t 
 *         STATUS_SUCCESS  if succeed.
 *         STATUS_ERRORPAR if error in input parameters.
 */
status_t GTMR_DRV_InstallCallback(uint32_t instance, gtmr_callback_t callback, void *parameter);

/**
 * @brief  Initialize GTMR Module,configure GTMR working mode according to structure gtmr_config_t.
 *
 * @param  instance GTMR instance number
 * @param  config   Pointer to the input configuration structure
 */
void GTMR_DRV_Init(const uint32_t instance, const gtmr_config_t *const config);

/**
 * @brief  Configure a GTMR instance based on the input configuration structure.such as interrupt.trigger mode.start
 * value.
 *
 * @param  instance   GTMR instance number
 * @param  config     Pointer to the input configuration structure
 */
void GTMR_DRV_SetConfig(const uint32_t instance, const gtmr_config_t *const config);

/**
 * @brief Start GTMR Module.The GTMR function begin to start counting.
 *
 * @param  instance GTMR instance number
 * @param  config   Pointer to the input configuration structure
 */
void GTMR_DRV_StartCounter(const uint32_t instance, const gtmr_config_t *const config);

/**
 * @brief  Stop the GTMR count, not stop GTMR module. Write 1 to stop the GTMR count,write 0 to restart GTMR count.
 *
 * @param  instance  GTMR instance number
 * @param  enable    enable or disable Stop Mode
 */
void GTMR_DRV_StopCounter(const uint32_t instance, bool enable);

/**
 * @brief Get GTMR timer full mask(FULL register) state,full mask also is the interrupt flag.
 *
 * GTMR is an overflow interrupt,when the counter equals to 0xFFFF_FFFF it  generate an interrupt.
 * 
 * @param  instance  GTMR instance number
 * @return true if  FULL Flag is 1
 * @return false if FULL Flag is 0
 */
bool GTMR_DRV_GetIntFlag(const uint32_t instance);

/**
 * @brief Clear interrupt Flag.
 *
 * GTMR is an overflow interrupt,when the counter equals to 0xFFFF_FFFF it  generate an interrupt.
 * 
 * @param  instance GTMR instance number.
 */
void GTMR_DRV_ClearIntFlag(const uint32_t instance);

/**
 * @brief Get the run state of a GTMR instance.
 *
 * @param  instance GTMR instance number.
 * @return true: Timer/Counter started.
 * @return false: Timer/Counter stopped.
 */
bool GTMR_DRV_IsRunning(const uint32_t instance);

/**
 * @brief  Enable/disable the GTMR interrupt.
 *
 * GTMR is an overflow interrupt,when the counter equals to 0xFFFF_FFFF it generate an interrupt.
 * 
 * @param  instance  GTMR instance.
 * @param  enableInterrupt The new state of the GTMR interrupt enable flag.
 */
void GTMR_DRV_SetInterrupt(const uint32_t instance, const bool enableInterrupt);

/**
 * @brief  Get the current counter value.
 *
 * @param  instance  GTMR instance
 * @return uint32_t
 *          The current counter value
 */
uint32_t GTMR_DRV_GetCounterValue(const uint32_t instance);

/**
 * @brief GTMR counter reset not GTMR module reset,The timer value set to initial value set by user after counter
 * reset.
 *
 * @param  instance GTMR instance number
 */
void GTMR_DRV_CounterReset(const uint32_t instance);

/**
 * @brief Enable Once Trigger.
 *
 * If GTMR is running in single-trigger mode,this function must be called to enable trigger out.
 * This function can be ignored if GTMR work in continues-trigger mode. 
 * 
 * @param  instance GTMR instance.
 */
void GTMR_DRV_GenOnceTrigger(const uint32_t instance);

/**
 * @brief De-initialize GTMR Module
 *
 * stop the GTMR and reset all registers to default value.
 *
 * @param  instance GTMR instance number
 */
void GTMR_DRV_Deinit(const uint32_t instance);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group GTMR_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __GTMR_DRIVER_H__ */