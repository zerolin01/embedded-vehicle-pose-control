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
 * @file spi_sd.c
 * @brief Examples for SPI module of RaspPi Port.
 * @author
 * @date 2023-12-13
 *
 * @details
 *
 *
 * @pins
 * | function name | port number | function number | connector |
 * |:------------- |:------------|:----------------|:----------|
 *     SPI3 SIN    |    PTB25    |      ALT5       | J12-19
 *     SPI3 SOUT   |    PTB26    |      ALT5       | J12-21
 *     SPI3 SCK    |    PTC3     |      ALT5       | J12-23
 *     SPI3 PCS0   |    PTB13    |      ALT5       | J12-24
 *
 * @connections
 *
 * @attention
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "rasppi.h"

/**
 * @brief The RaspPi SPI SD Example.
 *
 * @return void
 * true if run succeed
 * false if run failed
 */
bool RaspPi_SpiSd_Example(void)
{
    uint32_t capacity;
    uint32_t sectors;
    uint8_t val;
    uint8_t send_buf[512] = {0};
    uint8_t read_buf[512] = {0};
    uint8_t send_buf_multi[1024] = {0};
    uint8_t read_buf_multi[1024] = {0};

    for (int i = 0; i < 512; i++) {
        send_buf[i] = i;
    }

    for (int i = 0; i < 1024; i++) {
        send_buf_multi[i] = i;
    }

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr);

    SPI_DRV_MasterInit(INST_SPI_3, &g_stSpiState_3, &g_stSpi3MasterConfig0);

    val = SD_DRV_Init(INST_SPI_3);
    if (val != 0) return false;

    sectors = SD_DRV_GetSecOrCapCount(INST_SPI_3, 0);
    if (sectors == 0) return false;

    capacity = SD_DRV_GetSecOrCapCount(INST_SPI_3, 1);
    if (capacity == 0) return false;

    val = SD_DRV_WriteDisk(INST_SPI_3, send_buf, 0, 1);
    if (val != 0) return false;

    val = SD_DRV_ReadDisk(INST_SPI_3, read_buf, 0, 1);
    if (val != 0) return false;

    for (int i = 0; i < 512; i++) {
       if (read_buf[i] != send_buf[i]) {
           return false;
       }
    }

    val = SD_DRV_WriteDisk(INST_SPI_3, send_buf_multi, 5, 2);
    if (val != 0) return false;

    val = SD_DRV_ReadDisk(INST_SPI_3, read_buf_multi, 5, 2);
    if (val != 0) return false;

    for (int i = 0; i < 512; i++) {
        if (read_buf_multi[i] != send_buf_multi[i]) {
            return false;
        }
    }

    SPI_DRV_MasterDeinit(INST_SPI_3);
    return true;
}
