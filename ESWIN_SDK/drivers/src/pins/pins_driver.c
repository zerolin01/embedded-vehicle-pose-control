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
 * @file pins_driver.c
 * @brief PINS driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pins_driver.h"
#include "emps_platform.h"
#include "pins_hw_access.h"

/**
 *
 * Function Name : PINS_DRV_Init
 * Description   : This function configures the pins with the options provided in the given structure.
 *
 */
status_t PINS_DRV_Init(uint32_t pinCount, const pin_settings_config_t config[])
{
    uint32_t i;
    for (i = 0U; i < pinCount; i++) {
        PINS_Init(&config[i]);
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : PINS_DRV_SetPullSel
 * Description   : This function configures the internal resistor.
 *
 */
void PINS_DRV_SetPullSel(uint8_t port, uint32_t pin, port_pull_config_t pullConfig)
{
    PINS_SetPullSel(port, pin, pullConfig);
}

/**
 *
 * Function Name : PINS_DRV_SetMuxModeSel
 * Description   : This function configures the pin muxing.
 *
 */
void PINS_DRV_SetMuxModeSel(uint8_t port, uint32_t pin, port_mux_t mux)
{
    PINS_SetMuxModeSel(port, pin, mux);
}

/** GPIO parts. */
/**
 *
 * Function Name : PINS_DRV_SetPinIntSel
 * Description   : This function configures the port pin interrupt/PDMA request.
 *
 */
void PINS_DRV_SetPinIntSel(uint8_t port, uint32_t pin, port_interrupt_config_t intConfig)
{
    PINS_SetPinIntSel(port, pin, intConfig);
}

/**
 *
 * Function Name : PINS_DRV_PINS_ClearPinEdgeSel
 * Description   : This function Clear the port pin Edge Select.
 *
 */
void PINS_DRV_PINS_ClearPinEdgeSel(uint8_t port, uint32_t pin)
{
    PINS_ClearPinEdgeSel(port, pin);
}

/**
 *
 * Function Name : PINS_DRV_ClearPinIntSel
 * Description   : This function Clear the port pin interrupt request.
 *
 */
void PINS_DRV_ClearPinIntSel(uint8_t port, uint32_t pin)
{
    PINS_ClearPinIntSel(port, pin);
}

/**
 *
 * Function Name : PINS_DRV_GetPinIntSel
 * Description   : This function gets the current port pin interrupt/PDMA request configuration.
 *
 */
port_interrupt_config_t PINS_DRV_GetPinIntSel(const uint8_t port, uint32_t pin)
{
    return PINS_GetPinIntSel(port, pin);
}

/**
 *
 * Function Name : PINS_DRV_EnablePinDebounce
 * Description   : This function enables the port pin debounce.
 *
 */
void PINS_DRV_EnablePinDebounce(uint8_t port, uint32_t pin)
{
    PINS_EnablePinDebounce(port, pin);
}

/**
 *
 * Function Name : PINS_DRV_DisablePinDebounce
 * Description   : This function disables the port pin debounce.
 *
 */
void PINS_DRV_DisablePinDebounce(uint8_t port, uint32_t pin)
{
    PINS_DisablePinDebounce(port, pin);
}

/**
 *
 * Function Name : PINS_DRV_ClearPinIntFlagCmd
 * Description   : This function clears the individual pin-interrupt status flag.
 *
 */
void PINS_DRV_ClearPinIntFlagCmd(uint8_t port, uint32_t pin)
{
    PINS_ClearPinIntFlagCmd(port, pin);
}

/**
 *
 * Function Name : PINS_DRV_WritePinIMRCmd
 * Description   : This function writes the pin interrupt mask register.
 *
 */
void PINS_DRV_WritePinIMRCmd(uint8_t port, uint32_t pin, bool enable)
{
    PINS_WritePinIMRCmd(port, pin, enable);
}

/**
 *
 * Function Name : PINS_DRV_ClearPortIMRCmd
 * Description   : This function clears all interrupt mask register.
 *
 */
void PINS_DRV_ClearPortIMRCmd(void)
{
    PINS_ClearPortIMRCmd();
}

/**
 *
 * Function Name : PINS_DRV_GetPortIntFlag
 * Description   : This function reads the entire port interrupt status flag.
 *
 */
uint32_t PINS_DRV_GetPortIntFlag(const uint8_t port)
{
    return PINS_GetPortIntFlag(port);
}

/**
 *
 * Function Name : PINS_DRV_ClearPortIntFlagCmd
 * Description   : This function clears the entire port interrupt status flag.
 *
 */
void PINS_DRV_ClearPortIntFlagCmd(uint8_t port)
{
    PINS_ClearPortIntFlagCmd(port);
}

/**
 *
 * Function Name : PINS_DRV_GetPinsDirection
 * Description   : This function returns the current pins directions for a port. Pins
 * corresponding to bits with value of '1' are configured as output and
 * pins corresponding to bits with value of '0' are configured as input.
 *
 */
pins_channel_type_t PINS_DRV_GetPinsDirection(const uint8_t port)
{
    return PINS_GPIOGetPinsDirection(port);
}

/**
 *
 * Function Name : PINS_DRV_WritePinDirection
 * Description   : This function configures the direction for the given pin, with the
 * given value('1' for pin to be configured as output and '0' for pin to
 * be configured as input).
 *
 */
void PINS_DRV_WritePinDirection(uint8_t port, pins_channel_type_t pin, pins_level_type_t direction)
{
    PINS_GPIOWritePinDirection(port, pin, direction);
}

/**
 *
 * Function Name : PINS_DRV_SetPinsDirection
 * Description   : This function sets the direction configuration for all pins in a port.
 * Pins corresponding to bits with value of '1' will be configured as output
 * and pins corresponding to bits with value of '0' will be not effected.
 *
 */
void PINS_DRV_SetPinsDirection(uint8_t port, pins_channel_type_t pins)
{
    PINS_GPIOSetPinsDirection(port, pins);
}

/**
 *
 * Function Name : PINS_DRV_WritePinsDirection
 * Description   : This function sets the direction configuration for all pins in a port.
 * Pins corresponding to bits with value of '1' will be configured as output
 * and pins corresponding to bits with value of '0' will be configured as input.
 *
 */
void PINS_DRV_WritePinsDirection(uint8_t port, pins_channel_type_t pins)
{
    PINS_GPIOWritePinsDirection(port, pins);
}

/**
 *
 * Function Name : PINS_DRV_WritePin
 * Description   : This function writes the given pin from a port, with the given value
 * ('0' represents LOW, '1' represents HIGH).
 *
 */
void PINS_DRV_WritePin(uint8_t port, pins_channel_type_t pin, pins_level_type_t value)
{
    PINS_GPIOWritePin(port, pin, value);
}

/**
 *
 * Function Name : PINS_DRV_WritePins
 * Description   : This function writes all pins configured as output with the values given in
 * the parameter pins. '0' represents LOW, '1' represents HIGH.
 *
 */
void PINS_DRV_WritePins(uint8_t port, pins_channel_type_t pins)
{
    PINS_GPIOWritePins(port, pins);
}

/**
 *
 * Function Name : PINS_DRV_GetPinsOutput
 * Description   : This function returns the current output that is written to a port. Only pins
 * that are configured as output will have meaningful values.
 *
 */
pins_channel_type_t PINS_DRV_GetPinsOutput(const uint8_t port)
{
    return PINS_GPIOGetPinsOutput(port);
}

/**
 *
 * Function Name : PINS_DRV_SetPins
 * Description   : This function configures output pins listed in parameter pins (bits that are
 * '1') to have a value of 'set' (HIGH). Pins corresponding to '0' will be unaffected.
 *
 */
void PINS_DRV_SetPins(uint8_t port, pins_channel_type_t pins)
{
    PINS_GPIOSetPins(port, pins);
}

/**
 *
 * Function Name : PINS_DRV_ClearPins
 * Description   : This function configures output pins listed in parameter pins (bits that are
 * '1') to have a 'cleared' value (LOW). Pins corresponding to '0' will be unaffected.
 *
 */
void PINS_DRV_ClearPins(uint8_t port, pins_channel_type_t pins)
{
    PINS_GPIOClearPins(port, pins);
}

/**
 *
 * Function Name : PINS_DRV_TogglePin
 * Description   : This function toggles output pin listed in parameter pin (bits that are
 * '1'). Pins corresponding to '0' will be unaffected.
 *
 */
void PINS_DRV_TogglePin(uint8_t port, pins_channel_type_t pin)
{
    PINS_GPIOTogglePin(port, pin);
}

/**
 *
 * Function Name : PINS_DRV_TogglePins
 * Description   : This function toggles output pins listed in parameter pins (bits that are
 * '1'). Pins corresponding to '0' will be unaffected.
 *
 */
void PINS_DRV_TogglePins(uint8_t port, pins_channel_type_t pins)
{
    PINS_GPIOTogglePins(port, pins);
}

/**
 *
 * Function Name : PINS_DRV_ReadPins
 * Description   : This function returns the current input values from a port. Only pins
 * configured as input will have meaningful values.
 *
 */
pins_channel_type_t PINS_DRV_ReadPins(const uint8_t port)
{
    return PINS_GPIOReadPins(port);
}

/**
 *
 * Function Name : PINS_DRV_SetPinDma
 * Description   : This function configures port-pin's dma enable value and port's dma select value.
 *
 */
void PINS_DRV_SetPinDma(gpio_dma_channel_t channel, uint8_t port, pins_channel_type_t pin)
{
    PINS_GPIOSetPinDmaEn(port, pin);
    uint32_t pin128 = (uint32_t)(port * 32 + pin);
    PINS_GPIOSetChannelDmaSel(channel, pin128);
}

/**
 *
 * Function Name : PINS_DRV_ClearPinsDma
 * Description   : This function clear all dma enable value and dma select value.
 *
 */
void PINS_DRV_ClearPinsDma(void)
{
    PINS_GPIOClearPinsDmaEn(PORTA);
    PINS_GPIOClearPinsDmaEn(PORTB);
    PINS_GPIOClearPinsDmaEn(PORTC);
    PINS_GPIOClearPinsDmaEn(PORTD);
    PINS_GPIOClearChannelDmaSel(GPIO_DMA_CHANNEL_0);
    PINS_GPIOClearChannelDmaSel(GPIO_DMA_CHANNEL_1);
    PINS_GPIOClearChannelDmaSel(GPIO_DMA_CHANNEL_2);
    PINS_GPIOClearChannelDmaSel(GPIO_DMA_CHANNEL_3);
}

/**
 *
 * Function Name : PINS_DRV_SetPinDmaEn
 * Description   : This function configures port-pin's dma enable value.
 *
 */
void PINS_DRV_SetPinDmaEn(uint8_t port, pins_channel_type_t pin)
{
    PINS_GPIOSetPinDmaEn(port, pin);
}

/**
 *
 * Function Name : PINS_DRV_ClearPinsDmaEn
 * Description   : This function clear port's all dma enable values.
 *
 */
void PINS_DRV_ClearPinsDmaEn(uint8_t port)
{
    PINS_GPIOClearPinsDmaEn(port);
}

/**
 *
 * Function Name : PINS_DRV_SetChannelDmaSel
 * Description   : This function set channel dma sel pin128's value.
 *
 */
void PINS_DRV_SetChannelDmaSel(gpio_dma_channel_t channel, pins_channel_type_t pin128)
{
    PINS_GPIOSetChannelDmaSel(channel, pin128);
}

/**
 *
 * Function Name : PINS_DRV_ClearChannelDmaSel
 * Description   : This function clear channel dma sel pin128's value.
 *
 */
void PINS_DRV_ClearChannelDmaSel(gpio_dma_channel_t channel)
{
    PINS_GPIOClearChannelDmaSel(channel);
}
