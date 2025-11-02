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
 * @file adma_driver.h
 * @brief ADMA driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __ADMA_DRIVER_H__
#define __ADMA_DRIVER_H__

#include "emps_platform.h"
#include "status.h"
#include <stddef.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup ADMA_Driver ADMA Driver
 * @{
 */

/**
 * @brief ADMA channel interrupts.
 */
typedef enum {
    ADMA_CHN_ERR_INT    = 0U, /**< Error interrupt */
    ADMA_CHN_CH_END_INT = 1U, /**< CH_END interrupt. */
} adma_channel_interrupt_t;

/**
 * @brief ADMA operation modes.
 */
typedef enum {
    ADMA_OPERATION_INDEPENDENT_MODE = 0U, /**< Round-Robin arbitration. */
    ADMA_OPERATION_JOINT_MODE       = 1U, /**< Fixed Priority. */
} adma_operation_mode_t;

/**
 * @brief The user configuration structure for the ADMA driver.
 *
 * Use an instance of this structure with the ADMA_DRV_Init() function. This allows the user to configure
 * settings of the ADMA peripheral with a single function call.
 */
typedef struct {
    adma_operation_mode_t core0Mode; /**< ADMA operation mode. \D{ADMA_OPERATION_INDEPENDENT_MODE} */
    adma_operation_mode_t core1Mode; /**< ADMA operation mode. \D{ADMA_OPERATION_INDEPENDENT_MODE} */
    uint32_t core1ClkdivRatio;       /**< Ratio between main clock and core 1 clock. \D{1} */
} adma_user_config_t;

/**
 * @brief Channel status for ADMA channel.
 *
 * A structure describing the ADMA channel status. The user can get the status by callback parameter
 * or by calling ADMA_DRV_getStatus() function.
 */
typedef enum {
    ADMA_CHN_NORMAL = 0U, /**< ADMA channel normal state. */
    ADMA_CHN_ERROR        /**< An error occurred in the ADMA channel. */
} adma_chn_status_t;

/**
 * @brief Definition for the ADMA channel callback function.
 *
 * Prototype for the callback function registered in the ADMA driver.
 */
typedef void (*adma_callback_t)(void *parameter, adma_chn_status_t status);

/**
 * @brief Data structure for the ADMA channel state.
 */
typedef struct {
    uint8_t channel;                   /**< Channel number. */
    adma_callback_t callback;          /**< Callback function pointer for the ADMA channel. It will be called at
                                          the ADMA channel complete and ADMA channel error. */
    void *parameter;                   /**< Parameter for the callback function pointer. */
    volatile adma_chn_status_t status; /**< ADMA channel status. */
} adma_chn_state_t;

/**
 * @brief The user configuration structure for the an ADMA driver channel.
 *
 * Use an instance of this structure with the ADMA_DRV_ChannelInit() function. This allows the user to configure
 * settings of the ADMA channel with a single function call.
 */
typedef struct {
    uint8_t channel;          /**< ADMA channel number. \D{ADMA_CHN0_NUMBER} */
    void *callbackParam;      /**< Parameter passed to the channel callback. \D{NULL} */
    adma_callback_t callback; /**< Callback that will be registered for this channel. \D{NULL} */
} adma_channel_config_t;

/**
 * @brief A type for the ADMA transfer.
 */
typedef enum {
    ADMA_TRANSFER_PERIPH2MEM = 0U, /**< Transfer from peripheral to memory. */
    ADMA_TRANSFER_MEM2PERIPH,      /**< Transfer from memory to peripheral. */
    ADMA_TRANSFER_MEM2MEM,         /**< Transfer from memory to memory. */
    ADMA_TRANSFER_PERIPH2PERIPH,   /**< Transfer from peripheral to peripheral. */
} adma_transfer_type_t;

/**
 * @brief Runtime state structure for the ADMA driver.
 *
 * This structure holds data that is used by the ADMA peripheral driver to manage multi ADMA channels.
 * The user passes the memory for this run-time state structure and the ADMA driver populates the members.
 */
typedef struct {
    adma_chn_state_t *volatile dmaChnState[FEATURE_ADMA_CHANNELS]; /**< Pointer array storing channel state. */
} adma_state_t;

/**
 * @brief ADMA peripheral configuration.
 *
 * This structure configures the basic source/destination transfer attribute.
 */
typedef struct {
    adma_periph_num_t rdPeriphNum; /**< Number of peripheral to read from. Set 0 if the channel reads from a memory,
                                      or reads from a peripheral that does not use peripheral flow control. */
    uint32_t rdPeriphDelay;        /**< Number of cycles to wait for the peripheral read request signal to update
                                      after issuing the read clear signal. This is determined by the peripheral latency. */
    adma_periph_num_t wrPeriphNum; /**< Number of peripheral to write to. Set 0 if the channel writes to
                                      a memory, or writes to a peripheral that does not use peripheral flow control. */
    uint32_t wrPeriphDelay;        /**< Number of cycles to wait for the peripheral write request signal to update
                                      after issuing the write clear signal. This is determined by the peripheral latency */
} adma_peripheral_config_t;

/**
 * @brief ADMA transfer size configuration.
 *
 * This structure configures the basic source/destination transfer attribute.
 */
typedef struct {
    uint32_t srcAddr;     /**< Memory address pointing to the source data. */
    uint32_t destAddr;    /**< Memory address pointing to the destination data. */
    uint32_t bufferSize;  /**< Size of buffer to transfer. */
    uint32_t rdTokens;    /**< Number of AHB read commands to issue before the channel is released. */
    uint32_t rdBrustSize; /**< Maximum number of bytes of an AHB read burst. Possible values: 1, 2, 4,
                            data_width*N    (N=4,8,16). */
    uint32_t wrTokens;    /**< Number of AHB write commands to issue before the channel is released. */
    uint32_t wrBrustSize; /**< Maximum number of bytes of an AHB write burst. Possible values: 1, 2, 4,
                            data_width*N (N=4.8.16). */
    bool interruptEnable; /**< Enable the interrupt request when transfer completes */
    adma_peripheral_config_t periphConfig;
} adma_transfer_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initializes the ADMA module.
 *
 * This function initializes the run-time state structure to provide the ADMA channel allocation
 * release, protect, and track the state for channels. This function also resets the ADMA modules,
 * initializes the module to user-defined settings and default settings.
 *
 * @param dmaInstance The ADMA instance number.
 * @param admaState The pointer to the ADMA peripheral driver state structure. The user passes
 * the memory for this run-time state structure and the ADMA peripheral driver populates the
 * members. This run-time state structure keeps track of the ADMA channels status. The memory must
 * be kept valid before calling the ADMA_DRV_DeInit.
 * @param userConfig User configuration structure for ADMA peripheral drivers. The user populates the
 * members of this structure and passes the pointer of this structure into the function.
 * @param chnStateArray Array of pointers to run-time state structures for ADMA channels;
 * will populate the state structures inside the ADMA driver state structure.
 * @param chnConfigArray Array of pointers to channel initialization structures.
 * @param chnCount The number of ADMA channels to be initialized.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t ADMA_DRV_Init(const uint32_t dmaInstance, adma_state_t *admaState, const adma_user_config_t *userConfig,
                       adma_chn_state_t *const chnStateArray[], const adma_channel_config_t *const chnConfigArray[],
                       uint32_t chnCount);

/**
 * @brief De-initializes the ADMA module.
 *
 * This function resets the ADMA module to reset state and disables the interrupt to the core.
 *
 * @param dmaInstance The ADMA instance number.
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t ADMA_DRV_Deinit(const uint32_t dmaInstance);

/**
 * @brief Initializes an ADMA channel.
 *
 * This function initializes the run-time state structure for a ADMA channel, based on user
 * configuration. It will request the channel, set up the channel priority and install the
 * callback.
 *
 * @param dmaInstance The ADMA instance number.
 * @param admaChannelState Pointer to the ADMA channel state structure. The user passes
 * the memory for this run-time state structure and the ADMA peripheral driver populates the
 * members. This run-time state structure keeps track of the ADMA channel status. The memory must
 * be kept valid before calling the ADMA_DRV_ReleaseChannel.
 * @param admaChannelConfig User configuration structure for ADMA channel. The user populates the
 * members of this structure and passes the pointer of this structure into the function.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t ADMA_DRV_ChannelInit(const uint32_t dmaInstance, adma_chn_state_t *admaChannelState,
                              const adma_channel_config_t *admaChannelConfig);

/**
 * @brief Releases an ADMA channel.
 *
 * This function stops the ADMA channel and disables the interrupt of this channel. The channel state
 * structure can be released after this function is called.
 *
 * @param dmaInstance The ADMA instance number.
 * @param dmaChannel ADMA channel number.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t ADMA_DRV_ReleaseChannel(const uint32_t dmaInstance, uint8_t dmaChannel);

/**
 * @brief Configures a simple single block data transfer with ADMA.
 *
 * This function configures the descriptor for a single block transfer.
 * The function considers contiguous memory blocks, thus it configures the TCD
 * source/destination offset fields to cover the data buffer without gaps,
 * according to "transferSize" parameter (the offset is equal to the number of
 * bytes transferred in a source read/destination write).
 *
 * NOTE: For memory-to-peripheral or peripheral-to-memory transfers, make sure
 * the transfer size is equal to the data buffer size of the peripheral used,
 * otherwise only truncated chunks of data may be transferred (e.g. for a
 * communication IP with an 8-bit data register the transfer size should be 1B,
 * whereas for a 32-bit data register, the transfer size should be 4B). The
 * rationale of this constraint is that, on the peripheral side, the address
 * offset is set to zero, allowing to read/write data from/to the peripheral
 * in a single source read/destination write operation.
 *
 * @param dmaInstance The ADMA instance number.
 * @param dmaChannel ADMA channel number.
 * @param type Transfer type (M->M, P->M, M->P, P->P).
 * @param transferConfig Pointer to the transfer configuration structure.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS
 */
status_t ADMA_DRV_ConfigSingleBlockTransfer(const uint32_t dmaInstance, uint8_t dmaChannel, adma_transfer_type_t type,
                                            adma_transfer_config_t *transferConfig);

/**
 * @brief Starts an ADMA channel.
 *
 * This function enables the ADMA channel ADMA request.
 *
 * @param dmaInstance The ADMA instance number.
 * @param dmaChannel ADMA channel number.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t ADMA_DRV_StartChannel(const uint32_t dmaInstance, uint8_t dmaChannel);

/**
 * @brief Stops the ADMA channel.
 *
 * This function disables the ADMA channel ADMA request.
 *
 * @param dmaInstance The ADMA instance number.
 * @param dmaChannel ADMA channel number.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t ADMA_DRV_StopChannel(const uint32_t dmaInstance, uint8_t dmaChannel);

/**
 * @brief Resume the ADMA channel.
 *
 * This function resume the ADMA channel.
 *
 * @param dmaInstance The ADMA instance number.
 * @param dmaChannel ADMA channel number.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t ADMA_DRV_ResumeChannel(const uint32_t dmaInstance, uint8_t dmaChannel);

/**
 * @brief Suspend the ADMA channel.
 *
 * This function Suspend the ADMA channel.
 *
 * @param dmaInstance The ADMA instance number.
 * @param dmaChannel ADMA channel number.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t ADMA_DRV_SuspendChannel(const uint32_t dmaInstance, uint8_t dmaChannel);

/**
 * @brief Configures the source address for the ADMA channel.
 *
 * @param  dmaInstance The ADMA instance number.
 * @param  dmaChannel  ADMA channel number.
 * @param  address The pointer to the source memory address.
 */
void ADMA_DRV_SetSrcAddr(const uint32_t dmaInstance, uint8_t dmaChannel, uint32_t address);

/**
 * @brief  Configures the destination address for the ADMA channel.
 *
 * @param  dmaInstance The ADMA instance number.
 * @param  dmaChannel ADMA channel number.
 * @param  address The pointer to the destination memory address.
 */
void ADMA_DRV_SetDestAddr(const uint32_t dmaInstance, uint8_t dmaChannel, uint32_t address);

/**
 * @brief Disables/Enables the channel interrupt requests.
 *
 * @param  dmaInstance The ADMA instance number.
 * @param  dmaChannel ADMA channel number.
 * @param  intSrc Interrupt event (error/half major loop/complete major loop).
 * @param  enable Enable (true)/Disable (false) interrupts for the current channel.
 */
void ADMA_DRV_ConfigureInterrupt(const uint32_t dmaInstance, uint8_t dmaChannel, adma_channel_interrupt_t intSrc,
                                 bool enable);


/**
 * @brief Registers the callback function and the parameter for ADMA channel.
 *
 * This function registers the callback function and the parameter into the ADMA channel state structure.
 * The callback function is called when the channel is complete or a channel error occurs. The ADMA
 * driver passes the channel status to this callback function to indicate whether it is caused by the
 * channel complete event or the channel error event.
 *
 * To un-register the callback function, set the callback function to "NULL" and call this function.
 *
 * @param dmaInstance The ADMA instance number.
 * @param dmaChannel ADMA channel number.
 * @param callback The pointer to the callback function.
 * @param parameter The pointer to the callback function's parameter.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t ADMA_DRV_InstallCallback(const uint32_t dmaInstance, uint8_t dmaChannel, adma_callback_t callback,
                                  void *parameter);

/**
 * @brief  Gets the ADMA channel status.
 *
 * @param  dmaInstance The ADMA instance number.
 * @param  dmaChannel  ADMA channel number.
 * @return adma_chn_status_t Channel status.
 */
adma_chn_status_t ADMA_DRV_GetChannelStatus(const uint32_t dmaInstance, uint8_t dmaChannel);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group ADMA_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif