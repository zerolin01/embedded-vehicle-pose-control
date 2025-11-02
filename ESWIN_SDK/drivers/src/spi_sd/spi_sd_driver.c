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
 * @file sd_driver.c
 * @brief sd driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "spi_sd_driver.h"
#include "clock_driver.h"
#include "sd_hw_access.h"
#include "sd_register.h"
#include <string.h>

/**
 *
 * Function Name : SD_DRV_Init
 * Description   : Initializes a SPI instance for a sd mode operation.
 *
 * This function completes the SD card initialization process.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_DRV_Init(uint32_t instance)
{
    uint8_t  return_Val;
    uint16_t retry_Count;
    uint8_t  response_Val[SD_COMMAND_RESPONSE_SIZE];
    uint16_t count;
    uint8_t  command_Val;

    for (count = 0; count < 0xf; count++) {
        SD_SPI_WirteAndReadOneByte(instance, sdInitTxData);
    }

    retry_Count = 20;

    do {
        return_Val = SD_SendCommand(instance, CMD0, COMMAND_CMD0_ARG);
    } while ((return_Val != SD_IDLE) && retry_Count--);

    sdType = 0;

    if (return_Val == SD_IDLE) {
        if (SD_SendCommand(instance, CMD8, COMMAND_CMD8_ARG) == 1) {
            for (count = 0; count < 4; count++) {
                response_Val[count] = SD_SPI_WirteAndReadOneByte(instance, sdInitTxData);
            }

            if ((response_Val[2] == COMMAND_CMD8_R7_VOLTAGE_ACCEPTED) && (response_Val[3] == COMMAND_CMD8_R7_CHECK_PATTERN)) {
                retry_Count = 5000;

                do {
                    return_Val = SD_SendCommand(instance, ACMD41, COMMAND_ACMD41_ARG_HCS);
                } while (return_Val != 0x0 && retry_Count--);

                return_Val =  SD_SendCommand(instance, CMD58, COMMAND_CMD58_ARG);
                if (retry_Count && (return_Val == 0)) {
                    for (count = 0; count < 4; count++) {
                        response_Val[count] = SD_SPI_WirteAndReadOneByte(instance, sdInitTxData);
                    }

                    if (response_Val[0] & SD_OCR_CCS_MASK) {
                        sdType = SD_TYPE_V2HC;
                    } else {
                        sdType = SD_TYPE_V2;
                    }
                }
            }
        } else {
            return_Val = SD_SendCommand(instance, ACMD41, 0);
            retry_Count = 1000;

            if (return_Val <= 1) {
                sdType = SD_TYPE_V1;
                command_Val = ACMD41;
            } else {
                sdType = SD_TYPE_MMC;
                command_Val = CMD1;
            }

            do {
                return_Val = SD_SendCommand(instance, command_Val, 0);
            } while (return_Val && retry_Count--);

            if ((retry_Count == 0) && (SD_SendCommand(instance, CMD16, COMMAND_CMD16_ARG) != 0)) {
                sdType = SD_TYPE_ERR;
            }
        }
    }

    if (sdType) {
        return SD_SUCCESS;
    }

    return return_Val;
}

/**
 *
 * Function Name : SD_DRV_ReadDisk
 * Description   : Read sd card.
 *
 * This function completes the SD card read process.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_DRV_ReadDisk(uint32_t instance, uint8_t *buffer, uint32_t addr, uint32_t blockCount)
{
    uint8_t   return_Val;
    long long laddr = addr;

    if (sdType != SD_TYPE_V2HC) {
        laddr <<= 9;
    }

    if (blockCount == 1) {
        return_Val = SD_SendCommand(instance, CMD17, laddr);

        if (return_Val == 0) {
            return_Val = SD_ReceiveData(instance, buffer, SD_DEFAULT_SIZE);
        }
    } else {
        return_Val = SD_SendCommand(instance, CMD18, laddr);

        do {
            return_Val = SD_ReceiveData(instance, buffer, SD_DEFAULT_SIZE);
            buffer += SD_DEFAULT_SIZE;
        } while ((--blockCount) && (return_Val == 0));

        SD_SendCommand(instance, CMD12, COMMAND_CMD12_ARG);
    }

    return return_Val;
}

/**
 *
 * Function Name : SD_DRV_WriteDisk
 * Description   : Write sd card.
 *
 * This function completes the SD card write process.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_DRV_WriteDisk(uint32_t instance, uint8_t *buffer, uint32_t addr, uint32_t blockCount)
{
    uint8_t   retryCount = 20;
    uint8_t   return_Val = 0;
    long long laddr = addr;

    if (sdType != SD_TYPE_V2HC) {
        laddr <<= 9;
    }

    if (blockCount == 1) {
        return_Val = SD_SendCommand(instance, CMD24, laddr);

        if (return_Val == 0) {
            return_Val = SD_SendBlock(instance, buffer, SD_SEND_GINGLE_BLOCK_TOKEN);
        }
    } else {
        if (sdType != SD_TYPE_MMC) {
            do {
            	return_Val = SD_SendCommand(instance, ACMD23, blockCount);
            } while (return_Val && retryCount--);

            return_Val = SD_SendCommand(instance, CMD25, laddr);

            if (return_Val == 0) {
                do {
                    return_Val = SD_SendBlock(instance, buffer, SD_SEND_MULTI_BLOCK_TOKEN);
                    buffer += SD_DEFAULT_SIZE;
                } while ((--blockCount) && (return_Val == 0));

                return_Val = SD_SendBlock(instance, 0, SD_END_SEND_MULTI_BLOCK_TOKEN);
            }
        }
    }

    return return_Val;
}

/**
 *
 * Function Name : SD_DRV_GetSecOrCapCount
 * Description   : Get the total number of sectors of the sd card or the number of bytes of actual capacity.
 *
 * This function completes get the sd card sectors or capacity process.
 * When flag = 0/1, return sectors/capacity.
 *
 * @return sectors or capacity(MB).
 */
uint32_t SD_DRV_GetSecOrCapCount(uint32_t instance, uint8_t flag)
{
    uint8_t  CSD[16] = {0};
    uint32_t return_Val;
    uint8_t  n;
    uint16_t csize;

    if (SD_GetCSD(instance, CSD) != 0) {
        return 0;
    }

    if ((CSD[0] & 0xC0) == 0x40) {
        /* V2.0*/
        csize = CSD[9] + ((uint16_t)CSD[8] << 8) + ((uint32_t)(CSD[7] & 63) << 16) + 1;
        return_Val = (uint32_t)csize << 10;
    } else {
        /* V1.XX or MCC or V3*/
        n = (CSD[5] & 15) + ((CSD[10] & 128) >> 7) + ((CSD[9] & 3) << 1) + 2;
        csize = (CSD[8] >> 6) + ((uint16_t)CSD[7] << 2) + ((uint16_t)(CSD[6] & 3) << 10) + 1;
        return_Val = (uint32_t)csize << (n - 9);
    }

    if (flag == 0x0) {
        return return_Val;
    } else {
        return (return_Val >> 11);
    }
}
