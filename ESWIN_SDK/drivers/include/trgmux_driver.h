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
 * @file trgmux_driver.h
 * @brief TRGMUX driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __TRGMUX_DRIVER_H__
#define __TRGMUX_DRIVER_H__

#include "emps_platform.h"
#include "clock_driver.h"
#include "status.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup TRGMUX_Driver TRGMUX Driver
 * @{
 */

/**
 * @brief Configuration structure for pairing source triggers with target modules.
 */
typedef struct {
    trgmux_trigger_source_t euTriggerSource; /**< Selects one of the TRGMUX trigger sources. \D{TRGMUX_TRIG_SOURCE_VDD} */
    trgmux_target_module_t euTargetModule;   /**< Selects one of the TRGMUX target modules. \D{TRGMUX_TARGET_MODULE_MOTO_PITMR_TRG_CH0} */
    bool lockTargetModuleReg;                /**< If true, the LOCK bit of the target module register will be
                                                set by TRGMUX_DRV_INIT(), after the current mapping is configured. \D{false} */
} trgmux_inout_mapping_config_t;

/**
 * @brief User configuration structure for the TRGMUX driver.
 */
typedef struct {
    uint8_t numInOutMappingConfigs;                             /**< Number of in-out mappings defined in TRGMUX configuration. \D{1} */
    trgmux_inout_mapping_config_t *pstTrgmuxInOutMappingConfig; /**< Pointer to array of in-out mapping
                                                                    structures. \D{g_stTrgmuxInoutMappingConfig} */
} trgmux_user_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initialize a TRGMUX instance for operation.
 *
 * This function first resets the source triggers of all TRGMUX target modules to their default values,
 * then configures the TRGMUX with all the user defined in-out mappings.
 * If at least one of the target modules is locked, the function will not change any of the TRGMUX target modules
 * and return error code.
 *
 * @param  instance The TRGMUX instance number(0).
 * @param  pstTrgmuxUserConfig Pointer to the user configuration structure.
 * @return status_t Execution status:
 *                  STATUS_SUCCESS: Succeed.
 *                  STATUS_ERROR:   If at least one of the target module register is locked.
 */
status_t TRGMUX_DRV_Init(uint32_t instance, const trgmux_user_config_t *pstTrgmuxUserConfig);

/**
 * @brief Reset to default values the source triggers corresponding to all target modules,
 * if none of the target modules is locked.
 *
 * @param  instance The TRGMUX instance number(0).
 * @return status_t Execution status:
 *                  STATUS_SUCCESS: Succeed.
 *                  STATUS_ERROR:   If at least one of the target module register is locked.
 */
status_t TRGMUX_DRV_Deinit(uint32_t instance);

/**
 * @brief Configure a source trigger for a selected target module.
 *
 * This function configures a TRGMUX link between a source trigger and a target module,
 * if the requested target module is not locked.
 *
 * @param  instance The TRGMUX instance number(0).
 * @param  euTriggerSource One of the values in the trgmux_trigger_source_t enumeration.
 * @param  euTargetModule One of the values in the trgmux_target_module_t enumeration.
 * @return status_t Execution status:
 *                  STATUS_SUCCESS: Succeed.
 *                  STATUS_ERROR:   If at least one of the target module register is locked.
 */
status_t TRGMUX_DRV_SetTrigSourceForTargetModule(uint32_t instance, trgmux_trigger_source_t euTriggerSource,
                                                 trgmux_target_module_t euTargetModule);

/**
 * @brief Get the source trigger configured for a target module.
 *
 * This function returns the TRGMUX source trigger linked to a selected target module.
 *
 * @param  instance The TRGMUX instance number(0).
 * @param  euTargetModule One of the values in the trgmux_target_module_t enumeration.
 * @return trgmux_trigger_source_t Enum value corresponding to the trigger source configured
 * for the selected target module.
 */
trgmux_trigger_source_t TRGMUX_DRV_GetTrigSourceForTargetModule(uint32_t instance, trgmux_target_module_t euTargetModule);

/**
 * @brief Locks the TRGMUX register of a target module.
 *
 * This function sets the LK bit of the TRGMUX register corresponding to
 * the selected target module. Please note that some TRGMUX registers can contain up to 4
 * SEL bitfields, meaning that these registers can be used to configure up to 4 target
 * modules independently. Because the LK bit is only one per register, the configuration
 * of all target modules referred from that register will be locked.
 *
 * @param  instance The TRGMUX instance number(0).
 * @param  euTargetModule One of the values in the trgmux_target_module_t enumeration.
 */
void TRGMUX_DRV_SetLockForTargetModule(uint32_t instance, trgmux_target_module_t euTargetModule);

/**
 * @brief Get the Lock bit status of the TRGMUX register of a target module.
 *
 * This function gets the value of the LK bit from the TRGMUX register corresponding to
 * the selected target module.
 *
 * @param  instance The TRGMUX instance number(0).
 * @param  euTargetModule One of the values in the trgmux_target_module_t enumeration.
 * @return true: if the selected euTargetModule register is locked.
 * @return false: if the selected euTargetModule register is not locked.
 */
bool TRGMUX_DRV_GetLockForTargetModule(uint32_t instance, trgmux_target_module_t euTargetModule);

/**
 * @brief This function uses a SIM register in order to generate software
 * triggers to the target peripherals selected in TRGMUX.
 *
 * @param  instance The TRGMUX instance number(0).
 */
void TRGMUX_DRV_GenSWTrigger(uint32_t instance);

/**
 * @brief This function disable SIM software trigger in TRGMUX.
 *
 * @param  instance The TRGMUX instance number(0).
 */
void TRGMUX_DRV_ClearSWTrigger(uint32_t instance);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group TRGMUX_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __TRGMUX_DRIVER_H__ */
