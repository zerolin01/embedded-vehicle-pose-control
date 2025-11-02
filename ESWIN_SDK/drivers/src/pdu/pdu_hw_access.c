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
 * @file pdu_hw_access.c
 * @brief PDU driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pdu_hw_access.h"

/*
 *
 * Function Name : PDU_Init.
 * Description   : Resets the PDU registers to a known state.
 * 
 */
void PDU_Init(pdu_type_t *const base)
{
    OS_ASSERT(base != NULL);
    uint32_t chn, preChn;

    base->SC = 0U;
    PDU_Enable(base);
    base->MOD  = 0xFFFFU;
    base->IDLY = 0xFFFFU;
    /* For ADC trigger. */
    for (chn = 0U; chn < PDU_CH_COUNT; chn++) {
        base->CH[chn].C1 = 0U;
        base->CH[chn].S  = 0U;
        for (preChn = 0U; preChn < PDU_DLY_COUNT; preChn++) {
            PDU_SetAdcPreTriggerDelayValue(base, chn, preChn, 0U);
        }
    }

    /* For Pulse out trigger. */
    base->POEN = 0U;
    for (chn = 0U; chn < PDU_POnDLY_COUNT; chn++) {
        base->POnDLY[chn].PODLY = 0U;
    }

    /* Load the setting value. */
    PDU_SetLoadValuesCmd(base);
    PDU_Disable(base);
}

/*
 *
 * Function Name : PDU_ConfigTimer.
 * Description   : Configure the PDU timer.
 * 
 */
void PDU_ConfigTimer(pdu_type_t *const base, const pdu_timer_config_t *configPtr)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(configPtr != NULL);
    uint32_t sc;

    sc = base->SC;
    sc &= ~((uint32_t)PDU_SC_LDMOD_MASK | (uint32_t)PDU_SC_PDUEIE_MASK | (uint32_t)PDU_SC_PRESCALER_MASK |
            (uint32_t)PDU_SC_TRGSEL_MASK | (uint32_t)PDU_SC_MULT_MASK | (uint32_t)PDU_SC_CONT_MASK |
            (uint32_t)PDU_SC_DMAEN_MASK | (uint32_t)PDU_SC_PDUIE_MASK);

    sc |= PDU_SC_LDMOD((uint32_t)(configPtr->loadValueMode));
    if (configPtr->seqErrIntEnable) {
        sc |= PDU_SC_PDUEIE_MASK;
    }
    sc |= PDU_SC_PRESCALER((uint32_t)(configPtr->clkPreDiv));
    sc |= PDU_SC_TRGSEL((uint32_t)(configPtr->triggerInput));
    sc |= PDU_SC_MULT((uint32_t)(configPtr->clkPreMultFactor));
    if (configPtr->continuousModeEnable) {
        sc |= PDU_SC_CONT_MASK;
    }
    if (configPtr->dmaEnable) {
        sc |= PDU_SC_DMAEN_MASK;
    }
    if (configPtr->intEnable) {
        sc |= PDU_SC_PDUIE_MASK;
    }

    base->SC = sc;
}

/*
 *
 * Function Name : PDU_SetAdcPreTriggerBackToBackEnable.
 * Description   : Switches to enable the pre-trigger back-to-back mode.
 * 
 */
void PDU_SetAdcPreTriggerBackToBackEnable(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask, bool enable)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(chn < PDU_CH_COUNT);

    uint32_t c1 = base->CH[chn].C1;
    if (enable) {
        c1 |= PDU_C1_BB(preChnMask);
    } else {
        c1 &= ~PDU_C1_BB(preChnMask);
    }

    base->CH[chn].C1 = c1;
}

/*
 *
 * Function Name : PDU_SetAdcPreTriggerOutputEnable.
 * Description   : Switches to enable the pre-trigger output.
 * 
 */
void PDU_SetAdcPreTriggerOutputEnable(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask, bool enable)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(chn < PDU_CH_COUNT);

    uint32_t c1 = base->CH[chn].C1;
    if (enable) {
        c1 |= PDU_C1_TOS(preChnMask);
    } else {
        c1 &= ~PDU_C1_TOS(preChnMask);
    }

    base->CH[chn].C1 = c1;
}

/*
 *
 * Function Name : PDU_SetAdcPreTriggerEnable.
 * Description   : Switches to enable the pre-trigger.
 * 
 */
void PDU_SetAdcPreTriggerEnable(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask, bool enable)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(chn < PDU_CH_COUNT);

    uint32_t c1 = base->CH[chn].C1;
    if (enable) {
        c1 |= PDU_C1_EN(preChnMask);
    } else {
        c1 &= ~PDU_C1_EN(preChnMask);
    }

    base->CH[chn].C1 = c1;
}

/*
 *
 * Function Name : PDU_ClearAdcPreTriggerFlags.
 * Description   : Clears the flag which indicates that the PDU counter has reached the pre-trigger delay value.
 * 
 */
void PDU_ClearAdcPreTriggerFlags(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(chn < PDU_CH_COUNT);

    /* Write 0 to clear. */
    uint32_t s = base->CH[chn].S;
    s &= ~PDU_S_CF(preChnMask); // Update the change.

    base->CH[chn].S = s;
}

/*
 *
 * Function Name : PDU_ClearAdcPreTriggerSeqErrFlags.
 * Description   : Clears the flag which indicates that a sequence error has been detected.
 * 
 */
void PDU_ClearAdcPreTriggerSeqErrFlags(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(chn < PDU_CH_COUNT);
    volatile uint32_t dummy_read;

    /* Write 0 to clear. */
    uint32_t s = base->CH[chn].S;
    s &= ~PDU_S_ERR(preChnMask);

    base->CH[chn].S = s;

    /* This read-after-write guarantees that the write to clear operation is completed,
     * for the case when memory write buffering is enabled. */
    dummy_read = base->CH[chn].S;
    (void)dummy_read;
}

/*
 *
 * Function Name : PDU_SetCmpPulseOutEnable.
 * Description   : Switches to enable the pulse-out trigger
 * 
 */
void PDU_SetCmpPulseOutEnable(pdu_type_t *const base, uint32_t pulseChnMask, bool enable)
{
    OS_ASSERT(base != NULL);
    uint32_t poen = base->POEN;
    if (enable) {
        poen |= PDU_POEN_POEN(pulseChnMask);
    } else {
        poen &= ~PDU_POEN_POEN(pulseChnMask);
    }

    base->POEN = poen;
}
