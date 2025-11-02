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
 * @file iopmp_hw_access.c
 * @brief IOPMP driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "iopmp_hw_access.h"

/**
 *
 * Function Name : IOPMP_SetMasterAccessRight
 * Description   : Sets access permission for master in region descriptor.
 *
 */
void IOPMP_SetMasterAccessRight(iopmp_type_t *const base, uint8_t regionNum,
                                const iopmp_master_access_right_t *masterAccRight)
{
    OS_ASSERT(masterAccRight != NULL);
    uint32_t masterNum = masterAccRight->masterNum;

    uint32_t regionSize = 31 - __builtin_clz((masterAccRight->regionSize + 1) & (uint32_t)(~1));

    uint32_t tmp = IOPMP_ATTR_LOCK_REGION_EN(1U) | IOPMP_ATTR_LOCK_REGION_SIZE(regionSize - 1) |
                   IOPMP_ATTR_LOCK_REGION_AP(masterAccRight->accessRight);

    base->REGION_CFG[regionNum].ATTR_LOCK[masterNum] = tmp;
    tmp = tmp | IOPMP_ATTR_LOCK_REGION_LOCK(masterAccRight->regionLock ? 1UL : 0UL);
    base->REGION_CFG[regionNum].ATTR_LOCK[masterNum] = tmp;
}

/**
 *
 * Function Name : IOPMP_GetErrorInfo
 * Description   : Gets IOPMP detail error access info.
 *
 */
void IOPMP_GetErrorInfo(const iopmp_type_t *const base, iopmp_access_err_info_t *errInfoPtr)
{
    uint32_t tmp;

    /* Read the Error Detail Register */
    tmp = base->MPU_FAULT_STATUS;
    errInfoPtr->fault =
        (uint16_t)((tmp & (IOPMP_MPU_FALUT_STATUS_FAULT_000_MASK | IOPMP_MPU_FALUT_STATUS_FAULT_001_MASK |
                           IOPMP_MPU_FALUT_STATUS_FAULT_010_MASK | IOPMP_MPU_FALUT_STATUS_FAULT_101_MASK |
                           IOPMP_MPU_FALUT_STATUS_FAULT_11x_MASK)) >>
                   IOPMP_MPU_FALUT_STATUS_FAULT_000_SHIFT);

    /* Report Error Master Number to user */
    errInfoPtr->master =
        (uint8_t)((tmp & IOPMP_MPU_FALUT_STATUS_MASTER_ID_MASK) >> IOPMP_MPU_FALUT_STATUS_MASTER_ID_SHIFT);

    /* Report Error Address to user */
    errInfoPtr->addr = base->MPU_FAULT_ADDR;
}
