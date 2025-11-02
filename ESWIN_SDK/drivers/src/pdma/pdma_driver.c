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
 * @file pdma_driver.c
 * @brief PDMA driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-10
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "emps_platform.h"
#include "pdma_hw_access.h"
#include "pdma_register.h"
#include "clock_driver.h"

/**
 * @brief Array of base addresses for PDMA instances.
 */
static pdma_type_t *const g_pstpdmaBase[PDMA_INSTANCE_COUNT] = PDMA_BASE_PTRS;

/**
 * @brief Array of base addresses for DMAMUX instances.
 */
static dmamux_type_t *const g_dmaMuxBase[DMAMUX_INSTANCE_COUNT] = DMAMUX_BASE_PTRS;

/**
 * @brief Array of default PDMA channel interrupt handlers.
 */
static const IRQn_Type g_stpdmaIrqId[DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES] = PDMA_CHN_IRQS;

/**
 * @brief Array of default PDMA error interrupt handlers.
 */
static const IRQn_Type g_pdmaErrIrqId[PDMA_ERROR_INTERRUPT_NUMS] = PDMA_ERROR_IRQS;

/**
 * @brief PDMA global structure to maintain pDMA state
 */
static pdma_state_t *g_stVirtPdmaState;

/*
 *
 * Function Name : PDMA_ClearIntStatus.
 * Description   : Clear done and interrupt Status..
 * 
 */
static void PDMA_ClearIntStatus(uint8_t virtualChannel);

/*
 *
 * Function Name : PDMA_ClearSoftwareTCD.
 * Description   : The pointer to the pDMA TCD.
 * 
 */
static void PDMA_ClearSoftwareTCD(pdma_software_tcd_t *stcd);

/*
 *
 * Function Name : PDMA_ClearStructure.
 * Description   : Clears all bytes at the passed structure pointer..
 * 
 */
static void PDMA_ClearStructure(uint8_t *structPtr, size_t size);

/*
 *
 * Function Name : PDMA_DRV_ValidTransferSize.
 * Description   : Detects the byte size of each minor loop transfer by dma.
 * 
 */
static bool PDMA_DRV_ValidTransferSize(pdma_transfer_size_t size);

/*
 *
 * Function Name : PDMA_DRV_Init.
 * Description   : Initializes the PDMA module.
 * 
 */
status_t PDMA_DRV_Init(pdma_state_t *pdmaState, const pdma_user_config_t *userConfig,
                       pdma_chn_state_t *const chnStateArray[], const pdma_channel_config_t *const chnConfigArray[],
                       uint32_t chnCount)
{
    uint32_t index           = 0U;
    pdma_type_t *pdmaRegBase = NULL;
    IRQn_Type irqNumber      = NotAvail_IRQn;
    status_t pdmaStatus      = STATUS_SUCCESS;
    status_t chnInitStatus   = STATUS_SUCCESS;

    /* Check the state and configuration structure pointers are valid */
    OS_ASSERT((pdmaState != NULL) && (userConfig != NULL));

    /* Enable PDMA clock */
    CLOCK_SYS_ClockEnable(PDMA_CLK, true);

    /* Save the runtime state structure for the driver */
    g_stVirtPdmaState = pdmaState;

    /* Clear the state structure. */
    PDMA_ClearStructure((uint8_t *)g_stVirtPdmaState, sizeof(pdma_state_t));

    /* Init all PDMA instances */
    for (index = 0U; index < (uint32_t)PDMA_INSTANCE_COUNT; index++) {
        pdmaRegBase = g_pstpdmaBase[index];

        /* Init pDMA module on hardware level. */
        PDMA_Init(pdmaRegBase);

        /* Set arbitration mode */
        PDMA_SetChannelArbitrationMode(pdmaRegBase, userConfig->chnArbitration);
        PDMA_SetGroupArbitrationMode(pdmaRegBase, userConfig->groupArbitration);
        PDMA_SetGroupPriority(pdmaRegBase, userConfig->groupPriority);

        /* Set 'Halt on error' configuration */
        PDMA_SetHaltOnErrorCmd(pdmaRegBase, userConfig->haltOnError);
    }

    /* Enable the error interrupts for pDMA module. */
    for (index = 0U; index < (uint32_t)PDMA_INSTANCE_COUNT; index++) {
        /* Enable channel interrupt ID. */
        register_type_t type;
        type.trig_mode = CLIC_LEVEL_TRIGGER;
        type.lvl       = 1;
        type.priority  = 0;
        type.data_ptr  = (void *)index;
        irqNumber      = g_pdmaErrIrqId[index];
        OS_RequestIrq(irqNumber, PDMA_ErrorIRQHandler, &type);
        OS_EnableIrq(irqNumber);
    }

    /* Register all pdma channel interrupt handlers into vector table. */
    for (index = 0U; index < (uint32_t)DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES; index++) {
        /* Enable channel interrupt ID. */
        register_type_t type;
        type.trig_mode = CLIC_LEVEL_TRIGGER;
        type.lvl       = 1;
        type.priority  = 0;
        type.data_ptr  = (void *)index;

        irqNumber = g_stpdmaIrqId[index];
        OS_RequestIrq(irqNumber, PDMA_IRQHandler, &type);
        OS_EnableIrq(irqNumber);
    }

    /* Initialize all DMAMUX instances */
    for (index = 0U; index < (uint32_t)DMAMUX_INSTANCE_COUNT; index++) {
        DMAMUX_Reset(g_dmaMuxBase[index]);
    }

    /* Initialize the channels based on configuration list */
    if ((chnStateArray != NULL) && (chnConfigArray != NULL)) {
        for (index = 0U; index < chnCount; index++) {
            chnInitStatus = PDMA_DRV_ChannelInit(chnStateArray[index], chnConfigArray[index]);
            if (chnInitStatus != STATUS_SUCCESS) {
                pdmaStatus = chnInitStatus;
                break;
            }
        }
    }

    return pdmaStatus;
}

/*
 *
 * Function Name : PDMA_DRV_Deinit.
 * Description   : De-initializes the pDMA module.
 * 
 */
status_t PDMA_DRV_Deinit(void)
{
    uint32_t index                   = 0U;
    IRQn_Type irqNumber              = NotAvail_IRQn;
    const pdma_chn_state_t *chnState = NULL;

    /* Disable the error interrupts for pDMA module. */
    for (index = 0U; index < (uint32_t)PDMA_ERROR_INTERRUPT_NUMS; index++) {
        /* Enable channel interrupt ID. */
        irqNumber = g_pdmaErrIrqId[index];
        OS_DisableIrq(irqNumber);
    }

    if (g_stVirtPdmaState != NULL) {
        /* Release all pdma channel. */
        for (index = 0U; index < (uint32_t)DMA_VIRTUAL_CHANNELS; index++) {
            /* Release all channels. */
            chnState = g_stVirtPdmaState->virtChnState[index];
            if (chnState != NULL) {
                (void)PDMA_DRV_ReleaseChannel(chnState->virtChn);
            }
        }
        for (index = 0U; index < (uint32_t)DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES; index++) {
            /* Disable channel interrupts. */
            irqNumber = g_stpdmaIrqId[index];
            OS_DisableIrq(irqNumber);
        }
    }

    /* Disable PDMA clock */
    CLOCK_SYS_ClockEnable(PDMA_CLK, false);

    g_stVirtPdmaState = NULL;

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PDMA_DRV_ChannelInit.
 * Description   :  Initializes an PDMA channel.
 * 
 */
status_t PDMA_DRV_ChannelInit(pdma_chn_state_t *pdmaChannelState, const pdma_channel_config_t *pdmaChannelConfig)
{
    /* Check the state and configuration structure pointers are valid */
    OS_ASSERT((pdmaChannelState != NULL) && (pdmaChannelConfig != NULL));

    /* Check if the module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check the channel has not already been allocated */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[pdmaChannelConfig->virtChnConfig] == NULL);

    /* Check if the channel defined by user in the channel configuration structure is valid */
    OS_ASSERT(pdmaChannelConfig->virtChnConfig < DMA_VIRTUAL_CHANNELS);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(pdmaChannelConfig->virtChnConfig);

    /* Get PDMA channel from virtual channel */
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(pdmaChannelConfig->virtChnConfig);

    /* Get virtual channel value */
    uint8_t virtualChannel = pdmaChannelConfig->virtChnConfig;

    /* Get status */
    status_t retStatus = STATUS_SUCCESS;

    /* Load corresponding PDMA instance pointer */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];

    /* Reset the channel state structure to default value. */
    PDMA_ClearStructure((uint8_t *)pdmaChannelState, sizeof(pdma_chn_state_t));

    retStatus = PDMA_DRV_SetChannelRequestAndTrigger(
        pdmaChannelConfig->virtChnConfig, (uint8_t)pdmaChannelConfig->source, pdmaChannelConfig->enableTrigger);

    /* Clear the TCD registers for this channel */
    PDMA_TCDClearReg(pdmaRegBase, dmaChannel);

    if (retStatus == STATUS_SUCCESS) {
        /* Set virtual channel state */
        g_stVirtPdmaState->virtChnState[virtualChannel] = pdmaChannelState;
        /* Set virtual channel value */
        g_stVirtPdmaState->virtChnState[virtualChannel]->virtChn = virtualChannel;
        /* Set virtual channel status to normal */
        g_stVirtPdmaState->virtChnState[virtualChannel]->status = PDMA_CHN_NORMAL;

        /* Enable error interrupt for this channel */
        PDMA_SetErrorIntCmd(pdmaRegBase, dmaChannel, true);

        /* Set the channel priority, as defined in the configuration, only if fixed arbitration mode is selected */
        if ((PDMA_GetChannelArbitrationMode(pdmaRegBase) == PDMA_ARBITRATION_FIXED_PRIORITY) &&
            (pdmaChannelConfig->channelPriority != PDMA_CHN_DEFAULT_PRIORITY)) {
            PDMA_SetChannelPriority(pdmaRegBase, dmaChannel, pdmaChannelConfig->channelPriority);
        }
        /* Install the user callback */
        retStatus = PDMA_DRV_InstallCallback(pdmaChannelConfig->virtChnConfig, pdmaChannelConfig->callback,
                                             pdmaChannelConfig->callbackParam);
    }

    return retStatus;
}

/*
 *
 * Function Name : PDMA_DRV_InstallCallback.
 * Description   : Registers the callback function and the parameter for pDMA channel.
 * 
 */
status_t PDMA_DRV_InstallCallback(uint8_t virtualChannel, pdma_callback_t callback, void *parameter)
{
    /* Check the channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check the channel is allocated */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    g_stVirtPdmaState->virtChnState[virtualChannel]->callback  = callback;
    g_stVirtPdmaState->virtChnState[virtualChannel]->parameter = parameter;

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PDMA_DRV_ReleaseChannel.
 * Description   : Releases an pDMA channel,Free pDMA channel's hardware
 *                  and software resource.
 * 
 */
status_t PDMA_DRV_ReleaseChannel(uint8_t virtualChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check the PDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Get pointer to channel state */
    pdma_chn_state_t *chnState = g_stVirtPdmaState->virtChnState[virtualChannel];

    /* Check that virtual channel is initialized */
    OS_ASSERT(chnState != NULL);

    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];

    /* Stop pdma channel. */
    PDMA_SetDmaRequestCmd(pdmaRegBase, dmaChannel, false);

    /* Reset the channel state structure to default value. */
    PDMA_ClearStructure((uint8_t *)chnState, sizeof(pdma_chn_state_t));

    g_stVirtPdmaState->virtChnState[virtualChannel] = NULL;

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PDMA_ClearIntStatus.
 * Description   : Clear done and interrupt retStatus.
 * 
 */
static void PDMA_ClearIntStatus(uint8_t virtualChannel)
{
    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_ClearDoneStatusFlag(pdmaRegBase, dmaChannel);
    PDMA_ClearIntStatusFlag(pdmaRegBase, dmaChannel);
}

/*
 *
 * Function Name : PDMA_ClearSoftwareTCD.
 * Description   : Clear the software tcd structure.
 * 
 */
static void PDMA_ClearSoftwareTCD(pdma_software_tcd_t *stcd)
{
    PDMA_ClearStructure((uint8_t *)stcd, sizeof(pdma_software_tcd_t));
}

/*
 *
 * Function Name : PDMA_DRV_ErrorIRQHandler.
 * Description   : PDMA virtualChannel error interrupt handler.
 * 
 */
void PDMA_DRV_ErrorIRQHandler(uint8_t virtualChannel)
{
    uint32_t idx = 0;
    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_SetDmaRequestCmd(pdmaRegBase, dmaChannel, false);
    pdma_chn_state_t *chnState = g_stVirtPdmaState->virtChnState[virtualChannel];
    if ((chnState->destAddr >= ENVRAM_DCACHE_BASEADDR) && (chnState->destAddr < OCM_DCACHE_BASEADDR + OCM_DCACHE_SIZE)) {
        for (idx = 0; idx < (chnState->transferSize + CACHE_LINE) / CACHE_LINE; idx++) {
            // OS_FlushCache((void *)(chnState->destAddr + CACHE_LINE * idx));
            OS_InvalidCache((void *)(chnState->destAddr + CACHE_LINE * idx));
        }
    } else {
    }
    if (chnState != NULL) {

        chnState->status = PDMA_CHN_ERROR;
        if (chnState->callback != NULL) {
            chnState->callback(chnState->parameter, chnState->status);
        }
        PDMA_ClearIntStatus(virtualChannel);
        PDMA_ClearErrorIntStatusFlag(pdmaRegBase, dmaChannel);
    }
}

/*
 *
 * Function Name : PDMA_DRV_ConfigSingleBlockTransfer.
 * Description   : Configures a simple single block data transfer with PDMA.
 * 
 */
status_t PDMA_DRV_ConfigSingleBlockTransfer(uint8_t virtualChannel, pdma_transfer_type_t type, uint32_t srcAddr,
                                            uint32_t destAddr, pdma_transfer_size_t transferSize,
                                            uint32_t dataBufferSize)
{
    uint32_t idx = 0;
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Check if the value passed for 'transferSize' is valid */
    OS_ASSERT(PDMA_DRV_ValidTransferSize(transferSize));

    /* flush cache data */
    if ((srcAddr >= PFLASH_ICACHE_BASEADDR) && (srcAddr < OCM_DCACHE_BASEADDR + OCM_DCACHE_SIZE)) {
        for (idx = 0; idx < (dataBufferSize + CACHE_LINE) / CACHE_LINE; idx++) {
            OS_FlushCache((void *)(srcAddr + CACHE_LINE * idx));
        }
    }

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    status_t retStatus       = STATUS_SUCCESS;

    /* Compute the transfer offset, based on transfer size.
     * The number of bytes transferred in each source read/destination write
     * is obtained with the following formula:
     *    source_read_size = 2^SSIZE
     *    destination_write_size = 2^DSIZE
     */
    uint8_t transferOffset = (uint8_t)(1U << ((uint8_t)transferSize));

    /* Check that source address and destination address are valid */
    OS_ASSERT((srcAddr % transferOffset) == 0U);
    OS_ASSERT((destAddr % transferOffset) == 0U);

    /* The number of bytes to be transferred (buffer size) must
     * be a multiple of the source read/destination write size
     */
    if ((dataBufferSize % transferOffset) != 0U) {
        retStatus = STATUS_ERROR;
    }

    if (retStatus == STATUS_SUCCESS) {
        /* Clear transfer control descriptor for the current channel */
        PDMA_TCDClearReg(pdmaRegBase, dmaChannel);

        /* Configure the PDMA Engine to stall for a number of cycles after each R/W */
        PDMA_TCDSetEngineStall(pdmaRegBase, dmaChannel, PDMA_ENGINE_STALL_0_CYCLES);

        PDMA_SetMinorLoopMappingCmd(pdmaRegBase, true);

        /* Configure source and destination addresses */
        PDMA_TCDSetSrcAddr(pdmaRegBase, dmaChannel, srcAddr);
        PDMA_TCDSetDestAddr(pdmaRegBase, dmaChannel, destAddr);

        /* Set transfer size (1B/2B/4B/16B/32B) */
        PDMA_TCDSetAttribute(pdmaRegBase, dmaChannel, PDMA_MODULO_OFF, PDMA_MODULO_OFF, transferSize, transferSize);

        /* Configure source/destination offset. */
        switch (type) {
        case PDMA_TRANSFER_PERIPH2MEM:
            PDMA_TCDSetSrcOffset(pdmaRegBase, dmaChannel, 0);
            PDMA_TCDSetDestOffset(pdmaRegBase, dmaChannel, (int8_t)transferOffset);
            break;
        case PDMA_TRANSFER_MEM2PERIPH:
            PDMA_TCDSetSrcOffset(pdmaRegBase, dmaChannel, (int8_t)transferOffset);
            PDMA_TCDSetDestOffset(pdmaRegBase, dmaChannel, 0);
            break;
        case PDMA_TRANSFER_MEM2MEM:
            PDMA_TCDSetSrcOffset(pdmaRegBase, dmaChannel, (int8_t)transferOffset);
            PDMA_TCDSetDestOffset(pdmaRegBase, dmaChannel, (int8_t)transferOffset);
            break;
        case PDMA_TRANSFER_PERIPH2PERIPH:
            PDMA_TCDSetSrcOffset(pdmaRegBase, dmaChannel, 0);
            PDMA_TCDSetDestOffset(pdmaRegBase, dmaChannel, 0);
            break;
        default:
            /* This should never be reached - all the possible values have been handled. */
            break;
        }

        /* Set the total number of bytes to be transferred */
        PDMA_TCDSetNbytes(pdmaRegBase, dmaChannel, dataBufferSize);

        /* Set major iteration count to 1 (single block mode) */
        PDMA_TCDSetMajorCount(pdmaRegBase, dmaChannel, 1U);

        /* Enable interrupt when the transfer completes */
        PDMA_TCDSetMajorCompleteIntCmd(pdmaRegBase, dmaChannel, true);

        /* Set virtual channel status to normal */
        g_stVirtPdmaState->virtChnState[virtualChannel]->status       = PDMA_CHN_NORMAL;
        g_stVirtPdmaState->virtChnState[virtualChannel]->transferSize = dataBufferSize;
        g_stVirtPdmaState->virtChnState[virtualChannel]->destAddr     = destAddr;
    }

    return retStatus;
}

/*
 *
 * Function Name : PDMA_DRV_ConfigMultiBlockTransfer.
 * Description   : Configures a multiple block data transfer with PDMA.
 * 
 */
status_t PDMA_DRV_ConfigMultiBlockTransfer(uint8_t virtualChannel, pdma_transfer_type_t type, uint32_t srcAddr,
                                           uint32_t destAddr, pdma_transfer_size_t transferSize, uint32_t blockSize,
                                           uint32_t blockCount, bool disableReqOnCompletion)
{
    uint32_t tansferLen = 0, idx = 0;

    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    status_t retStatus = STATUS_SUCCESS;
    /* Compute the transfer offset, based on transfer size.
     * The number of bytes transferred in each source read/destination write
     * is obtained with the following formula:
     *    source_read_size = 2^SSIZE
     *    destination_write_size = 2^DSIZE
     */
    uint8_t transferOffset = (uint8_t)(1U << ((uint8_t)transferSize));

    /* Check that source address and destination address are valid */
    OS_ASSERT((srcAddr % transferOffset) == 0U);
    OS_ASSERT((destAddr % transferOffset) == 0U);

    /* Transfer OCM/ENVRAM source periph addr to cache addr */
    if ((srcAddr >= OCM_PERIPH_BASEADDR) && (srcAddr < OCM_PERIPH_BASEADDR + OCM_PERIPH_SIZE)) {
        srcAddr = srcAddr - (OCM_PERIPH_BASEADDR - OCM_DCACHE_BASEADDR);
    } else if ((srcAddr >= ENVRAM_PERIPH_BASEADDR) && (srcAddr < ENVRAM_PERIPH_BASEADDR + ENVRAM_PERIPH_SIZE)) {
        srcAddr = srcAddr - (ENVRAM_PERIPH_BASEADDR - ENVRAM_DCACHE_BASEADDR);
    } else {
    }

    /* Transfer OCM/ENVRAM dest periph addr to cache addr */
    if ((destAddr >= OCM_PERIPH_BASEADDR) && (destAddr < OCM_PERIPH_BASEADDR + OCM_PERIPH_SIZE)) {
        destAddr = destAddr - (OCM_PERIPH_BASEADDR - OCM_DCACHE_BASEADDR);
    } else if ((destAddr >= ENVRAM_PERIPH_BASEADDR) && (destAddr < ENVRAM_PERIPH_BASEADDR + ENVRAM_PERIPH_SIZE)) {
        destAddr = destAddr - (ENVRAM_PERIPH_BASEADDR - ENVRAM_DCACHE_BASEADDR);
    } else {
    }

    /* if dest data in cache area ,flush cache*/
    tansferLen = blockSize * blockCount;

    if ((srcAddr >= PFLASH_ICACHE_BASEADDR) && (srcAddr < OCM_DCACHE_BASEADDR + OCM_DCACHE_SIZE)) {
        for (idx = 0; idx < (tansferLen + CACHE_LINE) / CACHE_LINE; idx++) {
            OS_FlushCache((void *)(srcAddr + CACHE_LINE * idx));
        }
    }

    /* Configure the transfer for one data block */
    retStatus = PDMA_DRV_ConfigSingleBlockTransfer(virtualChannel, type, srcAddr, destAddr, transferSize, blockSize);

    g_stVirtPdmaState->virtChnState[virtualChannel]->transferSize = tansferLen;
    g_stVirtPdmaState->virtChnState[virtualChannel]->destAddr     = destAddr;

    if (retStatus == STATUS_SUCCESS) {
        pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];

        /* Set the number of data blocks */
        PDMA_TCDSetMajorCount(pdmaRegBase, dmaChannel, blockCount);

        /* Enable/disable requests upon completion */
        PDMA_TCDSetDisableDmaRequestAfterTCDDoneCmd(pdmaRegBase, dmaChannel, disableReqOnCompletion);
    }

    return retStatus;
}

/*
 *
 * Function Name : PDMA_DRV_ConfigLoopTransfer.
 * Description   : Configures the PDMA transfer in loop mode.
 * 
 */
status_t PDMA_DRV_ConfigLoopTransfer(uint8_t virtualChannel, pdma_transfer_config_t *transferConfig)
{
    uint32_t idx        = 0;
    uint32_t tansferLen = 0;
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Check the transfer configuration structure is valid */
    OS_ASSERT(transferConfig != NULL);

    /* Transfer OCM/ENVRAM source periph addr to cache addr */
    if ((transferConfig->srcAddr >= OCM_PERIPH_BASEADDR) && (transferConfig->srcAddr < OCM_PERIPH_BASEADDR + OCM_PERIPH_SIZE)) {
        transferConfig->srcAddr = transferConfig->srcAddr - (OCM_PERIPH_BASEADDR - OCM_DCACHE_BASEADDR);
    } else if ((transferConfig->srcAddr >= ENVRAM_PERIPH_BASEADDR) && (transferConfig->srcAddr < ENVRAM_PERIPH_BASEADDR + ENVRAM_PERIPH_SIZE)) {
        transferConfig->srcAddr = transferConfig->srcAddr - (ENVRAM_PERIPH_BASEADDR - ENVRAM_DCACHE_BASEADDR);
    } else {
    }
    /* Transfer OCM/ENVRAM dest periph addr to cache addr */
    if ((transferConfig->destAddr >= OCM_PERIPH_BASEADDR) && (transferConfig->destAddr < OCM_PERIPH_BASEADDR + OCM_PERIPH_SIZE)) {
        transferConfig->destAddr = transferConfig->destAddr - (OCM_PERIPH_BASEADDR - OCM_DCACHE_BASEADDR);
    } else if ((transferConfig->destAddr >= ENVRAM_PERIPH_BASEADDR) && (transferConfig->destAddr < ENVRAM_PERIPH_BASEADDR + ENVRAM_PERIPH_SIZE)) {
        transferConfig->destAddr = transferConfig->destAddr - (ENVRAM_PERIPH_BASEADDR - ENVRAM_DCACHE_BASEADDR);
    } else {
    }

    /* if source data in cache area ,flush chache*/
    tansferLen = transferConfig->minorByteTransferCount * transferConfig->loopTransferConfig->majorLoopIterationCount;

    if ((transferConfig->srcAddr >= PFLASH_ICACHE_BASEADDR) && (transferConfig->srcAddr < OCM_DCACHE_BASEADDR + OCM_DCACHE_SIZE)) {
        for (idx = 0; idx < (tansferLen + CACHE_LINE) / CACHE_LINE; idx++) {
            OS_FlushCache((void *)(transferConfig->srcAddr + CACHE_LINE * idx));
        }
    } else {
    }

    /* Compute the transfer offset, based on transfer size.
     * The number of bytes transferred in each source read/destination write
     * is obtained with the following formula:
     *    source_read_size = 2^SSIZE
     *    destination_write_size = 2^DSIZE
     */
    uint8_t srcTransferOffset  = (uint8_t)(1U << ((uint8_t)transferConfig->srcTransferSize));
    uint8_t destTransferOffset = (uint8_t)(1U << ((uint8_t)transferConfig->destTransferSize));

    /* Check that source address and destination address are valid */
    OS_ASSERT((transferConfig->srcAddr % srcTransferOffset) == 0U);
    OS_ASSERT((transferConfig->destAddr % destTransferOffset) == 0U);
    /* Check the minor/major loop properties are defined */
    OS_ASSERT(transferConfig->loopTransferConfig != NULL);

    /* If the modulo feature is enabled, check alignment of addresses */
    OS_ASSERT((transferConfig->srcModulo == PDMA_MODULO_OFF) ||
              ((transferConfig->srcAddr % (((uint32_t)1U) << (uint32_t)transferConfig->srcModulo)) == 0U));
    OS_ASSERT((transferConfig->destModulo == PDMA_MODULO_OFF) ||
              ((transferConfig->destAddr % (((uint32_t)1U) << (uint32_t)transferConfig->destModulo)) == 0U));

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];

    PDMA_SetMinorLoopMappingCmd(pdmaRegBase, true);
    /* Write the configuration in the transfer control descriptor registers */
    PDMA_DRV_PushConfigToReg(virtualChannel, transferConfig);

    /* Set virtual channel status to normal */
    g_stVirtPdmaState->virtChnState[virtualChannel]->status       = PDMA_CHN_NORMAL;
    g_stVirtPdmaState->virtChnState[virtualChannel]->destAddr     = transferConfig->destAddr;
    g_stVirtPdmaState->virtChnState[virtualChannel]->transferSize = tansferLen;
    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PDMA_DRV_StartChannel.
 * Description   : Starts an pDMA channel.
 * 
 */
status_t PDMA_DRV_StartChannel(uint8_t virtualChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Enable requests for current channel */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_SetDmaRequestCmd(pdmaRegBase, dmaChannel, true);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PDMA_DRV_StopChannel.
 * Description   : Stops the pDMA channel.
 * 
 */
status_t PDMA_DRV_StopChannel(uint8_t virtualChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Disable requests for current channel */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_SetDmaRequestCmd(pdmaRegBase, dmaChannel, false);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PDMA_DRV_SetChannelRequestAndTrigger.
 * Description   : Configures the PDMA request for the pDMA channel.
 * 
 */
status_t PDMA_DRV_SetChannelRequestAndTrigger(uint8_t virtualChannel, uint8_t request, bool enableTrigger)
{
    /* Check the virtual channel number is valid */
    OS_ASSERT(virtualChannel < (uint32_t)DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Retrieve the DMAMUX instance serving this request */
    uint8_t dmaMuxInstance = (uint8_t)DMAMUX_REQ_SRC_TO_INSTANCE(request);

    /* Get request index for the corresponding DMAMUX instance */
    uint8_t dmaMuxRequest = (uint8_t)DMAMUX_REQ_SRC_TO_CH(request);

    /* Get DMAMUX channel for the selected request */
    uint8_t dmaMuxChannel = (uint8_t)DMAMUX_DMA_CH_TO_CH(virtualChannel);

    /* Retrieve the appropriate DMAMUX instance */
    dmamux_type_t *dmaMuxRegBase = g_dmaMuxBase[dmaMuxInstance];

    /* Set request and trigger */
    DMAMUX_SetChannelCmd(dmaMuxRegBase, dmaMuxChannel, false);
    DMAMUX_SetChannelSource(dmaMuxRegBase, dmaMuxChannel, dmaMuxRequest);
    DMAMUX_SetChannelTrigger(dmaMuxRegBase, dmaMuxChannel, enableTrigger);

    DMAMUX_SetChannelCmd(dmaMuxRegBase, dmaMuxChannel, true);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PDMA_DRV_ClearTCD.
 * Description   : Clears all registers to 0 for the channel's TCD.
 * 
 */
void PDMA_DRV_ClearTCD(uint8_t virtualChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Clear the TCD memory */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDClearReg(pdmaRegBase, dmaChannel);
}

/*
 *
 * Function Name : PDMA_DRV_SetSrcAddr.
 * Description   : Configures the source address for the pDMA channel.
 * 
 */
void PDMA_DRV_SetSrcAddr(uint8_t virtualChannel, uint32_t address)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set channel TCD source address */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetSrcAddr(pdmaRegBase, dmaChannel, address);
}

/*
 *
 * Function Name : PDMA_DRV_SetSrcOffset.
 * Description   : Configures the source address signed offset for the pDMA channel.
 * 
 */
void PDMA_DRV_SetSrcOffset(uint8_t virtualChannel, int16_t offset)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set channel TCD source offset */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetSrcOffset(pdmaRegBase, dmaChannel, offset);
}

/*
 *
 * Function Name : PDMA_DRV_SetSrcReadChunkSize.
 * Description   :  Configures the source data chunk size (transferred in a read sequence).
 * 
 */
void PDMA_DRV_SetSrcReadChunkSize(uint8_t virtualChannel, pdma_transfer_size_t size)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set channel TCD source transfer size */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetSrcTransferSize(pdmaRegBase, dmaChannel, size);
}

/*
 *
 * Function Name : PDMA_DRV_SetSrcLastAddrAdjustment.
 * Description   : Configures the source address last adjustment.
 * 
 */
void PDMA_DRV_SetSrcLastAddrAdjustment(uint8_t virtualChannel, int32_t adjust)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set channel TCD source last adjustment */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetSrcLastAdjust(pdmaRegBase, dmaChannel, adjust);
}

/*
 *
 * Function Name : PDMA_DRV_SetDestLastAddrAdjustment.
 * Description   : Configures the destination address last adjustment.
 * 
 */
void PDMA_DRV_SetDestLastAddrAdjustment(uint8_t virtualChannel, int32_t adjust)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set channel TCD source last adjustment */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetDestLastAdjust(pdmaRegBase, dmaChannel, adjust);
}

/*
 *
 * Function Name : PDMA_DRV_SetDestAddr.
 * Description   : Configures the destination address for the pDMA channel.
 * 
 */
void PDMA_DRV_SetDestAddr(uint8_t virtualChannel, uint32_t address)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set channel TCD destination address */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetDestAddr(pdmaRegBase, dmaChannel, address);
}

/*
 *
 * Function Name : PDMA_DRV_SetDestOffset.
 * Description   : Configures the destination address signed offset for the pDMA channel.
 * 
 */
void PDMA_DRV_SetDestOffset(uint8_t virtualChannel, int16_t offset)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set channel TCD destination offset */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetDestOffset(pdmaRegBase, dmaChannel, offset);
}

/*
 *
 * Function Name : PDMA_DRV_SetDestWriteChunkSize.
 * Description   : Configures the destination data chunk size (transferred in a write sequence).
 * 
 */
void PDMA_DRV_SetDestWriteChunkSize(uint8_t virtualChannel, pdma_transfer_size_t size)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set channel TCD source transfer size */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetDestTransferSize(pdmaRegBase, dmaChannel, size);
}

/*
 *
 * Function Name : PDMA_DRV_SetMinorLoopBlockSize.
 * Description   : Configures the number of bytes to be transferred in each service request of the channel.
 * 
 */
void PDMA_DRV_SetMinorLoopBlockSize(uint8_t virtualChannel, uint32_t nbytes)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set channel TCD minor loop block size */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetNbytes(pdmaRegBase, dmaChannel, nbytes);
}

/*
 *
 * Function Name : PDMA_DRV_SetMajorLoopIterationCount.
 * Description   : Configures the number of major loop iterations.
 * 
 */
void PDMA_DRV_SetMajorLoopIterationCount(uint8_t virtualChannel, uint32_t majorLoopCount)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Set the major loop iteration count */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetMajorCount(pdmaRegBase, dmaChannel, majorLoopCount);
}

/*
 *
 * Function Name : PDMA_DRV_GetRemainingMajorIterationsCount.
 * Description   : Returns the remaining major loop iteration count.
 * 
 */
uint32_t PDMA_DRV_GetRemainingMajorIterationsCount(uint8_t virtualChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Retrieve the number of minor loops yet to be triggered */
    const pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];

    uint32_t count = PDMA_TCDGetCurrentMajorCount(pdmaRegBase, dmaChannel);

    return count;
}

/*
 *
 * Function Name : PDMA_DRV_DisableRequestsOnTransferComplete.
 * Description   : Disables/Enables the PDMA request after the major loop completes for the TCD.
 * 
 */
void PDMA_DRV_DisableRequestsOnTransferComplete(uint8_t virtualChannel, bool disable)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Disables/Enables the PDMA request upon TCD completion */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TCDSetDisableDmaRequestAfterTCDDoneCmd(pdmaRegBase, dmaChannel, disable);
}

/*
 *
 * Function Name : PDMA_DRV_ConfigureInterrupt.
 * Description   : Disables/Enables the channel interrupt requests.
 * 
 */
void PDMA_DRV_ConfigureInterrupt(uint8_t virtualChannel, pdma_channel_interrupt_t intSrc, bool enable)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Disables/Enables the channel interrupt requests. */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    switch (intSrc) {
    case PDMA_CHN_ERR_INT:
        /* Enable channel interrupt request when error conditions occur */
        PDMA_SetErrorIntCmd(pdmaRegBase, dmaChannel, enable);
        break;
    case PDMA_CHN_HALF_MAJOR_LOOP_INT:
        /* Enable channel interrupt request when major iteration count reaches halfway point */
        PDMA_TCDSetMajorHalfCompleteIntCmd(pdmaRegBase, dmaChannel, enable);
        break;
    case PDMA_CHN_MAJOR_LOOP_INT:
        /* Enable channel interrupt request when major iteration count reaches zero */
        PDMA_TCDSetMajorCompleteIntCmd(pdmaRegBase, dmaChannel, enable);
        break;
    default:
        /* This branch should never be reached if driver API is used properly */
        break;
    }
}

/*
 *
 * Function Name : PDMA_DRV_CancelTransfer.
 * Description   : Cancel the running transfer.
 * 
 */
void PDMA_DRV_CancelTransfer(bool error)
{
    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    uint32_t dmaInstance = 0U;

    for (dmaInstance = 0U; dmaInstance < (uint32_t)PDMA_INSTANCE_COUNT; dmaInstance++) {
        /* Cancel the running transfer. */
        pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
        if (error) {
            PDMA_CancelTransferWithError(pdmaRegBase);
        } else {
            PDMA_CancelTransfer(pdmaRegBase);
        }
    }
}

/*
 *
 * Function Name : PDMA_DRV_TriggerSwRequest.
 * Description   : Triggers a sw request for the current channel.
 * 
 */
void PDMA_DRV_TriggerSwRequest(uint8_t virtualChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Trigger the channel transfer. */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    PDMA_TriggerChannelStart(pdmaRegBase, dmaChannel);
}
#if 0
/*for test */
void PDMA_DRV_LoopDone(uint8_t virtualChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    /* Trigger the channel transfer. */
    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];
    volatile uint32_t regValTemp;
    do {
        OS_DelayUs(100);
        regValTemp = pdmaRegBase->TCD[dmaChannel].CSR;
    }while(!(regValTemp & DMA_TCD_CSR_DONE_MASK));

    PDMA_IRQHandler((void *)((size_t)virtualChannel));
}
#endif

/*
 *
 * Function Name : PDMA_DRV_PushConfigToSTCD.
 * Description   : Copies the channel configuration to the software TCD structure.
 * 
 */
void PDMA_DRV_PushConfigToSTCD(const pdma_transfer_config_t *config, pdma_software_tcd_t *stcd)
{
    if ((config != NULL) && (stcd != NULL)) {
        /* Clear the array of software TCDs passed by the user */
        PDMA_ClearSoftwareTCD(stcd);

        /* Set the software TCD fields */
        stcd->ATTR   = (uint16_t)(DMA_TCD_ATTR_SMOD(config->srcModulo) | DMA_TCD_ATTR_SSIZE(config->srcTransferSize) |
                                DMA_TCD_ATTR_DMOD(config->destModulo) | DMA_TCD_ATTR_DSIZE(config->destTransferSize));
        stcd->SADDR  = config->srcAddr;
        stcd->SOFF   = config->srcOffset;
        stcd->NBYTES = config->minorByteTransferCount;
        stcd->SLAST  = config->srcLastAddrAdjust;
        stcd->DADDR  = config->destAddr;
        stcd->DOFF   = config->destOffset;
        stcd->CITER  = (uint16_t)config->loopTransferConfig->majorLoopIterationCount;

        stcd->DLAST_SGA = config->destLastAddrAdjust;

        stcd->DLAST_SGA = config->destLastAddrAdjust;
        stcd->CSR       = (uint16_t)((config->interruptEnable ? 1UL : 0UL) << DMA_TCD_CSR_INTMAJOR_SHIFT);
        stcd->BITER     = (uint16_t)config->loopTransferConfig->majorLoopIterationCount;
    }
}

/*
 *
 * Function Name : PDMA_DRV_PushConfigToReg.
 * Description   : Copies the channel configuration to the TCD registers.
 * 
 */
void PDMA_DRV_PushConfigToReg(uint8_t virtualChannel, const pdma_transfer_config_t *tcd)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    /* Check the transfer configuration structure is valid */
    OS_ASSERT(tcd != NULL);

    /* Get PDMA instance from virtual channel */
    uint8_t dmaInstance = (uint8_t)DMA_VCH_TO_INSTANCE(virtualChannel);

    /* Get PDMA channel from virtual channel*/
    uint8_t dmaChannel = (uint8_t)DMA_VCH_TO_CH(virtualChannel);

    pdma_type_t *pdmaRegBase = g_pstpdmaBase[dmaInstance];

    /* Clear TCD registers */
    PDMA_TCDClearReg(pdmaRegBase, dmaChannel);

    /* Configure the PDMA Engine to stall for a number of cycles after each R/W */
    PDMA_TCDSetEngineStall(pdmaRegBase, dmaChannel, PDMA_ENGINE_STALL_0_CYCLES);

    /* Set source and destination addresses */
    PDMA_TCDSetSrcAddr(pdmaRegBase, dmaChannel, tcd->srcAddr);
    PDMA_TCDSetDestAddr(pdmaRegBase, dmaChannel, tcd->destAddr);
    /* Set source/destination modulo feature and transfer size */
    PDMA_TCDSetAttribute(pdmaRegBase, dmaChannel, tcd->srcModulo, tcd->destModulo, tcd->srcTransferSize,
                         tcd->destTransferSize);
    /* Set source/destination offset and last adjustment; for scatter/gather operation, destination
     * last adjustment is the address of the next TCD structure to be loaded by the pDMA engine */
    PDMA_TCDSetSrcOffset(pdmaRegBase, dmaChannel, tcd->srcOffset);
    PDMA_TCDSetDestOffset(pdmaRegBase, dmaChannel, tcd->destOffset);
    PDMA_TCDSetSrcLastAdjust(pdmaRegBase, dmaChannel, tcd->srcLastAddrAdjust);

    PDMA_TCDSetDestLastAdjust(pdmaRegBase, dmaChannel, tcd->destLastAddrAdjust);

    /* Configure channel interrupt */
    PDMA_TCDSetMajorCompleteIntCmd(pdmaRegBase, dmaChannel, tcd->interruptEnable);

    /* If loop configuration is available, copy minor/major loop setup to registers */
    if (tcd->loopTransferConfig != NULL) {
        PDMA_TCDSetSrcMinorLoopOffsetCmd(pdmaRegBase, dmaChannel, tcd->loopTransferConfig->srcOffsetEnable);
        PDMA_TCDSetDestMinorLoopOffsetCmd(pdmaRegBase, dmaChannel, tcd->loopTransferConfig->dstOffsetEnable);
        PDMA_TCDSetMinorLoopOffset(pdmaRegBase, dmaChannel, tcd->loopTransferConfig->minorLoopOffset);
        PDMA_TCDSetNbytes(pdmaRegBase, dmaChannel, tcd->minorByteTransferCount);

        PDMA_TCDSetChannelMinorLink(pdmaRegBase, dmaChannel, tcd->loopTransferConfig->minorLoopChnLinkNumber,
                                    tcd->loopTransferConfig->minorLoopChnLinkEnable);
        PDMA_TCDSetChannelMajorLink(pdmaRegBase, dmaChannel, tcd->loopTransferConfig->majorLoopChnLinkNumber,
                                    tcd->loopTransferConfig->majorLoopChnLinkEnable);

        PDMA_TCDSetMajorCount(pdmaRegBase, dmaChannel, tcd->loopTransferConfig->majorLoopIterationCount);
    } else {
        PDMA_TCDSetNbytes(pdmaRegBase, dmaChannel, tcd->minorByteTransferCount);
    }
}

/*
 *
 * Function Name : PDMA_DRV_ValidTransferSize.
 * Description   : Check if the transfer size value is legal (0/1/2/4/5).
 * 
 */
static bool PDMA_DRV_ValidTransferSize(pdma_transfer_size_t size)
{
    bool isValid;

    switch (size) {
    case PDMA_TRANSFER_SIZE_1B:
    case PDMA_TRANSFER_SIZE_2B:
    case PDMA_TRANSFER_SIZE_4B:
        isValid = true;
        break;
    default:
        isValid = false;
        break;
    }
    return isValid;
}

/*
 *
 * Function Name : PDMA_DRV_GetChannelStatus.
 * Description   : Gets the pDMA channel status.
 * 
 */
pdma_chn_status_t PDMA_DRV_GetChannelStatus(uint8_t virtualChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(virtualChannel < DMA_VIRTUAL_CHANNELS);

    /* Check that pDMA module is initialized */
    OS_ASSERT(g_stVirtPdmaState != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(g_stVirtPdmaState->virtChnState[virtualChannel] != NULL);

    return g_stVirtPdmaState->virtChnState[virtualChannel]->status;
}

/*
 *
 * Function Name : PDMA_GetDmaRegBaseAddr.
 * Description   :Returns PDMA Register Base Address. Gets the address of the selected PDMA module.
 * 
 */
pdma_type_t *PDMA_GetDmaRegBaseAddr(uint32_t instance)
{
    /* Check that instance is valid */
    OS_ASSERT(instance < PDMA_INSTANCE_COUNT);

    return g_pstpdmaBase[instance];
}

/*
 *
 * Function Name : PDMA_ClearStructure.
 * Description   : Clears all bytes at the passed structure pointer.
 * 
 */
static void PDMA_ClearStructure(uint8_t *structPtr, size_t size)
{
    while (size > 0U) {
        *structPtr = 0;
        structPtr++;
        size--;
    }
}

/*
 *
 * Function Name : PDMA_IRQHandler.
 * Description   : Interrupt registration function.
 * 
 */
void PDMA_IRQHandler(void *data)
{
    size_t virtualChannel = (size_t)data;
    uint32_t idx          = 0;

    const pdma_chn_state_t *chnState = g_stVirtPdmaState->virtChnState[virtualChannel];

    if ((chnState->destAddr >= ENVRAM_DCACHE_BASEADDR) && (chnState->destAddr < OCM_DCACHE_BASEADDR + OCM_DCACHE_SIZE)) {
        for (idx = 0; idx < (chnState->transferSize + CACHE_LINE) / CACHE_LINE; idx++) {
            //OS_FlushCache((void *)(chnState->destAddr + CACHE_LINE * idx));
            OS_InvalidCache((void *)(chnState->destAddr + CACHE_LINE * idx));
        }
    } else {
    }

    PDMA_ClearIntStatus(virtualChannel);

    if (chnState != NULL) {
        if (chnState->callback != NULL) {
            chnState->callback(chnState->parameter, chnState->status);
        }
    }
}
