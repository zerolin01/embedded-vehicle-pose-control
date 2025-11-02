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
 * @file cmp_driver.h
 * @brief CMP driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CMP_DRIVER_H__
#define __CMP_DRIVER_H__

#include "emps_platform.h"
#include "status.h"

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup CMP_Driver CMP Driver
 * @{
 */

/**
 * @brief Power Modes selection.
 */
typedef enum {
    CMP_ULTRA_LOW_SPEED = 0U, /**< Module in  ultra-low speed mode (3uA). */
    CMP_LOW_SPEED       = 1U, /**< Module in low speed mode(10uA). */
    CMP_MEDIUM_SPEED    = 2U, /**< Module in medium speed mode(50uA). */
    CMP_HIGH_SPEED      = 3U, /**< Module in high speed mode(100uA). */
} cmp_power_mode_t;

/**
 * @brief Voltage Reference selection.
 */
typedef enum {
    CMP_VIN1 = 0U, /**< Use Vin1 as supply reference source for DAC. */
    CMP_VIN2 = 1U, /**< Use Vin2 as supply reference source for DAC. */
} cmp_voltage_reference_t;

/**
 * @brief Port Mux Source selection.
 */
typedef enum {
    CMP_DAC   = CMP_DAC_SOURCE,  /**< Select DAC as source for the comparator port. */
    CMP_MUX   = CMP_MUX_SOURCE,  /**< Select MUX8 as source for the comparator port. */
    CMP_1_2V  = CMP_1_2V_SOURCE, /**< Select 1.2V as source for the comparator port. */
    CMP_VREFB = CMP_VREFB_SOURCE, /**< Select VERFB as source for the comparator port. */
} cmp_port_mux_t;

/**
 * @brief Comparator output invert selection.
 */
typedef enum {
    CMP_NORMAL = 0U, /**< Output signal isn't inverted. */
    CMP_INVERT = 1U, /**< Output signal is inverted. */
} cmp_inverter_t;

/**
 * @brief Comparator output select selection.
 */
typedef enum {
    CMP_COUT  = 0U, /**< Select COUT as comparator output signal. */
    CMP_COUTA = 1U, /**< Select COUTA as comparator output signal. */
} cmp_output_select_t;

/**
 * @brief Comparator output pin enable selection.
 */
typedef enum {
    CMP_UNAVAILABLE = 0U, /**< Comparator output isn't available to a specific pin. */
    CMP_AVAILABLE   = 1U, /**< Comparator output is available to a specific pin. */
} cmp_output_enable_t;

/**
 * @brief Comparator hysteresis control.
 */
typedef enum {
    CMP_LEVEL_HYS_DISABLED = 0U,
    CMP_LEVEL_HYS_EN       = 1U,
} cmp_hysteresis_t;

/**
 * @brief Comparator Round-Robin fixed port.
 */
typedef enum {
    CMP_PLUS_FIXED  = 0U, /**< The Plus port is fixed. Only the inputs to the Minus port are swept in each round. */
    CMP_MINUS_FIXED = 1U, /**< The Minus port is fixed. Only the inputs to the Plus port are swept in each round. */
} cmp_fixed_port_t;

/**
 * @brief Comparator output interrupt configuration.
 */
typedef enum {
    CMP_NO_EVENT     = 0U, /**< Comparator output interrupts are disabled OR no event occurred. */
    CMP_FALLING_EDGE = 1U, /**< Comparator output interrupts will be generated only on falling edge OR only falling edge event occurred. */
    CMP_RISING_EDGE  = 2U, /**< Comparator output interrupts will be generated only on rising edge OR only rising edge event occurred. */
    CMP_BOTH_EDGES   = 3U, /**< Comparator output interrupts will be generated on both edges OR both edges event occurred. */
} cmp_output_trigger_t;

/**
 * @brief Comparator functional modes.
 */
typedef enum {
    CMP_DISABLED                   = 0U,
    CMP_CONTINUOUS                 = 1U,
    CMP_SAMPLED_NONFILTRED_INT_CLK = 2U,
    CMP_SAMPLED_NONFILTRED_EXT_CLK = 3U,
    CMP_SAMPLED_FILTRED_INT_CLK    = 4U,
    CMP_SAMPLED_FILTRED_EXT_CLK    = 5U,
    CMP_WINDOWED                   = 6U,
    CMP_WINDOWED_RESAMPLED         = 7U,
    CMP_WINDOWED_FILTRED           = 8U,
} cmp_mode_t;

/**
 * @brief Comparator channels list (1bit/channel).
 */
typedef uint8_t cmp_ch_list_t;

/**
 * @brief Number of channel.
 */
typedef uint8_t cmp_ch_number_t;

/**
 * @brief Definition for the CMP callback function.
 *
 * Prototype for the callback function registered in the PMU driver.
 */
typedef void (*cmp_callback_t)(void *parameter);

/**
 * @brief Data structure for the dac state.
 *
 * This structure is used when calling the IOPMP_DRV_Init function.
 */
typedef struct {
    uint32_t instance;       /**< instance. */
    void *callbackParam;     /**< callback parameters. */
    cmp_callback_t callback; /**< The callback of interrupt. */
} cmp_state_t;

/**
 * @brief Defines the block configuration.
 *
 * This structure is used to configure only comparator block module(filtering, sampling, power_mode etc.).
 */
typedef struct {
    bool dmaTriggerState;                        /**< True if PDMA transfer trigger from comparator is enable. \D{false} */
    cmp_output_trigger_t outputInterruptTrigger; /**< CMP_NO_INTERRUPT comparator output would not trigger any interrupt. */
                                                 /**< CMP_FALLING_EDGE comparator output would trigger an interrupt on falling edge.*/
                                                 /**< CMP_RISING_EDGE comparator output would trigger an interrupt on rising edge.*/
                                                 /**< CMP_BOTH_EDGES comparator output would trigger an interrupt on rising and falling edges. \D{CMP_BOTH_EDGES} */
    cmp_mode_t mode;                             /**< Configuration structure which define: the comparator functional mode, sample period and sample count. \D{CMP_CONTINUOUS} */
    uint8_t filterSamplePeriod;                  /**< Filter sample period. \D{0xF} */
    uint8_t filterSampleCount;                   /**< Number of sample count for filtering. \D{0} */
    cmp_power_mode_t powerMode;                  /**< CMP_LOW_SPEED if low speed mode is selected. */
                                                 /**< CMP_HIGH_SPEED if high speed mode is selected. \D{CMP_LOW_SPEED} */
    cmp_inverter_t inverterState;                /**< CMP_NORMAL if does not invert the comparator output. */
                                                 /**< CMP_INVERT if inverts the comparator output. \D{CMP_NORMAL} */
    cmp_output_enable_t pinState;                /**< CMP_UNAVAILABLE if comparator output is not available to package pin. */
                                                 /**< CMP_AVAILABLE if comparator output is available to package pin. \D{CMP_AVAILABLE} */
    cmp_output_select_t outputSelect;            /**< CMP_COUT if output signal is equal to COUT(filtered). */
                                                 /**< CMP_COUTA if output signal is equal to COUTA(unfiltered). \D{CMP_COUTA} */
    cmp_hysteresis_t hysteresisLevel;            /**< CMP_LEVEL_HYS_DISABLED if hard block output has level 0 hysteresis. */
                                                 /**< CMP_LEVEL_HYS_EN if hard block output has level 1 hysteresis. \D{CMP_LEVEL_HYS_DISABLED} */
} cmp_comparator_t;

/**
 * @brief Defines the analog mux.
 *
 * This structure is used to configure the analog multiplexor to select compared signals.
 */
typedef struct {
    cmp_port_mux_t positivePortMux; /**< Select positive port signal. */
                                    /**< CMP_DAC if source is digital to analog converter. */
                                    /**< CMP_MUX if source is 8 ch MUX. \D{CMP_MUX} */
    cmp_port_mux_t negativePortMux; /**< Select negative port signal. */
                                    /**< CMP_DAC if source is digital to analog converter. */
                                    /**< CMP_MUX if source is 8 ch MUX. \D{CMP_MUX} */

    cmp_ch_number_t positiveInputMux; /**< Select which channel is selected for the plus mux. \D{0} */
    cmp_ch_number_t negativeInputMux; /**< Select which channel is selected for the minus mux. \D{1} */
} cmp_anmux_t;

/**
 * @brief Defines the trigger mode.
 *
 * This structure is used to configure the trigger mode operation when MCU enters STOP modes.
 */
typedef struct {
    bool roundRobinState;                  /**< True if Round-Robin is enabled. \D{false} */
    bool roundRobinInterruptState;         /**< True if Round-Robin interrupt is enabled. \D{false} */
    cmp_fixed_port_t fixedPort;            /**< CMP_PLUS_FIXED if plus port is fixed. */
                                           /**< CMP_MINUS_FIXED if minus port is fixed. \D{CMP_PLUS_FIXED} */
    cmp_ch_number_t fixedChannel;          /**< Select which channel would be assigned to the fixed port. \D{0} */
    uint8_t samples;                       /**< Select number of round-robin clock cycles for a given channel. \D{0} */
    uint16_t initializationDelay;           /**< Select dac and comparator initialization delay(clock cycles). \D{0} */
    cmp_ch_list_t roundRobinChannelsState; /**< One bite for each channel state. | CH7_state | CH6_state | ..... | CH1_state | CH0_state | \D{0} */
    cmp_ch_list_t programedState;          /**< Pre-programmed state for comparison result. \D{0} */
} cmp_trigger_mode_t;

/**
 * @brief Defines the comparator module configuration.
 *
 * This structure is used to configure all components of comparator module.
 */
typedef struct {
    cmp_comparator_t comparator;    /**< CMP comparator block module configure */
    cmp_anmux_t mux;                /**< CMP analog multiplexor configure */
    cmp_trigger_mode_t triggerMode; /**< CMP trigger mode configure */
} cmp_module_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Reset all registers.
 *
 * This function set all CMP registers to reset values.
 *
 * @param  instance Instance number.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_Reset(const uint32_t instance);

/**
 * @brief  Enable or Disable comparator module.
 *
 * This function enable or disable comparator module.
 *
 * @param  instance Instance number.
 * @param  enable Cmp enable state.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_SetEnableState(const uint32_t instance, bool enable);

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
status_t CMP_DRV_Init(const uint32_t instance, const cmp_module_t *const config, cmp_state_t *state);

/**
 * @brief Gets the current comparator configuration.
 *
 * This function returns the current configuration for comparator as a configuration structure.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_GetConfigAll(const uint32_t instance, cmp_module_t *const config);

/**
 * @brief Configure only the MUX component.
 *
 * This function configures the MUX with the options provided in the config structure.
 *
 * @param  instance Instance number.
 * @param  config  The configuration structure.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_ConfigMUX(const uint32_t instance, const cmp_anmux_t *config);

/**
 * @brief  Return configuration only for the MUX component.
 *
 * This function returns current configuration to determine which signals go to comparator ports.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_GetMUXConfig(const uint32_t instance, cmp_anmux_t *const config);

/**
 * @brief Configure trigger mode.
 *
 * This function configures the trigger mode with the options provided in the config structure.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_ConfigTriggerMode(const uint32_t instance, const cmp_trigger_mode_t *config);

/**
 * @brief Get current trigger mode configuration.
 *
 * This function returns the current trigger mode configuration for trigger mode.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_GetTriggerModeConfig(const uint32_t instance, cmp_trigger_mode_t *const config);

/**
 * @brief  Get comparator output flags.
 *
 * This function returns in flags comparator output flags(rising and falling edge).
 *
 * @param  instance Instance number.
 * @param  flags Pointer to output flags:
 *               NO_EVENT
 *               RISING_EDGE
 *               FALLING_EDGE
 *               BOTH_EDGE
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_GetOutputFlags(const uint32_t instance, cmp_output_trigger_t *flags);

/**
 * @brief Clear comparator output flags.
 *
 * This function clear comparator output flags(rising and falling edge).
 *
 * @param  instance Instance number.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_ClearOutputFlags(const uint32_t instance);

/**
 * @brief Gets input channels change flags.
 *
 * This function return in flags all input channels flags as uint8_t(1 bite
 * for each channel flag).
 *
 * @param  instance Instance number.
 * @param  flags Pointer to input flags.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_GetInputFlags(const uint32_t instance, cmp_ch_list_t *flags);

/**
 * @brief Clear comparator input channels flags.
 *
 * This function clear comparator input channels flags.
 *
 * @param  instance Instance number.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_ClearInputFlags(const uint32_t instance);

/**
 * @brief Configure only comparator features.
 *
 * This function configure only features related with comparator:
 * PDMA request, power mode, output select, interrupts enable, invert,
 * offset, hysteresis.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_ConfigComparator(const uint32_t instance, const cmp_comparator_t *config);

/**
 * @brief Return configuration for comparator from CMP module.
 *
 * This function return configuration for features related with comparator:
 * PDMA request, power mode, output select, interrupts enable, invert, offset, hysteresis.
 *
 * @param  instance Instance number.
 * @param  config The configuration structure.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t CMP_DRV_GetComparatorConfig(const uint32_t instance, cmp_comparator_t *config);

/**
 * @brief Set can enter/exit sleep mode according TCSR.
 *
 * @param instance The CMP instance number.
 * @param en Enter/exit sleep mode according TCSR.
 */
void CMP_DRV_SetSleepModeState(const uint32_t instance, bool enable);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group CMP_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif
