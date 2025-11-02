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
 * @file iopmp_hw_access.h
 * @brief IOPMP driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __IOPMP_HW_ACCESS_H__
#define __IOPMP_HW_ACCESS_H__

#include "iopmp_driver.h"
#include "iopmp_register.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Enables/Disables the IOPMP module.
 *
 * @param base The IOPMP peripheral base address.
 * @param enable State
 *            - true  : Enable the IOPMP module.
 *            - false : Disable the IOPMP module.
 */
static inline void IOPMP_Enable(iopmp_type_t *const base, bool enable)
{
    base->MPU_CTRL =
        (base->MPU_CTRL & ~IOPMP_MPU_CTRL_ATTR_REG_ENABLE_MASK) | IOPMP_MPU_CTRL_ATTR_REG_ENABLE(enable ? 1UL : 0UL);
}

/**
 * @brief Enables/Disables the IOPMP LUT lock.
 *
 * @param base The IOPMP peripheral base address.
 * @param enable State
 *            - true  : Enable the IOPMP LUT lock.
 *            - false : Disable the IOPMP LUT lock.
 */
static inline void IOPMP_LutLockEnable(iopmp_type_t *const base, bool enable)
{
    base->MPU_CTRL = (base->MPU_CTRL & ~IOPMP_MPU_CTRL_LUT_LOCK_MASK) | IOPMP_MPU_CTRL_LUT_LOCK(enable ? 1UL : 0UL);
}

/**
 * @brief Get the fault address.
 *
 * @param base The IOPMP peripheral base address.
 * @param regionNum The region number.
 * @param master The master number.
 * @return uint32_t the region descriptor lock state.
 *
 */
static inline uint32_t IOPMP_GetFaultAddress(const iopmp_type_t *const base)
{
    uint32_t tmp = base->MPU_FAULT_ADDR;
    tmp          = (tmp & IOPMP_MPU_FAULT_ADDR_MASK) >> IOPMP_MPU_FAULT_ADDR_SHIFT;

    return tmp;
}

/**
 * @brief Enables/Disables the fault interrupt.
 *
 * @param base The IOPMP peripheral base address.
 * @param enable State
 *            - true  : Enable the IOPMP fault interrupt.
 *            - false : Disable the IOPMP module interrupt.
 */
static inline void IOPMP_FaultInterruptEnable(iopmp_type_t *const base, bool enable)
{
    uint32_t tmp = base->MPU_FAULT_INT_ENABLE;
    tmp &= ~(IOPMP_MPU_FAULT_INT_STATUS_MASK);
    tmp |= IOPMP_MPU_FAULT_INT_STATUS(enable ? 1UL : 0UL);
    base->MPU_FAULT_INT_ENABLE = tmp;
}

/**
 * @brief Clears the error interrupt status.
 *
 * @param base The IOPMP peripheral base address.
 */
static inline void IOPMP_ClearInterruptStatus(iopmp_type_t *const base)
{
    base->MPU_FAULT_INT_STATUS = (base->MPU_FAULT_INT_STATUS);
}

/**
 * @brief Sets region start and end address.
 *
 * @param base The IOPMP peripheral base address.
 * @param regionNum IOPMP region number.
 * @param startAddr Region start address.
 */
static inline void IOPMP_SetRegionAddr(iopmp_type_t *const base, uint8_t regionNum, uint32_t startAddr)
{
    uint32_t tmp = base->REGION_CFG[regionNum].ADDR_LOCK;
    tmp &= ~(IOPMP_ADDR_LOCK_REG_BASE_ADDR_MASK);
    tmp |= IOPMP_ADDR_LOCK_REG_BASE_ADDR(startAddr);
    base->REGION_CFG[regionNum].ADDR_LOCK = tmp;
}

/**
 * @brief Sets access permission for master in region descriptor.
 *
 * @param base The IOPMP peripheral base address.
 * @param regionNum The region number.
 * @param masterAccRight Pointer to master number.
 */
void IOPMP_SetMasterAccessRight(iopmp_type_t *const base, uint8_t regionNum,
                                const iopmp_master_access_right_t *masterAccRight);

/**
 * @brief Get the master region descriptor lock state.
 *
 * @param base The IOPMP peripheral base address.
 * @param regionNum The region number.
 * @param master The master number.
 * @return the region descriptor lock state.
 *
 */
static inline bool IOPMP_GetRegionLockState(const iopmp_type_t *const base, uint8_t regionNum)
{
    uint32_t tmp = base->REGION_CFG[regionNum].ADDR_LOCK;
    tmp          = (tmp & IOPMP_ADDR_LOCK_REG_LOCK_MASK) >> IOPMP_ADDR_LOCK_REG_LOCK_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Set the master region descriptor lock state.
 *
 * @param base The IOPMP peripheral base address.
 * @param regionNum The region number.
 * @param enable Valid state
 *            - true  : Region descriptor is lock.
 *            - false : Region descriptor is unlock.
 */
static inline void IOPMP_SetRegionLockState(iopmp_type_t *const base, uint8_t regionNum, bool enable)
{
    uint32_t tmp = base->REGION_CFG[regionNum].ADDR_LOCK;
    tmp &= ~(IOPMP_ADDR_LOCK_REG_LOCK_MASK);
    tmp |= IOPMP_ADDR_LOCK_REG_LOCK(enable ? 1UL : 0UL);
    base->REGION_CFG[regionNum].ADDR_LOCK = tmp;
}

/**
 * @brief Enables/Disables the region descriptor.
 *
 * @param base The IOPMP peripheral base address.
 * @param regionNum The region number.
 * @param enable Valid state
 *            - true  : Region descriptor is valid.
 *            - false : Region descriptor is invalid.
 */
static inline void IOPMP_SetMasterRegionValidCmd(iopmp_type_t *const base, uint8_t regionNum, uint8_t master,
                                                 bool enable)
{
    uint32_t tmp = base->REGION_CFG[regionNum].ATTR_LOCK[master];
    tmp &= ~(IOPMP_ATTR_LOCK_REGION_EN_MASK);
    tmp |= IOPMP_ATTR_LOCK_REGION_EN(enable ? 1UL : 0UL);
    base->REGION_CFG[regionNum].ATTR_LOCK[master] = tmp;
}

/**
 * @brief Get the master region descriptor lock state.
 *
 * @param base The IOPMP peripheral base address.
 * @param regionNum The region number.
 * @param master The master number.
 * @return the region descriptor lock state.
 *
 */
static inline bool IOPMP_GetMasterRegionLockState(const iopmp_type_t *const base, uint8_t regionNum, uint8_t master)
{
    uint32_t tmp = base->REGION_CFG[regionNum].ATTR_LOCK[master];
    tmp          = (tmp & IOPMP_ATTR_LOCK_REGION_LOCK_MASK) >> IOPMP_ATTR_LOCK_REGION_LOCK_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Set the master region descriptor lock state.
 *
 * @param base The IOPMP peripheral base address.
 * @param regionNum The region number.
 * @param enable Valid state
 *            - true  : Region descriptor is lock.
 *            - false : Region descriptor is unlock.
 */
static inline void IOPMP_SetMasterRegionLockState(iopmp_type_t *const base, uint8_t regionNum, uint8_t master,
                                                  bool enable)
{
    uint32_t tmp = base->REGION_CFG[regionNum].ATTR_LOCK[master];
    tmp &= ~(IOPMP_ATTR_LOCK_REGION_LOCK_MASK);
    tmp |= IOPMP_ATTR_LOCK_REGION_LOCK(enable ? 1UL : 0UL);
    base->REGION_CFG[regionNum].ATTR_LOCK[master] = tmp;
}

/**
 * @brief Gets the error status of a specified slave port.
 *
 * @param base The IOPMP peripheral base address.
 * @return The error status:
 */
static inline bool IOPMP_GetErrorStatus(const iopmp_type_t *const base)
{
    uint32_t tmp = base->MPU_FAULT_STATUS;
    tmp          = (tmp & IOPMP_MPU_FALUT_STATUS_MASK) >> IOPMP_MPU_FALUT_STATUS_SHIFT;
    return (tmp != 0U) ? true : false;
}

/**
 * @brief Clears the fault flag.
 *
 * @param base The IOPMP peripheral base address.
 */
static inline void IOPMP_ClearErrorFlag(iopmp_type_t *const base)
{
    base->MPU_FAULT_STATUS = (base->MPU_FAULT_STATUS | IOPMP_MPU_FALUT_STATUS_ILLEGAL_MASK);
}

/**
 * @brief Reports the IOPMP access error detail information of error slave port.
 *
 * @param base The IOPMP peripheral base address.
 * @param errInfoPtr The pointer to the IOPMP access error information.
 */
void IOPMP_GetErrorInfo(const iopmp_type_t *const base, iopmp_access_err_info_t *errInfoPtr);

#if defined(__cplusplus)
}
#endif

#endif
