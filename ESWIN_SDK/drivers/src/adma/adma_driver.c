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
 * @file adma_driver.c
 * @brief ADMA driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "adma_hw_access.h"
#include "adma_register.h"

/**
 * @brief Array of base addresses for ADMA instances.
 */
static adma_type_t *const s_admaBase[ADMA_INSTANCE_COUNT] = ADMA_BASE_PTRS;

/**
 * @brief Array of default ADMA channel interrupt handlers.
 */
static const IRQn_Type s_admaIrqId[ADMA_IRQS_CH_COUNT] = ADMA_IRQS;

/**
 * @brief ADMA global structure to maintain adma state
 */
static adma_state_t *s_admaState[ADMA_INSTANCE_COUNT];

static void ADMA_DRV_IRQHandler(void *data);
static void ADMA_DRV_ClearStructure(uint8_t *structPtr, size_t size);

/**
 *
 * Function Name : ADMA_DRV_Init
 * Description   : Initializes the adma module.
 *
 */
status_t ADMA_DRV_Init(const uint32_t dmaInstance, adma_state_t *admaState, const adma_user_config_t *userConfig,
                       adma_chn_state_t *const chnStateArray[], const adma_channel_config_t *const chnConfigArray[],
                       uint32_t chnCount)
{
    uint32_t index = 0U;
    OS_RegisterType_t intType;
    // adma_type_t *admaRegBase = NULL;
    IRQn_Type irqNumber    = NotAvail_IRQn;
    status_t admaStatus    = STATUS_SUCCESS;
    status_t chnInitStatus = STATUS_SUCCESS;

    /* Check the state and configuration structure pointers are valid */
    OS_ASSERT((admaState != NULL));

    /* Check the module has not already been initialized */
    OS_ASSERT(s_admaState[dmaInstance] == NULL);

    /* Save the runtime state structure for the driver */
    s_admaState[dmaInstance] = admaState;

    /* Clear the state structure. */
    ADMA_DRV_ClearStructure((uint8_t *)s_admaState[dmaInstance], sizeof(adma_state_t));

#if 0
    /* Init ADMA instances */

    admaRegBase = s_admaBase[dmaInstance];

    /* Set operation mode */
    ADMA_SetCoreOperationMode(admaRegBase, 0, userConfig->core0Mode);
    ADMA_SetCoreOperationMode(admaRegBase, 1, userConfig->core1Mode);
    /* Sets the Core 1 clock divider */
    ADMA_SetCore1Clkdiv(admaRegBase, userConfig->core1ClkdivRatio);
#endif
    /* Register all adma interrupt handlers into vector table. */
    for (index = 0; index < ADMA_IRQS_CH_COUNT; index++) {
        intType.trig_mode = CLIC_LEVEL_TRIGGER;
        intType.lvl       = 1;
        intType.priority  = 0;
        intType.data_ptr  = (void *)dmaInstance;
        irqNumber         = s_admaIrqId[index];
        OS_RequestIrq(irqNumber, ADMA_DRV_IRQHandler, &intType);
        OS_EnableIrq(irqNumber);
    }

    /* Initialize the channels based on configuration list */
    if ((chnStateArray != NULL) && (chnConfigArray != NULL)) {
        for (index = 0U; index < chnCount; index++) {
            chnInitStatus = ADMA_DRV_ChannelInit(dmaInstance, chnStateArray[index], chnConfigArray[index]);
            if (chnInitStatus != STATUS_SUCCESS) {
                admaStatus = chnInitStatus;
                break;
            }
        }
    }

    return admaStatus;
}

/**
 *
 * Function Name : ADMA_DRV_Deinit
 * Description   : Deinitialize ADMA.
 *
 */
status_t ADMA_DRV_Deinit(const uint32_t dmaInstance)
{
    uint32_t index                   = 0U;
    IRQn_Type irqNumber              = NotAvail_IRQn;
    const adma_chn_state_t *chnState = NULL;

    if (s_admaState[dmaInstance] != NULL) {
        /* Release all adma channel. */
        for (index = 0U; index < (uint32_t)FEATURE_ADMA_CHANNELS; index++) {
            /* Release all channels. */
            chnState = s_admaState[dmaInstance]->dmaChnState[index];
            if (chnState != NULL) {
                (void)ADMA_DRV_ReleaseChannel(dmaInstance, chnState->channel);
            }
        }
        for (index = 0U; index < (uint32_t)ADMA_IRQS_CH_COUNT; index++) {
            /* Disable channel interrupts. */
            irqNumber = s_admaIrqId[index];
            OS_DisableIrq(irqNumber);
        }
    }

    s_admaState[dmaInstance] = NULL;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : ADMA_DRV_ChannelInit
 * Description   : Initialize ADMA channel.
 *
 */
status_t ADMA_DRV_ChannelInit(const uint32_t dmaInstance, adma_chn_state_t *admaChannelState,
                              const adma_channel_config_t *admaChannelConfig)
{
    /* Check the state and configuration structure pointers are valid */
    OS_ASSERT((admaChannelState != NULL) && (admaChannelConfig != NULL));

    /* Check if the module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check the channel has not already been allocated */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[admaChannelConfig->channel] == NULL);

    /* Check if the channel defined by user in the channel configuration structure is valid */
    OS_ASSERT(admaChannelConfig->channel < FEATURE_ADMA_CHANNELS);

    /* Get ADMA channel value */
    uint8_t dmaChannel = admaChannelConfig->channel;

    /* Get status */
    status_t retStatus = STATUS_SUCCESS;

    /* Load corresponding ADMA instance pointer */
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];

    /* Reset the channel state structure to default value. */
    ADMA_DRV_ClearStructure((uint8_t *)admaChannelState, sizeof(adma_chn_state_t));
    /* Set virtual channel state */
    s_admaState[dmaInstance]->dmaChnState[dmaChannel] = admaChannelState;
    /* Set virtual channel value */
    s_admaState[dmaInstance]->dmaChnState[dmaChannel]->channel = dmaChannel;
    /* Set virtual channel status to normal */
    s_admaState[dmaInstance]->dmaChnState[dmaChannel]->status = ADMA_CHN_NORMAL;

    /* Enable error interrupt for this channel */
    ADMA_SetErrorIntCmd(admaRegBase, dmaChannel, true);

    /* Install the user callback */
    retStatus = ADMA_DRV_InstallCallback(dmaInstance, admaChannelConfig->channel, admaChannelConfig->callback,
                                         admaChannelConfig->callbackParam);
    return retStatus;
}

/**
 *
 * Function Name : ADMA_DRV_InstallCallback
 * Description   : Register callback function and parameter.
 *
 */
status_t ADMA_DRV_InstallCallback(const uint32_t dmaInstance, uint8_t dmaChannel, adma_callback_t callback,
                                  void *parameter)
{
    /* Check the channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that ADMA module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check the channel is allocated */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);

    s_admaState[dmaInstance]->dmaChnState[dmaChannel]->callback  = callback;
    s_admaState[dmaInstance]->dmaChnState[dmaChannel]->parameter = parameter;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : ADMA_DRV_ReleaseChannel
 * Description   : Free adma channel's hardware and software resource.
 *
 */
status_t ADMA_DRV_ReleaseChannel(const uint32_t dmaInstance, uint8_t dmaChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check the ADMA module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Get pointer to channel state */
    adma_chn_state_t *chnState = s_admaState[dmaInstance]->dmaChnState[dmaChannel];

    /* Check that virtual channel is initialized */
    OS_ASSERT(chnState != NULL);

    adma_type_t *admaRegBase = s_admaBase[dmaInstance];

    /* Stop adma channel. */
    ADMA_SetDmaChannelRequestCmd(admaRegBase, dmaChannel, false);

    /* Reset the channel state structure to default value. */
    ADMA_DRV_ClearStructure((uint8_t *)chnState, sizeof(adma_chn_state_t));

    s_admaState[dmaInstance]->dmaChnState[dmaChannel] = NULL;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : ADMA_DRV_ClearIntStatus
 * Description   : Clear done and interrupt retStatus.
 *
 */
void ADMA_DRV_ClearIntStatus(const uint32_t dmaInstance, uint8_t dmaChannel)
{
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];
    ADMA_ClearChannelIntStatusFlag(admaRegBase, dmaChannel, ADMA_INT_CLEAR_REG_INT_CLR_CH_END_MASK);
}

/**
 *
 * Function Name : ADMA_DRV_IRQHandler
 * Description   : ADMA IRQ handler.
 *
 */
static void ADMA_DRV_IRQHandler(void *data)
{
    size_t dmaInstance       = (size_t)data;
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];

    uint32_t intStatus = ADMA_GetIntStatusFlag(admaRegBase);
    intStatus          = ((intStatus & ADMA_INT0_STATUS_CORE0_CH_INT_STATUS_MASK) |
                 (intStatus >> ADMA_INT0_STATUS_CORE1_CH_INT_STATUS_SHIFT));
    for (uint32_t dmaChannel = 0; dmaChannel < FEATURE_ADMA_CHANNELS; dmaChannel++) {
        if (!((intStatus >> dmaChannel) & 1)) {
            continue;
        }

        adma_chn_state_t *chnState = s_admaState[dmaInstance]->dmaChnState[dmaChannel];
        uint32_t intChnStatus      = ADMA_GetChannelIntStatusFlag(admaRegBase, dmaChannel);
        if (intChnStatus && (chnState != NULL)) {
            ADMA_DRV_ClearIntStatus(dmaInstance, dmaChannel);
            chnState->status = ADMA_CHN_NORMAL;
            if (chnState->callback != NULL) {
                chnState->callback(chnState->parameter, chnState->status);
            }
        }
        intChnStatus = ADMA_GetChannelErrorIntStatusFlag(admaRegBase, dmaChannel);
        if (intChnStatus && (chnState != NULL)) {
            chnState->status = ADMA_CHN_ERROR;
            if (chnState->callback != NULL) {
                chnState->callback(chnState->parameter, chnState->status);
            }
            ADMA_ClearChannelIntStatusFlag(admaRegBase, dmaChannel, ADMA_INT0_STATUS_CORE0_CH_INT_STATUS_MASK);
        }
    }
}

/**
 *
 * Function Name : ADMA_DRV_ConfigSingleBlockTransfer
 * Description   : Configures a ADMA single block transfer.
 *
 */
status_t ADMA_DRV_ConfigSingleBlockTransfer(const uint32_t dmaInstance, uint8_t dmaChannel, adma_transfer_type_t type,
                                            adma_transfer_config_t *transferConfig)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that adma module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];

    ADMA_SetCmdLast(admaRegBase, dmaChannel, true);
    /* Configure source and destination addresses */
    ADMA_SetSrcAddr(admaRegBase, dmaChannel, transferConfig->srcAddr);
    ADMA_SetDestAddr(admaRegBase, dmaChannel, transferConfig->destAddr);
    if (transferConfig->interruptEnable) {
        ADMA_SetCmdSetIntCmd(admaRegBase, dmaChannel, true);
        ADMA_SetIntCmd(admaRegBase, dmaChannel, true);
    }
    /* Configure source/destination offset. */
    switch (type) {
    case ADMA_TRANSFER_PERIPH2MEM:
        ADMA_SetRDIncr(admaRegBase, dmaChannel, 0);
        ADMA_SetWRIncr(admaRegBase, dmaChannel, 1);
        break;
    case ADMA_TRANSFER_MEM2PERIPH:
        ADMA_SetRDIncr(admaRegBase, dmaChannel, 1);
        ADMA_SetWRIncr(admaRegBase, dmaChannel, 0);
        break;
    case ADMA_TRANSFER_MEM2MEM:
        ADMA_SetRDIncr(admaRegBase, dmaChannel, 1);
        ADMA_SetWRIncr(admaRegBase, dmaChannel, 1);
        break;
    case ADMA_TRANSFER_PERIPH2PERIPH:
        ADMA_SetRDIncr(admaRegBase, dmaChannel, 0);
        ADMA_SetWRIncr(admaRegBase, dmaChannel, 0);
        break;
    default:
        /* This should never be reached - all the possible values have been handled. */
        break;
    }
    ADMA_SetRdBustRdTokens(admaRegBase, dmaChannel, transferConfig->rdBrustSize, transferConfig->rdTokens);
    ADMA_SetWrBustWrTokens(admaRegBase, dmaChannel, transferConfig->wrBrustSize, transferConfig->wrTokens);
    ADMA_SetPeriphConfig(admaRegBase, dmaChannel, &transferConfig->periphConfig);
    /* Set the total number of bytes to be transferred */
    ADMA_SetBufferSize(admaRegBase, dmaChannel, transferConfig->bufferSize);

    /* Set virtual channel status to normal */
    s_admaState[dmaInstance]->dmaChnState[dmaChannel]->status = ADMA_CHN_NORMAL;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : ADMA_DRV_StartChannel
 * Description   : Starts an adma channel.
 *
 */
status_t ADMA_DRV_StartChannel(const uint32_t dmaInstance, uint8_t dmaChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that adma module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);

    /* Enable requests for current channel */
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];
    ADMA_SetDmaChannelRequestCmd(admaRegBase, dmaChannel, true);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : ADMA_DRV_StopChannel
 * Description   : Stops an adma channel.
 *
 */
status_t ADMA_DRV_StopChannel(const uint32_t dmaInstance, uint8_t dmaChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that adma module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);

    /* Disable requests for current channel */
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];
    ADMA_SetDmaChannelRequestCmd(admaRegBase, dmaChannel, false);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : ADMA_DRV_ResumeChannel
 * Description   : Resume an adma channel.
 *
 */
status_t ADMA_DRV_ResumeChannel(const uint32_t dmaInstance, uint8_t dmaChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that adma module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);

    /* Resume requests for current channel */
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];
    ADMA_SetDmaChannelEnableCmd(admaRegBase, dmaChannel, true);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : ADMA_DRV_SuspendChannel
 * Description   : Suspend an adma channel.
 *
 */
status_t ADMA_DRV_SuspendChannel(const uint32_t dmaInstance, uint8_t dmaChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that adma module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);

    /* Resume requests for current channel */
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];
    ADMA_SetDmaChannelEnableCmd(admaRegBase, dmaChannel, false);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : ADMA_DRV_SetSrcAddr
 * Description   : Configures the source address for the adma channel.
 *
 */
void ADMA_DRV_SetSrcAddr(const uint32_t dmaInstance, uint8_t dmaChannel, uint32_t address)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that adma module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);

    /* Set channel source address */
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];
    ADMA_SetSrcAddr(admaRegBase, dmaChannel, address);
}

/**
 *
 * Function Name : ADMA_DRV_SetDestAddr
 * Description   : Configures the destination address for the adma channel.
 *
 */
void ADMA_DRV_SetDestAddr(const uint32_t dmaInstance, uint8_t dmaChannel, uint32_t address)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that adma module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);

    /* Set channel destination address */
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];
    ADMA_SetDestAddr(admaRegBase, dmaChannel, address);
}

/**
 *
 * Function Name : ADMA_DRV_ConfigureInterrupt
 * Description   : Disables/Enables the channel interrupt requests.
 *
 */
void ADMA_DRV_ConfigureInterrupt(const uint32_t dmaInstance, uint8_t dmaChannel, adma_channel_interrupt_t intSrc,
                                 bool enable)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that adma module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);

    /* Disables/Enables the channel interrupt requests. */
    adma_type_t *admaRegBase = s_admaBase[dmaInstance];
    switch (intSrc) {
    case ADMA_CHN_ERR_INT:
        /* Enable channel interrupt request when error conditions occur */
        ADMA_SetErrorIntCmd(admaRegBase, dmaChannel, enable);
        break;
    case ADMA_CHN_CH_END_INT:
        /* Enable channel interrupt request when major iteration count reaches halfway point */
        ADMA_SetIntCmd(admaRegBase, dmaChannel, enable);
        break;
        /* This branch should never be reached if driver API is used properly */
        break;
    }
}

/**
 *
 * Function Name : ADMA_DRV_GetChannelStatus
 * Description   : Returns the adma channel retStatus.
 *
 */
adma_chn_status_t ADMA_DRV_GetChannelStatus(const uint32_t dmaInstance, uint8_t dmaChannel)
{
    /* Check that virtual channel number is valid */
    OS_ASSERT(dmaChannel < FEATURE_ADMA_CHANNELS);

    /* Check that adma module is initialized */
    OS_ASSERT(s_admaState[dmaInstance] != NULL);

    /* Check that virtual channel is initialized */
    OS_ASSERT(s_admaState[dmaInstance]->dmaChnState[dmaChannel] != NULL);

    return s_admaState[dmaInstance]->dmaChnState[dmaChannel]->status;
}

/**
 *
 * Function Name : ADMA_DRV_GetDmaRegBaseAddr
 * Description   : Returns the ADMA register base address.
 *
 */
adma_type_t *ADMA_DRV_GetDmaRegBaseAddr(const uint32_t dmaInstance)
{
    /* Check that instance is valid */
    OS_ASSERT(dmaInstance < ADMA_INSTANCE_COUNT);

    return s_admaBase[dmaInstance];
}

/**
 *
 * Function Name : ADMA_DRV_ClearStructure
 * Description   : Clears all bytes at the passed structure pointer.
 *
 */
static void ADMA_DRV_ClearStructure(uint8_t *structPtr, size_t size)
{
    while (size > 0U) {
        *structPtr = 0;
        structPtr++;
        size--;
    }
}
