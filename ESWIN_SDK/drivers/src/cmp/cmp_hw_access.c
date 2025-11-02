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
 * @file cmp_hw_access.c
 * @brief CMP driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "cmp_hw_access.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_SetFunctionalMode
 * Description   : This function set CMP functional mode based on configuration
 * structure. If values for filter count or filter period are specific for selected mode
 * the value from input structure will be ignored.
 *
 *   FUNCTIONAL MODE                 | FILTER COUNT  | FILTER PERIOD
 *   ----------------------------------------------------------------
 *   CMP_DISABLED                    |    IGNORED    |    IGNORED
 *   CMP_CONTINUOUS                  |    IGNORED    |    IGNORED
 *   CMP_SAMPLED_NONFILTRED_INT_CLK  |    IGNORED    |    IGNORED
 *   CMP_SAMPLED_NONFILTRED_EXT_CLK  |    IGNORED    |     USED
 *   CMP_SAMPLED_FILTRED_INT_CLK     |     USED      |    IGNORED
 *   CMP_SAMPLED_FILTRED_EXT_CLK     |     USED      |     USED
 *   CMP_WINDOWED                    |    IGNORED    |    IGNORED
 *   CMP_WINDOWED_RESAMPLED          |    IGNORED    |     USED
 *   CMP_WINDOWED_FILTRED            |     USED      |     USED
 *
 *END**************************************************************************/
void CMP_SetFunctionalMode(cmp_type_t *baseAddr, cmp_mode_t mode, uint8_t filterSampleCount, uint8_t filterSamplePeriod)
{
    uint32_t tmp = baseAddr->C0;
    tmp = tmp & (~(CMP_C0_SE_MASK & CMP_C0_FSP_MASK & CMP_C0_FILTER_CNT_MASK & CMP_C0_EN_MASK & CMP_C0_WE_MASK));
    baseAddr->C0 = tmp;
    switch (mode) {
    case CMP_DISABLED: break;
    case CMP_CONTINUOUS:
        // tmp |= CMP_C0_EN(1U);
        break;
    case CMP_SAMPLED_NONFILTRED_EXT_CLK:
        //tmp |= CMP_C0_EN(1U);
        tmp |= CMP_C0_SE(1U);
        tmp |= CMP_C0_FILTER_CNT(1U);
        tmp |= CMP_C0_FSP(filterSamplePeriod);
        break;
    case CMP_SAMPLED_NONFILTRED_INT_CLK:
        // tmp |= CMP_C0_EN(1U);
        tmp |= CMP_C0_FILTER_CNT(1U);
        tmp |= CMP_C0_FSP(filterSamplePeriod);
        break;
    case CMP_SAMPLED_FILTRED_INT_CLK:
        // tmp |= CMP_C0_EN(1U);
        tmp |= CMP_C0_FILTER_CNT(filterSampleCount);
        tmp |= CMP_C0_FSP(filterSamplePeriod);
        break;
    case CMP_SAMPLED_FILTRED_EXT_CLK:
        // tmp |= CMP_C0_EN(1U);
        tmp |= CMP_C0_SE(1U);
        tmp |= CMP_C0_FILTER_CNT(filterSampleCount);
        tmp |= CMP_C0_FSP(filterSamplePeriod);
        break;
    case CMP_WINDOWED:
        // tmp |= CMP_C0_EN(1U);
        tmp |= CMP_C0_WE(1U);
        tmp |= CMP_C0_FILTER_CNT(0U);
        break;
    case CMP_WINDOWED_RESAMPLED:
        // tmp |= CMP_C0_EN(1U);
        tmp |= CMP_C0_WE(1U);
        tmp |= CMP_C0_FILTER_CNT(1U);
        tmp |= CMP_C0_FSP(filterSamplePeriod);
        break;
    case CMP_WINDOWED_FILTRED:
        // tmp |= CMP_C0_EN(1U);
        tmp |= CMP_C0_WE(1U);
        tmp |= CMP_C0_FILTER_CNT(filterSampleCount);
        tmp |= CMP_C0_FSP(filterSamplePeriod);
        break;
    default:
        /* Impossible case */
        break;
    }
    baseAddr->C0 = tmp;
}

/** Function Name : CMP_GetFunctionalMode
 * Description   : This function return current CMP functional mode. CMP module cand be used in
 * one of this modes: CMP_DISABLED , CMP_CONTINUOUS, CMP_SAMPLED_NONFILTRED_INT_CLK,
 * CMP_SAMPLED_NONFILTRED_EXT_CLK, CMP_SAMPLED_FILTRED_INT_CLK, CMP_SAMPLED_FILTRED_EXT_CLK,
 * CMP_WINDOWED, CMP_WINDOWED_RESAMPLED, CMP_WINDOWED_FILTRED
 *
 *END**************************************************************************/

cmp_mode_t CMP_GetFunctionalMode(const cmp_type_t *baseAddr)
{
    uint32_t state;
    uint32_t tmp    = baseAddr->C0;
    cmp_mode_t mode = CMP_DISABLED;
    state           = (((baseAddr->C0) & CMP_C0_EN_MASK) >> CMP_C0_EN_SHIFT) << 2U;
    state |= (((baseAddr->C0) & CMP_C0_WE_MASK) >> CMP_C0_WE_SHIFT) << 1U;
    state |= (((baseAddr->C0) & CMP_C0_SE_MASK) >> CMP_C0_SE_SHIFT);
    uint32_t filterSamplePeriod = (tmp & CMP_C0_FSP_MASK) >> CMP_C0_FSP_SHIFT;
    uint32_t filterSampleCount  = (tmp & CMP_C0_FILTER_CNT_MASK) >> CMP_C0_FILTER_CNT_SHIFT;

    switch (state) {
    case 4:
        if ((filterSamplePeriod == 0U) || (filterSampleCount == 0U)) {
            mode = CMP_CONTINUOUS;
        } else if (filterSampleCount == 1U) {
            mode = CMP_SAMPLED_NONFILTRED_INT_CLK;
        } else if (filterSampleCount > 1U) {
            mode = CMP_SAMPLED_FILTRED_INT_CLK;
        } else {
            /* Nothing */
        }
        break;
    case 5:
        if (filterSampleCount == 1U) {
            mode = CMP_SAMPLED_NONFILTRED_EXT_CLK;
        } else if (filterSampleCount > 1U) {
            mode = CMP_SAMPLED_FILTRED_EXT_CLK;
        } else {
            /* Nothing */
        }
        break;
    case 6:
        if ((filterSamplePeriod == 0U) || (filterSampleCount == 0U)) {
            mode = CMP_WINDOWED;
        } else if ((filterSampleCount == 1U) && (filterSamplePeriod > 0U)) {
            mode = CMP_WINDOWED_RESAMPLED;
        } else if ((filterSampleCount > 1U) && (filterSamplePeriod > 0U)) {
            mode = CMP_WINDOWED_FILTRED;
        } else {
            /* Nothing */
        }
        break;
    default:
        /* Impossible case */
        break;
    }

    if ((state & 0x4U) == 0U) {
        mode = CMP_DISABLED;
    }

    return mode;
}
