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
 * @file flexcan_driver.h
 * @brief FLEXCAN driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __FLEXCAN_DRIVER_H__
#define __FLEXCAN_DRIVER_H__

#include "emps_platform.h"
#include "osal.h"
#include "pdma_driver.h"

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup FLEXCAN_Driver FLEXCAN Driver
 * @{
 */

/**
 * @brief The type of the RxFIFO transfer (interrupts/PDMA).
 */
typedef enum {
    FLEXCAN_RXFIFO_USING_INTERRUPTS, /**< Use interrupts for RxFIFO. */
    FLEXCAN_RXFIFO_USING_POLLING,    /**< Use polling method for RxFIFO. */
    FLEXCAN_RXFIFO_USING_DMA         /**< Use PDMA for RxFIFO. */
} flexcan_rxfifo_transfer_type_t;

/**
 * @brief The type of the event which occurred when the callback was invoked.
 */
typedef enum {
    FLEXCAN_EVENT_RX_COMPLETE,               /**< A frame was received in the configured Rx MB. */
    FLEXCAN_EVENT_RXFIFO_COMPLETE,           /**< A frame was received in the Rx FIFO. */
    FLEXCAN_EVENT_RXFIFO_WARNING,            /**< Rx FIFO is almost full (5 frames). */
    FLEXCAN_EVENT_RXFIFO_OVERFLOW,           /**< Rx FIFO is full (incoming message was lost). */
    FLEXCAN_EVENT_TX_COMPLETE,               /**< A frame was sent from the configured Tx MB. */
    FLEXCAN_EVENT_ENHANCED_RXFIFO_COMPLETE,  /**< A frame was received in the Enhanced Rx FIFO. */
    FLEXCAN_EVENT_ENHANCED_RXFIFO_WATERMARK, /**< The number of messages available is greater
                                                than the watermark defined.*/
    FLEXCAN_EVENT_ENHANCED_RXFIFO_OVERFLOW,  /**< Enhanced Rx FIFO is full (incoming message was lost). */
    FLEXCAN_EVENT_ENHANCED_RXFIFO_UNDERFLOW, /**< An underflow condition occurred in the enhanced Rx FIFO. */
    FLEXCAN_EVENT_WAKEUP_TIMEOUT,            /**< An wake up event occurred due to timeout. */
    FLEXCAN_EVENT_WAKEUP_MATCH,              /**< An wake up event occurred due to matching. */
    FLEXCAN_EVENT_SELF_WAKEUP,               /**< A self wake up event occurred. */
    FLEXCAN_EVENT_DMA_COMPLETE,              /**< A complete transfer occurred on PDMA. */
    FLEXCAN_EVENT_DMA_ERROR,                 /**< A PDMA transfer fail, because of a PDMA channel error. */
    FLEXCAN_EVENT_BUSOFF,                    /**< FlexCAN module entered Bus Off state. */
    FLEXCAN_EVENT_RX_WARNING,                /**< The Rx error counter transitioned from less than 96 to greater than or equal to 96
                                 (interrupt mode only). */
    FLEXCAN_EVENT_TX_WARNING,                /**< The Tx error counter transitioned from less than 96 to greater than or equal to 96
                                 (interrupt mode only). */
    FLEXCAN_EVENT_ERROR

} flexcan_event_type_t;

/**
 * @brief The state of a given MB (idle/Rx busy/Tx busy).
 */
typedef enum {
    FLEXCAN_MB_IDLE,     /**< The MB is not used by any transfer. */
    FLEXCAN_MB_RX_BUSY,  /**< The MB is used for a reception. */
    FLEXCAN_MB_TX_BUSY,  /**< The MB is used for a transmission. */
    FLEXCAN_MB_DMA_ERROR /**< The MB is used as PDMA source and fail to transfer. */
} flexcan_mb_state_t;

/**
 * @brief FlexCAN Message Buffer ID type.
 */
typedef enum {
    FLEXCAN_MSG_ID_STD, /**< Standard ID. */
    FLEXCAN_MSG_ID_EXT  /**< Extended ID. */
} flexcan_msgbuff_id_type_t;

/**
 * @brief FlexCAN PE clock sources.
 */
typedef enum {
    FLEXCAN_CLK_SOURCE_OSC    = 0U, /**< The CAN engine clock source is the oscillator clock. */
    FLEXCAN_CLK_SOURCE_PERIPH = 1U  /**< The CAN engine clock source is the peripheral clock. */
} flexcan_clk_source_t;

/**
 * @brief FlexCAN message buffer structure.
 */
typedef struct {
    uint32_t cs;         /**< Code and Status. */
    uint32_t msgId;      /**< Message Buffer ID. */
    uint8_t data[64];    /**< Data bytes of the FlexCAN message. */
    uint8_t dataLen;     /**< Length of data in bytes. */
    uint8_t id_hit;      /**< Identifier Acceptance Filter Hit Indicator. */
    uint32_t time_stamp; /**< Free-Running Counter Time Stamp. */
} flexcan_msgbuff_t;

/**
 * @brief Information needed for internal handling of a given MB.
 */
typedef struct {
    flexcan_msgbuff_t *mb_message;     /**< The FlexCAN MB structure. */
    OS_SemaphoreId_t mbSema;           /**< Semaphore used for signaling completion of a blocking transfer. */
    volatile flexcan_mb_state_t state; /**< The state of the current MB (idle/Rx busy/Tx busy). */
    volatile bool isDmaBusy;           /**< The state of the current PDMA (idle/busy). */
    bool isPolling;                    /**< True if the transfer is Polling Mode.  */
    bool isBlocking;                   /**< True if the transfer is blocking. */
    bool isRemote;                     /**< True if the frame is a remote frame. */
    uint32_t time_stamp;               /**< TimeStamp of the Message. */
} flexcan_mb_handle_t;

/**
 * @brief Internal driver state information.
 *
 * @note The contents of this structure are internal to the driver and should not be
 *      modified by users. Also, contents of the structure are subject to change in
 *      future releases.
 */
typedef struct flexcan_state_t {
    flexcan_mb_handle_t mbs[FEATURE_FLEXCAN_MAX_MB_NUM]; /**< Array containing information related to each MB. */
    flexcan_mb_handle_t enhancedFifoOutput;              /**< Containing information output of Enhanced Rx FIFO. */
    void (*callback)(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx,
                     struct flexcan_state_t *driverState); /**< IRQ handler callback function. */
    void *callbackParam;                                   /**< Parameter used to pass user data when invoking the callback function. */
    void (*error_callback)(uint8_t instance, flexcan_event_type_t eventType,
                           struct flexcan_state_t *driverState); /**< Error IRQ handler callback function. */
    void *errorCallbackParam;                                    /**< Parameter used to pass user data when invoking the error callback function. */
    uint8_t rxFifoDMAChannel;                                    /**< PDMA channel number used for transfers. */
    uint32_t u32NumOfMbTransferByDMA;                            /**< The number of message buffers transferred by PDMA(major loop count). */

    flexcan_rxfifo_transfer_type_t transferType; /**< Type of RxFIFO transfer. */

    bool bIsLegacyFifoEn;   /**< This controls whether the Rx FIFO feature is enabled or not. */
    bool bIsEnhancedFifoEn; /**< This controls whether the Rx Enhanced FIFO feature is enabled or not. */
    uint32_t u32MaxMbNum;   /**< The maximum number of Message Buffers. */
    bool isIntActive;       /**< Save status of enabling/disabling interrupts in runtime. */
} flexcan_state_t;

/**
 * @brief FlexCAN data info from user.
 */
typedef struct {
    flexcan_msgbuff_id_type_t msg_id_type; /**< Type of message ID (standard or extended). */
    uint32_t data_length;                  /**< Length of Data in Bytes. */
    bool fd_enable;                        /**< Enable or disable FD. */
    uint8_t fd_padding;                    /**< Set a value for padding. It will be used when the data length code (DLC)
                                              specifies a bigger payload size than data_length to fill the MB. */
    bool enable_brs;                       /**< Enable bit rate switch inside a CAN FD format frame. */
    bool is_remote;                        /**< Specifies if the frame is standard or remote. */
} flexcan_data_info_t;

/**
 * @brief FlexCAN Rx FIFO filters number.
 */
typedef enum {
    FLEXCAN_RX_FIFO_ID_FILTERS_8   = 0x0, /**<   8 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_16  = 0x1, /**<  16 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_24  = 0x2, /**<  24 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_32  = 0x3, /**<  32 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_40  = 0x4, /**<  40 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_48  = 0x5, /**<  48 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_56  = 0x6, /**<  56 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_64  = 0x7, /**<  64 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_72  = 0x8, /**<  72 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_80  = 0x9, /**<  80 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_88  = 0xA, /**<  88 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_96  = 0xB, /**<  96 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_104 = 0xC, /**< 104 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_112 = 0xD, /**< 112 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_120 = 0xE, /**< 120 Rx FIFO Filters. */
    FLEXCAN_RX_FIFO_ID_FILTERS_128 = 0xF  /**< 128 Rx FIFO Filters. */
} flexcan_rx_fifo_id_filter_num_t;

/**
 * @brief FlexCAN Rx mask type.
 */
typedef enum {
    FLEXCAN_RX_MASK_GLOBAL,    /**< Rx global mask. */
    FLEXCAN_RX_MASK_INDIVIDUAL /**< Rx individual mask. */
} flexcan_rx_mask_type_t;

/**
 * @brief ID formats for Rx FIFO.
 */
typedef enum {
    FLEXCAN_RX_FIFO_ID_FORMAT_A, /**< One full ID (standard and extended) per ID Filter Table element. */
    FLEXCAN_RX_FIFO_ID_FORMAT_B, /**< Two full standard IDs or two partial 14-bit (standard and extended) IDs per ID
                                    Filter Table element. */
    FLEXCAN_RX_FIFO_ID_FORMAT_C, /**< Four partial 8-bit Standard IDs per ID Filter Table element. */
    FLEXCAN_RX_FIFO_ID_FORMAT_D  /**< All frames rejected. */
} flexcan_rx_fifo_id_element_format_t;

typedef enum {
    FLEXCAN_CAN_CLK_TIMESTAMP_SRC,   /**< Captured time base is CAN bit clock. */
    FLEXCAN_ONCHIP_CLK_TIMESTAMP_SRC /**< Captured time base is on-chip timer clock. */
} flexcan_timeStampClockConfigType_t;

typedef enum {
    FLEXCAN_MSGBUFFTIMESTAMP_TIMER, /**< Message buffer time stamp base is TIMER. */
    FLEXCAN_MSGBUFFTIMESTAMP_LOWER, /**< Message buffer time stamp base is lower 16 bits of high resolution timer. */
    FLEXCAN_MSGBUFFTIMESTAMP_UPPER  /**< Message buffer time stamp base is upper 16 bits of high resolution timer. */
} flexcan_msgBuffTimeStampType_t;

typedef enum {
    FLEXCAN_TIMESTAMPCAPTURE_DISABLE, /**< The high resolution time stamp capture is disabled. */
    FLEXCAN_TIMESTAMPCAPTURE_END,     /**< The high resolution time stamp is captured in the end of the CAN frame. */
    FLEXCAN_TIMESTAMPCAPTURE_START,   /**< The high resolution time stamp is captured in the start of the CAN frame. */
    FLEXCAN_TIMESTAMPCAPTURE_FD       /**< The high resolution time stamp is captured in the start of frame for classical CAN
                                   frames and in res bit for CAN FD frames. */
} Flexcan_TimeStampCaptureType;

/**
 * @brief FlexCAN Free Running Counter Time Stamp config.
 */
typedef struct {
    flexcan_timeStampClockConfigType_t
        timeStampSource;                                 /**< Timestamp Timer Source selection. \D{FLEXCAN_CAN_CLK_TIMESTAMP_SRC} */
    flexcan_msgBuffTimeStampType_t msgBuffTimeStampType; /**< This field selects which time base is used for capturing
                                                            the 16-bit TIME_STAMP field of the message buffer.
                                                            \D{FLEXCAN_MSGBUFFTIMESTAMP_TIMER} */
    Flexcan_TimeStampCaptureType hrConfigType;           /**< This field configures the point in time when a 32-bit time base is captured during a CAN frame and stored in the high resolution time stamp. \D{FLEXCAN_TIMESTAMPCAPTURE_DISABLE} */
} flexcan_timeStampConfigType_t;

/**
 * @brief FlexCAN Enhanced Rx FIFO filter type.
 */
typedef enum {
    FLEXCAN_ENHANCED_RX_FIFO_ONE_ID_FILTER = 0U, /**< Filter element with filter + mask scheme. */
    FLEXCAN_ENHANCED_RX_FIFO_RANGE_ID_FILTER,    /**< Filter element with range scheme. */
    FLEXCAN_ENHANCED_RX_FIFO_TWO_ID_FILTER       /**< Filter element with 2-filter scheme. */
} flexcan_enhancedFilterType_t;

/**
 * @brief FlexCAN Enhanced Rx FIFO ID filter table structure.
 */
typedef struct {
    flexcan_enhancedFilterType_t filterType; /**< Enhanced Rx FIFO filter type. */
    bool isExtendedFrame;                    /**< Extended frame. */
    bool rtr2;                               /**< Enhanced Rx FIFO RTR2. */
    bool rtr1;                               /**< Enhanced Rx FIFO RTR1. */
    uint32_t id2;                            /**< Enhanced Rx FIFO ID2 filter element. */
    uint32_t id1;                            /**< Enhanced Rx FIFO ID1 filter element. */
} flexcan_enhancedIdTableType_t;

/**
 * @brief FlexCAN Rx FIFO ID filter table structure.
 */
typedef struct {
    bool isRemoteFrame;   /**< Remote frame. */
    bool isExtendedFrame; /**< Extended frame. */
    uint32_t id;          /**< Rx FIFO ID filter element. */
} flexcan_id_table_t;

/**
 * @brief FlexCAN operation modes.
 */
typedef enum {
    FLEXCAN_NORMAL_MODE,      /**< Normal mode or user mode. */
    FLEXCAN_LISTEN_ONLY_MODE, /**< Listen-only mode. */
    FLEXCAN_LOOPBACK_MODE,    /**< Loop-back mode. */
    FLEXCAN_FREEZE_MODE,      /**< Freeze mode. */
    FLEXCAN_DISABLE_MODE      /**< Module disable mode. */
} flexcan_operation_modes_t;

/**
 * @brief FlexCAN payload sizes.
 */
typedef enum {
    FLEXCAN_PAYLOAD_SIZE_8 = 0, /**< FlexCAN message buffer payload size in bytes. */
    FLEXCAN_PAYLOAD_SIZE_16,    /**< FlexCAN message buffer payload size in bytes. */
    FLEXCAN_PAYLOAD_SIZE_32,    /**< FlexCAN message buffer payload size in bytes. */
    FLEXCAN_PAYLOAD_SIZE_64     /**< FlexCAN message buffer payload size in bytes. */
} flexcan_fd_payload_size_t;

typedef struct {
    flexcan_fd_payload_size_t blockR0;
    flexcan_fd_payload_size_t blockR1;
    flexcan_fd_payload_size_t blockR2;
    flexcan_fd_payload_size_t blockR3;
} flexcan_payload_block_size_t;
/**
 * @brief FlexCAN bitrate related structures.
 */
typedef struct {
    uint32_t propSeg;    /**< Propagation segment. \D{10UL} */
    uint32_t phaseSeg1;  /**< Phase segment 1. \D{13UL} */
    uint32_t phaseSeg2;  /**< Phase segment 2. \D{13UL} */
    uint32_t preDivider; /**< Clock prescaler division factor. \D{0UL} */
    uint32_t rJumpwidth; /**< Resync jump width. \D{3UL} */
} flexcan_time_segment_t;

/**
 * @brief FlexCAN configuration.
 */
typedef struct {
    uint32_t max_num_mb; /**< The maximum number of Message Buffers. \R{1,128UL} \R{1,16UL} \D{21UL} */
    /** The number of RX FIFO ID filters needed. \D{FLEXCAN_RX_FIFO_ID_FILTERS_8} */
    flexcan_rx_fifo_id_filter_num_t num_id_filters;
    bool is_rx_fifo_needed;               /**< 1 if needed; 0 if not. This controls whether the Rx FIFO feature is enabled or not.
                               \D{false} */
    uint32_t num_enhanced_std_id_filters; /**< The number of enhanced standard ID filter elements. \R{0,63UL} \D{2UL} */
    uint32_t
        num_enhanced_ext_id_filters;       /**< The number of enhanced extended ID filter elements. \R{0,127UL} \D{0UL} */
    uint32_t num_enhanced_watermark;       /**< The number of enhanced Rx FIFO watermark. \R{0,63UL} \D{0UL} */
    bool is_enhanced_rx_fifo_needed;       /**< 1 if needed; 0 if not. This controls whether the Enhanced Rx FIFO feature is
                                        enabled or not. \D{false} */
    flexcan_operation_modes_t flexcanMode; /**< User configurable FlexCAN operation modes. \D{FLEXCAN_NORMAL_MODE} */
    flexcan_payload_block_size_t
        payload;    /**< The payload size of the mailboxes specified in bytes. \D{FLEXCAN_PAYLOAD_SIZE_64} */
    bool fd_enable; /**< Enable/Disable the Flexible Data Rate feature. \D{true} */
    flexcan_clk_source_t
        pe_clock; /**< The clock source of the CAN Protocol Engine (PE). \D{FLEXCAN_CLK_SOURCE_PERIPH} */
    flexcan_time_segment_t
        bitrate;                        /**< The bitrate used for standard frames or for the arbitration phase of FD frames. */
    flexcan_time_segment_t bitrate_cbt; /**< The bitrate used for the data phase of FD frames. */
    flexcan_rxfifo_transfer_type_t
        transfer_type;        /**< Specifies if the Rx FIFO uses interrupts or PDMA. \D{FLEXCAN_RXFIFO_USING_INTERRUPTS} */
    uint8_t rxFifoDMAChannel; /**< Specifies the PDMA channel number to be used for PDMA transfers. \D{0U} */

    flexcan_timeStampConfigType_t time_stamp; /* config Free Running Counter Time Stamp */
} flexcan_user_config_t;

/**
 * @brief Pretended Networking ID filter.
 */
typedef struct {
    bool extendedId;  /**< Specifies if the ID is standard or extended. */
    bool remoteFrame; /**< Specifies if the frame is standard or remote. */
    uint32_t id;      /**< Specifies the ID value. */
} flexcan_pn_id_filter_t;

/**
 * @brief Pretended Networking payload filter.
 */
typedef struct {
    uint8_t dlcLow;       /**< Specifies the lower limit of the payload size. */
    uint8_t dlcHigh;      /**< Specifies the upper limit of the payload size. */
    uint8_t payload1[8U]; /**< Specifies the payload to be matched (for MATCH_EXACT), the lower limit
                              (for MATCH_GEQ and MATCH_RANGE) or the upper limit (for MATCH_LEQ). */
    uint8_t payload2[8U]; /**< Specifies the mask (for MATCH_EXACT) or the upper limit (for MATCH_RANGE). */
} flexcan_pn_payload_filter_t;

/**
 * @brief Pretended Networking filtering combinations.
 */
typedef enum {
    FLEXCAN_FILTER_ID,               /**< Message ID filtering only. */
    FLEXCAN_FILTER_ID_PAYLOAD,       /**< Message ID and payload filtering. */
    FLEXCAN_FILTER_ID_NTIMES,        /**< Message ID filtering occurring a specified number of times. */
    FLEXCAN_FILTER_ID_PAYLOAD_NTIMES /**< Message ID and payload filtering  occurring a specified number of times. */
} flexcan_pn_filter_combination_t;

/**
 * @brief Pretended Networking matching schemes.
 */
typedef enum {
    FLEXCAN_FILTER_MATCH_EXACT, /**< Match an exact target value. */
    FLEXCAN_FILTER_MATCH_GEQ,   /**< Match greater than or equal to a specified target value. */
    FLEXCAN_FILTER_MATCH_LEQ,   /**< Match less than or equal to a specified target value. */
    FLEXCAN_FILTER_MATCH_RANGE  /**< Match inside a range, greater than or equal to a specified lower limit and smaller
                                   than or  equal to a specified upper limit. */
} flexcan_pn_filter_selection_t;

/**
 * @brief Pretended Networking configuration structure.
 */
typedef struct {
    bool wakeUpTimeout;                         /**< Specifies if an wake up event is triggered on timeout. */
    bool wakeUpMatch;                           /**< Specifies if an wake up event is triggered on match. */
    uint16_t numMatches;                        /**< The number of matches needed before generating an wake up event. */
    uint16_t matchTimeout;                      /**< Defines a timeout value that generates an wake up event if wakeUpTimeout is true. */
    flexcan_pn_filter_combination_t filterComb; /**< Defines the filtering scheme used. */
    flexcan_pn_id_filter_t
        idFilter1;                                   /**< The configuration of the first ID filter (match exact / lower limit / upper limit). */
    flexcan_pn_id_filter_t idFilter2;                /**< The configuration of the second ID filter (mask / upper limit). */
    flexcan_pn_filter_selection_t idFilterType;      /**< Defines the ID filtering scheme. */
    flexcan_pn_filter_selection_t payloadFilterType; /**< Defines the payload filtering scheme. */
    flexcan_pn_payload_filter_t payloadFilter;       /**< The configuration of the payload filter. */
} flexcan_pn_config_t;

/**
 * @brief FlexCAN Driver callback function type.
 */
typedef void (*flexcan_callback_t)(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx,
                                   flexcan_state_t *flexcanState);

/**
 * @brief FlexCAN Driver error callback function type.
 */
typedef void (*flexcan_error_callback_t)(uint8_t instance, flexcan_event_type_t eventType,
                                         flexcan_state_t *flexcanState);

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Sets the FlexCAN bit rate for standard frames or the arbitration phase of FD frames.
 *
 * @param   instance    A FlexCAN instance number.
 * @param   bitrate     A pointer to the FlexCAN bit rate settings.
 */
void FLEXCAN_DRV_SetBitrate(uint8_t instance, const flexcan_time_segment_t *bitrate);

/**
 * @brief Sets the FlexCAN bit rate for the data phase of FD frames (BRS enabled).
 *
 * @param   instance    A FlexCAN instance number.
 * @param   bitrate     A pointer to the FlexCAN bit rate settings.
 */
void FLEXCAN_DRV_SetBitrateCbt(uint8_t instance, const flexcan_time_segment_t *bitrate);

/**
 * @brief Gets the FlexCAN bit rate for standard frames or the arbitration phase of FD frames.
 *
 * @param   instance    A FlexCAN instance number.
 * @param   bitrate     A pointer to a variable for returning the FlexCAN bit rate settings.
 */
void FLEXCAN_DRV_GetBitrate(uint8_t instance, flexcan_time_segment_t *bitrate);

/**
 * @brief Gets the FlexCAN bit rate for the data phase of FD frames (BRS enabled).
 *
 * @param   instance    A FlexCAN instance number.
 * @param   bitrate     A pointer to a variable for returning the FlexCAN bit rate settings.
 */
void FLEXCAN_DRV_GetBitrateFD(uint8_t instance, flexcan_time_segment_t *bitrate);

/**
 * @brief Sets the Rx masking type.
 *
 * @param   instance     A FlexCAN instance number.
 * @param   type         The FlexCAN RX mask type.
 */
void FLEXCAN_DRV_SetRxMaskType(uint8_t instance, flexcan_rx_mask_type_t type);

/**
 * @brief Sets the FlexCAN Rx FIFO global mask (standard or extended).
 * This mask is applied to all filters ID regardless the ID Filter format.
 *
 * @param   instance    A FlexCAN instance number.
 * @param   id_type     Standard ID or extended ID mask type.
 * @param   mask        Mask Value. In FIFO mode, when using ID Format A or B,
                        bit 31 encodes RTR check and bit 30 encodes IDE check respectively.
                        For ID Format C, bits 31 and 30 are ignored.
 */
void FLEXCAN_DRV_SetRxFifoGlobalMask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint32_t mask);

/**
 * @brief Sets the FlexCAN Rx MB global mask (standard or extended).
 *
 * @param   instance    A FlexCAN instance number.
 * @param   id_type     Standard ID or extended ID.
 * @param   mask        Mask value.
 */
void FLEXCAN_DRV_SetRxMbGlobalMask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint32_t mask);

/**
 * @brief Sets the FlexCAN Rx MB 14 mask (standard or extended).
 *
 * @param   instance    A FlexCAN instance number.
 * @param   id_type     Standard ID or extended ID.
 * @param   mask        Mask value.
 */
void FLEXCAN_DRV_SetRxMb14Mask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint32_t mask);

/**
 * @brief Sets the FlexCAN Rx MB 15 mask (standard or extended).
 *
 * @param   instance    A FlexCAN instance number.
 * @param   id_type     Standard ID or extended ID.
 * @param   mask        Mask value.
 */
void FLEXCAN_DRV_SetRxMb15Mask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint32_t mask);

/**
 * @brief  Sets the FlexCAN Rx individual mask (standard or extended).
 *
 * @param  instance A FlexCAN instance number.
 * @param  id_type A standard ID or an extended ID.
 * @param  mb_idx Index of the message buffer.
 * @param  mask Mask Value. In FIFO mode, when using ID Format A or B,
                bit 31 encodes RTR check and bit 30 encodes IDE check respectively.
                For ID Format C, bits 31 and 30 are ignored.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The index of the message buffer is invalid.
 */
status_t FLEXCAN_DRV_SetRxIndividualMask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint8_t mb_idx,
                                         uint32_t mask);

/**
 * @brief Gets the default configuration structure.
 *
 * This function gets the default configuration structure, with the following settings:
 * - 16 message buffers.
 * - flexible data rate disabled.
 * - Rx FIFO disabled.
 * - normal operation mode.
 * - 8 byte payload size.
 * - Protocol Engine clock = Oscillator clock.
 * - bitrate of 500K bit/s (computed for sample point = 87.5).
 *
 * @param  config The configuration structure.
 * @return uint32_t The bitrate for generated configuration structure.
 */
uint32_t FLEXCAN_DRV_GetDefaultConfig(flexcan_user_config_t *config);

/**
 * @brief Initializes the FlexCAN peripheral.
 *
 * @param  instance A FlexCAN instance number.
 * @param  state Pointer to the FlexCAN driver state structure.
 * @param  data The FlexCAN platform data.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The index of a message buffer is invalid.
 * STATUS_ERROR: Other error occurred.
 */
status_t FLEXCAN_DRV_Init(uint8_t instance, flexcan_state_t *state, const flexcan_user_config_t *data);

/**
 * @brief Shuts down a FlexCAN instance.
 *
 * @param  instance A FlexCAN instance number.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_ERROR: Failed.
 */
status_t FLEXCAN_DRV_Deinit(uint8_t instance);

/**
 * @brief Enables/Disables the Transceiver Delay Compensation feature and sets
 * the Transceiver Delay Compensation Offset (offset value to be added to the
 * measured transceiver's loop delay in order to define the position of the
 * delayed comparison point when bit rate switching is active).
 *
 * @param   instance A FlexCAN instance number.
 * @param   enable Enable/Disable Transceiver Delay Compensation
 * @param   offset Transceiver Delay Compensation Offset
 */
void FLEXCAN_DRV_SetTDCOffset(uint8_t instance, bool enable, uint8_t offset);

/**
 * @brief Gets the value of the Transceiver Delay Compensation.
 *
 * @param  instance The FlexCAN base address.
 * @return uint8_t  The value of the transceiver loop delay measured from the transmitted
 *                  EDL to R0 transition edge to the respective received one added to the TDCOFF
 *                  value specified by FLEXCAN_HAL_SetTDCOffset.
 */
uint8_t FLEXCAN_DRV_GetTDCValue(uint8_t instance);

/**
 * @brief Gets the value of the TDC Fail flag.
 *
 * @param  instance The FlexCAN base address.
 * @return true: Indicates that the TDC mechanism is out of range, unable to compensate the transceiver's loop delay and
 *               successfully compare the delayed received bits to the transmitted ones.
 * @return false: Indicates that the TDC mechanism is not out of range.
 */
bool FLEXCAN_DRV_GetTDCFail(uint8_t instance);

/**
 * @brief Clears the TDC Fail flag.
 *
 * @param  instance The FlexCAN base address.
 */
void FLEXCAN_DRV_ClearTDCFail(uint8_t instance);

/**
 * @brief FlexCAN transmit message buffer field configuration.
 *
 * @param  instance A FlexCAN instance number.
 * @param  mb_idx Index of the message buffer.
 * @param  tx_info Data info.
 * @param  msg_id ID of the message to transmit.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The index of the message buffer is invalid.
 */
status_t FLEXCAN_DRV_ConfigTxMb(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info, uint32_t msg_id);

/**
 * @brief Configures a transmit message buffer for remote frame response.
 *
 * @param  instance A FlexCAN instance number.
 * @param  mb_idx Index of the message buffer.
 * @param  tx_info Data info.
 * @param  msg_id ID of the message to transmit.
 * @param  mb_data Bytes of the FlexCAN message.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The index of the message buffer is invalid.
 */
status_t FLEXCAN_DRV_ConfigRemoteResponseMb(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info,
                                            uint32_t msg_id, const uint8_t *mb_data);

/**
 * @brief Sends a CAN frame using the specified message buffer, in a blocking manner.
 *
 * This function sends a CAN frame using a configured message buffer. The function
 * blocks until either the frame was sent, or the specified timeout expired.
 *
 * @param  instance A FlexCAN instance number.
 * @param  mb_idx Index of the message buffer.
 * @param  tx_info Data info.
 * @param  msg_id ID of the message to transmit.
 * @param  mb_data Bytes of the FlexCAN message.
 * @param  timeout_ms A timeout for the transfer in milliseconds.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The index of a message buffer is invalid.
 * STATUS_BUSY: A resource is busy.
 * STATUS_TIMEOUT: The timeout is reached.
 */
status_t FLEXCAN_DRV_SendBlocking(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info, uint32_t msg_id,
                                  const uint8_t *mb_data, uint32_t timeout_ms);

/**
 * @brief Sends a CAN frame using the specified message buffer.
 *
 * This function sends a CAN frame using a configured message buffer. The function
 * returns immediately. If a callback is installed, it will be invoked after
 * the frame was sent.
 *
 * @param   instance   A FlexCAN instance number.
 * @param   mb_idx     Index of the message buffer.
 * @param   tx_info    Data info.
 * @param   msg_id     ID of the message to transmit.
 * @param   mb_data    Bytes of the FlexCAN message.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The index of a message buffer is invalid.
 * STATUS_BUSY: A resource is busy.
 */
status_t FLEXCAN_DRV_Send(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info, uint32_t msg_id,
                          const uint8_t *mb_data);

/**
 * @brief FlexCAN receive message buffer field configuration.
 *
 * @param   instance                   A FlexCAN instance number.
 * @param   mb_idx                     Index of the message buffer.
 * @param   rx_info                    Data info.
 * @param   msg_id                     ID of the message to transmit.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The index of a message buffer is invalid.
 */
status_t FLEXCAN_DRV_ConfigRxMb(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *rx_info, uint32_t msg_id);

/**
 * @brief FlexCAN Rx FIFO field configuration.
 *
 * @note The number of elements in the ID filter table is defined by the
 *       following formula:
 *       - for format A: The number of Rx FIFO ID filters.
 *       - for format B: Twice the number of Rx FIFO ID filters.
 *       - for format C: Four times the number of Rx FIFO ID filters.
 *       The user must provide the exact number of elements in order to avoid
 *       any misconfiguration.
 *
 *       Each element in the ID filter table specifies an ID to be used as
 *       acceptance criteria for the FIFO as follows:
 *       - for format A: In the standard frame format, bits 10 to 0 of the ID
 *         are used for frame identification. In the extended frame format, bits
 *         28 to 0 are used.
 *       - for format B: In the standard frame format, bits 10 to 0 of the ID
 *         are used for frame identification. In the extended frame format, only
 *         the 14 most significant bits (28 to 15) of the ID are compared to the
 *         14 most significant bits (28 to 15) of the received ID.
 *       - for format C: In both standard and extended frame formats, only the 8
 *         most significant bits (7 to 0 for standard, 28 to 21 for extended) of
 *         the ID are compared to the 8 most significant bits (7 to 0 for
 *         standard, 28 to 21 for extended) of the received ID.
 *
 * @param   instance           A FlexCAN instance number.
 * @param   id_format          The format of the Rx FIFO ID Filter Table Elements.
 * @param   id_filter_table    The ID filter table elements which contain RTR
 *                             bit, IDE bit, and Rx message ID.
 */
void FLEXCAN_DRV_ConfigRxFifo(uint8_t instance, flexcan_rx_fifo_id_element_format_t id_format,
                              const flexcan_id_table_t *id_filter_table);

/**
 * @param   instance           A FlexCAN instance number.
 * @param   id_filter_table    The ID filter table elements which contain RTR
 *                             bit, IDE bit, and Rx message ID.
 */
void FLEXCAN_DRV_ConfigEnhancedRxFifo(uint8_t instance, const flexcan_enhancedIdTableType_t *id_filter_table);

/**
 * @brief Receives a CAN frame using the specified message buffer, in a blocking manner.
 *
 * This function receives a CAN frame using a configured message buffer. The function
 * blocks until either a frame was received, or the specified timeout expired.
 *
 * @param   instance   A FlexCAN instance number.
 * @param   mb_idx     Index of the message buffer.
 * @param   data       The FlexCAN receive message buffer data.
 * @param   timeout_ms A timeout for the transfer in milliseconds.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The index of a message buffer is invalid.
 * STATUS_BUSY: A resource is busy.
 * STATUS_TIMEOUT: The timeout is reached.
 */
status_t FLEXCAN_DRV_ReceiveBlocking(uint8_t instance, uint8_t mb_idx, flexcan_msgbuff_t *data, uint32_t timeout_ms);

/**
 * @brief Receives a CAN frame using the specified message buffer.
 *
 * This function receives a CAN frame using a configured message buffer. The function
 * returns immediately. If a callback is installed, it will be invoked after
 * the frame was received and read into the specified buffer.
 *
 * @param   instance   A FlexCAN instance number.
 * @param   mb_idx     Index of the message buffer.
 * @param   data       The FlexCAN receive message buffer data.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The index of a message buffer is invalid.
 * STATUS_BUSY: A resource is busy.
 */
status_t FLEXCAN_DRV_Receive(uint8_t instance, uint8_t mb_idx, flexcan_msgbuff_t *data);

/**
 * @brief Receives a CAN frame using the message FIFO, in a blocking manner.
 *
 * This function receives a CAN frame using the Rx FIFO. The function blocks until
 * either a frame was received, or the specified timeout expired.
 *
 * @param   instance    A FlexCAN instance number.
 * @param   data        The FlexCAN receive message buffer data.
 * @param   timeout_ms  A timeout for the transfer in milliseconds.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_BUSY: A resource is busy.
 * STATUS_TIMEOUT: The timeout is reached.;
 * STATUS_ERROR: Other error occurred.
 */
status_t FLEXCAN_DRV_RxFifoBlocking(uint8_t instance, flexcan_msgbuff_t *data, uint32_t timeout_ms);

/**
 * @brief Receives a CAN frame using the message FIFO.
 *
 * This function receives a CAN frame using the Rx FIFO. The function returns
 * immediately. If a callback is installed, it will be invoked after the frame
 * was received and read into the specified buffer.
 *
 * @param   instance    A FlexCAN instance number.
 * @param   data        The FlexCAN receive message buffer data.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_BUSY: A resource is busy.
 * STATUS_ERROR: Other error occurred.
 */
status_t FLEXCAN_DRV_RxFifo(uint8_t instance, flexcan_msgbuff_t *data);

/**
 * @brief Ends a non-blocking FlexCAN transfer early.
 *
 * @param   instance   A FlexCAN instance number.
 * @param   mb_idx     The index of the message buffer.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_CAN_NO_TRANSFER_IN_PROGRESS: No transfer was running.
 */
status_t FLEXCAN_DRV_AbortTransfer(uint8_t instance, uint8_t mb_idx);

/**
 * @brief Returns whether the previous FlexCAN transfer has finished.
 *
 * When performing an async transfer, call this function to ascertain the state of the
 * current transfer: in progress (or busy) or complete (success).
 *
 * @param instance The FlexCAN instance number.
 * @param mb_idx The index of the message buffer.
 * @return status_t
 * STATUS_SUCCESS: Successful.
 * STATUS_BUSY: A resource is busy.
 * STATUS_ERROR: In case of a PDMA error transfer.
 */
status_t FLEXCAN_DRV_GetTransferStatus(uint8_t instance, uint8_t mb_idx);

/**
 * @brief Returns reported error conditions.
 *
 * Reports various error conditions detected in the reception and transmission of a CAN frame
 * and some general status of the device.
 *
 * @param instance The FlexCAN instance number.
 * @return uint32_t Value of the Error and Status 1 register.
 */
uint32_t FLEXCAN_DRV_GetErrorStatus(uint8_t instance);

/**
 * @brief Installs a callback function for the IRQ handler.
 *
 * @param instance The FlexCAN instance number.
 * @param callback The callback function.
 * @param callbackParam User parameter passed to the callback function through the state parameter.
 */
void FLEXCAN_DRV_InstallEventCallback(uint8_t instance, flexcan_callback_t callback, void *callbackParam);

/**
 * @brief Installs an error callback function for the IRQ handler and enables error interrupts.
 *
 * @param instance The FlexCAN instance number.
 * @param callback The error callback function.
 * @param callbackParam User parameter passed to the error callback function through the state parameter.
 */
void FLEXCAN_DRV_InstallErrorCallback(uint8_t instance, flexcan_error_callback_t callback, void *callbackParam);

/**
 * @brief Get time stamp of MB.
 *
 * @param instance The FlexCAN instance number.
 * @param mb_idx The index of the message buffer.
 * @return uint32_t Value of the time stamp.
 */
uint32_t FLEXCAN_DRV_GetMsgBuffTimestamp(uint8_t instance, uint32_t mb_idx);

/**
 * @brief Configures Pretended Networking settings.
 *
 * @param instance The FlexCAN instance number.
 * @param enable Enable/Disable Pretended Networking mode.
 * @param pnConfig Pointer to the Pretended Networking configuration structure.
 */
void FLEXCAN_DRV_ConfigPN(uint8_t instance, bool enable, const flexcan_pn_config_t *pnConfig);

/**
 * @brief Extracts one of the frames which triggered the wake up event.
 *
 * @param   instance    The FlexCAN instance number.
 * @param   wmbIndex  The index of the message buffer to be extracted.
 * @param   wmb  Pointer to the message buffer structure where the frame will be saved.
 */
void FLEXCAN_DRV_GetWMB(uint8_t instance, uint8_t wmbIndex, flexcan_msgbuff_t *wmb);

/**
 * @brief Set can enter/exit stop mode according TCSR.
 *
 * @param instance The FlexCAN instance number.
 * @param en Enter/exit stop mode according TCSR.
 */
void FLEXCAN_DRV_TcsrStopZoneMode(uint8_t instance, bool en);

/**
 * @brief Check stop ack after set the stop mode according the TCSR
 * 
 * @param  instance The FlexCAN instance number.
 * @return true 
 * @return false 
 */
bool FLEXCAN_DRV_CheckTcsrStopACK(uint8_t instance);

/**
 * @brief Gets the frame data of the MB.
 * 
 * @param  instance The FlexCAN instance number.
 * @param  mb_idx The index of the message buffer.
 * @param  data The user buffer to store frame data
 * @return status_t
 * STATUS_SUCCESS: Successful
 * STATUS_ERROR: The MB is not full
 * STATUS_BUSY: The MB is not idle
 * STATUS_CAN_BUFF_OUT_OF_RANGE: The MB is out of range or occupied by RxFIFO
 */
status_t FLEXCAN_DRV_RxMessageBuffer(uint8_t instance, uint32_t mb_idx, flexcan_msgbuff_t *data);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
/* end of group FLEXCAN_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __FLEXCAN_DRIVER_H__ */
