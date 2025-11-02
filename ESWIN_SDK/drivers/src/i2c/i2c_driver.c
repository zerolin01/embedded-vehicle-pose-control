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
 * @file i2c_driver.c
 * @brief I2C driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "clock_driver.h"
#include "i2c_hw_access.h"

/** Constraints used for baud rate computation. */
#define CLKHI_MIN_VALUE   (1U)
#define CLKLO_MIN_VALUE   (3U)
#define CLKHI_MAX_VALUE   ((1U << I2C_MCCR0_CLKHI_WIDTH) - 1U)
#define CLKLO_MAX_VALUE   ((1U << I2C_MCCR0_CLKLO_WIDTH) - 1U)
#define DATAVD_MIN_VALUE  (1U)
#define SETHOLD_MIN_VALUE (2U)
#define BUSIDLE_MAX_VALUE (0xFFFU)

/** Table of base addresses for I2C instances. */
static i2c_type_t *const g_i2cBase[I2C_INSTANCE_COUNT] = I2C_BASE_PTRS;

/** I2C PDMA request sources. */
static const uint8_t g_i2cDMASrc[I2C_INSTANCE_COUNT][2] = I2C_PDMA_REQ;

/** Pointer to runtime state structure.*/
static i2c_master_state_t *g_i2cMasterStatePtr[I2C_INSTANCE_COUNT] = {NULL};
static i2c_slave_state_t *g_i2cSlaveStatePtr[I2C_INSTANCE_COUNT]   = {NULL};

/** Table for i2c IRQ numbers. */
static const IRQn_Type g_i2cMasterIrqId[I2C_INSTANCE_COUNT] = I2C_MASTER_IRQS;
static const IRQn_Type g_i2cSlaveIrqId[I2C_INSTANCE_COUNT]  = I2C_SLAVE_IRQS;

/** PCC clock sources, for getting the input clock frequency. */
static const clock_names_t g_i2cClock[I2C_INSTANCE_COUNT] = I2C_PCC_CLOCKS;

/** Callback for master PDMA transfer done. */
static void I2C_MasterCompleteDMATransfer(void *parameter, pdma_chn_status_t status);

/**
 * @brief Direction enum of a I2C transfer: transmit or receive.
 * 
 */
typedef enum {
    I2C_TX_REQ = 0, /**< The driver will perform an I2C transmit transfer. */
    I2C_RX_REQ = 1, /**< The driver will perform an I2C receive transfer. */
} i2c_transfer_direction_t;

/**
 * @brief I2C PDMA internal parameters structure.
 *
 * This structure is used in PDMA transfers. It contains different
 * variables required for setting and maintaining a PDMA transfer.
 *
 */
typedef struct {
    /* DRIVER_INTERNAL_USE_ONLY */
    uint8_t dmaChannel;                         /**< Channel number for the PDMA channel. */
    pdma_transfer_type_t dmaTransferType;       /**< Type for the PDMA transfer. */
    uint32_t i2cDataRegAddr;                    /**< An i2c data register address. */
    uint8_t *bufferTransfer;                    /**< Buffer used for transfer. */
    uint32_t transferSize;                      /**< Size of the data to be transferred. */
    i2c_transfer_direction_t transferDirection; /**< Tells if the driver will make a receive or transmit PDMA transfer. */
} i2c_dma_transfer_params_t;

/**
 *
 * Function Name : I2C_MasterCmdQueueEmpty
 * Description   : Checks if there are any commands in the master software queue.
 *
 */
static inline bool I2C_MasterCmdQueueEmpty(const i2c_master_state_t *master)
{
    OS_ASSERT(master != NULL);

    return (master->cmdQueue.writeIdx == master->cmdQueue.readIdx);
}

/**
 *
 * Function Name : I2C_MasterResetQueue
 * Description   : Resets the master software queue.
 *
 */
static inline void I2C_MasterResetQueue(i2c_master_state_t *master)
{
    OS_ASSERT(master != NULL);

    master->cmdQueue.readIdx  = 0U;
    master->cmdQueue.writeIdx = 0U;
}

/**
 *
 * Function Name : I2C_MasterQueueCmd
 * Description   : Queues a command in the hardware FIFO or in the master software queue.
 *
 */
static inline void I2C_MasterQueueCmd(i2c_type_t *baseAddr, i2c_master_state_t *master, i2c_master_command_t cmd,
                                      uint8_t data)
{
    OS_ASSERT(master != NULL);
    OS_ASSERT(baseAddr != NULL);

    uint16_t txFIFOCount = I2C_GetMasterTxFIFOCount(baseAddr);
    uint16_t txFIFOSize  = I2C_GetMasterTxFIFOSize(baseAddr);

    /* Check if there is room in the hardware FIFO. */
    if (txFIFOCount < txFIFOSize) {
        I2C_CmdMasterTransmit(baseAddr, cmd, data);
    } else {
        /* Hardware FIFO full, use software FIFO. */
        OS_ASSERT(master->cmdQueue.writeIdx < I2C_MASTER_CMD_QUEUE_SIZE);

        master->cmdQueue.cmd[master->cmdQueue.writeIdx]  = cmd;
        master->cmdQueue.data[master->cmdQueue.writeIdx] = data;
        master->cmdQueue.writeIdx++;
    }
}

/**
 *
 * Function Name : I2C_MasterSendQueuedCmd
 * Description   : Transfers commands from the master software queue to the hardware FIFO.
 *
 */
static inline void I2C_MasterSendQueuedCmd(i2c_type_t *baseAddr, i2c_master_state_t *master)
{
    OS_ASSERT(master != NULL);
    OS_ASSERT(baseAddr != NULL);

    uint16_t txFIFOCount = I2C_GetMasterTxFIFOCount(baseAddr);
    uint16_t txFifoSize  = I2C_GetMasterTxFIFOSize(baseAddr);

    while ((!I2C_MasterCmdQueueEmpty(master)) && (txFIFOCount < txFifoSize)) {
        I2C_CmdMasterTransmit(baseAddr, master->cmdQueue.cmd[master->cmdQueue.readIdx],
                              master->cmdQueue.data[master->cmdQueue.readIdx]);
        master->cmdQueue.readIdx++;

        txFIFOCount = I2C_GetMasterTxFIFOCount(baseAddr);
    }

    if (I2C_MasterCmdQueueEmpty(master)) {
        /* Reset queue. */
        I2C_MasterResetQueue(master);
    }
}

/**
 *
 * Function Name : I2C_MasterSendAddress
 * Description   : Send start event and slave address parameter receive specifies the direction of the transfer.
 *
 */
static void I2C_MasterSendAddress(i2c_type_t *baseAddr, i2c_master_state_t *master, bool receive)
{
    uint8_t addrByte;
    i2c_master_command_t startCommand;

    OS_ASSERT(master != NULL);
    OS_ASSERT(baseAddr != NULL);

    {
        /* Normal START command. */
        startCommand = I2C_MASTER_COMMAND_START;
    }

    if (master->is10bitAddr) {
        /* 10-bit addressing. */
        /* First address byte: 1111 0XXD, where XX are bits 10 and 9 of address, and D = 0(transmit). */
        addrByte = (uint8_t)(0xF0U + ((master->slaveAddress >> 7U) & 0x6U) + (uint8_t)0U);
        I2C_MasterQueueCmd(baseAddr, master, startCommand, addrByte);
        /* Second address byte: Remaining 8 bits of address. */
        addrByte = (uint8_t)(master->slaveAddress & 0xFFU);
        I2C_MasterQueueCmd(baseAddr, master, I2C_MASTER_COMMAND_TRANSMIT, addrByte);
        if (true == receive) {
            /* Receiving from 10-bit slave - must send repeated start and resend first address byte. */
            /* First address byte: 1111 0XXD, where XX are bits 10 and 9 of address, and D = 1 (receive). */
            addrByte = (uint8_t)(0xF0U + ((master->slaveAddress >> 7U) & 0x6U) + (uint8_t)1U);
            I2C_MasterQueueCmd(baseAddr, master, startCommand, addrByte);
        }
    } else {
        /* 7-bit addressing. */
        /* Address byte: slave 7-bit address + D = 0(transmit) or 1 (receive). */
        addrByte = (uint8_t)((master->slaveAddress << 1U) + (uint8_t)receive);
        I2C_MasterQueueCmd(baseAddr, master, startCommand, addrByte);
    }
}

/**
 *
 * Function Name : I2C_MasterQueueData
 * Description   : Queues transmit data in the I2C tx fifo until it is full.
 *
 */
static void I2C_MasterQueueData(i2c_type_t *baseAddr, i2c_master_state_t *master)
{
    OS_ASSERT(master != NULL);
    OS_ASSERT(baseAddr != NULL);

    uint16_t txFIFOCount = I2C_GetMasterTxFIFOCount(baseAddr);
    uint16_t txFifoSize  = I2C_GetMasterTxFIFOSize(baseAddr);

    /* Don't queue any data if there are commands in the software queue. */
    if (I2C_MasterCmdQueueEmpty(master)) {
        while ((master->txSize > 0U) && (txFIFOCount < txFifoSize)) {
            I2C_CmdMasterTransmit(baseAddr, I2C_MASTER_COMMAND_TRANSMIT, master->txBuff[0U]);
            master->txBuff++;
            master->txSize--;

            txFIFOCount = I2C_GetMasterTxFIFOCount(baseAddr);
        }
    }
}

/**
 *
 * Function Name : I2C_MasterEndTransfer
 * Description   : Ends current transmission or reception.
 *
 */
static void I2C_MasterEndTransfer(i2c_type_t *baseAddr, i2c_master_state_t *master, bool sendStop, bool resetFIFO)
{
    OS_ASSERT(master != NULL);
    OS_ASSERT(baseAddr != NULL);

    /* Disable all events. */
    I2C_SetMasterInt(baseAddr,
                     I2C_MASTER_FIFO_ERROR_INT | I2C_MASTER_ARBITRATION_LOST_INT | I2C_MASTER_NACK_DETECT_INT |
                         I2C_MASTER_TRANSMIT_DATA_INT | I2C_MASTER_RECEIVE_DATA_INT,
                     false);

    if (I2C_USING_DMA == master->transferType) {
        /* Stop PDMA channel. */
        (void)PDMA_DRV_StopChannel(master->dmaChannel);

        /* Disable I2C PDMA request. */
        if (master->rxSize != (uint16_t)0) {
            (void)I2C_SetMasterRxDMA(baseAddr, false);
        } else {
            (void)I2C_SetMasterTxDMA(baseAddr, false);
        }
    }

    if (true == resetFIFO) {
        /* Reset FIFOs if requested. */
        I2C_ResetMasterTxFIFOCmd(baseAddr);
        I2C_ResetMasterRxFIFOCmd(baseAddr);
    }

    /* Queue STOP command if requested. */
    if (true == sendStop) {
        I2C_CmdMasterTransmit(baseAddr, I2C_MASTER_COMMAND_STOP, 0U);
    }

    master->txBuff  = NULL;
    master->txSize  = 0;
    master->rxBuff  = NULL;
    master->rxSize  = 0;
    master->i2cIdle = true;
}

/**
 *
 * Function Name : I2C_SlaveEndTransfer
 * Description   : Ends current transmission or reception.
 *
 */
static void I2C_SlaveEndTransfer(i2c_type_t *baseAddr, i2c_slave_state_t *slave)
{
    OS_ASSERT(slave != NULL);
    OS_ASSERT(baseAddr != NULL);

    /* Deactivate events. */

    I2C_SetSlaveInt(baseAddr,
                    I2C_SLAVE_BIT_ERROR_INT | I2C_SLAVE_FIFO_ERROR_INT | I2C_SLAVE_STOP_DETECT_INT |
                        I2C_SLAVE_REPEATED_START_INT | I2C_SLAVE_ADDRESS_VALID_INT | I2C_SLAVE_RECEIVE_DATA_INT |
                        I2C_SLAVE_TRANSMIT_DATA_INT,
                    false);

    /* For PDMA we must disable the PDMA request. */
    if (I2C_USING_DMA == slave->transferType) {
        if (slave->rxSize != (uint16_t)0) {
            (void)I2C_SetSlaveRxDMA(baseAddr, false);
        } else {
            (void)I2C_SetSlaveTxDMA(baseAddr, false);
        }
    }

    /* Disable I2C slave. */
    I2C_SetSlaveEnable(baseAddr, false);

    slave->isTransferInProgress = false;
    slave->rxBuff               = NULL;
    slave->rxSize               = 0U;
    slave->txBuff               = NULL;
    slave->txSize               = 0U;
    slave->repeatedStarts       = 0U;
}

/**
 *
 * Function Name : I2C_MasterSetOperatingMode
 * Description   : Sets the operating mode of the I2C master.
 *
 */
static void I2C_MasterSetOperatingMode(uint32_t instance, i2c_mode_t operatingMode)
{
    i2c_type_t *baseAddr;
    i2c_master_state_t *master;

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

#if (I2C_HAS_ULTRA_FAST_MODE)
    if (I2C_ULTRAFAST_MODE == operatingMode) {
        I2C_SetMasterPinConfig(baseAddr, I2C_CFG_2PIN_OUTPUT_ONLY);
        I2C_SetMasterNACKConfig(baseAddr, I2C_NACK_IGNORE);
    } else
#endif
    {
        I2C_SetMasterPinConfig(baseAddr, I2C_CFG_2PIN_OPEN_DRAIN);
        I2C_SetMasterNACKConfig(baseAddr, I2C_NACK_RECEIVE);
    }

    master->operatingMode = operatingMode;
}

/**
 *
 * Function Name : I2C_SlaveSetOperatingMode
 * Description   : Sets the operating mode of the I2C slave.
 *
 */
static void I2C_SlaveSetOperatingMode(uint32_t instance, i2c_mode_t operatingMode)
{
    i2c_type_t *baseAddr;
    i2c_slave_state_t *slave;

    baseAddr = g_i2cBase[instance];
    slave    = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

#if (I2C_HAS_ULTRA_FAST_MODE)
    if (I2C_ULTRAFAST_MODE == operatingMode) {
        I2C_SetSlaveIgnoreNACK(baseAddr, I2C_SLAVE_NACK_CONTINUE_TRANSFER);
        I2C_SetSlaveTransmitNACK(baseAddr, I2C_SLAVE_TRANSMIT_NACK);
        /* Disable all clock stretching in ultra-fast mode. */
        I2C_SetSlaveACKStall(baseAddr, false);
        I2C_SetSlaveTXDStall(baseAddr, false);
        I2C_SetSlaveRXStall(baseAddr, false);
        I2C_SetSlaveAddrStall(baseAddr, false);
    } else
#endif
    {
        I2C_SetSlaveIgnoreNACK(baseAddr, I2C_SLAVE_NACK_END_TRANSFER);
        I2C_SetSlaveTransmitNACK(baseAddr, I2C_SLAVE_TRANSMIT_ACK);
        /* Enable clock stretching except ACKSTALL (we don't need to send ACK/NACK manually). */
        I2C_SetSlaveACKStall(baseAddr, false);
        I2C_SetSlaveTXDStall(baseAddr, true);
        I2C_SetSlaveRXStall(baseAddr, true);
        I2C_SetSlaveAddrStall(baseAddr, true);
    }

    /* Set I2C slave Clock Hold Time. */
    I2C_SetSlaveClockHoldTime(baseAddr, 2);
    /* Set I2C slave Data Valid Delay. */
    I2C_SetSlaveDataValidDelay(baseAddr, 2);
    /* Disable detection of the High-speed Mode master code. */
    I2C_SetSlaveHighSpeedModeDetect(baseAddr, false);

    slave->operatingMode = operatingMode;
}

/**
 *
 * Function Name : I2C_ConfigureDmaTransfer
 * Description   : Configures the PDMA transfer.
 *
 */
static void I2C_ConfigureDmaTransfer(uint32_t instance, const i2c_dma_transfer_params_t *dmaTransParams)
{
    /* Configure PDMA channel. */
    if (I2C_TX_REQ == dmaTransParams->transferDirection) {
        (void)PDMA_DRV_SetChannelRequestAndTrigger(dmaTransParams->dmaChannel, g_i2cDMASrc[instance][I2C_TX_REQ],
                                                   false);
        (void)PDMA_DRV_ConfigMultiBlockTransfer(dmaTransParams->dmaChannel, dmaTransParams->dmaTransferType,
                                                (uint32_t)dmaTransParams->bufferTransfer,
                                                (uint32_t)dmaTransParams->i2cDataRegAddr, PDMA_TRANSFER_SIZE_1B,
                                                (uint32_t)1U, (uint32_t)dmaTransParams->transferSize, false);
    } else {
        (void)PDMA_DRV_SetChannelRequestAndTrigger(dmaTransParams->dmaChannel, g_i2cDMASrc[instance][I2C_RX_REQ],
                                                   false);
        (void)PDMA_DRV_ConfigMultiBlockTransfer(dmaTransParams->dmaChannel, dmaTransParams->dmaTransferType,
                                                (uint32_t)dmaTransParams->i2cDataRegAddr,
                                                (uint32_t)dmaTransParams->bufferTransfer, PDMA_TRANSFER_SIZE_1B,
                                                (uint32_t)1U, (uint32_t)dmaTransParams->transferSize, false);
    }
}

/**
 *
 * Function Name : I2C_MasterStartDmaTransfer
 * Description   : Starts the PDMA transfer for master.
 *
 */
static void I2C_MasterStartDmaTransfer(uint32_t instance)
{
    i2c_type_t *baseAddr       = g_i2cBase[instance];
    i2c_master_state_t *master = g_i2cMasterStatePtr[instance];
    i2c_dma_transfer_params_t dmaTransParams;
    bool receive = false;

    dmaTransParams.dmaChannel = master->dmaChannel;
    if (master->txSize > 0U) {
        /* Configure watermarks for transmit PDMA for master. */
        uint32_t txBytes = I2C_GetMasterTxFIFOSize(baseAddr);
        if (txBytes > master->txSize) {
            txBytes = master->txSize;
        }
        I2C_SetMasterTxFIFOWatermark(baseAddr, (uint16_t)(txBytes - 1U));

        dmaTransParams.dmaTransferType   = PDMA_TRANSFER_MEM2PERIPH;
        dmaTransParams.i2cDataRegAddr    = (uint32_t)(&(baseAddr->MTDR));
        dmaTransParams.bufferTransfer    = (uint8_t *)master->txBuff;
        dmaTransParams.transferDirection = I2C_TX_REQ;
        dmaTransParams.transferSize      = master->txSize;

    } else {
        /* Configure watermarks for receive PDMA for master. */
        I2C_SetMasterRxFIFOWatermark(baseAddr, 0U);

        receive = true;

        dmaTransParams.dmaTransferType   = PDMA_TRANSFER_PERIPH2MEM;
        dmaTransParams.i2cDataRegAddr    = (uint32_t)(&(baseAddr->MRDR));
        dmaTransParams.bufferTransfer    = master->rxBuff;
        dmaTransParams.transferDirection = I2C_RX_REQ;
        dmaTransParams.transferSize      = master->rxSize;
    }

    (void)I2C_ConfigureDmaTransfer(instance, &dmaTransParams);

    /* Disable PDMA requests for channel when transfer is done. */
    PDMA_DRV_DisableRequestsOnTransferComplete(dmaTransParams.dmaChannel, true);

    /* Call callback function when all the bytes were transferred. */
    (void)PDMA_DRV_InstallCallback(dmaTransParams.dmaChannel, (I2C_MasterCompleteDMATransfer), (void *)(instance));

    /* Start channel. */
    (void)PDMA_DRV_StartChannel(dmaTransParams.dmaChannel);

    I2C_MasterSendAddress(baseAddr, master, receive);

    /* Enable transmit/receive PDMA requests. */
    if (master->txSize > (uint32_t)0U) {
        (void)I2C_SetMasterTxDMA(baseAddr, true);
    } else {
        I2C_MasterQueueCmd(baseAddr, master, I2C_MASTER_COMMAND_RECEIVE, (uint8_t)(master->rxSize - 1U));
        (void)I2C_SetMasterRxDMA(baseAddr, true);
    }
}

/**
 *
 * Function Name : I2C_SlaveStartDmaTransfer
 * Description   : Starts the PDMA transfer for slave.
 *
 */
static void I2C_SlaveStartDmaTransfer(uint32_t instance)
{
    i2c_type_t *baseAddr           = g_i2cBase[instance];
    const i2c_slave_state_t *slave = g_i2cSlaveStatePtr[instance];
    i2c_dma_transfer_params_t dmaTransParams;

    if (slave->txSize > (uint32_t)0U) {
        dmaTransParams.dmaChannel        = slave->dmaChannel;
        dmaTransParams.dmaTransferType   = PDMA_TRANSFER_MEM2PERIPH;
        dmaTransParams.i2cDataRegAddr    = (uint32_t)(&(baseAddr->STDR));
        dmaTransParams.bufferTransfer    = (uint8_t *)slave->txBuff;
        dmaTransParams.transferDirection = I2C_TX_REQ;
        dmaTransParams.transferSize      = slave->txSize;
    } else {
        dmaTransParams.dmaChannel        = slave->dmaChannel;
        dmaTransParams.dmaTransferType   = PDMA_TRANSFER_PERIPH2MEM;
        dmaTransParams.i2cDataRegAddr    = (uint32_t)(&(baseAddr->SRDR));
        dmaTransParams.bufferTransfer    = slave->rxBuff;
        dmaTransParams.transferDirection = I2C_RX_REQ;
        dmaTransParams.transferSize      = slave->rxSize;
    }

    (void)I2C_ConfigureDmaTransfer(instance, &dmaTransParams);
    /* Adjustment added to source address at the beginning of TX buffer. */
    PDMA_DRV_SetSrcLastAddrAdjustment(dmaTransParams.dmaChannel, -(int32_t)(slave->txSize));

    /* Start channel. */
    (void)PDMA_DRV_StartChannel(dmaTransParams.dmaChannel);

    /* Enable transmit/receive PDMA requests. */
    if (slave->txSize > (uint32_t)0U) {
        (void)I2C_SetSlaveTxDMA(baseAddr, true);
    } else {
        (void)I2C_SetSlaveRxDMA(baseAddr, true);
    }
}

/**
 *
 * Function Name : I2C_MasterCompleteDMATransfer
 * Description   : Callback for master PDMA transfer done.Finish up a transfer PDMA for master.
 * The main purpose of this function is to create a function compatible with PDMA callback type.
 *
 */
static void I2C_MasterCompleteDMATransfer(void *parameter, pdma_chn_status_t status)
{
    i2c_type_t *baseAddr;
    i2c_master_state_t *master;

    uint32_t instance = (uint32_t)parameter;

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];

    if ((master->txSize > 0U) && (status != PDMA_CHN_ERROR)) {
        master->txSize = 0U;

        I2C_SetMasterTxFIFOWatermark(baseAddr, 0U);

        /* Disable transmit data PDMA requests. */
        (void)I2C_SetMasterTxDMA(baseAddr, false);

        /* Activate transmit data events. */
        I2C_SetMasterInt(baseAddr, (uint32_t)I2C_MASTER_TRANSMIT_DATA_INT, true);
    } else {
        /* Signal transfer end for blocking transfers. */
        I2C_MasterEndTransfer(baseAddr, master, master->sendStop, false);

        if (true == master->blocking) {
            (void)OS_SemaphoreRelease(master->idleSemaphore);
        }

        /* Report status error if an error occurred in PDMA channel. */
        if (PDMA_CHN_ERROR == status) {
            master->status = STATUS_ERROR;
        } else {
            master->status = STATUS_SUCCESS;
        }

        if (master->masterCallback != NULL) {
            master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
    }
}

/**
 *
 * Function Name : I2C_MasterReinit
 * Description   : Re-initialize the I2C master.
 *
 */
static status_t I2C_MasterReinit(uint32_t instance, i2c_master_state_t *master)
{
    i2c_type_t *baseAddr;
    i2c_baud_rate_params_t baudRate;

    baseAddr                      = g_i2cBase[instance];
    g_i2cMasterStatePtr[instance] = master;

    /* Re-initialize driver status structure. */
    master->txBuff  = NULL;
    master->txSize  = 0;
    master->rxBuff  = NULL;
    master->rxSize  = 0;
    master->i2cIdle = true;

    I2C_MasterResetQueue(master);

    /* Initialize module. */
    I2C_Init(baseAddr);

    /* Set baud rate. */
    baudRate = master->baudrate;
    (void)I2C_DRV_MasterSetBaudRate(instance, master->operatingMode, baudRate);

    /* Set slave address. */
    I2C_DRV_MasterSetSlaveAddr(instance, master->slaveAddress, master->is10bitAddr);

    /* Enable I2C master. */
    I2C_SetMasterEnable(baseAddr, true);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_MasterWaitTransferEnd
 * Description   : Waits for the end of a blocking transfer.
 *
 */
static status_t I2C_MasterWaitTransferEnd(uint32_t instance, uint32_t timeout)
{
    i2c_type_t *baseAddr;
    OS_Status_t osifError = OS_OK;
    i2c_master_state_t *master;
    uint16_t rxFifoFill = 0;

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];

    /* Wait for transfer to be completed by the IRQ. */
    osifError = OS_SemaphoreAcquire(master->idleSemaphore, timeout);

    if (OS_ErrorTimeout == osifError) {
        /* If master is sending data transfer is aborted now in case timeout occurred. */
        if (master->txSize != 0U) {
            I2C_MasterEndTransfer(baseAddr, master, false, true);

            master->status = STATUS_TIMEOUT;
        } else {
            if (I2C_USING_DMA == master->transferType) {
                /* Stop PDMA channel and activate interrupts. */
                (void)PDMA_DRV_StopChannel(master->dmaChannel);
            }

            /* Disable interrupts to check number of bytes in rx fifo. */
            I2C_SetMasterInt(baseAddr, (uint32_t)I2C_MASTER_RECEIVE_DATA_INT, false);

            /* Check number of bytes in rx fifo. */
            rxFifoFill = I2C_GetMasterRxFIFOCount(baseAddr);

            /* In case both rx size and number of bytes in rx fifo is 0, then the transfer ended successfully. */
            if ((rxFifoFill == master->rxSize) && (0U == master->rxSize)) {
                master->status = STATUS_SUCCESS;

                /* Blocking transfer is over. */
                master->blocking = false;

                return master->status;
            }

            /* Set watermark to rxFifoFill in case the rx size is grater than the number of bytes in the rx fifo. */
            if (rxFifoFill < master->rxSize) {
                master->abortedTransfer = true;
                I2C_SetMasterRxFIFOWatermark(baseAddr, rxFifoFill);

                master->status = STATUS_TIMEOUT;
            }

            I2C_SetMasterInt(baseAddr, (uint32_t)I2C_MASTER_RECEIVE_DATA_INT, true);

            osifError = OS_SemaphoreAcquire(master->idleSemaphore, timeout);
            if (OS_ErrorTimeout == osifError) {
                (void)I2C_MasterReinit(instance, master);
                master->status = STATUS_TIMEOUT;
            }

            master->abortedTransfer = false;
        }
    } else {
        do {
            I2C_GetMasterRxFIFOCount(baseAddr);
        } while (I2C_GetMasterRxFIFOCount(baseAddr));
    }

    /* Blocking transfer is over. */
    master->blocking = false;
    return master->status;
}

/**
 *
 * Function Name : I2C_SlaveWaitTransferEnd
 * Description   : Waits for the end of a blocking transfer.
 *
 */
static status_t I2C_SlaveWaitTransferEnd(uint32_t instance, uint32_t timeout)
{
    OS_Status_t osifError = OS_OK;
    i2c_type_t *baseAddr;
    i2c_slave_state_t *slave;

    baseAddr = g_i2cBase[instance];
    slave    = g_i2cSlaveStatePtr[instance];

    /* Wait for transfer to be completed by the IRQ. */
    osifError = OS_SemaphoreAcquire(slave->idleSemaphore, timeout);

    if (OS_ErrorTimeout == osifError) {
        I2C_SlaveEndTransfer(baseAddr, slave);
        slave->status = STATUS_TIMEOUT;
    }

    /* Blocking transfer is over. */
    slave->blocking = false;
    return slave->status;
}

/**
 *
 * Function Name : I2C_MasterHandleTransmitDataRequest
 * Description   : Handle a transmit request for master.
 *
 */
static void I2C_MasterHandleTransmitDataRequest(i2c_type_t *baseAddr, i2c_master_state_t *master)
{
    /* More data needed for transmission. */
    if (!I2C_MasterCmdQueueEmpty(master)) {
        /* If there are queued commands, send them. */
        I2C_MasterSendQueuedCmd(baseAddr, master);
    } else if (master->txBuff != NULL) {
        /* A transmission is in progress. */
        if (0U == master->txSize) {
            /* There is no more data in buffer, the transmission is over. */
            I2C_MasterEndTransfer(baseAddr, master, master->sendStop, false);

            /* Signal transfer end for blocking transfers. */
            if (true == master->blocking) {
                (void)OS_SemaphoreRelease(master->idleSemaphore);
            }

            master->status = STATUS_SUCCESS;

            if (master->masterCallback != NULL) {
                master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
            }
        } else {
            /* Queue data bytes to fill tx fifo. */
            I2C_MasterQueueData(baseAddr, master);
        }
    } else {
        /* No more commands and no transmission in progress - disable tx event. */
        I2C_SetMasterInt(baseAddr, (uint32_t)I2C_MASTER_TRANSMIT_DATA_INT, false);
    }
}

/**
 *
 * Function Name : I2C_MasterHandleReceiveDataReadyEvent
 * Description   : Handle a receive request for master.
 *
 */
static void I2C_MasterHandleReceiveDataReadyEvent(i2c_type_t *baseAddr, i2c_master_state_t *master)
{
    /* Received data ready. */
    OS_ASSERT(master->rxBuff != NULL);

    /* Transfer received data to user buffer. */
    while ((I2C_GetMasterRxFIFOCount(baseAddr) > 0U) && (master->rxSize > 0U)) {
        master->rxBuff[0U] = I2C_GetMasterRxData(baseAddr);
        master->rxBuff++;
        master->rxSize--;
    }
    if (0U == master->rxSize) {
        /* Done receiving. */
        I2C_MasterEndTransfer(baseAddr, master, master->sendStop, false);

        /* Signal transfer end for blocking transfers. */
        if (true == master->blocking) {
            (void)OS_SemaphoreRelease(master->idleSemaphore);
        }

        master->status = STATUS_SUCCESS;

        if (master->masterCallback != NULL) {
            master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
    } else if (master->rxSize <= I2C_GetMasterRxFIFOWatermark(baseAddr)) {
        /* Reduce rx watermark to receive the last few bytes. */
        I2C_SetMasterRxFIFOWatermark(baseAddr, (uint16_t)(master->rxSize - 1U));
    } else {
        /* Continue reception. */
    }
}

/**
 *
 * Function Name : I2C_SlaveHandleAddressValidEvent
 * Description   : Handle an address valid event for slave.
 *
 */
static void I2C_SlaveHandleAddressValidEvent(uint32_t instance, const i2c_type_t *baseAddr,
                                             i2c_slave_state_t *slave)
{
    uint16_t receivedAddr;

    receivedAddr = I2C_GetSlaveReceivedAddr(baseAddr);
    if ((receivedAddr & 1U) != (uint16_t)0U) {
        /* Request from master to transmit data. */
        if ((slave->slaveCallback != NULL) && slave->slaveListening) {
            slave->slaveCallback(I2C_SLAVE_EVENT_TX_REQ, slave->callbackParam);
        }

        if (I2C_USING_INTERRUPTS == slave->transferType) {
            /* Enable interrupt for transmitting data. */
            I2C_SetSlaveInt(g_i2cBase[instance], (uint32_t)I2C_SLAVE_TRANSMIT_DATA_INT, true);
        }

        slave->txUnderrunWarning = false;

        if ((I2C_USING_DMA == slave->transferType) && slave->slaveListening) {
            (void)I2C_SlaveStartDmaTransfer(instance);
        }

    } else {
        /* Request from master to receive data. */
        if ((slave->slaveCallback != NULL) && slave->slaveListening) {
            slave->slaveCallback(I2C_SLAVE_EVENT_RX_REQ, slave->callbackParam);
        }

        if ((I2C_USING_DMA == slave->transferType) && slave->slaveListening) {
            (void)I2C_SlaveStartDmaTransfer(instance);
        }
    }

    slave->status = STATUS_BUSY;
}

/**
 *
 * Function Name : I2C_SlaveHandleTransmitDataEvent
 * Description   : Handle a transmit data event for slave.
 *
 */
static void I2C_SlaveHandleTransmitDataEvent(i2c_type_t *baseAddr, i2c_slave_state_t *slave)
{
    if (true == slave->txUnderrunWarning) {
        /* Another Tx event after underflow warning means the dummy char was sent. */
        slave->status = STATUS_I2C_TX_UNDERRUN;
    }

    if (0U == slave->txSize) {
        /* Out of data, call callback to allow user to provide a new buffer. */
        if (slave->slaveCallback != NULL) {
            slave->slaveCallback(I2C_SLAVE_EVENT_TX_EMPTY, slave->callbackParam);
        }
    }

    if (0U == slave->txSize) {
        /*
         * Still no data, record tx underflow event and send dummy char.
         * Special case after the last tx byte: the device will ask for more data
         * but the dummy char will not be sent if NACK and then STOP condition are
         * received from master. So only record a "warning" for now.
         */
        slave->txUnderrunWarning = true;
        I2C_TransmitSlaveData(baseAddr, (uint8_t)0xFFU);
    } else {
        I2C_TransmitSlaveData(baseAddr, slave->txBuff[0U]);
        slave->txBuff++;
        slave->txSize--;
    }
}

/**
 *
 * Function Name : I2C_SlaveHandleReceiveDataEvent
 * Description   : Handle a receive data event for slave.
 *
 */
static void I2C_SlaveHandleReceiveDataEvent(const i2c_type_t *baseAddr, i2c_slave_state_t *slave)
{
    if (0U == slave->rxSize) {
        /* No more room for data, call callback to allow user to provide a new buffer. */
        if (slave->slaveCallback != NULL) {
            slave->slaveCallback(I2C_SLAVE_EVENT_RX_FULL, slave->callbackParam);
        }
    }

    if (0U == slave->rxSize) {
        /* Still no room for data, record rx overrun event and dummy read data. */
        slave->status = STATUS_I2C_RX_OVERRUN;
        (void)I2C_GetSlaveData(baseAddr);
    } else {
        slave->rxBuff[0U] = I2C_GetSlaveData(baseAddr);
        slave->rxBuff++;
        slave->rxSize--;
    }
}

/**
 *
 * Function Name : I2C_DRV_MasterInit
 * Description   : Initialize the I2C master mode driver.
 *
 */
status_t I2C_DRV_MasterInit(uint32_t instance, const i2c_master_user_config_t *userConfigPtr,
                            i2c_master_state_t *master)
{
    i2c_type_t *baseAddr;
    status_t retVal;
    uint32_t inputClock;
    i2c_baud_rate_params_t baudRate;
    OS_RegisterType_t uType = {0};

    OS_ASSERT(master != NULL);
    OS_ASSERT(userConfigPtr != NULL);
    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    clock_names_t i2cClocks[I2C_INSTANCE_COUNT] = I2C_PCC_CLOCKS;
    CLOCK_SYS_ClockEnable(i2cClocks[instance], true);

    /* Check to see if the I2C master instance is already initialized. */
    OS_ASSERT(NULL == g_i2cMasterStatePtr[instance]);

    /* Check the protocol clock frequency. */
    retVal = CLOCK_SYS_GetFreq(g_i2cClock[instance], &inputClock);
    OS_ASSERT(STATUS_SUCCESS == retVal);
    OS_ASSERT(inputClock > 0U);

    baseAddr                      = g_i2cBase[instance];
    g_i2cMasterStatePtr[instance] = master;

    /* Initialize driver status structure. */
    master->rxBuff       = NULL;
    master->rxSize       = 0U;
    master->txBuff       = NULL;
    master->txSize       = 0U;
    master->status       = STATUS_SUCCESS;
    master->i2cIdle      = true;
    master->slaveAddress = userConfigPtr->slaveAddress;
    master->is10bitAddr  = userConfigPtr->is10bitAddr;
    master->transferType = userConfigPtr->transferType;
    /* Store PDMA channel number used in transfer. */
    master->dmaChannel      = userConfigPtr->dmaChannel;
    master->masterCallback  = userConfigPtr->masterCallback;
    master->callbackParam   = userConfigPtr->callbackParam;
    master->blocking        = false;
    master->baudrate        = userConfigPtr->baudRate;
    master->abortedTransfer = false;

    /* Initialize the semaphore. */

    master->idleSemaphore = OS_SemaphoreNew(1, 1, NULL);
    OS_ASSERT(master->idleSemaphore != NULL);

    I2C_MasterResetQueue(master);

    /* Initialize module. */
    I2C_Init(baseAddr);

    /* Install I2C irq handler. */
    uType.trig_mode = CLIC_LEVEL_TRIGGER;
    uType.lvl       = 2;
    uType.priority  = 1;
    uType.data_ptr  = (void *)instance;
    OS_RequestIrq(g_i2cMasterIrqId[instance], I2C_DRV_MasterIRQHandler, &uType);

    /* Enable I2c interrupt. */
    OS_EnableIrq(g_i2cMasterIrqId[instance]);

    /* Set baud rate. */
    baudRate = userConfigPtr->baudRate;
    (void)I2C_DRV_MasterSetBaudRate(instance, userConfigPtr->operatingMode, baudRate);

    /* Set master glitch filter cycle count. */
    I2C_SetMasterGlitchFilterSCL(baseAddr, 1);
    /* Set slave address. */
    I2C_DRV_MasterSetSlaveAddr(instance, userConfigPtr->slaveAddress, userConfigPtr->is10bitAddr);

    /* Enable I2C master. */
    I2C_SetMasterEnable(baseAddr, true);

    (void)retVal;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_MasterDeinit
 * Description   : Deinitialize the I2C master mode driver.
 *
 */
status_t I2C_DRV_MasterDeinit(uint32_t instance)
{
    i2c_type_t *baseAddr;
    const i2c_master_state_t *master;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    /* Destroy the semaphore. */
    (void)OS_SemaphoreDelete(master->idleSemaphore);

    g_i2cMasterStatePtr[instance] = NULL;

    /* Clear master glitch filter cycle count. */
    I2C_SetMasterGlitchFilterSCL(baseAddr, 0);
    /* Disable master. */
    I2C_SetMasterEnable(baseAddr, false);

    /* Disable I2c interrupt. */
    OS_DisableIrq(g_i2cMasterIrqId[instance]);
    /* Uninstall I2C irq. */
    OS_UnRequestIrq(g_i2cMasterIrqId[instance]);
    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_SetMasterBusIdleTimeout
 * Description   : Set bus idle timeout period for I2C master mode driver.
 *
 */
void I2C_DRV_SetMasterBusIdleTimeout(uint32_t instance, uint16_t timeout)
{
    i2c_type_t *baseAddr;
    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    uint16_t timeout_temp = timeout;

    /* Limit timeout value since BUSIDLE field is 12-bit long. */
    if (timeout_temp > BUSIDLE_MAX_VALUE) {
        timeout_temp = BUSIDLE_MAX_VALUE;
    }

    baseAddr = g_i2cBase[instance];

    /* Disable master. */
    I2C_SetMasterEnable(baseAddr, false);

    /* Set BUSIDLE value for MCFGR2. */
    I2C_SetMasterBusIdleTimeout(baseAddr, timeout_temp);

    /* Enable master. */
    I2C_SetMasterEnable(baseAddr, true);
}

/**
 *
 * Function Name : I2C_DRV_MasterGetBaudRate
 * Description   : Returns the currently configured baud rate.
 *
 */
void I2C_DRV_MasterGetBaudRate(uint32_t instance, i2c_baud_rate_params_t *baudRate)
{
    const i2c_type_t *baseAddr;
    const i2c_master_state_t *master;
    status_t retVal;
    uint32_t prescaler;
    uint32_t clkLo;
    uint32_t clkHi;
    uint32_t inputClock;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    /* Get the protocol clock frequency. */
    retVal = CLOCK_SYS_GetFreq(g_i2cClock[instance], &inputClock);
    OS_ASSERT(STATUS_SUCCESS == retVal);
    OS_ASSERT(inputClock > 0U);

    /* Ignoring the glitch filter, the baud rate formula is:
       SCL_freq = Input_freq / (2^PRESCALER * (CLKLO + CLKHI + 2)).
    */
    prescaler = (uint32_t)I2C_GetMasterPrescaler(baseAddr);
    clkHi     = (uint32_t)I2C_GetMasterClockHighPeriod(baseAddr);
    clkLo     = (uint32_t)I2C_GetMasterClockLowPeriod(baseAddr);

    *baudRate = inputClock / (((uint32_t)1U << prescaler) * (clkLo + clkHi + (uint32_t)2U));

    (void)retVal;
    (void)master;
}

/**
 *
 * Function Name : I2C_DRV_MasterSetBaudRate
 * Description   : Set the baud rate for any subsequent I2C communication.
 *
 */
status_t I2C_DRV_MasterSetBaudRate(uint32_t instance, const i2c_mode_t operatingMode,
                                   const i2c_baud_rate_params_t baudRate)
{
    i2c_type_t *baseAddr;
    const i2c_master_state_t *master;
    status_t retVal;
    uint32_t inputClock;
    uint32_t minPrescaler = 0U;
    uint32_t prescaler;
    uint32_t clkTotal;
    uint32_t clkLo;
    uint32_t clkHi;
    uint32_t setHold;
    uint32_t dataVd;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    /* Check if driver is busy. */
    if (!master->i2cIdle) {
        return STATUS_BUSY;
    }

    /* Get the protocol clock frequency. */
    retVal = CLOCK_SYS_GetFreq(g_i2cClock[instance], &inputClock);
    OS_ASSERT(STATUS_SUCCESS == retVal);
    OS_ASSERT(inputClock > 0U);

    /* Disable master. */
    I2C_SetMasterEnable(baseAddr, false);

    /* Ignoring the glitch filter, the baud rate formula is:
       SCL_freq = Input_freq / (2^PRESCALER * (CLKLO + CLKHI + 2)).
       Assume CLKLO = CLKHI, SETHOLD = CLKHI, DATAVD = CLKHI/2.
    */

    if (baudRate != 0U) {
        /* Compute minimum prescaler for which CLKLO and CLKHI values are in valid range. Always round up. */
        minPrescaler =
            ((inputClock - 1U) / ((baudRate) * (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U))) + (uint32_t)1U;
        for (prescaler = 0U; prescaler < 7U; prescaler++) {
            if (((uint32_t)1U << prescaler) >= minPrescaler) {
                break;
            }
        }

        /* Compute CLKLO and CLKHI values for this prescaler. Round to nearest integer. */
        clkTotal = (inputClock + ((baudRate << prescaler) >> 1U)) / (baudRate << prescaler);
    } else {
        prescaler = 7U;
        clkTotal  = (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U);
    }

    if (clkTotal > (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U)) {
        clkTotal = (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U);
    }

    /*
     * If we try to compute clk high and low values using clkTotal equal with 0
     * (this is the case when the baudrate is 0), we will get negative values for
     * them, so we set them to 0 for this case.
     */
    if (clkTotal <= 1U) {
        clkHi = 0U;
        clkLo = 0U;
    } else {
        clkHi = (clkTotal - 2U) / 2U;
        clkLo = clkTotal - 2U - clkHi;
    }

    if (clkHi < CLKHI_MIN_VALUE) {
        clkHi = CLKHI_MIN_VALUE;
    }
    if (clkLo < CLKLO_MIN_VALUE) {
        clkLo = CLKLO_MIN_VALUE;
    }

    /* Compute DATAVD and SETHOLD. */
    setHold = clkHi;
    dataVd  = clkHi >> 1U;
    if (setHold < SETHOLD_MIN_VALUE) {
        setHold = SETHOLD_MIN_VALUE;
    }
    if (dataVd < DATAVD_MIN_VALUE) {
        dataVd = DATAVD_MIN_VALUE;
    }

    /* Apply settings. */
    I2C_SetMasterPrescaler(baseAddr, (i2c_master_prescaler_t)prescaler);
    I2C_SetMasterDataValidDelay(baseAddr, (uint8_t)dataVd);
    I2C_SetMasterSetupHoldDelay(baseAddr, (uint8_t)setHold);
    I2C_SetMasterClockHighPeriod(baseAddr, (uint8_t)clkHi);
    I2C_SetMasterClockLowPeriod(baseAddr, (uint8_t)clkLo);

    /* Perform other settings related to the chosen operating mode. */
    I2C_MasterSetOperatingMode(instance, operatingMode);

    /* Re-enable master. */
    I2C_SetMasterEnable(baseAddr, true);

    (void)minPrescaler;
    (void)master;
    (void)retVal;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_SetMasterHostRequestEnable
 * Description   : Configures the I2C Host Request Enable straus.
 *
 */
void I2C_DRV_SetMasterHostRequestEnable(uint32_t instance, bool configuration)
{
    i2c_type_t *baseAddr;
    baseAddr = g_i2cBase[instance];
    I2C_SetMasterHostRequestEnable(baseAddr, configuration);
}

/**
 *
 * Function Name : I2C_DRV_SetMasterHostHostRequestPolarity
 * Description   : Configures the I2C Host Request Polarity status.
 *
 */
void I2C_DRV_SetMasterHostHostRequestPolarity(uint32_t instance, bool configuration)
{
    i2c_type_t *baseAddr;
    baseAddr = g_i2cBase[instance];
    I2C_SetMasterHostRequestPolarity(baseAddr, configuration);
}

/**
 *
 * Function Name : I2C_DRV_SetMasterHostRequestSelect
 * Description   : Configures the source of the Host Request Select input.
 *
 */
void I2C_DRV_SetMasterHostRequestSelect(uint32_t instance, bool configuration)
{
    i2c_type_t *baseAddr;
    baseAddr = g_i2cBase[instance];
    I2C_SetMasterHostRequestSelect(baseAddr, configuration);
}

/**
 *
 * Function Name : I2C_DRV_MasterSetSlaveAddr
 * Description   : Set the slave address for any subsequent I2C communication.
 *
 */
void I2C_DRV_MasterSetSlaveAddr(uint32_t instance, const uint16_t address, const bool is10bitAddr)
{
    i2c_master_state_t *master;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    master = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    master->slaveAddress = address;
    master->is10bitAddr  = is10bitAddr;
}

/**
 *
 * Function Name : I2C_DRV_MasterSendData
 * Description   : Perform a non-blocking send transaction on the I2C bus.
 *
 */
status_t I2C_DRV_MasterSendData(uint32_t instance, const uint8_t *txBuff, uint32_t txSize, bool sendStop)
{
    i2c_type_t *baseAddr;
    i2c_master_state_t *master;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    /* Check if driver is busy. */
    if (!master->i2cIdle) {
        return STATUS_BUSY;
    }

    /* Copy parameters to driver state structure. */
    master->txBuff   = txBuff;
    master->txSize   = txSize;
    master->sendStop = sendStop;
    master->i2cIdle  = false;
    master->status   = STATUS_BUSY;

    if (I2C_USING_DMA == master->transferType) {
        I2C_SetMasterInt(
            baseAddr, I2C_MASTER_FIFO_ERROR_INT | I2C_MASTER_ARBITRATION_LOST_INT | I2C_MASTER_NACK_DETECT_INT, true);

        I2C_MasterStartDmaTransfer(instance);
    } else {
        /* Initiate communication. */
        I2C_MasterSendAddress(baseAddr, master, false);

        /* Queue data bytes to fill tx fifo. */
        I2C_MasterQueueData(baseAddr, master);

        /* Set tx FIFO watermark. */
        I2C_SetMasterTxFIFOWatermark(baseAddr, 0U);

        /* Enable relevant events. */
#if (I2C_HAS_ULTRA_FAST_MODE)
        if (I2C_ULTRAFAST_MODE == master->operatingMode) {
            /* Do not enable NACK event reporting in ultra-fast mode. */
            I2C_SetMasterInt(baseAddr,
                             I2C_MASTER_FIFO_ERROR_INT | I2C_MASTER_ARBITRATION_LOST_INT | I2C_MASTER_TRANSMIT_DATA_INT,
                             true);
        } else
#endif
        {
            I2C_SetMasterInt(baseAddr,
                             I2C_MASTER_FIFO_ERROR_INT | I2C_MASTER_ARBITRATION_LOST_INT | I2C_MASTER_NACK_DETECT_INT |
                                 I2C_MASTER_TRANSMIT_DATA_INT,
                             true);
        }
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_MasterSendDataBlocking
 * Description   : Perform a blocking send transaction on the I2C bus.
 *
 */
status_t I2C_DRV_MasterSendDataBlocking(uint32_t instance, const uint8_t *txBuff, uint32_t txSize, bool sendStop,
                                        uint32_t timeout)
{

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    i2c_master_state_t *master = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    /* Check if driver is busy. */
    if (!master->i2cIdle) {
        return STATUS_BUSY;
    }

    /* Mark transfer as blocking. */
    master->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result. */
    (void)OS_SemaphoreAcquire(master->idleSemaphore, 0);

    (void)I2C_DRV_MasterSendData(instance, txBuff, txSize, sendStop);

    /* Wait for transfer to end. */
    return I2C_MasterWaitTransferEnd(instance, timeout);
}

/**
 *
 * Function Name : I2C_DRV_MasterAbortTransferData
 * Description   : Abort a non-blocking I2C Master transmission or reception.
 *
 */
status_t I2C_DRV_MasterAbortTransferData(uint32_t instance)
{
    i2c_type_t *baseAddr;
    i2c_master_state_t *master;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    if (master->rxBuff != NULL) {
        /* Aborting a reception not supported because hardware will continue the
           current command even if the FIFO is reset and this could last indefinitely. */
        return STATUS_UNSUPPORTED;
    }

    /* End transfer: force stop generation, reset FIFOs. */
    master->status = STATUS_I2C_ABORTED;
    I2C_MasterEndTransfer(baseAddr, master, true, true);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_MasterReceiveData
 * Description   : Perform a non-blocking receive transaction on the I2C bus.
 *
 */
status_t I2C_DRV_MasterReceiveData(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize, bool sendStop)
{
    i2c_type_t *baseAddr;
    i2c_master_state_t *master;
    uint16_t rxBytes;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    OS_ASSERT(rxSize <= 256U);

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    /* Check if driver is busy. */
    if (!master->i2cIdle) {
        return STATUS_BUSY;
    }

#if (I2C_HAS_ULTRA_FAST_MODE)
    if (I2C_ULTRAFAST_MODE == master->operatingMode) {
        /* No reception possible in ultra-fast mode. */
        return STATUS_ERROR;
    }
#endif

    /* Copy parameters to driver state structure. */
    master->rxSize   = rxSize;
    master->i2cIdle  = false;
    master->sendStop = sendStop;
    master->rxBuff   = rxBuff;
    master->status   = STATUS_BUSY;

    if (I2C_USING_DMA == master->transferType) {
        I2C_SetMasterInt(
            baseAddr, I2C_MASTER_FIFO_ERROR_INT | I2C_MASTER_ARBITRATION_LOST_INT | I2C_MASTER_NACK_DETECT_INT, true);

        I2C_MasterStartDmaTransfer(instance);
    } else {
        /* Initiate communication. */
        I2C_MasterSendAddress(baseAddr, master, true);
        /* Queue receive command for rxSize bytes. */
        I2C_MasterQueueCmd(baseAddr, master, I2C_MASTER_COMMAND_RECEIVE, (uint8_t)(rxSize - 1U));

        /* Set rx FIFO watermark. */
        rxBytes = I2C_GetMasterRxFIFOSize(baseAddr);
        if (rxBytes > rxSize) {
            rxBytes = (uint8_t)rxSize;
        }
        I2C_SetMasterRxFIFOWatermark(baseAddr, (uint16_t)(rxBytes - 1U));

        /* Enable relevant events. */
        if (!I2C_MasterCmdQueueEmpty(master)) {
            /* Enable tx event too if there are commands in the software FIFO. */
            I2C_SetMasterInt(baseAddr,
                             I2C_MASTER_FIFO_ERROR_INT | I2C_MASTER_ARBITRATION_LOST_INT | I2C_MASTER_NACK_DETECT_INT |
                                 I2C_MASTER_TRANSMIT_DATA_INT | I2C_MASTER_RECEIVE_DATA_INT,
                             true);
        } else {
            I2C_SetMasterInt(baseAddr,
                             I2C_MASTER_FIFO_ERROR_INT | I2C_MASTER_ARBITRATION_LOST_INT | I2C_MASTER_NACK_DETECT_INT |
                                 I2C_MASTER_RECEIVE_DATA_INT,
                             true);
        }
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_MasterReceiveDataBlocking
 * Description   : Perform a blocking receive transaction on the I2C bus.
 *
 */
status_t I2C_DRV_MasterReceiveDataBlocking(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize, bool sendStop, uint32_t timeout)
{
    status_t retVal = STATUS_SUCCESS;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    i2c_master_state_t *master = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    /* Check if driver is busy. */
    if (!master->i2cIdle) {
        return STATUS_BUSY;
    }

    /* Mark transfer as blocking. */
    master->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result. */
    (void)OS_SemaphoreAcquire(master->idleSemaphore, 0);

    retVal = I2C_DRV_MasterReceiveData(instance, rxBuff, rxSize, sendStop);

#if (I2C_HAS_ULTRA_FAST_MODE)
    if (retVal != STATUS_SUCCESS) {
        master->blocking = false;
        return retVal;
    }
#endif

    (void)retVal;

    /* Wait for transfer to end. */
    return I2C_MasterWaitTransferEnd(instance, timeout);
}

/**
 *
 * Function Name : I2C_DRV_MasterGetTransferStatus
 * Description   : Return the current status of the I2C master transfer.
 *
 * When performing an a-sync (non-blocking) transfer, the user can call this function
 * to ascertain the state of the current transfer. In addition, if the transfer is still
 * in progress, the user can get the number of words that should be receive.
 *
 */
status_t I2C_DRV_MasterGetTransferStatus(uint32_t instance, uint32_t *bytesRemaining)
{
    const i2c_type_t *baseAddr;
    const i2c_master_state_t *master;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    if ((bytesRemaining != NULL) && (I2C_USING_INTERRUPTS == master->transferType)) {
        if (master->txSize > 0U) {
            /* Send data. */
            /* Remaining bytes = bytes in buffer + bytes in tx FIFO. */
            *bytesRemaining = master->txSize + (uint32_t)I2C_GetMasterTxFIFOCount(baseAddr);
        } else if (master->rxSize > 0U) {
            /* Receive data. */
            /* Remaining bytes = free space in buffer - bytes in rx FIFO. */
            *bytesRemaining = master->rxSize - (uint32_t)I2C_GetMasterRxFIFOCount(baseAddr);
        } else {
            *bytesRemaining = 0U;
        }
    }

    return master->status;
}

/**
 *
 * Function Name : I2C_DRV_MasterGetDefaultConfig
 * Description   : Gets the default configuration structure for master.
 *
 */
void I2C_DRV_MasterGetDefaultConfig(i2c_master_user_config_t *config)
{
    config->slaveAddress   = 32U;
    config->is10bitAddr    = false;
    config->operatingMode  = I2C_STANDARD_MODE;
    config->baudRate       = 100000U;
    config->transferType   = I2C_USING_INTERRUPTS;
    config->dmaChannel     = 0U;
    config->masterCallback = NULL;
    config->callbackParam  = NULL;
}

/**
 *
 * Function Name : I2C_DRV_MasterIRQHandler
 * Description   : Handle non-blocking master operation when I2C interrupt occurs.
 *
 */
void I2C_DRV_MasterIRQHandler(void *data)
{
    uint32_t instance = (uint32_t)data;
    i2c_type_t *baseAddr;
    i2c_master_state_t *master;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    baseAddr = g_i2cBase[instance];
    master   = g_i2cMasterStatePtr[instance];
    OS_ASSERT(master != NULL);

    /* Check which event caused the interrupt. */
    if (I2C_GetMasterTransmitDataRequestEvent(baseAddr)) {
        I2C_MasterHandleTransmitDataRequest(baseAddr, master);
    }

    if ((I2C_GetMasterReceiveDataReadyEvent(baseAddr))) {
        if (master->abortedTransfer) {
            /* Signal transfer end for blocking transfers. */
            if (true == master->blocking) {
                (void)OS_SemaphoreRelease(master->idleSemaphore);
            }

            master->status = STATUS_TIMEOUT;

            (void)I2C_MasterReinit(instance, master);

        } else {
            I2C_MasterHandleReceiveDataReadyEvent(baseAddr, master);
        }
    }

    if (I2C_GetMasterFIFOErrorEvent(baseAddr)) {
        /* FIFO error. */
        I2C_ClearMasterFIFOErrorEvent(baseAddr);

        master->status = STATUS_ERROR;

        /* End transfer: no stop generation (the module will handle that by itself if needed), reset FIFOs. */
        I2C_MasterEndTransfer(baseAddr, master, false, true);

        /* Signal transfer end for blocking transfers. */
        if (true == master->blocking) {
            (void)OS_SemaphoreRelease(master->idleSemaphore);
        }

        if (master->masterCallback != NULL) {
            master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
    }

    if (I2C_GetMasterArbitrationLostEvent(baseAddr)) {
        /* Arbitration lost. */
        I2C_ClearMasterArbitrationLostEvent(baseAddr);

        /* End transfer: no stop generation (the module will handle that by itself if needed), reset FIFOs. */
        I2C_MasterEndTransfer(baseAddr, master, false, true);

        /* Signal transfer end for blocking transfers. */
        if (true == master->blocking) {
            (void)OS_SemaphoreRelease(master->idleSemaphore);
        }

        master->status = STATUS_I2C_ARBITRATION_LOST;

        if (master->masterCallback != NULL) {
            master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
    }

    /* Received NACK. */
    if (I2C_GetMasterNACKDetectEvent(baseAddr)) {

#if (I2C_HAS_ULTRA_FAST_MODE)
        /* Ignore NACK in Ultra Fast mode. */
        if (master->operatingMode != I2C_ULTRAFAST_MODE) {
#endif
            /* Signal transfer end for blocking transfers. */
            if (true == master->blocking) {
                (void)OS_SemaphoreRelease(master->idleSemaphore);
            }

            master->status = STATUS_I2C_RECEIVED_NACK;

            /* End transfer: no stop generation (the module will handle that by itself if needed), reset FIFOs. */
            I2C_MasterEndTransfer(baseAddr, master, false, true);

            if (master->masterCallback != NULL) {
                master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
            }

            /* Clear NACK flag. */
            I2C_ClearMasterNACKDetectEvent(baseAddr);
#if (I2C_HAS_ULTRA_FAST_MODE)
        }
#endif
    }
}

/**
 *
 * Function Name : I2C_DRV_SlaveInit
 * Description   : Initialize the I2C slave mode driver.
 *
 */
status_t I2C_DRV_SlaveInit(uint32_t instance, const i2c_slave_user_config_t *userConfigPtr, i2c_slave_state_t *slave)
{
    i2c_type_t *baseAddr;
    status_t retVal;
    uint32_t inputClock;
    OS_RegisterType_t uType = {0};

    OS_ASSERT(slave != NULL);
    OS_ASSERT(userConfigPtr != NULL);
    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    clock_names_t i2cClocks[I2C_INSTANCE_COUNT] = I2C_PCC_CLOCKS;
    CLOCK_SYS_ClockEnable(i2cClocks[instance], true);

    OS_ASSERT(NULL == g_i2cSlaveStatePtr[instance]);

    /*
     * Check the protocol clock frequency.
     * I2C slave remains operational, even when the I2C functional
     * clock is disabled, so we don't need to check if inputClock is 0.
     */
    retVal = CLOCK_SYS_GetFreq(g_i2cClock[instance], &inputClock);
    OS_ASSERT(STATUS_SUCCESS == retVal);

    baseAddr                     = g_i2cBase[instance];
    g_i2cSlaveStatePtr[instance] = slave;

    /* Initialize driver status structure. */
    slave->status         = STATUS_SUCCESS;
    slave->slaveListening = userConfigPtr->slaveListening;
    slave->slaveCallback  = userConfigPtr->slaveCallback;
    slave->callbackParam  = userConfigPtr->callbackParam;
    slave->txBuff         = NULL;
    slave->rxBuff         = NULL;
    slave->txSize         = 0U;
    slave->rxSize         = 0U;
    slave->transferType   = userConfigPtr->transferType;
    /* Store PDMA channel number used in transfer. */
    slave->dmaChannel           = userConfigPtr->dmaChannel;
    slave->isTransferInProgress = false;
    slave->blocking             = false;
    slave->is10bitAddress       = userConfigPtr->is10bitAddr;
    slave->repeatedStarts       = 0U;

    /* Initialize the semaphore. */
    slave->idleSemaphore = OS_SemaphoreNew(1, 1, NULL);
    OS_ASSERT(NULL != slave->idleSemaphore);

    /* Initialize module. */
    I2C_Init(baseAddr);

    /* Install I2C irq handler. */
    uType.trig_mode = CLIC_LEVEL_TRIGGER;
    uType.lvl       = 2;
    uType.priority  = 1;
    uType.data_ptr  = (void *)instance;
    OS_RequestIrq(g_i2cSlaveIrqId[instance], I2C_DRV_SlaveIRQHandler, &uType);

    /* Enable i2c interrupt. */
    OS_EnableIrq(g_i2cSlaveIrqId[instance]);
    /* Configure slave address. */
    I2C_SetSlaveAddr0(baseAddr, userConfigPtr->slaveAddress);
    if (userConfigPtr->is10bitAddr) {
        I2C_SetSlaveAddrConfig(baseAddr, I2C_SLAVE_ADDR_MATCH_0_10BIT);
    } else {
        I2C_SetSlaveAddrConfig(baseAddr, I2C_SLAVE_ADDR_MATCH_0_7BIT);
    }

    /* Configure operating mode. */
    I2C_SlaveSetOperatingMode(instance, userConfigPtr->operatingMode);

    if (userConfigPtr->slaveListening) {
        if (I2C_USING_DMA == slave->transferType) {
            /* Activate events. */
            I2C_SetSlaveInt(baseAddr,
                            I2C_SLAVE_BIT_ERROR_INT | I2C_SLAVE_FIFO_ERROR_INT | I2C_SLAVE_STOP_DETECT_INT |
                                I2C_SLAVE_REPEATED_START_INT | I2C_SLAVE_ADDRESS_VALID_INT,
                            true);
        }
        if (I2C_USING_INTERRUPTS == slave->transferType) {
            /* Activate events. */
            I2C_SetSlaveInt(baseAddr,
                            I2C_SLAVE_BIT_ERROR_INT | I2C_SLAVE_FIFO_ERROR_INT | I2C_SLAVE_STOP_DETECT_INT |
                                I2C_SLAVE_REPEATED_START_INT | I2C_SLAVE_ADDRESS_VALID_INT |
                                I2C_SLAVE_RECEIVE_DATA_INT | I2C_SLAVE_TRANSMIT_DATA_INT,
                            true);
        }
        /* Enable I2C slave Filter. */
        I2C_SetSlaveFilterEnable(baseAddr, true);
        /* Enable I2C slave. */
        I2C_SetSlaveEnable(baseAddr, true);
    }

    (void)retVal;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_SlaveDeinit
 * Description   : De-initialize the I2C slave mode driver.
 *
 */
status_t I2C_DRV_SlaveDeinit(uint32_t instance)
{
    i2c_type_t *baseAddr;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    baseAddr                       = g_i2cBase[instance];
    const i2c_slave_state_t *slave = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    /* Destroy the semaphore. */
    (void)OS_SemaphoreDelete(slave->idleSemaphore);

    if ((I2C_USING_DMA == slave->transferType) && (slave->slaveListening)) {
        /* Disable I2C PDMA requests. */
        (void)I2C_SetSlaveRxDMA(baseAddr, false);
        (void)I2C_SetSlaveTxDMA(baseAddr, false);
    }

    g_i2cSlaveStatePtr[instance] = NULL;

    /* Clear I2C slave Clock Hold Time. */
    I2C_SetSlaveClockHoldTime(baseAddr, 0);
    /* Clear I2C slave Data Valid Delay. */
    I2C_SetSlaveDataValidDelay(baseAddr, 0);
    /* Disable I2C slave Filter. */
    I2C_SetSlaveFilterEnable(baseAddr, false);
    /* Disable I2C slave. */
    I2C_SetSlaveEnable(baseAddr, false);

    /* Disable i2c interrupt. */
    OS_DisableIrq(g_i2cSlaveIrqId[instance]);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_SlaveSetTxBuffer
 * Description   : Provide a buffer for transmitting data.
 *
 */
status_t I2C_DRV_SlaveSetTxBuffer(uint32_t instance, const uint8_t *txBuff, uint32_t txSize)
{
    i2c_slave_state_t *slave;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    slave = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    slave->txBuff = txBuff;
    slave->txSize = txSize;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_SlaveSetRxBuffer
 * Description   : Provide a buffer for receiving data.
 *
 */
status_t I2C_DRV_SlaveSetRxBuffer(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize)
{
    i2c_slave_state_t *slave;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    slave = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    slave->rxBuff = rxBuff;
    slave->rxSize = rxSize;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_SlaveSendData
 * Description   : Perform a non-blocking send transaction on the I2C bus.
 *
 */
status_t I2C_DRV_SlaveSendData(uint32_t instance, const uint8_t *txBuff, uint32_t txSize)
{
    i2c_type_t *baseAddr;
    i2c_slave_state_t *slave;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    baseAddr = g_i2cBase[instance];
    slave    = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    /* If the slave is in listening mode the user should not use this function or the blocking counterpart. */
    OS_ASSERT(false == slave->slaveListening);

    /* Check if slave is busy. */
    if (slave->isTransferInProgress) {
        return STATUS_BUSY;
    }

    slave->txBuff = txBuff;
    slave->txSize = txSize;
    slave->status = STATUS_BUSY;

    if (I2C_USING_DMA == slave->transferType) {
        /* Activate events. */
        I2C_SetSlaveInt(baseAddr,
                        I2C_SLAVE_BIT_ERROR_INT | I2C_SLAVE_FIFO_ERROR_INT | I2C_SLAVE_STOP_DETECT_INT |
                            I2C_SLAVE_REPEATED_START_INT | I2C_SLAVE_ADDRESS_VALID_INT,
                        true);

        /* Enable I2C slave. */
        I2C_SetSlaveEnable(baseAddr, true);

        slave->isTransferInProgress = true;

        I2C_SlaveStartDmaTransfer(instance);
    } else {
        /* Activate events. */
        I2C_SetSlaveInt(baseAddr,
                        I2C_SLAVE_BIT_ERROR_INT | I2C_SLAVE_FIFO_ERROR_INT | I2C_SLAVE_STOP_DETECT_INT |
                            I2C_SLAVE_REPEATED_START_INT | I2C_SLAVE_ADDRESS_VALID_INT,
                        true);
        /* Enable I2C slave. */
        I2C_SetSlaveEnable(baseAddr, true);

        slave->isTransferInProgress = true;
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_SlaveSendDataBlocking
 * Description   : Perform a blocking send transaction on the I2C bus.
 *
 */
status_t I2C_DRV_SlaveSendDataBlocking(uint32_t instance, const uint8_t *txBuff, uint32_t txSize, uint32_t timeout)
{
    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    i2c_slave_state_t *slave = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    /* Check if slave is busy. */
    if (slave->isTransferInProgress) {
        return STATUS_BUSY;
    }

    /* Mark transfer as blocking. */
    slave->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result. */
    (void)OS_SemaphoreAcquire(slave->idleSemaphore, 0);

    (void)I2C_DRV_SlaveSendData(instance, txBuff, txSize);

    /* Wait for transfer to end. */
    return I2C_SlaveWaitTransferEnd(instance, timeout);
}

/**
 *
 * Function Name : I2C_DRV_SlaveReceiveData
 * Description   : Perform a non-blocking receive transaction on the I2C bus.
 *
 */
status_t I2C_DRV_SlaveReceiveData(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize)
{
    i2c_type_t *baseAddr;
    i2c_slave_state_t *slave;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    baseAddr = g_i2cBase[instance];
    slave    = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    /* If the slave is in listening mode the user should not use this function or the blocking counterpart. */
    OS_ASSERT(false == slave->slaveListening);

    /* Check if slave is busy. */
    if (slave->isTransferInProgress) {
        return STATUS_BUSY;
    }

    slave->rxBuff = rxBuff;
    slave->rxSize = rxSize;
    slave->status = STATUS_BUSY;

    if (I2C_USING_DMA == slave->transferType) {
        /* Activate events. */
        I2C_SetSlaveInt(baseAddr,
                        I2C_SLAVE_BIT_ERROR_INT | I2C_SLAVE_FIFO_ERROR_INT | I2C_SLAVE_STOP_DETECT_INT |
                            I2C_SLAVE_REPEATED_START_INT | I2C_SLAVE_ADDRESS_VALID_INT,
                        true);

        /* Enable I2C slave. */
        I2C_SetSlaveEnable(baseAddr, true);

        slave->isTransferInProgress = true;

        I2C_SlaveStartDmaTransfer(instance);
    } else {
        slave->isTransferInProgress = true;

        /* Activate events. */
        I2C_SetSlaveInt(baseAddr,
                        I2C_SLAVE_BIT_ERROR_INT | I2C_SLAVE_FIFO_ERROR_INT | I2C_SLAVE_STOP_DETECT_INT |
                            I2C_SLAVE_REPEATED_START_INT | I2C_SLAVE_ADDRESS_VALID_INT | I2C_SLAVE_RECEIVE_DATA_INT,
                        true);

        /* Enable I2C slave. */
        I2C_SetSlaveEnable(baseAddr, true);
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_SlaveReceiveDataBlocking
 * Description   : Perform a blocking receive transaction on the I2C bus.
 *
 */
status_t I2C_DRV_SlaveReceiveDataBlocking(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize, uint32_t timeout)
{
    OS_ASSERT(instance < I2C_INSTANCE_COUNT);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    i2c_slave_state_t *slave = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    /* Check if slave is busy. */
    if (slave->isTransferInProgress) {
        return STATUS_BUSY;
    }

    /* Mark transfer as blocking. */
    slave->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result. */
    (void)OS_SemaphoreAcquire(slave->idleSemaphore, 0);

    (void)I2C_DRV_SlaveReceiveData(instance, rxBuff, rxSize);

    /* Wait for transfer to end. */
    return I2C_SlaveWaitTransferEnd(instance, timeout);
}

/**
 *
 * Function Name : I2C_DRV_SlaveGetTransferStatus
 * Description   : Return the current status of the I2C slave transfer.
 *
 * When performing an a-sync (non-blocking) transfer, the user can call this function
 * to ascertain the state of the current transfer. In addition, if the transfer is still
 * in progress, the user can get the number of words that should be receive.
 *
 */
status_t I2C_DRV_SlaveGetTransferStatus(uint32_t instance, uint32_t *bytesRemaining)
{
    const i2c_slave_state_t *slave;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    slave = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    if ((bytesRemaining != NULL) && (I2C_USING_INTERRUPTS == slave->transferType)) {
        if (slave->txSize > 0U) {
            /* Send data. */
            *bytesRemaining = slave->txSize;
        } else if (slave->rxSize > 0U) {
            /* Receive data. */
            *bytesRemaining = slave->rxSize;
        } else {
            *bytesRemaining = 0U;
        }
    }

    return slave->status;
}

/**
 *
 * Function Name : I2C_DRV_SlaveAbortTransferData
 * Description   : Abort a non-blocking I2C Master transmission or reception.
 *
 */
status_t I2C_DRV_SlaveAbortTransferData(uint32_t instance)
{
    i2c_slave_state_t *slave;
    i2c_type_t *baseAddr;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    baseAddr = g_i2cBase[instance];
    slave    = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    if (!slave->slaveListening) {
        slave->status = STATUS_I2C_ABORTED;
        I2C_SlaveEndTransfer(baseAddr, slave);
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : I2C_DRV_SlaveGetDefaultConfig
 * Description   : Gets the default configuration structure for slave.
 *
 */
void I2C_DRV_SlaveGetDefaultConfig(i2c_slave_user_config_t *config)
{
    config->slaveAddress   = 32U;
    config->is10bitAddr    = false;
    config->slaveListening = true;
    config->operatingMode  = I2C_STANDARD_MODE;
    config->transferType   = I2C_USING_INTERRUPTS;
    config->dmaChannel     = 0U;
    config->slaveCallback  = NULL;
    config->callbackParam  = NULL;
}

/**
 *
 * Function Name : I2C_DRV_SlaveEndTransferHandler
 * Description   : Handle slave end transfer operations.
 *
 */
static void I2C_DRV_SlaveEndTransferHandler(i2c_slave_state_t *slave, i2c_type_t *baseAddr)
{
    /* Check slave state. */
    OS_ASSERT(slave != NULL);

    /* Stop PDMA channel if slave is transferring data in PDMA mode. */
    if (I2C_USING_DMA == slave->transferType) {
        (void)PDMA_DRV_StopChannel(slave->dmaChannel);
    }

    if (!slave->slaveListening) {

        I2C_SlaveEndTransfer(baseAddr, slave);

        /* Signal transfer end for blocking transfers. */
        if (true == slave->blocking) {
            (void)OS_SemaphoreRelease(slave->idleSemaphore);
        }
    }

    if (slave->slaveCallback != NULL) {
        slave->slaveCallback(I2C_SLAVE_EVENT_STOP, slave->callbackParam);
    }
}

/**
 *
 * Function Name : I2C_DRV_SlaveIRQHandler
 * Description   : Handle non-blocking slave operation when I2C interrupt occurs.
 *
 */
void I2C_DRV_SlaveIRQHandler(void *data)
{
    uint32_t instance = (uint32_t)data;
    i2c_type_t *baseAddr;
    i2c_slave_state_t *slave;
    bool stopDetect = false, repeatStartDetect = false;

    OS_ASSERT(instance < I2C_INSTANCE_COUNT);

    baseAddr = g_i2cBase[instance];
    slave    = g_i2cSlaveStatePtr[instance];
    OS_ASSERT(slave != NULL);

    /* Check for error event. */
    if (I2C_GetSlaveBitErrorEvent(baseAddr)) {
        slave->status = STATUS_ERROR;
        I2C_ClearSlaveBitErrorEvent(baseAddr);
        /* Deactivate interrupts for transmitting data. */
        I2C_SetSlaveInt(baseAddr, (uint32_t)I2C_SLAVE_TRANSMIT_DATA_INT, false);
        I2C_DRV_SlaveEndTransferHandler(slave, baseAddr);
    }
#if (I2C_HAS_ULTRA_FAST_MODE)
    else if (I2C_GetSlaveFIFOErrorEvent(baseAddr)) {
        /* In Ultra-Fast mode clock stretching is disabled, so it is possible to get
           this event if the slave can't keep up. */
        slave->status = STATUS_I2C_RX_OVERRUN;
        I2C_ClearSlaveFIFOErrorEvent(baseAddr);
        I2C_DRV_SlaveEndTransferHandler(slave, baseAddr);
    }
#endif
    else {
        /* Check for repeated start or stop condition. */
        stopDetect        = I2C_GetSlaveSTOPDetectEvent(baseAddr);
        repeatStartDetect = I2C_GetSlaveRepeatedStartEvent(baseAddr);

        if (repeatStartDetect) {
            slave->repeatedStarts++;

            if ((1U == slave->repeatedStarts) && (slave->is10bitAddress)) {
                repeatStartDetect = false;
                I2C_ClearSlaveRepeatedStartEvent(baseAddr);
            }
        }

        if ((true == stopDetect) || (true == repeatStartDetect)) {
            /* Either STOP or repeated START have the same meaning here: the current transfer is over. */
            I2C_ClearSlaveSTOPDetectEvent(baseAddr);
            I2C_ClearSlaveRepeatedStartEvent(baseAddr);

            /* Deactivate interrupts for transmitting data. */
            I2C_SetSlaveInt(baseAddr, (uint32_t)I2C_SLAVE_TRANSMIT_DATA_INT, false);
            if (STATUS_BUSY == slave->status) {
                /* Report success if no error was recorded. */
                slave->status = STATUS_SUCCESS;
            }

            I2C_DRV_SlaveEndTransferHandler(slave, baseAddr);
        } else {
            /* Check which event caused the interrupt. */
            if (I2C_GetSlaveAddressValidEvent(baseAddr)) {
                I2C_SlaveHandleAddressValidEvent(instance, baseAddr, slave);
            }

            if (I2C_GetSlaveTransmitDataEvent(baseAddr)) {
                if (I2C_GetSlaveInt(baseAddr, (uint32_t)I2C_SLAVE_TRANSMIT_DATA_INT)) {
                    I2C_SlaveHandleTransmitDataEvent(baseAddr, slave);
                }
            }

            if (I2C_GetSlaveReceiveDataEvent(baseAddr)) {
                if (I2C_GetSlaveInt(baseAddr, (uint32_t)I2C_SLAVE_RECEIVE_DATA_INT)) {
                    I2C_SlaveHandleReceiveDataEvent(baseAddr, slave);
                }
            }
        }
    }
}
