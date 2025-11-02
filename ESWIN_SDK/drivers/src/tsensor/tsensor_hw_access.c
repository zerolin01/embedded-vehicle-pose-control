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
 * @file tsensor_hw_access.c
 * @brief TSENSOR driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-09
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "tsensor_hw_access.h"
#include <stddef.h>

/**
 *
 * Function Name : TSENSOR_SetTsensorEn
 * Description   : This function set the TSENSOR TsensorEn register.
 *
 */
void TSENSOR_SetTsensorEn(tsensor_type_t *const base)
{
    base->tsensorTsensorEn = TSENSOR_TSEN_EN_EN(0x1U);
}

/**
 *
 * Function Name : TSENSOR_GetTsensorEn
 * Description   : This function set the TSENSOR TsensorEn register.
 *
 */
uint32_t TSENSOR_GetTsensorEn(tsensor_type_t *const base)
{
    return (uint32_t)(base->tsensorTsensorEn);
}

/**
 *
 * Function Name : TSENSOR_ClearTsensorEn
 * Description   : This function clear the TSENSOR TsensorEn register.
 *
 */
void TSENSOR_ClearTsensorEn(tsensor_type_t *const base)
{
    base->tsensorTsensorEn = TSENSOR_TSEN_EN_EN(0x0U);
}

/**
 *
 * Function Name : TSENSOR_WriteRegPtat
 * Description   : This function write the TSENSOR regPtatLv register.
 *
 */
void TSENSOR_WriteRegPtat(tsensor_type_t *const base, uint8_t mode, uint8_t lv)
{
    uint32_t regValue = TSENSOR_ReadRegPtatLv(base);

    regValue &= ~(TSENSOR_REG_PTAT_LV_MASK);
    regValue |= TSENSOR_REG_PTAT_LV(lv);

    regValue &= ~(TSENSOR_REG_PTAT_MODE_MASK);
    regValue |= TSENSOR_REG_PTAT_MODE(mode);

    base->tsensorRegPtatLv = regValue;
}

/**
 *
 * Function Name : TSENSOR_ReadRegPtatLv
 * Description   : This function read the TSENSOR regPtatLv register.
 *
 */
uint32_t TSENSOR_ReadRegPtatLv(const tsensor_type_t *const base)
{
    return (uint32_t)(base->tsensorRegPtatLv);
}
