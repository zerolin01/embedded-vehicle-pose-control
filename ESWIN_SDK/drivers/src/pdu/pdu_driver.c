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
 * @file pdu_driver.c
 * @brief PDU driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pdu_driver.h"
#include "pdu_hw_access.h"

/** Table of base addresses for  PDU instances. */
static pdu_type_t *const g_pstPduBase[PDU_INSTANCE_COUNT] = PDU_BASE_PTRS;

/** Table to save  PDU IRQ enum numbers. */
static const IRQn_Type g_stPduIrqId[PDU_INSTANCE_COUNT] = PDU_IRQS;

/** Table to save  Sequence Error PDU IRQ enum numbers. */
static const IRQn_Type g_stPduErrIrqId[PDU_INSTANCE_COUNT] = PDU_ERR_IRQS;

/** Interrupt of the configuration structure **/
typedef struct {
    uint32_t instance;
    pdu_callback_t callBack;
    void *parameter;
} pdu_irq_t;

/**  Sequence Error Interrupt of the configuration structure **/
typedef struct {
    pdu_callback_t callBack;
    void *parameter;
} pdu_err_irq_t;

static pdu_err_irq_t g_stdPduErrIRQ[PDU_INSTANCE_COUNT];

static pdu_irq_t g_stPduIRQ[PDU_INSTANCE_COUNT];

static const clock_names_t g_stPduClkNames[PDU_INSTANCE_COUNT] = {PDU0_CLK, PDU1_CLK};

/**   PDU IRQ handler  */
void PDU_IRQHandler(void *para)
{
    uint32_t instance = (uint32_t)para;

    if (NULL != g_stPduIRQ[instance].callBack) {
        g_stPduIRQ[instance].callBack(g_stPduIRQ[instance].parameter);
    } else {
    }

    /* Clear  PDU instance Interrupt flag */
    PDU_DRV_ClearTimerIntFlag(instance);
}

/**   PDU Sequence Error IRQ handler  */
void PDU_ERR_IRQHandler(void *para)
{

    uint16_t instance = (uint32_t)para;
    uint16_t chn      = 0;
    uint32_t preMask;

    if (NULL != g_stdPduErrIRQ[instance].callBack) {
        g_stdPduErrIRQ[instance].callBack(g_stdPduErrIRQ[instance].parameter);
    } else {
    }

    /* Clear  PDU instance Interrupt flag */
    for (chn = 0; chn < PDU_CH_COUNT; chn++) {
        preMask = PDU_DRV_GetAdcPreTriggerSeqErrFlags(instance, chn, 0xFF);
        if (preMask > 0) {
            PDU_DRV_ClearAdcPreTriggerSeqErrFlags(instance, chn, preMask);
        }
    }
}

/*
 *
 * Function Name : PDU_DRV_InstallCallback.
 * Description   : Registers the callback function and the parameter for PDU.
 * 
 */
status_t PDU_DRV_InstallCallback(uint32_t instance, pdu_callback_t callback, void *parameter)
{
    status_t reStatus = STATUS_SUCCESS;

    if ((NULL == callback) || (instance >= 2)) {
        return STATUS_ERRORPAR;
    }

    g_stPduIRQ[instance].callBack  = callback;
    g_stPduIRQ[instance].parameter = parameter;

    return reStatus;
}

/*
 *
 * Function Name : PDU_ERR_DRV_InstallCallback.
 * Description   : Registers the callback function and the parameter for PDU sequence error.
 * 
 */
status_t PDU_ERR_DRV_InstallCallback(uint32_t instance, pdu_callback_t callback, void *parameter)
{
    status_t reStatus = STATUS_SUCCESS;

    if ((NULL == callback) || (instance >= 2)) {
        return STATUS_ERRORPAR;
    }

    g_stdPduErrIRQ[instance].callBack  = callback;
    g_stdPduErrIRQ[instance].parameter = parameter;

    return reStatus;
}

/*
 *
 * Function Name : PDU_DRV_Init.
 * Description   : Initializes the  PDU counter and triggers input.
 * 
 * This function initializes the PDU counter,  input triggers and general pre-trigger 
 * settings. It resets  PDU registers and enables the clock for  PDU. So it should be
 * called before any operation to  PDU module. After initialized, the  PDU can
 * act as a triggered timer, which lays the foundation for other features in
 * PDU module.
 *
 */
void PDU_DRV_Init(const uint32_t instance, const pdu_timer_config_t *const userConfigPtr)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    OS_ASSERT(userConfigPtr != NULL);

    status_t clkStatus;

    pdu_type_t *base = g_pstPduBase[instance];

    register_type_t type;

    clock_names_t instanceClkName = g_stPduClkNames[instance];

    /* Enable PDU clock */
    CLOCK_SYS_ClockEnable(g_stPduClkNames[instance], true);

    /* Save the relevant parameters needed for the interrupt */

    g_stPduIRQ[instance].callBack  = userConfigPtr->callBack;
    g_stPduIRQ[instance].parameter = userConfigPtr->parameter;

    /* Save the relevant parameters needed for the Sequence Error interrupt */
    g_stdPduErrIRQ[instance].callBack  = userConfigPtr->seqErrCallBack;
    g_stdPduErrIRQ[instance].parameter = userConfigPtr->seqErrParameter;

    /* Registration interrupt and enable PDU interrupt in NVIC level*/
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = (void *)instance;
    OS_RequestIrq(g_stPduIrqId[instance], PDU_IRQHandler, &type);
    OS_EnableIrq(g_stPduIrqId[instance]);

    /* Registration  Sequence Error interrupt and enable PDU Sequence Error interrupt in NVIC level*/
    OS_RequestIrq(g_stPduErrIrqId[instance], PDU_ERR_IRQHandler, &type);
    OS_EnableIrq(g_stPduErrIrqId[instance]);
    /* Get the  PDU clock as configured in the clock manager */
    clkStatus = CLOCK_SYS_GetFreq(instanceClkName, NULL);

    /* Exit if clock is not enabled for current instance. */
    OS_ASSERT(clkStatus == STATUS_SUCCESS);
    (void)clkStatus;

    /* Reset the registers for  PDU module to reset state. */
    PDU_Init(base);
    PDU_ConfigTimer(base, userConfigPtr);

    /* Note: this configuration is common between  PDU instances 0 and 1 (configures the same register even if
       configured for either  PDU instance) Enabling when configuring PDU0, then disabling when configuring PDU1, will
       result in being disabled. */
    if (userConfigPtr->instanceBackToBackEnable) {
        PDU_TCSR_EnableInstanceBackToBack();
    } else {
        PDU_TCSR_DisableInstanceBackToBack();
    }

    if (userConfigPtr->interchannelBackToBackEnable) {
        PDU_TCSR_EnableInterchannelBackToBack(instance);
    } else {
        PDU_TCSR_DisableInterchannelBackToBack(instance);
    }
}

/*
 *
 * Function Name : PDU_DRV_Deinit.
 * Description   : De-initializes the  PDU module.
 * 
 */
void PDU_DRV_Deinit(const uint32_t instance)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    OS_DisableIrq(g_stPduIrqId[instance]);
    OS_DisableIrq(g_stPduErrIrqId[instance]);

    OS_UnRequestIrq(g_stPduIrqId[instance]);
    OS_UnRequestIrq(g_stPduErrIrqId[instance]);

    /* Note: instance back to back configuration is common between  PDU instances 0 and 1 (configures the same register
       even if configured for either  PDU instance)
       Enabling when configuring PDU0, then disabling when configuring PDU1, will result in being disabled. */
    PDU_TCSR_DisableInstanceBackToBack();

    PDU_TCSR_DisableInterchannelBackToBack(instance);

    PDU_Disable(base);
}

/*
 *
 * Function Name : PDU_DRV_GetDefaultConfig.
 * Description   : Gets the default configuration structure of PDU with default settings.
 * This function is used to initialize the hardware configuration structure to default values.
 * 
 */
void PDU_DRV_GetDefaultConfig(pdu_timer_config_t *const config)
{
    OS_ASSERT(config != NULL);
    /* Load immediately after load operation. */
    config->loadValueMode = PDU_LOAD_VAL_IMMEDIATELY;
    /* Sequence error interrupt disable */
    config->seqErrIntEnable = false;
    /* Prescaler divider select by MULT */
    config->clkPreDiv = PDU_CLK_PREDIV_BY_1;
    /* Select multiplication source mode */
    config->clkPreMultFactor = PDU_CLK_PREMULT_FACT_AS_1;
    /* Trigger input source selection */
    config->triggerInput = PDU_TRIGGER_IN0;
    /* Run in One-shot mode */
    config->continuousModeEnable = false;
    /* Disable PDMA */
    config->dmaEnable = false;
    /* Disable interrupt */
    config->intEnable = false;
    /* Disable instanceBackToBackEnable */
    config->instanceBackToBackEnable = false;
    /* Disable interchannelBackToBackEnable */
    config->interchannelBackToBackEnable = false;
}

/*
 *
 * Function Name : PDU_DRV_Enable.
 * Description   : Enable the PDU module.
 * 
 */
void PDU_DRV_Enable(const uint32_t instance)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);

    pdu_type_t *base = g_pstPduBase[instance];
    PDU_Enable(base);
}

/*
 *
 * Function Name : PDU_DRV_Disable.
 * Description   : Disables the  PDU module.
 * 
 */
void PDU_DRV_Disable(const uint32_t instance)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);

    pdu_type_t *base = g_pstPduBase[instance];
    PDU_Disable(base);
}

/*
 *
 * Function Name : PDU_DRV_SoftTriggerCmd.
 * Description   : When PDU is set to software trigger modes, user calling this function
 * can trigger the PDU module.
 * 
 */
void PDU_DRV_SoftTriggerCmd(const uint32_t instance)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    PDU_SetSoftTriggerCmd(base);
}

/*
 *
 * Function Name : PDU_DRV_GetTimerValue.
 * Description   : Gets the current value of the  PDU counter.
 * 
 */
uint32_t PDU_DRV_GetTimerValue(const uint32_t instance)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    pdu_type_t const *base = g_pstPduBase[instance];

    return PDU_GetTimerValue(base);
}

/*
 *
 * Function Name : PDU_DRV_GetTimerIntFlag.
 * Description   : Gets the PDU interrupt flag.
 * 
 */
bool PDU_DRV_GetTimerIntFlag(const uint32_t instance)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    pdu_type_t const *base = g_pstPduBase[instance];

    return PDU_GetTimerIntFlag(base);
}

/*
 *
 * Function Name : PDU_DRV_ClearTimerIntFlag.
 * Description   : Clears the interrupt flag.
 * 
 */
void PDU_DRV_ClearTimerIntFlag(const uint32_t instance)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    PDU_ClearTimerIntFlag(base);
}

/*
 *
 * Function Name : PDU_DRV_LoadValuesCmd.
 * Description   : Executes the command of loading values.
 * 
 */
void PDU_DRV_LoadValuesCmd(const uint32_t instance)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    PDU_SetLoadValuesCmd(base);
}

/*
 *
 * Function Name : PDU_DRV_SetTimerModulusValue.
 * Description   :Set PDU timer modulus.
 * 
 */
void PDU_DRV_SetTimerModulusValue(const uint32_t instance, const uint16_t value)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    PDU_SetTimerModulusValue(base, value);
}

/*
 *
 * Function Name : PDU_DRV_SetValueForTimerInterrupt.
 * Description   : Set the value for the PDU interrupt.
 * 
 */
void PDU_DRV_SetValueForTimerInterrupt(const uint32_t instance, const uint16_t value)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    PDU_SetValueForTimerInterrupt(base, value);
}

/*
 *
 * Function Name : PDU_DRV_ConfigAdcPreTrigger.
 * Description   : Configure PDU channel function.
 * 
 */
void PDU_DRV_ConfigAdcPreTrigger(const uint32_t instance, const uint32_t chn,
                                 const pdu_adc_pretrigger_config_t *configPtr)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    OS_ASSERT(chn < PDU_CH_COUNT);
    OS_ASSERT(configPtr != NULL);

    pdu_type_t *base = g_pstPduBase[instance];

    PDU_SetAdcPreTriggerEnable(base, chn, ((uint32_t)1U << (configPtr->adcPreTriggerIdx)), configPtr->preTriggerEnable);
    PDU_SetAdcPreTriggerOutputEnable(base, chn, ((uint32_t)1U << (configPtr->adcPreTriggerIdx)),
                                     configPtr->preTriggerOutputEnable);
    PDU_SetAdcPreTriggerBackToBackEnable(base, chn, ((uint32_t)1U << (configPtr->adcPreTriggerIdx)),
                                         configPtr->preTriggerBackToBackEnable);
}

/*
 *
 * Function Name : PDU_DRV_GetAdcPreTriggerFlags.
 * Description   : Get PDU channel flag in the  PDU module.
 * 
 */
uint32_t PDU_DRV_GetAdcPreTriggerFlags(const uint32_t instance, const uint32_t chn, const uint32_t preChnMask)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    OS_ASSERT(chn < PDU_CH_COUNT);
    pdu_type_t const *base = g_pstPduBase[instance];

    return PDU_GetAdcPreTriggerFlags(base, chn, preChnMask);
}

/*
 *
 * Function Name : PDU_DRV_ClearAdcPreTriggerFlags.
 * Description   : Clear PDU channel pre_trigger flag.
 * 
 */
void PDU_DRV_ClearAdcPreTriggerFlags(const uint32_t instance, const uint32_t chn, const uint32_t preChnMask)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    OS_ASSERT(chn < PDU_CH_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    PDU_ClearAdcPreTriggerFlags(base, chn, preChnMask);
}

/*
 *
 * Function Name : PDU_DRV_GetAdcPreTriggerSeqErrFlags.
 * Description   : Get PDU sequence error flag.
 * 
 */
uint32_t PDU_DRV_GetAdcPreTriggerSeqErrFlags(const uint32_t instance, const uint32_t chn, const uint32_t preChnMask)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    OS_ASSERT(chn < PDU_CH_COUNT);
    pdu_type_t const *base = g_pstPduBase[instance];

    return PDU_GetAdcPreTriggerSeqErrFlags(base, chn, preChnMask);
}

/*
 *
 * Function Name : PDU_DRV_ClearAdcPreTriggerSeqErrFlags.
 * Description   : Clear the sequence error flag.
 * 
 */
void PDU_DRV_ClearAdcPreTriggerSeqErrFlags(const uint32_t instance, const uint32_t chn, const uint32_t preChnMask)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    OS_ASSERT(chn < PDU_CH_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    PDU_ClearAdcPreTriggerSeqErrFlags(base, chn, preChnMask);
}

/*
 *
 * Function Name : PDU_DRV_SetAdcPreTriggerDelayValue.
 * Description   : Set the PDU pre_trigger delay value in the  PDU module.
 * 
 */
void PDU_DRV_SetAdcPreTriggerDelayValue(const uint32_t instance, const uint32_t chn, const uint32_t preChn,
                                        const uint16_t DLY)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    OS_ASSERT(chn < PDU_CH_COUNT);

    pdu_type_t *base = g_pstPduBase[instance];

    PDU_SetAdcPreTriggerDelayValue(base, chn, preChn, DLY);
}

/*
 *
 * Function Name : PDU_DRV_SetCmpPulseOutEnable.
 * Description   : enable or disable the CMP pulse out.
 * 
 */
void PDU_DRV_SetCmpPulseOutEnable(const uint32_t instance, const uint32_t pulseChnMask, const bool enable)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    PDU_SetCmpPulseOutEnable(base, pulseChnMask, enable);
}

/*
 *
 * Function Name : PDU_DRV_SetCmpPulseOutDelay.
 * Description   : Set the CMP pulse out low and high delay value for the PDU module.
 * 
 */
void PDU_DRV_SetCmpPulseOutDelay(const uint32_t instance, const uint32_t pulseChn, const uint16_t DLY1,
                                 const uint16_t DLY2)
{
    OS_ASSERT(instance < PDU_INSTANCE_COUNT);
    OS_ASSERT(pulseChn < PDU_POnDLY_COUNT);
    pdu_type_t *base = g_pstPduBase[instance];

    PDU_SetCmpPulseOutDelay(base, pulseChn, DLY1, DLY2);
}
