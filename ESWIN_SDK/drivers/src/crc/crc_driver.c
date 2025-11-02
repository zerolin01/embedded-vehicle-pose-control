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
 * @file crc_driver.c
 * @brief CRC driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "crc_driver.h"
#include "clock_driver.h"
#include "emps_platform.h"

#include "crc_hw_access.h"

static crc_type_t *const g_pstCrcBase[] = CRC_BASE_PTRS;

/**
 *
 * Function Name : CRC_DRV_Init
 * Description   : This function initializes CRC driver based on user configuration input.
 * The user must make sure that the clock is enabled.
 *
 */
status_t CRC_DRV_Init(uint32_t instance, const crc_user_config_t *userConfigPtr)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    OS_ASSERT(userConfigPtr != NULL);

    CLOCK_SYS_ClockEnable(CRC_CLK, true);

    crc_type_t *base   = g_pstCrcBase[instance];
    status_t retStatus = STATUS_SUCCESS;

    /* Set the default configuration */
    CRC_Init(base);
    /* Set the CRC configuration */
    retStatus = CRC_DRV_Configure(instance, userConfigPtr);

    return retStatus;
}

/**
 *
 * Function Name : CRC_DRV_Deinit
 * Description   : This function sets the default configuration.
 *
 */
status_t CRC_DRV_Deinit(uint32_t instance)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    crc_type_t *base = g_pstCrcBase[instance];

    /* Set the default configuration */
    CRC_Init(base);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : CRC_DRV_GetCrc8
 * Description   : This function appends 8-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and result are calculated
 * from the seed new value (new CRC calculation).
 *
 */
uint32_t CRC_DRV_GetCrc8(uint32_t instance, uint8_t data, bool newSeed, uint32_t seed)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    crc_type_t *base = g_pstCrcBase[instance];

    /* If newSeed is true then write a seed to initial checksum */
    if (newSeed) {
        /* Write a seed - initial checksum */
        CRC_SetSeedReg(base, seed);
    }
    /* Write 8-bit data */
    CRC_SetDataLLReg(base, data);

    /* Result of the CRC calculation */
    return CRC_GetCrcResult(base);
}

/**
 *
 * Function Name : CRC_DRV_GetCrc16
 * Description   : This function appends 16-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and result are calculated
 * from the seed new value (new CRC calculation).
 *
 */
uint32_t CRC_DRV_GetCrc16(uint32_t instance, uint16_t data, bool newSeed, uint32_t seed)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    crc_type_t *base = g_pstCrcBase[instance];

    /* If newSeed is true then write a seed to initial checksum */
    if (newSeed) {
        /* Write a seed - initial checksum */
        CRC_SetSeedReg(base, seed);
    }
    /* Write 16-bit data */
    CRC_SetDataLReg(base, data);

    /* Result of the CRC calculation */
    return CRC_GetCrcResult(base);
}

/**
 *
 * Function Name : CRC_DRV_GetCrc32
 * Description   : This function appends 32-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and result are calculated
 * from the seed new value (new CRC calculation).
 *
 */
uint32_t CRC_DRV_GetCrc32(uint32_t instance, uint32_t data, bool newSeed, uint32_t seed)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    crc_type_t *base = g_pstCrcBase[instance];

    /* If newSeed is true then write a seed to initial checksum */
    if (newSeed) {
        /* Write a seed - initial checksum */
        CRC_SetSeedReg(base, seed);
    }

    /* Write 32-bit data */
    CRC_SetDataReg(base, data);

    /* Result of the CRC calculation */
    return CRC_GetCrcResult(base);
}

/**
 *
 * Function Name : CRC_DRV_WriteData8
 * Description   : This function appends a block of bytes to the current 8-bit CRC calculation.
 *
 */
void CRC_DRV_WriteData8(uint32_t instance, const uint8_t *data, uint32_t dataSize)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    OS_ASSERT(data != NULL);
    uint32_t i;
    crc_type_t *base = g_pstCrcBase[instance];

    /* 8-bit writes till end of data buffer */
    for (i = 0U; i < dataSize; i++) {
        CRC_SetDataLLReg(base, data[i]);
    }
}

/**
 *
 * Function Name : CRC_DRV_WriteData16
 * Description   : This function appends a block of bytes to the current 16-bit CRC calculation.
 *
 */
void CRC_DRV_WriteData16(uint32_t instance, const uint16_t *data, uint32_t dataSize)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    OS_ASSERT(data != NULL);
    uint32_t i;
    crc_type_t *base = g_pstCrcBase[instance];

    /* 16-bit writes till end of data buffer */
    for (i = 0U; i < (dataSize / 2); i++) {
        CRC_SetDataLReg(base, data[i]);
    }
}

/**
 *
 * Function Name : CRC_DRV_WriteData32
 * Description   : This function appends a block of bytes to the current 32-bit CRC calculation.
 *
 */
void CRC_DRV_WriteData32(uint32_t instance, const uint32_t *data, uint32_t dataSize)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    OS_ASSERT(data != NULL);
    uint32_t i;
    crc_type_t *base = g_pstCrcBase[instance];

    /* 32-bit writes till end of data buffer */
    for (i = 0U; i < (dataSize / 4); i++) {
        CRC_SetDataReg(base, data[i]);
    }
}

/**
 *
 * Function Name : CRC_DRV_GetCrcResult
 * Description   : This function returns the current result of the CRC calculation.
 *
 */
uint32_t CRC_DRV_GetCrcResult(uint32_t instance)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    const crc_type_t *base = g_pstCrcBase[instance];

    /* Result of the CRC calculation */
    return CRC_GetCrcResult(base);
}

/**
 *
 * Function Name : CRC_DRV_Configure
 * Description   : This function configures the CRC module from a user configuration structure.
 *
 */
status_t CRC_DRV_Configure(uint32_t instance, const crc_user_config_t *userConfigPtr)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    OS_ASSERT(userConfigPtr != NULL);
    crc_type_t *base = g_pstCrcBase[instance];

    /* Set CRC mode */
    CRC_SetProtocolWidth(base, userConfigPtr->crcWidth);
    /* Set transposes options */
    CRC_SetReadTranspose(base, userConfigPtr->readTranspose);
    /* Set CRC polynomial */
    CRC_SetPolyReg(base, userConfigPtr->polynomial);
    /* Set writes transposes */
    CRC_SetWriteTranspose(base, userConfigPtr->writeTranspose);
    /* Sets complement or inversion checksum */
    CRC_SetFXorMode(base, userConfigPtr->complementChecksum);
    /* Write a seed - initial checksum */
    CRC_SetSeedReg(base, userConfigPtr->seed);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : CRC_DRV_GetConfig
 * Description   : This function Get configures of the CRC module currently.
 *
 */
status_t CRC_DRV_GetConfig(uint32_t instance, crc_user_config_t *const userConfigPtr)
{
    OS_ASSERT(instance < CRC_INSTANCE_COUNT);
    OS_ASSERT(userConfigPtr != NULL);
    const crc_type_t *const base = g_pstCrcBase[instance];

    /* Gets CRC mode */
    userConfigPtr->crcWidth = CRC_GetProtocolWidth(base);
    /* Gets transposes and complement options */
    userConfigPtr->readTranspose = CRC_GetReadTranspose(base);
    /* Get a polynomial */
    userConfigPtr->polynomial = CRC_GetPolyReg(base);
    /* Gets transposes options */
    userConfigPtr->writeTranspose = CRC_GetWriteTranspose(base);
    /* Gets complement or inversion checksum */
    userConfigPtr->complementChecksum = CRC_GetFXorMode(base);
    /* Get a seed - initial checksum */
    userConfigPtr->seed = CRC_GetDataReg(base);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : CRC_DRV_GetDefaultConfig
 * Description   : This function Get default configures the CRC module for user configuration structure.
 *
 */
status_t CRC_DRV_GetDefaultConfig(crc_user_config_t *const userConfigPtr)
{
    OS_ASSERT(userConfigPtr != NULL);

    /* Gets CRC mode default is 16 bit */
    userConfigPtr->crcWidth = FEATURE_CRC_DEFAULT_WIDTH;
    /* Gets default a polynomial default is reset value */
    userConfigPtr->polynomial = FEATURE_CRC_DEFAULT_POLYNOMIAL;
    /* Gets default read transposes none */
    userConfigPtr->readTranspose = FEATURE_CRC_DEFAULT_READ_TRANSPOSE;
    /* Gets default write transpose none */
    userConfigPtr->writeTranspose = FEATURE_CRC_DEFAULT_WRITE_TRANSPOSE;
    /* Gets default no complement or inversion checksum */
    userConfigPtr->complementChecksum = FEATURE_CRC_DEFAULT_COMPLEMENT_CHECKSUM;
    /* Gets default a seed - initial checksum */
    userConfigPtr->seed = FEATURE_CRC_DEFAULT_SEED;

    return STATUS_SUCCESS;
}
