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
 * @file cmu_hw_access.c
 * @brief 
 * @author abu (abu@eswincomputing.com)
 * @date 2023-05-23
 * 
 * Modification History :
 * Date:               Version:                    Author:     
 * Changes: 
 * 
 */

#include "cmu_hw_access.h"

/*
 * Function Name : CMU_Set_ThHval
 * Description   : Set the CMU threshold high value register.
 */
void CMU_Set_ThHval(cmu_type_t *base, uint32_t hval)
{
    base->CMU_TH_HVAL = hval;
    return;
}

/*
 * Function Name : CMU_Set_ThLval
 * Description   : Set the CMU threshold low value register.
 */
void CMU_Set_ThLval(cmu_type_t *base, uint32_t lval)
{
    base->CMU_TH_LVAL = lval;
    return;
}

/*
 * Function Name : CMU_Get_MeasDone
 * Description   : Get the CMU measure done register.
 */
uint32_t CMU_Get_MeasDone(cmu_type_t *base)
{
    return base->CMU_MON_MEAS_DONE;
}

/*
 * Function Name : CMU_Set_ItAbnormal_Mask
 * Description   : Enable/disable the abnormal interrupt bit.
 */
void CMU_Set_ItAbnormal_Mask(cmu_common_type_t *base, uint32_t instance, bool enable)
{
    uint32_t tmp = (base->CMU_IT_MASK) & (~CMU_IT_ABNORMAL_MASK_MASK(instance));
    tmp |= (uint32_t)enable << CMU_IT_ABNORMAL_MASK_SHIFT(instance);
    (base->CMU_IT_MASK) = tmp;
    return;
}

/*
 * Function Name : CMU_Set_ItNormal_Mask
 * Description   : Enable/disable the normal interrupt bit.
 */
void CMU_Set_ItNormal_Mask(cmu_common_type_t *base, uint32_t instance, bool enable)
{
    uint32_t tmp = (base->CMU_IT_MASK) & (~CMU_IT_NORMAL_MASK_MASK(instance));
    tmp |= (uint32_t)enable << CMU_IT_NORMAL_MASK_SHIFT(instance);
    (base->CMU_IT_MASK) = tmp;
    return;
}

/*
 * Function Name : CMU_Clear_ItAbnormal
 * Description   : Clear the abnormal interrupt bit.
 */
void CMU_Clear_ItAbnormal(cmu_common_type_t *base, uint32_t instance)
{
    base->CMU_IT_CLEAR |= CMU_IT_ABNORMAL_CLEAR_MASK(instance);
    return;
}

/*
 * Function Name : CMU_Clear_ItNormal
 * Description   : Clear the normal interrupt bit.
 */
void CMU_Clear_ItNormal(cmu_common_type_t *base, uint32_t instance)
{
    base->CMU_IT_CLEAR |= CMU_IT_NORMAL_STATUS_MASK(instance);
    return;
}

/*
 * Function Name : CMU_Get_ItStatus
 * Description   : Get the measure status.
 */
uint32_t CMU_Get_ItStatus(cmu_common_type_t *base)
{
    return base->CMU_IT_STATUS;
}

/*
 * Function Name : CMU_Enable
 * Description   : Enable/disable the CMU.
 */
void CMU_Enable(cmu_common_type_t *base, uint32_t instance, bool enable)
{
    uint32_t tmp = (base->CMU_EN) & (~CMU_EN_MASK(instance));
    tmp |= (uint32_t)enable << CMU_EN_SHIFT(instance);
    (base->CMU_EN) = tmp;
    return;
}