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
 * @file ewm_driver.c
 * @brief EWM driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-10
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "ewm_hw_access.h"
#include "ewm_register.h"
#include "clock_driver.h"
#include <stddef.h>

static ewm_type_t *const g_pstEwmBase[] = EWM_BASE_PTRS;

/** Table to save  EWM IRQ enum numbers. */
static const IRQn_Type g_stEwmIrqId[EWM_INSTANCE_COUNT] = EWM_IRQS;

/** Interrupt of the configuration structure **/
typedef struct {
    uint16_t instance;
    ewm_callback_t callBack;
    void *parameter;
} ewm_irq_t;

static ewm_irq_t g_stEwmIRQ;

/*
 *
 * Function Name : EWM_IRQHandler
 * Description   : interrupt handling functions
 *
 */
static void EWM_IRQHandler(void *para)
{
    uint32_t instance = 0;

    instance = g_stEwmIRQ.instance;

    if (NULL != g_stEwmIRQ.callBack) {
        g_stEwmIRQ.callBack(g_stEwmIRQ.parameter);
    } else {
    }

    EWM_DRV_ClearIntFlag(instance);
}

/*
 *
 * Function Name : EWM_DRV_InstallCallback.
 * Description   : Registers the callback function and the parameter for EWM.
 * 
 */
status_t EWM_DRV_InstallCallback(uint32_t instance, ewm_callback_t callback, void *parameter)
{

    if ((NULL == callback) || (instance != 0)) {
        return STATUS_ERRORPAR;
    }

    g_stEwmIRQ.callBack  = callback;
    g_stEwmIRQ.parameter = parameter;
    g_stEwmIRQ.instance  = instance;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : EWM_DRV_Init
 * Description   : Init EWM Module.
 *
 */
status_t EWM_DRV_Init(uint32_t instance, const ewm_init_config_t *config)
{
    OS_ASSERT(instance < EWM_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    register_type_t type;

    /*checkout whether ewm is enabled*/
    bool ewmEnableStatus = false;

    /* Return status variable */
    status_t statusCode = STATUS_SUCCESS;

    /* Flag to store if the module is enabled */
    uint8_t regValue = 0U;

    /* Base pointer */
    ewm_type_t *base = g_pstEwmBase[instance];

    /* Enable EWM clock */
    CLOCK_SYS_ClockEnable(EWM_CLK, true);

    /* EWM can be enabled only once after it is powered on*/
    ewmEnableStatus = EWM_IsEnabled(base);
    if (ewmEnableStatus) {
        statusCode = STATUS_BUSY;
        return statusCode;
    }
    /* Save the relevant parameter needed for the interrupt */
    g_stEwmIRQ.callBack  = config->callBack;
    g_stEwmIRQ.instance  = instance;
    g_stEwmIRQ.parameter = config->parameter;

    /* Registration EWM interrupt */
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = NULL;
    OS_RequestIrq(g_stEwmIrqId[instance], EWM_IRQHandler, &type);

    /* Check if the windows values are not correct */
    if ((config->compareHigh <= config->compareLow) || (config->compareHigh > EWM_CMPH_MAX_VALUE)) {
        statusCode = STATUS_ERRORPAR;
    } else {
        /* Set clock prescaler */
        EWM_SetPrescaler(base, config->prescaler);
        /* Set compare high values */
        EWM_SetCompareHigh(base, config->compareHigh);
        /* Set compare low values*/
        EWM_SetCompareLow(base, config->compareLow);

        /* Set EWM interrupt*/
        if (config->interruptEnable) {
            regValue = EWM_CTRL_INTEN(EWM_ENABLE);
            OS_EnableIrq(g_stEwmIrqId[instance]);

        } else {
            regValue = EWM_CTRL_INTEN(EWM_DISABLE);
            OS_DisableIrq(g_stEwmIrqId[instance]);
        }

        /* Set EWM_in status */
        switch (config->assertLogic) {
            /* Input pin enabled, Input asserted on logic 0*/
        case EWM_IN_ASSERT_ON_LOGIC_ZERO:
            regValue |= EWM_CTRL_INEN_MASK;
            break;
            /* Input pin enabled, Input asserted on logic 1*/
        case EWM_IN_ASSERT_ON_LOGIC_ONE:
            regValue |= (EWM_CTRL_INEN_MASK | EWM_CTRL_ASSIN_MASK);
            break;
        case EWM_IN_ASSERT_DISABLED:
            break;
        default:
            /* Input pin disabled */
            break;
        }

        regValue = regValue | EWM_CTRL_EWMEN(EWM_ENABLE);
        /* Write the configuration into the Control register */
        EWM_SetControl(base, regValue);
    }

    /* Return the status code */
    return statusCode;
}

/**
 *
 * Function Name : EWM_DRV_Refresh
 * Description   : Refresh EWM Module
 *
 * similar to WDOG  service the dog
 *
 */
void EWM_DRV_Refresh(uint32_t instance)
{
    OS_ASSERT(instance < EWM_INSTANCE_COUNT);

    ewm_type_t *base = g_pstEwmBase[instance];

    /* Write the refresh values */
    EWM_Refresh(base);
}

/**
 *
 * Function Name : EWM_DRV_GetInputPinAssertLogic
 * Description   : Get the EWM_in pin assert logic.
 *
 */
ewm_in_assert_logic_t EWM_DRV_GetInputPinAssertLogic(uint32_t instance)
{
    OS_ASSERT(instance < EWM_INSTANCE_COUNT);

    const ewm_type_t *const base = g_pstEwmBase[instance];

    uint8_t regValue = 0;
    /* Variable where to save the retrieved configuration */
    ewm_in_assert_logic_t returnValue;

    regValue = EWM_GetControl(base);

    /*Get ewm_in enable status*/
    regValue &= EWM_CTRL_INEN_MASK;

    /*if ewm_in enabled, get ewm_in pin status */
    if (regValue) {

        regValue = EWM_GetControl(base);

        regValue &= EWM_CTRL_ASSIN_MASK;

        if (0 != regValue) {
            returnValue = EWM_IN_ASSERT_ON_LOGIC_ONE;
        } else {
            returnValue = EWM_IN_ASSERT_ON_LOGIC_ZERO;
        }

    } else {
        /* Pin is disabled */
        returnValue = EWM_IN_ASSERT_DISABLED;
    }

    return returnValue;
}

/**
 *
 * Function Name : EWM_DRV_ClearIntFlag
 * Description   : Clear EWM interrupt Flag
 *
 */
void EWM_DRV_ClearIntFlag(uint32_t instance)
{
    OS_ASSERT(instance < EWM_INSTANCE_COUNT);

    uint8_t regValue = 0U;

    ewm_type_t *base = g_pstEwmBase[instance];

    regValue = (uint8_t)(~EWM_CTRL_INTEN_MASK);

    base->CTRL = base->CTRL & regValue;
}

/**
 *
 * Function Name : EWM_DRV_GetDefaultConfig
 * Description   : Init configuration structure to default values.
 *
 */
void EWM_DRV_GetDefaultConfig(ewm_init_config_t *config)
{
    OS_ASSERT(config != NULL);
    /* Disable interrupts */
    config->interruptEnable = false;
    /* Input pin enabled and configured to assert on logic 0 */
    config->assertLogic = EWM_IN_ASSERT_ON_LOGIC_ZERO;
    /* Maximum prescaler */
    config->prescaler = 0xFEU;
    /* Maximum service window */
    config->compareHigh = EWM_CMPH_MAX_VALUE;
    config->compareLow  = EWM_CMPL_MIN_VALUE;
}

/*
 *
 * Function Name : EWM_DRV_Deinit
 * Description   : De-initialize EWM Module
 *
 * stop the GTMR and reset all registers to default value.
 *
 */
void EWM_DRV_Deinit(const uint32_t instance)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    /* Disable GTMR IRQ in  in NVIC level */
    OS_DisableIrq(g_stEwmIrqId[instance]);

    /* Destroy registration function */
    OS_UnRequestIrq(g_stEwmIrqId[instance]);
}
