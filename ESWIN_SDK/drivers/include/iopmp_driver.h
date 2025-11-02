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
 * @file iopmp_driver.h
 * @brief IOPMP driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __IOPMP_DRIVER_H__
#define __IOPMP_DRIVER_H__

#include "emps_platform.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief IOPMP detail error access info.
 */
typedef struct {
    uint8_t master; /**< Access error master.     */
    uint16_t fault; /**< Access fault.            */
    uint32_t addr;  /**< Access error address.    */
} iopmp_access_err_info_t;

/**
 * @brief IOPMP access rights.
 */
typedef enum {
    IOPMP_MACHINE_NONE_USER_NONE = 0x00U,
    IOPMP_MACHINE_RW_USER_NONE   = 0x01U,
    IOPMP_MACHINE_RW_USER_R      = 0x02U,
    IOPMP_MACHINE_RW_USER_RW     = 0x03U,
    IOPMP_MACHINE_R_USER_NONE    = 0x05U,
    IOPMP_MACHINE_R_USER_R       = 0x06U,
} iopmp_access_rights_t;

/**
 * @brief IOPMP master access rights.
 */
typedef struct {
    uint8_t masterNum;                 /**< Master number. \D{0} */
    bool regionLock;                   /**< Lock region. \D{false} */
    uint32_t regionSize;               /**< Memory region size. \D{0} */
    iopmp_access_rights_t accessRight; /**< Access right. \D{IOPMP_MACHINE_RW_USER_RW} */
} iopmp_master_access_right_t;

/**
 * @brief IOPMP user region configuration structure.
 *
 * This structure is used when calling the IOPMP_DRV_Init function.
 */
typedef struct {
    bool addrLock;                                     /**< \D{false} */
    uint32_t startAddr;                                /**< Memory region start address. \D{0} */
    const iopmp_master_access_right_t *masterAccRight; /**< Access permission for masters. */
} iopmp_user_config_t;

/**
 * @brief Definition for the IOPMP callback function.
 *
 * Prototype for the callback function registered in the PMU driver.
 */
typedef void (*iopmp_callback_t)(void *parameter, iopmp_access_err_info_t errInfo);

/**
 * @brief IOPMP user region configuration structure.
 *
 * This structure is used when calling the IOPMP_DRV_Init function.
 */
typedef struct {
    uint32_t instance;
    void *callbackParam;       /**< callback parameters. */
    iopmp_callback_t callback; /**< The callback of interrupt. */
} iopmp_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief The function initializes the memory protection unit by setting the access configurations
 * of all available masters, process identifier and the memory location for the given regions;
 * and activate module finally.
 *
 * Please note that access rights for region 0 will always be configured and
 * regionCnt takes values between 1 and the maximum region count supported by the hardware.
 * The user must make sure that the clock is enabled.
 *
 * @param  instance The IOPMP peripheral instance number.
 * @param  regionCnt The number of configured regions.
 * @param  userConfigArr The pointer to the array of IOPMP user configure structure, \see iopmp_user_config_t.
 * @param  state The pointer to the IOPMP driver state structure.
 * @return status_t
 *              STATUS_SUCCESS:    Operation was successful.
 *              STATUS_ERROR:      Operation failed due to master number is out of range supported by hardware.
 */
status_t IOPMP_DRV_Init(uint32_t instance, uint8_t regionCnt, const iopmp_user_config_t *userConfigArr,
                        iopmp_state_t *state);

/**
 * @brief De-initializes the memory protection unit by reseting all regions to default
 * and disable module.
 *
 * @param  instance The IOPMP peripheral instance number.
 */
void IOPMP_DRV_Deinit(uint32_t instance);

/**
 * @brief Sets the region start and end address.
 *
 * @param  instance The IOPMP peripheral instance number.
 * @param  regionNum The region number.
 * @param  startAddr The region start address.
 */
void IOPMP_DRV_SetRegionAddr(uint32_t instance, uint8_t regionNum, uint32_t startAddr);

/**
 * @brief Sets the region configuration.
 * Updates the access configuration of all available masters,
 * process identifier and memory location in a given region.
 *
 * @param  instance The IOPMP peripheral instance number.
 * @param  regionNum The region number.
 * @param  userConfigPtr The region configuration structure pointer.
 * @return status_t
 *              STATUS_SUCCESS:    Operation was successful.
 *              STATUS_ERROR:      Operation failed due to master number is out of range supported by hardware.
 */
status_t IOPMP_DRV_SetRegionConfig(uint32_t instance, uint8_t regionNum, const iopmp_user_config_t *userConfigPtr);

/**
 * @brief Configures access permission for bus master in region.
 *
 * @param  instance The IOPMP peripheral instance number.
 * @param  regionNum The IOPMP region number.
 * @param  accessRightsPtr The pointer to access permission structure, \see iopmp_master_access_right_t.
 * @return status_t
 *              STATUS_SUCCESS:    Operation was successful.
 *              STATUS_ERROR:      Operation failed due to master number is out of range supported by hardware.
 */
status_t IOPMP_DRV_SetMasterAccessRights(uint32_t instance, uint8_t regionNum,
                                         const iopmp_master_access_right_t *accessRightsPtr);

/**
 * @brief Checks and gets the IOPMP access error detail information for a slave port.
 *  Clears bus error flag if an error occurs.
 *
 * @param  instance The IOPMP peripheral instance number.
 * @param  errInfoPtr The pointer to access error info structure, \see iopmp_access_err_info_t.
 * @return true: An error has occurred.
 * @return false: No error has occurred.
 */
bool IOPMP_DRV_GetDetailErrorAccessInfo(uint32_t instance, iopmp_access_err_info_t *errInfoPtr);

/**
 * @brief Enables/Disables region descriptor.
 * Please note that region 0 should not be disabled.
 *
 * @param  instance The IOPMP peripheral instance number.
 * @param  regionNum The region number.
 * @param  master The master number.
 * @param  enable enable or disable region.
 */
void IOPMP_DRV_EnableMasterRegion(uint32_t instance, uint8_t regionNum, uint8_t master, bool enable);

/**
 * @brief Lock master region descriptor.
 * Please note that region 0 should not be disabled.
 *
 * @param  instance The IOPMP peripheral instance number.
 * @param  regionNum The region number.
 * @param  master The master number.
 */
void IOPMP_DRV_LockMasterRegion(uint32_t instance, uint8_t regionNum, uint8_t master);

/**
 * @brief  Lock region descriptor.
 *
 * Please note that region 0 should not be disabled.
 *
 * @param  instance The IOPMP peripheral instance number.
 * @param  regionNum The region number.
 */
void IOPMP_DRV_LockRegion(uint32_t instance, uint8_t regionNum);

/**
 * @brief Lock LUT descriptor.
 *
 * Please note that region 0 should not be disabled.
 *
 * @param  instance The IOPMP peripheral instance number.
 */
void IOPMP_DRV_LockLut(uint32_t instance);

/**
 * @brief Sets the master.
 *
 * @param  master  The master number.
 * @return status_t
 * STATUS_SUCCESS:    Operation was successful.
 * STATUS_ERROR:      Operation failed due to master number is out of range supported by hardware.
 */
status_t IOPMP_DRV_SetMaster(uint8_t master);

#if defined(__cplusplus)
}
#endif

#endif
