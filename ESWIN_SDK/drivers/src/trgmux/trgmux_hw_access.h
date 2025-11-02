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
 * @file trgmux_hw_access.h
 * @brief TRGMUX driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __TRGMUX_HW_ACCESS_H__
#define __TRGMUX_HW_ACCESS_H__

#include "trgmux_driver.h"
#include "trgmux_register.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Restore the TRGMUX module to reset value.
 *
 * This function restores the TRGMUX module to reset value.
 *
 * @param  instance The Trgmux instance 0:trgmux 1:tmrmux
 * @param  base The TRGMUX peripheral base address.
 * @return status_t Execution status:
 *                  STATUS_SUCCESS: Succeed.
 *                  STATUS_ERROR:   If at least one of the target module register is locked.
 */
status_t TRGMUX_Init(uint32_t instance, trgmux_type_t *base);

/**
 * @brief Configures a source trigger for a target module.
 *
 * This function configures a TRGMUX link between a source trigger and a target module,
 * if the requested target module is not locked.
 *
 * @param  base The TRGMUX peripheral base address.
 * @param  euTriggerSource One of the values in the trgmux_trigger_source_t enumeration.
 * @param  euTargetModule One of the values in the trgmux_target_module_t enumeration.
 */
void TRGMUX_SetTrigSourceForTargetModule(trgmux_type_t *base, trgmux_trigger_source_t euTriggerSource,
                                         trgmux_target_module_t euTargetModule);

/**
 * @brief Get the source trigger configured for a target module.
 *
 * This function returns the TRGMUX source trigger linked to a selected target module.
 *
 * @param  base The TRGMUX peripheral base address.
 * @param  euTargetModule One of the values in the trgmux_target_module_t enumeration.
 * @return trgmux_trigger_source_t Enum value corresponding to the trigger source configured for the given target
 * module.
 */
trgmux_trigger_source_t TRGMUX_GetTrigSourceForTargetModule(const trgmux_type_t *base,
                                                            trgmux_target_module_t euTargetModule);

/**
 * @brief Lock the TRGMUX register of a target module.
 *
 * This function sets the LK bit of the TRGMUX register corresponding to
 * the selected target module. Please note that some TRGMUX registers can contain up to 4
 * SEL bitfields, meaning that these registers can be used to configure up to 4 target
 * modules independently. Because the LK bit is only one per register, the configuration
 * of all target modules referred from that register will be locked.
 *
 * @param  base The TRGMUX peripheral base address.
 * @param  euTargetModule One of the values in the trgmux_target_module_t enumeration.
 */
void TRGMUX_SetLockForTargetModule(trgmux_type_t *base, trgmux_target_module_t euTargetModule);

/**
 * @brief Get the Lock bit status of the TRGMUX register of a target module.
 *
 * This function gets the value of the LK bit from the TRGMUX register corresponding to
 * the selected target module.
 *
 * @param  base The TRGMUX peripheral base address.
 * @param  euTargetModule One of the values in the trgmux_target_module_t enumeration.
 * @return true: The state of the LK bit is locked.
 * @return false: The state of the LK bit is not locked.
 */
bool TRGMUX_GetLockForTargetModule(const trgmux_type_t *base, trgmux_target_module_t euTargetModule);

#if defined(__cplusplus)
}
#endif

#endif /* __TRGMUX_HW_ACCESS_H__ */
