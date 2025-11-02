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
 * @file pins_hw_access.h
 * @brief PINS driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PINS_HW_ACCESS_H__
#define __PINS_HW_ACCESS_H__

#include "pins_driver.h"
#include "pins_register.h"
#include "status.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initializes the pins with the given configuration structure.
 *
 * This function configures the pins with the options provided in the given structure.
 *
 * @param  config Defines the converter configuration.
 */
void PINS_Init(const pin_settings_config_t *config);

/**
 * @brief Get pinmux port control register value.
 * 
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin PORT pin number [0-31].
 * @return uint32_t Pinmux port control register value.
 */
static inline uint32_t PINS_GetPortControlRegister(uint8_t port, uint32_t pin)
{
    OS_ASSERT(port < PINMUX_PORT_COUNT);
    switch (port) {
    case PORTA:
        return ((uint32_t)((pins_type_t *)PINMUX_BASE)->PORTA_PCR[pin]);
        break;
    case PORTB:
        return ((uint32_t)((pins_type_t *)PINMUX_BASE)->PORTB_PCR[pin]);
        break;
    case PORTC:
        return ((uint32_t)((pins_type_t *)PINMUX_BASE)->PORTC_PCR[pin]);
        break;
    case PORTD:
        return ((uint32_t)((pins_type_t *)PINMUX_BASE)->PORTD_PCR[pin]);
        break;
    default:
        return 0U;
        break;
    }
}

/**
 * @brief Set pinmux port control register value.
 * 
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin PORT pin number [0-31].
 * @param  value Pinmux port control register value.
 */
static inline void PINS_SetPortControlRegister(uint8_t port, uint32_t pin, uint32_t value)
{
    OS_ASSERT(port < PINMUX_PORT_COUNT);
    switch (port) {
    case PORTA:
        (((pins_type_t *)PINMUX_BASE)->PORTA_PCR[pin]) = value;
        break;
    case PORTB:
        (((pins_type_t *)PINMUX_BASE)->PORTB_PCR[pin]) = value;
        break;
    case PORTC:
        (((pins_type_t *)PINMUX_BASE)->PORTC_PCR[pin]) = value;
        break;
    case PORTD:
        (((pins_type_t *)PINMUX_BASE)->PORTD_PCR[pin]) = value;
        break;
    default:
        break;
    }
}

/**
 * @brief Configures the internal resistor.
 *
 * Pull configuration is valid in all digital pin muxing modes.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin PORT pin number [0-31].
 * @param  pullConfig Internal resistor pull feature selection:
 *                    - PORT_PULL_NOT_ENABLED: internal pull-down or pull-up resistor is not enabled.
 *                    - PORT_PULL_DOWN_ENABLED: internal pull-down resistor is enabled.
 *                    - PORT_PULL_UP_ENABLED: internal pull-up resistor is enabled.
 */
void PINS_SetPullSel(uint8_t port, uint32_t pin, port_pull_config_t pullConfig);

/**
 * @brief Configures the pin muxing.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin PORT pin number [0-31].
 * @param  mux Pin muxing slot selection:
 *             - PORT_PIN_DISABLED: Pin disabled.
 *             - PORT_MUX_AS_GPIO : Set as GPIO.
 *             - PORT_MUX_ADC_INTERLEAVE : For ADC interleaved
 *             - others           : chip-specific.
 */
void PINS_SetMuxModeSel(uint8_t port, uint32_t pin, port_mux_t mux);

/**
 * @brief Configures the port pin interrupt request.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @param  intConfig Interrupt configuration:
 *                   - PORT_INT_LOW_LEVEL    : Interrupt when logic zero.
 *                   - PORT_INT_HIGH_LEVEL   : Interrupt when logic one.
 *                   - PORT_INT_RISING_EDGE  : Interrupt on rising edge.
 *                   - PORT_INT_FALLING_EDGE : Interrupt on falling edge.
 *                   - PORT_INT_EITHER_EDGE  : Interrupt on either edge.
 *                   - PORT_INT_DISABLED     : Interrupt disabled.
 */
static inline void PINS_SetPinIntSel(uint8_t port, uint32_t pin, port_interrupt_config_t intConfig)
{
    OS_ASSERT(pin < PINMUX_PORT_PCR_COUNT);
    uint32_t regValue = 0;
    regValue          = GPIO_PIN_VALUE_GET(port, EDGE_SEL);
    regValue &= (uint32_t)(~((uint32_t)1U << pin));
    GPIO_PIN_VALUE_SET(port, EDGE_SEL) = GPIO_EDGE_SEL_ES(regValue);
    switch (intConfig) {
    case PORT_INT_EITHER_EDGE:
        regValue = GPIO_PIN_VALUE_GET(port, EDGE_SEL);
        regValue |= (uint32_t)((uint32_t)1 << pin);
        GPIO_PIN_VALUE_SET(port, EDGE_SEL) = GPIO_EDGE_SEL_ES(regValue);
        break;
    case PORT_INT_LOW_LEVEL:
    case PORT_INT_HIGH_LEVEL:
    case PORT_INT_RISING_EDGE:
    case PORT_INT_FALLING_EDGE:
        if (pin < 16) {
            regValue = GPIO_ICR1_VALUE_GET(port);
            regValue &= (uint32_t)(~((uint32_t)3U << (pin << 1)));
            regValue |= (uint32_t)((uint32_t)intConfig << (pin << 1));
            GPIO_ICR1_VALUE_SET(port) = GPIO_ICR1_IC(regValue);
        } else {
            regValue = GPIO_ICR2_VALUE_GET(port);
            regValue &= (uint32_t)(~((uint32_t)3U << ((pin - 16) << 1)));
            regValue |= (uint32_t)((uint32_t)intConfig << ((pin - 16) << 1));
            GPIO_ICR2_VALUE_SET(port) = GPIO_ICR2_IC(regValue);
        }
        break;
    case PORT_INT_DISABLED:
        return;
    default:
        /* Nothing to configure. */
        OS_ASSERT(false);
        break;
    }

    regValue = GPIO_PIN_VALUE_GET(port, IMR);
    regValue |= (uint32_t)((uint32_t)1U << pin);
    GPIO_PIN_VALUE_SET(port, IMR) = GPIO_IMR_IM(regValue);
}

/**
 * @brief Clear the port pin Edge Select.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
static inline void PINS_ClearPinEdgeSel(uint8_t port, uint32_t pin)
{
    OS_ASSERT(pin < PINMUX_PORT_PCR_COUNT);
    uint32_t regValue = GPIO_PIN_VALUE_GET(port, EDGE_SEL);
    regValue &= (uint32_t)(~((uint32_t)1U << pin));
    GPIO_PIN_VALUE_SET(port, EDGE_SEL) = GPIO_EDGE_SEL_ES(regValue);
}

/**
 * @brief Clear the port pin interrupt request.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
static inline void PINS_ClearPinIntSel(uint8_t port, uint32_t pin)
{
    OS_ASSERT(pin < PINMUX_PORT_PCR_COUNT);
    uint32_t regValue = 0;
    PINS_ClearPinEdgeSel(port, pin);
    if (pin < 16) {
        regValue = GPIO_ICR1_VALUE_GET(port);
        regValue &= (uint32_t)(~((uint32_t)3U << (pin << 1)));
        GPIO_ICR1_VALUE_SET(port) = GPIO_ICR1_IC(regValue);
    } else {
        regValue = GPIO_ICR2_VALUE_GET(port);
        regValue &= (uint32_t)(~((uint32_t)3U << ((pin - 16) << 1)));
        GPIO_ICR2_VALUE_SET(port) = GPIO_ICR2_IC(regValue);
    }
    regValue = GPIO_PIN_VALUE_GET(port, IMR);
    regValue &= (uint32_t)(~((uint32_t)1U << pin));
    GPIO_PIN_VALUE_SET(port, IMR) = GPIO_IMR_IM(regValue);
}

/**
 * @brief Gets the current port pin interrupt request configuration.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @return port_interrupt_config_t Interrupt configuration:
 *                                 - PORT_INT_LOW_LEVEL    : Interrupt when logic zero.
 *                                 - PORT_INT_HIGH_LEVEL   : Interrupt when logic one.
 *                                 - PORT_INT_RISING_EDGE  : Interrupt on rising edge.
 *                                 - PORT_INT_FALLING_EDGE : Interrupt on falling edge.
 *                                 - PORT_INT_EITHER_EDGE  : Interrupt on either edge.
 *                                 - PORT_INT_DISABLED     : Interrupt disabled.
 */
static inline port_interrupt_config_t PINS_GetPinIntSel(const uint8_t port, uint32_t pin)
{
    OS_ASSERT(pin < PINMUX_PORT_PCR_COUNT);
    uint32_t regValue = GPIO_PIN_VALUE_GET(port, EDGE_SEL);
    if (regValue & (1 << pin)) {
        return (port_interrupt_config_t)PORT_INT_EITHER_EDGE;
    }

    if (pin < 16) {
        regValue = GPIO_ICR1_VALUE_GET(port);
        regValue &= (uint32_t)(~((uint32_t)3U << (pin << 1)));
        regValue = regValue >> (pin << 1);
    } else {
        pin -= 16;
        regValue = GPIO_ICR2_VALUE_GET(port);
        regValue &= (uint32_t)(~((uint32_t)3U << (pin << 1)));
        regValue = regValue >> (pin << 1);
    }

    return (port_interrupt_config_t)regValue;
}

/**
 * @brief Enables the port pin interrupt debounce.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
static inline void PINS_EnablePinDebounce(uint8_t port, uint32_t pin)
{
    uint32_t regValue = GPIO_PIN_VALUE_GET(port, DEB_SEL);
    regValue |= (uint32_t)((uint32_t)1U << pin);
    GPIO_PIN_VALUE_SET(port, DEB_SEL) = GPIO_DEB_SEL_DS(regValue);
}

/**
 * @brief Disables the port pin interrupt debounce.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
static inline void PINS_DisablePinDebounce(uint8_t port, uint32_t pin)
{
    uint32_t regValue = GPIO_PIN_VALUE_GET(port, DEB_SEL);
    regValue &= (uint32_t)(~((uint32_t)1U << pin));
    GPIO_PIN_VALUE_SET(port, DEB_SEL) = GPIO_DEB_SEL_DS(regValue);
}

/**
 * @brief Clears the individual pin-interrupt status flag.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
static inline void PINS_ClearPinIntFlagCmd(uint8_t port, uint32_t pin)
{
    OS_ASSERT(pin < PINMUX_PORT_PCR_COUNT);
    GPIO_PIN_VALUE_SET(port, ISR) = GPIO_ISR_IS(1U << pin);
}

/**
 * @brief Writes the Pin Interrupt Mask Register.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @param  enable Enable/disable IMR of the pin.
 */
static inline void PINS_WritePinIMRCmd(uint8_t port, uint32_t pin, bool enable)
{
    OS_ASSERT(pin < PINMUX_PORT_PCR_COUNT);
    uint32_t regValue = 0;
    regValue          = GPIO_PIN_VALUE_GET(port, IMR);
    regValue |= (uint32_t)((uint32_t)enable << pin);
    GPIO_PIN_VALUE_SET(port, IMR) = GPIO_IMR_IM(regValue);
}

/**
 * @brief Clears All Interrupt Mask Register.
 *
 */
static inline void PINS_ClearPortIMRCmd(void)
{
    GPIO_PIN_VALUE_SET(PORTA, IMR) = ~GPIO_IMR_IM_MASK;
    GPIO_PIN_VALUE_SET(PORTB, IMR) = ~GPIO_IMR_IM_MASK;
    GPIO_PIN_VALUE_SET(PORTC, IMR) = ~GPIO_IMR_IM_MASK;
    GPIO_PIN_VALUE_SET(PORTD, IMR) = ~GPIO_IMR_IM_MASK;
}

/**
 * @brief Reads the entire port interrupt status flag.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @return uint32_t All 32 pin interrupt status flags. For specific bit:
 *                  - 0: interrupt is not detected.
 *                  - 1: interrupt is detected.
 */
static inline uint32_t PINS_GetPortIntFlag(const uint8_t port)
{
    uint32_t regValue = GPIO_PIN_VALUE_GET(port, ISR);
    return regValue;
}

/**
 * @brief Clears the entire port interrupt status flag.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 */
static inline void PINS_ClearPortIntFlagCmd(uint8_t port)
{
    GPIO_PIN_VALUE_SET(port, ISR) = GPIO_ISR_IS_MASK;
}

/**
 * @brief Get the pins directions configuration for a port.
 *
 * This function returns the current pins directions for a port. Pins
 * corresponding to bits with value of '1' are configured as output and
 * pins corresponding to bits with value of '0' are configured as input.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @return uint32_t GPIO directions. Each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *                  - 0: corresponding pin is set to input.
 *                  - 1: corresponding pin is set to output.
 */
static inline uint32_t PINS_GPIOGetPinsDirection(const uint8_t port)
{
    return GPIO_PIN_VALUE_GET(port, DIR);
}

/**
 * @brief Configure the direction for a certain pin from a port.
 *
 * This function configures the direction for the given pin, with the given value
 * ('1' for pin to be configured as output and '0' for pin to be configured as input).
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @param  direction Direction of GPIO :
 *                   - 0: corresponding pin is set to input.
 *                   - 1: corresponding pin is set to output.
 */
static inline void PINS_GPIOWritePinDirection(uint8_t port, uint32_t pin, pins_level_type_t direction)
{
    uint32_t pinsDirections = GPIO_PIN_VALUE_GET(port, DIR);
    pinsDirections &= (uint32_t)(~((uint32_t)1U << pin));
    pinsDirections |= (uint32_t)((uint32_t)direction << pin);
    GPIO_PIN_VALUE_SET(port, DIR) = GPIO_DIR(pinsDirections);
}

/**
 * @brief Set the pins directions configuration for a port.
 *
 * This function sets the direction configuration for all pins in a port.
 * Pins corresponding to bits with value of '1' will be configured as output
 * and pins corresponding to bits with value of '0' will not effected.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is not effected.
 *              - 1: corresponding pin is set to output.
 */
static inline void PINS_GPIOSetPinsDirection(uint8_t port, uint32_t pins)
{
    uint32_t pinsDirections = GPIO_PIN_VALUE_GET(port, DIR);
    pinsDirections |= (uint32_t)(pins);
    GPIO_PIN_VALUE_SET(port, DIR) = GPIO_DIR(pinsDirections);
}

/**
 * @brief Write the pins directions configuration for a port.
 *
 * This function sets the direction configuration for all pins in a port.
 * Pins corresponding to bits with value of '1' will be configured as output
 * and pins corresponding to bits with value of '0' will be configured as input.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is set to input.
 *              - 1: corresponding pin is set to output.
 */
static inline void PINS_GPIOWritePinsDirection(uint8_t port, uint32_t pins)
{
    GPIO_PIN_VALUE_SET(port, DIR) = GPIO_DIR(pins);
}

/**
 * @brief Write a pin of a port with a given value.
 *
 * This function writes the given pin from a port, with the given value
 * ('0' represents LOW, '1' represents HIGH).
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 * @param  value Pin value to be written:
 *               - 0: corresponding pin is set to LOW.
 *               - 1: corresponding pin is set to HIGH.
 */
static inline void PINS_GPIOWritePin(uint8_t port, uint32_t pin, pins_level_type_t value)
{
    uint32_t pinsValues = GPIO_PIN_VALUE_GET(port, DR);
    pinsValues &= (uint32_t)(~((uint32_t)1U << pin));
    pinsValues |= (uint32_t)((uint32_t)value << pin);
    GPIO_PIN_VALUE_SET(port, DR) = GPIO_DR(pinsValues);
}

/**
 * @brief Write all pins of a port.
 *
 * This function writes all pins configured as output with the values given in
 * the parameter pins. '0' represents LOW, '1' represents HIGH.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is set to LOW.
 *              - 1: corresponding pin is set to HIGH.
 */
static inline void PINS_GPIOWritePins(uint8_t port, uint32_t pins)
{
    GPIO_PIN_VALUE_SET(port, DR) = GPIO_DR(pins);
}

/**
 * @brief Get the current output from a port.
 *
 * This function returns the current output that is written to a port. Only pins
 * that are configured as output will have meaningful values.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @return uint32_t GPIO outputs. Each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *                  - 0: corresponding pin is set to LOW.
 *                  - 1: corresponding pin is set to HIGH.
 */
static inline uint32_t PINS_GPIOGetPinsOutput(const uint8_t port)
{
    return GPIO_PIN_VALUE_GET(port, DR);
}

/**
 * @brief Write pins with 'Set' value.
 *
 * This function configures output pins listed in parameter pins (bits that are '1') to have a value of 'set' (HIGH).
 * Pins corresponding to '0' will be unaffected.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is not effected.
 *              - 1: corresponding pin is set to HIGH.
 */
static inline void PINS_GPIOSetPins(uint8_t port, uint32_t pins)
{
    uint32_t pinsValues = GPIO_PIN_VALUE_GET(port, DR);
    pinsValues |= (uint32_t)(pins);
    GPIO_PIN_VALUE_SET(port, DR) = GPIO_DR(pinsValues);
}

/**
 * @brief Write pins to 'Clear' value.
 *
 * This function configures output pins listed in parameter pins (bits that are '1') to have a 'cleared' value (LOW).
 * Pins corresponding to '0' will be unaffected.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is not set to clear.
 *              - 1: corresponding pin is set to clear.
 */
static inline void PINS_GPIOClearPins(uint8_t port, uint32_t pins)
{
    uint32_t pinsValues = GPIO_PIN_VALUE_GET(port, DR);
    pinsValues &= (uint32_t)(~(pins));
    GPIO_PIN_VALUE_SET(port, DR) = GPIO_DR(pinsValues);
}

/**
 * @brief Toggle pin value.
 *
 * This function toggles output pin listed in parameter pin (bits that are '1').
 * Pins corresponding to '0' will be unaffected.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
static inline void PINS_GPIOTogglePin(uint8_t port, uint32_t pin)
{
    uint32_t pinsValues = GPIO_PIN_VALUE_GET(port, DR);
    pinsValues ^= 1U << pin;
    GPIO_PIN_VALUE_SET(port, DR) = GPIO_DR(pinsValues);
}

/**
 * @brief Toggle pins value.
 *
 * This function toggles output pins listed in parameter pins (bits that are '1').
 * Pins corresponding to '0' will be unaffected.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pins Pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *              - 0: corresponding pin is not set to toggle.
 *              - 1: corresponding pin is set to toggle.
 */
static inline void PINS_GPIOTogglePins(uint8_t port, uint32_t pins)
{
    uint32_t pinsValues = GPIO_PIN_VALUE_GET(port, DR);
    pinsValues ^= pins;
    GPIO_PIN_VALUE_SET(port, DR) = GPIO_DR(pinsValues);
}

/**
 * @brief Read input pins.
 *
 * This function returns the current input values from a port. Only pins
 * configured as input will have meaningful values.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @return uint32_t GPIO inputs. Each bit represents one pin (LSB is pin 0, MSB is pin 31). For each bit:
 *                  - 0: corresponding pin is read as LOW.
 *                  - 1: corresponding pin is read as HIGH.
 */
static inline uint32_t PINS_GPIOReadPins(const uint8_t port)
{
    return GPIO_PIN_VALUE_GET(port, PSR);
}

/**
 * @brief Configure the dma enable value.
 *
 * This function configures port-pin's dma enable value.
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 * @param  pin GPIO pin number [0-31].
 */
static inline void PINS_GPIOSetPinDmaEn(uint8_t port, uint32_t pin)
{
    uint32_t pinsDmaEn = GPIO_PIN_VALUE_GET(port, DMA_EN);
    pinsDmaEn |= (uint32_t)((uint32_t)1U << pin);
    GPIO_PIN_VALUE_SET(port, DMA_EN) = GPIO_DIR(pinsDmaEn);
}

/**
 * @brief Clear dma enable value.
 *
 * This function clear port's all dma enable values
 *
 * @param  port Pinmux port offset [PORTA, PORTB, PORTC, PORTD].
 */
static inline void PINS_GPIOClearPinsDmaEn(uint8_t port)
{
    GPIO_PIN_VALUE_SET(port, DMA_EN) = 0U;
}

/**
 * @brief Set dma select value.
 *
 * This function set channel's dma sel value.
 *
 * @param  channel Configures the gpio dma channel.
 * @param  pin GPIO pin number [0-31].
 */
static inline void PINS_GPIOSetChannelDmaSel(gpio_dma_channel_t const channel, uint32_t pin)
{
    GPIO_DMA_SEL_CHANNEL_SET(channel) = GPIO_DMA_SEL(pin);
}

/**
 * @brief Clear dma select value.
 *
 * This function clear channel's dma select value.
 *
 * @param  channel Configures the gpio dma channel.
 */
static inline void PINS_GPIOClearChannelDmaSel(gpio_dma_channel_t const channel)
{
    GPIO_DMA_SEL_CHANNEL_SET(channel) = 0U;
}

#if defined(__cplusplus)
}
#endif

#endif /* __PINS_HW_ACCESS_H__ */
