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
 * @file ewm_driver.h
 * @brief EWM driver header file
 * @note 1.The EWM can be enabled only once after it is powered on or reset,if the EWM is enabled again,a bus error
 * occurs.
 *
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __EWM_DRIVER_H__
#define __EWM_DRIVER_H__

#include "emps_platform.h"
#include "osal.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup EWM_Driver EWM Driver
 * @{
 */

#define EWM_ENABLE (0x1U)

#define EWM_DISABLE (0x0U)

/** @brief EWM Compare High register maximum value */
#define EWM_CMPH_MAX_VALUE (0xFEU)

/** @brief EWM Compare Low register minimum value */
#define EWM_CMPL_MIN_VALUE (0x00U)

/** @brief First byte of the EWM Service key */
#define EWM_KEY_FIRST_BYTE (0xB4U)

/** @brief Second byte of the EWM Service key */
#define EWM_KEY_SECOND_BYTE (0x2CU)

/**
 * @brief Definition for the EWM channel callback function.
 */
typedef void (*ewm_callback_t)(void *parameter);

/**
 * @brief EWM input pin configuration.
 */
typedef enum {
    EWM_IN_ASSERT_DISABLED      = 0x00U, /**< Input pin disabled. */
    EWM_IN_ASSERT_ON_LOGIC_ZERO = 0x01U, /**< Input pin asserts EWM when on logic 0. */
    EWM_IN_ASSERT_ON_LOGIC_ONE  = 0x02U  /**< Input pin asserts EWM when on logic 1. */
} ewm_in_assert_logic_t;

/**
 * @brief Declarations of configuration structures
 */
typedef struct {
    ewm_in_assert_logic_t assertLogic; /**< Assert logic for EWM input pin. \D{EWM_IN_ASSERT_DISABLED} */
    bool interruptEnable;              /**< Enable EWM interrupt. \D{true} */
    uint8_t prescaler;                 /**< EWM clock prescaler. \D{0x1} */
    uint8_t compareLow;                /**< Compare low value. \D{0x2} */
    uint8_t compareHigh;               /**< Compare high value. \D{0xFE} */
    ewm_callback_t callBack;           /**< Callback function pointer for the EWM */
    void *parameter;                   /**< EWM callBack parameter */
} ewm_init_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Interrupt registration function.
 * 
 * @param  instance     EWM instance.     
 * @param  callback     Callback function.   
 * @param  parameter    The argument to the callback function.     
 * @return ** status_t 
 *         STATUS_SUCCESS  if succeed.
 *         STATUS_ERRORPAR if error in input parameters.
 */
status_t EWM_DRV_InstallCallback(uint32_t instance, ewm_callback_t callback, void *parameter);

/**
 * @brief  Init EWM Module.
 *
 * Config EWM enable interrupt,configure refresh window,configure EWM EWM_in pin and enable EWM at last.
 *
 * @param  instance EWM instance.
 * @param  config   Pointer to the module configuration structure.
 * @return status_t
 *         - STATUS_SUCCESS: The operation is successful.
 *         - STATUS_ERROR: The windows values are not correct or the instance is already enabled.
 */
status_t EWM_DRV_Init(uint32_t instance, const ewm_init_config_t *config);

/**
 * @brief Refresh EWM Module similar to WDOG module service the dog
 *
 * @param  instance EWM instance number
 */
void EWM_DRV_Refresh(uint32_t instance);

/**
 * @brief Clear EWM interrupt Flag
 *
 * @param  instance EWM instance number
 */
void EWM_DRV_ClearIntFlag(uint32_t instance);

/**
 * @brief Get the default configuration structure.
 *
 * @param  config Pointer to the configuration structure to initialize.
 */
void EWM_DRV_GetDefaultConfig(ewm_init_config_t *config);

/**
 * @brief Get the EWM_in pin assert logic.
 *
 * @param  instance EWM instance number
 * @return ewm_in_assert_logic_t
 *         EWM_IN_ASSERT_DISABLED      if EWM in disabled
 *         EWM_IN_ASSERT_ON_LOGIC_ZERO if EWM is asserted when EWM_in is logic 0
 *         EWM_IN_ASSERT_ON_LOGIC_ONE  if EWM is asserted when EWM_in is logic 1
 */
ewm_in_assert_logic_t EWM_DRV_GetInputPinAssertLogic(uint32_t instance);

/**
 * @brief De-initialize GTMR Module
 * 
 * The EWM module cannot be disabled after it is disabled.therefore, the function is 
 * used to disable the interrupt function on the EWM NVIC layer and deregister the interrupt function.
 * 
 * @param  instance EWM instance number
 */
void EWM_DRV_Deinit(const uint32_t instance);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group FLEXCAN_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif
