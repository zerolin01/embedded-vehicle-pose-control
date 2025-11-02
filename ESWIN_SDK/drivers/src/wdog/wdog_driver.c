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
 * @file wdog_driver.c
 * @brief WDOG driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "wdog_hw_access.h"
#include "clock_driver.h"

/*
 * @brief Table of base addresses for WDOG instances.
 */
static wdog_type_t *const sg_pstWdogBase[WDOG_INSTANCE_COUNT] = WDOG_BASE_PTRS;

/*
 * @brief Table to save WDOG IRQ enum numbers defined in CMSIS header file.
 */
static const IRQn_Type g_stWdogIrqId[WDOG_INSTANCE_COUNT] = WDOG_IRQS;

/** Interrupt of the configuration structure **/
typedef struct {
    uint16_t instance;
    wdog_callback_t callBack;
    void *parameter;
} wdog_irq_t;

static wdog_irq_t g_stWdogIRQ;

/*
 *
 * Function Name : WDOG_IRQHandler
 * Description   : interrupt handling functions
 *
 */
static void WDOG_IRQHandler(void *para)
{
    uint32_t instance = 0;

    instance = g_stWdogIRQ.instance;

    if (NULL != g_stWdogIRQ.callBack) {
        g_stWdogIRQ.callBack(g_stWdogIRQ.parameter);
    } else {
    }

    WDOG_DRV_ClearIntFlag(instance);
}

/*
 *
 * Function Name : WDOG_DRV_InstallCallback.
 * Description   : Registers the callback function and the parameter for WDOG.
 * 
 */
status_t WDOG_DRV_InstallCallback(uint32_t instance, wdog_callback_t callback, void *parameter)
{

    if ((NULL == callback) || (instance != 0)) {
        return STATUS_ERRORPAR;
    }

    g_stWdogIRQ.callBack  = callback;
    g_stWdogIRQ.parameter = parameter;
    g_stWdogIRQ.instance  = instance;

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : WDOG_DRV_Init
 * Description   : Initializes the WDOG driver.
 *
 */
status_t WDOG_DRV_Init(uint32_t instance, const wdog_user_config_t *userConfigPtr)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);
    OS_ASSERT(userConfigPtr != NULL);

    wdog_type_t *base = sg_pstWdogBase[instance];
    status_t status   = STATUS_SUCCESS;
    register_type_t type;

    /* Enable WDOG clock */
    CLOCK_SYS_ClockEnable(WDOG_CLK, true);

    /* Save the relevant parameter needed for the interrupt */
    g_stWdogIRQ.instance  = 0;
    g_stWdogIRQ.callBack  = userConfigPtr->callBack;
    g_stWdogIRQ.parameter = userConfigPtr->parameter;

    /* Registration interrupt */
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = NULL;
    OS_RequestIrq(g_stWdogIrqId[instance], WDOG_IRQHandler, &type);

    /* Enable WDOG interrupt in NVIC level */
    OS_EnableIrq(g_stWdogIrqId[instance]);

    /* If interrupt value greater than or equal to the timeout value. Or timeout value is 0 */
    if ((userConfigPtr->intValue >= userConfigPtr->timeoutValue) ||
        (userConfigPtr->timeoutValue == WDOG_MINIMUM_TIMEOUT_VALUE)) {
        status = STATUS_ERRORPAR;
        return status;
    } else {

        /* Configure the WDOG module */
        status = WDOG_Config(base, userConfigPtr);
    }

    return status;
}

/*
 *
 * Function Name : WDOG_DRV_Deinit
 * Description   : De-initializes the WDOG driver
 *
 */
void WDOG_DRV_Deinit(uint32_t instance)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);
    wdog_type_t *base = sg_pstWdogBase[instance];

    /* Disable WDOG timeout interrupt */
    OS_DisableIrq(g_stWdogIrqId[instance]);

    /* Destroy registration function */
    OS_UnRequestIrq(g_stWdogIrqId[instance]);

    /* Disable WDOG */
    WDOG_Disable(base);
}

/*
 *
 * Function Name : WDOG_DRV_StartWDOG
 * Description   : WDOG Start
 *
 */
void WDOG_DRV_StartWDOG(uint32_t instance)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);

    wdog_type_t *base = sg_pstWdogBase[instance];
    WDOG_Enable(base);
}

/*
 *
 * Function Name : WDOG_DRV_GetConfig
 * Description   : Gets the current configuration of the WDOG.
 *
 */
void WDOG_DRV_GetConfig(uint32_t instance, wdog_user_config_t *const config)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    wdog_type_t *base = sg_pstWdogBase[instance];

    *config = WDOG_GetConfig(base);
}

/*
 *
 * Function Name : WDOG_DRV_GetDefaultConfig
 * Description   : Gets default configuration of the WDOG.
 *
 */
void WDOG_DRV_GetDefaultConfig(wdog_user_config_t *const config)
{
    OS_ASSERT(config != NULL);

    config->stop         = WDOG_WORK_SUSPEND;
    config->intEnable    = true;
    config->timeoutValue = WDOG_TO_RESET_VALUE;
    config->intValue     = WDOG_TO_RESET_INT_VALUE;
}

/*
 *
 * Function Name : WDOG_DRV_ClearIntFlag
 * Description   : Clear interrupt flag of the WDOG.
 *
 */
void WDOG_DRV_ClearIntFlag(uint32_t instance)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);
    wdog_type_t *base = sg_pstWdogBase[instance];

    /* Clear interrupt flag of the WDOG */
    WDOG_ClearIntFlag(base);
}

/*
 *
 * Function Name : WDOG_DRV_Trigger
 * Description   : Refreshes the WDOG counter.
 *
 */
void WDOG_DRV_Trigger(uint32_t instance)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);
    wdog_type_t *base = sg_pstWdogBase[instance];

    WDOG_Refresh(base);
}

/*
 *
 * Function Name : WDOG_DRV_GetCounter
 * Description   : Get the value of the WDOG counter that user config.
 *
 */
uint16_t WDOG_DRV_GetCounter(uint32_t instance)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);
    const wdog_type_t *base = sg_pstWdogBase[instance];

    return (uint16_t)((base->WCR & WDOG_WCR_WT_MASK) >> WDOG_WCR_WT_SHIFT);
}

/*
 *
 * Function Name : WDOG_DRV_SetTimeoutValue
 * Description   : Set the value of the WDOG timeout.
 *
 */
void WDOG_DRV_SetTimeoutValue(uint32_t instance, uint16_t timeout)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);
    wdog_type_t *base = sg_pstWdogBase[instance];
    WDOG_SetTimeOutValue(base, timeout);
}

/*
 *
 * Function Name : WDOG_DRV_SetInt
 * Description   : Set how long time WDOG interrupt happened before the WDOG time-out. And enable the interrupt.
 *
 * This field is write once only.Once the software does a write access to this field, is will get locked and cannot be
 * reprogrammed until the next system reset assertion.
 *
 */
void WDOG_DRV_SetInt(uint32_t instance, uint16_t intval)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);
    wdog_type_t *base = sg_pstWdogBase[instance];

    WDOG_SetInt(base, intval);
}

/*
 *
 * Function Name : WDOG_DRV_SoftWareReset
 * Description   : Software generated reset
 *
 * Config WDOG software reset mode(immediate or delay mode),and enable software reset.Delay mode delay two 32K cycles
 * before reset. when use this mode can config immediate software mode.
 *
 */
void WDOG_DRV_SoftWareReset(uint32_t instance, wdog_software_reset_mode_t mode)
{
    OS_ASSERT(instance < WDOG_INSTANCE_COUNT);
    wdog_type_t *base = sg_pstWdogBase[instance];

    /* config WDOG software reset mode immediate or delay(reg WCR.SRE)  */
    base->WCR &= ~WDOG_WCR_SRE_MASK;
    base->WCR |= WDOG_WCR_SRE(mode);

    /* software reset*/
    base->WCR &= ~WDOG_WCR_SRS_MASK;
}

