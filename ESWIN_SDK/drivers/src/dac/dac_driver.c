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
 * @file dac_driver.c
 * @brief DAC driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include <stddef.h>

#include "clock_driver.h"
#include "dac_driver.h"
#include "dac_hw_access.h"

/**
 * @brief Table of base addresses for DAC instances.
 */
static dac_type_t *const g_dacBase[] = DAC_BASE_PTRS;

/**
 * @brief clock sources, for getting the input clock frequency
 */
static const clock_names_t g_dacClock[DAC_INSTANCE_COUNT] = DAC_CRG_CLOCKS;

/**
 * @brief Table of IRQs for DAC instances.
 */
static const IRQn_Type s_dacIrqId[DAC_INSTANCE_COUNT] = DAC_IRQS;

/**
 *
 * Function Name : DAC_DRV_IRQHandler
 * Description   : IRQ Handler.
 *
 */
static void DAC_DRV_IRQHandler(void *data)
{
    dac_state_t *state = (dac_state_t *)data;
    DAC_DRV_SetIntEnable(state->instance, false);
    if (state->callback) {
        state->callback(state->callbackParam);
    }
    DAC_DRV_SetIntEnable(state->instance, true);
}

/**
 *
 * Function Name : DAC_DRV_Reset
 * Description   : This function set all DAC registers to reset values.
 *
 */
status_t DAC_DRV_Reset(const uint32_t instance)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);
    dac_type_t *baseAddr = g_dacBase[instance];
    baseAddr->C0         = 0;
    baseAddr->C1         = 0;
    baseAddr->C2         = 0;
    baseAddr->C3         = 0;
    return status;
}

/*
 *
 * Function Name : DAC_DRV_Init
 * Description   : Configure all components from comparator module.
 *
 */
status_t DAC_DRV_Init(const uint32_t instance, const dac_module_t* const config, dac_state_t *state)
{
    status_t  status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);
    dac_type_t* base = g_dacBase[instance];

    clock_names_t dacClocks[ADC_INSTANCE_COUNT] = DAC_CRG_CLOCKS;
    CLOCK_SYS_ClockEnable(dacClocks[instance], true);

#if 0
    if (config->dataMode) {
        DAC_DRV_SetSampleRate(instance, config->sampleRate);
    } else {
        DAC_SetFrequencyWord(base, 0xFFFFFFFF);
    }
#else
    DAC_DRV_SetSampleRate(instance, 1000000);
#endif

    DAC_SetOutputVolBuffer(base, config->outputVolBuf);
    if (config->interruptEnable) {
        OS_RegisterType_t type;
        type.trig_mode = CLIC_LEVEL_TRIGGER;
        type.lvl       = 1;
        type.priority  = 0;
        type.data_ptr  = (void *)state;
        state->instance = instance;
        OS_RequestIrq(s_dacIrqId[instance], DAC_DRV_IRQHandler, &type);
        /* Enable Comparator interrupt */
        OS_EnableIrq(s_dacIrqId[instance]);
    }
    DAC_SetIntEnable(base, config->interruptEnable);
    DAC_SetEnableState(base, config->state);

    return status;
}

/**
 *
 * Function Name : DAC_DRV_GetConfig
 * Description   : Return configuration for DAC component from comparator module.
 *
 */
status_t DAC_DRV_GetConfig(const uint32_t instance, dac_module_t *const config)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);
    const dac_type_t *base = g_dacBase[instance];

    config->state           = DAC_GetEnableState(base);
    //config->dataMode        = DAC_GetDataMode(base);
    //config->waterLine       = DAC_GetWaterLine(base);
    config->interruptEnable = DAC_GetIntEnable(base);
    config->outputVolBuf    = DAC_GetOutputVolBuffer(base);

    //DAC_DRV_GetSampleRate(instance, &config->sampleRate);

    return status;
}

/**
 *
 * Function Name : DAC_DRV_GetSampleRate
 * Description   : This function get the DAC sampling rate.
 *
 */
status_t DAC_DRV_GetSampleRate(const uint32_t instance, uint32_t *sampleRate)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);
    OS_ASSERT(sampleRate != NULL);

    dac_type_t *const base = g_dacBase[instance];

    uint32_t dacFreq   = 0U;
    status_t clkStatus = CLOCK_SYS_GetFreq(g_dacClock[instance], &dacFreq);
    OS_ASSERT(clkStatus == STATUS_SUCCESS);
    (void)clkStatus;

    uint32_t frequencyWord = base->C2;
    frequencyWord          = (frequencyWord & DAC_C2_FW_MASK) >> DAC_C2_FW_SHIFT;

    *sampleRate = (uint32_t)((double)(dacFreq * frequencyWord) / (double)FEATURE_DAC_CALC_FREQWORD_CONST);

    return status;
}

/**
 *
 * Function Name : DAC_DRV_SetSampleRate
 * Description   : This function sets the configured sample ratet.
 *
 */
status_t DAC_DRV_SetSampleRate(const uint32_t instance, const uint32_t sampleRate)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);

    dac_type_t *const base = g_dacBase[instance];

    uint32_t dacFreq   = 0U;
    status_t clkStatus = CLOCK_SYS_GetFreq(g_dacClock[instance], &dacFreq);
    OS_ASSERT(clkStatus == STATUS_SUCCESS);
    (void)clkStatus;

    double frequencyWord = (uint64_t)FEATURE_DAC_CALC_FREQWORD_CONST / (double)dacFreq * (double)sampleRate + 0.5;
    base->C2             = ((base->C2) & (~(DAC_C2_FW_MASK))) | ((uint32_t)frequencyWord << DAC_C2_FW_SHIFT);

    return status;
}

/**
 *
 * Function Name : DAC_DRV_GetFiFOFullState
 * Description   : Return fifo full flag for DAC.
 *
 */
bool DAC_DRV_GetFiFOFullState(const uint32_t instance)
{
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);
    const dac_type_t *base = g_dacBase[instance];

    return DAC_GetFifoFullState(base);
}

/**
 *
 * Function Name : DAC_DRV_GetFiFOEmptyState
 * Description   : Return fifo Empty flag for DAC.
 *
 */
bool DAC_DRV_GetFiFOEmptyState(const uint32_t instance)
{
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);
    const dac_type_t *base = g_dacBase[instance];

    return DAC_GetFifoEmptyState(base);
}

/**
 *
 * Function Name : DAC_DRV_SetEnableState
 * Description   : This function enable the dac conversion.
 *
 */
status_t DAC_DRV_SetEnableState(const uint32_t instance, bool enable)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);

    dac_type_t *base = g_dacBase[instance];
    DAC_SetEnableState(base, enable);

    return status;
}

/**
 *
 * Function Name : DAC_DRV_SetDataInput
 * Description   : This function sets the DAC input data in data mode.
 *
 */
status_t DAC_DRV_SetDataInput(const uint32_t instance, uint32_t data)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);

    dac_type_t *base = g_dacBase[instance];
    DAC_SetDataInput(base, data);

    return status;
}

/**
 *
 * Function Name : DAC_DRV_SetIntEnable
 * Description   : This function enable/disable interrupt.
 *
 */
status_t DAC_DRV_SetIntEnable(const uint32_t instance, bool enable)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < DAC_INSTANCE_COUNT);

    dac_type_t *base = g_dacBase[instance];
    DAC_SetIntEnable(base, enable);

    return status;
}