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
 * @file tsensor_driver.h
 * @brief TSENSOR driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __TSENSOR_DRIVER_H__
#define __TSENSOR_DRIVER_H__

#include "clock_driver.h"
#include "status.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup TSENSOR_Driver TSENSOR Driver
 * @{
 */

/**
 * @brief TSENSOR mode types.
 */
typedef enum {
    TSENSOR_LOW_POWER_MODE  = 0x0U, /**< TSENSOR low power mode. */
    TSENSOR_HIGH_POWER_MODE = 0x1U, /**< TSENSOR high power mode. */
} tsensor_power_mode_t;

/**
 * @brief TSENSOR temperature coefficient values.
 */
typedef enum {
    TSENSOR_COEFFICIENT_2_740_MV = 0x0U,  /**< TSENSOR temperature coefficient values = 2.740mV. */
    TSENSOR_COEFFICIENT_2_756_MV = 0x1U,  /**< TSENSOR temperature coefficient values = 2.756mV. */
    TSENSOR_COEFFICIENT_2_773_MV = 0x2U,  /**< TSENSOR temperature coefficient values = 2.773mV. */
    TSENSOR_COEFFICIENT_2_789_MV = 0x3U,  /**< TSENSOR temperature coefficient values = 2.789mV. */
    TSENSOR_COEFFICIENT_2_805_MV = 0x4U,  /**< TSENSOR temperature coefficient values = 2.805mV. */
    TSENSOR_COEFFICIENT_2_821_MV = 0x5U,  /**< TSENSOR temperature coefficient values = 2.821mV. */
    TSENSOR_COEFFICIENT_2_838_MV = 0x6U,  /**< TSENSOR temperature coefficient values = 2.838mV. */
    TSENSOR_COEFFICIENT_2_854_MV = 0x7U,  /**< TSENSOR temperature coefficient values = 2.854mV. */
    TSENSOR_COEFFICIENT_2_870_MV = 0x8U,  /**< TSENSOR temperature coefficient values = 2.870mV. */
    TSENSOR_COEFFICIENT_2_886_MV = 0x9U,  /**< TSENSOR temperature coefficient values = 2.886mV. */
    TSENSOR_COEFFICIENT_2_903_MV = 0xAU,  /**< TSENSOR temperature coefficient values = 2.903mV. */
    TSENSOR_COEFFICIENT_2_919_MV = 0xBU,  /**< TSENSOR temperature coefficient values = 2.919mV. */
    TSENSOR_COEFFICIENT_2_935_MV = 0xCU,  /**< TSENSOR temperature coefficient values = 2.935mV. */
    TSENSOR_COEFFICIENT_2_951_MV = 0xDU,  /**< TSENSOR temperature coefficient values = 2.951mV. */
    TSENSOR_COEFFICIENT_2_968_MV = 0xEU,  /**< TSENSOR temperature coefficient values = 2.968mV. */
    TSENSOR_COEFFICIENT_2_984_MV = 0xFU,  /**< TSENSOR temperature coefficient values = 2.984mV. */
    TSENSOR_COEFFICIENT_3_000_MV = 0x10U, /**< TSENSOR temperature coefficient values = 3.000mV. */
    TSENSOR_COEFFICIENT_3_014_MV = 0x11U, /**< TSENSOR temperature coefficient values = 3.014mV. */
    TSENSOR_COEFFICIENT_3_028_MV = 0x12U, /**< TSENSOR temperature coefficient values = 3.028mV. */
    TSENSOR_COEFFICIENT_3_042_MV = 0x13U, /**< TSENSOR temperature coefficient values = 3.042mV. */
    TSENSOR_COEFFICIENT_3_056_MV = 0x14U, /**< TSENSOR temperature coefficient values = 3.056mV. */
    TSENSOR_COEFFICIENT_3_070_MV = 0x15U, /**< TSENSOR temperature coefficient values = 3.070mV. */
    TSENSOR_COEFFICIENT_3_084_MV = 0x16U, /**< TSENSOR temperature coefficient values = 3.084mV. */
    TSENSOR_COEFFICIENT_3_098_MV = 0x17U, /**< TSENSOR temperature coefficient values = 3.098mV. */
    TSENSOR_COEFFICIENT_3_112_MV = 0x18U, /**< TSENSOR temperature coefficient values = 3.112mV. */
    TSENSOR_COEFFICIENT_3_126_MV = 0x19U, /**< TSENSOR temperature coefficient values = 3.126mV. */
    TSENSOR_COEFFICIENT_3_140_MV = 0x1AU, /**< TSENSOR temperature coefficient values = 3.140mV. */
    TSENSOR_COEFFICIENT_3_154_MV = 0x1BU, /**< TSENSOR temperature coefficient values = 3.154mV. */
    TSENSOR_COEFFICIENT_3_168_MV = 0x1CU, /**< TSENSOR temperature coefficient values = 3.168mV. */
    TSENSOR_COEFFICIENT_3_182_MV = 0x1DU, /**< TSENSOR temperature coefficient values = 3.182mV. */
    TSENSOR_COEFFICIENT_3_196_MV = 0x1EU, /**< TSENSOR temperature coefficient values = 3.196mV. */
    TSENSOR_COEFFICIENT_3_210_MV = 0x1FU, /**< TSENSOR temperature coefficient values = 3.210mV. */
} tsensor_coefficient_value_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Set the TSENSOR TsensorEn register.
 *
 * @param  instance The TSENSOR instance number(0).
 */
void TSENSOR_DRV_SetTsensorEn(uint32_t instance);

/**
 * @brief Get the TSENSOR TsensorEn register.
 *
 * @param  instance The TSENSOR instance number(0).
 * @return uint32_t TSENSOR TsensorEn register value.
 */
uint32_t TSENSOR_DRV_GetTsensorEn(uint32_t instance);

/**
 * @brief Clear the TSENSOR TsensorEn register.
 *
 * @param  instance The TSENSOR instance number(0).
 */
void TSENSOR_DRV_ClearTsensorEn(uint32_t instance);

/**
 * @brief Write the TSENSOR regPtat register.
 *
 * @param  instance The TSENSOR instance number(0).
 * @param  value TSENSOR regPtatLv register value.
 */
void TSENSOR_DRV_WriteRegPtat(uint32_t instance, tsensor_power_mode_t mode, tsensor_coefficient_value_t lv);

/**
 * @brief Read the TSENSOR regPtatLv register.
 *
 * @param  instance The TSENSOR instance number(0).
 * @return uint32_t TSENSOR regPtatLv register value.
 */
uint32_t TSENSOR_DRV_ReadRegPtatLv(uint32_t instance);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group TSENSOR_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif
