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
 * @file flexcan_hw_access.h
 * @brief FLEXCAN driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __FLEXCAN_HW_ACCESS_H__
#define __FLEXCAN_HW_ACCESS_H__

#include "emps_platform.h"
#include "flexcan_driver.h"
#include "flexcan_register.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief FlexCAN message buffer CODE for Rx buffers
 */
enum {
    FLEXCAN_RX_INACTIVE = 0x0, /**< MB is not active.*/
    FLEXCAN_RX_BUSY     = 0x1, /**< FlexCAN is updating the contents of the MB.*/
                               /**<  The CPU must not access the MB.*/
    FLEXCAN_RX_FULL    = 0x2,  /**< MB is full.*/
    FLEXCAN_RX_EMPTY   = 0x4,  /**< MB is active and empty.*/
    FLEXCAN_RX_OVERRUN = 0x6,  /**< MB is overwritten into a full buffer.*/
    FLEXCAN_RX_RANSWER = 0xA,  /**< A frame was configured to recognize a Remote Request Frame*/
                               /**<  and transmit a Response Frame in return.*/
    FLEXCAN_RX_NOT_USED = 0xF  /**< Not used*/
};

/**
 * @brief FlexCAN message buffer CODE FOR Tx buffers
 */
enum {
    FLEXCAN_TX_INACTIVE = 0x08, /**< MB is not active.*/
    FLEXCAN_TX_ABORT    = 0x09, /**< MB is aborted.*/
    FLEXCAN_TX_DATA     = 0x0C, /**< MB is a TX Data Frame(MB RTR must be 0).*/
    FLEXCAN_TX_REMOTE   = 0x1C, /**< MB is a TX Remote Request Frame (MB RTR must be 1).*/
    FLEXCAN_TX_TANSWER  = 0x0E, /**< MB is a TX Response Request Frame from.*/
                                /**<  an incoming Remote Request Frame.*/
    FLEXCAN_TX_NOT_USED = 0xF   /**< Not used*/
};

/**
 * @brief FlexCAN message buffer transmission types
 */
enum {
    FLEXCAN_MB_STATUS_TYPE_TX,          /**< Transmit MB*/
    FLEXCAN_MB_STATUS_TYPE_TX_REMOTE,   /**< Transmit remote request MB*/
    FLEXCAN_MB_STATUS_TYPE_RX,          /**< Receive MB*/
    FLEXCAN_MB_STATUS_TYPE_RX_REMOTE,   /**< Receive remote request MB*/
    FLEXCAN_MB_STATUS_TYPE_RX_TX_REMOTE /**< FlexCAN remote frame receives remote request and*/
                                        /**<  transmits MB.*/
};

/**
 * @brief FlexCAN error interrupt types
 */
typedef enum {
    FLEXCAN_INT_RX_WARNING = CAN_CTRL1_RWRNMSK_MASK, /**< RX warning interrupt*/
    FLEXCAN_INT_TX_WARNING = CAN_CTRL1_TWRNMSK_MASK, /**< TX warning interrupt*/
    FLEXCAN_INT_ERR        = CAN_CTRL1_ERRMSK_MASK,  /**< Error interrupt*/
    FLEXCAN_INT_BUSOFF     = CAN_CTRL1_BOFFMSK_MASK, /**< Bus off interrupt*/
} flexcan_int_type_t;

/**
 * @brief FlexCAN Message Buffer code and status for transmit and receive
 */
typedef struct {
    uint32_t code;                       /**< MB code for TX or RX buffers.*/
                                         /**< Defined by flexcan_mb_code_rx_t and flexcan_mb_code_tx_t */
    flexcan_msgbuff_id_type_t msgIdType; /**< Type of message ID (standard or extended)*/
    uint32_t dataLen;                    /**< Length of Data in Bytes*/
    bool fd_enable;
    uint8_t fd_padding;
    bool enable_brs; /**< Enable bit rate switch*/
} flexcan_msgbuff_code_status_t;

#define FLEXCAN_ID_EXT_MASK  0x3FFFFU
#define FLEXCAN_ID_EXT_SHIFT 0
#define FLEXCAN_ID_EXT_WIDTH 18

#define FLEXCAN_ID_STD_MASK  0x1FFC0000U
#define FLEXCAN_ID_STD_SHIFT 18
#define FLEXCAN_ID_STD_WIDTH 11

#define FLEXCAN_ID_PRIO_MASK  0xE0000000U
#define FLEXCAN_ID_PRIO_SHIFT 29
#define FLEXCAN_ID_PRIO_WIDTH 3
/** CS Bit Fields */
#define FLEXCAN_CS_TIME_STAMP_MASK  0xFFFFU
#define FLEXCAN_CS_TIME_STAMP_SHIFT 0
#define FLEXCAN_CS_TIME_STAMP_WIDTH 16

#define FLEXCAN_CS_DLC_MASK  0xF0000U
#define FLEXCAN_CS_DLC_SHIFT 16
#define FLEXCAN_CS_DLC_WIDTH 4

#define FLEXCAN_CS_RTR_MASK  0x100000U
#define FLEXCAN_CS_RTR_SHIFT 20
#define FLEXCAN_CS_RTR_WIDTH 1

#define FLEXCAN_CS_IDE_MASK  0x200000U
#define FLEXCAN_CS_IDE_SHIFT 21
#define FLEXCAN_CS_IDE_WIDTH 1

#define FLEXCAN_CS_SRR_MASK  0x400000U
#define FLEXCAN_CS_SRR_SHIFT 22
#define FLEXCAN_CS_SRR_WIDTH 1

#define FLEXCAN_CS_CODE_MASK  0xF000000U
#define FLEXCAN_CS_CODE_SHIFT 24
#define FLEXCAN_CS_CODE_WIDTH 4

#define FLEXCAN_MB_EDL_MASK 0x80000000U
#define FLEXCAN_MB_BRS_MASK 0x40000000U

#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_RTR_SHIFT (31U)       /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format A&B RTR mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_IDE_SHIFT (30U)       /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format A&B IDE mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_RTR_SHIFT (15U)        /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B RTR-2 mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_IDE_SHIFT (14U)        /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B IDE-2 mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_MASK (0x3FFFFFFFU) /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format A extended mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_SHIFT (1U)         /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format A extended shift.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_MASK (0x3FF80000U) /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format A standard mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_SHIFT (19U)        /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format A standard shift.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK (0x1FFF8000U) /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B extended mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK1 (0x3FFFU)    /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B extended mask1.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1 (16U)       /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B extended shift 1.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT2 (0U)        /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B extended shift 2.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK (0x7FFU)      /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B standard mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT1 (19U)       /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B standard shift1.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT2 (3U)        /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B standard shift2.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_CMP_SHIFT (15U)    /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format B extended compare shift.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_MASK (0x1FE00000U) /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format C mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_MASK (0x7F8U)      /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format C mask.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1 (24U)           /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format C shift1.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2 (16U)           /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format C shift2.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3 (8U)            /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format C shift3.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT4 (0U)            /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format C shift4.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_CMP_SHIFT (21U)    /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format C extended compare shift.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_CMP_SHIFT (3U)     /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format C standard compare shift.*/
#define FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK (0xFFU)           /**< FlexCAN RX FIFO ID filter*/
                                                                 /**< format C mask.*/

/* @brief Frames available in Rx FIFO flag shift */
#define FLEXCAN_RXFIFO_FRAME_AVAILABLE_SHIFT (5U)
/* @brief Rx FIFO warning flag shift */
#define FLEXCAN_RXFIFO_WARNING_SHIFT (6U)
/* @brief Rx FIFO overflow flag shift */
#define FLEXCAN_RXFIFO_OVERFLOW_SHIFT (7U)

/* @brief enhanced idhit mask, shift and width */
#define FLEXCAN_ENHANCED_IDHIT_MASK  (0x7FU)
#define FLEXCAN_ENHANCED_IDHIT_SHIFT (0U)
#define FLEXCAN_ENHANCED_IDHIT_WIDTH (7U)

#define FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_FSCH_SHIFT (30U) /**< FlexCAN Enhanced RX FIFO ID filter*/
/** Standard & Extended FSCH shift.*/
#define FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_RTR2_SHIFT (27U) /**< FlexCAN Enhanced RX FIFO ID filter*/
/** Standard RTR-2 shift.*/
#define FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_RTR1_SHIFT (11U) /**< FlexCAN Enhanced RX FIFO ID filter*/
/** Standard RTR-1 shift.*/
#define FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_EXT_RTR_SHIFT (29U) /**< FlexCAN Enhanced RX FIFO ID filter*/
/** Extended RTR shift.*/
#define FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_SHIFT2 (16U) /**< FlexCAN Enhanced RX FIFO ID filter*/
/** Standard ID-2 shift.*/
#define FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_SHIFT1 (0U) /**< FlexCAN Enhanced RX FIFO ID filter*/
/** Standard ID-1 shift.*/
#define FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_MASK (0x7FFU) /**< FlexCAN Enhanced RX FIFO ID filter*/
/** Standard ID mask.*/
#define FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_EXT_SHIFT (0U) /**< FlexCAN Enhanced RX FIFO ID filter*/
/** Extended ID shift.*/
#define FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_EXT_MASK (0x1FFFFFFFU) /**< FlexCAN Enhanced RX FIFO ID filter*/
/** Mask for enable all enhanced interrupts */
#define FLEXCAN_ENHACED_RX_FIFO_ALL_INTERRUPT_MASK \
    (CAN_ERFIER_ERFUFWIE_MASK | CAN_ERFIER_ERFOVFIE_MASK | CAN_ERFIER_ERFWMIIE_MASK | CAN_ERFIER_ERFDAIE_MASK)

/** Mask for disable all enhanced interrupts */
#define FLEXCAN_ENHACED_RX_FIFO_NO_INTERRUPT_MASK (0U)

/** @brief Frames available in Enhanced Rx FIFO flag shift */
#define FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE (28U)
/** @brief Enhanced Rx FIFO Watermark Indication flag shift */
#define FLEXCAN_ENHANCED_RXFIFO_WATERMARK (29U)
/** @brief Enhanced Rx FIFO Overflow  flag shift */
#define FLEXCAN_ENHANCED_RXFIFO_OVERFLOW (30U)
/** @brief Enhanced Rx FIFO Underflow flag shift */
#define FLEXCAN_ENHANCED_RXFIFO_UNDERFLOW (31U)

#define REV_BYTES_32(a, b) \
    (b = ((a & 0xFF000000U) >> 24U) | ((a & 0xFF0000U) >> 8U) | ((a & 0xFF00U) << 8U) | ((a & 0xFFU) << 24U))

/**
 * @brief FlexCAN endianness handling
 */
#ifdef CORE_BIG_ENDIAN
#define FlexcanSwapBytesInWordIndex(index) (index)
#define FlexcanSwapBytesInWord(a, b)       (b = a)
#elif defined CORE_LITTLE_ENDIAN
#define FlexcanSwapBytesInWordIndex(index) (((index) & ~3U) + (3U - ((index)&3U)))
#define FlexcanSwapBytesInWord(a, b)       REV_BYTES_32(a, b)
#else
#error "No endianness defined!"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Get the CAN instance according to the CAN's base address.
 *
 * @param  base The FlexCAN base address
 * @return uint8_t The CAN instance;
 */
static inline uint8_t FLEXCAN_Base2Instance(const can_type_t *base)
{
    uint8_t i;
    can_type_t *const flexcanBase[] = CAN_BASE_PTRS;
    for (i = 0; i < CAN_INSTANCE_COUNT; i++) {
        if (base == flexcanBase[i])
            break;
    }

    OS_ASSERT(i < CAN_INSTANCE_COUNT);

    return i;
}

/**
 * @brief Get The Max no of MBs allowed on CAN instance.
 *
 * @param   base    The FlexCAN base address
 * @return uint32_t The Max No of MBs on the CAN instance;
 */
uint32_t FLEXCAN_GetMaxMbNum(const can_type_t *base);

/**
 *  @brief Calculate the Individual/Global Mask as format ID type in FIFO Mode.
 *
 *  @param id_type  A standard ID or an extended ID
 *  @param formatType Filter Table Format
 *  @param mask Mask ID aligned LSB
 *  @return uint32_t Mask calculated according to format type
 */
uint32_t FLEXCAN_GetRxFifoMask(flexcan_msgbuff_id_type_t id_type, flexcan_rx_fifo_id_element_format_t formatType,
                               uint32_t mask);

/**
 * @brief Enables FlexCAN controller.
 *
 * @param   base    The FlexCAN base address
 */
void FLEXCAN_Enable(can_type_t *base);

/**
 * @brief Disables FlexCAN controller.
 *
 * @param   base    The FlexCAN base address
 */
void FLEXCAN_Disable(can_type_t *base);

/**
 * @brief Return last Message Buffer Occupied By RxFIFO
 *
 * @param   x    Number of Configured RxFIFO Filters
 * @return uint32_t number of last MB occupied by RxFIFO
 */
static inline uint32_t RxFifoOccupiedLastMsgBuff(uint32_t x)
{
    return (5U + ((((x) + 1U) * 8U) / 4U));
}

/**
 * @brief Checks if the FlexCAN is enabled.
 *
 * @param   base    The FlexCAN base address
 * @return  true if enabled; false if disabled
 */
static inline bool FLEXCAN_IsEnabled(const can_type_t *base)
{
    return ((((base->MCR & CAN_MCR_MDIS_MASK) >> CAN_MCR_MDIS_SHIFT) != 0U) ? false : true);
}

/**
 * @brief Selects the clock source for FlexCAN.
 *
 * @param   base The FlexCAN base address
 * @param   clk         The FlexCAN clock source
 */
static inline void FLEXCAN_SelectClock(can_type_t *base, flexcan_clk_source_t clk)
{
    base->CTRL1 = (base->CTRL1 & ~CAN_CTRL1_CLKSRC_MASK) | CAN_CTRL1_CLKSRC(clk);
}

/**
 * @brief Initializes the FlexCAN controller.
 *
 * @param   base  The FlexCAN base address
 */
void FLEXCAN_Init(can_type_t *base);

/**
 * @brief Sets the FlexCAN time segments for setting up bit rate.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments, which need to be set for the bit rate.
 */
static inline void FLEXCAN_SetTimeSegments(can_type_t *base, const flexcan_time_segment_t *timeSeg)
{
    OS_ASSERT(timeSeg != NULL);

    (base->CTRL1) = ((base->CTRL1) &
                     ~((CAN_CTRL1_PROPSEG_MASK | CAN_CTRL1_PSEG2_MASK | CAN_CTRL1_PSEG1_MASK | CAN_CTRL1_PRESDIV_MASK) |
                       CAN_CTRL1_RJW_MASK));

    (base->CTRL1) = ((base->CTRL1) | (CAN_CTRL1_PROPSEG(timeSeg->propSeg) | CAN_CTRL1_PSEG2(timeSeg->phaseSeg2) |
                                      CAN_CTRL1_PSEG1(timeSeg->phaseSeg1) | CAN_CTRL1_PRESDIV(timeSeg->preDivider) |
                                      CAN_CTRL1_RJW(timeSeg->rJumpwidth)));
}

/**
 * @brief Sets the FlexCAN Rx individual mask for ID filtering in the Rx Message Buffers and the Rx FIFO.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   mask     Individual mask
 */
static inline void FLEXCAN_SetRxIndividualMask(can_type_t *base, uint32_t msgBuffIdx, uint32_t mask)
{
    base->RXIMR[msgBuffIdx] = mask;
}

/**
 * @brief Sets the FlexCAN RX FIFO global mask.
 *
 * @param   base  The FlexCAN base address
 * @param   Mask     Sets mask
 */
static inline void FLEXCAN_SetRxFifoGlobalMask(can_type_t *base, uint32_t Mask)
{
    (base->RXFGMASK) = Mask;
}

/**
 * @brief Sets the FlexCAN extended time segments for setting up bit rate.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments, which need to be set for the bit rate.
 */
static inline void FLEXCAN_SetExtendedTimeSegments(can_type_t *base, const flexcan_time_segment_t *timeSeg)
{
    OS_ASSERT(timeSeg != NULL);

    /* If extended bit time definitions are enabled, use CBT register */
    (base->CBT) =
        ((base->CBT) & ~((CAN_CBT_EPROPSEG_MASK | CAN_CBT_EPSEG2_MASK | CAN_CBT_EPSEG1_MASK | CAN_CBT_EPRESDIV_MASK) |
                         CAN_CBT_ERJW_MASK));

    (base->CBT) = ((base->CBT) | (CAN_CBT_EPROPSEG(timeSeg->propSeg) | CAN_CBT_EPSEG2(timeSeg->phaseSeg2) |
                                  CAN_CBT_EPSEG1(timeSeg->phaseSeg1) | CAN_CBT_EPRESDIV(timeSeg->preDivider) |
                                  CAN_CBT_ERJW(timeSeg->rJumpwidth)));
}

/**
 * @brief Sets the FlexCAN time segments for setting up bit rate for FD BRS.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments, which need to be set for the bit rate.
 */
static inline void FLEXCAN_SetFDTimeSegments(can_type_t *base, const flexcan_time_segment_t *timeSeg)
{
    OS_ASSERT(timeSeg != NULL);

    /* Set FlexCAN time segments*/
    (base->FDCBT) =
        ((base->FDCBT) &
         ~((CAN_FDCBT_FPROPSEG_MASK | CAN_FDCBT_FPSEG2_MASK | CAN_FDCBT_FPSEG1_MASK | CAN_FDCBT_FPRESDIV_MASK) |
           CAN_FDCBT_FRJW_MASK));

    (base->FDCBT) = ((base->FDCBT) | (CAN_FDCBT_FPROPSEG(timeSeg->propSeg) | CAN_FDCBT_FPSEG2(timeSeg->phaseSeg2) |
                                      CAN_FDCBT_FPSEG1(timeSeg->phaseSeg1) | CAN_FDCBT_FPRESDIV(timeSeg->preDivider) |
                                      CAN_FDCBT_FRJW(timeSeg->rJumpwidth)));
}

/**
 * @brief Gets the FlexCAN extended time segments used for setting up bit rate.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments read for bit rate
 */
static inline void FLEXCAN_GetExtendedTimeSegments(const can_type_t *base, flexcan_time_segment_t *timeSeg)
{
    OS_ASSERT(timeSeg != NULL);

    timeSeg->preDivider = ((base->CBT) & CAN_CBT_EPRESDIV_MASK) >> CAN_CBT_EPRESDIV_SHIFT;
    timeSeg->propSeg    = ((base->CBT) & CAN_CBT_EPROPSEG_MASK) >> CAN_CBT_EPROPSEG_SHIFT;
    timeSeg->phaseSeg1  = ((base->CBT) & CAN_CBT_EPSEG1_MASK) >> CAN_CBT_EPSEG1_SHIFT;
    timeSeg->phaseSeg2  = ((base->CBT) & CAN_CBT_EPSEG2_MASK) >> CAN_CBT_EPSEG2_SHIFT;
    timeSeg->rJumpwidth = ((base->CBT) & CAN_CBT_ERJW_MASK) >> CAN_CBT_ERJW_SHIFT;
}

/**
 * @brief Gets the FlexCAN time segments to calculate the bit rate.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments read for bit rate
 */
static inline void FLEXCAN_GetTimeSegments(const can_type_t *base, flexcan_time_segment_t *timeSeg)
{
    OS_ASSERT(timeSeg != NULL);

    timeSeg->preDivider = ((base->CTRL1) & CAN_CTRL1_PRESDIV_MASK) >> CAN_CTRL1_PRESDIV_SHIFT;
    timeSeg->propSeg    = ((base->CTRL1) & CAN_CTRL1_PROPSEG_MASK) >> CAN_CTRL1_PROPSEG_SHIFT;
    timeSeg->phaseSeg1  = ((base->CTRL1) & CAN_CTRL1_PSEG1_MASK) >> CAN_CTRL1_PSEG1_SHIFT;
    timeSeg->phaseSeg2  = ((base->CTRL1) & CAN_CTRL1_PSEG2_MASK) >> CAN_CTRL1_PSEG2_SHIFT;
    timeSeg->rJumpwidth = ((base->CTRL1) & CAN_CTRL1_RJW_MASK) >> CAN_CTRL1_RJW_SHIFT;
}

/**
 * @brief Gets the  FlexCAN time segments for FD BRS to calculate the bit rate.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments read for bit rate
 */
static inline void FLEXCAN_GetFDTimeSegments(const can_type_t *base, flexcan_time_segment_t *timeSeg)
{
    OS_ASSERT(timeSeg != NULL);

    timeSeg->preDivider = ((base->FDCBT) & CAN_FDCBT_FPRESDIV_MASK) >> CAN_FDCBT_FPRESDIV_SHIFT;
    timeSeg->propSeg    = ((base->FDCBT) & CAN_FDCBT_FPROPSEG_MASK) >> CAN_FDCBT_FPROPSEG_SHIFT;
    timeSeg->phaseSeg1  = ((base->FDCBT) & CAN_FDCBT_FPSEG1_MASK) >> CAN_FDCBT_FPSEG1_SHIFT;
    timeSeg->phaseSeg2  = ((base->FDCBT) & CAN_FDCBT_FPSEG2_MASK) >> CAN_FDCBT_FPSEG2_SHIFT;
    timeSeg->rJumpwidth = ((base->FDCBT) & CAN_FDCBT_FRJW_MASK) >> CAN_FDCBT_FRJW_SHIFT;
}

/**
 * @brief Get freeze status of the FlexCAN module.
 *
 * @param   base     The FlexCAN base address
 * @return  true if module is in freeze mode; false if module is not in freeze mode
 */
static inline bool FLEXCAN_GetFreezeMode(const can_type_t *base)
{
    return ((base->MCR & CAN_MCR_FRZACK_MASK) == CAN_MCR_FRZACK_MASK);
}

/**
 * @brief Un freezes the FlexCAN module.
 *
 * @param   base     The FlexCAN base address
 */
void FLEXCAN_ExitFreezeMode(can_type_t *base);

/**
 * @brief Freezes the FlexCAN module.
 * In case of Bus-Off error  this function can Soft reset,
 * the FlexCan interface, check errata 9595
 *
 * @param   base     The FlexCAN base address
 */
void FLEXCAN_EnterFreezeMode(can_type_t *base);

/**
 * @brief Set operation mode.
 *
 * @param   base  The FlexCAN base address
 * @param   mode  Set an operation mode
 */
void FLEXCAN_SetOperationMode(can_type_t *base, flexcan_operation_modes_t mode);

/**
 * @brief Check selected MB index is out of range
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx  Message Buffer index
 */
bool FLEXCAN_IsOutOfRangeMbIdx(const can_type_t *base, uint32_t msgBuffIdx);

/**
 * @brief Enables/Disables Flexible Data rate (if supported).
 *
 * @param   base    The FlexCAN base address
 * @param   enable  true to enable; false to disable
 */
static inline void FLEXCAN_SetFDEnabled(can_type_t *base, bool enable)
{
    /* Enable FD */
    base->MCR = (base->MCR & ~CAN_MCR_FDEN_MASK) | CAN_MCR_FDEN(enable ? 1UL : 0UL);

    /* Enable the use of extended bit time definitions */
    base->CBT = (base->CBT & ~CAN_CBT_BTF_MASK) | CAN_CBT_BTF(enable ? 1UL : 0UL);

    /* Disable Transmission Delay Compensation by default */
    base->FDCTRL &= ~(CAN_FDCTRL_TDCEN_MASK | CAN_FDCTRL_TDCOFF_MASK);
}

/**
 * @brief Checks if the Flexible Data rate feature is enabled.
 *
 * @param   base    The FlexCAN base address
 * @return  true if enabled; false if disabled
 */
static inline bool FLEXCAN_IsFDEnabled(const can_type_t *base)
{
    return (((base->MCR & CAN_MCR_FDEN_MASK) >> CAN_MCR_FDEN_SHIFT) != 0U);
}

/**
 * @brief Sets the payload size of the MBs.
 *
 * @param   base         The FlexCAN base address
 * @param   payloadSize  The payload size
 */
void FLEXCAN_SetPayloadSize(can_type_t *base, flexcan_payload_block_size_t payloadSize);

/**
 * @brief  Gets the payload size of the MBs.
 * 
 * @param  base The FlexCAN base address
 * @param  maxMbNum The Max no of MBs
 * @return uint8_t The payload size in bytes
 */
uint8_t FLEXCAN_GetMbPayloadSize(const can_type_t *base, uint32_t maxMbNum);

/**
 * @brief Compute payload size.
 *
 * @param   dlcValue   The dlc Value.
 * @return uint8_t The payload size.
 */
uint8_t FLEXCAN_ComputePayloadSize(uint8_t dlcValue);

/**
 * @brief Check the flexcan instance if has an extra MB ram memory
 * 
 * @param  base  The FlexCAN base address
 * @return true It has an extra MB ram memory
 * @return false It has not an MB ram memory
 */
bool FLEXCAN_IsExpandableMemoryAvailable(const can_type_t *base);

/**
 * @brief Sets the FlexCAN message buffer fields for transmitting.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @return uint32_t Pointer to the beginning of the MBs space address
 */
volatile uint32_t *FLEXCAN_GetMsgBuffRegion(can_type_t *base, uint32_t msgBuffIdx);

/**
 * @brief Sets the FlexCAN message buffer fields for transmitting.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx   Index of the message buffer
 * @param   cs           CODE/status values (TX)
 * @param   msgId        ID of the message to transmit
 * @param   msgData      Bytes of the FlexCAN message
 * @param   isRemote     Will set RTR remote Flag
 * @return status_t STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_SetTxMsgBuff(can_type_t *base, uint32_t msgBuffIdx, const flexcan_msgbuff_code_status_t *cs,
                              uint32_t msgId, const uint8_t *msgData, const bool isRemote);

/**
 * @brief Writes the abort code into the CODE field of the requested Tx message
 * buffer.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx Index of the message buffer
 */
void FLEXCAN_AbortTxMsgBuff(can_type_t *base, uint32_t msgBuffIdx);

/**
 * @brief   : Writes the Inactive Rx code into the CODE field of the requested
 * Rx message buffer the MB to active Rx. This will force even the unlock of the RxMB.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx Index of the message buffer
 */
void FLEXCAN_ResetRxMsgBuff(can_type_t *base, uint32_t msgBuffIdx);

/**
 * @brief Sets the FlexCAN message buffer fields for receiving.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   cs           CODE/status values (RX)
 * @param   msgId       ID of the message to receive
 * @return status_t STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_SetRxMsgBuff(can_type_t *base, uint32_t msgBuffIdx, const flexcan_msgbuff_code_status_t *cs,
                              uint32_t msgId);

/**
 * @brief   : Get a message buffer timestamp value.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx Index of the message buffer
 * @return uint32_t The time stamp of the MB.
 */
uint32_t FLEXCAN_GetMsgBuffTimestamp(can_type_t *base, uint32_t msgBuffIdx);

/**
 * @brief Gets the FlexCAN message buffer fields.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   msgBuff           The fields of the message buffer
 */
void FLEXCAN_GetMsgBuff(can_type_t *base, uint32_t msgBuffIdx, flexcan_msgbuff_t *msgBuff);

/**
 * @brief Locks the FlexCAN Rx message buffer.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 *
 */
void FLEXCAN_LockRxMsgBuff(can_type_t *base, uint32_t msgBuffIdx);

/**
 * @brief Unlocks the FlexCAN Rx message buffer.
 *
 * @param   base     The FlexCAN base address
 */
static inline void FLEXCAN_UnlockRxMsgBuff(const can_type_t *base)
{
    /* Unlock the mailbox by reading the free running timer */
    (void)base->TIMER;
}

/**
 * @brief Enables the Rx FIFO.
 *
 * @param   base     The FlexCAN base address
 * @param   numOfFilters    The number of Rx FIFO filters
 * @return  The status of the operation
 * @retval  STATUS_SUCCESS RxFIFO was successfully enabled
 * @retval status_t STATUS_ERROR RxFIFO could not be enabled (e.g. the FD feature
 *          was enabled, and these two features are not compatible)
 */
status_t FLEXCAN_EnableRxFifo(can_type_t *base, uint32_t numOfFilters);

/**
 * @brief Checks if Rx FIFO is enabled.
 *
 * @param   base     The FlexCAN base address
 * @return  RxFifo status (true = enabled / false = disabled)
 */
static inline bool FLEXCAN_IsRxFifoEnabled(const can_type_t *base)
{
    return (((base->MCR & CAN_MCR_RFEN_MASK) >> CAN_MCR_RFEN_SHIFT) != 0U);
}

/**
 * @brief Return Rx FIFO Id Format.
 *
 * @param   base     The FlexCAN base address
 * @return flexcan_rx_fifo_id_element_format_t RxFifo Id Format
 */
static inline flexcan_rx_fifo_id_element_format_t FLEXCAN_GetRxFifoIdFormat(const can_type_t *base)
{
    flexcan_rx_fifo_id_element_format_t id_type = FLEXCAN_RX_FIFO_ID_FORMAT_A;
    switch ((base->MCR & CAN_MCR_IDAM_MASK) >> CAN_MCR_IDAM_SHIFT) {
    case 0U:
        id_type = FLEXCAN_RX_FIFO_ID_FORMAT_A;
        break;
    case 1U:
        id_type = FLEXCAN_RX_FIFO_ID_FORMAT_B;
        break;
    case 2U:
        id_type = FLEXCAN_RX_FIFO_ID_FORMAT_C;
        break;
    case 3U:
        id_type = FLEXCAN_RX_FIFO_ID_FORMAT_D;
        break;
    default: {
        /* This case should never be reached */
        OS_ASSERT(false);
        break;
    }
    }
    return id_type;
}

/**
 * @brief Sets  the maximum number of Message Buffers.
 *
 * @param   base  The FlexCAN base address
 * @param   maxMsgBuffNum     Maximum number of message buffers
 * @return status_t STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_SetMaxMsgBuffNum(can_type_t *base, uint32_t maxMsgBuffNum);

/**
 * @brief Gets the maximum number of Message Buffers.
 *
 * @param   base  The FlexCAN base address
 * @return uint32_t the configured number of message buffers
 */
static inline uint32_t FLEXCAN_GetMaxMsgBuffNum(const can_type_t *base)
{
    return (((base->MCR) & CAN_MCR_MAXMB_MASK) >> CAN_MCR_MAXMB_SHIFT);
}

/**
 * @brief Sets the FlexCAN Rx FIFO fields.
 *
 * @param   base             The FlexCAN base address
 * @param   idFormat         The format of the Rx FIFO ID Filter Table Elements
 * @param   idFilterTable    The ID filter table elements which contain RTR bit,
 *                           IDE bit, and RX message ID.
 */
void FLEXCAN_SetRxFifoFilter(can_type_t *base, flexcan_rx_fifo_id_element_format_t idFormat,
                             const flexcan_id_table_t *idFilterTable);

/**
 * @brief Gets the FlexCAN Rx FIFO data.
 *
 * @param   base    The FlexCAN base address
 * @param   rxFifo  The FlexCAN receive FIFO data
 */
void FLEXCAN_ReadRxFifo(const can_type_t *base, flexcan_msgbuff_t *rxFifo);

/**
 * @brief Enables/Disables the FlexCAN Message Buffer interrupt.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   enable       choose enable or disable
 * @return status_t STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_SetMsgBuffIntCmd(can_type_t *base, uint32_t msgBuffIdx, bool enable);

/**
 * @brief Enables error interrupt of the FlexCAN module.
 * @param   base     The FlexCAN base address
 * @param   errType     The interrupt type
 * @param   enable       choose enable or disable
 */
void FLEXCAN_SetErrIntCmd(can_type_t *base, flexcan_int_type_t errType, bool enable);

/**
 * @brief Get the interrupt enable\disabled of the message buffers.
 *
 * @param   base        The FlexCAN base address
 * @param   msgBuffIdx  Index of the message buffer
 * @return uint8_t flag The value of interrupt of the message buffer.
 *                      1 - interrupt enabled;
 *                      0 - interrupt disabled;
 */
static inline uint8_t FLEXCAN_GetMsgBuffIntStatus(const can_type_t *base, uint32_t msgBuffIdx)
{
    uint32_t flag = 0U;

    if (msgBuffIdx < 32U) {
        flag = ((base->IMASK1 & ((uint32_t)1U << (msgBuffIdx % 32U))) >> (msgBuffIdx % 32U));
    }

    if ((msgBuffIdx >= 32U) && (msgBuffIdx < 64U)) {
        flag = ((base->IMASK2 & ((uint32_t)1U << (msgBuffIdx % 32U))) >> (msgBuffIdx % 32U));
    }

    if (msgBuffIdx >= 64U) {
        flag = ((base->IMASK3 & ((uint32_t)1U << (msgBuffIdx % 32U))) >> (msgBuffIdx % 32U));
    }

    return (uint8_t)flag;
}

/**
 * @brief Gets the individual FlexCAN MB interrupt flag.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @return uint8_t the individual Message Buffer interrupt flag (0 and 1 are the flag value)
 */
static inline uint8_t FLEXCAN_GetMsgBuffIntStatusFlag(const can_type_t *base, uint32_t msgBuffIdx)
{
    uint8_t flag = 0;
    uint32_t mask;

    if (msgBuffIdx < 32U) {
        mask = base->IMASK1 & CAN_IMASK1_BUF31TO0M_MASK;
        flag = (uint8_t)(((base->IFLAG1 & mask) >> (msgBuffIdx % 32U)) & 1U);
    }

    if ((msgBuffIdx >= 32U) && (msgBuffIdx < 64U)) {
        mask = base->IMASK2 & CAN_IMASK2_BUF63TO32M_MASK;
        flag = (uint8_t)(((base->IFLAG2 & mask) >> (msgBuffIdx % 32U)) & 1U);
    }

    if ((msgBuffIdx >= 64U) && (msgBuffIdx < 96U)) {
        mask = base->IMASK3 & CAN_IMASK3_BUF95TO64M_MASK;
        flag = (uint8_t)(((base->IFLAG3 & mask) >> (msgBuffIdx % 32U)) & 1U);
    }

    if (msgBuffIdx >= 96U) {
        mask = base->IMASK4 & CAN_IMASK4_BUF127TO96M_MASK;
        flag = (uint8_t)(((base->IFLAG4 & mask) >> (msgBuffIdx % 32U)) & 1U);
    }

    return flag;
}

/**
 * @brief Clears the FIFO
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_ClearFIFO(can_type_t *base)
{
    base->IFLAG1 = CAN_IFLAG1_BUF0I_MASK;
}

/**
 * @brief Clears the interrupt flag of the message buffers.
 *
 * @param   base        The FlexCAN base address
 * @param   msgBuffIdx  Index of the message buffer
 */
static inline void FLEXCAN_ClearMsgBuffIntStatusFlag(can_type_t *base, uint32_t msgBuffIdx)
{
    uint32_t flag = ((uint32_t)1U << (msgBuffIdx % 32U));

    /* Clear the corresponding message buffer interrupt flag*/
    if (msgBuffIdx < 32U) {
        (base->IFLAG1) = (flag);
    }

    if ((msgBuffIdx >= 32U) && (msgBuffIdx < 64U)) {
        (base->IFLAG2) = (flag);
    }

    if ((msgBuffIdx >= 64U) && (msgBuffIdx < 96U)) {
        (base->IFLAG3) = (flag);
    }

    if (msgBuffIdx >= 96U) {
        (base->IFLAG4) = (flag);
    }
}

/**
 * @brief Get the interrupt flag of the message buffers.
 *
 * @param   base        The FlexCAN base address
 * @param   msgBuffIdx  Index of the message buffer
 * @return uint8_t flag        The value of interrupt flag of the message buffer.
 */
static inline uint8_t FLEXCAN_GetBuffStatusFlag(const can_type_t *base, uint32_t msgBuffIdx)
{
    uint32_t flag = 0U;

    if (msgBuffIdx < 32U) {
        flag = ((base->IFLAG1 & ((uint32_t)1U << (msgBuffIdx % 32U))) >> (msgBuffIdx % 32U));
    }

    if ((msgBuffIdx >= 32U) && (msgBuffIdx < 64U)) {
        flag = ((base->IFLAG2 & ((uint32_t)1U << (msgBuffIdx % 32U))) >> (msgBuffIdx % 32U));
    }

    if ((msgBuffIdx >= 64U) && (msgBuffIdx < 96U)) {
        flag = ((base->IFLAG3 & ((uint32_t)1U << (msgBuffIdx % 32U))) >> (msgBuffIdx % 32U));
    }

    if (msgBuffIdx >= 96U) {
        flag = ((base->IFLAG4 & ((uint32_t)1U << (msgBuffIdx % 32U))) >> (msgBuffIdx % 32U));
    }

    return (uint8_t)flag;
}

/**
 * @brief Clears all other interrupts in ERRSTAT register (Error, Busoff, Wakeup).
 *
 * @param   base     The FlexCAN base address
 */
void FLEXCAN_ClearErrIntStatusFlag(can_type_t *base);

/**
 * @brief Clears BusOff interrupts in ESR register
 *
 * @param   base     The FlexCAN base address
 */
void FLEXCAN_ClearBusOffIntStatusFlag(can_type_t *base);

/**
 * @brief Gets the error counters.
 *
 * @param   base  The FlexCAN base address
 * @return uint32_t the value of the FlexCAN error counters
 */
static inline uint32_t FLEXCAN_GetErrorCounters(const can_type_t *base)
{
    return base->ECR;
}

/**
 * @brief Sets the Rx masking type.
 *
 * @param   base  The FlexCAN base address
 * @param   type         The FlexCAN Rx mask type
 */
static inline void FLEXCAN_SetRxMaskType(can_type_t *base, flexcan_rx_mask_type_t type)
{
    /* Set RX masking type (RX global mask or RX individual mask) */
    if (type == FLEXCAN_RX_MASK_GLOBAL) {
        /* Enable Global RX masking */
        base->MCR = (base->MCR & ~CAN_MCR_IRMQ_MASK) | CAN_MCR_IRMQ(0U);
    } else {
        /* Enable Individual Rx Masking and Queue */
        base->MCR = (base->MCR & ~CAN_MCR_IRMQ_MASK) | CAN_MCR_IRMQ(1U);
    }
}

/**
 * @brief Get the no of Rx FIFO ID Filter Table Elements Affected by Rx Individual Masks
 *
 * @param   base  The FlexCAN base address
 * @return uint8_t the max no of elements affected by Rx Individual Mask register RXMIR in RxFIFO Mode
 */
static inline uint8_t FLEXCAN_GetNoOfIndividualMBsRxFIFO(const can_type_t *base)
{
    /* Get the number of RX FIFO Filters*/
    uint8_t ret = (uint8_t)(((base->CTRL2) & CAN_CTRL2_RFFN_MASK) >> CAN_CTRL2_RFFN_SHIFT);
    /* Max filters configured by individual mask are (7 + RFFN * 2) depends on the FIFO size,
     * max allowed value is 31 RXIMR */
    ret = (uint8_t)(7U + ((uint32_t)ret << 1U));

    return ret;
}

/**
 * @brief Sets the FlexCAN Rx individual standard mask for ID filtering in the Rx MBs and the Rx FIFO.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   stdMask     Individual standard mask
 */
static inline void FLEXCAN_SetRxIndividualStdMask(can_type_t *base, uint32_t msgBuffIdx, uint32_t stdMask)
{
    (base->RXIMR[msgBuffIdx]) = (stdMask << FLEXCAN_ID_STD_SHIFT) & FLEXCAN_ID_STD_MASK;
}

/**
 * @brief Sets the FlexCAN Rx individual extended mask for ID filtering in the Rx Message Buffers and the Rx FIFO.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   extMask     Individual extended mask
 */
static inline void FLEXCAN_SetRxIndividualExtMask(can_type_t *base, uint32_t msgBuffIdx, uint32_t extMask)
{
    base->RXIMR[msgBuffIdx] = (extMask << FLEXCAN_ID_EXT_SHIFT) & (FLEXCAN_ID_STD_MASK | FLEXCAN_ID_EXT_MASK);
}

/**
 * @brief Sets the FlexCAN Rx Message Buffer global standard mask.
 *
 * @param   base  The FlexCAN base address
 * @param   stdMask     Standard mask
 */
static inline void FLEXCAN_SetRxMsgBuffGlobalStdMask(can_type_t *base, uint32_t stdMask)
{
    (base->RXMGMASK) = (((uint32_t)(((uint32_t)(stdMask)) << FLEXCAN_ID_STD_SHIFT)) & FLEXCAN_ID_STD_MASK);
}

/**
 * @brief Sets the FlexCAN RX Message Buffer global extended mask.
 *
 * @param   base  The FlexCAN base address
 * @param   extMask     Extended mask
 */
static inline void FLEXCAN_SetRxMsgBuffGlobalExtMask(can_type_t *base, uint32_t extMask)
{
    (base->RXMGMASK) =
        (((uint32_t)(((uint32_t)(extMask)) << FLEXCAN_ID_EXT_SHIFT)) & (FLEXCAN_ID_STD_MASK | FLEXCAN_ID_EXT_MASK));
}

/**
 * @brief Sets the FlexCAN Rx Message Buffer 14 standard mask.
 *
 * @param   base  The FlexCAN base address
 * @param   stdMask     Standard mask
 */
static inline void FLEXCAN_SetRxMsgBuff14StdMask(can_type_t *base, uint32_t stdMask)
{
    (base->RX14MASK) = (((uint32_t)(((uint32_t)(stdMask)) << FLEXCAN_ID_STD_SHIFT)) & FLEXCAN_ID_STD_MASK);
}

/**
 * @brief Sets the FlexCAN RX Message Buffer 14 extended mask.
 *
 * @param   base  The FlexCAN base address
 * @param   extMask     Extended mask
 */
static inline void FLEXCAN_SetRxMsgBuff14ExtMask(can_type_t *base, uint32_t extMask)
{
    (base->RX14MASK) =
        (((uint32_t)(((uint32_t)(extMask)) << FLEXCAN_ID_EXT_SHIFT)) & (FLEXCAN_ID_STD_MASK | FLEXCAN_ID_EXT_MASK));
}

/**
 * @brief Sets the FlexCAN Rx Message Buffer 15 standard mask.
 *
 * @param   base  The FlexCAN base address
 * @param   stdMask     Standard mask
 */
static inline void FLEXCAN_SetRxMsgBuff15StdMask(can_type_t *base, uint32_t stdMask)
{
    (base->RX15MASK) = (((uint32_t)(((uint32_t)(stdMask)) << FLEXCAN_ID_STD_SHIFT)) & FLEXCAN_ID_STD_MASK);
}

/**
 * @brief Sets the FlexCAN RX Message Buffer 15 extended mask.
 *
 * @param   base  The FlexCAN base address
 * @param   extMask     Extended mask
 */
static inline void FLEXCAN_SetRxMsgBuff15ExtMask(can_type_t *base, uint32_t extMask)
{
    (base->RX15MASK) =
        (((uint32_t)(((uint32_t)(extMask)) << FLEXCAN_ID_EXT_SHIFT)) & (FLEXCAN_ID_STD_MASK | FLEXCAN_ID_EXT_MASK));
}

/**
 * @brief Enables/Disables the Stuff Bit Count for CAN FD frames.
 *
 * If enabled, the modulo 8 count of variable stuff bits inserted plus the respective
 * parity bit (even parity calculated over the 3-bit modulo 8 count) are combined as
 * the 4-bit Stuff Count field and inserted before the CRC Sequence field. CRC
 * calculation extends beyond the end of Data field and takes the Stuff Count field bits
 * into account.
 *
 * @param   base  The FlexCAN base address
 * @param   enable Enable/Disable Stuff Bit Count
 */
static inline void FLEXCAN_SetStuffBitCount(can_type_t *base, bool enable)
{
    base->CTRL2 = (base->CTRL2 & ~CAN_CTRL2_ISOCANFDEN_MASK) | CAN_CTRL2_ISOCANFDEN(enable ? 1UL : 0UL);
}

/**
 * @brief Enables/Disables the Self Reception feature.
 *
 * If enabled, FlexCAN is allowed to receive frames transmitted by itself.
 *
 * @param   base  The FlexCAN base address
 * @param   enable Enable/Disable Self Reception
 */
static inline void FLEXCAN_SetSelfReception(can_type_t *base, bool enable)
{
    base->MCR = (base->MCR & ~CAN_MCR_SRXDIS_MASK) | CAN_MCR_SRXDIS(enable ? 0UL : 1UL);
}

/**
 * @brief Enables/Disables the Transceiver Delay Compensation feature and sets
 * the Transceiver Delay Compensation Offset (offset value to be added to the
 * measured transceiver's loop delay in order to define the position of the
 * delayed comparison point when bit rate switching is active).
 *
 * @param   base  The FlexCAN base address
 * @param   enable Enable/Disable Transceiver Delay Compensation
 * @param   offset Transceiver Delay Compensation Offset
 */
static inline void FLEXCAN_SetTDCOffset(can_type_t *base, bool enable, uint8_t offset)
{
    uint32_t tmp;

    OS_ASSERT(offset <= 0x1F); /* The TDCOFF filed is 5 bits width */

    tmp = base->FDCTRL;
    tmp &= ~(CAN_FDCTRL_TDCEN_MASK | CAN_FDCTRL_TDCOFF_MASK);

    if (enable) {
        tmp = tmp | CAN_FDCTRL_TDCEN_MASK;
        tmp = tmp | CAN_FDCTRL_TDCOFF(offset);
    }

    base->FDCTRL = tmp;
}

/**
 * @brief Check if selected instance support FD feature or not?
 * this function shall return True if selected instance support FD and False
 * otherwise
 *
 * @param   instance  The FlexCAN instance id
 */
static inline bool FLEXCAN_InstanceHasFD(uint8_t instance)
{
    static const bool fdInstances[] = FEATURE_FLEXCAN_INSTANCES_HAS_FD;
    return fdInstances[instance];
}

/**
 * @brief Enables/Disables the PDMA support for RxFIFO.
 *
 * @param   base  The FlexCAN base address
 * @param   enable Enable/Disable PDMA support
 */
static inline void FLEXCAN_SetRxFifoDMA(can_type_t *base, bool enable)
{
    base->MCR = (base->MCR & ~CAN_MCR_DMA_MASK) | CAN_MCR_DMA(enable ? 1UL : 0UL);
}

/**
 * @brief Checks if Enhanced Rx FIFO is enabled.
 *
 * @param   base     The FlexCAN base address
 * @return  EnhancedRxFifo status (true = enabled / false = disabled)
 */
static inline bool FLEXCAN_IsEnhancedRxFifoEnabled(const can_type_t *base)
{
    return ((((base->ERFCR & CAN_ERFCR_ERFEN_MASK) >> CAN_ERFCR_ERFEN_SHIFT) != 0U) ? (true) : (false));
}

/**
 * Function Name : FLEXCAN_SetEnhancedRxFifoIntAll
 * @param   base     The FlexCAN base address
 * @param   enable   enable or disable
 * Description   : Enable/Disable All(Underflow, Overflow, Watermark and frame available)
 *                 interrupts for Enhanced Rx FIFO.
 */
static inline void FLEXCAN_SetEnhancedRxFifoIntAll(can_type_t *base, bool enable)
{
    if (true == enable) {
        (base->ERFIER) = (uint32_t)(FLEXCAN_ENHACED_RX_FIFO_ALL_INTERRUPT_MASK);
    } else {
        (base->ERFIER) = (uint32_t)(FLEXCAN_ENHACED_RX_FIFO_NO_INTERRUPT_MASK);
    }
}

/**
 * @brief Resets Enhanced Rx FIFO engine and state.
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_ClearEnhancedRxFifoEngine(can_type_t *base)
{
    base->ERFSR = base->ERFSR | CAN_ERFSR_ERFCLR_MASK;
}

/**
 * @brief Clears the Enhanced Rx FIFO
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_ClearEnhancedFIFO(can_type_t *base)
{
    base->ERFSR = CAN_ERFSR_ERFCLR_MASK;
}

/**
 * @brief Configure the number of words to transfer for each Enhanced Rx FIFO data element in PDMA mode.
 *
 * @param   base    The FlexCAN base address
 * @param   numOfWords   The number of words to transfer
 */
static inline void FLEXCAN_ConfigEnhancedRxFifoDMA(can_type_t *base, uint32_t numOfWords)
{
    base->ERFCR =
        (base->ERFCR & (~CAN_ERFCR_DMALW_MASK)) | (((numOfWords - 1U) << CAN_ERFCR_DMALW_SHIFT) & CAN_ERFCR_DMALW_MASK);
}

/**
 * @brief Gets the individual FlexCAN Enhanced Rx FIFO flag.
 *
 * @param   base        The FlexCAN base address
 * @param   intFlag     Index of the Enhanced Rx FIFO flag
 * @return uint8_t the individual Enhanced Rx FIFO flag (0 and 1 are the flag value)
 */
static inline uint8_t FLEXCAN_GetEnhancedRxFIFOStatusFlag(const can_type_t *base, uint32_t intFlag)
{
    return (uint8_t)((base->ERFSR & ((uint32_t)1U << ((uint8_t)intFlag & (uint8_t)0x1F))) >>
                     ((uint8_t)intFlag & (uint8_t)0x1F));
}

/**
 * @brief Clears the interrupt flag of the Enhanced Rx FIFO.
 *
 * @param   base  The FlexCAN base address
 * @param   intFlag       Index of the Enhanced Rx FIFO interrupt flag
 */
static inline void FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(can_type_t *base, uint32_t intFlag)
{
    (base->ERFSR) = (uint32_t)1U << intFlag;
}

/**
 * @brief Gets the individual FlexCAN Enhanced Rx FIFO interrupt flag.
 *
 * @param   base        The FlexCAN base address
 * @param   intFlag     Index of the Enhanced Rx FIFO interrupt flag
 * @return uint8_t the individual Enhanced Rx FIFO interrupt flag (0 and 1 are the flag value)
 */
static inline uint8_t FLEXCAN_GetEnhancedRxFIFOIntStatusFlag(const can_type_t *base, uint32_t intFlag)
{
    return (uint8_t)((base->ERFIER & ((uint32_t)1U << ((uint8_t)intFlag & (uint8_t)0x1F))) >>
                     ((uint8_t)intFlag & (uint8_t)0x1F));
}
/**
 * @brief Checks if FlexCAN has Enhanced Rx FIFO.
 *
 * @param   base  The FlexCAN base address
 * @return  EnhancedRxFifo status (TRUE = available / FALSE = unavailable)
 */
bool FLEXCAN_IsEnhancedRxFifoAvailable(const can_type_t *base);

/**
 * @brief Enable the enhanced rx fifo.
 *
 * @param   base  The FlexCAN base address
 * @param   numOfStdIDFilters  The number of the standard ID filters
 * @param   numOfExtIDFilters  The number of the extended ID filters
 * @param   numOfWatermark  The number of the water mark
 * @return  status_t
 *          STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_EnableEnhancedRxFifo(can_type_t *base, uint32_t numOfStdIDFilters, uint32_t numOfExtIDFilters,
                                      uint32_t numOfWatermark);
/**
 * @brief Set the enhanced rx fifo filters.
 *
 * @param   base  The FlexCAN base address
 * @param   idFilterTable  The pointer of the enhanced id filter table.
 */
void FLEXCAN_SetEnhancedRxFifoFilter(can_type_t *base, const flexcan_enhancedIdTableType_t *idFilterTable);

/**
 * @brief Clear Enhance fifo data.
 *
 * @param   base  The FlexCAN base address
 */
void FLEXCAN_ClearOutputEnhanceFIFO(can_type_t *base);

/**
 * @brief Read Enhance fifo data.
 *
 * @param   base  The FlexCAN base address
 * @param   config  The config of time stamp.
 */
void FLEXCAN_ConfigTimestamp(can_type_t *base, const flexcan_timeStampConfigType_t *config);

/**
 * @brief Checks if High Resolution Time Stamp is enabled.
 *
 * @param   base     The FlexCAN base address
 * @return  HRTimeStamp status (true = enabled / false = disabled)
 */
static inline bool FLEXCAN_IsHRTimeStampEnabled(const can_type_t *base)
{
    return ((((base->CTRL2 & CAN_CTRL2_TSTAMPCAP_MASK) >> CAN_CTRL2_TSTAMPCAP_SHIFT) != 0U) ? true : false);
}

/**
 * @brief Configures Pretended Networking mode filtering selection.
 *
 * @param   base  The FlexCAN base address
 * @param   wakeUpTimeout  enable/disable wake up by timeout
 * @param   wakeUpMatch  enable/disable wake up by match
 * @param   numMatches  set number of messages matching the same filtering criteria
 * @param   filterComb  set filtering combination selection
 * @param   idFilterType  set ID filtering selection
 * @param   payloadFilterType  set payload filtering selection
 *
 */
static inline void FLEXCAN_SetPNFilteringSelection(can_type_t *base, bool wakeUpTimeout, bool wakeUpMatch,
                                                   uint16_t numMatches, flexcan_pn_filter_combination_t filterComb,
                                                   flexcan_pn_filter_selection_t idFilterType,
                                                   flexcan_pn_filter_selection_t payloadFilterType)
{
    uint32_t tmp;

    tmp = base->CTRL1_PN;
    tmp &= ~(CAN_CTRL1_PN_WTOF_MSK_MASK | CAN_CTRL1_PN_WUMF_MSK_MASK | CAN_CTRL1_PN_NMATCH_MASK |
             CAN_CTRL1_PN_PLFS_MASK | CAN_CTRL1_PN_IDFS_MASK | CAN_CTRL1_PN_FCS_MASK);
    tmp |= CAN_CTRL1_PN_WTOF_MSK(wakeUpTimeout ? 1UL : 0UL);
    tmp |= CAN_CTRL1_PN_WUMF_MSK(wakeUpMatch ? 1UL : 0UL);
    tmp |= CAN_CTRL1_PN_NMATCH(numMatches);
    tmp |= CAN_CTRL1_PN_FCS(filterComb);
    tmp |= CAN_CTRL1_PN_IDFS(idFilterType);
    tmp |= CAN_CTRL1_PN_PLFS(payloadFilterType);
    base->CTRL1_PN = tmp;
}

/**
 * @brief Set PN timeout value.
 *
 * @param   base  The FlexCAN base address
 * @param   timeoutValue  timeout for no message matching
 */
static inline void FLEXCAN_SetPNTimeoutValue(can_type_t *base, uint16_t timeoutValue)
{
    base->CTRL2_PN = (base->CTRL2_PN & ~CAN_CTRL2_PN_MATCHTO_MASK) | CAN_CTRL2_PN_MATCHTO(timeoutValue);
}

/**
 * @brief Configures the Pretended Networking ID Filter 1.
 *
 * @param   base  The FlexCAN base address
 * @param   idFilter  The ID Filter configuration
 */
static inline void FLEXCAN_SetPNIdFilter1(can_type_t *base, flexcan_pn_id_filter_t idFilter)
{
    uint32_t tmp;

    tmp = base->FLT_ID1;
    tmp &= ~(CAN_FLT_ID1_FLT_IDE_MASK | CAN_FLT_ID1_FLT_RTR_MASK | CAN_FLT_ID1_FLT_ID1_MASK);
    tmp |= CAN_FLT_ID1_FLT_IDE(idFilter.extendedId ? 1UL : 0UL);
    tmp |= CAN_FLT_ID1_FLT_RTR(idFilter.remoteFrame ? 1UL : 0UL);
    if (idFilter.extendedId) {
        tmp |= CAN_FLT_ID1_FLT_ID1(idFilter.id);
    } else {
        tmp |= CAN_FLT_ID1_FLT_ID1(idFilter.id << FLEXCAN_ID_STD_SHIFT);
    }
    base->FLT_ID1 = tmp;
}

/**
 * @brief Configures the Pretended Networking ID Filter 2 Check IDE&RTR.
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_SetPNIdFilter2Check(can_type_t *base)
{
    base->FLT_ID2_IDMASK |= CAN_FLT_ID2_IDMASK_IDE_MSK_MASK | CAN_FLT_ID2_IDMASK_RTR_MSK_MASK;
}

/**
 * @brief Configures the Pretended Networking ID Filter 2.
 *
 * @param   base  The FlexCAN base address
 * @param   pnConfig  The pretended networking configuration
 */
static inline void FLEXCAN_SetPNIdFilter2(can_type_t *base, const flexcan_pn_config_t *pnConfig)
{
    uint32_t tmp;

    tmp = base->FLT_ID2_IDMASK;
    tmp &=
        ~(CAN_FLT_ID2_IDMASK_IDE_MSK_MASK | CAN_FLT_ID2_IDMASK_RTR_MSK_MASK | CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_MASK);
    tmp |= CAN_FLT_ID2_IDMASK_IDE_MSK(pnConfig->idFilter2.extendedId ? 1UL : 0UL);
    tmp |= CAN_FLT_ID2_IDMASK_RTR_MSK(pnConfig->idFilter2.remoteFrame ? 1UL : 0UL);
    /* Check if idFilter1 is extended and apply accordingly mask */
    if (pnConfig->idFilter1.extendedId) {
        tmp |= CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK(pnConfig->idFilter2.id);
    } else {
        tmp |= CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK(pnConfig->idFilter2.id << FLEXCAN_ID_STD_SHIFT);
    }
    base->FLT_ID2_IDMASK = tmp;
}

/**
 * @brief Set PN DLC Filter.
 *
 * @param   base  The FlexCAN base address
 * @param   timeoutValue  timeout for no message matching
 */
static inline void FLEXCAN_SetPNDlcFilter(can_type_t *base, uint8_t dlcLow, uint8_t dlcHigh)
{
    uint32_t tmp;

    tmp = base->FLT_DLC;
    tmp &= ~(CAN_FLT_DLC_FLT_DLC_HI_MASK | CAN_FLT_DLC_FLT_DLC_HI_MASK);
    tmp |= CAN_FLT_DLC_FLT_DLC_HI(dlcHigh);
    tmp |= CAN_FLT_DLC_FLT_DLC_LO(dlcLow);
    base->FLT_DLC = tmp;
}

/**
 * @brief Set PN Payload High Filter 1.
 *
 * @param   base  The FlexCAN base address
 * @param   payload  message payload filter
 */
static inline void FLEXCAN_SetPNPayloadHighFilter1(can_type_t *base, const uint8_t *payload)
{
    base->PL1_HI = CAN_PL1_HI_Data_byte_4(payload[4]) | CAN_PL1_HI_Data_byte_5(payload[5]) |
                   CAN_PL1_HI_Data_byte_6(payload[6]) | CAN_PL1_HI_Data_byte_7(payload[7]);
}

/**
 * @brief Set PN Payload Low Filter 1.
 *
 * @param   base  The FlexCAN base address
 * @param   payload  message payload filter
 */
static inline void FLEXCAN_SetPNPayloadLowFilter1(can_type_t *base, const uint8_t *payload)
{
    base->PL1_LO = CAN_PL1_LO_Data_byte_0(payload[0]) | CAN_PL1_LO_Data_byte_1(payload[1]) |
                   CAN_PL1_LO_Data_byte_2(payload[2]) | CAN_PL1_LO_Data_byte_3(payload[3]);
}

/**
 * @brief Set PN Payload High Filter 2.
 *
 * @param   base  The FlexCAN base address
 * @param   payload  message payload filter
 */
static inline void FLEXCAN_SetPNPayloadHighFilter2(can_type_t *base, const uint8_t *payload)
{
    base->PL2_PLMASK_HI = CAN_PL2_PLMASK_HI_Data_byte_4(payload[4]) | CAN_PL2_PLMASK_HI_Data_byte_5(payload[5]) |
                          CAN_PL2_PLMASK_HI_Data_byte_6(payload[6]) | CAN_PL2_PLMASK_HI_Data_byte_7(payload[7]);
}

/**
 * @brief Set PN Payload Low Filter 2.
 *
 * @param   base  The FlexCAN base address
 * @param   payload  message payload filter
 */
static inline void FLEXCAN_SetPNPayloadLowFilter2(can_type_t *base, const uint8_t *payload)
{
    base->PL2_PLMASK_LO = CAN_PL2_PLMASK_LO_Data_byte_0(payload[0]) | CAN_PL2_PLMASK_LO_Data_byte_1(payload[1]) |
                          CAN_PL2_PLMASK_LO_Data_byte_2(payload[2]) | CAN_PL2_PLMASK_LO_Data_byte_3(payload[3]);
}

/**
 * @brief Configures the Pretended Networking mode.
 *
 * @param   base  The FlexCAN base address
 * @param   pnConfig  The pretended networking configuration
 */
void FLEXCAN_ConfigPN(can_type_t *base, const flexcan_pn_config_t *pnConfig);

/**
 * @brief Enables/Disables the Pretended Networking mode.
 *
 * @param   base  The FlexCAN base address
 * @param   enable  Enable/Disable Pretending Networking
 */
static inline void FLEXCAN_SetPN(can_type_t *base, bool enable)
{
    base->MCR = (base->MCR & ~CAN_MCR_PNET_EN_MASK) | CAN_MCR_PNET_EN(enable ? 1UL : 0UL);
}

/**
 * @brief Checks if the Pretended Networking mode is enabled/disabled.
 *
 * @param   base  The FlexCAN base address
 * @return  false if Pretended Networking mode is disabled;
 *          true if Pretended Networking mode is enabled
 */
static inline bool FLEXCAN_IsPNEnabled(const can_type_t *base)
{
    return (((base->MCR & CAN_MCR_PNET_EN_MASK) >> CAN_MCR_PNET_EN_SHIFT) != 0U);
}

/**
 * @brief Gets the Wake Up by Timeout Flag Bit.
 *
 * @param   base  The FlexCAN base address
 * @return uint8_t the Wake Up by Timeout Flag Bit
 */
static inline uint8_t FLEXCAN_GetWTOF(const can_type_t *base)
{
    return (uint8_t)((base->WU_MTC & CAN_WU_MTC_WTOF_MASK) >> CAN_WU_MTC_WTOF_SHIFT);
}

/**
 * @brief Clears the Wake Up by Timeout Flag Bit.
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_ClearWTOF(can_type_t *base)
{
    base->WU_MTC |= CAN_WU_MTC_WTOF_MASK;
}

/**
 * @brief Gets the Wake Up by Match Flag Bit.
 *
 * @param   base  The FlexCAN base address
 * @return uint8_t the Wake Up by Match Flag Bit
 */
static inline uint8_t FLEXCAN_GetWUMF(const can_type_t *base)
{
    return (uint8_t)((base->WU_MTC & CAN_WU_MTC_WUMF_MASK) >> CAN_WU_MTC_WUMF_SHIFT);
}

/**
 * @brief Clears the Wake Up by Match Flag Bit.
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_ClearWUMF(can_type_t *base)
{
    base->WU_MTC |= CAN_WU_MTC_WUMF_MASK;
}

/**
 * @brief Disable Error Detection and Correction of Memory Errors.
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_DisableMemErrorDetection(can_type_t *base)
{
    /* Enable write of MECR register */
    base->CTRL2 |= CAN_CTRL2_ECRWRE(1);
    /* Enable write of MECR */
    base->MECR = CAN_MECR_ECRWRDIS(0);
    /* Disable Error Detection and Correction mechanism,
     * that will set CAN in Freeze Mode in case of trigger */
    base->MECR = CAN_MECR_NCEFAFRZ(0);
    /* Disable write of MECR */
    base->CTRL2 |= CAN_CTRL2_ECRWRE(0);
}

#if defined(__cplusplus)
}
#endif

#endif /* __FLEXCAN_HW_ACCESS_H__ */
