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
 * @file wdog_driver.h
 * @brief WDOG driver header file.
 *
 * @note 1.WDOG must be disabled before config WDOG function register.
 * @note 2.WDOG has software reset function. see WDOG_DRV_SoftWareReset() for more details\n.
 * @note 3.WCR.WDW, WCR.WDBG, WCR.WDZST, WICR.WICT, WICR.WIE registers can only be written once before the next reset.
 *
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __WDOG_DRIVER_H__
#define __WDOG_DRIVER_H__

#include "emps_platform.h"
#include "status.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup WDOG_Driver WDOG Driver
 * @{
 */

/**
 * @brief Definition for the gtmr channel callback function.
 */
typedef void (*wdog_callback_t)(void *parameter);

/* @brief Default reset value of the CR register. */
#define WDOG_CR_RESET_VALUE (0x10U)

/* @brief The value minimum of the timeout register. */
#define WDOG_MINIMUM_TIMEOUT_VALUE (0x0U)

/* @brief The reset value of the timeout register. */
#define WDOG_TO_RESET_VALUE (0x400U)

#define WDOG_TO_RESET_INT_VALUE (0x40U)

/* @brief WDOG max timeout value.  */
#define WDOG_MAX_TIMEOUT_VALUE (32768.0F)

/* @brief WDOG min timeout value. */
#define WDOG_MIN_TIMEOUT_VALUE (0.5F)

/* @brief WDOG max interrupt value*/
#define WDOG_MAX_INTERRUPT_VALUE (32767.5F)

/* @brief WDOG min interrupt value*/
#define WDOG_MIN_INTERRUPT_VALUE (0.0F)

/**
 * @brief Set work mode for the WDOG.
 */
typedef enum {
    WDOG_WORK_CONTINUE = 0x00U, /* Work continue. */
    WDOG_WORK_SUSPEND  = 0x01U, /* Work suspend. */
} wdog_work_mode_t;

/**
 * @brief Set reset status of the WDOG.
 */
typedef enum {
    WDOG_SOFTWARE = 0U, /* Software reset status. */
    WDOG_TIMEOUT  = 1U, /* Timeout reset status.  */
    WDOG_POWER_ON = 4U, /* Power on reset status. */
} wdog_reset_status_t;

/**
 * @brief Set SoftWare mode:immediate reset or Delayed reset
 */
typedef enum {
    WDOG_SOFTWARE_IMMEDIATE_RESET = 0x00U, /* Work continue. */
    WDOG_SOFTWARE_DELAY_RESET     = 0x01U, /* Work suspend. */
} wdog_software_reset_mode_t;

/**
 * @brief WDOG user configuration structure.
 */
typedef struct {
    wdog_work_mode_t stop;    /**< Whether WDOG work in stop model. true: work continue. false:work suspend */
    bool intEnable;           /**< If true, an interrupt request is generated before reset. */
    uint16_t timeoutValue;    /**< The timeout value. Timeout time = (timeoutValue * 0.5) + 0.5  */
    uint16_t intValue;        /**< Interrupt before timeout.  Interrupt time  = intValue * 0.5 */
    wdog_callback_t callBack; /**< Callback function pointer for the WDOG */
    void *parameter;          /**< WDOG callBack parameter */
} wdog_user_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Interrupt registration function.
 * 
 * @param  instance     WDOG  instance.     
 * @param  callback     Callback function.   
 * @param  parameter    The argument to the callback function.     
 * @return ** status_t 
 *         STATUS_SUCCESS  if succeed.
 *         STATUS_ERRORPAR if error in input parameters.
 */
status_t WDOG_DRV_InstallCallback(uint32_t instance, wdog_callback_t callback, void *parameter);

/**
 * @brief Initializes WDOG.
 *
 *  WDOG_DRV_Init contains config WDOG timeout value,config WDOG interrupt value, enable or disable WDOG interrupt,
 * enable WDOG module.
 *
 * @param  instance WDOG  instance.
 * @param  userConfigPtr Pointer to the WDOG user configuration structure.
 * @return status_t
 *         STATUS_SUCCESS  if init succeed.
 *         STATUS_ERROR    if init failed.
 *         STATUS_ERRORPAR wdog_user_config_t struct timeout value or interrupt value error.
 */
status_t WDOG_DRV_Init(uint32_t instance, const wdog_user_config_t *userConfigPtr);

/**
 * @brief  De-initializes WDOG.
 *
 * @param  instance WDOG instance.
 */
void WDOG_DRV_Deinit(uint32_t instance);

/**
 * @brief Get current configuration of the WDOG.
 *
 * @param  instance WDOG instance.
 * @param  config  current WDOG wdog_user_config_t struct.
 */
void WDOG_DRV_GetConfig(uint32_t instance, wdog_user_config_t *const config);

/**
 * @brief Get default wdog_user_config_t struct.
 *
 * @param  config  default wdog_user_config_t struct.
 */
void WDOG_DRV_GetDefaultConfig(wdog_user_config_t *const config);

/**
 * @brief  Clear interrupt flag of the WDOG.
 *
 * @param  instance WDOG instance.
 */
void WDOG_DRV_ClearIntFlag(uint32_t instance);

/**
 * @brief  Refreshes the WDOG counter.
 *
 * @param  instance WDOG instance number.
 */
void WDOG_DRV_Trigger(uint32_t instance);

/**
 * @brief Gets the value of the WDOG counter.
 *
 * @param  instance WDOG instance.
 * @return uint16_t The value of the WDOG counter.
 */
uint16_t WDOG_DRV_GetCounter(uint32_t instance);

/**
 * @brief Set the value of the WDOG timeout.
 *
 * @param  instance WDOG instance.
 * @param  timeout  The value of the WDOG timeout.
 */
void WDOG_DRV_SetTimeoutValue(uint32_t instance, uint16_t timeout);

/**
 * @brief  Set the time before the counter timeout that the interrupt happens.
 *
 * This field is write once only.Once the software does a write access to this field, is will get locked and cannot be
 * reprogrammed until the next system reset assertion.
 *
 * @param  instance WDOG instance.
 * @param  intval   The value of the interrupt before timeout.
 */
void WDOG_DRV_SetInt(uint32_t instance, const uint16_t intval);

/**
 * @brief  WDOG_DRV_StartWDOG.
 *
 * @param  instance WDOG instance.
 */
void WDOG_DRV_StartWDOG(uint32_t instance);

/**
 * @brief  WDOG software reset.
 *
 * WDOG system reset can be triggered directly through this function. Software reset mode can be divide into immediate
 * reset and delay reset. delay reset means delay two 32K cycles before reset.
 *
 * @param  instance  WDOG instance.
 * @param  mode      software reset mode,immediate reset or delayed reset.
 */
void WDOG_DRV_SoftWareReset(uint32_t instance, wdog_software_reset_mode_t mode);

#if defined(__cplusplus)
}
#endif
/**
 * @}
 */
/* end of group WDOG_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */
#endif /* __WDOG_DRIVER_H__ */
