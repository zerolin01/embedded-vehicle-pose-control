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
 * @file sd_hw_access.h
 * @brief SD driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#ifndef __SD_HW_ACCESS_H__
#define __SD_HW_ACCESS_H__

#include "sd_register.h"
#include "pins_driver.h"
#include "spi_master_driver.h"
#include "spi_common.h"
#include "spi_hw_access.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* The response value size obtained after the command is sent. */
#define SD_COMMAND_RESPONSE_SIZE       4
/* Sd card idle value. */
#define SD_IDLE                        0x01
/* Delay value of sd card ready. */
#define SD_WAIT_READY_DELAY            0xFFFFFF
/* Spi sends this variable to get the sd response value. */
static volatile uint8_t sdInitTxData = 0xFF;
static volatile uint8_t sdInitRxData = 0xFF;
/* Sd card type. */
static volatile uint8_t sdType;

/**
 *
 * Function Name : SD_SPI_WirteAndReadOneByte
 * Description   : Send a byte of data.
 *
 * This function is used for spi to send one byte of data and return the received data.
 *
 * @return uint8_t rxdata.
 */
uint8_t SD_SPI_WirteAndReadOneByte(uint32_t instance, uint8_t txdata);

/**
 *
 * Function Name : SD_WaitReady
 * Description   : Sd card waiting ready.
 *
 * This function is used for sd card waiting ready.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_WaitReady(uint32_t instance);

/**
 *
 * Function Name : SD_SendCommand
 * Description   : Send a command to the SD card.
 *
 * This function is used for send a command to the SD card.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_SendCommand(uint32_t instance, uint8_t command, uint32_t arg);

/**
 *
 * Function Name : SD_GetResponse
 * Description   : Wait for the sd card to respond.
 *
 * This function wait the SD card respond value.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_GetResponse(uint32_t instance, uint8_t response);

/**
 *
 * Function Name : SD_ReceiveData
 * Description   : Data is read from the sd card once.
 *
 * This function completes the SD card read one byte process.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_ReceiveData(uint32_t instance, uint8_t *buffer, int16_t length);

/**
 *
 * Function Name : SD_SendBlock
 * Description   : Send data to sd card.
 *
 * This function completes send data toSD card.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_SendBlock(uint32_t instance, uint8_t *buffer, uint8_t command);

/**
 *
 * Function Name : SD_GetCSD
 * Description   : Get the CSD information of the SD card.
 *
 * This function get the CSD information of the SD card, including capacity and speed information.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_GetCSD(uint32_t instance, uint8_t *csd_data);

/**
 *
 * Function Name : SD_GetStatus
 * Description   : Get the status of the SD card.
 *
 * This function get the status of the SD card.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_GetStatus(uint32_t instance);

/**
 *
 * Function Name : SD_GetCID
 * Description   : Obtain the CID information of the SD card.
 *
 * This function obtain the CID information of the SD card, including the manufacturer information.
 *
 * @return SD_ERROR or SD_SUCCESS.
 */
uint8_t SD_GetCID(uint32_t instance, uint8_t *cid_data);


#endif /* __SD_HW_ACCESS_H__ */
