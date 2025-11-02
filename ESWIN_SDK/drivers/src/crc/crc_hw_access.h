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
 * @file crc_hw_access.h
 * @brief CRC driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CRC_HW_ACCESS_H__
#define __CRC_HW_ACCESS_H__

#include "crc_driver.h"
#include "crc_register.h"

/**
 * @brief Initializes the module to default configuration.
 * (Initial checksum: 0U,
 * Default polynomial: 0x1021U,
 * Type of read transpose: CRC_TRANSPOSE_NONE,
 * Type of write transpose: CRC_TRANSPOSE_NONE,
 * No complement of checksum read,
 * 32-bit CRC).
 *
 * @param  base The CRC peripheral base address.
 */
void CRC_Init(crc_type_t *const base);

/**
 * @brief Returns the current result of the CRC calculation.
 *
 * @param  base The CRC peripheral base address.
 * @return uint32_t The CRC current 32-bit calculation result.
 */
uint32_t CRC_GetCrcResult(const crc_type_t *const base);

/**
 * @brief Sets seed value for CRC computation.
 *
 * @param  base The CRC peripheral base address.
 * @param  seedValue The CRC computation 32-bit seed value.
 */
void CRC_SetSeedReg(crc_type_t *const base, uint32_t seedValue);

/**
 * @brief Gets the current CRC result.
 *
 * @param  base The CRC peripheral base address.
 * @return uint32_t The CRC current 32-bit result.
 */
static inline uint32_t CRC_GetDataReg(const crc_type_t *const base)
{
    return base->DATAu.DATA;
}

/**
 * @brief Sets the 32-bit of CRC data register.
 *
 * @param  base The CRC peripheral base address.
 * @param  value The CRC data register 32-bit value.
 */
static inline void CRC_SetDataReg(crc_type_t *const base, uint32_t value)
{
    base->DATAu.DATA = value;
}

/**
 * @brief Gets the upper 16 bits of the current CRC result.
 *
 * @param  base The CRC peripheral base address.
 * @return uint16_t The CRC current upper 16 bits result.
 */
static inline uint16_t CRC_GetDataHReg(const crc_type_t *const base)
{
    return base->DATAu.DATA_16.H;
}

/**
 * @brief Gets the lower 16 bits of the current CRC result.
 *
 * @param  base The CRC peripheral base address.
 * @return uint16_t The CRC current lower 16 bits result.
 */
static inline uint16_t CRC_GetDataLReg(const crc_type_t *const base)
{
    return base->DATAu.DATA_16.L;
}

/**
 * @brief Sets the lower 16 bits of CRC data register - L.
 *
 * @param  base The CRC peripheral base address.
 * @param  value The CRC current lower 16 bits value.
 */
static inline void CRC_SetDataLReg(crc_type_t *const base, uint16_t value)
{
    base->DATAu.DATA_16.L = value;
}

/**
 * @brief Sets the Low Lower Byte - LL.
 *
 * @param  base The CRC peripheral base address.
 * @param  value The CRC Low Lower Byte value.
 */
static inline void CRC_SetDataLLReg(crc_type_t *const base, uint8_t value)
{
    base->DATAu.DATA_8.LL = value;
}

/**
 * @brief Gets the polynomial register value.
 *
 * @param  base The CRC peripheral base address.
 * @return uint32_t The CRC polynomial register value.
 */
static inline uint32_t CRC_GetPolyReg(const crc_type_t *const base)
{
    return base->GPOLY;
}

/**
 * @brief Sets the polynomial register.
 *
 * @param  base The CRC peripheral base address.
 * @param  value The CRC polynomial value.
 */
static inline void CRC_SetPolyReg(crc_type_t *const base, uint32_t value)
{
    base->GPOLY = value;
}

/**
 * @brief Sets the CRC_DATA register mode.
 *
 * @param  base The CRC peripheral base address.
 * @param  enable CRC seed or data mode, CRC seed mode (true)/CRC data mode (false).
 */
static inline void CRC_SetSeedOrDataMode(crc_type_t *const base, bool enable)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_WAS_MASK);
    ctrlTemp |= CRC_CTRL_WAS(enable ? 1UL : 0UL);
    base->CTRL = ctrlTemp;
}

/**
 * @brief Gets complement read of CRC data register.
 * This function gets complement read of CRC data register.
 * Some CRC protocols require the final checksum to be XORed with 0xFFFFFFFF
 * or 0xFFFF. Complement mode enables "on the fly" complementing of read data.
 *
 * @param  base The CRC peripheral base address.
 * @return true: Invert or complement the read value of the CRC Data register.
 * @return false: No XOR on reading.
 */
static inline bool CRC_GetFXorMode(const crc_type_t *const base)
{
    return ((base->CTRL & CRC_CTRL_FXOR_MASK) >> CRC_CTRL_FXOR_SHIFT) != 0U;
}

/**
 * @brief Sets complement read of CRC data register.
 * This function sets complement read of CRC data register.
 * Some CRC protocols require the final checksum to be XORed with 0xFFFFFFFF
 * or 0xFFFF. Complement mode enables "on the fly" complementing of read data.
 *
 * @param  base The CRC peripheral base address.
 * @param  enable Enable or disable complementing of read data.
 */
static inline void CRC_SetFXorMode(crc_type_t *const base, bool enable)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_FXOR_MASK);
    ctrlTemp |= CRC_CTRL_FXOR(enable ? 1UL : 0UL);
    base->CTRL = ctrlTemp;
}

/**
 * @brief Gets the CRC protocol width.
 *
 * @param  base The CRC peripheral base address.
 * @return crc_bit_width_t CRC protocol width.
 *         - CRC_BITS_16: 16-bit CRC protocol.
 *         - CRC_BITS_32: 32-bit CRC protocol.
 */
static inline crc_bit_width_t CRC_GetProtocolWidth(const crc_type_t *const base)
{
    crc_bit_width_t retVal = CRC_BITS_16;

    if (((base->CTRL & CRC_CTRL_TCRC_MASK) >> CRC_CTRL_TCRC_SHIFT) != 0U) {
        retVal = CRC_BITS_32;
    }

    return retVal;
}

/**
 * @brief Sets the CRC protocol width.
 *
 * @param  base The CRC peripheral base address.
 * @param  width The CRC protocol width.
 *            - CRC_BITS_16: 16-bit CRC protocol.
 *            - CRC_BITS_32: 32-bit CRC protocol.
 */
static inline void CRC_SetProtocolWidth(crc_type_t *const base, crc_bit_width_t width)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_TCRC_MASK);
    ctrlTemp |= CRC_CTRL_TCRC(width);
    base->CTRL = ctrlTemp;
}

/**
 * @brief Gets the CRC transpose type for writes.
 *
 * @param  base The CRC peripheral base address.
 * @return crc_transpose_t CRC input transpose type for writes.
 */
static inline crc_transpose_t CRC_GetWriteTranspose(const crc_type_t *const base)
{
    crc_transpose_t type;
    uint32_t temp = (base->CTRL & CRC_CTRL_TOT_MASK) >> CRC_CTRL_TOT_SHIFT;

    switch (temp) {
    case 1U:
        type = CRC_TRANSPOSE_BITS;
        break;
    case 2U:
        type = CRC_TRANSPOSE_BITS_AND_BYTES;
        break;
    case 3U:
        type = CRC_TRANSPOSE_BYTES;
        break;
    default:
        type = CRC_TRANSPOSE_NONE;
        break;
    }

    return type;
}

/**
 * @brief Sets the CRC transpose type for writes.
 *
 * @param  base The CRC peripheral base address.
 * @param  transp The CRC input transpose type.
 */
static inline void CRC_SetWriteTranspose(crc_type_t *const base, crc_transpose_t transp)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_TOT_MASK);
    ctrlTemp |= CRC_CTRL_TOT(transp);
    base->CTRL = ctrlTemp;
}

/**
 * @brief Gets the CRC transpose type for reads.
 *
 * @param  base The CRC peripheral base address.
 * @return crc_transpose_t CRC output transpose type.
 */
static inline crc_transpose_t CRC_GetReadTranspose(const crc_type_t *const base)
{
    crc_transpose_t type;
    uint32_t temp = (base->CTRL & CRC_CTRL_TOTR_MASK) >> CRC_CTRL_TOTR_SHIFT;

    switch (temp) {
    case 1U:
        type = CRC_TRANSPOSE_BITS;
        break;
    case 2U:
        type = CRC_TRANSPOSE_BITS_AND_BYTES;
        break;
    case 3U:
        type = CRC_TRANSPOSE_BYTES;
        break;
    default:
        type = CRC_TRANSPOSE_NONE;
        break;
    }

    return type;
}

/**
 * @brief Sets the CRC transpose type for reads.
 *
 * @param  base The CRC peripheral base address.
 * @param  transp The CRC output transpose type.
 */
static inline void CRC_SetReadTranspose(crc_type_t *const base, crc_transpose_t transp)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_TOTR_MASK);
    ctrlTemp |= CRC_CTRL_TOTR(transp);
    base->CTRL = ctrlTemp;
}

#endif /* __CRC_HW_ACCESS_H__ */
