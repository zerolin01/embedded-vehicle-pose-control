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
 * @file sd_hw_access.c
 * @brief SD driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "sd_hw_access.h"

/**
 *
 * Function Name : SD_SPI_WirteAndReadOneByte
 * Description   : Send a byte of data.
 *
 * This function is used for spi to send one byte of data and return the received data.
 *
 * @return uint8_t rxdata.
 */
uint8_t SD_SPI_WirteAndReadOneByte(uint32_t instance, uint8_t txdata)
{
    uint8_t rxdata = 0;
    SPI_DRV_MasterTransferBlocking(instance, &txdata, &rxdata, sizeof(rxdata), 1000);
    return  rxdata;
}

/**
 *
 * Function Name : SD_WaitReady
 * Description   : Sd card waiting ready.
 *
 * This function is used for sd card waiting ready.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_WaitReady(uint32_t instance)
{
    uint32_t wait_Count = 0;

    do {
        if (SD_SPI_WirteAndReadOneByte(instance, sdInitTxData) == sdInitRxData) {
            return SD_SUCCESS;
        }

        wait_Count++;
    } while (wait_Count < SD_WAIT_READY_DELAY);

    return SD_ERROR;
}

/**
 *
 * Function Name : SD_SendCommand
 * Description   : Send a command to the SD card.
 *
 * This function is used for send a command to the SD card.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_SendCommand(uint32_t instance, uint8_t command, uint32_t arg)
{
    uint8_t return_Val;
    uint8_t retry_Count;
    uint8_t crc_Val = COMMAND_BYTE6_CRC_MASK;

    if (command & CMD_AND_ACMD_DIS_VALUE) {
        command &= APPCOMMAND_CLEAR_VALUE;

        return_Val = SD_SendCommand(instance, CMD55, 0);
        if (return_Val > 1)
            return return_Val;
    }

    if (command != CMD12) {
        if (SD_WaitReady(instance) != STATUS_SUCCESS) {
            return 0xFF;
        }
    }

    SD_SPI_WirteAndReadOneByte(instance, command | COMMAND_BYTE1_CMD_MASK);
    SD_SPI_WirteAndReadOneByte(instance, arg >> COMMAND_BYTE2_AGR_MASK);
    SD_SPI_WirteAndReadOneByte(instance, arg >> COMMAND_BYTE3_AGR_MASK);
    SD_SPI_WirteAndReadOneByte(instance, arg >> COMMAND_BYTE4_AGR_MASK);
    SD_SPI_WirteAndReadOneByte(instance, arg >> COMMAND_BYTE5_AGR_MASK);

    if (command == CMD0)
        crc_Val = COMMAND_CMD0_CRC_MASK;

    if (command == CMD8)
        crc_Val = COMMAND_CMD8_CRC_MASK;

    SD_SPI_WirteAndReadOneByte(instance, crc_Val);


    if (command == CMD12) {
        SD_SPI_WirteAndReadOneByte(instance, sdInitTxData);
    }

    retry_Count = 20;

    do {
        return_Val = SD_SPI_WirteAndReadOneByte(instance, sdInitTxData);
    } while ((return_Val & 0x80) && retry_Count--);

    return return_Val;
}

/**
 *
 * Function Name : SD_GetResponse
 * Description   : Wait for the sd card to respond.
 *
 * This function wait the SD card respond value.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_GetResponse(uint32_t instance, uint8_t response)
{
    uint16_t count_Val = SD_WAIT_RESPONSE_COUNT;

    while ((SD_SPI_WirteAndReadOneByte(instance, 0xFF) != response) && count_Val) {
        count_Val--;
    }

    if (count_Val == 0) {
        return SD_ERROR;
    }

    return SD_SUCCESS;
}

/**
 *
 * Function Name : SD_ReceiveData
 * Description   : Data is read from the sd card once.
 *
 * This function completes the SD card read one byte process.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_ReceiveData(uint32_t instance, uint8_t *buffer, int16_t length)
{
    if (SD_GetResponse(instance, SD_RESPONSE_READ_VALUE)) {
        return SD_ERROR;
    }

    while (length--) {
        *buffer = SD_SPI_WirteAndReadOneByte(instance, sdInitTxData);
        buffer++;
    }

    SD_SPI_WirteAndReadOneByte(instance, sdInitTxData);
    SD_SPI_WirteAndReadOneByte(instance, sdInitTxData);
    return SD_SUCCESS;
}

/**
 *
 * Function Name : SD_SendBlock
 * Description   : Send data to sd card.
 *
 * This function completes send data toSD card.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_SendBlock(uint32_t instance, uint8_t *buffer, uint8_t command)
{
    uint16_t byteCount;
    uint8_t  return_Val;

    if (SD_WaitReady(instance)) {
        return SD_ERROR;
    }

    SD_SPI_WirteAndReadOneByte(instance, command);

    if (command != SD_END_SEND_MULTI_BLOCK_TOKEN) {
        for (byteCount = 0; byteCount < 512; byteCount++) {
            SD_SPI_WirteAndReadOneByte(instance, buffer[byteCount]);
        }

        SD_SPI_WirteAndReadOneByte(instance, 0xFF);
        SD_SPI_WirteAndReadOneByte(instance, 0xFF);
        return_Val = SD_SPI_WirteAndReadOneByte(instance, 0xFF);

        if ((return_Val & SD_SEND_MULTI_BLOCK_RESPONSE_MASK) != SD_SEND_MULTI_BLOCK_SUCCESS) {
            return SD_ERROR;
        }
    }
    return SD_SUCCESS;
}

/**
 *
 * Function Name : SD_GetCSD
 * Description   : Get the CSD information of the SD card.
 *
 * This function get the CSD information of the SD card, including capacity and speed information.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_GetCSD(uint32_t instance, uint8_t *csd_data)
{
    uint8_t return_Val;
    return_Val = SD_SendCommand(instance, CMD9, COMMAND_CMD9_ARG);

    if (return_Val == 0) {
        return_Val = SD_ReceiveData(instance, csd_data, 16);
    }

    return return_Val;
}

/**
 *
 * Function Name : SD_GetStatus
 * Description   : Get the status of the SD card.
 *
 * This function get the status of the SD card.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_GetStatus(uint32_t instance)
{
    uint8_t return_Val;
    uint8_t retry_Count = 20;

    do {
        return_Val = SD_SendCommand(instance, ACMD13, COMMAND_ACMD13_ARG);
    } while (COMMAND_ACMD13_ARG && retry_Count--);

    return return_Val;
}

/**
 *
 * Function Name : SD_GetCID
 * Description   : Obtain the CID information of the SD card.
 *
 * This function obtain the CID information of the SD card, including the manufacturer information.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_GetCID(uint32_t instance, uint8_t *cid_data)
{
    uint8_t return_Val;
    return_Val = SD_SendCommand(instance, CMD10, COMMAND_CMD10_ARG);

    if (return_Val == 0x0) {
        return_Val = SD_ReceiveData(instance, cid_data, 16);
    }

    return return_Val;
}
