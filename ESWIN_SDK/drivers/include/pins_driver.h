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
 * @file pins_driver.h
 * @brief PINS driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PINS_DRIVER_H__
#define __PINS_DRIVER_H__

#include "emps_platform.h"
#include "status.h"
#include <stddef.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup PINS_Driver PINS Driver
 * @{
 */

/**
 * @brief Type of a GPIO channel representation.
 */
typedef uint32_t pins_channel_type_t;

/**
 * @brief Type of a port levels representation.
 */
typedef uint8_t pins_level_type_t;

/**
 * @brief Configures the gpio dma channel.
 */
typedef enum {
    GPIO_DMA_CHANNEL_0 = 0x0U, /**< Gpio channel 0. */
    GPIO_DMA_CHANNEL_1 = 0x1U, /**< Gpio channel 1. */
    GPIO_DMA_CHANNEL_2 = 0x2U, /**< Gpio channel 2. */
    GPIO_DMA_CHANNEL_3 = 0x3U  /**< Gpio channel 3. */
} gpio_dma_channel_t;

/**
 * @brief Configures the port data direction.
 */
typedef enum {
    GPIO_INPUT_DIRECTION       = 0x0U, /**< General purpose input direction. */
    GPIO_OUTPUT_DIRECTION      = 0x1U, /**< General purpose output direction. */
    GPIO_UNSPECIFIED_DIRECTION = 0x2U  /**< General purpose unspecified direction. */
} port_data_direction_t;

/**
 * @brief Internal resistor pull feature selection.
 */
typedef enum {
    PORT_INTERNAL_PULL_NOT_ENABLED  = 0U, /**< Internal pull-down or pull-up resistor is not enabled. */
    PORT_INTERNAL_PULL_DOWN_ENABLED = 1U, /**< Internal pull-down resistor is enabled. */
    PORT_INTERNAL_PULL_UP_ENABLED   = 2U  /**< Internal pull-up resistor is enabled. */
} port_pull_config_t;

/**
 * @brief Configures the drive strength.
 */
typedef enum {
    PORT_STR0_DRIVE_STRENGTH = 0U, /**< Resistor is set to   3mA(VDDIO=5V). */
    PORT_STR1_DRIVE_STRENGTH = 1U, /**< Resistor is set to  10mA(VDDIO=5V). */
    PORT_STR2_DRIVE_STRENGTH = 2U, /**< Resistor is set to  15mA(VDDIO=5V). */
    PORT_STR3_DRIVE_STRENGTH = 3U, /**< Resistor is set to  20mA(VDDIO=5V). */
} port_drive_strength_t;

/**
 * @brief Configures the Pin mux selection.
 */
typedef enum {
    PORT_MUX_ALT0 = 0U, /**< Corresponding pin is disabled, but is used as an analog pin. */
    PORT_MUX_ALT1 = 1U, /**< Corresponding pin is configured as GPIO. */
    PORT_MUX_ALT2 = 2U, /**< Chip-specific. */
    PORT_MUX_ALT3 = 3U, /**< Chip-specific. */
    PORT_MUX_ALT4 = 4U, /**< Chip-specific. */
    PORT_MUX_ALT5 = 5U, /**< Chip-specific. */
    PORT_MUX_ALT6 = 6U, /**< Chip-specific. */
    PORT_MUX_ALT7 = 7U, /**< Chip-specific. */
} port_mux_t;

/**
 * @brief Configures the interrupt generation condition.
 */
typedef enum {
    PORT_INT_LOW_LEVEL    = 0x0U, /**< Interrupt when logic 0. */
    PORT_INT_HIGH_LEVEL   = 0x1U, /**< Interrupt when logic 1. */
    PORT_INT_RISING_EDGE  = 0x2U, /**< Interrupt on rising edge. */
    PORT_INT_FALLING_EDGE = 0x3U, /**< Interrupt on falling edge. */
    PORT_INT_EITHER_EDGE  = 0x4U, /**< Interrupt on either edge. */
    PORT_INT_DISABLED     = 0xFU, /**< Disable interrupt. */
} port_interrupt_config_t;

/**
 * @brief Defines the converter configuration.
 *
 * This structure is used to configure the pins.
 */
typedef struct {
    uint8_t base;                      /**< Port base number. \D{PORTA} */
    uint32_t pinPortIdx;               /**< Port pin number. \D{0U} */
    port_pull_config_t pullConfig;     /**< Internal resistor pull feature selection. \D{PORT_INTERNAL_PULL_NOT_ENABLED} */
    port_drive_strength_t driveSelect; /**< Configures the drive strength. \D{PORT_STR0_DRIVE_STRENGTH} */
    port_mux_t mux;                    /**< Pin (C55: Out) mux selection. \D{PORT_MUX_ALT1} */
    bool isGpio;                       /**< If config for GPIO. \D{false} */
    port_data_direction_t direction;   /**< Configures the port data direction. \D{GPIO_OUTPUT_DIRECTION} */
    pins_level_type_t initValue;       /**< Pin output initial value. \D{1U} */
    port_interrupt_config_t intConfig; /**< Interrupt generation condition. \D{PORT_INT_DISABLED} */
    bool clearIntFlag;                 /**< Clears the interrupt status flag or not. \D{false} */
    bool debounceEnable;               /**< Enable debounce or not. \D{false} */
} pin_settings_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initializes the pins with the given configuration structure.
 *
 * This function configures the pins with the options provided in the
 * provided structure.
 *
 * @param  pinCount The number of configured pins in structure.
 * @param  config The configuration structure.
 * @return status_t Status return codes.
 */
status_t PINS_DRV_Init(uint32_t pinCount, const pin_settings_config_t config[]);

/**
 * @brief Configures the internal resistor.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin PORT pin number [0-31].
 * @param  pullConfig The pull configuration.
 */
void PINS_DRV_SetPullSel(uint8_t port, uint32_t pin, port_pull_config_t pullConfig);

/**
 * @brief Configures the pin muxing.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin PORT pin number [0-31].
 * @param  mux Pin muxing slot selection.
 */
void PINS_DRV_SetMuxModeSel(uint8_t port, uint32_t pin, port_mux_t mux);

/** GPIO parts. */
/**
 * @brief Configures the port pin interrupt request.
 *
 * This function configures the port pin interrupt/PDMA request.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @param  intConfig The interrupt generation condition.
 */
void PINS_DRV_SetPinIntSel(uint8_t port, uint32_t pin, port_interrupt_config_t intConfig);

/**
 * @brief Clear the port pin interrupt request.
 *
 * This function configures the port pin interrupt/PDMA request.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
void PINS_DRV_ClearPinIntSel(uint8_t port, uint32_t pin);

/**
 * @brief Gets the current port pin interrupt request configuration.
 *
 * This function gets the current port pin interrupt/PDMA request configuration.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @return port_interrupt_config_t The interrupt generation condition.
 */
port_interrupt_config_t PINS_DRV_GetPinIntSel(const uint8_t port, uint32_t pin);

/**
 * @brief Enables debounce for interrupt pin.
 *
 * This function configures the port pin debounce.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
void PINS_DRV_EnablePinDebounce(uint8_t port, uint32_t pin);

/**
 * @brief Disables debounce for interrupt pin.
 *
 * This function clear the port pin debounce.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
void PINS_DRV_DisablePinDebounce(uint8_t port, uint32_t pin);

/**
 * @brief Clears the individual pin-interrupt status flag.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
void PINS_DRV_ClearPinIntFlagCmd(uint8_t port, uint32_t pin);

/**
 * @brief Writes the pin interrupt mask register.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @param  enable Enable/disable IMR of the pin.
 */
void PINS_DRV_WritePinIMRCmd(uint8_t port, uint32_t pin, bool enable);

/**
 * @brief Clears All Interrupt Mask Register.
 *
 */
void PINS_DRV_ClearPortIMRCmd(void);

/**
 * @brief Reads the entire port interrupt status flag.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @return uint32_t All 32 pin interrupt status flags.
 */
uint32_t PINS_DRV_GetPortIntFlag(const uint8_t port);

/**
 * @brief Clears the entire port interrupt status flag.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 */
void PINS_DRV_ClearPortIntFlagCmd(uint8_t port);

/**
 * @brief Get the pins directions configuration for a port.
 *
 * This function returns the current pins directions for a port. Pins
 * corresponding to bits with value of '1' are configured as output and
 * pins corresponding to bits with value of '0' are configured as input.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @return pins_channel_type_t GPIO directions. Each bit represents one pin (LSB is pin 0, MSB is
 *                             pin 31). For each bit:
 *                             - 0: corresponding pin is set to input.
 *                             - 1: corresponding pin is set to output.
 */
pins_channel_type_t PINS_DRV_GetPinsDirection(const uint8_t port);

/**
 * @brief Configure the direction for a certain pin from a port.
 *
 * This function configures the direction for the given pin, with the
 * given value('1' for pin to be configured as output and '0' for pin to
 * be configured as input).
 *
 * Note: With some platforms when you want to set a pin as output only and disable
 * input completely, it is required to call PINS_DRV_SetPortInputDisable if platform
 * has this feature.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @param  direction Direction of GPIO :
 *                   - 0: corresponding pin is set to input.
 *                   - 1: corresponding pin is set to output.
 */
void PINS_DRV_WritePinDirection(uint8_t port, pins_channel_type_t pin, pins_level_type_t direction);

/**
 * @brief Set the pins directions configuration for a port.
 *
 * This function sets the direction configuration for all pins
 * in a port. Pins corresponding to bits with value of '1' will be configured as output
 * and pins corresponding to bits with value of '0' will be not effected.
 *
 * Note: With some platforms when you want to set a pin as output only and disable
 * input completely, it is required to call PINS_DRV_SetPortInputDisable if platform
 * has this feature.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is not effected.
 *              - 1: corresponding pin is set to output.
 */
void PINS_DRV_SetPinsDirection(uint8_t port, pins_channel_type_t pins);

/**
 * @brief Write the pins directions configuration for a port.
 *
 * This function sets the direction configuration for all pins
 * in a port. Pins corresponding to bits with value of '1' will be configured as output
 * and pins corresponding to bits with value of '0' will be configured as input.
 *
 * Note: With some platforms when you want to set a pin as output only and disable
 * input completely, it is required to call PINS_DRV_SetPortInputDisable if platform
 * has this feature.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is set to input.
 *              - 1: corresponding pin is set to output.
 */
void PINS_DRV_WritePinsDirection(uint8_t port, pins_channel_type_t pins);

/**
 * @brief This function writes the given pin from a port, with the given value
 * ('0' represents LOW, '1' represents HIGH).
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @param  value Value of pin [0 or 1].
 */
void PINS_DRV_WritePin(uint8_t port, pins_channel_type_t pin, pins_level_type_t value);

/**
 * @brief This function writes all pins configured as output with the values given in
 * the parameter pins. '0' represents LOW, '1' represents HIGH.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is set to LOW.
 *              - 1: corresponding pin is set to HIGH.
 */
void PINS_DRV_WritePins(uint8_t port, pins_channel_type_t pins);

/**
 * @brief This function returns the current output that is written to a port. Only pins
 * that are configured as output will have meaningful values.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @return pins_channel_type_t Type of a GPIO channel representation.
 */
pins_channel_type_t PINS_DRV_GetPinsOutput(const uint8_t port);

/**
 * @brief This function configures output pins listed in parameter pins (bits that are
 * '1') to have a value of 'set' (HIGH). Pins corresponding to '0' will be unaffected.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is set to input.
 *              - 1: corresponding pin is set to output.
 */
void PINS_DRV_SetPins(uint8_t port, pins_channel_type_t pins);

/**
 * @brief This function configures output pins listed in parameter pins (bits that are
 * '1') to have a 'cleared' value (LOW). Pins corresponding to '0' will be unaffected.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is set to input.
 *              - 1: corresponding pin is set to output.
 */
void PINS_DRV_ClearPins(uint8_t port, pins_channel_type_t pins);

/**
 * @brief This function toggles output pin listed in parameter pin (bits that are
 * '1'). Pins corresponding to '0' will be unaffected.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
void PINS_DRV_TogglePin(uint8_t port, pins_channel_type_t pin);

/**
 * @brief This function toggles output pins listed in parameter pins (bits that are
 * '1'). Pins corresponding to '0' will be unaffected.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is set to input.
 *              - 1: corresponding pin is set to output.
 */
void PINS_DRV_TogglePins(uint8_t port, pins_channel_type_t pins);

/**
 * @brief This function returns the current input values from a port. Only pins
 * configured as input will have meaningful values.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @return pins_channel_type_t Type of a GPIO channel representation.
 */
pins_channel_type_t PINS_DRV_ReadPins(const uint8_t port);

/**
 * @brief This function configures base-pin's dma enable value and base's dma select value.
 *
 * @param  channel Configures the gpio dma channel.
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
void PINS_DRV_SetPinDma(gpio_dma_channel_t channel, uint8_t port, pins_channel_type_t pin);

/**
 * @brief This function clear all dma enable value and dma select value.
 *
 */
void PINS_DRV_ClearPinsDma(void);

/**
 * @brief This function configures port-pin's dma enable value.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
void PINS_DRV_SetPinDmaEn(uint8_t port, pins_channel_type_t pin);

/**
 * @brief This function clear port's all dma enable values.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 */
void PINS_DRV_ClearPinsDmaEn(uint8_t port);

/**
 * @brief This function set channel dma sel pin128's value.
 *
 * @param  channel Configures the gpio dma channel.
 * @param  pin128 Type of a GPIO channel representation.
 */
void PINS_DRV_SetChannelDmaSel(gpio_dma_channel_t channel, pins_channel_type_t pin128);

/**
 * @brief This function clear channel dma sel pin128's value.
 *
 * @param  channel Configures the gpio dma channel.
 */
void PINS_DRV_ClearChannelDmaSel(gpio_dma_channel_t channel);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group PINS_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __PINS_DRIVER_H__ */
