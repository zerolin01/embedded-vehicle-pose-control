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
 * @file supertmr_hw_access.c
 * @brief SUPERTMR driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "supertmr_hw_access.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_InitModule
 * Description   : Initializes the SUPERTMR. This function will enable the superTimer module
 * and selects one pre-scale factor for the SUPERTMR clock source.
 *
 *END**************************************************************************/
void SUPERTMR_DRV_InitModule(supertmr_type_t *const supertmrBase, supertmr_clock_ps_t supertmrClockPrescaler)
{
    /* Use SUPERTMR mode */
    SUPERTMR_DRV_Enable(supertmrBase, true);
    SUPERTMR_DRV_SetClockPs(supertmrBase, supertmrClockPrescaler);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_Reset
 * Description   : Resets the SUPERTMR registers. All the register use in the driver should be
 * reset to default value of each register.
 *
 *END**************************************************************************/
void SUPERTMR_DRV_Reset(supertmr_type_t *const supertmrBase)
{
    uint8_t chnIndex;

    /* WPDIS is set when WPEN bit is read as a 1 and then 1 is written to WPDIS */
    ((supertmrBase)->FMS) &= 0U;
    /* This is the reset value for MODE register. WPDIS bit is set to disable write protection */
    ((supertmrBase)->MODE) = 0x00000004U;
    ((supertmrBase)->SC) &= 0U;
    ((supertmrBase)->CNT)   = 0U;
    ((supertmrBase)->MOD)   = 0U;
    ((supertmrBase)->CNTIN) = 0U;
    ((supertmrBase)->STATUS) &= 0U;
    ((supertmrBase)->SYNC)     = 0U;
    ((supertmrBase)->OUTINIT)  = 0U;
    ((supertmrBase)->OUTMASK)  = 0U;
    ((supertmrBase)->COMBINE)  = 0U;
    ((supertmrBase)->DEADTIME) = 0U;
    ((supertmrBase)->EXTTRIG) &= 0U;
    ((supertmrBase)->POL)        = 0U;
    ((supertmrBase)->FILTER)     = 0U;
    ((supertmrBase)->FLTCTRL)    = 0U;
    ((supertmrBase)->QDCTRL)     = 0U;
    ((supertmrBase)->CONF)       = 0U;
    ((supertmrBase)->FLTPOL)     = 0U;
    ((supertmrBase)->SYNCONF)    = 0U;
    ((supertmrBase)->INVCTRL)    = 0U;
    ((supertmrBase)->SWOCTRL)    = 0U;
    ((supertmrBase)->PWMLOAD)    = 0U;
    ((supertmrBase)->HCR)        = 0U;
    ((supertmrBase)->MOD_MIRROR) = 0U;

    /* Set to reset value all CnV and CnSC registers */
    for (chnIndex = 0; chnIndex < SUPERTMR_CHANNEL_COUNT; chnIndex++) {
        ((supertmrBase)->CONTROLS[chnIndex].CnSC) &= 0U;
        ((supertmrBase)->CONTROLS[chnIndex].CnV) = 0U;
        ((supertmrBase)->CV_MIRROR[chnIndex])    = 0U;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetChnTriggerCmd
 * Description   : Enables or disables the generation of the SUPERTMR peripheral timer channel trigger.
 * Enables or disables the generation of the SUPERTMR peripheral timer channel trigger when the
 * SUPERTMR counter is equal to its initial value.
 *
 *END**************************************************************************/
void SUPERTMR_DRV_SetChnTriggerCmd(supertmr_type_t *const supertmrBase, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    uint32_t value = 0U;

    if (channel < 2U) {
        value = (uint32_t)SUPERTMR_EXTTRIG_CH0TRIG_MASK << (uint32_t)channel;
    } else if (channel < 6U) {
        value = (uint32_t)SUPERTMR_EXTTRIG_CH2TRIG_MASK << ((uint32_t)(channel)-2U);
    } else {
        value = (uint32_t)SUPERTMR_EXTTRIG_CH6TRIG_MASK << ((uint32_t)(channel)-6U);
    }

    if (true == enable) {
        ((supertmrBase)->EXTTRIG) |= value;
    } else {
        ((supertmrBase)->EXTTRIG) &= ~value;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetChnInputCaptureFilter
 * Description   : Sets the SUPERTMR peripheral timer channel input capture filter value.
 *
 *END**************************************************************************/
void SUPERTMR_DRV_SetChnInputCaptureFilter(supertmr_type_t *const supertmrBase, uint8_t channel, uint8_t value)
{
    OS_ASSERT(CHAN4_IDX > channel);

    switch (channel) {
    case CHAN0_IDX:
        SUPERTMR_RMW_FILTER(supertmrBase, SUPERTMR_FILTER_CH0FVAL_MASK, SUPERTMR_FILTER_CH0FVAL(value));
        break;
    case CHAN1_IDX:
        SUPERTMR_RMW_FILTER(supertmrBase, SUPERTMR_FILTER_CH1FVAL_MASK, SUPERTMR_FILTER_CH1FVAL(value));
        break;
    case CHAN2_IDX:
        SUPERTMR_RMW_FILTER(supertmrBase, SUPERTMR_FILTER_CH2FVAL_MASK, SUPERTMR_FILTER_CH2FVAL(value));
        break;
    case CHAN3_IDX:
        SUPERTMR_RMW_FILTER(supertmrBase, SUPERTMR_FILTER_CH3FVAL_MASK, SUPERTMR_FILTER_CH3FVAL(value));
        break;
    default:
        /* Nothing to do */
        break;
    }
}
