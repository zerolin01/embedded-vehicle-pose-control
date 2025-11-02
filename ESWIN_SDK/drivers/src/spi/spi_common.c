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
 * @file spi_common.c
 * @brief SPI driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:        Version:        Author:
 * Changes:
 *
 */
#include "spi_hw_access.h"
#include <assert.h>

/** @brief SPI clock*/
clock_names_t g_spiClock[] = SPI_CLOCK_NAMES;

/**
 * @brief Table of pstBase pointers for SPI instances.
 */
spi_type_t *g_spiBase[SPI_INSTANCE_COUNT] = SPI_BASE_PTRS;

/**
 * @brief Table to save SPI IRQ enumeration numbers defined in the CMSIS header file.
 */
#define SPI_IRQ_NUM SPI_INSTANCE_COUNT + 4
IRQn_Type g_spiIrqId[SPI_IRQ_NUM] = SPI_IRQS;

/** Pointer to runtime state structure.*/
spi_state_t *g_pstSpiState[SPI_INSTANCE_COUNT] = {NULL, NULL, NULL, NULL};

uint8_t g_spiInstance[SPI_INSTANCE_COUNT] = {0, 1, 2, 3};

/**
 *
 * Function Name : SPI_IRQHandler
 * Description   : SPI IRQ Handler that registered to original default IRQ Handler
 *
 */
void SPI_IRQHandler(void *data)
{
    uint8_t inst = *(uint8_t *)data;
    SPI_DRV_IRQHandler(inst);
    return;
}

/**
 *
 * Function Name : SPI_DRV_IRQHandler
 * Description   : The function SPI_DRV_IRQHandler passes IRQ control to either the master or
 * slave driver.
 *
 * The address of the IRQ handlers are checked to make sure they are non-zero before
 * they are called. If the IRQ handler's address is zero, it means that driver was
 * not present in the link (because the IRQ handlers are marked as weak). This would
 * actually be a program error, because it means the master/slave config for the IRQ
 * was set incorrectly.
 *
 */
void SPI_DRV_IRQHandler(uint32_t instance)
{
    if (instance < SPI_INSTANCE_COUNT) {
        const spi_type_t *pstBase = g_spiBase[instance];

        if (SPI_IsMaster(pstBase)) {
            /* Master mode.*/
            SPI_DRV_MasterIRQHandler(instance);
        } else {
            /* Slave mode.*/
            SPI_DRV_SlaveIRQHandler(instance);
        }
    }
}

/**
 *
 * Function Name : SPI_DRV_FillupTxBuffer
 * Description   : Fill up the TX FIFO with data.
 * This function fills up the TX FIFO with data based on the bytes/frame.
 * This is not a public API as it is called from other driver functions.
 *
 */
void SPI_DRV_FillupTxBuffer(uint32_t instance)
{
    /* Instantiate local variable of type spi_master_state_t and point to global state. */
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];
    uint32_t wordToSend      = 0;
    uint16_t numOfBytes;
    uint8_t availableSpace = (uint8_t)(pstSpiState->fifoSize - (uint8_t)SPI_ReadTxCount(pstBase));

    /* Fill the TX buffer. */
    while (availableSpace != 0U) {
        if (pstSpiState->isPcsContinuous == true) {
            if (pstSpiState->txCount == 1U) {
                /* Disable continuous PCS */
                SPI_ClearContCBit(pstBase);
                pstSpiState->txCount = 0U;
                break;
            }
        }
        /* Get the number of bytes which can be written in a single 32 bits word. */
        if ((pstSpiState->bytesPerFrame - pstSpiState->txFrameCnt) <= (uint16_t)4) {
            numOfBytes = (uint16_t)(pstSpiState->bytesPerFrame - pstSpiState->txFrameCnt);
        } else {
            numOfBytes = 4U;
        }
        wordToSend = 0;

        if (pstSpiState->txBuff != NULL) {
            switch (pstSpiState->bytesPerFrame) {
            case 1:
                wordToSend = *((const uint8_t *)(pstSpiState->txBuff));
                pstSpiState->txBuff += sizeof(uint8_t);
                break;
            case 2:
                wordToSend = *((const uint16_t *)(pstSpiState->txBuff));
                pstSpiState->txBuff += sizeof(uint16_t);
                break;
            default:
                wordToSend = *((const uint32_t *)(pstSpiState->txBuff));
                pstSpiState->txBuff += sizeof(uint32_t);
                break;
            }
        }
        pstSpiState->txFrameCnt =
            (uint16_t)((pstSpiState->txFrameCnt + numOfBytes) % pstSpiState->bytesPerFrame);
        SPI_WriteData(pstBase, wordToSend);

        /* Update internal variable used in transmission. */
        pstSpiState->txCount = (uint16_t)(pstSpiState->txCount - numOfBytes);
        /* Verify if all bytes were send. */
        if (pstSpiState->txCount == 0U) {
            break;
        }
        availableSpace = (uint8_t)(availableSpace - 1U);
    }
}

/**
 *
 * Function Name : SPI_DRV_ReadRXBuffer
 * Description   : Read all data from RX FIFO
 * This function will read all data from RX FIFO and will transfer this information in
 * RX software buffer.
 * This is not a public API as it is called from other driver functions.
 */
void SPI_DRV_ReadRXBuffer(uint32_t instance)
{
    spi_state_t *pstSpiState  = g_pstSpiState[instance];
    const spi_type_t *pstBase = g_spiBase[instance];
    uint32_t receivedWord;
    uint16_t numOfBytes;
    uint16_t j;
    uint8_t filledSpace = (uint8_t)SPI_ReadRxCount(pstBase);
    while (filledSpace != 0U) {
        receivedWord = SPI_ReadData(pstBase);
        /* Get the number of bytes which can be read from this 32 bites */
        if ((pstSpiState->bytesPerFrame - pstSpiState->rxFrameCnt) <= (uint16_t)4) {
            numOfBytes = (uint16_t)(pstSpiState->bytesPerFrame - pstSpiState->rxFrameCnt);
        } else {
            numOfBytes = 4U;
        }
        /* Generate the word which will be write in buffer. */
        for (j = 0; j < numOfBytes; j++) {

            *(pstSpiState->rxBuff) = (uint8_t)(receivedWord >> (j * 8U));
            pstSpiState->rxBuff++;
        }
        pstSpiState->rxFrameCnt = (uint16_t)((pstSpiState->rxFrameCnt + numOfBytes) % pstSpiState->bytesPerFrame);

        /* Update internal variable used in transmission. */
        pstSpiState->rxCount = (uint16_t)(pstSpiState->rxCount - numOfBytes);
        /* Verify if all bytes were sent. */
        if (pstSpiState->rxCount == 0U) {
            break;
        }
        filledSpace = (uint8_t)(filledSpace - 1U);
    }
}

/**
 *
 * Function Name : SPI_DRV_DisableTEIEInterrupts
 * Description   : Disable the TEIE interrupts at the end of a transfer.
 * Disable the interrupts and clear the status for transmit/receive errors.
 *
 */
void SPI_DRV_DisableTEIEInterrupts(uint32_t instance)
{
    spi_type_t *pstBase = g_spiBase[instance];

    SPI_SetIntMode(pstBase, SPI_TRANSMIT_ERROR, false);
    SPI_SetIntMode(pstBase, SPI_RECEIVE_ERROR, false);
    (void)SPI_ClearStatusFlag(pstBase, SPI_TRANSMIT_ERROR);
    (void)SPI_ClearStatusFlag(pstBase, SPI_RECEIVE_ERROR);
}

/**
 *
 * Function Name : SPI_DRV_ConfigSleepMode
 * Description   : Config TCSR SPI enter sleep mode
 *
 */
void SPI_DRV_ConfigSleepMode(uint32_t instance, uint8_t sleep)
{
    uint32_t reg = TCSR->STOP;
    reg          = (reg & ~(TCSR_STOP_SPI0_MASK << instance)) |
          ((sleep & 0x1) << (TCSR_STOP_SPI0_SHIFT + instance));
    TCSR->STOP = reg;
    return;
}

/**
 *
 * Function Name : SPI_DRV_SwitchSleepMode
 * Description   : Config SPI module switch in/out sleep mode
 *
 */
void SPI_DRV_SwitchSleepMode(uint32_t instance, uint8_t sleep)
{
    //switch spi sleep mode
    SPI_DRV_ConfigSleepMode(instance, sleep);
}

/**
 *
 * Function Name : SPI_DRV_CheckSleepMode
 * Description   : Config SPI check if in sleep mode
 *
 */
int SPI_DRV_CheckSleepMode(uint32_t instance)
{
    uint32_t reg = TCSR->STOPACK;
    reg          = (reg & (TCSR_STOP_SPI0_MASK << instance));

    return (reg >> (TCSR_STOP_SPI0_SHIFT + instance));
}
/**
 *
 * Function Name : SPI_DRV_MasterCompleteDMATransfer
 * Description   : Finish up a transfer PDMA.
 * The main purpose of this function is to create a function compatible with PDMA callback type
 */
static void SPI_DRV_MasterCompleteDMATransferE(void *parameter, pdma_chn_status_t status)
{
    uint32_t instance        = (uint32_t)parameter;
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];
    if (status == PDMA_CHN_ERROR) {
        pstSpiState->euStatus = SPI_TRANSMIT_FAIL;
    } else {
        /* Enable transfer complete flag interrupt to catch the end of the transfer. */
        pstSpiState->txCount = 0;
        pstSpiState->rxCount = 0;
        SPI_SetIntMode(pstBase, SPI_TRANSFER_COMPLETE, true);
        pstSpiState->isTransferInProgress = false;
    }
}
/**
 *
 * Function Name : SPI_DRV_WriteData
 * Description   : Write data to TDR
 *
 */
status_t SPI_DRV_WriteDataDma(uint32_t instance, uint8_t *data, uint32_t size)
{
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];

    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    OS_ASSERT(data != NULL);
    /* Clear all interrupts sources */
    (void)SPI_ClearStatusFlag(pstBase, SPI_ALL_STATUS);
    /* Verify if the number of bytes is divided by number of bytes/frame. */
    if ((size % pstSpiState->bytesPerFrame) != (uint16_t)0) {
        return STATUS_ERROR;
    }
    pdma_transfer_size_t dmaTransferSize = PDMA_TRANSFER_SIZE_1B;
    /* When SPI use PDMA frames with 3 bytes size are not accepted. */
    switch (pstSpiState->bytesPerFrame) {
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

    if (data != NULL) {
        (void)PDMA_DRV_ConfigMultiBlockTransfer(
            pstSpiState->txDMAChannel, PDMA_TRANSFER_MEM2PERIPH, (uint32_t)data,
            (uint32_t)(&(pstBase->TDR)), dmaTransferSize, (uint32_t)1U << (uint8_t)(dmaTransferSize),
            (uint32_t)size / (uint32_t)((uint32_t)1U << (uint8_t)(dmaTransferSize)), true);
    }
    /* If RX buffer is null the transfer is done when all bytes were sent. */
    (void)PDMA_DRV_InstallCallback(pstSpiState->txDMAChannel, (SPI_DRV_MasterCompleteDMATransferE),
                                   (void *)(instance));

    /* Start TX channel */
    (void)PDMA_DRV_StartChannel(pstSpiState->txDMAChannel);
    /* Update transfer status */
    pstSpiState->isTransferInProgress = true;

    SPI_SetTxDmaCmd(pstBase, true);
    while (SPI_ReadTxCount(pstBase) == pstSpiState->fifoSize) {
    }

    return STATUS_SUCCESS;
}
/**
 *
 * Function Name : SPI_DRV_WriteData
 * Description   : Write data to TDR
 *
 */
status_t SPI_DRV_WriteData(uint32_t instance, uint8_t *data, uint32_t size)
{
    uint32_t i               = 0;
    uint32_t wordToSend      = 0;
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];
    uint32_t bytesPerframe   = (uint16_t)((pstSpiState->bitsPerFrame + 7U) / 8U);
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    OS_ASSERT(data != NULL);
    /* Clear all interrupts sources */
    (void)SPI_ClearStatusFlag(pstBase, SPI_ALL_STATUS);
    /* Verify if the number of bytes is divided by number of bytes/frame. */
    if ((size % pstSpiState->bytesPerFrame) != (uint16_t)0) {
        return STATUS_ERROR;
    }
    for (i = 0; i < size;) {
        while (pstSpiState->fifoSize - (uint8_t)SPI_ReadTxCount(pstBase) == 0) {
        }
        switch (bytesPerframe) {
        case 1:
            wordToSend = *((const uint8_t *)(data + i));
            i += 1;
            break;
        case 2:
            wordToSend = *((const uint16_t *)(data + i));
            i += 2;
            break;
        default:
            wordToSend = *((const uint32_t *)(data + i));
            i += 4;
            break;
        }
        SPI_WriteData(pstBase, wordToSend);
    }
    while (SPI_ReadTxCount(pstBase) == pstSpiState->fifoSize) {
    }
    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : SPI_DRV_WriteCmd
 * Description   : Write Cmd to TCR
 *
 */
status_t SPI_DRV_WriteCmd(uint32_t instance, const spi_tx_cfg_t *pstTxCfg)
{
    spi_type_t *pstBase      = g_spiBase[instance];
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_tx_cmd_config_t txCmdCfg;
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    if (pstTxCfg->txMask == 1) { //must meet or transfer cannot stop
        OS_ASSERT(pstTxCfg->contTransfer != 1);
    }
    if (pstTxCfg->changeTransferType) {
        SPI_Disable(pstBase);
        if (pstTxCfg->euWidth == SPI_SINGLE_BIT_XFER) {
            (void)SPI_SetPinConfigMode(pstBase, SPI_SDI_IN_SDO_OUT, SPI_DATA_OUT_RETAINED, true);
        } else if (pstTxCfg->euWidth == SPI_TWO_BIT_XFER) {
            (void)SPI_SetPinConfigMode(pstBase, SPI_SDO_IN_OUT, SPI_DATA_OUT_TRISTATE, false);
        } else if (pstTxCfg->euWidth == SPI_FOUR_BIT_XFER) {
            (void)SPI_SetPinConfigMode(pstBase, SPI_SDO_IN_OUT, SPI_DATA_OUT_TRISTATE, false);
        } else {
            (void)SPI_SetPinConfigMode(pstBase, SPI_SDO_IN_OUT, SPI_DATA_OUT_TRISTATE, false);
        }

        if ((pstTxCfg->txMask == 1) && (pstTxCfg->rxMask == 0) &&
            ((pstTxCfg->euWidth == SPI_FOUR_BIT_XFER) | (pstTxCfg->euWidth == SPI_TWO_BIT_XFER))) {
            (void)SPI_SetPinConfigMode(pstBase, SPI_SDI_IN_OUT, SPI_DATA_OUT_TRISTATE, false);
        }
        SPI_Enable(pstBase);
    }

    SPI_GetTxCommandReg(g_spiBase[instance], &txCmdCfg);
    txCmdCfg.contCmd      = pstTxCfg->contCmd;
    txCmdCfg.contTransfer = pstTxCfg->contTransfer;
    txCmdCfg.euWidth      = pstTxCfg->euWidth;
    txCmdCfg.frameSize    = pstTxCfg->frameSize;
    txCmdCfg.rxMask       = pstTxCfg->rxMask;
    txCmdCfg.txMask       = pstTxCfg->txMask;
    if (txCmdCfg.euWidth == SPI_SINGLE_BIT_XFER_HALF_DUPLEX) {
        txCmdCfg.euWidth = SPI_SINGLE_BIT_XFER;
    }
    pstSpiState->bitsPerFrame  = pstTxCfg->frameSize;
    pstSpiState->bytesPerFrame = (uint16_t)((pstSpiState->bitsPerFrame + 7U) / 8U);
    pstSpiState->euWidth       = txCmdCfg.euWidth;

    while (pstSpiState->fifoSize - (uint8_t)SPI_ReadTxCount(pstBase) == 0) {
    }
    SPI_SetTxCommandReg(g_spiBase[instance], &txCmdCfg);
    while (SPI_ReadTxCount(pstBase) == pstSpiState->fifoSize) {
    }
    return STATUS_SUCCESS;
}
/**
 *
 * Function Name : SPI_DRV_MasterCompleteRX
 * Description   : Check if errors are detected on RX channel
 * The main purpose of this function is to check PDMA errors on rx channel
 */
static void SPI_DRV_MasterCompleteRXE(void *parameter, pdma_chn_status_t euStatus)
{
    uint32_t instance        = (uint32_t)parameter;
    spi_state_t *pstSpiState = g_pstSpiState[instance];

    if (euStatus == PDMA_CHN_ERROR) {
        pstSpiState->euStatus = SPI_TRANSMIT_FAIL;
    }
    if (pstSpiState->callback != NULL) {
        pstSpiState->callback(pstSpiState, SPI_EVENT_END_TRANSFER, pstSpiState->callbackParam);
    }
    pstSpiState->isTransferInProgress = false;
}
/**
 *
 * Function Name : SPI_DRV_RecvData
 * Description   : Write data to TDR
 *
 */
void SPI_DRV_RecvDataDma(uint32_t instance, uint8_t *data, uint32_t size)
{
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    OS_ASSERT(data != NULL);
    pstSpiState->rxBuff  = data;
    pstSpiState->rxCount = size;
    pdma_transfer_size_t dmaTransferSize = PDMA_TRANSFER_SIZE_1B;
    pstSpiState->bytesPerFrame = size;
    /* When SPI use PDMA frames with 3 bytes size are not accepted. */
    switch (pstSpiState->bytesPerFrame) {
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
    if (data != NULL) {
        (void)PDMA_DRV_ConfigMultiBlockTransfer(
            pstSpiState->rxDMAChannel, PDMA_TRANSFER_PERIPH2MEM, (uint32_t)(&(pstBase->RDR)),
            (uint32_t)data, dmaTransferSize, (uint32_t)1U << (uint8_t)(dmaTransferSize),
            (uint32_t)size / (uint32_t)((uint32_t)1U << (uint8_t)(dmaTransferSize)), true);
        (void)PDMA_DRV_InstallCallback(pstSpiState->rxDMAChannel, (SPI_DRV_MasterCompleteRXE),
                                       (void *)(instance));
        /* Start RX channel */
        (void)PDMA_DRV_StartChannel(pstSpiState->rxDMAChannel);
        /* Update transfer status */
        pstSpiState->isTransferInProgress = true;
        SPI_SetRxDmaCmd(pstBase, true);
    }


    return;
}

/**
 *
 * Function Name : SPI_DRV_RecvData
 * Description   : Write data to TDR
 *
 */
void SPI_DRV_RecvData(uint32_t instance, uint8_t *data, uint32_t size)
{
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    OS_ASSERT(data != NULL);
    pstSpiState->rxBuff  = data;
    pstSpiState->rxCount = size;
    SPI_SetIntMode(pstBase, SPI_RX_DATA_FLAG, true);

    return;
}

/**
 *
 * Function Name : SPI_DRV_WaitSendComplete
 * Description   : wait transfer complete
 *
 */
void SPI_DRV_WaitSendComplete(uint32_t instance)
{
    spi_type_t *pstBase = g_spiBase[instance];

    while (SPI_GetStatusFlag(pstBase, SPI_TRANSFER_COMPLETE) == false) {
    }
    return;
}

/**
 *
 * Function Name : SPI_DRV_WaitRecvComplete
 * Description   : wait transfer complete
 *
 */
void SPI_DRV_WaitRecvComplete(uint32_t instance)
{
    spi_type_t *pstBase = g_spiBase[instance];
    /** when complete recv will clear SPI_RX_DATA_FLAG in interrupt,
        cannot check SPI_TRANSFER_COMPLETE for SPI_TRANSFER_COMPLETE will clear in interrupt
    */
    while (SPI_GetIntMode(pstBase, SPI_RX_DATA_FLAG) == true) {
    }
    return;
}

/**
 *
 * Function Name : SPI_DRV_WaitRecvComplete
 * Description   : wait transfer complete
 *
 */
void SPI_DRV_WaitRecvCompleteDma(uint32_t instance)
{
    spi_type_t *pstBase = g_spiBase[instance];

    while (SPI_GetStatusFlag(pstBase, SPI_FRAME_COMPLETE) == false) {
    }
    return;
}
