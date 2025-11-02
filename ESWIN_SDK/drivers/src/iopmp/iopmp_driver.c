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
 * @file iopmp_driver.c
 * @brief IOPMP driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "iopmp_hw_access.h"
#include <stddef.h>

/**
 * @brief Table of base addresses for IOPMP instances.
 */
static iopmp_type_t *const s_iopmpBase[IOPMP_INSTANCE_COUNT] = IOPMP_BASE_PTRS;

/**
 * @brief Table of IRQs for IOPMP instances.
 */
static const IRQn_Type s_iopmpIrqId[IOPMP_INSTANCE_COUNT] = IOPMP_IRQS;

/**
 *
 * Function Name : IOPMP_DRV_IRQHandler
 * Description   : IRQ Handler.
 *
 */
static void IOPMP_DRV_IRQHandler(void *data)
{
    iopmp_access_err_info_t errInfo;
    iopmp_state_t *state = (iopmp_state_t *)data;
    if (state != NULL) {
        iopmp_type_t *base = s_iopmpBase[state->instance];
        IOPMP_DRV_GetDetailErrorAccessInfo(state->instance, &errInfo);
        IOPMP_ClearInterruptStatus(base);
        if (state->callback != NULL) {
            state->callback(state->callbackParam, errInfo);
        }
    }
}

/**
 *
 * Function Name : IOPMP_DRV_Init
 * Description   : Initializes the memory protection unit by setting the access
 * configurations of all available masters, The user must make sure that the clock is enabled.
 *
 */
status_t IOPMP_DRV_Init(uint32_t instance, uint8_t regionCnt, const iopmp_user_config_t *userConfigArr,
                        iopmp_state_t *state)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);
    OS_ASSERT((regionCnt > 0U) && (regionCnt <= IOPMP_REGION_COUNT));
    OS_ASSERT(userConfigArr != NULL);

    iopmp_type_t *base = s_iopmpBase[instance];
    status_t retStatus = STATUS_SUCCESS;
    uint8_t regionNum  = 0U;

    /* De-initializes all region descriptor */
    IOPMP_DRV_Deinit(instance);

    for (regionNum = 0U; regionNum < regionCnt; regionNum++) {
        /* Sets region configuration */
        retStatus = IOPMP_DRV_SetRegionConfig(instance, regionNum, &userConfigArr[regionNum]);

        if (retStatus != STATUS_SUCCESS) {
            break;
        }
    }

    if (retStatus == STATUS_SUCCESS) {
        /* Enable Intr */
        state->instance = instance;
        register_type_t type;
        type.lvl       = 1;
        type.priority  = 0;
        type.data_ptr  = (void *)state;
        type.trig_mode = CLIC_LEVEL_TRIGGER;
        OS_RequestIrq(s_iopmpIrqId[instance], IOPMP_DRV_IRQHandler, &type);
        OS_EnableIrq(s_iopmpIrqId[instance]);
        IOPMP_FaultInterruptEnable(base, true);
        /* Enables the IOPMP module operation */
        IOPMP_Enable(base, true);
        // IOPMP_LutLockEnable(base, true);
    }

    return retStatus;
}

/**
 *
 * Function Name : IOPMP_DRV_Deinit
 * Description   : De-initializes the memory protection unit by reseting
 * all regions to default and disable module.
 *
 */
void IOPMP_DRV_Deinit(uint32_t instance)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);

    iopmp_type_t *base = s_iopmpBase[instance];
    /* Disables the entire IOPMP module */
    IOPMP_Enable(base, false);

    /* Clears slave error flag */
    IOPMP_ClearErrorFlag(base);
}

/**
 *
 * Function Name : IOPMP_DRV_SetRegionAddr
 * Description   : Sets the region start address and end address.
 *
 */
void IOPMP_DRV_SetRegionAddr(uint32_t instance, uint8_t regionNum, uint32_t startAddr)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);
    OS_ASSERT(regionNum < IOPMP_REGION_COUNT);

    iopmp_type_t *base = s_iopmpBase[instance];

    /* Sets a region's start and end addresses */
    IOPMP_SetRegionAddr(base, regionNum, startAddr);
}

/**
 *
 * Function Name : IOPMP_DRV_SetRegionConfig
 * Description   : Sets the region configuration.
 * Updates the access configuration of all available masters and 
 * memory location in a given region.
 *
 */
status_t IOPMP_DRV_SetRegionConfig(uint32_t instance, uint8_t regionNum, const iopmp_user_config_t *userConfigPtr)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);
    OS_ASSERT(regionNum < IOPMP_REGION_COUNT);
    OS_ASSERT(userConfigPtr != NULL);

    iopmp_type_t *base  = s_iopmpBase[instance];
    uint8_t masterIdx   = 0U;
    status_t result = STATUS_SUCCESS;

    /* Sets access right for masters */
    for (masterIdx = 0U; masterIdx < IOPMP_MASTER_COUNT; masterIdx++) {
        if (userConfigPtr->masterAccRight[masterIdx].masterNum <= IOPMP_MASTER_COUNT) {
            IOPMP_SetMasterAccessRight(base, regionNum, &userConfigPtr->masterAccRight[masterIdx]);
        } else {
            result = STATUS_ERROR;
            break;
        }
    }

    /* Configures address, process identifier except region 0 */
    if (result == STATUS_SUCCESS) {
        /* Sets a region's start and end addresses */
        IOPMP_SetRegionAddr(base, regionNum, userConfigPtr->startAddr);
        /* Enables the region descriptor valid bit */
        IOPMP_SetRegionLockState(base, regionNum, userConfigPtr->addrLock);
    }

    return result;
}

/**
 *
 * Function Name : IOPMP_DRV_SetMasterAccessRights
 * Description   : Configures access permission for bus master in region.
 *
 */
status_t IOPMP_DRV_SetMasterAccessRights(uint32_t instance, uint8_t regionNum,
                                         const iopmp_master_access_right_t *accessRightsPtr)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);
    OS_ASSERT(regionNum < IOPMP_REGION_COUNT);
    OS_ASSERT(accessRightsPtr != NULL);

    iopmp_type_t *base  = s_iopmpBase[instance];
    status_t result = STATUS_SUCCESS;

    /* Sets access rights for master */
    if (accessRightsPtr->masterNum <= IOPMP_MASTER_COUNT) {
        IOPMP_SetMasterAccessRight(base, regionNum, accessRightsPtr);
    } else {
        result = STATUS_ERROR;
    }

    return result;
}

/**
 *
 * Function Name : IOPMP_DRV_GetDetailErrorAccessInfo
 * Description   : Checks and gets the IOPMP access error detail information
 * for a slave port. Clears bus error flag if an error occurs.
 *
 */
bool IOPMP_DRV_GetDetailErrorAccessInfo(uint32_t instance, iopmp_access_err_info_t *errInfoPtr)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);
    OS_ASSERT(errInfoPtr != NULL);

    iopmp_type_t *base = s_iopmpBase[instance];
    bool errorStatus   = false;

    /* Gets slaver port error status */
    errorStatus = IOPMP_GetErrorStatus(base);

    /* Check if there is access violation in the slave port */
    if (errorStatus) {
        /* Get the slave port detail error */
        IOPMP_GetErrorInfo(base, errInfoPtr);

        /* Clears slave port error flag */
        IOPMP_ClearErrorFlag(base);
    }

    return errorStatus;
}

/**
 *
 * Function Name : IOPMP_DRV_EnableMasterRegion
 * Description   : Enables/Disables region descriptor.
 * Please note that region 0 should not be disabled.
 *
 */
void IOPMP_DRV_EnableMasterRegion(uint32_t instance, uint8_t regionNum, uint8_t master, bool enable)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);
    OS_ASSERT(regionNum < IOPMP_REGION_COUNT);
    OS_ASSERT(regionNum > 0U);

    iopmp_type_t *base = s_iopmpBase[instance];

    /* Enables/Disables region descriptor */
    IOPMP_SetMasterRegionValidCmd(base, regionNum, master, enable);
}

/**
 *
 * Function Name : IOPMP_DRV_LockMasterRegion
 * Description   : Lock master region descriptor.
 * Please note that region 0 should not be disabled.
 *
 */
void IOPMP_DRV_LockMasterRegion(uint32_t instance, uint8_t regionNum, uint8_t master)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);
    OS_ASSERT(regionNum < IOPMP_REGION_COUNT);
    OS_ASSERT(regionNum > 0U);

    iopmp_type_t *base = s_iopmpBase[instance];

    /* Lock region descriptor */
    IOPMP_SetMasterRegionLockState(base, regionNum, master, true);
}

/**
 *
 * Function Name : IOPMP_DRV_LockRegion
 * Description   : Lock region descriptor.
 * Please note that region 0 should not be disabled.
 *
 */
void IOPMP_DRV_LockRegion(uint32_t instance, uint8_t regionNum)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);
    OS_ASSERT(regionNum < IOPMP_REGION_COUNT);
    OS_ASSERT(regionNum > 0U);

    iopmp_type_t *base = s_iopmpBase[instance];

    /* Lock region descriptor */
    IOPMP_SetRegionLockState(base, regionNum, true);
}

/**
 *
 * Function Name : IOPMP_DRV_LockLut
 * Description   : Lock region descriptor.
 * Please note that region 0 should not be disabled.
 *
 */
void IOPMP_DRV_LockLut(uint32_t instance)
{
    OS_ASSERT(instance < IOPMP_INSTANCE_COUNT);

    iopmp_type_t *base = s_iopmpBase[instance];

    /* Lock region descriptor */
    IOPMP_LutLockEnable(base, true);
}

/**
 *
 * Function Name : IOPMP_DRV_SetMaster
 * Description   : Set Master.
 *
 */
status_t IOPMP_DRV_SetMaster(uint8_t master)
{
    OS_ASSERT(master < IOPMP_MASTER_COUNT);

    tcsr_type_t *const tcsrBase = TCSR;
    tcsrBase->MPUMASTERID       = master;

    return STATUS_SUCCESS;
}
