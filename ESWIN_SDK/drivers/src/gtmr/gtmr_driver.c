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
 * @file gtmr_driver.c
 * @brief GTRM driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "gtmr_driver.h"
#include "clock_driver.h"
#include "gtmr_hw_access.h"

/*
 * @brief Table of base addresses for GTMR instances
 */
static gtmr_type_t *const g_pstGtmrBase[GTMR_INSTANCE_COUNT] = GTMR_BASE_PTRS;

/** Table to save  GTMR IRQ enum numbers. */
static const IRQn_Type g_stGtmrIrqId[GTMR_INSTANCE_COUNT] = GTMR_IRQS;

/** Interrupt of the configuration structure **/
typedef struct {
    uint16_t instance;
    gtmr_callback_t callBack;
    void *parameter;
} gtmr_irq_t;

static gtmr_irq_t g_stGtmrIRQ;

/*
 *
 * Function Name : GTMR_IRQHandler
 * Description   : interrupt handling functions
 */
static void GTMR_IRQHandler(void *para)
{
    uint32_t instance;

    instance = g_stGtmrIRQ.instance;

    if (NULL != g_stGtmrIRQ.callBack) {
        g_stGtmrIRQ.callBack(g_stGtmrIRQ.parameter);
    } else {
    }

    GTMR_DRV_ClearIntFlag(instance);
}

/*
 *
 * Function Name : GTMR_DRV_InstallCallback.
 * Description   : Registers the callback function and the parameter for GTMR.
 * 
 */
status_t GTMR_DRV_InstallCallback(uint32_t instance, gtmr_callback_t callback, void *parameter)
{

    if ((NULL == callback) || (instance != 0)) {
        return STATUS_ERRORPAR;
    }

    g_stGtmrIRQ.callBack  = callback;
    g_stGtmrIRQ.parameter = parameter;
    g_stGtmrIRQ.instance  = instance;

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : GTMR_DRV_Init
 * Description   : Initialize GTMR Module,configure GTMR working mode according to structure gtmr_config_t
 *
 */
void GTMR_DRV_Init(const uint32_t instance, const gtmr_config_t *const config)
{

    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    CLOCK_SYS_ClockEnable(GTMR_CLK, true);

    GTMR_DRV_SetConfig(instance, config);
}

/*
 *
 * Function Name : GTMR_DRV_SetConfig
 * Description   : Configure a GTMR instance based on the input configuration
 * structure.such as interrupt, trigger mode, start value.
 *
 */
void GTMR_DRV_SetConfig(const uint32_t instance, const gtmr_config_t *const config)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    uint32_t StartValue            = config->startValue;
    uint32_t TriggerValue          = config->triggerValue;
    bool TriggerContinueMode       = config->continuousModeEnable;
    gtmr_Triggermode_t TriggerType = config->triggerMode;
    register_type_t type;

    /* Save the relevant parameter needed for the interrupt */
    g_stGtmrIRQ.callBack  = config->callBack;
    g_stGtmrIRQ.parameter = config->parameter;
    g_stGtmrIRQ.instance  = instance;

    /* Registration interrupt */
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = NULL;
    OS_RequestIrq(g_stGtmrIrqId[instance], GTMR_IRQHandler, &type);

    /* Reset GTMR (clear GTMR register)  */
    GTMR_Reset(base);

    /* Enable or disable GTMR interrupt */
    GTMR_EnableInt(base, config->interruptEnable);

    /* Enable/disable GTMR interrupt in NVIC level */
    if (config->interruptEnable) {
        OS_EnableIrq(g_stGtmrIrqId[instance]);
    } else {
        OS_DisableIrq(g_stGtmrIrqId[instance]);
    }

    /* Config the way for GTMR to be triggered(software or trgmux)*/
    GTMR_SetTriggerType(base, TriggerType);

    /* Config GTMR Counter start value, GTMR count from the  StartValue to 0xFFFFFFFF*/
    GTMR_SetStartValue(base, StartValue);

    /* When the count value is equal to the TriggerValue, GTMR generates a trigger signal*/
    GTMR_SetTriggerValue(base, TriggerValue);

    /* Configure the GTMR in single mode or continuous mode*/
    GTMR_SetTriggerContinuous(base, TriggerContinueMode);
}

/*
 *
 * Function Name : GTMR_DRV_StartCounter
 * Description   : Start GTMR Module.The GTMR function begin to start counting.
 *
 */
void GTMR_DRV_StartCounter(const uint32_t instance, const gtmr_config_t *const config)

{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    gtmr_Triggermode_t TriggerType = config->triggerMode;

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    GTMR_Enable(base);
    GTMR_GenSoftWareTrigger(base, TriggerType);
}

/*
 *
 * Function Name : GTMR_DRV_StopCounter
 * Description   : Stop the GTMR count, not stop GTMR module. Write 1 to stop the GTMR count,write to restart GTMR
 * count.
 *
 */
void GTMR_DRV_StopCounter(const uint32_t instance, bool enable)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    GTMR_Stop(base, enable);
}

/*
 *
 * Function Name : GTMR_DRV_GetIntFlag
 * Description   : Get GTMR timer full mask(FULL register) state. full mask also is the interrupt flag
 *
 */
bool GTMR_DRV_GetIntFlag(const uint32_t instance)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    const gtmr_type_t *const base = g_pstGtmrBase[instance];

    bool compareFlag = GTMR_GetIntFlag(base);

    return compareFlag;
}

/*
 *
 * Function Name : GTMR_DRV_ClearIntFlag
 * Description   : Clear GTMR interrupt(full) Flag,GTMR is an overflow interrupt,when the counter equals to 0xFFFF_FFFF
 * it generate an interrupt.
 *
 */
void GTMR_DRV_ClearIntFlag(const uint32_t instance)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    GTMR_ClearInt(base);
}

/*
 *
 * Function Name : GTMR_DRV_IsRunning
 * Description   : Get the running state of a GTMR instance.
 *
 */
bool GTMR_DRV_IsRunning(const uint32_t instance)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    const gtmr_type_t *const base = g_pstGtmrBase[instance];

    bool runningState = GTMR_GetEnable(base);

    return runningState;
}

/*
 *
 * Function Name : GTMR_DRV_SetInterrupt
 * Description   : Enable/disable the GTMR interrupt.
 *
 */
void GTMR_DRV_SetInterrupt(const uint32_t instance, const bool enableInterrupt)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    GTMR_EnableInt(base, enableInterrupt);
}

/*
 *
 * Function Name : GTMR_DRV_GetCounterValue
 * Description   : Get the current counter value.
 *
 */
uint32_t GTMR_DRV_GetCounterValue(const uint32_t instance)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    uint32_t counterVal = GTMR_GetCounterValue(base);

    return counterVal;
}

/*
 *
 * Function Name : GTMR_DRV_ClearTriggerFlag
 * Description   : Clear GTMR Trigger(TRIGF) Flag.
 *
 */
void GTMR_DRV_ClearTriggerFlag(const uint32_t instance)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    GTMR_ClearTriggerFlag(base);
}

/*
 *
 * Function Name : GTMR_DRV_CounterReset
 * Description   : GTMR counter reset not GTMR module reset. After reset the counter value set to user-specified
 * initial value.
 *
 */
void GTMR_DRV_CounterReset(const uint32_t instance)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    GTMR_CounterReset(base);
}

/*
 *
 * Function Name : GTMR_DRV_GenOnceTrigger
 * Description   : Enable Once Trigger, If GTMR is running in single-trigger mode,this function must be called to enable
 * trigger out. This function can be ignored if GTMR work in continues-trigger mode.
 *
 */
void GTMR_DRV_GenOnceTrigger(const uint32_t instance)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    GTMR_GenOnceTrigger(base);
}

/*
 *
 * Function Name : GTMR_DRV_Deinit
 * Description   : De-initialize GTMR Module
 *
 * stop the GTMR and reset all registers to default value.
 *
 */
void GTMR_DRV_Deinit(const uint32_t instance)
{
    OS_ASSERT(instance < GTMR_INSTANCE_COUNT);

    gtmr_type_t *const base = g_pstGtmrBase[instance];

    /* Disable GTMR IRQ in  in NVIC level */
    OS_DisableIrq(g_stGtmrIrqId[instance]);

    /* Destroy registration function */
    OS_UnRequestIrq(g_stGtmrIrqId[instance]);

    GTMR_Disable(base);

    GTMR_Reset(base);
}
