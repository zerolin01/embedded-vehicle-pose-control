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
 * @file crc_hw_access.c
 * @brief CRC driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "crc_hw_access.h"

#define CRC_INITIAL_SEED (0U)

/**
 *
 * Function Name : CRC_Init
 * Description   : This function initializes the module to default configuration.
 * (Initial checksum: 0U,
 * Default polynomial: 0x1021U,
 * Type of read transpose: CRC_TRANSPOSE_NONE,
 * Type of write transpose: CRC_TRANSPOSE_NONE,
 * No complement of checksum read,
 * 32-bit CRC).
 *
 */
void CRC_Init(crc_type_t *const base)
{
    /* Set CRC mode to 32-bit */
    CRC_SetProtocolWidth(base, CRC_BITS_32);

    /* Set read/write transpose and complement checksum to none */
    CRC_SetWriteTranspose(base, CRC_TRANSPOSE_NONE);
    CRC_SetReadTranspose(base, CRC_TRANSPOSE_NONE);
    CRC_SetFXorMode(base, false);

    /* Write polynomial to 0x1021U */
    CRC_SetPolyReg(base, FEATURE_CRC_DEFAULT_POLYNOMIAL);

    /* Write seed to zero */
    CRC_SetSeedOrDataMode(base, true);
    CRC_SetDataReg(base, CRC_INITIAL_SEED);
    CRC_SetSeedOrDataMode(base, false);
}

/**
 *
 * Function Name : CRC_SetSeedReg
 * Description   : This function set seed value for new of CRC module computation.
 *
 */
void CRC_SetSeedReg(crc_type_t *const base, uint32_t seedValue)
{
    CRC_SetSeedOrDataMode(base, true);
    /* Write a seed - initial checksum */
    CRC_SetDataReg(base, seedValue);
    CRC_SetSeedOrDataMode(base, false);
}

/**
 *
 * Function Name : CRC_GetCrcResult
 * Description   : This function returns the current result of the CRC calculation.
 *
 */
uint32_t CRC_GetCrcResult(const crc_type_t *const base)
{
    crc_bit_width_t width = CRC_GetProtocolWidth(base);
    crc_transpose_t transpose;
    uint32_t result;

    if (CRC_BITS_16 == width) {
        transpose = CRC_GetReadTranspose(base);
        if ((CRC_TRANSPOSE_BITS_AND_BYTES == transpose) || (CRC_TRANSPOSE_BYTES == transpose)) {
            /* Returns upper 16 bits of CRC because of transposition in 16-bit mode */
            result = (uint32_t)CRC_GetDataHReg(base);
        } else {
            result = (uint32_t)CRC_GetDataLReg(base);
        }
    } else {
        result = CRC_GetDataReg(base);
    }

    return result;
}
