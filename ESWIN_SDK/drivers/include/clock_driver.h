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
 * @file clock_driver.h
 * @brief Clock driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CLOCK_DRIVER_H__
#define __CLOCK_DRIVER_H__

#include "emps_platform.h"
#include "osal.h"
#include <stdbool.h>
#include <stdint.h>

#if (defined(PLATFORM_EAM2011))
#include "../src/clock/EAM2011/clock_EAM2011.h"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Gets the clock frequency for a specific clock name.
 *
 * This function checks the current clock configurations and then calculates
 * the clock frequency for a specific clock name defined in clock_names_t.
 * Clock modules must be properly configured before using this function.
 * See features.h for supported clock names for different chip families.
 * The returned value is in Hertz. If it cannot find the clock name
 * or the name is not supported for a specific chip family, it returns an
 * STATUS_UNSUPPORTED. If frequency is required for a peripheral and the
 * module is not clocked, then STATUS_MCU_GATED_OFF status is returned.
 * Frequency is returned if a valid address is provided. If frequency is
 * required for a peripheral that doesn't support protocol clock, the zero
 * value is provided.
 *
 * @param  clockName Clock names defined in clock_names_t.
 * @param  frequency Returned clock frequency value in Hertz.
 * @return status_t Error code defined in status_t.
 */
status_t CLOCK_DRV_GetFreq(clock_names_t clockName, uint32_t *frequency);

/**
 * @brief Set clock configuration according to pre-defined structure.
 *
 * @param  config Pointer to configuration structure.
 * @return status_t Error code.
 */
status_t CLOCK_SYS_Init(clock_user_config_t const *config);

/**
 * @brief Enable/Disable clock
 *
 * @param  clockName Clock names defined in clock_names_t.
 * @param  enable enable/disable state.
 * @return status_t Error code defined in status_t.
 */
status_t CLOCK_SYS_ClockEnable(clock_names_t clockName, bool enable);

/**
 * @brief  System driver calls,do not need to pay attention
 *
 * @param  name  clock name
 * @param  divider division factor
 * @return status_t Error code defined in status_t.
 */
status_t CLOCK_SYS_SetClkDivider(clock_names_t name, crg_clock_divider_t divider);

#if defined(__cplusplus)
}
#endif

#endif
