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
 * @file crc_driver.h
 * @brief CRC driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CRC_DRIVER_H__
#define __CRC_DRIVER_H__

#include "emps_platform.h"
#include "status.h"
#include <stddef.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup CRC_Driver CRC Driver
 * @{
 */

/**
 * @brief CRC type of transpose of read write data.
 */
typedef enum {
    CRC_TRANSPOSE_NONE           = 0x00U, /**< No transpose. */
    CRC_TRANSPOSE_BITS           = 0x01U, /**< Transpose bits in bytes. */
    CRC_TRANSPOSE_BITS_AND_BYTES = 0x02U, /**< Transpose bytes and bits in bytes. */
    CRC_TRANSPOSE_BYTES          = 0x03U  /**< Transpose bytes. */
} crc_transpose_t;

/**
 * @brief CRC bit width.
 */
typedef enum {
    CRC_BITS_16 = 0U, /**< Generate 16-bit CRC code. */
    CRC_BITS_32 = 1U  /**< Generate 32-bit CRC code. */
} crc_bit_width_t;

/**
 * @brief CRC configuration structure.
 */
typedef struct {
    crc_bit_width_t crcWidth;       /**< Selects 16-bit or 32-bit CRC protocol. \D{CRC_BITS_32} */
    uint32_t polynomial;            /**< CRC Polynomial, MSBit first.
                                    * Example polynomial: 0x1021U = 1_0000_0010_0001 = x^12+x^5+1 \D{0x04C11DB7UL} */
    crc_transpose_t readTranspose;  /**< Type of transpose when reading CRC result. \D{CRC_TRANSPOSE_BITS_AND_BYTES} */
    crc_transpose_t writeTranspose; /**< Type of transpose when writing CRC input data. \D{CRC_TRANSPOSE_BITS} */
    bool complementChecksum;        /**< True if the result shall be complement of the actual checksum. \D{true} */
    uint32_t seed;                  /**< Starting checksum value. \R{0x0UL,0xFFFFFFFFUL}\D{0xFFFFFFFFUL} */
} crc_user_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initializes the CRC module.
 *
 * This function initializes CRC driver based on user configuration input.
 * The user must make sure that the clock is enabled.
 *
 * @param  instance The CRC instance number(0).
 * @param  userConfigPtr Pointer to structure of initialization.
 * @return status_t Execution status (success).
 */
status_t CRC_DRV_Init(uint32_t instance, const crc_user_config_t *userConfigPtr);

/**
 * @brief Sets the default configuration.
 *
 * @param  instance The CRC instance number(0).
 * @return status_t Execution status (success).
 */
status_t CRC_DRV_Deinit(uint32_t instance);

/**
 * @brief Appends 8-bit data to the current CRC calculation and returns new result.
 *
 * This function appends 8-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation).
 *
 * @param  instance The CRC instance number(0).
 * @param  data Input data for CRC calculation.
 * @param  newSeed Sets new CRC calculation:
 * - true: New seed set and used for new calculation.
 * - false: Seed argument ignored, continues old calculation.
 * @param  seed New seed if newSeed is true, else ignored.
 * @return uint32_t New CRC result.
 */
uint32_t CRC_DRV_GetCrc8(uint32_t instance, uint8_t data, bool newSeed, uint32_t seed);

/**
 * @brief Appends 16-bit data to the current CRC calculation and returns new result.
 *
 * This function appends 16-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation).
 *
 * @param  instance The CRC instance number(0).
 * @param  data 8-bit Input data for CRC calculation.
 * @param  newSeed Sets new CRC calculation:
 * - true: New seed set and used for new calculation.
 * - false: Seed argument ignored, continues old calculation.
 * @param  seed New seed if newSeed is true, else ignored.
 * @return uint32_t New CRC result.
 */
uint32_t CRC_DRV_GetCrc16(uint32_t instance, uint16_t data, bool newSeed, uint32_t seed);

/**
 * @brief Appends 32-bit data to the current CRC calculation and returns new result.
 *
 * This function appends 32-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation).
 *
 * @param  instance The CRC instance number(0).
 * @param  data 16-bit Input data for CRC calculation.
 * @param  newSeed Sets new CRC calculation:
 * - true: New seed set and used for new calculation.
 * - false: Seed argument ignored, continues old calculation.
 * @param  seed New seed if newSeed is true, else ignored.
 * @return uint32_t New CRC result.
 */
uint32_t CRC_DRV_GetCrc32(uint32_t instance, uint32_t data, bool newSeed, uint32_t seed);

/**
 * @brief Appends a block of bytes to the current 8bit CRC calculation.
 *
 * @param  instance The CRC instance number(0).
 * @param  data 32-bit Data for current CRC calculation.
 * @param  dataSize Length of data to be calculated.
 */
void CRC_DRV_WriteData8(uint32_t instance, const uint8_t *data, uint32_t dataSize);

/**
 * @brief Appends a block of bytes to the current 16bit CRC calculation.
 *
 * @param  instance The CRC instance number(0).
 * @param  data Data for current CRC calculation.
 * @param  dataSize Length of data to be calculated.
 */
void CRC_DRV_WriteData16(uint32_t instance, const uint16_t *data, uint32_t dataSize);

/**
 * @brief Appends a block of bytes to the current 32-bit CRC calculation.
 *
 * @param  instance The CRC instance number(0).
 * @param  data Data for current CRC calculation.
 * @param  dataSize Length of data to be calculated.
 */
void CRC_DRV_WriteData32(uint32_t instance, const uint32_t *data, uint32_t dataSize);

/**
 * @brief Returns the current result of the CRC calculation.
 *
 * @param  instance The CRC instance number(0).
 * @return uint32_t Result of CRC calculation.
 */
uint32_t CRC_DRV_GetCrcResult(uint32_t instance);

/**
 * @brief Configures the CRC module from a user configuration structure.
 *
 * @param  instance The CRC instance number(0).
 * @param  userConfigPtr Pointer to structure of initialization.
 * @return status_t Execution status (success).
 */
status_t CRC_DRV_Configure(uint32_t instance, const crc_user_config_t *userConfigPtr);

/**
 * @brief Get configures of the CRC module currently.
 *
 * @param  instance The CRC instance number(0).
 * @param  userConfigPtr Pointer to structure of initialization.
 * @return status_t Execution status (success).
 */
status_t CRC_DRV_GetConfig(uint32_t instance, crc_user_config_t *const userConfigPtr);

/**
 * @brief Get default configures the CRC module for configuration structure.
 *
 * @param  userConfigPtr Pointer to structure of initialization.
 * @return status_t Execution status (success).
 */
status_t CRC_DRV_GetDefaultConfig(crc_user_config_t *const userConfigPtr);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group CRC_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __CRC_DRIVER_H__ */
