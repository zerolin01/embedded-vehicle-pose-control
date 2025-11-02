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
 * @file spi_slave_driver.c
 * @brief SPI slave driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:        Version:        Author:
 * Changes:
 *
 */

#include "spi_slave_driver.h"
#include "clock_driver.h"
#include "spi_hw_access.h"
#include <string.h>

/**
 * @brief Table of pstBase pointers for SPI instances.
 */
extern spi_type_t *g_spiBase[SPI_INSTANCE_COUNT];

/**
 * @brief Table to save SPI IRQ enumeration numbers defined in the CMSIS header file.
 */
extern IRQn_Type g_spiIrqId[SPI_INSTANCE_COUNT];

/** Pointer to runtime state structure.*/
extern spi_state_t *g_pstSpiState[SPI_INSTANCE_COUNT];

extern uint8_t g_spiInstance[SPI_INSTANCE_COUNT];

/** Callback for PDMA transfer done.*/
static void SPI_DRV_SlaveCompleteDMATransfer(void *parameter, pdma_chn_status_t status);

/**
 *
 * Function Name : SPI_DRV_SlaveGetDefaultConfig
 * Description   : Return default configuration for SPI master.
 *
 * Initializes a structured provided by user with the configuration
 * of an interrupt based SPI transfer.
 *
 */
void SPI_DRV_SlaveGetDefaultConfig(spi_slave_config_t *pstSpiConfig)
{
    OS_ASSERT(pstSpiConfig != NULL);

    pstSpiConfig->euWhichPcs     = SPI_PCS0;
    pstSpiConfig->euPcsPolarity  = SPI_ACTIVE_HIGH;
    pstSpiConfig->bitcount       = 8U;
    pstSpiConfig->euClkPhase     = SPI_CLOCK_PHASE_1ST_EDGE;
    pstSpiConfig->euClkPolarity  = SPI_SCK_ACTIVE_HIGH;
    pstSpiConfig->lsbFirst       = false;
    pstSpiConfig->euTransferType = SPI_USING_INTERRUPTS;
    pstSpiConfig->rxDMAChannel   = 0;
    pstSpiConfig->txDMAChannel   = 0;
    pstSpiConfig->callback       = NULL;
    pstSpiConfig->callbackParam  = NULL;
}

/**
 *
 * Function Name : SPI_DRV_SlaveInit
 * Description   : Initializes a SPI instance for a slave mode operation, using interrupt mechanism.
 *
 * This function un-gates the clock to the SPI module, initializes the SPI for
 * slave mode. After it is  initialized, the SPI module is configured in slave mode and the
 * user can start transmitting and receiving data by calling send, receive, and transfer functions.
 * This function indicates SPI slave uses an interrupt mechanism.
 *
 * @return status_t An error code or STATUS_SUCCESS.
 */
status_t SPI_DRV_SlaveInit(uint32_t instance, spi_state_t *pstSpiState,
                           const spi_slave_config_t *pstSlaveConfig)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(pstSpiState != NULL);
    OS_ASSERT(pstSlaveConfig != NULL);
    spi_type_t *pstBase           = g_spiBase[instance];
    status_t errorCode            = STATUS_SUCCESS;
    OS_SemaphoreAttr_t semSpiAttr = {0};
    OS_RegisterType_t uType       = {0};

    /* enable spi instance clock */
    CLOCK_SYS_ClockEnable(g_spiClock[instance], true);

    pstSpiState->lsb            = pstSlaveConfig->lsbFirst;
    pstSpiState->bitsPerFrame   = pstSlaveConfig->bitcount;
    pstSpiState->euTransferType = pstSlaveConfig->euTransferType;
    pstSpiState->isBlocking     = false;
    /* Store PDMA channels numbers used for PDMA transfer */
    pstSpiState->rxDMAChannel = pstSlaveConfig->rxDMAChannel;
    pstSpiState->txDMAChannel = pstSlaveConfig->txDMAChannel;
    /* Store callback */
    pstSpiState->callback      = pstSlaveConfig->callback;
    pstSpiState->callbackParam = pstSlaveConfig->callbackParam;
    /* Calculate the bytes/frame for pstSpiState->bytesPerFrame. */
    pstSpiState->bytesPerFrame = (uint16_t)((pstSpiState->bitsPerFrame + 7U) / 8U);
    /* Due to PDMA limitations frames of 3 bytes/frame will be internally handled as 4 bytes/frame. */
    if (pstSpiState->bytesPerFrame == 3U) {
        pstSpiState->bytesPerFrame = 4U;
    }
    /* Due to some limitations all frames bigger than 4 bytes/frame must be composed only from 4 bytes chunks. */
    if (pstSpiState->bytesPerFrame > 4U) {
        pstSpiState->bytesPerFrame = (((pstSpiState->bytesPerFrame - 1U) / 4U) + 1U) * 4U;
    }
    pstSpiState->isTransferInProgress = false;
    /* Initialize the semaphore */
    semSpiAttr.name           = "spi_slave";
    semSpiAttr.cb_mem         = NULL;
    pstSpiState->spiSemaphore = OS_SemaphoreNew(1, 1, &semSpiAttr);
    OS_ASSERT(NULL != pstSpiState->spiSemaphore);

    g_pstSpiState[instance] = pstSpiState;

    /* Configure registers */
    SPI_Init(pstBase);

    /* Configure spi to slave mode */
    (void)SPI_SetMasterSlaveMode(pstBase, SPI_SLAVE);

    /* Set Pin settings */
    if (pstSlaveConfig->euWidth == SPI_SINGLE_BIT_XFER) {
        /* Set Pin configuration such that SDO=in and SDI=out */
        //(void)SPI_SetPinConfigMode(pstBase, SPI_SDI_OUT_SDO_IN, SPI_DATA_OUT_RETAINED, true);
        /* Set Pin configuration such that SDO=out and SDI=in */
        (void)SPI_SetPinConfigMode(pstBase, SPI_SDI_IN_SDO_OUT, SPI_DATA_OUT_RETAINED, true);
    } else {
        (void)SPI_SetPinConfigMode(pstBase, SPI_SDO_IN_OUT, SPI_DATA_OUT_TRISTATE, true);
    }

    /* Calculate the FIFO size for the SPI */
    SPI_GetFifoSizes(pstBase, &(pstSpiState->fifoSize));

    /* Set Data Match Function */
    if (pstSlaveConfig->stDataMatchCfg.dataMatchMode != SPI_DATA_MATCH_DISABLE) {
        pstSpiState->stDataMatchCfg.data0                = pstSlaveConfig->stDataMatchCfg.data0;
        pstSpiState->stDataMatchCfg.data1                = pstSlaveConfig->stDataMatchCfg.data1;
        pstSpiState->stDataMatchCfg.dataMatchMode        = pstSlaveConfig->stDataMatchCfg.dataMatchMode;
        pstSpiState->stDataMatchCfg.receiveDataMatchOnly = pstSlaveConfig->stDataMatchCfg.receiveDataMatchOnly;

        SPI_SetDataMatch(pstBase, &(pstSlaveConfig->stDataMatchCfg));
        /* Enable data match interrupts sources */
        SPI_SetIntMode(pstBase, SPI_DATA_MATCH, true);
    } else {
        /* Disable data match interrupts sources */
        SPI_SetIntMode(pstBase, SPI_DATA_MATCH, false);
    }

    /* Set polarity */
    (void)SPI_SetPcsPolarityMode(pstBase, pstSlaveConfig->euWhichPcs, pstSlaveConfig->euPcsPolarity);

    /* Write the TCR for this transfer */
    spi_tx_cmd_config_t txCmdCfg = {.frameSize     = pstSpiState->bitsPerFrame,
                                    .euWidth       = SPI_SINGLE_BIT_XFER,
                                    .txMask        = false,
                                    .rxMask        = false,
                                    .byteSwap      = false,
                                    .lsbFirst      = pstSlaveConfig->lsbFirst,
                                    .euClkPhase    = pstSlaveConfig->euClkPhase,
                                    .euClkPolarity = pstSlaveConfig->euClkPolarity,
                                    .euWhichPcs    = pstSlaveConfig->euWhichPcs};

    /* Write to the TX CMD register */
    SPI_SetTxCommandReg(pstBase, &txCmdCfg);
    SPI_Enable(pstBase);
    if (pstSpiState->euTransferType == SPI_USING_INTERRUPTS) {
        /* Install SPI irq handler */
        uType.trig_mode = CLIC_LEVEL_TRIGGER;
        uType.lvl       = 251;
        uType.priority  = 1;
        uType.data_ptr  = &(g_spiInstance[instance]);
        OS_RequestIrq(g_spiIrqId[instance], SPI_IRQHandler, &uType);
        //OS_RequestIrq(g_spiIrqId[instance + 4], SPI_IRQHandler, &uType);

        /* Enable the interrupt source */
        OS_EnableIrq(g_spiIrqId[instance]);
        //OS_EnableIrq(g_spiIrqId[instance + 4]);
    }

    return errorCode;
}

/**
 *
 * Function Name : SPI_DRV_SlaveDeinit
 * Description   : Shuts down an SPI instance interrupt mechanism.
 *
 * Disables the SPI module, gates its clock, and changes the SPI slave driver state to NonInit for the
 * SPI slave module which is initialized with interrupt mechanism. After de-initialization, the
 * user can re-initialize the SPI slave module with other mechanisms.
 *
 * @return status_t
 * STATUS_SUCCESS if driver starts to send/receive data successfully.
 * STATUS_ERROR if driver is error and needs to clean error.
 * STATUS_BUSY if a transfer is in progress
 */
status_t SPI_DRV_SlaveDeinit(uint32_t instance)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    /* Instantiate local variable of type spi_master_state_t and point to global state */
    const spi_state_t *pstSpiState = (spi_state_t *)g_pstSpiState[instance];
    spi_type_t *pstBase            = g_spiBase[instance];
    status_t errorCode             = STATUS_SUCCESS;

    /* Check if a transfer is still in progress */
    OS_ASSERT(pstSpiState->isTransferInProgress == false);
    /* Destroy the semaphore */
    if (OS_SemaphoreDelete(pstSpiState->spiSemaphore) != OS_OK) {
        errorCode = STATUS_ERROR;
    }
    OS_ASSERT(errorCode == STATUS_SUCCESS);
    /* Reset the SPI registers to their default state, including disabling the SPI */
    SPI_Init(pstBase);

    /* Disable the interrupt*/
    OS_DisableIrq(g_spiIrqId[instance]);

    /* Clear the state pointer. */
    g_pstSpiState[instance] = NULL;

    return errorCode;
}

/**
 *
 * Function Name : SPI_DRV_SlaveTransferBlocking
 * Description   : Transfers data on SPI bus using a blocking call.
 *
 * This function checks the driver status and mechanism and transmits/receives data through the SPI
 * bus. If the sendBuffer is NULL, the transmit process is ignored. If the receiveBuffer is NULL, the
 * receive process is ignored. If both the receiveBuffer and the sendBuffer are available, the transmit and the
 * receive progress is processed. If only the receiveBuffer is available, the receive is
 * processed. Otherwise, the transmit is processed. This function only returns when the
 * processes are completed. This function uses an interrupt mechanism.
 * Depending on frame size sendBuffer and receiveBuffer must be aligned like this:
 * -1 byte if frame size <= 8 bits
 * -2 bytes if 8 bits < frame size <= 16 bits
 * -4 bytes if 16 bits < frame size
 *
 * @return status_t
 * STATUS_SUCCESS if driver starts to send/receive data successfully.
 * STATUS_ERROR if driver is error and needs to clean error.
 * STATUS_BUSY if a transfer is in progress
 * STATUS_TIMEOUT if time out reached while transferring is in progress.
 */
status_t SPI_DRV_SlaveTransferBlocking(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                                       uint16_t transferByteCount, uint32_t timeout)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    spi_state_t *state        = (spi_state_t *)g_pstSpiState[instance];
    const spi_type_t *pstBase = g_spiBase[instance];
    status_t error            = STATUS_SUCCESS;
    status_t osifError        = STATUS_SUCCESS;

    /* Check if another transfer is in progress */
    if (SPI_GetStatusFlag(pstBase, SPI_MODULE_BUSY)) {
        return STATUS_BUSY;
    }

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)OS_SemaphoreAcquire(state->spiSemaphore, 0);
    state->isBlocking = true;

    error = SPI_DRV_SlaveTransfer(instance, sendBuffer, receiveBuffer, transferByteCount);
    if (error != STATUS_SUCCESS) {
        state->isBlocking = false;
        SPI_DRV_DisableTEIEInterrupts(instance);
        return error;
    }
    /* As this is a synchronous transfer, wait until the transfer is complete.*/
    if (OS_SemaphoreAcquire(state->spiSemaphore, timeout) != OS_OK) {
        osifError = STATUS_ERROR;
    }
    OS_ASSERT(osifError == STATUS_SUCCESS);
    if (osifError == STATUS_TIMEOUT) {
        /* Set isBlocking variable to false to avoid dummy semaphore post. */
        state->isBlocking = false;
        /* Complete transfer. */
        (void)SPI_DRV_SlaveAbortTransfer(instance);
        return (STATUS_TIMEOUT);
    }

    SPI_DRV_DisableTEIEInterrupts(instance);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : SPI_DRV_SlaveTransfer
 * Description   : Starts the transfer data on SPI bus using a non-blocking call.
 *
 * This function checks the driver status and mechanism and transmits/receives data through the SPI
 * bus. If the sendBuffer is NULL, the transmit process is ignored. If the receiveBuffer is NULL, the
 * receive process is ignored. If both the receiveBuffer and the sendBuffer are available, the transmit and the
 * receive progress is processed. If only the receiveBuffer is available, the receive is
 * processed. Otherwise, the transmit is processed. This function only returns when the
 * processes are completed. This function uses an interrupt mechanism.
 * Depending on frame size sendBuffer and receiveBuffer must be aligned like this:
 * -1 byte if frame size <= 8 bits
 * -2 bytes if 8 bits < frame size <= 16 bits
 * -4 bytes if 16 bits < frame size
 *
 * @return status_t
 * STATUS_SUCCESS if driver starts to send/receive data successfully.
 * STATUS_ERROR if driver is error and needs to clean error.
 * STATUS_BUSY if a transfer is in progress
 */
status_t SPI_DRV_SlaveTransfer(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                               uint16_t transferByteCount)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    OS_ASSERT(!((sendBuffer == NULL) && (receiveBuffer == NULL)));
    spi_type_t *pstBase                  = g_spiBase[instance];
    spi_state_t *state                   = (spi_state_t *)g_pstSpiState[instance];
    state->dummy                         = 0xFFU;
    pdma_transfer_size_t dmaTransferSize = PDMA_TRANSFER_SIZE_1B;
    const uint8_t *buffer;

    /* The number of transferred bytes should be divisible by frame size */
    if ((uint16_t)(transferByteCount % state->bytesPerFrame) != (uint16_t)0) {
        return STATUS_ERROR;
    }
    /* Check if SPI module isn't busy */
    if (state->isTransferInProgress == true) {
        return STATUS_BUSY;
    }
    /* Initialize the status of the current transfer */
    state->euStatus = SPI_TRANSFER_OK;
    /* Clean RX and TX buffers */
    SPI_SetFlushFifoCmd(pstBase, true, true);
    /* The second flush command is used to avoid the case when one word is still in shifter. */
    SPI_SetFlushFifoCmd(pstBase, true, true);
    /* Clear all interrupts sources */
    (void)SPI_ClearStatusFlag(pstBase, SPI_ALL_STATUS);
    /* Enable fault interrupts sources */
    SPI_SetIntMode(pstBase, SPI_TRANSMIT_ERROR, true);
    SPI_SetIntMode(pstBase, SPI_RECEIVE_ERROR, true);
    /* Enable data match interrupts sources */
    if (state->stDataMatchCfg.dataMatchMode != SPI_DATA_MATCH_DISABLE) {
        SPI_SetIntMode(pstBase, SPI_DATA_MATCH, true);
    }
    /* Fill out the other members of the run-time state structure. */
    state->txBuff = (const uint8_t *)sendBuffer;
    state->rxBuff = (uint8_t *)receiveBuffer;
    if (state->euTransferType == SPI_USING_INTERRUPTS) {

        if (state->txBuff != NULL) {
            state->txCount = transferByteCount;
            SPI_ClearTxMaskBit(pstBase);
        } else {
            state->txCount = 0;
            SPI_SetTxmskBit(pstBase);
        }

        if (state->rxBuff != NULL) {
            state->rxCount = transferByteCount;
            SPI_ClearRxmaskBit(pstBase);
        } else {
            state->rxCount = 0;
            SPI_SetRxmskBit(pstBase);
        }

        state->txFrameCnt      = 0;
        state->rxFrameCnt      = 0;
        state->isPcsContinuous = false;
        /* Configure watermarks */
        SPI_SetRxWatermarks(pstBase, 0U);
        SPI_SetTxWatermarks(pstBase, 2U);

        state->isTransferInProgress = true;
        /* Enable interrupts for RX and TX only if it's necessary */
        if (state->txBuff != NULL) {
            SPI_SetIntMode(pstBase, SPI_TX_DATA_FLAG, true);
        }
        if (state->rxBuff != NULL) {
            SPI_SetIntMode(pstBase, SPI_RX_DATA_FLAG, true);
        }

    } else {
        /* Configure watermarks */
        SPI_SetRxWatermarks(pstBase, 0U);
        SPI_SetTxWatermarks(pstBase, 3U);
        /* When SPI use PDMA frames with 3 bytes size are not accepted. */
        switch (state->bytesPerFrame) {
        case 1:
            dmaTransferSize = PDMA_TRANSFER_SIZE_1B;
            break;
        case 2:
            dmaTransferSize = PDMA_TRANSFER_SIZE_2B;
            break;
        case 4:
            dmaTransferSize = PDMA_TRANSFER_SIZE_4B;
            break;
        default:
            dmaTransferSize = PDMA_TRANSFER_SIZE_4B;
            break;
        }

        if (receiveBuffer != NULL) {
            state->rxCount = transferByteCount;
            buffer         = receiveBuffer;
        } else {
            state->rxCount = 0U;
            /* if there is no data to receive, use dummy data as destination for PDMA transfer */
            buffer = (uint8_t *)(&(state->dummy));
        }
        (void)PDMA_DRV_ConfigMultiBlockTransfer(
            state->rxDMAChannel, PDMA_TRANSFER_PERIPH2MEM, (uint32_t)(&(pstBase->RDR)), (uint32_t)buffer,
            dmaTransferSize, (uint32_t)1U << (uint8_t)(dmaTransferSize),
            (uint32_t)transferByteCount / (uint32_t)((uint32_t)1U << (uint8_t)(dmaTransferSize)), true);
        if (receiveBuffer == NULL) {
            /* if there is no data to receive, don't increment destination offset */
            PDMA_DRV_SetDestOffset(state->rxDMAChannel, 0);
        }

        if (sendBuffer != NULL) {
            state->txCount = transferByteCount;
            buffer         = sendBuffer;
        } else {
            state->txCount = 0U;
            /* if there is no data to send, use dummy data as source for PDMA transfer */
            buffer = (uint8_t *)(&(state->dummy));
        }
        (void)PDMA_DRV_ConfigMultiBlockTransfer(
            state->txDMAChannel, PDMA_TRANSFER_MEM2PERIPH, (uint32_t)buffer, (uint32_t)(&(pstBase->TDR)),
            dmaTransferSize, (uint32_t)1U << (uint8_t)(dmaTransferSize),
            (uint32_t)transferByteCount / (uint32_t)((uint32_t)1U << (uint8_t)(dmaTransferSize)), true);
        if (sendBuffer == NULL) {
            /* if there is no data to transmit, don't increment source offset */
            PDMA_DRV_SetSrcOffset(state->txDMAChannel, 0);
        }

        (void)PDMA_DRV_InstallCallback(state->rxDMAChannel, (SPI_DRV_SlaveCompleteDMATransfer), (void *)(instance));
        state->isTransferInProgress = true;

        /* Start RX channel */
        (void)PDMA_DRV_StartChannel(state->rxDMAChannel);
        /* Start TX channel */
        (void)PDMA_DRV_StartChannel(state->txDMAChannel);
        /* Enable SPI PDMA requests */
        SPI_SetRxDmaCmd(pstBase, true);
        SPI_SetTxDmaCmd(pstBase, true);
    }
    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : SPI_DRV_SlaveIRQHandler
 * Description   : Interrupt handler for SPI slave mode.
 *
 * This handler uses the buffers stored in the spi_master_state_t structs to transfer data.
 *
 */
void SPI_DRV_SlaveIRQHandler(uint32_t instance)
{
    spi_type_t *pstBase      = g_spiBase[instance];
    spi_state_t *pstSpiState = (spi_state_t *)g_pstSpiState[instance];

    /* If an error is detected the transfer will be aborted */
    if ((bool)SPI_GetStatusFlag(pstBase, SPI_TRANSMIT_ERROR) && (pstSpiState->txBuff != NULL)) {
        (void)SPI_DRV_SlaveAbortTransfer(instance);
        pstSpiState->euStatus = SPI_TRANSMIT_FAIL;
        return;
    }
    if (SPI_GetStatusFlag(pstBase, SPI_RECEIVE_ERROR) && (pstSpiState->rxBuff != NULL)) {
        (void)SPI_DRV_SlaveAbortTransfer(instance);
        pstSpiState->euStatus = SPI_RECEIVE_FAIL;
        return;
    }

    /* Receive data */
    if (SPI_GetStatusFlag(pstBase, SPI_RX_DATA_FLAG)) {
        if ((pstSpiState->rxCount != (uint8_t)0)) {
            SPI_DRV_ReadRXBuffer(instance);
        }
    }
    /* Transmit data */
    if (SPI_GetStatusFlag(pstBase, SPI_TX_DATA_FLAG)) {
        if ((pstSpiState->txCount != (uint8_t)0)) {
            SPI_DRV_FillupTxBuffer(instance);
        }
    }
    /* Match data */
    if (SPI_GetStatusFlag(pstBase, SPI_DATA_MATCH)) {
        if (pstSpiState->callback != NULL) {
            pstSpiState->callback(pstSpiState, SPI_EVENT_DATA_MATCH, pstSpiState->callbackParam);
        }
        /* Clear match interrupts sources */
        (void)SPI_ClearStatusFlag(pstBase, SPI_DATA_MATCH);
    }
    /* If all bytes are sent disable interrupt TDF */
    if (pstSpiState->txCount == (uint8_t)0) {
        SPI_SetIntMode(pstBase, SPI_TX_DATA_FLAG, false);
    }
    /* If all bytes are received disable interrupt RDF */
    if (pstSpiState->rxCount == (uint8_t)0) {
        SPI_SetIntMode(pstBase, SPI_RX_DATA_FLAG, false);
    }
    if (pstSpiState->rxCount == (uint8_t)0) {
        if (pstSpiState->txCount == (uint8_t)0) {
            /* Disable fault interrupts sources */
            SPI_SetIntMode(pstBase, SPI_TRANSMIT_ERROR, false);
            SPI_SetIntMode(pstBase, SPI_RECEIVE_ERROR, false);

            /* Call the callback if it is defined */
            if (pstSpiState->callback != NULL) {
                pstSpiState->callback(pstSpiState, SPI_EVENT_END_TRANSFER, pstSpiState->callbackParam);
            }

            /* If the transfer is blocking post the semaphore */
            if (pstSpiState->isBlocking == true) {
                (void)OS_SemaphoreRelease(pstSpiState->spiSemaphore);
                pstSpiState->isBlocking = false;
            }

            /* Update internal state of the transfer */
            pstSpiState->isTransferInProgress = false;
        }
    }
}

/**
 *
 * Function Name : SPI_DRV_SlaveAbortTransfer
 * Description   : Aborts the transfer that started by a non-blocking call transfer function.
 *
 * This function stops the transfer which was started by the calling the SPI_DRV_SlaveTransfer() function.
 *
 * @return status_t
 * STATUS_SUCCESS if everything is OK.
 */
status_t SPI_DRV_SlaveAbortTransfer(uint32_t instance)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    spi_type_t *pstBase = g_spiBase[instance];
    spi_state_t *state  = (spi_state_t *)g_pstSpiState[instance];

    if (state->euTransferType == SPI_USING_INTERRUPTS) {
        /* Disable interrupts */
        SPI_SetIntMode(pstBase, SPI_TX_DATA_FLAG, false);
        SPI_SetIntMode(pstBase, SPI_RX_DATA_FLAG, false);
    } else {
        /* Disable SPI PDMA request */
        SPI_SetRxDmaCmd(pstBase, false);
        SPI_SetTxDmaCmd(pstBase, false);
    }

    SPI_DRV_DisableTEIEInterrupts(instance);

    state->isTransferInProgress = false;
    /* Clean RX and TX buffers */
    SPI_SetFlushFifoCmd(pstBase, true, true);
    /* The second flush command is used to avoid the case when one word is still in shifter. */
    SPI_SetFlushFifoCmd(pstBase, true, true);
    if (state->isBlocking == true) {
        (void)OS_SemaphoreRelease(state->spiSemaphore);
        state->isBlocking = false;
    }
    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : SPI_DRV_SlaveGetTransferStatus
 * Description   : Returns whether the previous transfer is finished.
 *
 * When performing an a-sync transfer, the user can call this function to ascertain
 * the state of the current transfer: in progress (or busy) or complete (success).
 * In addition, if the transfer is still in progress, the user can get the number
 * of bytes that have been transferred up to now.
 *
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully, or
 * STATUS_BUSY The transfer is still in progress.
 * STATUS_ERROR if driver is error and needs to clean error.
 */
status_t SPI_DRV_SlaveGetTransferStatus(uint32_t instance, uint32_t *bytesRemained)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    const spi_state_t *pstSpiState = (spi_state_t *)g_pstSpiState[instance];

    /* Fill in the bytes transferred.*/
    if (bytesRemained != NULL) {
        *bytesRemained = pstSpiState->txCount;
    }
    if (pstSpiState->euStatus == SPI_TRANSFER_OK) {
        return (status_t)(pstSpiState->isTransferInProgress ? STATUS_BUSY : STATUS_SUCCESS);
    } else {
        return STATUS_ERROR;
    }
}

/**
 *
 * Function Name : SPI_DRV_SlaveCompleteDMATransfer
 * Description   : Finish up a transfer PDMA.
 * The main purpose of this function is to create a function compatible with PDMA callback type
 * @param euStatus pdma channel status struct
 */
static void SPI_DRV_SlaveCompleteDMATransfer(void *parameter, pdma_chn_status_t euStatus)
{
    uint32_t instance        = (uint32_t)parameter;
    spi_state_t *pstSpiState = (spi_state_t *)g_pstSpiState[instance];

    (void)euStatus;
    (void)SPI_DRV_SlaveAbortTransfer(instance);

    /* Check RX and TX PDMA channels. */
    if (PDMA_DRV_GetChannelStatus(pstSpiState->txDMAChannel) == PDMA_CHN_ERROR) {
        pstSpiState->euStatus = SPI_TRANSMIT_FAIL;
    }
    if (PDMA_DRV_GetChannelStatus(pstSpiState->rxDMAChannel) == PDMA_CHN_ERROR) {
        pstSpiState->euStatus = SPI_RECEIVE_FAIL;
    }

    if (pstSpiState->callback != NULL) {
        pstSpiState->callback(pstSpiState, SPI_EVENT_END_TRANSFER, pstSpiState->callbackParam);
    }
}