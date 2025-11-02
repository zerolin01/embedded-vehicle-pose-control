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
 * @file spi_master_driver.c
 * @brief SPI master driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:        Version:        Author:
 * Changes:
 *
 */

#include "spi_master_driver.h"
#include "clock_driver.h"
#include "spi_hw_access.h"
#include "spi_register.h"
#include <string.h>

/**
 * @brief Table of pstBase pointers for SPI instances.
 */
extern spi_type_t *g_spiBase[SPI_INSTANCE_COUNT];

/**
 * @brief Table to save SPI IRQ enumeration numbers defined in the CMSIS header file.
 */
extern IRQn_Type g_spiIrqId[SPI_INSTANCE_COUNT];

/** @brief Pointer to runtime state structure.*/
extern spi_state_t *g_pstSpiState[SPI_INSTANCE_COUNT];

extern uint8_t g_spiInstance[SPI_INSTANCE_COUNT];

/** This function initialize a new SPI transfer */
static status_t SPI_DRV_MasterStartTransfer(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                                            uint16_t transferByteCount);

/** This function cleans up state structure and hardware after a transfer is complete .*/
static void SPI_DRV_MasterCompleteTransfer(uint32_t instance);

/** Callback for PDMA transfer done.*/
static void SPI_DRV_MasterCompleteDMATransfer(void *parameter, pdma_chn_status_t euStatus);

/** Callback for RX PDMA channel*/
static void SPI_DRV_MasterCompleteRX(void *parameter, pdma_chn_status_t euStatus);

/**
 * 
 * Function Name : SPI_DRV_MasterGetDefaultConfig
 * Description   : Initializes a structured provided by user with the configuration
 * of an interrupt based SPI transfer. Source clock for SPI is configured to
 * 8MHz. If the applications uses other frequency is necessary to update spiSrcClk field.
 *
 */
void SPI_DRV_MasterGetDefaultConfig(spi_master_config_t *pstSpiConfig)
{
    OS_ASSERT(pstSpiConfig != NULL);

    pstSpiConfig->bitsPerSec      = 1000000UL,
    pstSpiConfig->euWhichPcs      = SPI_PCS0;
    pstSpiConfig->euPcsPolarity   = SPI_ACTIVE_HIGH;
    pstSpiConfig->isPcsContinuous = false;
    pstSpiConfig->bitcount        = 8U;
    pstSpiConfig->euClkPhase      = SPI_CLOCK_PHASE_1ST_EDGE;
    pstSpiConfig->euClkPolarity   = SPI_SCK_ACTIVE_HIGH;
    pstSpiConfig->lsbFirst        = false;
    pstSpiConfig->euTransferType  = SPI_USING_INTERRUPTS;
    pstSpiConfig->rxDMAChannel    = 0;
    pstSpiConfig->txDMAChannel    = 0;
    pstSpiConfig->callback        = NULL;
    pstSpiConfig->callbackParam   = NULL;
}

/**
 * 
 * Function Name : SPI_DRV_MasterGetClock
 * Description   : Get SPI Clock
 *
 */
static uint32_t SPI_DRV_MasterGetClock(uint32_t instance)
{
    uint32_t functionalClk = 0;
    status_t clkErr        = STATUS_SUCCESS;
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
#if defined(EMPS_SIMULATION)
    functionalClk = 40000000;
#else
    clkErr = CLOCK_SYS_GetFreq(g_spiClock[instance], &functionalClk);
#endif
    
    
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(functionalClk > 0U);

    return functionalClk;
}

/**
 * 
 * Function Name : SPI_DRV_MasterInit
 * Description   : Initializes a SPI instance for interrupt driven master mode operation.
 *
 * This function uses an interrupt-driven method for transferring data.
 * In this function, the term "pstSpiConfig" is used to indicate the SPI device for which the SPI
 * master is communicating.
 * This function initializes the run-time state structure to track the ongoing
 * transfers, un-gates the clock to the SPI module, resets the SPI module,
 * configures the IRQ state structure, enables the module-level interrupt to the core, and
 * enables the SPI module.
 *
 * @return status_t
 * An error code or STATUS_SUCCESS.
 */
status_t SPI_DRV_MasterInit(uint32_t instance, spi_state_t *pstSpiState,
                            const spi_master_config_t *pstSpiConfig)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(pstSpiState != NULL);
    OS_ASSERT(pstSpiConfig != NULL);
    spi_type_t *pstBase           = g_spiBase[instance];
    status_t errorCode            = STATUS_SUCCESS;
    OS_SemaphoreAttr_t semSpiAttr = {0};
    OS_RegisterType_t uType       = {0};

    /* enable spi instance clock */
    CLOCK_SYS_ClockEnable(g_spiClock[instance], true);
    /* Save runtime structure pointers so irq handler can point to the correct state structure */
    g_pstSpiState[instance] = pstSpiState;
    /* Reset the SPI registers to their default state */
    SPI_Init(pstBase);
    /* Set for master mode */
    (void)SPI_SetMasterSlaveMode(pstBase, SPI_MASTER);
    if (pstSpiConfig->euWidth == SPI_SINGLE_BIT_XFER) {
        (void)SPI_SetPinConfigMode(pstBase, SPI_SDI_IN_SDO_OUT, SPI_DATA_OUT_RETAINED, true);
    } else if (pstSpiConfig->euWidth == SPI_TWO_BIT_XFER) {
        (void)SPI_SetPinConfigMode(pstBase, SPI_SDO_IN_OUT, SPI_DATA_OUT_TRISTATE, false);
    } else if (pstSpiConfig->euWidth == SPI_FOUR_BIT_XFER) {
        (void)SPI_SetPinConfigMode(pstBase, SPI_SDO_IN_OUT, SPI_DATA_OUT_TRISTATE, false);
    } else {
        (void)SPI_SetPinConfigMode(pstBase, SPI_SDO_IN_OUT, SPI_DATA_OUT_TRISTATE, false);
    }
    /* Calculate the FIFO size for the SPI */
    SPI_GetFifoSizes(pstBase, &(pstSpiState->fifoSize));

    /* Set Data Match Function */
    if (pstSpiConfig->stDataMatchCfg.dataMatchMode != SPI_DATA_MATCH_DISABLE) {
        pstSpiState->stDataMatchCfg.data0                = pstSpiConfig->stDataMatchCfg.data0;
        pstSpiState->stDataMatchCfg.data1                = pstSpiConfig->stDataMatchCfg.data1;
        pstSpiState->stDataMatchCfg.dataMatchMode        = pstSpiConfig->stDataMatchCfg.dataMatchMode;
        pstSpiState->stDataMatchCfg.receiveDataMatchOnly = pstSpiConfig->stDataMatchCfg.receiveDataMatchOnly;

        SPI_SetDataMatch(pstBase, &(pstSpiConfig->stDataMatchCfg));
        /* Enable data match interrupts sources */
        SPI_SetIntMode(pstBase, SPI_DATA_MATCH, true);
    } else {
        /* Disable data match interrupts sources */
        SPI_SetIntMode(pstBase, SPI_DATA_MATCH, false);
    }

    /* Configure bus for this device. If NULL is passed, we assume the caller has
     * preConfigured the bus and doesn't wish to re-configure it again for this transfer.
     * Do nothing for calculatedBaudRate. If the user wants to know the calculatedBaudRate
     * then they can call this function separately.
     */
    errorCode = SPI_DRV_MasterConfigureBus(instance, pstSpiConfig, NULL);
    if (errorCode != STATUS_SUCCESS) {
        return errorCode;
    }
    /* When TX is null the value sent on the bus will be 0 */
    pstSpiState->dummy = 0;
    /* Initialize the semaphore */
    semSpiAttr.name           = "spi_master";
    semSpiAttr.cb_mem         = NULL;
    pstSpiState->spiSemaphore = OS_SemaphoreNew(1, 1, &semSpiAttr);
    OS_ASSERT(NULL != pstSpiState->spiSemaphore);

    if (pstSpiState->euTransferType == SPI_USING_INTERRUPTS) {
        /* Install SPI irq handler */
        uType.trig_mode = CLIC_LEVEL_TRIGGER;
        uType.lvl       = 250;
        uType.priority  = 1;
        uType.data_ptr  = &(g_spiInstance[instance]);
        OS_RequestIrq(g_spiIrqId[instance], SPI_IRQHandler, &uType);
        //OS_RequestIrq(g_spiIrqId[instance + 4], SPI_IRQHandler, &uType);

        /* Enable the interrupt */
        OS_EnableIrq(g_spiIrqId[instance]);
        //OS_EnableIrq(g_spiIrqId[instance + 4]);
    }

    /* Set Master Trigger enable */
    SPI_SetTriggerEnable(pstBase, pstSpiConfig->RequestEnable);
    SPI_SetTriggerSelect(pstBase, pstSpiConfig->euRequestSelect);

    SPI_SetTriggerPolarity(pstBase, pstSpiConfig->euRequestPolarity);
    /* Finally, enable SPI */
    SPI_Enable(pstBase);
    return errorCode;
}

/**
 * 
 * Function Name : SPI_DRV_MasterDeinit
 * Description   : Shuts down a SPI instance.
 *
 * This function resets the SPI peripheral, gates its clock, and disables the interrupt to
 * the core.  It first checks to see if a transfer is in progress and if so returns an error
 * status.
 *
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully, or
 * STATUS_BUSY The transfer is still in progress.
 * STATUS_ERROR if driver is error and needs to clean error.
 *
 */
status_t SPI_DRV_MasterDeinit(uint32_t instance)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    /* Instantiate local variable of type spi_state_t and point to global state */
    const spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase            = g_spiBase[instance];
    status_t errorCode             = STATUS_SUCCESS;

    /* Check if a transfer is still in progress */
    OS_ASSERT(pstSpiState->isTransferInProgress == false);

    /* Reset the SPI registers to their default state, including disabling the SPI */
    SPI_Init(pstBase);
    /* Disable the interrupt */
    OS_DisableIrq(g_spiIrqId[instance]);
    /* Clear the state pointer. */
    g_pstSpiState[instance] = NULL;

    /* Destroy the semaphore */
    if (OS_SemaphoreDelete(pstSpiState->spiSemaphore) != OS_OK) {
        errorCode = STATUS_ERROR;
    }

    OS_ASSERT(errorCode == STATUS_SUCCESS);
    return errorCode;
}

/**
 * 
 * Function Name : SPI_DRV_MasterSetDelay
 * Description   : Configures the SPI master mode bus timing delay options.
 *
 * This function involves the SPI module's delay options to
 * "fine tune" some of the signal timings and match the timing needs of a slower peripheral device.
 * This is an optional function that can be called after the SPI module has been initialized for
 * master mode. The timings are adjusted in terms of cycles of the baud rate clock.
 * The bus timing delays that can be adjusted are listed below:
 *
 * SCK to PCS Delay: Adjustable delay option between the last edge of SCK to the de-assertion
 *                   of the PCS signal.
 *
 * PCS to SCK Delay: Adjustable delay option between the assertion of the PCS signal to the
 *                   first SCK edge.
 *
 * Delay between Transfers: Adjustable delay option between the de-assertion of the PCS signal for
 *                          a frame to the assertion of the PCS signal for the next frame.
 *
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully, or
 * STATUS_ERROR if driver is error and needs to clean error.
 */
status_t SPI_DRV_MasterSetDelay(uint32_t instance, uint32_t delayBetweenTransfers, uint32_t delaySCKtoPCS,
                                uint32_t delayPCStoSCK)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);

    /* Instantiate local variable of type spi_type_t and point to global state */
    spi_type_t *pstBase = g_spiBase[instance];
    status_t errorCode  = STATUS_SUCCESS;

    /* Disable module */
    errorCode = SPI_Disable(pstBase);
    if (errorCode != STATUS_SUCCESS) {
        return errorCode;
    }

    (void)SPI_SetDelay(pstBase, SPI_SCK_TO_PCS, delaySCKtoPCS);
    (void)SPI_SetDelay(pstBase, SPI_PCS_TO_SCK, delayPCStoSCK);
    (void)SPI_SetDelay(pstBase, SPI_BETWEEN_TRANSFER, delayBetweenTransfers);
    /* Enable module */
    SPI_Enable(pstBase);
    return errorCode;
}

/**
 * 
 * Function Name : SPI_DRV_MasterConfigureBus
 * Description   : Configures the SPI port physical parameters to access a device on the bus when the LSPI
 *        instance is configured for interrupt operation.
 *
 * In this function, the term "spiConfig" is used to indicate the SPI device for which the SPI
 * master is communicating. This is an optional function as the spiConfig parameters are
 * normally configured in the initialization function or the transfer functions, where these various
 * functions would call the configure bus function.
 * This is an example to set up the spi_master_config_t structure
 * to call the SPI_DRV_MasterConfigureBus function by passing in these parameters:
   @code
    spi_master_config_t spiConfig1;   You can also declare spiConfig2, spiConfig3, etc
    spiConfig.bitsPerSec = 500000;
    spiConfig.euWhichPcs = SPI_PCS0;
    spiConfig.euPcsPolarity = SPI_ACTIVE_LOW;
    spiConfig.isPcsContinuous = false;
    spiConfig.bitCount = 16;
    spiConfig.euClkPhase = SPI_CLOCK_PHASE_1ST_EDGE;
    spiConfig.euClkPolarity = SPI_ACTIVE_HIGH;
    spiConfig.lsbFirst= false;
    spiConfig.euTransferType = SPI_USING_INTERRUPTS;
   @endcode
 *
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully, or
 * STATUS_ERROR if driver is error and needs to clean error.
 */
status_t SPI_DRV_MasterConfigureBus(uint32_t instance, const spi_master_config_t *pstSpiConfig,
                                    uint32_t *calculatedBaudRate)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(pstSpiConfig != NULL);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    /* Instantiate local variable of type spi_state_t and point to global state */
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];
    uint32_t baudRate;

    /* The Transmit Command Register (TCR) Prescale value is calculated as part of the baud rate
       calculation. The value is stored in the run-time state structure for later programming
       in the TCR. */
    uint32_t tcrPrescaleValue;

    /* First, per the spec, we need to disable the SPI module before setting the delay */

    if (SPI_Disable(pstBase) != STATUS_SUCCESS) {
        /* If error is returned, the SPI is busy */
        return STATUS_ERROR;
    }

    /* Check the bitcount to make sure it falls within the boundary conditions */
    if ((pstSpiConfig->bitcount < 8U) || (pstSpiConfig->bitcount > 4096U)) {
        return STATUS_ERROR;
    }

    /* Configure internal state structure for SPI */
    pstSpiState->bitsPerFrame = pstSpiConfig->bitcount;
    /* Get the Clock frequence for the SPI */
    pstSpiState->spiSrcClk       = SPI_DRV_MasterGetClock(instance);
    pstSpiState->isPcsContinuous = pstSpiConfig->isPcsContinuous;
    pstSpiState->lsb             = pstSpiConfig->lsbFirst;
    /* Save transfer type PDMA/Interrupt */
    pstSpiState->euTransferType = pstSpiConfig->euTransferType;
    /* Update transfer status */
    pstSpiState->isTransferInProgress = false;
    pstSpiState->isBlocking           = false;
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
    /* Store PDMA channel number used in transfer */
    pstSpiState->rxDMAChannel = pstSpiConfig->rxDMAChannel;
    pstSpiState->txDMAChannel = pstSpiConfig->txDMAChannel;
    /* Store callback */
    pstSpiState->callback      = pstSpiConfig->callback;
    pstSpiState->callbackParam = pstSpiConfig->callbackParam;
    /* Configure the desired PCS polarity */
    (void)SPI_SetPcsPolarityMode(pstBase, pstSpiConfig->euWhichPcs, pstSpiConfig->euPcsPolarity);
    /* Set up the baud rate */
    baudRate = SPI_SetBaudRate(pstBase, pstSpiConfig->bitsPerSec, pstSpiState->spiSrcClk, &tcrPrescaleValue);
    /* Enable sampling point delay */
    SPI_SetSamplingPoint(pstBase, true);
    /* Now, re-enable the SPI module */
    SPI_Enable(pstBase);
    /* If the baud rate return is "0", it means there was an error */
    if (baudRate == (uint32_t)0) {
        return STATUS_ERROR;
    }
    /* If the user wishes to know the calculated baud rate, then pass it back */
    if (calculatedBaudRate != NULL) {
        *calculatedBaudRate = baudRate;
    }
    /* Write the TCR for this transfer. */
    spi_tx_cmd_config_t txCmdCfg = {.frameSize     = pstSpiState->bitsPerFrame,
                                    .euWidth       = pstSpiConfig->euWidth,
                                    .txMask        = false,
                                    .rxMask        = false,
                                    .contCmd       = false,
                                    .contTransfer  = pstSpiConfig->isPcsContinuous,
                                    .byteSwap      = false,
                                    .lsbFirst      = pstSpiConfig->lsbFirst,
                                    .euWhichPcs    = pstSpiConfig->euWhichPcs,
                                    .preDiv        = tcrPrescaleValue,
                                    .euClkPhase    = pstSpiConfig->euClkPhase,
                                    .euClkPolarity = pstSpiConfig->euClkPolarity};
    if(pstSpiConfig->euWidth == SPI_SINGLE_BIT_XFER_HALF_DUPLEX) {
        txCmdCfg.euWidth = SPI_SINGLE_BIT_XFER;
    }
    SPI_SetTxCommandReg(pstBase, &txCmdCfg);
    return STATUS_SUCCESS;
}

/**
 * 
 * Function Name : SPI_DRV_MasterTransferBlocking
 * Description   : Performs an interrupt driven blocking SPI master mode transfer.
 *
 * This function simultaneously sends and receives data on the SPI bus, as SPI is naturally
 * a full-duplex bus. The function does not return until the transfer is complete.
 * This function allows the user to optionally pass in a SPI configuration structure which
 * allows the user to change the SPI bus attributes in conjunction with initiating a SPI transfer.
 * The difference between passing in the SPI configuration structure here as opposed to the
 * configure bus function is that the configure bus function returns the calculated baud rate where
 * this function does not. The user can also call the configure bus function prior to the transfer
 * in which case the user would simply pass in a NULL to the transfer function's device structure
 * parameter.
 * Depending on frame size sendBuffer and receiveBuffer must be aligned like this:
 * -1 byte if frame size <= 8 bits
 * -2 bytes if 8 bits < frame size <= 16 bits
 * -4 bytes if 16 bits < frame size
 *
 * @return status_t
 * STATUS_SUCCESS The transfer was successful, or
 * STATUS_BUSY Cannot perform transfer because a transfer is already in progress, or
 * STATUS_TIMEOUT The transfer timed out and was aborted.
 */
status_t SPI_DRV_MasterTransferBlocking(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                                        uint16_t transferByteCount, uint32_t timeout)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    /* Instantiate local variable of type spi_state_t and point to global state */
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];
    status_t error           = STATUS_SUCCESS;
    status_t osifError       = STATUS_SUCCESS;
    /* If the transfer count is zero, then return immediately.*/
    if (transferByteCount == (uint16_t)0) {
        return error;
    }

    /* Check if another transfer is in progress */
    if (SPI_GetStatusFlag(pstBase, SPI_MODULE_BUSY)) {
        return STATUS_BUSY;
    }

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    OS_SemaphoreAcquire(pstSpiState->spiSemaphore, 0);
    pstSpiState->isBlocking = true;

    error = SPI_DRV_MasterStartTransfer(instance, sendBuffer, receiveBuffer, transferByteCount);
    /* Start the transfer process, if it returns an error code, return this back to user */
    if (error != STATUS_SUCCESS) {
        /* Disable interrupt requests*/
        SPI_SetIntMode(pstBase, SPI_TX_DATA_FLAG, false);
        SPI_SetIntMode(pstBase, SPI_RX_DATA_FLAG, false);

        SPI_DRV_DisableTEIEInterrupts(instance);
        SPI_SetIntMode(pstBase, SPI_TRANSFER_COMPLETE, false);
        (void)SPI_ClearStatusFlag(pstBase, SPI_TRANSFER_COMPLETE);

        pstSpiState->isBlocking = false;
        return error;
    }

    /* As this is a synchronous transfer, wait until the transfer is complete.*/
    if (OS_SemaphoreAcquire(pstSpiState->spiSemaphore, timeout) != OS_OK) {
        osifError = STATUS_ERROR;
    }
    OS_ASSERT(osifError == STATUS_SUCCESS);

    /* If a timeout occurs, stop the transfer by setting the isTransferInProgress to false and
     * disabling interrupts, then return the timeout error status.
     */
    if (osifError == STATUS_TIMEOUT) {
        /* Set isBlocking variable to false to avoid dummy semaphore post. */
        pstSpiState->isBlocking = false;
        /* Complete transfer. */
        SPI_DRV_MasterCompleteTransfer(instance);
        return (STATUS_TIMEOUT);
    }

    SPI_DRV_DisableTEIEInterrupts(instance);
    SPI_SetIntMode(pstBase, SPI_TRANSFER_COMPLETE, false);
    (void)SPI_ClearStatusFlag(pstBase, SPI_TRANSFER_COMPLETE);

    return error;
}

/**
 * 
 * Function Name : SPI_DRV_MasterTransfer
 * Description   : Performs an interrupt driven non-blocking SPI master mode transfer.
 *
 * This function simultaneously sends and receives data on the SPI bus, as SPI is naturally
 * a full-duplex bus. The function returns immediately after initiating the transfer. The user
 * needs to check whether the transfer is complete using the SPI_DRV_MasterGetTransferStatus
 * function.
 * This function allows the user to optionally pass in a SPI configuration structure which
 * allows the user to change the SPI bus attributes in conjunction with initiating a SPI transfer.
 * The difference between passing in the SPI configuration structure here as opposed to the
 * configure bus function is that the configure bus function returns the calculated baud rate where
 * this function does not. The user can also call the configure bus function prior to the transfer
 * in which case the user would simply pass in a NULL to the transfer function's device structure
 * parameter.
 * Depending on frame size sendBuffer and receiveBuffer must be aligned like this:
 * -1 byte if frame size <= 8 bits
 * -2 bytes if 8 bits < frame size <= 16 bits
 * -4 bytes if 16 bits < frame size
 *
 * @return status_t
 * STATUS_SUCCESS The transfer was successful, or
 * STATUS_BUSY Cannot perform transfer because a transfer is already in progress
 */
status_t SPI_DRV_MasterTransfer(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                                uint16_t transferByteCount)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    status_t error = STATUS_SUCCESS;
    /* If the transfer count is zero, then return immediately.*/
    if (transferByteCount == (uint16_t)0) {
        return STATUS_SUCCESS;
    }

    /* Start the transfer process, if it returns an error code, return this back to user */
    error = SPI_DRV_MasterStartTransfer(instance, sendBuffer, receiveBuffer, transferByteCount);
    if (error != STATUS_SUCCESS) {
        return error;
    }

    /* Else, return immediately as this is an async transfer */
    return STATUS_SUCCESS;
}

/**
 * 
 * Function Name : SPI_DRV_MasterGetTransferStatus
 * Description   : Returns whether the previous interrupt driven transfer is completed.
 *
 * When performing an a-sync (non-blocking) transfer, the user can call this function to ascertain
 * the state of the current transfer: in progress (or busy) or complete (success).
 * In addition, if the transfer is still in progress, the user can get the number of words that
 * have been transferred up to now.
 *
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully, or
 * STATUS_BUSY The transfer is still in progress. framesTransferred is filled
 * with the number of words that have been transferred so far.
 */
status_t SPI_DRV_MasterGetTransferStatus(uint32_t instance, uint32_t *bytesRemained)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    /* Instantiate local variable of type spi_state_t and point to global state */
    const spi_state_t *pstSpiState = g_pstSpiState[instance];
    /* Fill in the bytes transferred.*/
    if (bytesRemained != NULL) {
        *bytesRemained = pstSpiState->rxCount;
    }
    if (pstSpiState->euStatus == SPI_TRANSFER_OK) {
        return (status_t)(pstSpiState->isTransferInProgress ? STATUS_BUSY : STATUS_SUCCESS);
    } else {
        return STATUS_ERROR;
    }
}
/**
 * 
 * Function Name : SPI_DRV_MasterAbortTransfer
 * Description   : Terminates an interrupt driven asynchronous transfer early.
 *
 * During an a-sync (non-blocking) transfer, the user has the option to terminate the transfer early
 * if the transfer is still in progress.
 * 
 * @return status_t
 * STATUS_SUCCESS The transfer was successful, or
 * SPI_STATUS_NO_TRANSFER_IN_PROGRESS No transfer is currently in progress.
 */
status_t SPI_DRV_MasterAbortTransfer(uint32_t instance)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    spi_type_t *pstBase = g_spiBase[instance];
    /* Stop the running transfer. */
    SPI_DRV_MasterCompleteTransfer(instance);
    SPI_SetFlushFifoCmd(pstBase, true, true);
    /* The second flush command is used to avoid the case when one word is still in shifter. */
    SPI_SetFlushFifoCmd(pstBase, true, true);
    return STATUS_SUCCESS;
}

/**
 * 
 * Function Name : SPI_DRV_SetPcs
 * Description   : Select the chip to communicate with.
 *
 * The main purpose of this function is to set the PCS and the appropriate polarity.
 *
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully, or
 * STATUS_ERROR if driver is error and needs to clean error.
 */
status_t SPI_DRV_SetPcs(uint32_t instance, spi_which_pcs_t euWhichPcs, spi_signal_polarity_t euPolarity)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    OS_ASSERT((uint32_t)euWhichPcs < 4U);

    spi_type_t *pstBase = g_spiBase[instance];
    status_t result;

    if (SPI_Disable(pstBase) != STATUS_SUCCESS) {
        return STATUS_ERROR;
    }
    result = SPI_SetPcsPolarityMode(pstBase, euWhichPcs, euPolarity);
    if (result != STATUS_SUCCESS) {
        return STATUS_ERROR;
    }
    SPI_Enable(pstBase);
    SPI_SetPcs(pstBase, euWhichPcs);
    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : SPI_DRV_MasterStartTransfer
 * Description   : Configure a non-blocking transfer.
 *
 * The number of transferByteCount must be divided by number of bytes/frame.
 * The sendBuffer must be not NULL, but receiveBuffer can be NULL.
 *
 */
static status_t SPI_DRV_MasterStartTransfer(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                                            uint16_t transferByteCount)
{
    OS_ASSERT(instance < SPI_INSTANCE_COUNT);
    OS_ASSERT(g_pstSpiState[instance] != NULL);
    /* Instantiate local variable of type spi_master_state_t and point to global state */
    spi_state_t *pstSpiState             = g_pstSpiState[instance];
    spi_type_t *pstBase                  = g_spiBase[instance];
    pdma_transfer_size_t dmaTransferSize = PDMA_TRANSFER_SIZE_1B;

    /* Check that we're not busy. */
    if (SPI_GetStatusFlag(pstBase, SPI_MODULE_BUSY)) {
        return STATUS_BUSY;
    }

    /* Verify if the number of bytes is divided by number of bytes/frame. */
    if ((transferByteCount % pstSpiState->bytesPerFrame) != (uint16_t)0) {
        return STATUS_ERROR;
    }

    /* Clean RX and TX buffers */
    SPI_SetFlushFifoCmd(pstBase, true, true);
    /* The second flush command is used to avoid the case when one word is still in shifter. */
    SPI_SetFlushFifoCmd(pstBase, true, true);

    if (pstSpiState->isPcsContinuous == true) {
        SPI_SetContCBit(pstBase);
    }
    pstSpiState->euStatus = SPI_TRANSFER_OK;
    /* Clear all interrupts sources */
    (void)SPI_ClearStatusFlag(pstBase, SPI_ALL_STATUS);
    /* Enable fault interrupts sources */
    SPI_SetIntMode(pstBase, SPI_TRANSMIT_ERROR, true);
    if (receiveBuffer != NULL) {
        SPI_SetIntMode(pstBase, SPI_RECEIVE_ERROR, true);
    }

    /* Configure rxCount depending on transfer type.*/
    if (receiveBuffer == NULL) {
        pstSpiState->rxCount = 0;
        SPI_SetRxmskBit(pstBase);
    } else {
        pstSpiState->rxCount = transferByteCount;
        SPI_ClearRxmaskBit(pstBase);
    }

    /* Configure watermarks */
    SPI_SetRxWatermarks(pstBase, 0U);
    SPI_SetTxWatermarks(pstBase, 2U);

    if (pstSpiState->euTransferType == SPI_USING_INTERRUPTS) {

        /* Fill out the other members of the run-time state structure. */
        pstSpiState->txBuff     = (const uint8_t *)sendBuffer;
        pstSpiState->rxBuff     = (uint8_t *)receiveBuffer;
        pstSpiState->txFrameCnt = 0;
        pstSpiState->rxFrameCnt = 0;
        pstSpiState->txCount    = transferByteCount;
        /*For continuous mode an extra word must be written to negate the PCS */
        if (pstSpiState->isPcsContinuous == true) {
            pstSpiState->txCount++;
        }

        /* Update transfer status */
        pstSpiState->isTransferInProgress = true;
        /* Enable RDF interrupt if RX buffer is not NULL. */
        if (pstSpiState->rxBuff != NULL) {
            SPI_SetIntMode(pstBase, SPI_RX_DATA_FLAG, true);
        }
        
        /* Enable the TDF and RDF interrupt. */
        SPI_SetIntMode(pstBase, SPI_TX_DATA_FLAG, true);
    } else {
        pstSpiState->txBuff = (const uint8_t *)sendBuffer;
        pstSpiState->rxBuff = (uint8_t *)receiveBuffer;
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
        /* Configure TX PDMA channel */
        if (sendBuffer != NULL) {
            (void)PDMA_DRV_ConfigMultiBlockTransfer(
                pstSpiState->txDMAChannel, PDMA_TRANSFER_MEM2PERIPH, (uint32_t)sendBuffer,
                (uint32_t)(&(pstBase->TDR)), dmaTransferSize, (uint32_t)1U << (uint8_t)(dmaTransferSize),
                (uint32_t)transferByteCount / (uint32_t)((uint32_t)1U << (uint8_t)(dmaTransferSize)), true);
        } else {
            (void)PDMA_DRV_ConfigMultiBlockTransfer(
                pstSpiState->txDMAChannel, PDMA_TRANSFER_PERIPH2PERIPH, (uint32_t)(&(pstSpiState->dummy)),
                (uint32_t)(&(pstBase->TDR)), dmaTransferSize, (uint32_t)1U << (uint8_t)(dmaTransferSize),
                (uint32_t)transferByteCount / (uint32_t)((uint32_t)1U << (uint8_t)(dmaTransferSize)), true);
        }
        /* Configure RX PDMA channel if is used in current transfer. */
        if (receiveBuffer != NULL) {
            (void)PDMA_DRV_ConfigMultiBlockTransfer(
                pstSpiState->rxDMAChannel, PDMA_TRANSFER_PERIPH2MEM, (uint32_t)(&(pstBase->RDR)),
                (uint32_t)receiveBuffer, dmaTransferSize, (uint32_t)1U << (uint8_t)(dmaTransferSize),
                (uint32_t)transferByteCount / (uint32_t)((uint32_t)1U << (uint8_t)(dmaTransferSize)), true);
            (void)PDMA_DRV_InstallCallback(pstSpiState->rxDMAChannel, (SPI_DRV_MasterCompleteRX),
                                           (void *)(instance));
            /* Start RX channel */
            (void)PDMA_DRV_StartChannel(pstSpiState->rxDMAChannel);
        }

        /* If RX buffer is null the transfer is done when all bytes were sent. */
        (void)PDMA_DRV_InstallCallback(pstSpiState->txDMAChannel, (SPI_DRV_MasterCompleteDMATransfer),
                                       (void *)(instance));

        /* Start TX channel */
        (void)PDMA_DRV_StartChannel(pstSpiState->txDMAChannel);
        /* Update transfer status */
        pstSpiState->isTransferInProgress = true;
        /* Enable SPI PDMA request */
        if (receiveBuffer != NULL) {
            SPI_SetRxDmaCmd(pstBase, true);
        }
        SPI_SetTxDmaCmd(pstBase, true);
    }
    return STATUS_SUCCESS;
}

/**
 * 
 * Function Name : SPI_DRV_MasterCompleteTransfer
 * Description   : Finish up a transfer.
 * Cleans up after a transfer is complete. Interrupts are disabled, and the SPI module
 * is disabled. This is not a public API as it is called from other driver functions.
 */
static void SPI_DRV_MasterCompleteTransfer(uint32_t instance)
{
    /* instantiate local variable of type spi_master_state_t and point to global state */
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];
    /* The transfer is complete.*/
    pstSpiState->isTransferInProgress = false;
    if (pstSpiState->euTransferType == SPI_USING_DMA) {
        /* Disable SPI PDMA request */
        SPI_SetRxDmaCmd(pstBase, false);
        SPI_SetTxDmaCmd(pstBase, false);
    } else {
        /* Disable (clear) interrupt requests */
        SPI_SetIntMode(pstBase, SPI_RX_DATA_FLAG, false);
        SPI_SetIntMode(pstBase, SPI_TX_DATA_FLAG, false);
    }

    SPI_DRV_DisableTEIEInterrupts(instance);
    SPI_SetIntMode(pstBase, SPI_TRANSFER_COMPLETE, false);
    (void)SPI_ClearStatusFlag(pstBase, SPI_TRANSFER_COMPLETE);
    if (pstSpiState->isBlocking == true) {
        (void)OS_SemaphoreRelease(pstSpiState->spiSemaphore);
        pstSpiState->isBlocking = false;
    }
    if (pstSpiState->callback != NULL) {
        pstSpiState->callback(pstSpiState, SPI_EVENT_END_TRANSFER, pstSpiState->callbackParam);
    }
    trace("SPI_DRV_MasterCompleteTransfer\r\n");
}

/**
 * 
 * Function Name : SPI_DRV_MasterCompleteDMATransfer
 * Description   : Finish up a transfer PDMA.
 * The main purpose of this function is to create a function compatible with PDMA callback type
 */
static void SPI_DRV_MasterCompleteDMATransfer(void *parameter, pdma_chn_status_t status)
{
    uint32_t instance        = (uint32_t)parameter;
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];

    if (status == PDMA_CHN_ERROR) {
        (void)SPI_DRV_MasterAbortTransfer(instance);
        pstSpiState->euStatus = SPI_TRANSMIT_FAIL;
    } else {
        if (pstSpiState->isPcsContinuous == true) {
            SPI_ClearContCBit(pstBase);
        }

        /* Enable transfer complete flag interrupt to catch the end of the transfer. */
        pstSpiState->txCount = 0;
        pstSpiState->rxCount = 0;
        SPI_SetIntMode(pstBase, SPI_TRANSFER_COMPLETE, true);
        pstSpiState->isTransferInProgress = false;
    }
}

/**
 * 
 * Function Name : SPI_DRV_MasterCompleteRX
 * Description   : Check if errors are detected on RX channel
 * The main purpose of this function is to check PDMA errors on rx channel
 */
static void SPI_DRV_MasterCompleteRX(void *parameter, pdma_chn_status_t euStatus)
{
    uint32_t instance        = (uint32_t)parameter;
    spi_state_t *pstSpiState = g_pstSpiState[instance];

    if (euStatus == PDMA_CHN_ERROR) {
        (void)SPI_DRV_MasterAbortTransfer(instance);
        pstSpiState->euStatus = SPI_TRANSMIT_FAIL;
    }
    if (pstSpiState->callback != NULL) {
        pstSpiState->callback(pstSpiState, SPI_EVENT_END_TRANSFER, pstSpiState->callbackParam);
    }
    (void)OS_SemaphoreRelease(pstSpiState->spiSemaphore);
}

/**
 * 
 * Function Name : SPI_DRV_MasterIRQHandler
 * Description   : Interrupt handler for SPI master mode.
 * This handler uses the buffers stored in the spi_state_t structs to transfer data.
 * This is not a public API as it is called whenever an interrupt occurs.
 */
void SPI_DRV_MasterIRQHandler(uint32_t instance)
{
    /* Instantiate local variable of type spi_master_state_t and point to global state */
    spi_state_t *pstSpiState = g_pstSpiState[instance];
    spi_type_t *pstBase      = g_spiBase[instance];

    /* If an error is detected the transfer will be aborted */
    if ((bool)SPI_GetStatusFlag(pstBase, SPI_TRANSMIT_ERROR) && (pstSpiState->txBuff != NULL)) {
        (void)SPI_DRV_MasterAbortTransfer(instance);
        (void)SPI_ClearStatusFlag(pstBase, SPI_TRANSMIT_ERROR);
        pstSpiState->euStatus = SPI_TRANSMIT_FAIL;
        return;
    }
    if (SPI_GetStatusFlag(pstBase, SPI_RECEIVE_ERROR) && (pstSpiState->rxBuff != NULL)) {
        (void)SPI_DRV_MasterAbortTransfer(instance);
        (void)SPI_ClearStatusFlag(pstBase, SPI_RECEIVE_ERROR);
        pstSpiState->euStatus = SPI_RECEIVE_FAIL;
        return;
    }

    /* RECEIVE IRQ handler: Check read buffer only if there are remaining bytes to read. */
    if (SPI_GetStatusFlag(pstBase, SPI_RX_DATA_FLAG)) {
        if (pstSpiState->rxCount != (uint16_t)0) {
            SPI_DRV_ReadRXBuffer(instance);
        }
    }
    /* Transmit data */
    if (SPI_GetStatusFlag(pstBase, SPI_TX_DATA_FLAG)) {
        if ((pstSpiState->txCount != (uint16_t)0)) {
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
    if (pstSpiState->txCount == (uint16_t)0) {
        /* Disable TX flag. Software buffer is empty.*/
        SPI_SetIntMode(pstBase, SPI_TX_DATA_FLAG, false);
        SPI_SetIntMode(pstBase, SPI_TRANSFER_COMPLETE, true);
        /* Check if we're done with this transfer.*/
        if (pstSpiState->rxCount == (uint16_t)0) {
            if (SPI_GetStatusFlag(pstBase, SPI_TRANSFER_COMPLETE) == true) {
                SPI_DRV_MasterCompleteTransfer(instance);
            }
        }
    }
}

