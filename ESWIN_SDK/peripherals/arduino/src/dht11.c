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
 * @file dht11.c
 * @brief Examples for DHT11 module of Arduino Port.
 * @author abu (abu@eswincomputing.com)
 * @date 2023-12-13
 *
 * @details
 *
 *
 * @pins
 * | function name | port number | function number | connector |
 * |:------------- |:------------|:----------------|:----------|
 *      GPIOB11    |    PORTB11  |      ALT1       | J13-5
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

#include "arduino.h"
#include <stdio.h>
#include <string.h>

static pin_settings_config_t g_stPinmuxConfigArr_output[1] = {
    {
        .base           = PORTB,
        .pinPortIdx     = 11U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 1,
        .intConfig      = PORT_INT_LOW_LEVEL,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
};

static pin_settings_config_t g_stPinmuxConfigArr_input[1] = {
    {
        .base           = PORTB,
        .pinPortIdx     = 11U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_INPUT_DIRECTION,
        .initValue      = 1,
        .intConfig      = PORT_INT_LOW_LEVEL,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
};

int hum_int;
int hum_dec;
int temp_int;
int temp_dec;

#define OUTPUT 1
#define INPUT 0

static int digitalRead(uint8_t port, pins_channel_type_t pin)
{
    return (PINS_DRV_ReadPins(port) >> pin) & 1;
}
// return values:
//  0 : OK
// -1 : checksum error
// -2 : timeout
// PORTB, 11 ~ D4
static int read_temps(uint8_t port, pins_channel_type_t pin)
{
    // BUFFER TO RECEIVE
    uint8_t bits[5];
    uint8_t cnt = 7;
    uint8_t idx = 0;

    // EMPTY BUFFER
    for (int i=0; i< 5; i++) bits[i] = 0;

    // REQUEST SAMPLE
    PINS_DRV_Init(1, g_stPinmuxConfigArr_output);
    PINS_DRV_WritePinDirection(port, pin, OUTPUT);
    PINS_DRV_WritePin(port, pin, 0);
    OS_DelayMs(18);
    PINS_DRV_WritePin(port, pin, 1);

    OS_DelayUs(30);
    PINS_DRV_Init(1, g_stPinmuxConfigArr_input);
    PINS_DRV_WritePinDirection(port, pin, INPUT);

    // ACKNOWLEDGE or TIMEOUT
    unsigned int loopCnt = 10000;
    while(digitalRead(port, pin) == 0)
        if (loopCnt-- == 0) return -2;

    loopCnt = 10000;
    while(digitalRead(port, pin) == 1)
        if (loopCnt-- == 0) return -3;

    uint32_t uS2Ticks  = OS_Msec2Tick(40)/1000;
    // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
    for (int i=0; i<40; i++)
    {
        loopCnt = 10000;
        while(digitalRead(port, pin) == 0)
            if (loopCnt-- == 0) return -4;
        uint32_t timeStart      = 0U;
        uint32_t timeElapsed    = 0U;
        timeStart = OS_KernelGetTickCount();

        loopCnt = 10000;
        while(digitalRead(port, pin) == 1)
            if (loopCnt-- == 0) return -5;

        timeElapsed = OS_KernelGetTickCount() - timeStart;
        if (timeElapsed > uS2Ticks) bits[idx] |= (1 << cnt);
        if (cnt == 0)   // next byte?
        {
            cnt = 7;    // restart at MSB
            idx++;      // next byte!
        }
        else cnt--;
    }

    // WRITE TO RIGHT VARS
    hum_int = bits[0];
    hum_dec = bits[1];
    temp_int = bits[2];
    temp_dec = bits[3];

    uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];

    if (bits[4] != sum) return -1;
    return 0;
}

/**
 * @brief The Arduino DHT11 Example.
 *
 * @return void.
 *
 */
void Arduino_Dht11_Example(void)
{
    OS_DelayMs(30);

    /* connect dht11 to portb11 */
    int chk = read_temps(PORTB, 11);  // D4
    if (chk != 0) {
        printf("readerror:%d\r\n", chk);
    } else {
        printf("Humidity: %d.%d, Temperature: %d.%d\r\n", hum_int, hum_dec, temp_int, temp_dec);
    }
}
