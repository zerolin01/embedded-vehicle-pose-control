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
 * @file pdma_driver.h
 * @brief PDMA driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PDMA_DRIVER_H__
#define __PDMA_DRIVER_H__

#include "emps_platform.h"
#include "status.h"
#include <stddef.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup PDMA_Driver PDMA Driver
 * @{
 */

/* cache line */
#define CACHE_LINE 0x10

/** @brief Number of PDMA channels. */
#define DMAMUX_CHANNELS (32U)

/** @brief Number of PDMA channels. */
#define DMA_CHANNELS (32U)

/** @brief Number of PDMA virtual channels. */
#define DMA_VIRTUAL_CHANNELS (DMA_CHANNELS * PDMA_INSTANCE_COUNT)

/** @brief PDMA virtual channel to channel */
#define DMA_VCH_TO_CH(x) ((x) & ((uint32_t)DMA_CHANNELS - 1U))

/** @brief PDMA channel to instance */
#define DMA_VCH_TO_INSTANCE(x) ((x) >> (uint32_t)FEATURE_DMA_CH_WIDTH)

/** @brief Number of PDMA error interrupt lines. */
#define PDMA_ERROR_INTERRUPT_NUMS (1U)

/** @brief Mapping between request source and DMAMUX instance */
#define DMAMUX_REQ_SRC_TO_INSTANCE(x) (0U)

/** @brief Conversion from pDMA channel index to DMAMUX channel. */
#define DMAMUX_DMA_CH_TO_CH(x) (x)

/** @brief Conversion from request source to the actual DMAMUX channel */
#define DMAMUX_REQ_SRC_TO_CH(x) (x)

/** @brief Number of PDMA virtual interrupt lines. */
#define DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES ((uint32_t)DMA_CHANNELS_INTERRUPT_LINES * (uint32_t)PDMA_INSTANCE_COUNT)

/** @brief Conversion from DMAMUX channel DMAMUX register index. */
#define DMAMUX_CHN_REG_INDEX(x) (x)
/**
 * @brief Macro for the memory size needed for the software TCD.
 */
#define STCD_SIZE(number) (((number)*32U) - 1U)

#define STCD_ADDR(address) (((uint32_t)address + 31UL) & ~0x1FUL)

/**
 * @brief Macro for accessing the least significant bit of the ERR register.
 */
#define PDMA_ERR_LSB_MASK 1U

/**
 * @brief PDMA channel interrupts.
 */
typedef enum {
    PDMA_CHN_ERR_INT = 0U,        /**< Error interrupt. */
    PDMA_CHN_HALF_MAJOR_LOOP_INT, /**< Half major loop interrupt. */
    PDMA_CHN_MAJOR_LOOP_INT       /**< Complete major loop interrupt. */
} pdma_channel_interrupt_t;

/**
 * @brief PDMA channel arbitration algorithm used for selection among channels.
 */
typedef enum {
    PDMA_ARBITRATION_FIXED_PRIORITY = 0U, /**< Fixed Priority. */
    PDMA_ARBITRATION_ROUND_ROBIN          /**< Round-Robin arbitration. */
} pdma_arbitration_algorithm_t;

/**
 * @brief PDMA channel priority setting.
 */
typedef enum {
    PDMA_CHN_PRIORITY_0       = 0U,
    PDMA_CHN_PRIORITY_1       = 1U,
    PDMA_CHN_PRIORITY_2       = 2U,
    PDMA_CHN_PRIORITY_3       = 3U,
    PDMA_CHN_PRIORITY_4       = 4U,
    PDMA_CHN_PRIORITY_5       = 5U,
    PDMA_CHN_PRIORITY_6       = 6U,
    PDMA_CHN_PRIORITY_7       = 7U,
    PDMA_CHN_PRIORITY_8       = 8U,
    PDMA_CHN_PRIORITY_9       = 9U,
    PDMA_CHN_PRIORITY_10      = 10U,
    PDMA_CHN_PRIORITY_11      = 11U,
    PDMA_CHN_PRIORITY_12      = 12U,
    PDMA_CHN_PRIORITY_13      = 13U,
    PDMA_CHN_PRIORITY_14      = 14U,
    PDMA_CHN_PRIORITY_15      = 15U,
    PDMA_CHN_DEFAULT_PRIORITY = 255U
} pdma_channel_priority_t;

/**
 * @brief PDMA group priority setting.
 */
typedef enum {

    PDMA_GRP0_PRIO_LOW_GRP1_PRIO_HIGH = 0U,
    PDMA_GRP0_PRIO_HIGH_GRP1_PRIO_LOW = 1U
} pdma_group_priority_t;

/**
 * @brief PDMA modulo configuration.
 */
typedef enum {
    PDMA_MODULO_OFF = 0U,
    PDMA_MODULO_2B,
    PDMA_MODULO_4B,
    PDMA_MODULO_8B,
    PDMA_MODULO_16B,
    PDMA_MODULO_32B,
    PDMA_MODULO_64B,
    PDMA_MODULO_128B,
    PDMA_MODULO_256B,
    PDMA_MODULO_512B,
    PDMA_MODULO_1KB,
    PDMA_MODULO_2KB,
    PDMA_MODULO_4KB,
    PDMA_MODULO_8KB,
    PDMA_MODULO_16KB,
    PDMA_MODULO_32KB,
    PDMA_MODULO_64KB,
    PDMA_MODULO_128KB,
    PDMA_MODULO_256KB,
    PDMA_MODULO_512KB,
    PDMA_MODULO_1MB,
    PDMA_MODULO_2MB,
    PDMA_MODULO_4MB,
    PDMA_MODULO_8MB,
    PDMA_MODULO_16MB,
    PDMA_MODULO_32MB,
    PDMA_MODULO_64MB,
    PDMA_MODULO_128MB,
    PDMA_MODULO_256MB,
    PDMA_MODULO_512MB,
    PDMA_MODULO_1GB,
    PDMA_MODULO_2GB
} pdma_modulo_t;

/**
 * @brief PDMA transfer configuration.
 */
typedef enum {
    PDMA_TRANSFER_SIZE_1B = 0x0U,
    PDMA_TRANSFER_SIZE_2B = 0x1U,
    PDMA_TRANSFER_SIZE_4B = 0x2U,
} pdma_transfer_size_t;

/**
 * @brief The user configuration structure for the pDMA driver.
 */
typedef struct {
    pdma_arbitration_algorithm_t chnArbitration;   /**< PDMA channel arbitration. \D{PDMA_ARBITRATION_FIXED_PRIORITY} */
    pdma_arbitration_algorithm_t groupArbitration; /**< PDMA group arbitration. \D{PDMA_ARBITRATION_FIXED_PRIORITY} */
    pdma_group_priority_t groupPriority;           /**< PDMA group priority. Used while pDMA
                                                        group arbitration is set to fixed priority. \D{PDMA_GRP0_PRIO_LOW_GRP1_PRIO_HIGH} */
    bool haltOnError;                              /**< Any error causes the HALT bit to set. Subsequently, all
                                                        service requests are ignored until the HALT bit is cleared. \D{false} */
} pdma_user_config_t;

/**
 * @brief Channel status for pDMA channel.
 */
typedef enum {
    PDMA_CHN_NORMAL = 0U, /**< PDMA channel normal state. */
    PDMA_CHN_ERROR        /**< An error occurred in the pDMA channel. */
} pdma_chn_status_t;

/**
 * @brief Definition for the pDMA channel callback function.
 */
typedef void (*pdma_callback_t)(void *parameter, pdma_chn_status_t status);

/**
 * @brief Data structure for the pDMA channel state.
 */
typedef struct {
    uint8_t virtChn;                   /**< Virtual channel number. */
    pdma_callback_t callback;          /**< Callback function pointer for the pDMA channel. It will
                                          be called at the pDMA channel complete and pDMA channel error. */
    void *parameter;                   /**< Parameter for the callback function pointer. */
    volatile pdma_chn_status_t status; /**< PDMA channel status. */
    uint32_t srcAddr;                  /**< Source Addr. */
    uint32_t destAddr;                 /**< Dest Addr. */
    uint32_t transferSize;             /**< Transfer size. */
} pdma_chn_state_t;

/**
 * @brief The user configuration structure for the an pDMA driver channel.
 */
typedef struct {
    pdma_group_priority_t
        groupPriority;                       /**< PDMA group priority. Used while pDMA
                                                group arbitration is set to fixed priority. \D{PDMA_GRP0_PRIO_LOW_GRP1_PRIO_HIGH} */
    pdma_channel_priority_t channelPriority; /**< PDMA channel priority - only used when channel
                                                arbitration mode is 'Fixed priority'. \D{PDMA_CHN_DEFAULT_PRIORITY} */
    uint8_t virtChnConfig;                   /**< PDMA virtual channel number. \D{0} */
    dma_request_source_t source;             /**< Selects the source of the PDMA request for this channel. \D{PDMA_REQ_DISABLED} */
    pdma_callback_t callback;                /**< Callback that will be registered for this channel. \D{NULL} */
    void *callbackParam;                     /**< Parameter passed to the channel callback. \D{NULL} */
    bool enableTrigger;                      /**< Enables the periodic trigger capability for the PDMA channel. \D{false} */
} pdma_channel_config_t;

/**
 * @brief A type for the PDMA transfer.
 */
typedef enum {
    PDMA_TRANSFER_PERIPH2MEM = 0U, /**< Transfer from peripheral to memory. */
    PDMA_TRANSFER_MEM2PERIPH,      /**< Transfer from memory to peripheral. */
    PDMA_TRANSFER_MEM2MEM,         /**< Transfer from memory to memory. */
    PDMA_TRANSFER_PERIPH2PERIPH    /**< Transfer from peripheral to peripheral. */
} pdma_transfer_type_t;

/**
 * @brief Runtime state structure for the pDMA driver.
 */
typedef struct {
    pdma_chn_state_t
        *volatile virtChnState[(uint32_t)DMA_VIRTUAL_CHANNELS]; /**< Pointer array storing channel state. */
} pdma_state_t;

/**
 * @brief PDMA loop transfer configuration.
 */
typedef struct {
    uint32_t majorLoopIterationCount; /**< Number of major loop iterations. */
    bool srcOffsetEnable;             /**< Selects whether the minor loop offset is applied to the
                                         source address upon minor loop completion. */
    bool dstOffsetEnable;             /**< Selects whether the minor loop offset is applied to the
                                         destination address upon minor loop completion. */
    int32_t minorLoopOffset;          /**< Sign-extended offset applied to the source or destination address
                                         to form the next-state value after the minor loop completes. */
    bool minorLoopChnLinkEnable;      /**< Enables channel-to-channel linking on minor loop complete. */
    uint8_t minorLoopChnLinkNumber;   /**< The number of the next channel to be started by PDMA
                                         engine when minor loop completes. */
    bool majorLoopChnLinkEnable;      /**< Enables channel-to-channel linking on major loop complete. */
    uint8_t majorLoopChnLinkNumber;   /**< The number of the next channel to be started by PDMA
                                         engine when major loop completes. */
} pdma_loop_transfer_config_t;

/**
 * @brief PDMA transfer size configuration.
 */
typedef struct {
    uint32_t srcAddr;                                /**< Memory address pointing to the source data. */
    uint32_t destAddr;                               /**< Memory address pointing to the destination data. */
    pdma_transfer_size_t srcTransferSize;            /**< Source data transfer size. */
    pdma_transfer_size_t destTransferSize;           /**< Destination data transfer size. */
    int16_t srcOffset;                               /**< Sign-extended offset applied to the current source address
                                              to form the next-state value as each source read/write is completed. */
    int16_t destOffset;                              /**< Sign-extended offset applied to the current destination
                                              address to form the next-state value as each source
                                              read/write is completed. */
    int32_t srcLastAddrAdjust;                       /**< Last source address adjustment. */
    int32_t destLastAddrAdjust;                      /**< Last destination address adjustment. Note here it is only
                                              valid when scatter/gather feature is not enabled. */
    pdma_modulo_t srcModulo;                         /**< Source address modulo. */
    pdma_modulo_t destModulo;                        /**< Destination address modulo. */
    uint32_t minorByteTransferCount;                 /**< Number of bytes to be transferred in each service request of the channel. */
    bool interruptEnable;                            /**< Enable the interrupt request when the major loop count completes */
    pdma_loop_transfer_config_t *loopTransferConfig; /**< Pointer to loop transfer configuration structure
                                                        (defines minor/major loop attributes)
                                                        Note: this field is only used when minor loop mapping is
                                                              enabled from PDMA configuration. */
} pdma_transfer_config_t;

#if (defined(CORE_LITTLE_ENDIAN))
/**
 * @brief pDMA TCD.
 */
typedef struct {
    uint32_t SADDR;
    int16_t SOFF;
    uint16_t ATTR;
    uint32_t NBYTES;
    int32_t SLAST;
    uint32_t DADDR;
    int16_t DOFF;
    uint16_t CITER;
    int32_t DLAST_SGA;
    uint16_t CSR;
    uint16_t BITER;
} pdma_software_tcd_t;

#elif (defined(CORE_BIG_ENDIAN))
/**
 * @brief pDMA TCD.
 */
typedef struct {
    uint32_t SADDR;
    uint16_t ATTR;
    int16_t SOFF;
    uint32_t NBYTES;
    int32_t SLAST;
    uint32_t DADDR;
    uint16_t CITER;
    int16_t DOFF;
    int32_t DLAST_SGA;
    uint16_t BITER;
    uint16_t CSR;
} pdma_software_tcd_t;
#else
#error "Endianness not defined!"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Initializes the pDMA module.
 *
 * @param  pdmaState      The pointer to the pDMA peripheral driver state structure.
 * @param  userConfig     User configuration structure for pDMA peripheral drivers.
 * @param  chnStateArray  Array of pointers to run-time state structures for pDMA channels;
 * @param  chnConfigArray Array of pointers to channel initialization structures.
 * @param  chnCount       The number of pDMA channels to be initialized.
 * @return status_t
 * STATUS_SUCCESS: successful.
 * STATUS_ERROR: PDMA init error.
 */
status_t PDMA_DRV_Init(pdma_state_t *pdmaState, const pdma_user_config_t *userConfig,
                       pdma_chn_state_t *const chnStateArray[], const pdma_channel_config_t *const chnConfigArray[],
                       uint32_t chnCount);

/**
 * @brief De-initializes the pDMA module.
 *
 * @return status_t
 * STATUS_SUCCESS: successful.
 */
status_t PDMA_DRV_Deinit(void);

/**
 * @brief Initializes an pDMA channel.
 *
 * @param  pdmaChannelState  Pointer to the pDMA channel state structure.
 * @param  pdmaChannelConfig User configuration structure for pDMA channel.
 * @return status_t
 * STATUS_SUCCESS: successful.
 */
status_t PDMA_DRV_ChannelInit(pdma_chn_state_t *pdmaChannelState, const pdma_channel_config_t *pdmaChannelConfig);

/**
 * @brief Releases an pDMA channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @return status_t
 * STATUS_SUCCESS: successful.
 */
status_t PDMA_DRV_ReleaseChannel(uint8_t virtualChannel);

/**
 * @brief  Copies the channel configuration to the TCD registers.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  tcd Pointer to the channel configuration structure.
 */
void PDMA_DRV_PushConfigToReg(uint8_t virtualChannel, const pdma_transfer_config_t *tcd);

/**
 * @brief  Copies the channel configuration to the software TCD structure.
 *
 * @param  config Pointer to the channel configuration structure.
 * @param  stcd Pointer to the software TCD structure.
 */
void PDMA_DRV_PushConfigToSTCD(const pdma_transfer_config_t *config, pdma_software_tcd_t *stcd);

/**
 * @brief  Configures a simple single block data transfer with PDMA.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  type           Transfer type (M->M, P->M, M->P, P->P).
 * @param  srcAddr        A source register address or a source memory address.
 * @param  destAddr       A destination register address or a destination memory address.
 * @param  transferSize   The number of bytes to be transferred on every PDMA write/read.
 * @param  dataBufferSize The total number of bytes to be transferred.
 * @return status_t
 * STATUS_SUCCESS: successful.
 * STATUS_ERROR: In case of a PDMA error transfer.
 */
status_t PDMA_DRV_ConfigSingleBlockTransfer(uint8_t virtualChannel, pdma_transfer_type_t type, uint32_t srcAddr,
                                            uint32_t destAddr, pdma_transfer_size_t transferSize,
                                            uint32_t dataBufferSize);

/**
 * @brief  Configures a multiple block data transfer with PDMA.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  type           Transfer type (M->M, P->M, M->P, P->P).
 * @param  srcAddr        A source register address or a source memory address.
 * @param  destAddr       A destination register address or a destination memory address.
 * @param  transferSize   The number of bytes to be transferred on every PDMA write/read.
 * @param  blockSize      The total number of bytes inside a block.
 * @param  blockCount      The total number of data blocks (one block is transferred upon a PDMA request).
 * @param  disableReqOnCompletion This parameter specifies whether the PDMA channel should be disabled when the transfer
 *                                is complete (further requests will remain untreated).
 * @return status_t
 * STATUS_SUCCESS: successful.
 * STATUS_ERROR: If a PDMA error transfer.
 */
status_t PDMA_DRV_ConfigMultiBlockTransfer(uint8_t virtualChannel, pdma_transfer_type_t type, uint32_t srcAddr,
                                           uint32_t destAddr, pdma_transfer_size_t transferSize, uint32_t blockSize,
                                           uint32_t blockCount, bool disableReqOnCompletion);

/**
 * @brief  Configures the PDMA transfer in loop mode.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  transferConfig  Pointer to the transfer configuration structure; this structure defines fields for setting
 *                         up the basic transfer and also a pointer to a memory strucure that defines the loop chain
 *                         properties (minor/major).
 * @return status_t
 * STATUS_SUCCESS: successful.
 */
status_t PDMA_DRV_ConfigLoopTransfer(uint8_t virtualChannel, pdma_transfer_config_t *transferConfig);

/**
 * @brief  Cancel the running transfer.
 *
 * @param  error Error If true, an error will be logged for the current transfer.
 */
void PDMA_DRV_CancelTransfer(bool error);

/**
 * @brief Starts an pDMA channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @return status_t
 * STATUS_SUCCESS: successful.
 */
status_t PDMA_DRV_StartChannel(uint8_t virtualChannel);

/**
 * @brief Stops the pDMA channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @return status_t
 * STATUS_SUCCESS: successful.
 */
status_t PDMA_DRV_StopChannel(uint8_t virtualChannel);

/**
 * @brief Configures the PDMA request for the pDMA channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  request        PDMA request source.
 * @param  enableTrigger  PDMA channel periodic trigger.
 * @return status_t
 * STATUS_SUCCESS: successful.
 * STATUS_UNSUPPORTED: In case of a PDMA error transfer.
 */
status_t PDMA_DRV_SetChannelRequestAndTrigger(uint8_t virtualChannel, uint8_t request, bool enableTrigger);

/**
 * @brief  Clears all registers to 0 for the channel's TCD.
 *
 * @param  virtualChannel PDMA virtual channel number.
 */
void PDMA_DRV_ClearTCD(uint8_t virtualChannel);

/**
 * @brief Configures the source address for the pDMA channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  address The pointer to the source memory address.
 */
void PDMA_DRV_SetSrcAddr(uint8_t virtualChannel, uint32_t address);

/**
 * @brief  Configures the source address signed offset for the pDMA channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  offset Signed-offset for source address.
 */
void PDMA_DRV_SetSrcOffset(uint8_t virtualChannel, int16_t offset);

/**
 * @brief Configures the source data chunk size (transferred in a read sequence).
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  size Source transfer size.
 */
void PDMA_DRV_SetSrcReadChunkSize(uint8_t virtualChannel, pdma_transfer_size_t size);

/**
 * @brief  Configures the source address last adjustment.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  adjust Adjustment value.
 */
void PDMA_DRV_SetSrcLastAddrAdjustment(uint8_t virtualChannel, int32_t adjust);

/**
 * @brief Configures the destination address for the pDMA channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  address The pointer to the destination memory address.
 */
void PDMA_DRV_SetDestAddr(uint8_t virtualChannel, uint32_t address);

/**
 * @brief  Configures the destination address signed offset for the pDMA channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  offset signed-offset.
 */
void PDMA_DRV_SetDestOffset(uint8_t virtualChannel, int16_t offset);

/**
 * @brief Configures the destination data chunk size (transferred in a write sequence).
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  size Destination transfer size.
 */
void PDMA_DRV_SetDestWriteChunkSize(uint8_t virtualChannel, pdma_transfer_size_t size);

/**
 * @brief  Configures the destination address last adjustment.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  adjust Adjustment value.
 */
void PDMA_DRV_SetDestLastAddrAdjustment(uint8_t virtualChannel, int32_t adjust);

/**
 * @brief  Configures the number of bytes to be transferred in each service request of the channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  nbytes Number of bytes to be transferred in each service request of the channel.
 */
void PDMA_DRV_SetMinorLoopBlockSize(uint8_t virtualChannel, uint32_t nbytes);

/**
 * @brief Configures the number of major loop iterations.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  majorLoopCount Number of major loop iterations.
 */
void PDMA_DRV_SetMajorLoopIterationCount(uint8_t virtualChannel, uint32_t majorLoopCount);

/**
 * @brief  Returns the remaining major loop iteration count.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @return uint32_t
 *         Number of major loop iterations yet to be triggered
 */
uint32_t PDMA_DRV_GetRemainingMajorIterationsCount(uint8_t virtualChannel);

/**
 * @brief  Disables/Enables the PDMA request after the major loop completes for the TCD.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  disable Disable (true)/Enable (false) PDMA request after TCD complete.
 */
void PDMA_DRV_DisableRequestsOnTransferComplete(uint8_t virtualChannel, bool disable);

/**
 * @brief  Disables/Enables the channel interrupt requests.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  intSrc         Interrupt event (error/half major loop/complete major loop).
 * @param  enable         Enable (true)/Disable (false) interrupts for the current channel.
 */
void PDMA_DRV_ConfigureInterrupt(uint8_t virtualChannel, pdma_channel_interrupt_t intSrc, bool enable);

/**
 * @brief Triggers a sw request for the current channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 */
void PDMA_DRV_TriggerSwRequest(uint8_t virtualChannel);

/**
 * @brief  Registers the callback function and the parameter for pDMA channel.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @param  callback       The pointer to the callback function.
 * @param  parameter      The pointer to the callback function's parameter.
 * @return status_t
 * STATUS_SUCCESS: successful.
 */
status_t PDMA_DRV_InstallCallback(uint8_t virtualChannel, pdma_callback_t callback, void *parameter);

/**
 * @brief Gets the pDMA channel status.
 *
 * @param  virtualChannel PDMA virtual channel number.
 * @return pdma_chn_status_t
 *         PDMA_CHN_NORMAL: PDMA channel normal state.
 *         PDMA_CHN_ERROR: An error occurred in the pDMA channel.
 */
pdma_chn_status_t PDMA_DRV_GetChannelStatus(uint8_t virtualChannel);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group PDMA_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif
