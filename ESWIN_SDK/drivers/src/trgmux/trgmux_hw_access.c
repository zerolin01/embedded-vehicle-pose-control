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
 * @file trgmux_hw_access.c
 * @brief TRGMUX driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "trgmux_hw_access.h"
#include <stddef.h>

/** Number of SEL bitfields in one TRGMUX register. */
#define TRGMUX_NUM_SEL_BITFIELDS_PER_REG (4U)
/** Get the index of the TRGMUX register. */
#define TRGMUX_IDX_REG(x) ((uint8_t)((uint8_t)(x) / TRGMUX_NUM_SEL_BITFIELDS_PER_REG))
/** Get the index of the SEL bitfield inside TRGMUX register. */
#define TRGMUX_IDX_SEL_BITFIELD_REG(x) ((uint8_t)((uint8_t)(x) % TRGMUX_NUM_SEL_BITFIELDS_PER_REG))

/**
 *
 * Function Name : TRGMUX_Init
 * Description   : This function restores the TRGMUX module to reset value.
 *
 */
status_t TRGMUX_Init(uint32_t instance, trgmux_type_t *base)
{
    OS_ASSERT(NULL != base);
    OS_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    /* Constant array storing the value of all TRGMUX output(target module) identifiers. */
    static const trgmux_target_module_t s_trgmuxTargetModule[] = FEATURE_TRGMUX_TARGET_MODULE;
    static const tmrmux_target_module_t s_tmrmuxTargetModule[] = FEATURE_TMRMUX_TARGET_MODULE;

    uint8_t count   = 0U;
    bool lock       = false;
    status_t status = STATUS_ERROR;

    static const trgmux_target_module_t *targetModule = NULL;
    uint32_t target_size                              = 0;

    switch (instance) {
    case 0:
        targetModule = s_trgmuxTargetModule;
        target_size  = sizeof(s_trgmuxTargetModule) / sizeof(trgmux_target_module_t);
        break;
    case 1:
        targetModule = (trgmux_target_module_t *)s_tmrmuxTargetModule;
        target_size  = sizeof(s_tmrmuxTargetModule) / sizeof(trgmux_target_module_t);
        break;
    default:
        break;
    }

    /* Check if any of the TRGMUX registers is locked */
    while ((count < target_size) && (lock != true)) {
        lock = TRGMUX_GetLockForTargetModule(base, targetModule[count]);
        count++;
    }

    /* Abort operations if at least one of the target module is locked. */
    if (false == lock) {
        /* Set all SEL bitfields of all TRGMUX registers to default value. */
        for (count = 0U; count < target_size; count++) {
            /* Write the TRGMUX register. */
            uint32_t index = TRGMUX_IDX_REG(targetModule[count]);
            uint32_t mask  = ~((uint32_t)TRGMUX_TRGMUX_N_SEL0_MASK << (TRGMUX_TRGMUX_N_SEL1_SHIFT * TRGMUX_IDX_SEL_BITFIELD_REG(targetModule[count])));
            base->trgmuxN[index] &= mask;
        }

        status = STATUS_SUCCESS;
    }

    return status;
}

/**
 *
 * Function Name : TRGMUX_SetTrigSourceForTargetModule
 * Description   : This function configures a TRGMUX link between a source trigger
 * and a target module, if the requested target module is not locked.
 *
 */
void TRGMUX_SetTrigSourceForTargetModule(trgmux_type_t *base, trgmux_trigger_source_t euTriggerSource,
                                         trgmux_target_module_t euTargetModule)
{
    OS_ASSERT(base != NULL);

    uint32_t tmpReg;
    /* Read value of entire TRGMUX register in a temp variable. */
    tmpReg = base->trgmuxN[TRGMUX_IDX_REG(euTargetModule)];
    /* Clear first the SEL bitfield inside the TRGMUX register. */
    tmpReg &= ~((uint32_t)TRGMUX_TRGMUX_N_SEL0_MASK
                << (TRGMUX_TRGMUX_N_SEL1_SHIFT * TRGMUX_IDX_SEL_BITFIELD_REG(euTargetModule)));
    /* Configure the SEL bitfield to the desired value. */
    tmpReg |= ((uint32_t)euTriggerSource)
              << ((uint8_t)(TRGMUX_TRGMUX_N_SEL1_SHIFT * TRGMUX_IDX_SEL_BITFIELD_REG(euTargetModule)));
    /* Write back the TRGMUX register. */
    base->trgmuxN[TRGMUX_IDX_REG(euTargetModule)] = tmpReg;
}

/**
 *
 * Function Name : TRGMUX_GetTrigSourceForTargetModule
 * Description   : This function returns the TRGMUX source trigger linked to
 * a selected target module.
 *
 */
trgmux_trigger_source_t TRGMUX_GetTrigSourceForTargetModule(const trgmux_type_t *base,
                                                            trgmux_target_module_t euTargetModule)
{
    OS_ASSERT(base != NULL);

    uint32_t trigSource;
    /* Perform the update operation. */
    trigSource = ((base->trgmuxN[TRGMUX_IDX_REG(euTargetModule)] >>
                   (TRGMUX_TRGMUX_N_SEL1_SHIFT * TRGMUX_IDX_SEL_BITFIELD_REG(euTargetModule))) &
                  TRGMUX_TRGMUX_N_SEL0_MASK);

    return (trgmux_trigger_source_t)(trigSource);
}

/**
 *
 * Function Name : TRGMUX_SetLockForTargetModule
 * Description   : This function sets the LK bit of the TRGMUX register corresponding.
 *
 */
void TRGMUX_SetLockForTargetModule(trgmux_type_t *base, trgmux_target_module_t euTargetModule)
{
    OS_ASSERT(base != NULL);

    /* Perform the update operation */
    base->trgmuxN[TRGMUX_IDX_REG(euTargetModule)] |= (((uint32_t)1U) << TRGMUX_TRGMUX_N_LK_SHIFT);
}

/**
 *
 * Function Name : TRGMUX_GetLockForTargetModule
 * Description   : Get the Lock bit status of the TRGMUX register of a target module.
 *
 */
bool TRGMUX_GetLockForTargetModule(const trgmux_type_t *base, trgmux_target_module_t euTargetModule)
{
    OS_ASSERT(base != NULL);

    uint32_t lockVal;
    bool lock;

    /* Get the lock bit value */
    lockVal = ((base->trgmuxN[TRGMUX_IDX_REG(euTargetModule)] & TRGMUX_TRGMUX_N_LK_MASK) >> TRGMUX_TRGMUX_N_LK_SHIFT);

    lock = (0U == lockVal) ? false : true;

    return lock;
}
