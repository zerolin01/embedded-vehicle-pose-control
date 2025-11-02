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
 * @file pdu_driver.h
 * @brief PDU driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PDU_DRIVER_H__
#define __PDU_DRIVER_H__

#include "clock_driver.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup PDU_Driver PDU Driver
 * @{
 */

/**
 * @brief Definition for the pdu channel callback function.
 */
typedef void (*pdu_callback_t)(void *parameter);

/**
 * @brief  Interrupt registration function.
 *
 * @param  instance   PDU  instance.
 * @param  callback   Callback function.
 * @param  parameter  The argument to the callback function.
 * @return ** status_t
 *         STATUS_SUCCESS  if succeed.
 *         STATUS_ERRORPAR if error in input parameters.
 */
status_t PDU_DRV_InstallCallback(uint32_t instance, pdu_callback_t callback, void *parameter);

/**
 * @brief  Interrupt registration function.
 *
 * @param  instance   PDU  instance.
 * @param  chn        PDU channel.
 * @param  callback   Callback function.
 * @param  parameter  The argument to the callback function.
 * @return ** status_t
 *         STATUS_SUCCESS  if succeed.
 *         STATUS_ERRORPAR if error in input parameters.
 */
status_t PDU_ERR_DRV_InstallCallback(uint32_t instance, pdu_callback_t callback, void *parameter);

/**
 * @brief Defines the type of value load mode for the PDU module.
 */
typedef enum {
    PDU_LOAD_VAL_IMMEDIATELY                       = 0U, /**< Loaded immediately after load operation. */
    PDU_LOAD_VAL_AT_MODULO_COUNTER                 = 1U, /**< Loaded when counter hits the modulo after load operation. */
    PDU_LOAD_VAL_AT_NEXT_TRIGGER                   = 2U, /**< Loaded when detecting an input trigger after load operation. */
    PDU_LOAD_VAL_AT_MODULO_COUNTER_OR_NEXT_TRIGGER = 3U, /**< Loaded when counter hits the modulo or \
                                                        detecting an input trigger after load operation. */
} pdu_load_value_mode_t;

/**
 * @brief Defines the type of prescaler divider for the PDU counter clock.
 */
typedef enum {
    PDU_CLK_PREDIV_BY_1   = 0U, /**< Counting divided by   1 x prescaler multiplication factor (selected by MULT). */
    PDU_CLK_PREDIV_BY_2   = 1U, /**< Counting divided by   2 x prescaler multiplication factor (selected by MULT). */
    PDU_CLK_PREDIV_BY_4   = 2U, /**< Counting divided by   4 x prescaler multiplication factor (selected by MULT). */
    PDU_CLK_PREDIV_BY_8   = 3U, /**< Counting divided by   8 x prescaler multiplication factor (selected by MULT). */
    PDU_CLK_PREDIV_BY_16  = 4U, /**< Counting divided by  16 x prescaler multiplication factor (selected by MULT). */
    PDU_CLK_PREDIV_BY_32  = 5U, /**< Counting divided by  32 x prescaler multiplication factor (selected by MULT). */
    PDU_CLK_PREDIV_BY_64  = 6U, /**< Counting divided by  64 x prescaler multiplication factor (selected by MULT). */
    PDU_CLK_PREDIV_BY_128 = 7U, /**< Counting divided by 128 x prescaler multiplication factor (selected by MULT). */
} pdu_clk_prescaler_div_t;

/**
 * @brief Defines the type of trigger source mode for the PDU.
 */
typedef enum {
    PDU_TRIGGER_IN0      = 0U,  /**< Source trigger comes from TRGMUX. */
    PDU_SOFTWARE_TRIGGER = 15U, /**< Select software trigger. */
} pdu_trigger_src_t;

/**
 * @brief Defines the type of the multiplication source mode for PDU.
 */
typedef enum {
    PDU_CLK_PREMULT_FACT_AS_1  = 0U, /**< Multiplication factor is  1. */
    PDU_CLK_PREMULT_FACT_AS_10 = 1U, /**< Multiplication factor is 10. */
    PDU_CLK_PREMULT_FACT_AS_20 = 2U, /**< Multiplication factor is 20. */
    PDU_CLK_PREMULT_FACT_AS_40 = 3U, /**< Multiplication factor is 40. */
} pdu_clk_prescaler_mult_factor_t;

/**
 * @brief Defines the type of structure for basic timer in PDU.
 */
typedef struct {
    pdu_load_value_mode_t loadValueMode;              /**< Select the load mode. \D{PDU_LOAD_VAL_IMMEDIATELY} */
    bool seqErrIntEnable;                             /**< Enable PDU Sequence Error Interrupt. \D{false} */
    pdu_clk_prescaler_div_t clkPreDiv;                /**< Select the prescaler divider. \D{PDU_CLK_PREDIV_BY_2} */
    pdu_clk_prescaler_mult_factor_t clkPreMultFactor; /**< Select multiplication factor for prescaler. \D{PDU_CLK_PREMULT_FACT_AS_10} */
    pdu_trigger_src_t triggerInput;                   /**< Select the trigger input source. \D{PDU_SOFTWARE_TRIGGER} */
    bool continuousModeEnable;                        /**< Enable the continuous mode. \D{true} */
    bool dmaEnable;                                   /**< Enable the dma for timer. \D{false} */
    bool intEnable;                                   /**< Enable the interrupt for timer. \note: interrupt is generated only if PDMA is disabled. \D{true} */
    bool instanceBackToBackEnable;                    /**< Enable the instance back to back mode between PDU0 CH0 and PDU1 CH0
                                       pre-triggers, forming a ring (PDU0_CH0_pretrigger7 -> PDU1_CH0_pretrigger0 and
                                       PDU1_CH0_pretrigger7 -> PDU0_CH0_pretrigger0). \n \note: this configuration is
                                       common between all PDU instances (configures the same register
                                       TCSR_PDUBBSEL[PDU_BB_SEL] even if configured for either PDU instance) Enabling
                                       when configuring PDU0, then disabling when configuring PDU1, will result in being
                                       disabled. \n \note: must not be enabled simultaneously with inter-channel back to
                                       back on any PDU instance. \D{false} */
    bool interchannelBackToBackEnable;                /**< Enable the inter-channel back to back mode between PDUx CH0 and PDUx CH1
                                          pre-triggers, forming a ring (PDUx_CH0_pretrigger7 -> PDUx_CH1_pretrigger0 and
                                          PDUx_CH1_pretrigger7 -> PDUx_CH0_pretrigger0). \n \note: this configuration is
                                          dedicated to each PDU instance. \n \note: must not be enabled simultaneously
                                          with instance back to back on any PDU instance. \D{false} */

    pdu_callback_t callBack; /**< Callback function pointer for the PDU */

    void *parameter; /**< PDU callBack parameter */

    pdu_callback_t seqErrCallBack; /**< Callback function pointer for the PDU Sequence Error Interrupt*/

    void *seqErrParameter; /**< PDU callBack parameter Sequence Error Interrupt */
} pdu_timer_config_t;

/**
 * @brief Defines the type of structure for configuring ADC's pre_trigger.
 */
typedef struct {
    uint32_t adcPreTriggerIdx;       /**< Setting pre_trigger's index. \D{0} */
    bool preTriggerEnable;           /**< Enable the pre_trigger. \D{true} */
    bool preTriggerOutputEnable;     /**< Enable the pre_trigger output. @internal gui name="Trigger output"
                                        id="AdcTriggerOutput" \D{false} */
    bool preTriggerBackToBackEnable; /**< Enable the back to back mode for ADC pre_trigger.
                                        If enabled, the pretrigger will be activated automatically when the ADC COCO
                                        flag corresponding to the previous pretrigger in the chain, is set.
                                        The previous pretrigger for pretriggers with index 0, depend on features
                                        instanceBackToBackEnable and interchannelBackToBackEnable. \D{false} */

} pdu_adc_pretrigger_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Initializes the PDU counter and triggers input.
 *
 * This function initializes the PDU counter,  input triggers and general pre-trigger
 * settings. It resets  PDU registers and enables the clock for  PDU. So it should be
 * called before any operation to  PDU module. After initialized, the  PDU can
 * act as a triggered timer, which lays the foundation for other features in
 * PDU module.
 *
 * @param  instance PDU instance ID.
 * @param  userConfigPtr Pointer to the user configuration structure.
 */
void PDU_DRV_Init(const uint32_t instance, const pdu_timer_config_t *userConfigPtr);

/**
 * @brief De-initializes the PDU module.
 *
 * When the  PDU module is not used. Calling this function would shut down the
 * PDU module and reduce the power consumption.
 *
 * Note: instance back to back configuration is common between  PDU instances 0 and 1 (configures the same register even
 * if configured for either  PDU instance) This function disables it, so affects all other instances.
 *
 * @param  instance PDU instance ID.
 */
void PDU_DRV_Deinit(const uint32_t instance);

/**
 * @brief Gets the default configuration structure of PDU with default settings.
 *This function is used to initialize the hardware configuration structure to default values.
 * @param  config Pointer to PDU configuration structure.
 */
void PDU_DRV_GetDefaultConfig(pdu_timer_config_t *const config);

/**
 * @brief Enable the PDU module.
 *
 * @param  instance  PDU instance(0 ~ 1).
 */
void PDU_DRV_Enable(const uint32_t instance);

/**
 * @brief  Disables the  PDU module.
 *
 * @param  instance  PDU instance(0 ~ 1).
 */
void PDU_DRV_Disable(const uint32_t instance);

/**
 * @brief Trigger PDU by software.
 *
 * When PDU is set to software trigger modes, user calling this function
 * can trigger the PDU module.
 *
 * @param  instance  PDU instance(0 ~ 1).
 */
void PDU_DRV_SoftTriggerCmd(const uint32_t instance);

/**
 * @brief Gets the current value of the  PDU counter.
 *
 * @param  instance  PDU instance(0 ~ 1).
 * @return uint32_t
 *         Current  PDU counter value.
 */
uint32_t PDU_DRV_GetTimerValue(const uint32_t instance);

/**
 * @brief  Gets the PDU interrupt flag
 *
 * @param  instance  PDU instance(0 ~ 1).
 * @return bool
 *         interrupt flag.
 */
bool PDU_DRV_GetTimerIntFlag(const uint32_t instance);

/**
 * @brief Clears the interrupt flag.
 *
 * @param  instance PDU instance ID.
 */
void PDU_DRV_ClearTimerIntFlag(const uint32_t instance);

/**
 * @brief Executes the command of loading values.
 *
 * The new values of MOD, IDLY, CHnDLYm, and POyDLY registers will not take effect until user calling this function.
 *
 * @param  instance PDU instance(0 ~ 1).
 */
void PDU_DRV_LoadValuesCmd(const uint32_t instance);

/**
 * @brief Set PDU timer modulus.
 *
 * The period of the counter.when the counter reaches this value, it will be reset back to zero.
 *
 * @param  instance  PDU instance(0 ~ 1).
 * @param  value  PDU  modulus value.
 */
void PDU_DRV_SetTimerModulusValue(const uint32_t instance, const uint16_t value);

/**
 * @brief Set the value for the PDU interrupt.
 *
 * when the counter is equal to the IDLY, PDU module generate an interrupt.
 *
 * @param  instance  PDU instance(0 ~ 1).
 * @param  value    interrupt value.
 */
void PDU_DRV_SetValueForTimerInterrupt(const uint32_t instance, const uint16_t value);

/**
 * @brief Configure PDU channel function.
 *
 * configure PDU pre-trigger function include enable or disable pre-trigger,enable or disable bypass mode,
 * enable or disable back to back mode.
 *
 * @param  instance   PDU instance(0 ~ 1).
 * @param  chn        PDU channel(0 ~ 2).
 * @param  configPtr Pointer to the user configuration structure.
 */
void PDU_DRV_ConfigAdcPreTrigger(const uint32_t instance, const uint32_t chn,
                                 const pdu_adc_pretrigger_config_t *configPtr);

/**
 * @brief  Get PDU channel pre-trigger flag in the  PDU module.
 *
 * The CF[m] field  is set when the PDU counter(PDU_CNT) matches the value ChnDLYm + 1.
 *
 * @param  instance  PDU instance.(0 ~ 1)
 * @param  chn       PDU channel(0 ~ 2)
 * @param  preChnMask PDU channel PDU_CHnS CF mask
 * @return ** uint32_t PDU_CHnS CF state.
 */
uint32_t PDU_DRV_GetAdcPreTriggerFlags(const uint32_t instance, const uint32_t chn, const uint32_t preChnMask);

/**
 * @brief Clear PDU channel  pre_trigger flag.
 *
 * @param  instance PDU instance(0 ~ 1).
 * @param  chn  PDU channel(0 ~ 2).
 * @param  preChnMask PDU_CHnS CF flag.
 */
void PDU_DRV_ClearAdcPreTriggerFlags(const uint32_t instance, const uint32_t chn, const uint32_t preChnMask);

/**
 * @brief Get PDU sequence error flag.
 *
 * @param  instance    PDU instance(0 ~ 1).
 * @param  chn         PDU channel(0 ~2).
 * @param  preChnMask  PDU  sequence error mask.
 * @return uint32_t
 *         sequence error flag.
 */
uint32_t PDU_DRV_GetAdcPreTriggerSeqErrFlags(const uint32_t instance, const uint32_t chn, const uint32_t preChnMask);

/**
 * @brief Clear the sequence error flag.
 *
 * @param  instance  PDU instance(0 ~ 1).
 * @param  chn       PDU channel(0 ~ 2).
 * @param  preChnMask  PDU channel sequence error mask.
 */
void PDU_DRV_ClearAdcPreTriggerSeqErrFlags(const uint32_t instance, const uint32_t chn, const uint32_t preChnMask);

/**
 * @brief Set the PDU pre_trigger delay value in the  PDU module.
 *
 * The pre-trigger asserts when the counter is equal to DLY.
 *
 * @param  instance PDU instance,(0 ~ 1).
 * @param  chn      PDU channel(0 ~ 2).
 * @param  preChn   PDU pre channel(0 ~7).
 * @param  DLY      PDU pre-trigger delay value.
 */
void PDU_DRV_SetAdcPreTriggerDelayValue(const uint32_t instance, const uint32_t chn, const uint32_t preChn,
                                        const uint16_t DLY);

/**
 * @brief  enable or disable the CMP pulse out.
 *
 * @param  instance      PDU instance .
 * @param  pulseChnMask  Pulse channel mask.
 * @param  enable        enable or disable.
 */
void PDU_DRV_SetCmpPulseOutEnable(const uint32_t instance, const uint32_t pulseChnMask, bool enable);

/**
 * @brief Set the CMP pulse out low and high delay value for the PDU module.
 *
 * Pulse-Out goes high when the PDU counter is equal to DLY1,Pulse-Out goes low when the PDU counter goes to the DLY2.
 *
 * @param  instance  PDU instance.
 * @param  pulseChn  PDU  Pulse channel.
 * @param  DLY1 PDU_POnDLY register bit(16 ~ 31) DLY1.
 * @param  DLY2 PDU_POnDLY register bit(0 ~ 15)  DLY2.
 */
void PDU_DRV_SetCmpPulseOutDelay(const uint32_t instance, const uint32_t pulseChn, const uint16_t DLY1,
                                 const uint16_t DLY2);
#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group PDU_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */
#endif
