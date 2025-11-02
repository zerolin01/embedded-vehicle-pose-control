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
 * @file pins_hw_access.c
 * @brief PINS driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pins_hw_access.h"
#include "pins_register.h"

/**
 *
 * Function Name : PINS_Init
 * Description   : This function configures the pins with the options provided in the provided structure.
 *
 */
void PINS_Init(const pin_settings_config_t *config)
{
    OS_ASSERT(config->base < PINMUX_PORT_COUNT);
    OS_ASSERT(config->pinPortIdx < PINMUX_PORT_PCR_COUNT);
    uint32_t regValue = PINS_GetPortControlRegister(config->base, config->pinPortIdx);

    port_mux_t muxing;

    switch (config->pullConfig) {
    case PORT_INTERNAL_PULL_NOT_ENABLED: {
        regValue &= ~(PORT_PCR_PE_MASK);
    } break;
    case PORT_INTERNAL_PULL_DOWN_ENABLED: {
        regValue &= ~(PORT_PCR_PE_MASK);
        regValue |= PORT_PCR_PE(1U);

        regValue &= ~(PORT_PCR_PS_MASK);
    } break;
    case PORT_INTERNAL_PULL_UP_ENABLED: {
        regValue &= ~(PORT_PCR_PE_MASK);
        regValue |= PORT_PCR_PE(1U);

        regValue &= ~(PORT_PCR_PS_MASK);
        regValue |= PORT_PCR_PS(1U);
    } break;
    default:
        /* Invalid command. */
        OS_ASSERT(false);
        break;
    }

    regValue &= ~(PORT_PCR_DS_MASK);
    regValue |= PORT_PCR_DS(config->driveSelect);

    regValue &= ~(PORT_PCR_FS_MASK);
    muxing = config->mux;
    regValue |= PORT_PCR_FS(muxing);
    PINS_SetPortControlRegister(config->base, config->pinPortIdx, regValue);

    if (config->isGpio != false) {
        OS_ASSERT(config->direction < GPIO_UNSPECIFIED_DIRECTION);
        /* Configure initial value for output. */
        if (GPIO_OUTPUT_DIRECTION == config->direction) {
            PINS_GPIOWritePin(config->base, config->pinPortIdx, config->initValue);
        }

        /* Configure debounce. */
        if (false == config->debounceEnable) {
            PINS_DisablePinDebounce(config->base, config->pinPortIdx);
        } else {
            PINS_EnablePinDebounce(config->base, config->pinPortIdx);
        }

        /* Configure intConfig. */
        PINS_SetPinIntSel(config->base, config->pinPortIdx, config->intConfig);

        /* Configure clearIntFlag. */
        if (config->clearIntFlag) {
            PINS_ClearPinIntFlagCmd(config->base, config->pinPortIdx);
        }

        /* Configure direction. */
        PINS_GPIOWritePinDirection(config->base, config->pinPortIdx, config->direction);
    } else {
        PINS_GPIOWritePin(config->base, config->pinPortIdx, (pins_level_type_t)0U);
        PINS_DisablePinDebounce(config->base, config->pinPortIdx);
        PINS_ClearPinIntSel(config->base, config->pinPortIdx);
        PINS_ClearPinIntFlagCmd(config->base, config->pinPortIdx);
        PINS_GPIOWritePinDirection(config->base, config->pinPortIdx, GPIO_INPUT_DIRECTION);
    }
}

/**
 *
 * Function Name : PINS_SetPullSel
 * Description   : Configures the internal resistor.
 *
 * Pull configuration is valid in all digital pin muxing modes.
 *
 */
void PINS_SetPullSel(uint8_t port, uint32_t pin, port_pull_config_t pullConfig)
{
    OS_ASSERT(pin < PINMUX_PORT_PCR_COUNT);
    uint32_t regValue = PINS_GetPortControlRegister(port, pin);
    switch (pullConfig) {
    case PORT_INTERNAL_PULL_NOT_ENABLED: {
        regValue &= ~(PORT_PCR_PE_MASK);
        PINS_SetPortControlRegister(port, pin, regValue);
    } break;
    case PORT_INTERNAL_PULL_DOWN_ENABLED: {
        regValue &= ~(PORT_PCR_PE_MASK);
        regValue |= PORT_PCR_PE(1U);

        regValue &= ~(PORT_PCR_PS_MASK);
        PINS_SetPortControlRegister(port, pin, regValue);
    } break;
    case PORT_INTERNAL_PULL_UP_ENABLED: {
        regValue &= ~(PORT_PCR_PE_MASK);
        regValue |= PORT_PCR_PE(1U);

        regValue &= ~(PORT_PCR_PS_MASK);
        regValue |= PORT_PCR_PS(1U);
        PINS_SetPortControlRegister(port, pin, regValue);
    } break;
    default:
        /* Invalid command. */
        OS_ASSERT(false);
        break;
    }
}

/**
 *
 * Function Name : PINS_SetMuxModeSel
 * Description   : This function configures the pin muxing and support configuring
 * for the pins that have ADC interleaved channel as well.
 *
 */
void PINS_SetMuxModeSel(uint8_t port, uint32_t pin, port_mux_t mux)
{
    OS_ASSERT(pin < PINMUX_PORT_PCR_COUNT);
    uint32_t regValue = PINS_GetPortControlRegister(port, pin);
    port_mux_t muxing = mux;

    regValue &= ~(PORT_PCR_FS_MASK);
    regValue |= PORT_PCR_FS(muxing);

    PINS_SetPortControlRegister(port, pin, regValue);
}
