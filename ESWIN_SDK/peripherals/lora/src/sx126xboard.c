/*!
 * \file      sx1262mbxcas-board.c
 *
 * \brief     Target board SX1262MBXCAS shield driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
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
 */
#include <stdlib.h>
#include "radio.h"
#include "sx126xboard.h"
#include "osal.h"
#include "spi_master_driver.h"

uint8_t SPI_WriteBuffer[256+4];
uint8_t SPI_ReadBuffer[256+4];

/*!
 * Flag used to mark if basic io init have finished.
 */
static bool LoRaIOInitFlag = false;


#if defined( USE_RADIO_DEBUG )
/*!
 * \brief Writes new Tx debug pin state
 *
 * \param [IN] state Debug pin state
 */
static void SX126xDbgPinTxWrite( uint8_t state );

/*!
 * \brief Writes new Rx debug pin state
 *
 * \param [IN] state Debug pin state
 */
static void SX126xDbgPinRxWrite( uint8_t state );
#endif

/*!
 * \brief Holds the internal operating mode of the radio
 */
static RadioOperatingModes_t OperatingMode;
DioIrqHandler *DIOx_IRQ_Function;

#define LORA_SPI                                    1
#define BOARD_TCXO_WAKEUP_TIME                      5

void SX126xIoInit( void )
{
}

void SX126xIoIrqInit( DioIrqHandler dioIrq )
{
    DIOx_IRQ_Function=dioIrq;

    if(DIOx_IRQ_Function != NULL) {
        /* System interrupt type configuration. */
        OS_RegisterType_t type;
        type.trig_mode = CLIC_LEVEL_TRIGGER;
        type.lvl       = 1;
        type.priority  = 0;
        type.data_ptr  = NULL;

        /* Set system interrupt request for PORTD04. */
        OS_RequestIrq(GPIOD_4_IRQn, dioIrq, &type);
        /* Enable system interrupt for PORTD04. */
        OS_EnableIrq(GPIOD_4_IRQn);
    } else {
        OS_DisableIrq(GPIOD_4_IRQn);
    }

    LoRaIOInitFlag = true;
}

void SX126xIoDbgInit( void )
{
#if defined( USE_RADIO_DEBUG )
    // GPIO_InitTypeDef GPIO_InitStruct = {0};

    // /* GPIO Ports Clock Enable */
    // __HAL_RCC_GPIOA_CLK_ENABLE();

    // /*Configure GPIO pin Output Level */
    // HAL_GPIO_WritePin(GPIOA, DBG_PIN_TX_Pin|DBG_PIN_RX_Pin, GPIO_PIN_RESET);

    // /*Configure GPIO pins : DBG_PIN_TX_Pin DBG_PIN_RX_Pin as output */
    // GPIO_InitStruct.Pin = DBG_PIN_TX_Pin|DBG_PIN_RX_Pin;
    // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
}

void SX126xReset( void )
{
    OS_DelayMs( 10 );
    PINS_DRV_WritePin(PORTC, 10, 0);
    OS_DelayMs( 20 );
    PINS_DRV_WritePin(PORTC, 10, 1);
    OS_DelayMs( 10 );
}

void SX126xWaitOnBusy( void )
{
    uint8_t val = (uint8_t)(PINS_DRV_ReadPins(PORTD) & ((uint32_t)(1 << 2)));
    while( 1 == val);
}

void SX126xWakeup( void )
{
    SPI_WriteBuffer[0] = RADIO_GET_STATUS;
    SPI_WriteBuffer[1] =  0;
    SPI_DRV_MasterTransferBlocking(LORA_SPI, SPI_WriteBuffer, SPI_ReadBuffer, 2, 500);

    // Wait for chip to be ready.
    SX126xWaitOnBusy( );

    // Update operating mode context variable
    SX126xSetOperatingMode( MODE_STDBY_RC );
}

void SX126xWriteCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{
    SX126xCheckDeviceReady( );

    SPI_WriteBuffer[0] = command ;
    memcpy(&SPI_WriteBuffer[1],buffer,size);
    SPI_DRV_MasterTransferBlocking(LORA_SPI, SPI_WriteBuffer, SPI_ReadBuffer, size+1, 1000);

    if( command != RADIO_SET_SLEEP ) {
        SX126xWaitOnBusy( );
    }
}

uint8_t SX126xReadCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{
    uint8_t status = 0;
    SX126xCheckDeviceReady( );

    SPI_WriteBuffer[0] = command;
    memset(&SPI_WriteBuffer[1],0,size+1);
    SPI_DRV_MasterTransferBlocking(LORA_SPI, SPI_WriteBuffer, SPI_ReadBuffer, size+2, 500);

    status = SPI_ReadBuffer[1];
    memcpy(buffer,&SPI_ReadBuffer[2],size);
    SX126xWaitOnBusy( );
    return status;
}

void SX126xWriteRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
    SX126xCheckDeviceReady( );

    SPI_WriteBuffer[0] = RADIO_WRITE_REGISTER;
    SPI_WriteBuffer[1] = ( address & 0xFF00 ) >> 8;
    SPI_WriteBuffer[2] = address & 0x00FF;
    memcpy(&SPI_WriteBuffer[3],buffer,size);
    SPI_DRV_MasterTransferBlocking(LORA_SPI, SPI_WriteBuffer, SPI_ReadBuffer, size+3, 500);

    SX126xWaitOnBusy( );
}

void SX126xReadRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
    SX126xCheckDeviceReady( );

    SPI_WriteBuffer[0] = RADIO_READ_REGISTER;
    SPI_WriteBuffer[1] = ( address & 0xFF00 ) >> 8;
    SPI_WriteBuffer[2] = address & 0x00FF;
    memset(&SPI_WriteBuffer[3],0,size+1);
    SPI_DRV_MasterTransferBlocking(LORA_SPI, SPI_WriteBuffer, SPI_ReadBuffer, size+4, 500);

    memcpy(buffer,&SPI_ReadBuffer[4],size);
    SX126xWaitOnBusy( );
}

void SX126xWriteBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
    SX126xCheckDeviceReady();

    SPI_WriteBuffer[0] = RADIO_WRITE_BUFFER;
    SPI_WriteBuffer[1] = offset;
    memcpy(&SPI_WriteBuffer[2],buffer,size);
    SPI_DRV_MasterTransferBlocking(LORA_SPI, SPI_WriteBuffer, SPI_ReadBuffer, size+2, 500);

    SX126xWaitOnBusy();
}

void SX126xReadBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
    SX126xCheckDeviceReady( );

    SPI_WriteBuffer[0] = RADIO_READ_BUFFER;
    SPI_WriteBuffer[1] = offset;
    memset(&SPI_WriteBuffer[2],0,size+1);
    SPI_DRV_MasterTransferBlocking(LORA_SPI, SPI_WriteBuffer, SPI_ReadBuffer, size+3, 500);

    memcpy(buffer,&SPI_ReadBuffer[3],size);
    SX126xWaitOnBusy( );
}

uint32_t SX126xGetDio1PinState( void )
{
    uint8_t val = (uint8_t)(PINS_DRV_ReadPins(PORTD) & ((uint32_t)(1 << 4)));
    if (1 == val) {
        return 1;
    } else {
        return 0;
    }
}

#if defined( USE_RADIO_DEBUG )
static void SX126xDbgPinTxWrite( uint8_t state )
{
        if(state)
        {
            HAL_GPIO_WritePin(DBG_PIN_TX_GPIO_Port, DBG_PIN_TX_Pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(DBG_PIN_TX_GPIO_Port, DBG_PIN_TX_Pin, GPIO_PIN_RESET);
        }
}

static void SX126xDbgPinRxWrite( uint8_t state )
{
        if(state)
        {
            HAL_GPIO_WritePin(DBG_PIN_RX_GPIO_Port, DBG_PIN_RX_Pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(DBG_PIN_RX_GPIO_Port, DBG_PIN_RX_Pin, GPIO_PIN_RESET);
        }
}
#endif

void SX126xIoTcxoInit( void )
{

}

uint32_t SX126xGetBoardTcxoWakeupTime( void )
{
    return BOARD_TCXO_WAKEUP_TIME;
}
void SX126xAntSwTx( void )
{
    PINS_DRV_WritePin(PORTC, 19, 0);
    PINS_DRV_WritePin(PORTC, 21, 1);
}
void SX126xAntSwRx( void )
{
    PINS_DRV_WritePin(PORTC, 19, 1);
    PINS_DRV_WritePin(PORTC, 21, 0);
}
void SX126xAntSwOff( void )
{
    PINS_DRV_WritePin(PORTC, 19, 0);
    PINS_DRV_WritePin(PORTC, 21, 0);
}

void SX126xIoRfSwitchInit( void )
{
   SX126xSetDio2AsRfSwitchCtrl( false );
}

RadioOperatingModes_t SX126xGetOperatingMode( void )
{
    return OperatingMode;
}

void SX126xAntSwOn( void )
{

}

void SX126xSetOperatingMode( RadioOperatingModes_t mode )
{
    OperatingMode = mode;

    switch( mode )
    {
        case MODE_TX:
            SX126xAntSwTx();
#if defined( USE_RADIO_DEBUG )
            SX126xDbgPinTxWrite( 1 );
            SX126xDbgPinRxWrite( 0 );
#endif
            break;
        case MODE_RX:
        case MODE_RX_DC:
            case MODE_CAD:
                SX126xAntSwRx();
#if defined( USE_RADIO_DEBUG )
            SX126xDbgPinTxWrite( 0 );
            SX126xDbgPinRxWrite( 1 );
#endif
            break;
        default:
            SX126xAntSwOff();
#if defined( USE_RADIO_DEBUG )
            SX126xDbgPinTxWrite( 0 );
            SX126xDbgPinRxWrite( 0 );
#endif
            break;
    }
}

void SX126xIoDeInit( void )
{

}

void SX126xWriteRegister( uint16_t address, uint8_t value )
{
    SX126xWriteRegisters( address, &value, 1 );
}

uint8_t SX126xReadRegister( uint16_t address )
{
    uint8_t data;
    SX126xReadRegisters( address, &data, 1 );
    return data;
}

void SX126xSetRfTxPower( int8_t power )
{
    SX126xSetTxParams( power, RADIO_RAMP_40_US );
}

uint8_t SX126xGetDeviceId( void )
{
   return SX1262;
}

bool SX126xCheckRfFrequency( uint32_t frequency )
{
    return true;
}
