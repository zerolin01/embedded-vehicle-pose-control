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
 * @file lora_driver.c
 * @brief lora driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "lora_driver.h"
#include "radio.h"
#include "sx126x.h"

/* 470000000Hz */
#define RF_FREQUENCY                                470000000
/* 22dBm */
#define TX_OUTPUT_POWER                             22
/* 0:125kHz; 1:250kHz; 2:500kHz; 3:Reserved */
#define LORA_BANDWIDTH                              0
/* [SF7..SF12] */
#define LORA_SPREADING_FACTOR                       9
/* 1:4/5, 2:4/6, 3:4/7, 4:4/8 */
#define LORA_CODINGRATE                             1
/* Same for Tx and Rx */
#define LORA_PREAMBLE_LENGTH                        8
/* Symbols */
#define LORA_SYMBOL_TIMEOUT                         5
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
/* rx continue mode(when set Rx Continue Mode, not need judge timeout) */
#define RX_TIMEOUT_VALUE                            0
/* Define the payload size here */
#define BUFFER_SIZE                                 255

static RadioEvents_t RadioEvents;
uint8_t LoRa_TX_buffer[BUFFER_SIZE],LoRa_RX_buffer[BUFFER_SIZE];
uint8_t tx_cnt = 0;
uint8_t rx_cnt = 0;

/**
 *
 * Function Name : OnTxDone
 * Description   : Configures lora send complete.
 *
 */
void OnTxDone(void)
{
    Radio.Rx(RX_TIMEOUT_VALUE);
    printf("lora send.\r\n");
    printf("TxDone.\r\n");
    tx_cnt++;
}

/**
 *
 * Function Name : OnRxDone
 * Description   : Configures lora receive complete.
 *
 */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    memcpy(LoRa_RX_buffer, payload, size > BUFFER_SIZE ? BUFFER_SIZE : size);

    printf("RxDone, size=%d, rssi=%d, snr=%d\r\ndata=", size, rssi, snr);
    for(uint8_t i=0; i<size; i++) {
        printf("%c", payload[i]);
    }
    printf("\r\n");
    rx_cnt++;
}

/**
 *
 * Function Name : OnTxTimeout
 * Description   : Configures lora send timeout.
 *
 */
void OnTxTimeout(void)
{
    Radio.Rx(RX_TIMEOUT_VALUE);
    printf("TxTimeout!\r\n");
}

/**
 *
 * Function Name : OnRxTimeout
 * Description   : Configures lora receive timeout.
 *
 */
void OnRxTimeout(void)
{
    Radio.Rx(RX_TIMEOUT_VALUE);
    printf("RxTimeout!\r\n");
}

/**
 *
 * Function Name : OnRxTimeout
 * Description   : Configures lora receive error.
 *
 */
void OnRxError(void)
{
    Radio.Rx(RX_TIMEOUT_VALUE);
    printf("RxError!\r\n");
}

/**
 *
 * Function Name : LoRa_DRV_Send
 * Description   : Configures lora send data.
 *
 */
void LoRa_DRV_Send(uint8_t *data, uint8_t size)
{
    Radio.Send(data, size);
    Radio.IrqProcess();
}

/**
 *
 * Function Name : LoRa_DRV_Receive
 * Description   : Configures lora receive data.
 *
 */
void LoRa_DRV_Receive(void)
{
    Radio.IrqProcess();
}

/**
 *
 * Function Name : LoRa_DRV_Sleep
 * Description   : Configures lora sleep.
 *
 */
void LoRa_DRV_Sleep(void)
{
    printf("lora sleep.\r\n");
    Radio.Sleep();
    Radio.IrqProcess();
}

/**
 *
 * Function Name : LoRa_DRV_TxContinuousWave
 * Description   : Configures lora send continuous wave.
 *
 */
void LoRa_DRV_TxContinuousWave(void)
{
    printf("lora tx continuous wave.\r\n");
    Radio.IrqProcess();
    Radio.SetTxContinuousWave(RF_FREQUENCY, TX_OUTPUT_POWER, 0xffff);
}

/**
 *
 * Function Name : LoRa_DRV_Init
 * Description   : Configures lora init.
 *
 */
void LoRa_DRV_Init(void)
{
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;
    Radio.Init(&RadioEvents);

    /* use for test spi and lora communication */
    /* uint8_t Test_Reg=0;
    uint8_t Ori_Reg=0;
    Ori_Reg = Radio.Read(0x6c0);
    Radio.Write(0x6c0, 0x11);
    Test_Reg = Radio.Read(0x6c0);
    Radio.Write(0x6c0, Ori_Reg);
    if (Test_Reg != 0x11) {
        printf("lora init fail.\r\n");
        while(1);
    } */

    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
    Radio.SetMaxPayloadLength(MODEM_LORA, BUFFER_SIZE);
    Radio.Rx(RX_TIMEOUT_VALUE);

    printf("lora init finish.\r\n");
}
