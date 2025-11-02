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
 * @file cmu_hw_access.h
 * @brief 
 * @author abu (abu@eswincomputing.com)
 * @date 2023-05-23
 * 
 * Modification History :
 * Date:               Version:                    Author:     
 * Changes: 
 * 
 */

#ifndef __CMU_HW_ACCESS_H__
#define __CMU_HW_ACCESS_H__

#include "cmu_register.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Set the CMU threshold high value register.
 * 
 * @param  base Module base pointer of type cmu_type_t.
 * @param  hval high threshold value
 */
void CMU_Set_ThHval(cmu_type_t *base, uint32_t hval);

/**
 * @brief Set the CMU threshold low value register.
 * 
 * @param  base Module base pointer of type cmu_type_t.
 * @param  lval low threshold value
 */
void CMU_Set_ThLval(cmu_type_t *base, uint32_t lval);

/**
 * @brief Get the CMU measure done register.
 * 
 * @param  base Module base pointer of type cmu_type_t.
 * @return uint32_t register value
 */
uint32_t CMU_Get_MeasDone(cmu_type_t *base);

/**
 * @brief Enable/disable the abnormal interrupt bit.
 * 
 * @param  base Module base pointer of type cmu_type_t.
 * @param  instance The CMU instance index.
 * @param  enable mask or not
 */
void CMU_Set_ItAbnormal_Mask(cmu_common_type_t *base, uint32_t instance, bool enable);

/**
 * @brief Enable/disable the normal interrupt bit.
 * 
 * @param  base Module base pointer of type cmu_type_t.
 * @param  instance The CMU instance index.
 * @param  enable mask or not
 */
void CMU_Set_ItNormal_Mask(cmu_common_type_t *base, uint32_t instance, bool enable);

/**
 * @brief Clear the abnormal interrupt bit.
 * 
 * @param  base Module base pointer of type cmu_type_t.
 * @param  instance The CMU instance index.
 */
void CMU_Clear_ItAbnormal(cmu_common_type_t *base, uint32_t instance);

/**
 * @brief Clear the normal interrupt bit.
 * 
 * @param  base Module base pointer of type cmu_type_t.
 * @param  instance The CMU instance index.
 */
void CMU_Clear_ItNormal(cmu_common_type_t *base, uint32_t instance);

/**
 * @brief Get the measure status.
 * 
 * @param  base Module base pointer of type cmu_type_t.
 * @return uint32_t register value
 */
uint32_t CMU_Get_ItStatus(cmu_common_type_t *base);

/**
 * @brief Enable/disable the CMU.
 * 
 * @param  base Module base pointer of type cmu_type_t.
 * @param  instance The CMU instance index.
 * @param  enable enable or not
 */
void CMU_Enable(cmu_common_type_t *base, uint32_t instance, bool enable);

#if defined(__cplusplus)
}
#endif

#endif /* __CMU_HW_ACCESS_H__ */