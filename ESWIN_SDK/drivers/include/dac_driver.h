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
 * @file dac_driver.h
 * @brief DAC driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __DAC_DRIVER_H__
#define __DAC_DRIVER_H__

#include "emps_platform.h"
#include "status.h"

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup DAC_Driver DAC Driver
 * @{
 */

/**
 * @brief Definition for the DAC callback function.
 *
 * Prototype for the callback function registered in the PMU driver.
 */
typedef void (*dac_callback_t)(void *parameter);

/**
 * @brief Data structure for the dac state.
 *
 * This structure is used when calling the IOPMP_DRV_Init function.
 */
typedef struct {
    uint32_t instance;       /**< instance. */
    void *callbackParam;     /**< callback parameters. */
    dac_callback_t callback; /**< The callback of interrupt. */
} dac_state_t;

/**
 * @brief Defines the comparator module configuration.
 *
 * This structure is used to configure all components of comparator module.
 */
typedef struct {
    bool interruptEnable;              /**< True if interrupt is enabled. \D{false} */
    bool outputVolBuf;                 /**< True if voltage buffer is enabled. \D{false} */
    bool state;
} dac_module_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Reset all registers.
 *
 * This function set all DAC registers to reset values.
 *
 * @param  instance Instance number.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t DAC_DRV_Reset(const uint32_t instance);

/**
 * @brief Configure all comparator features with the given configuration structure.
 *
 * This function configures the comparator module with the options provided in the config structure.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 * @param  state The state structure.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t DAC_DRV_Init(const uint32_t instance, const dac_module_t *const config, dac_state_t *state);

/**
 * @brief  Return current configuration for DAC.
 *
 * This function returns current configuration only for DAC.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t DAC_DRV_GetConfig(const uint32_t instance, dac_module_t *const config);

/**
 * @brief Get the DAC Sampling  rate.
 *
 * @param  instance Dac base pointer.
 * @param  sampleRate Dac output sampling rate.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 */
status_t DAC_DRV_GetSampleRate(const uint32_t instance, uint32_t *sampleRate);

/**
 * @brief Sets the Sample Rate configuration.
 *
 * This function returns the configured sample rate for the DAC.
 *
 * @param  instance Dac instance.
 * @param  sampleRate The dac sample rate.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 */
status_t DAC_DRV_SetSampleRate(const uint32_t instance, const uint32_t sampleRate);

/**
 * @brief Set DAC input data.
 *
 *This function Set DAC input data.

 * @param  instance Instance number.
 * @param  data the input data
 * @return status_t
 * STATUS_SUCCESS : Completed successfully.
 */
status_t DAC_DRV_SetDataInput(const uint32_t instance, uint32_t data);

/**
 * @brief Enable DAC.
 *
 * @param  instance Instance number.
 * @param  enable The dac enable state.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 */
status_t DAC_DRV_SetEnableState(const uint32_t instance, bool enable);

/**
 * @brief Set DAC Interrupt enable.
 *
 * This function Set DAC Interrupt enable.
 *
 * @param  instance Instance number.
 * @param  enable The dac interrupt enable state.
 * @return status_t
 * STATUS_SUCCESS : Completed successfully.
 */
status_t DAC_DRV_SetIntEnable(const uint32_t instance, bool enable);

/**
 * @brief Get DAC FIFO full flag.
 *
 * This function Get DAC FIFO full flag.
 *
 * @param  instance Instance number.
 * @return status_t
 * true : FIFO is full.
 * false : FIFO is not full.
 */
bool DAC_DRV_GetFiFOFullState(const uint32_t instance);

/**
 * @brief Get DAC FIFO empty flag.
 *
 * This function Get DAC FIFO empty flag.
 *
 * @param  instance Instance number.
 * @return status_t
 * true : FIFO is empty.
 * false : FIFO is not empty.
 */
bool DAC_DRV_GetFiFOEmptyState(const uint32_t instance);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group DAC_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif
