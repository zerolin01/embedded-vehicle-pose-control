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
 * @file trgmux_driver.c
 * @brief TRGMUX driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "trgmux_driver.h"
#include "trgmux_hw_access.h"
#include "clock_driver.h"
#include <stddef.h>

/** Table of base addresses for TRGMUX instances. */
static trgmux_type_t *const s_trgmuxBase[TRGMUX_INSTANCE_COUNT] = TRGMUX_BASE_PTRS;

/**
 *
 * Function Name : TRGMUX_DRV_Init
 * Description   : This function first resets the source triggers of all TRGMUX target modules
 * to their default values, then configures the TRGMUX with all the user defined in-out mappings.
 * If at least one of the target modules is locked, the function will not change any of the
 * TRGMUX target modules and return an error code.
 *
 */
status_t TRGMUX_DRV_Init(uint32_t instance, const trgmux_user_config_t *pstTrgmuxUserConfig)
{
    OS_ASSERT(instance < TRGMUX_INSTANCE_COUNT);
    OS_ASSERT(NULL != pstTrgmuxUserConfig);

    CLOCK_SYS_ClockEnable(TRGMUX_CLK, true);

    status_t status;
    trgmux_type_t *base = s_trgmuxBase[instance];
    uint8_t count;

    /* Reset source triggers of all TRGMUX target modules to default. */
    status = TRGMUX_Init(instance, base);

    if (STATUS_SUCCESS == status) {
        /* Loop through all in-out mappings in the configuration and apply them in TRGMUX. */
        for (count = 0U; count < pstTrgmuxUserConfig->numInOutMappingConfigs; count++) {
            TRGMUX_SetTrigSourceForTargetModule(base,
                                                pstTrgmuxUserConfig->pstTrgmuxInOutMappingConfig[count].euTriggerSource,
                                                pstTrgmuxUserConfig->pstTrgmuxInOutMappingConfig[count].euTargetModule);
        }

        /* Loop through all in-out mappings in the configuration and lock them if required. */
        for (count = 0U; count < pstTrgmuxUserConfig->numInOutMappingConfigs; count++) {
            if (pstTrgmuxUserConfig->pstTrgmuxInOutMappingConfig[count].lockTargetModuleReg) {
                TRGMUX_SetLockForTargetModule(base,
                                              pstTrgmuxUserConfig->pstTrgmuxInOutMappingConfig[count].euTargetModule);
            }
        }
    }

    return status;
}

/**
 *
 * Function Name : TRGMUX_DRV_Deinit
 * Description   : Reset to default values the source triggers corresponding to all target modules,
 * if none of the target modules is locked.
 *
 */
status_t TRGMUX_DRV_Deinit(uint32_t instance)
{
    OS_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    trgmux_type_t *base = s_trgmuxBase[instance];
    status_t status;

    /* Reset source triggers of all TRGMUX target modules to default. */
    status = TRGMUX_Init(instance, base);

    return status;
}

/**
 *
 * Function Name : TRGMUX_DRV_SetTrigSourceForTargetModule
 * Description   : This function configures a TRGMUX link between a source trigger and a target module,
 * if the requested target module is not locked.
 *
 */
status_t TRGMUX_DRV_SetTrigSourceForTargetModule(uint32_t instance, trgmux_trigger_source_t euTriggerSource,
                                                 trgmux_target_module_t euTargetModule)
{
    OS_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    trgmux_type_t *base = s_trgmuxBase[instance];
    status_t status;
    bool lock;

    lock = TRGMUX_GetLockForTargetModule(base, euTargetModule);

    if (true == lock) {
        status = STATUS_ERROR;
    } else {
        /* Configure link between trigger source and target module. */
        TRGMUX_SetTrigSourceForTargetModule(base, euTriggerSource, euTargetModule);
        status = STATUS_SUCCESS;
    }

    return status;
}

/**
 *
 * Function Name : TRGMUX_DRV_GetTrigSourceForTargetModule
 * Description   : This function returns the TRGMUX source trigger linked to a selected target module.
 *
 */
trgmux_trigger_source_t TRGMUX_DRV_GetTrigSourceForTargetModule(uint32_t instance, trgmux_target_module_t euTargetModule)
{
    OS_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    const trgmux_type_t *base = s_trgmuxBase[instance];

    return TRGMUX_GetTrigSourceForTargetModule(base, euTargetModule);
}

/**
 *
 * Function Name : TRGMUX_DRV_SetLockForTargetModule
 * Description   : This function locks the TRGMUX register of a selected target module.
 *
 */
void TRGMUX_DRV_SetLockForTargetModule(uint32_t instance, trgmux_target_module_t euTargetModule)
{
    OS_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    trgmux_type_t *base = s_trgmuxBase[instance];

    TRGMUX_SetLockForTargetModule(base, euTargetModule);
}

/**
 *
 * Function Name : TRGMUX_DRV_GetLockForTargetModule
 * Description   : This function gets the value of the LK bit from the TRGMUX register
 * corresponding to the selected target module.
 *
 */
bool TRGMUX_DRV_GetLockForTargetModule(uint32_t instance, trgmux_target_module_t euTargetModule)
{
    OS_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    const trgmux_type_t *base = s_trgmuxBase[instance];

    return TRGMUX_GetLockForTargetModule(base, euTargetModule);
}

/**
 *
 * Function Name : TRGMUX_DRV_GenSWTrigger
 * Description   : This function uses a SIM register in order to generate software
 * triggers to the target peripherals selected in TRGMUX.
 *
 */
void TRGMUX_DRV_GenSWTrigger(uint32_t instance)
{
    OS_ASSERT(instance < 1);

    static tcsr_type_t *const s_tcsrBase[TCSR_INSTANCE_COUNT] = TCSR_BASE_PTRS;

    /* The trigger is generated only when writing from 0 to 1. */
    s_tcsrBase[0U]->MISCTRL1 = 0U;
    s_tcsrBase[0U]->MISCTRL1 = TCSR_MISCTRL1_SW_TRG(1U);
}

/**
 *
 * Function Name : TRGMUX_DRV_ClearSWTrigger
 * Description   : This function disable SIM software trigger in TRGMUX.
 *
 */
void TRGMUX_DRV_ClearSWTrigger(uint32_t instance)
{
    OS_ASSERT(instance < 1);

    static tcsr_type_t *const s_tcsrBase[TCSR_INSTANCE_COUNT] = TCSR_BASE_PTRS;

    s_tcsrBase[0U]->MISCTRL1 = 0U;
}
