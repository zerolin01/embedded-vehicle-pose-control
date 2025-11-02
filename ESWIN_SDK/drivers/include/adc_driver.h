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
 * @file adc_driver.h
 * @brief ADC driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __ADC_DRIVER_H__
#define __ADC_DRIVER_H__

#include "emps_platform.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup ADC_Driver ADC Driver
 * @{
 */

/**
 * @brief Conversion resolution selection.
 */
typedef enum {
    ADC_RESOLUTION_8BIT  = 0x00U, /**< 8-bit resolution mode. */
    ADC_RESOLUTION_10BIT = 0x01U, /**< 10-bit resolution mode. */
    ADC_RESOLUTION_12BIT = 0x02U, /**< 12-bit resolution mode. */
} adc_resolution_t;

/**
 * @brief Trigger type selection.
 */
typedef enum {
    ADC_TRIGGER_SOFTWARE = 0x00U, /**< Software trigger. */
    ADC_TRIGGER_HARDWARE = 0x01U, /**< Hardware trigger. */
} adc_trigger_t;

/**
 * @brief Pretrigger types selectable from Trigger Latching and Arbitration Unit.
 */
typedef enum {
    ADC_PRETRIGGER_SEL_PDU    = 0x00U, /**< PDU pretrigger selected. */
    ADC_PRETRIGGER_SEL_TRGMUX = 0x01U, /**< TRGMUX pretrigger selected. */
    ADC_PRETRIGGER_SEL_SW     = 0x02U, /**< Software pretrigger selected. */
} adc_pretrigger_sel_t;

/**
 * @brief Trigger source selectable from Trigger Latching and Arbitration Unit.
 */
typedef enum {
    ADC_TRIGGER_SEL_PDU    = 0x00U, /**< PDU trigger selected. */
    ADC_TRIGGER_SEL_TRGMUX = 0x01U, /**< TRGMUX trigger selected. */
} adc_trigger_sel_t;

/**
 * @brief Software pretriggers which may be set from Trigger Latching and Arbitration Unit.
 */
typedef enum {
    ADC_SW_PRETRIGGER_DISABLED = 0x00U, /**< SW pretrigger disabled. */
    ADC_SW_PRETRIGGER_0        = 0x04U, /**< SW pretrigger 0. */
    ADC_SW_PRETRIGGER_1        = 0x05U, /**< SW pretrigger 1. */
    ADC_SW_PRETRIGGER_2        = 0x06U, /**< SW pretrigger 2. */
    ADC_SW_PRETRIGGER_3        = 0x07U, /**< SW pretrigger 3. */
} adc_sw_pretrigger_t;

/**
 * @brief Hardware average selection.
 */
typedef enum {
    ADC_AVERAGE_4  = 0x00U, /**< Hardware average of  4 samples. */
    ADC_AVERAGE_8  = 0x01U, /**< Hardware average of  8 samples. */
    ADC_AVERAGE_16 = 0x02U, /**< Hardware average of 16 samples. */
    ADC_AVERAGE_32 = 0x03U, /**< Hardware average of 32 samples. */
} adc_average_t;

/**
 * @brief Enumeration of input channels assignable to a control channel.
 */
typedef enum {
    ADC_INPUTCHAN_EXT0     = 0x00U,                          /**< External input channel  0. */
    ADC_INPUTCHAN_EXT1     = 0x01U,                          /**< External input channel  1. */
    ADC_INPUTCHAN_EXT2     = 0x02U,                          /**< External input channel  2. */
    ADC_INPUTCHAN_EXT3     = 0x03U,                          /**< External input channel  3. */
    ADC_INPUTCHAN_EXT4     = 0x04U,                          /**< External input channel  4. */
    ADC_INPUTCHAN_EXT5     = 0x05U,                          /**< External input channel  5. */
    ADC_INPUTCHAN_EXT6     = 0x06U,                          /**< External input channel  6. */
    ADC_INPUTCHAN_EXT7     = 0x07U,                          /**< External input channel  7. */
    ADC_INPUTCHAN_EXT8     = 0x08U,                          /**< External input channel  8. */
    ADC_INPUTCHAN_EXT9     = 0x09U,                          /**< External input channel  9. */
    ADC_INPUTCHAN_EXT10    = 0x0AU,                          /**< External input channel 10. */
    ADC_INPUTCHAN_EXT11    = 0x0BU,                          /**< External input channel 11. */
    ADC_INPUTCHAN_EXT12    = 0x0CU,                          /**< External input channel 12. */
    ADC_INPUTCHAN_EXT13    = 0x0DU,                          /**< External input channel 13. */
    ADC_INPUTCHAN_EXT14    = 0x0EU,                          /**< External input channel 14. */
    ADC_INPUTCHAN_EXT15    = 0x0FU,                          /**< External input channel 15. */
    ADC_INPUTCHAN_EXT16    = 0x10U,                          /**< External input channel 16. */
    ADC_INPUTCHAN_EXT17    = 0x11U,                          /**< External input channel 17. */
    ADC_INPUTCHAN_EXT18    = 0x12U,                          /**< External input channel 18. */
    ADC_INPUTCHAN_EXT19    = 0x13U,                          /**< External input channel 19. */
    ADC_INPUTCHAN_EXT20    = 0x14U,                          /**< External input channel 20. */
    ADC_INPUTCHAN_EXT21    = 0x15U,                          /**< External input channel 21. */
    ADC_INPUTCHAN_EXT22    = 0x16U,                          /**< External input channel 22. */
    ADC_INPUTCHAN_EXT23    = 0x17U,                          /**< External input channel 23. */
    ADC_INPUTCHAN_EXT24    = 0x18U,                          /**< External input channel 24. */
    ADC_INPUTCHAN_DISABLED = FEATURE_ADC_INPUTCHAN_DISABLED, /**< Channel disabled.          */
} adc_inputchannel_t;

/**
 * @brief ADMA Data Output Format.
 *
 * <pre>
 *
 * FMT0
 *  63           32         11     0
 *  +------------+----------+------+
 *  | Timestamp  |     R    | Data |
 *  +------------+----------+------+
 *
 * FMT1
 *  32         16  11     0
 *  +----------+---+------+
 *  |  Channel | R | Data |
 *  +----------+---+------+
 *
 * FMT2
 *  15  11     0
 *  +---+------+
 *  | R | Data |
 *  +---+------+
 *
 * </pre>
 *
 */
typedef enum {
    ADC_ADMA_DATAFMT_MODE0, /**< ADMA output FMT0. */
    ADC_ADMA_DATAFMT_MODE1, /**< ADMA output FMT1. */
    ADC_ADMA_DATAFMT_MODE2, /**< ADMA output FMT2. */
} adc_adma_datafmt_t;

/**
 * @brief Type of a ADC Scanning channel representation.
 */
typedef uint32_t scan_channel_type_t;

/**
 * @brief Definition for the ADC callback function.
 *
 * Prototype for the callback function registered in the PMU driver.
 */
typedef void (*adc_callback_t)(void *parameter, uint32_t intStatus);

/**
 * @brief Data structure for the dac state.
 *
 * This structure is used when calling the IOPMP_DRV_Init function.
 */
typedef struct {
    uint32_t instance;       /**< instance. */
    void *callbackParam;     /**< callback parameters. */
    adc_callback_t callback; /**< The callback of interrupt. */
} adc_state_t;

/**
 * @brief Defines the converter configuration.
 */
typedef struct {
    uint32_t sampleRate;                  /**< ADC sample rate (sps). \R{1Ksps,2.85Msps} \D{100000} */
    adc_resolution_t resolution;          /**< ADC resolution (\note EAM2011 only support 12 bit). \D{ADC_RESOLUTION_12BIT} */
    adc_trigger_t trigger;                /**< ADC trigger type (software, hardware) - affects only the first control channel. \D{ADC_TRIGGER_SOFTWARE} */
    adc_pretrigger_sel_t pretriggerSel;   /**< Pretrigger source selected from Trigger Latching and \
                                              Arbitration Unit affects only the first 4 control channels. \D{ADC_PRETRIGGER_SEL_PDU} */
    adc_trigger_sel_t triggerSel;         /**< Trigger source selected from Trigger Latching and Arbitration Unit. \D{ADC_TRIGGER_SEL_PDU} */
    bool dmaEnable;                       /**< Enable ADMA for the ADC. \D{false} */
    adc_adma_datafmt_t dataFmt;           /**< ADMA data format. \D{ADC_ADMA_DATAFMT_MODE1} */
    bool continuousConvEnable;            /**< Enable Continuous conversions. \D{false} */
    bool scanningEnable;                  /**< Enable Scanning mode conversions. \D{false} */
    scan_channel_type_t scanningChannles; /**< Scanning channles. \D{0} */
    bool differenceEnable;                /**< Difference mode enable. \D{0} */
} adc_converter_config_t;

/**
 * @brief Defines the hardware compare configuration.
 *
 * This structure is used to configure the hardware compare feature for the ADC.
 *
 */
typedef struct {
    bool compareEnable;            /**< Enable the compare feature. \D{false} */
    bool compareGreaterThanEnable; /**< Enable Greater-Than functionality. \D{false} */
    bool compareRangeFuncEnable;   /**< Enable Range functionality. \D{false} */
    uint16_t compVal1;             /**< First Compare Value. \D{0x0U} */
    uint16_t compVal2;             /**< Second Compare Value. \D{0x0U} */
} adc_compare_config_t;

/**
 * @brief Defines the hardware average configuration.
 *
 * This structure is used to configure the hardware average feature for the ADC.
 */
typedef struct {
    bool hwAvgEnable;        /**< Enable averaging functionality. \D{false} */
    adc_average_t hwAverage; /**< Selection for number of samples used for averaging. \D{ADC_AVERAGE_4} */
} adc_average_config_t;

/**
 * @brief Defines the control channel configuration.
 *
 * This structure is used to configure a control channel of the ADC.
 */
typedef struct {
    bool interruptEnable;       /**< Enable interrupts for this channel. \D{false} */
    adc_inputchannel_t channel; /**< Selection of input channel for measurement. \D{ADC_INPUTCHAN_EXT8} */
} adc_chan_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @name Converter
 */
/** @{*/

/**
 * @brief Initializes the adc module.
 *
 * @param  instance Instance number.
 * @param  state   The state structure.
 */
void ADC_DRV_Init(const uint32_t instance, adc_state_t *state);

/**
 * @brief DeInitializes the adc module.
 *
 * @param  instance Instance number.
 * @param  state   The state structure.
 */
void ADC_DRV_Deinit(const uint32_t instance);

/**
 * @brief Initializes the converter configuration structure.
 *
 * @param  config The configuration structure.
 */
void ADC_DRV_InitConverterStruct(adc_converter_config_t *const config);

/**
 * @brief  Configures the converter with the given configuration structure.
 *
 * @param  instance Instance number.
 * @param  config   The configuration structure.
 */
void ADC_DRV_ConfigConverter(const uint32_t instance, const adc_converter_config_t *const config);

/**
 * @brief Gets the current converter configuration.
 *
 * @param  instance Instance number.
 * @param  config   The configuration structure.
 */
void ADC_DRV_GetConverterConfig(const uint32_t instance, adc_converter_config_t *const config);

/** @}*/

/**
 * @name Hardware Compare
 * The Hardware Compare feature of the EAM2011 ADC is a versatile mechanism that
 * can be used to monitor that a value is within certain values. Measurements can
 * be monitored to be within certain ranges:
 *  - less than/ greater than a fixed value
 *  - inside or outside of a certain range
 *
 * Two compare values can be configured (the second value is used only for range
 * function mode). The compare values must be written in 12-bit resolution mode
 * regardless of the actual used resolution mode.
 *
 * Once the hardware compare feature is enabled, a conversion is considered
 * complete only when the measured value is within the allowable range set by
 * the configuration.
 */
/** @{*/

/**
 * @brief Initializes the Hardware Compare configuration structure.
 *
 * @param  config The configuration structure.
 */
void ADC_DRV_InitHwCompareStruct(adc_compare_config_t *const config);

/**
 * @brief  Configures the Hardware Compare feature with the given configuration structure.
 *
 * @param  instance Instance number.
 * @param  config   The configuration structure.
 */
void ADC_DRV_ConfigHwCompare(const uint32_t instance, const adc_compare_config_t *const config);

/**
 * @brief  Gets the current Hardware Compare configuration.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 */
void ADC_DRV_GetHwCompareConfig(const uint32_t instance, adc_compare_config_t *const config);

/** @}*/

/**
 * @name Hardware Average
 * The Hardware Average feature of the EAM2011 allows for a set of measurements
 * to be averaged together as a single conversion. The number of samples to be
 * averaged is selectable (4, 8, 16 or 32 samples).
 */
/** @{*/

/**
 * @brief  Initializes the Hardware Average configuration structure.
 *
 * @param  config The configuration structure.
 */
void ADC_DRV_InitHwAverageStruct(adc_average_config_t *const config);

/**
 * @brief Configures the Hardware Average feature with the given configuration structure.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 */
void ADC_DRV_ConfigHwAverage(const uint32_t instance, const adc_average_config_t *const config);

/**
 * @brief  Gets the current Hardware Average configuration.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 */
void ADC_DRV_GetHwAverageConfig(const uint32_t instance, adc_average_config_t *const config);

/** @}*/

/**
 * @name Channel configuration
 */
/** @{*/

/**
 * @brief Initializes the control channel configuration structure.
 *
 * @param  config The configuration structure.
 */
void ADC_DRV_InitChanStruct(adc_chan_config_t *const config);

/**
 * @brief Configures the selected control channel with the given configuration structure.
 *
 * @param  instance Instance number.
 * @param  chanIndex The control channel index.
 * @param  config The configuration structure.
 */
void ADC_DRV_ConfigChan(const uint32_t instance, const uint8_t chanIndex, const adc_chan_config_t *const config);

/**
 * @brief Gets the current control channel configuration for the selected channel index.
 *
 * @param  instance Instance number.
 * @param  chanIndex The control channel index.
 * @param  config  The configuration structure.
 */
void ADC_DRV_GetChanConfig(const uint32_t instance, const uint8_t chanIndex, adc_chan_config_t *const config);

/**
 * @brief This function sets the software pretrigger - affects only first 4 control channels.
 *
 * @param  instance Instance number.
 * @param  swPretrigger The swPretrigger to be enabled.
 */
void ADC_DRV_SetSwPretrigger(const uint32_t instance, const adc_sw_pretrigger_t swPretrigger);

/** @}*/

/**
 * @name Converter
 * Converter specific methods. These are used to configure and use the A/D
 * Converter specific functionality.
 */
/** @{*/

/**
 * @brief Resets the converter (sets all configurations to reset values).
 *
 * @param  instance Instance number.
 */
void ADC_DRV_Reset(const uint32_t instance);

/**
 * @brief Sets the ADC Open Flag state.
 *
 * @param  instance Instance number.
 * @param  state The new ADC Open Flag state.
 */
void ADC_DRV_SetOperationState(const uint32_t instance, bool state);

/**
 * @brief  Triggers a sw request for adc Conversion.
 *
 * @param  instance Instance number.
 */
void ADC_DRV_TriggerSwRequest(const uint32_t instance);

/**
 * @brief Waits for a conversion/calibration to finish.
 *
 * @param  instance Instance number.
 */
void ADC_DRV_WaitConvDone(const uint32_t instance);

/**
 * @brief  Gets the control channel Conversion Complete Flag state.
 *
 * @param  instance Instance number.
 * @param  chanIndex The adc control channel index.
 * @return true: Conversion Complete.
 * @return false: Conversion failed.
 */
bool ADC_DRV_GetConvCompleteFlag(const uint32_t instance, const uint8_t chanIndex);

/**
 * @brief Gets the last result for the selected control channel.
 *
 * @param  instance Instance number.
 * @param  chanIndex The converter control channel index.
 * @param  result The result raw value.
 */
void ADC_DRV_GetChanResult(const uint32_t instance, const uint8_t chanIndex, uint16_t *const result);

/**
 * @brief  Executes an Auto-Calibration.
 *
 * @param  instance Instance number.
 */
void ADC_DRV_AutoCalibration(const uint32_t instance);

/** @}*/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
/* end of group ADC_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif