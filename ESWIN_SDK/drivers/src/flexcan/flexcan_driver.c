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
 * @file flexcan_driver.c
 * @brief FLEXCAN driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "clock_driver.h"
#include "flexcan_hw_access.h"

#define FLEXCAN_MB_HANDLE_RXFIFO   (0U)
#define FLEXCAN_MB_ENHANCED_RXFIFO (255U)

/* CAN bit timing values */
#define FLEXCAN_NUM_TQ_MIN  (8U)
#define FLEXCAN_NUM_TQ_MAX  (26U)
#define FLEXCAN_PRESDIV_MAX (256U)
#define FLEXCAN_PSEG1_MAX   (8U)
#define FLEXCAN_PSEG2_MIN   (1U)
#define FLEXCAN_PSEG2_MAX   (8U)
#define FLEXCAN_PROPSEG_MAX (8U)
#define FLEXCAN_TSEG1_MIN   (2U)
#define FLEXCAN_TSEG1_MAX   (17U)
#define FLEXCAN_TSEG2_MIN   (2U)
#define FLEXCAN_TSEG2_MAX   (9U)
#define FLEXCAN_RJW_MAX     (3U)

/* Table of base addresses for CAN instances. */
static can_type_t *const g_flexcanBase[] = CAN_BASE_PTRS;

/* Tables to save CAN IRQ enum numbers defined in CMSIS header file. */
static const IRQn_Type g_flexcanWakeUpMatchIrqId[]                         = CAN_WakeUp_Match_IRQS;
static const IRQn_Type g_flexcanWakeUpTimeOutIrqId[]                       = CAN_WakeUp_TimeOut_IRQS;
static const IRQn_Type g_flexcanTxWarningIrqId[]                           = CAN_Tx_Warning_IRQS;
static const IRQn_Type g_flexcanErrorIrqId[]                               = CAN_Error_IRQS;
static const IRQn_Type g_flexcanBusOffIrqId[]                              = CAN_Bus_Off_IRQS;
static const IRQn_Type g_flexcanOredMessageBufferIrqId[CAN_INSTANCE_COUNT] = FEATURE_FLEXCAN_MB_IRQS;

static const IRQn_Type g_flexcanEnhancedRxfifoIrqId[CAN_INSTANCE_COUNT] = CAN_ENHANCED_RX_FIFO_IRQS;
/* Pointer to runtime state structure.*/
static flexcan_state_t *g_flexcanStatePtr[CAN_INSTANCE_COUNT] = {NULL};

/* Store CAN instance .*/
static can_instance_t g_flexcanInstance[CAN_INSTANCE_COUNT];

static status_t FLEXCAN_StartSendData(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info,
                                      uint32_t msg_id, const uint8_t *mb_data, bool isBlocking);
static status_t FLEXCAN_StartRxMessageBufferData(uint8_t instance, uint8_t mb_idx, flexcan_msgbuff_t *data,
                                                 bool isBlocking);
static status_t FLEXCAN_StartRxMessageFifoData(uint8_t instance, flexcan_msgbuff_t *data, bool isBlocking);
static void FLEXCAN_CompleteTransfer(uint8_t instance, uint32_t mb_idx);
static void FLEXCAN_CompleteRxMessageFifoData(uint8_t instance);
static status_t FLEXCAN_InitRxFifo(can_type_t *base, const flexcan_user_config_t *data);
static void FLEXCAN_CompleteRxFifoDataDMA(void *parameter, pdma_chn_status_t status);
static uint32_t FLEXCAN_BitrateToTimeSeg(uint32_t bitrate, uint32_t clkFreq, flexcan_time_segment_t *timeSeg);
static inline void FLEXCAN_IRQHandlerRxFIFO(uint8_t instance, uint32_t mb_idx);
static void FLEXCAN_IRQHandlerRxMB(uint8_t instance, uint32_t mb_idx);
static inline void FLEXCAN_EnableIRQs(uint8_t instance);
static void FLEXCAN_WakeUpHandler(void *data);
static void FLEXCAN_TxWarning_IRQHandler(void *data);
static void FLEXCAN_Error_IRQHandler(void *data);
static void FLEXCAN_BusOff_IRQHandler(void *data);
static void FLEXCAN_IRQHandler(void *data);
static status_t FLEXCAN_ProccessLegacyRxFIFO(uint8_t instance, uint32_t timeout_ms);
static void FLEXCAN_IRQHandlerEnhancedRxFIFO(uint8_t instance, uint32_t intType);
static inline void FLEXCAN_ProcessIRQHandlerEnhancedRxFIFO(uint8_t u8Instance);
static void FLEXCAN_CompleteRxMessageEnhancedFifoData(uint8_t instance);
static status_t FLEXCAN_StartRxMessageEnhancedFifoData(uint8_t instance, flexcan_msgbuff_t *data, bool isBlocking);
static status_t FLEXCAN_ProccessEnhancedRxFifo(uint8_t u8Instance, uint32_t u32TimeoutMs);
static void FLEXCAN_EnhancedRxFIFODataIRQHandler(void *data);
static void FLEXCAN_ReadEnhancedRxFifo(const can_type_t *base, flexcan_msgbuff_t *rxFifo);

/*
 * Function Name : FLEXCAN_DRV_SetBitrate
 * Description   : Set FlexCAN baudrate.
 * This function will set up all the time segment values for classical frames or the
 * extended time segments for the arbitration phase of FD frames. Those time segment
 * values are passed in by the user and are based on the required baudrate.
 *
 */
void FLEXCAN_DRV_SetBitrate(uint8_t instance, const flexcan_time_segment_t *bitrate)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(bitrate != NULL);

    bool fdEnabled   = false;
    can_type_t *base = g_flexcanBase[instance];
    bool instHasFd   = FLEXCAN_InstanceHasFD(instance);
    if (instHasFd) {
        fdEnabled = FLEXCAN_IsFDEnabled(base);
    }

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    if (fdEnabled) {
        /* Set extended time segments */
        FLEXCAN_SetExtendedTimeSegments(base, bitrate);
    } else {
        /* Set time segments */
        FLEXCAN_SetTimeSegments(base, bitrate);
    }

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_SetBitrateCbt
 * Description   : Set FlexCAN's bitrate.
 * This function will set up all the time segment values for the data phase of
 * FD frames. Those time segment values are passed in by the user and are based
 * on the required baudrate.
 *
 */
void FLEXCAN_DRV_SetBitrateCbt(uint8_t instance, const flexcan_time_segment_t *bitrate)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(bitrate != NULL);

    can_type_t *base = g_flexcanBase[instance];

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    /* Set time segments */
    FLEXCAN_SetFDTimeSegments(base, bitrate);
    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_GetBitrate
 * Description   : Get FlexCAN baudrate.
 * This function will be return the current bit rate settings for classical frames
 * or the arbitration phase of FD frames.
 *
 */
void FLEXCAN_DRV_GetBitrate(uint8_t instance, flexcan_time_segment_t *bitrate)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(bitrate != NULL);

    bool fdEnabled         = false;
    const can_type_t *base = g_flexcanBase[instance];
    bool instHasFd         = FLEXCAN_InstanceHasFD(instance);
    if (instHasFd) {
        fdEnabled = FLEXCAN_IsFDEnabled(base);
    }

    if (true == fdEnabled) {
        /* Get the Extended time segments */
        FLEXCAN_GetExtendedTimeSegments(base, bitrate);
    } else {
        /* Get the time segments */
        FLEXCAN_GetTimeSegments(base, bitrate);
    }
}

/*
 * Function Name : FLEXCAN_DRV_GetBitrateFD
 * Description   : Get FlexCAN baudrate.
 * This function will be return the current bit rate settings for the data phase
 * of FD frames.
 *
 */
void FLEXCAN_DRV_GetBitrateFD(uint8_t instance, flexcan_time_segment_t *bitrate)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(bitrate != NULL);

    const can_type_t *base = g_flexcanBase[instance];

    /* Get the time segments */
    FLEXCAN_GetFDTimeSegments(base, bitrate);
}

/*
 * Function Name : FLEXCAN_DRV_SetMaskType
 * Description   : Set RX masking type.
 * This function will set RX masking type as RX global mask or RX individual
 * mask.
 *
 */
void FLEXCAN_DRV_SetRxMaskType(uint8_t instance, flexcan_rx_mask_type_t type)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base = g_flexcanBase[instance];

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    FLEXCAN_SetRxMaskType(base, type);

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_SetRxFifoGlobalMask
 * Description   : Set Rx FIFO global mask as the 11-bit standard mask or the
 * 29-bit extended mask.
 *
 */
void FLEXCAN_DRV_SetRxFifoGlobalMask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint32_t mask)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    flexcan_rx_fifo_id_element_format_t formatType;
    can_type_t *base  = g_flexcanBase[instance];
    uint32_t calcMask = 0U;

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    if (true == FLEXCAN_IsRxFifoEnabled(base)) {
        formatType = FLEXCAN_GetRxFifoIdFormat(base);
        calcMask   = FLEXCAN_GetRxFifoMask(id_type, formatType, mask);

        switch (formatType) {
        case FLEXCAN_RX_FIFO_ID_FORMAT_A:
            FLEXCAN_SetRxFifoGlobalMask(base, calcMask);
            break;
        case FLEXCAN_RX_FIFO_ID_FORMAT_B:
            FLEXCAN_SetRxFifoGlobalMask(base, (calcMask | (calcMask >> FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1)));
            break;
        case FLEXCAN_RX_FIFO_ID_FORMAT_C:
            FLEXCAN_SetRxFifoGlobalMask(base, (calcMask | (calcMask >> FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1) |
                                               (calcMask >> FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2) |
                                               (calcMask >> FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3)));
            break;
        default:
            /* Will Enable all filter fields mask */
            FLEXCAN_SetRxFifoGlobalMask(base, 0xFFFFFFFFU);
            break;
        }
    }

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_SetRxMbGlobalMask
 * Description   : Set Rx Message Buffer global mask as the 11-bit standard mask
 * or the 29-bit extended mask.
 *
 */
void FLEXCAN_DRV_SetRxMbGlobalMask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint32_t mask)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base = g_flexcanBase[instance];

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    if (id_type == FLEXCAN_MSG_ID_STD) {
        /* Set standard global mask for RX MB */
        FLEXCAN_SetRxMsgBuffGlobalStdMask(base, mask);
    } else if (id_type == FLEXCAN_MSG_ID_EXT) {
        /* Set extended global mask for RX MB */
        FLEXCAN_SetRxMsgBuffGlobalExtMask(base, mask);
    } else {
        /* Should not get here */
    }

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_SetRxMb14Mask
 * Description   : Set Rx Message Buffer 14 mask as the 11-bit standard mask
 *                  or the 29-bit extended mask.
 *
 */
void FLEXCAN_DRV_SetRxMb14Mask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint32_t mask)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base = g_flexcanBase[instance];

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    if (id_type == FLEXCAN_MSG_ID_STD) {
        /* Set standard global mask for RX MB */
        FLEXCAN_SetRxMsgBuff14StdMask(base, mask);
    } else if (id_type == FLEXCAN_MSG_ID_EXT) {
        /* Set extended global mask for RX MB */
        FLEXCAN_SetRxMsgBuff14ExtMask(base, mask);
    } else {
        /* Should not get here */
    }

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_SetRxMb15Mask
 * Description   : Set Rx Message Buffer 15 mask as the 11-bit standard mask
 * or the 29-bit extended mask.
 *
 */
void FLEXCAN_DRV_SetRxMb15Mask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint32_t mask)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base = g_flexcanBase[instance];

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    if (id_type == FLEXCAN_MSG_ID_STD) {
        /* Set standard global mask for RX MB */
        FLEXCAN_SetRxMsgBuff15StdMask(base, mask);
    } else if (id_type == FLEXCAN_MSG_ID_EXT) {
        /* Set extended global mask for RX MB */
        FLEXCAN_SetRxMsgBuff15ExtMask(base, mask);
    } else {
        /* Should not get here */
    }

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_SetRxIndividualMask
 * Description   : Set Rx individual mask as the 11-bit standard mask or the
 * 29-bit extended mask.
 *
 */
status_t FLEXCAN_DRV_SetRxIndividualMask(uint8_t instance, flexcan_msgbuff_id_type_t id_type, uint8_t mb_idx,
                                         uint32_t mask)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base = g_flexcanBase[instance];

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    if ((mb_idx > FLEXCAN_GetMaxMsgBuffNum(base)) || (mb_idx >= CAN_RXIMR_COUNT)) {
        if (freeze == false) {
            FLEXCAN_ExitFreezeMode(base);
        }
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    if (false == FLEXCAN_IsRxFifoEnabled(base)) {
        if (id_type == FLEXCAN_MSG_ID_STD) {
            /* Set standard individual mask */
            FLEXCAN_SetRxIndividualStdMask(base, mb_idx, mask);
        } else if (id_type == FLEXCAN_MSG_ID_EXT) {
            /* Set extended individual mask */
            FLEXCAN_SetRxIndividualExtMask(base, mb_idx, mask);
        } else {
            /* Should not get here */
        }
    } else {
        if (mb_idx <= FLEXCAN_GetNoOfIndividualMBsRxFIFO(base)) {
            /* In FIFO Mode get the Id Filter Format already configured by FLEXCAN_DRV_ConfigRxFifo */
            flexcan_rx_fifo_id_element_format_t formatType = FLEXCAN_GetRxFifoIdFormat(base);
            uint32_t calcMask                              = FLEXCAN_GetRxFifoMask(id_type, formatType, mask);

            switch (formatType) {
            case FLEXCAN_RX_FIFO_ID_FORMAT_A:
                FLEXCAN_SetRxIndividualMask(base, mb_idx, calcMask);
                break;
            case FLEXCAN_RX_FIFO_ID_FORMAT_B:
                FLEXCAN_SetRxIndividualMask(base, mb_idx,
                                            (calcMask | (calcMask >> FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1)));
                break;
            case FLEXCAN_RX_FIFO_ID_FORMAT_C:
                FLEXCAN_SetRxIndividualMask(base, mb_idx,
                                            (calcMask | (calcMask >> FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1) |
                                             (calcMask >> FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2) |
                                             (calcMask >> FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3)));
                break;
            default:
                /* Will Enable all filter fields mask */
                FLEXCAN_SetRxIndividualMask(base, mb_idx, 0xFFFFFFFFU);
                break;
            }
        } else {
            if (id_type == FLEXCAN_MSG_ID_STD) {
                /* Set standard individual mask */
                FLEXCAN_SetRxIndividualStdMask(base, mb_idx, mask);
            } else if (id_type == FLEXCAN_MSG_ID_EXT) {
                /* Set extended individual mask */
                FLEXCAN_SetRxIndividualExtMask(base, mb_idx, mask);
            } else {
                /* Should not get here */
            }
        }
    }

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }

    return STATUS_SUCCESS;
}

/*
 * Function Name : FLEXCAN_EnableIRQs
 * Description   : This function enable FLEXCAN instance Interrupts
 *
 */
static inline void FLEXCAN_EnableIRQs(uint8_t instance)
{
    OS_RegisterType_t uType = {0};
    can_type_t *base        = g_flexcanBase[instance];

    uType.trig_mode = CLIC_LEVEL_TRIGGER;
    uType.lvl       = 200;
    uType.priority  = 1;
    uType.data_ptr  = (void *)(uint32_t)instance;

    if (NotAvail_IRQn != g_flexcanWakeUpMatchIrqId[instance]) {
        OS_RequestIrq(g_flexcanWakeUpMatchIrqId[instance], FLEXCAN_WakeUpHandler, &uType);
        OS_EnableIrq(g_flexcanWakeUpMatchIrqId[instance]);
    }

    if (NotAvail_IRQn != g_flexcanWakeUpTimeOutIrqId[instance]) {
        OS_RequestIrq(g_flexcanWakeUpTimeOutIrqId[instance], FLEXCAN_WakeUpHandler, &uType);
        OS_EnableIrq(g_flexcanWakeUpTimeOutIrqId[instance]);
    }

    OS_RequestIrq(g_flexcanTxWarningIrqId[instance], FLEXCAN_TxWarning_IRQHandler, &uType);
    OS_EnableIrq(g_flexcanTxWarningIrqId[instance]);

    OS_RequestIrq(g_flexcanErrorIrqId[instance], FLEXCAN_Error_IRQHandler, &uType);
    OS_EnableIrq(g_flexcanErrorIrqId[instance]);

    OS_RequestIrq(g_flexcanBusOffIrqId[instance], FLEXCAN_BusOff_IRQHandler, &uType);
    OS_EnableIrq(g_flexcanBusOffIrqId[instance]);

    OS_RequestIrq(g_flexcanOredMessageBufferIrqId[instance], FLEXCAN_IRQHandler, &uType);
    OS_EnableIrq(g_flexcanOredMessageBufferIrqId[instance]);

    if (FLEXCAN_IsEnhancedRxFifoAvailable(base)) {
        OS_RequestIrq(g_flexcanEnhancedRxfifoIrqId[instance], FLEXCAN_EnhancedRxFIFODataIRQHandler, &uType);
        OS_EnableIrq(g_flexcanEnhancedRxfifoIrqId[instance]);
    }
}

/*
 * Function Name : FLEXCAN_InitRxFifo
 * Description   : Initialize fifo and dma if requested.
 *
 * This is not a public API as it is called from other driver functions.
 */
static status_t FLEXCAN_InitRxFifo(can_type_t *base, const flexcan_user_config_t *data)
{
    status_t result = STATUS_SUCCESS;
    /* Enable RxFIFO feature, if requested. This might fail if the FD mode is
     * enabled. */
    if (data->is_rx_fifo_needed) {
        result = FLEXCAN_EnableRxFifo(base, (uint32_t)data->num_id_filters);
    }

    /* Enable Enhanced RxFIFO feature, if requested.
     * This might fail if the current CAN instance does not support Enhanced
     * RxFIFO or the Rx FIFO is enabled. */
    if (data->is_enhanced_rx_fifo_needed) {
        if (FLEXCAN_RXFIFO_USING_DMA == data->transfer_type) {
            result = FLEXCAN_EnableEnhancedRxFifo(base, (uint32_t)data->num_enhanced_std_id_filters,
                                                  (uint32_t)data->num_enhanced_ext_id_filters,
                                                  (uint32_t)0U); // for dma, each a frame received -> a minor loop
        } else {
            result = FLEXCAN_EnableEnhancedRxFifo(base, (uint32_t)data->num_enhanced_std_id_filters,
                                                  (uint32_t)data->num_enhanced_ext_id_filters,
                                                  (uint32_t)data->num_enhanced_watermark);
        }
    }

    if (STATUS_SUCCESS == result) {
        /* Enable PDMA support for RxFIFO transfer, if requested. */
        if (FLEXCAN_RXFIFO_USING_DMA == data->transfer_type) {
            if (FLEXCAN_IsRxFifoEnabled(base)) {
                FLEXCAN_SetRxFifoDMA(base, true);
            } else if (FLEXCAN_IsEnhancedRxFifoAvailable(base)) {
                if (FLEXCAN_IsEnhancedRxFifoEnabled(base)) {
                    FLEXCAN_SetRxFifoDMA(base, true);
                    FLEXCAN_ConfigEnhancedRxFifoDMA(base, 20U); // always transfer 80 bytes (DMALW = 19)
                } else {
                    result = STATUS_ERROR;
                }
            } else {
                result = STATUS_ERROR;
            }
        } else {
            if (FLEXCAN_IsEnhancedRxFifoAvailable(base)) {
                /* Clear Enhanced Rx FIFO status.*/
                FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE);
                FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_WATERMARK);
                FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_OVERFLOW);
                FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_UNDERFLOW);
                /* Clear the Enhanced RX FIFO engine */
                FLEXCAN_ClearEnhancedRxFifoEngine(base);
            }
            FLEXCAN_SetRxFifoDMA(base, false);
        }
    }

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_Init
 * Description   : Initialize FlexCAN driver.
 * This function will select a source clock, reset FlexCAN module, set maximum
 * number of message buffers, initialize all message buffers as inactive, enable
 * RX FIFO if needed, mask all mask bits, disable all MB interrupts, enable
 * FlexCAN normal mode, and enable all the error interrupts if needed.
 *
 */
status_t FLEXCAN_DRV_Init(uint8_t instance, flexcan_state_t *state, const flexcan_user_config_t *data)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(state != NULL);
    OS_ASSERT(g_flexcanStatePtr[instance] == NULL);

    const clock_names_t canClocks[CAN_INSTANCE_COUNT] = FLEXCAN_CLOCKS;
    CLOCK_SYS_ClockEnable(canClocks[instance], true);

    uint32_t frequency = 0;
    CLOCK_SYS_GetFreq(canClocks[instance], &frequency);

    status_t result;
    can_type_t *base = g_flexcanBase[instance];
    bool instHasFd   = FLEXCAN_InstanceHasFD(instance);
    bool fdEnabled   = false;
    flexcan_time_segment_t bitrate;
    uint32_t i, j;

    g_flexcanInstance[instance].instance   = instance;
    g_flexcanInstance[instance].CANBasePtr = base;

    if (FLEXCAN_IsEnabled(base)) {
        /* To enter Disable Mode requires FreezeMode first */
        FLEXCAN_EnterFreezeMode(base);
        FLEXCAN_Disable(base);
    }

    /* Select a source clock for the FlexCAN engine */
    FLEXCAN_SelectClock(base, data->pe_clock);

    /* Enable the CAN clock */
    FLEXCAN_Enable(base);

    FLEXCAN_EnterFreezeMode(base);

    /* Initialize FLEXCAN device */
    FLEXCAN_Init(base);

    if (true == instHasFd) {
        /* Enable/Disable FD and check FD was set as expected.
         * Setting FD as enabled might fail if the current
         * CAN instance does not support FD. */
        FLEXCAN_SetFDEnabled(base, data->fd_enable);
        if (FLEXCAN_IsFDEnabled(base) != data->fd_enable) {
            return STATUS_ERROR;
        }

        /* If the FD feature is enabled, enable the Stuff Bit Count,
         * in order to be ISO-compliant. */
        FLEXCAN_SetStuffBitCount(base, data->fd_enable);
    } else if (data->fd_enable) {
        /* Return STATUS_ERROR when instance doesn't support FD
         * but fd_enable is set to True by user. */
        return STATUS_ERROR;
    } else {
        /* Do Nothing */
    }

    /* Disable the self reception feature if FlexCAN is not in loopback mode. */
    if (FLEXCAN_LOOPBACK_MODE != data->flexcanMode) {
        FLEXCAN_SetSelfReception(base, false);

        if (data->fd_enable) {
            uint8_t tdcOffset = (data->bitrate_cbt.phaseSeg1 + data->bitrate_cbt.propSeg
                + 2) * (data->bitrate_cbt.preDivider + 1);
            FLEXCAN_SetTDCOffset(base, true, tdcOffset);
        }
    }

    result = FLEXCAN_InitRxFifo(base, data);
    if (STATUS_SUCCESS != result) {
        FLEXCAN_Disable(base);
        return result;
    }

    if (true == instHasFd) {
        /* Set payload size. */
        FLEXCAN_SetPayloadSize(base, data->payload);
    }

    result = FLEXCAN_SetMaxMsgBuffNum(base, data->max_num_mb);
    if (STATUS_SUCCESS != result) {
        return result;
    }

    if (instHasFd) {
        fdEnabled = FLEXCAN_IsFDEnabled(base);
    }

    /* Set bit rate. */
    if (fdEnabled) {
        bitrate = data->bitrate;
        FLEXCAN_SetExtendedTimeSegments(base, &bitrate);
        bitrate = data->bitrate_cbt;
        FLEXCAN_SetFDTimeSegments(base, &bitrate);
    } else {
        bitrate = data->bitrate;
        FLEXCAN_SetTimeSegments(base, &bitrate);
    }

    /* Select mode */
    FLEXCAN_SetOperationMode(base, data->flexcanMode);

    FLEXCAN_ConfigTimestamp(base, &data->time_stamp);

    if (FLEXCAN_FREEZE_MODE != data->flexcanMode) {
        FLEXCAN_ExitFreezeMode(base);
    }

    /* Enable FlexCAN interrupts.*/
    FLEXCAN_EnableIRQs(instance);

    uint32_t maxNumMB = FLEXCAN_GetMaxMbNum(base);
    for (i = 0; i < maxNumMB; i++) {
        state->mbs[i].mbSema = OS_SemaphoreNew(1, 0, NULL);
        if (state->mbs[i].mbSema == NULL) {
            for (j = 0; j < i; j++) {
                (void)OS_SemaphoreDelete(state->mbs[j].mbSema);
            }
            return STATUS_ERROR;
        }
        state->mbs[i].isBlocking = false;
        state->mbs[i].mb_message = NULL;
        state->mbs[i].state      = FLEXCAN_MB_IDLE;
        state->mbs[i].time_stamp = 0U;
    }

    state->enhancedFifoOutput.mbSema     = OS_SemaphoreNew(1, 0, NULL);
    state->enhancedFifoOutput.isBlocking = false;
    state->enhancedFifoOutput.mb_message = NULL;
    state->enhancedFifoOutput.state      = FLEXCAN_MB_IDLE;

    FLEXCAN_DisableMemErrorDetection(base);

    /* Store transfer type and PDMA channel number used in transfer */
    state->transferType            = data->transfer_type;
    state->rxFifoDMAChannel        = data->rxFifoDMAChannel;
    state->u32NumOfMbTransferByDMA = (data->num_enhanced_watermark + 1);
    /* Clear Callbacks in case of auto variables garbage */
    state->callback           = NULL;
    state->callbackParam      = NULL;
    state->error_callback     = NULL;
    state->errorCallbackParam = NULL;
    state->isIntActive        = true;
    state->u32MaxMbNum        = data->max_num_mb;
    state->bIsEnhancedFifoEn  = data->is_enhanced_rx_fifo_needed;
    /* Save runtime structure pointers so irq handler can point to the correct state structure */
    g_flexcanStatePtr[instance] = state;

    return STATUS_SUCCESS;
}

/*
 * Function Name : FLEXCAN_DRV_ConfigTxMb
 * Description   : Configure a Tx message buffer.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will set up the message buffer fields,
 * configure the message buffer code for Tx buffer as INACTIVE, and enable the
 * Message Buffer interrupt.
 *
 */
status_t FLEXCAN_DRV_ConfigTxMb(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info, uint32_t msg_id)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(tx_info != NULL);

    flexcan_msgbuff_code_status_t cs;
    can_type_t *base = g_flexcanBase[instance];
    bool instHasFd   = FLEXCAN_InstanceHasFD(instance);

    /* Check if the MB index is in range */
    if (FLEXCAN_IsOutOfRangeMbIdx(base, mb_idx)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    /* Initialize transmit mb*/
    cs.dataLen   = tx_info->data_length;
    cs.msgIdType = tx_info->msg_id_type;
    cs.code      = (uint32_t)FLEXCAN_TX_INACTIVE;
    cs.enable_brs = false;
    cs.fd_enable  = false;
    cs.fd_padding = tx_info->fd_padding;
    if (instHasFd) {
        cs.enable_brs = tx_info->enable_brs;
        cs.fd_enable  = tx_info->fd_enable;
    }

    return FLEXCAN_SetTxMsgBuff(base, mb_idx, &cs, msg_id, NULL, false);
}

/*
 * Function Name : FLEXCAN_DRV_ConfigRemoteResponseMb
 * Description   : Configures a transmit message buffer for remote frame
 * response. This function will first check if RX FIFO is enabled. If RX FIFO is
 * enabled, the function will make sure if the MB requested is not occupied by
 * the RX FIFO and ID filter table. Then this function will set up the message
 * buffer fields, configure the message buffer code for Tx buffer as RX_RANSWER,
 * and enable the Message Buffer interrupt.
 *
 */
status_t FLEXCAN_DRV_ConfigRemoteResponseMb(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info,
                                            uint32_t msg_id, const uint8_t *mb_data)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(tx_info != NULL);

    flexcan_msgbuff_code_status_t cs;
    can_type_t *base = g_flexcanBase[instance];

    if (mb_idx > (((base->MCR) & CAN_MCR_MAXMB_MASK) >> CAN_MCR_MAXMB_SHIFT)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    /* Initialize transmit mb */
    cs.dataLen   = tx_info->data_length;
    cs.msgIdType = tx_info->msg_id_type;
    cs.code      = (uint32_t)FLEXCAN_RX_RANSWER;

    FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);

    return FLEXCAN_SetTxMsgBuff(base, mb_idx, &cs, msg_id, mb_data, tx_info->is_remote);
}

/*
 * Function Name : FLEXCAN_DRV_SendBlocking
 * Description   : This function sends a CAN frame using a configured message
 * buffer. The function blocks until either the frame was sent, or the specified
 * timeout expired.
 *
 */
status_t FLEXCAN_DRV_SendBlocking(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info, uint32_t msg_id,
                                  const uint8_t *mb_data, uint32_t timeout_ms)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(tx_info != NULL);

    status_t result;
    flexcan_state_t *state = g_flexcanStatePtr[instance];
    can_type_t *base       = g_flexcanBase[instance];

    /* Check if the MB index is in range */
    if (FLEXCAN_IsOutOfRangeMbIdx(base, mb_idx)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    result = FLEXCAN_StartSendData(instance, mb_idx, tx_info, msg_id, mb_data, true);

    if (STATUS_SUCCESS == result) {
        OS_Status_t status;

        /* Enable message buffer interrupt */
        (void)FLEXCAN_SetMsgBuffIntCmd(base, mb_idx, true);

        status = OS_SemaphoreAcquire(state->mbs[mb_idx].mbSema, timeout_ms);

        /* Disable message buffer interrupt */
        (void)FLEXCAN_SetMsgBuffIntCmd(base, mb_idx, false);

        if (OS_ErrorTimeout == status) {
            if (FLEXCAN_MB_IDLE != state->mbs[mb_idx].state) {
                /* Clear message buffer flag */
                FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
                FLEXCAN_AbortTxMsgBuff(base, mb_idx);
                uint32_t flexcan_mb_config;

                /* Wait to finish abort operation */
                while (FLEXCAN_GetBuffStatusFlag(base, mb_idx) == 0U) {
                    /* Do Nothing wait for the flag */
                }

                volatile const uint32_t *flexcan_mb = FLEXCAN_GetMsgBuffRegion(base, mb_idx);
                flexcan_mb_config                   = *flexcan_mb;
                /* Check if the MBs have been safely Inactivated */
                if (((flexcan_mb_config & FLEXCAN_CS_CODE_MASK) >> FLEXCAN_CS_CODE_SHIFT) ==
                    (uint32_t)FLEXCAN_TX_INACTIVE) {
                    /* Transmission have occurred */
                    result = STATUS_SUCCESS;
                }
                if (((flexcan_mb_config & FLEXCAN_CS_CODE_MASK) >> FLEXCAN_CS_CODE_SHIFT) ==
                    (uint32_t)FLEXCAN_TX_ABORT) {
                    /* Transmission have been aborted */
                    result = STATUS_TIMEOUT;
                }
                /* Clear message buffer flag */
                FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
                state->mbs[mb_idx].state = FLEXCAN_MB_IDLE;
            } else {
                /* Transmission have occurred */
                result = STATUS_SUCCESS;
                /* add dummy wait for reset state->mbs[mb_idx].mbSema at beginning of block functions */
                (void)OS_SemaphoreAcquire(state->mbs[mb_idx].mbSema, 0);
            }
        }
    }

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_Send
 * Description   : This function sends a CAN frame using a configured message
 * buffer. The function returns immediately. If a callback is installed, it will
 * be invoked after the frame was sent.
 *
 */
status_t FLEXCAN_DRV_Send(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info, uint32_t msg_id,
                          const uint8_t *mb_data)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(tx_info != NULL);

    status_t result;
    can_type_t *base = g_flexcanBase[instance];

    /* Check if the MB index is in range */
    if (FLEXCAN_IsOutOfRangeMbIdx(base, mb_idx)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    result = FLEXCAN_StartSendData(instance, mb_idx, tx_info, msg_id, mb_data, false);
    if (STATUS_SUCCESS == result) {
        /* Enable message buffer interrupt */
        result = FLEXCAN_SetMsgBuffIntCmd(base, mb_idx, true);
    }

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_ConfigMb
 * Description   : Configure a Rx message buffer.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will set up the message buffer fields,
 * configure the message buffer code for Rx message buffer as NOT_USED, enable
 * the Message Buffer interrupt, configure the message buffer code for Rx
 * message buffer as INACTIVE, copy user's buffer into the message buffer data
 * area, and configure the message buffer code for Rx message buffer as EMPTY.
 *
 */
status_t FLEXCAN_DRV_ConfigRxMb(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *rx_info, uint32_t msg_id)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(rx_info != NULL);

    status_t result;
    flexcan_msgbuff_code_status_t cs;
    can_type_t *base = g_flexcanBase[instance];
    bool instHasFd   = FLEXCAN_InstanceHasFD(instance);

    /* Check if the MB index is in range */
    if (FLEXCAN_IsOutOfRangeMbIdx(base, mb_idx)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    cs.dataLen   = rx_info->data_length;
    cs.msgIdType = rx_info->msg_id_type;
    if (instHasFd) {
        cs.fd_enable = rx_info->fd_enable;
    }

    /* Clear the message buffer flag if previous remained triggered */
    FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);

    /* Initialize rx mb */
    cs.code = (uint32_t)FLEXCAN_RX_NOT_USED;
    result  = FLEXCAN_SetRxMsgBuff(base, mb_idx, &cs, msg_id);

    if (STATUS_SUCCESS == result) {
        /* Initialize receive MB */
        cs.code = (uint32_t)FLEXCAN_RX_INACTIVE;
        result  = FLEXCAN_SetRxMsgBuff(base, mb_idx, &cs, msg_id);
    }

    if (STATUS_SUCCESS == result) {
        /* Set up FlexCAN message buffer fields for receiving data */
        cs.code = (uint32_t)FLEXCAN_RX_EMPTY;
        result  = FLEXCAN_SetRxMsgBuff(base, mb_idx, &cs, msg_id);
    }

    if (STATUS_SUCCESS == result) {
        /* Clear the message buffer flag if previous remained triggered */
        FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
    }

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_ConfigRxFifo
 * Description   : Configure RX FIFO ID filter table elements.
 * This function will configure RX FIFO ID filter table elements, and enable RX
 * FIFO interrupts.
 *
 */
void FLEXCAN_DRV_ConfigRxFifo(uint8_t instance, flexcan_rx_fifo_id_element_format_t id_format,
                              const flexcan_id_table_t *id_filter_table)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base = g_flexcanBase[instance];

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    /* Initialize rx fifo */
    FLEXCAN_SetRxFifoFilter(base, id_format, id_filter_table);

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_ConfigEnhancedRxFifo
 * Description   : Configure Enhanced RX FIFO ID filter table elements.
 * This function will configure Enhanced RX FIFO ID filter table elements, and enable Enhanced RX
 * FIFO interrupts.
 *
 */
void FLEXCAN_DRV_ConfigEnhancedRxFifo(uint8_t instance, const flexcan_enhancedIdTableType_t *id_filter_table)
{
    can_type_t *base = g_flexcanBase[instance];

    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(FLEXCAN_IsEnhancedRxFifoAvailable(base));
    OS_ASSERT(id_filter_table != NULL);

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    /* Initialize rx fifo */
    FLEXCAN_SetEnhancedRxFifoFilter(base, id_filter_table);

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_ReceiveBlocking
 * Description   : This function receives a CAN frame into a configured message
 * buffer. The function blocks until either a frame was received, or the
 * specified timeout expired.
 *
 */
status_t FLEXCAN_DRV_ReceiveBlocking(uint8_t instance, uint8_t mb_idx, flexcan_msgbuff_t *data, uint32_t timeout_ms)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    status_t result;
    flexcan_state_t *state = g_flexcanStatePtr[instance];
    can_type_t *base       = g_flexcanBase[instance];

    /* Check if the MB index is in range */
    if (FLEXCAN_IsOutOfRangeMbIdx(base, mb_idx)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    result = FLEXCAN_StartRxMessageBufferData(instance, mb_idx, data, true);

    if (STATUS_SUCCESS == result) {
        OS_Status_t status;

        status = OS_SemaphoreAcquire(state->mbs[mb_idx].mbSema, timeout_ms);

        /* Disable message buffer interrupt */
        (void)FLEXCAN_SetMsgBuffIntCmd(base, mb_idx, false);

        if (OS_ErrorTimeout == status) {
            /* Consider the MB state has been changed by interrupt as frame received */
            if (state->mbs[mb_idx].state == FLEXCAN_MB_IDLE) {
                /* add dummy wait for reset state->mbs[mb_idx].mbSema at beginning of block functions */
                (void)OS_SemaphoreAcquire(state->mbs[mb_idx].mbSema, 0);
                return STATUS_SUCCESS;
            } else {
                result                   = STATUS_TIMEOUT;
                state->mbs[mb_idx].state = FLEXCAN_MB_IDLE;
            }
        }
    }

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_Receive
 * Description   : This function receives a CAN frame into a configured message
 * buffer. The function returns immediately. If a callback is installed, it will
 * be invoked after the frame was received and read into the specified buffer.
 *
 */
status_t FLEXCAN_DRV_Receive(uint8_t instance, uint8_t mb_idx, flexcan_msgbuff_t *data)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    const can_type_t *base = g_flexcanBase[instance];

    status_t result;

    /* Check if the MB index is in range */
    if (FLEXCAN_IsOutOfRangeMbIdx(base, mb_idx)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    result = FLEXCAN_StartRxMessageBufferData(instance, mb_idx, data, false);

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_RxFifoBlocking
 * Description   : This function receives a CAN frame using the Rx FIFO. The
 * function blocks until either a frame was received, or the specified timeout
 * expired.
 *
 */
status_t FLEXCAN_DRV_RxFifoBlocking(uint8_t instance, flexcan_msgbuff_t *data, uint32_t timeout_ms)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    status_t result;
    can_type_t *base = g_flexcanBase[instance];

    if (true == FLEXCAN_IsEnhancedRxFifoAvailable(base)) {
        if (true == FLEXCAN_IsEnhancedRxFifoEnabled(base)) {
            result = FLEXCAN_StartRxMessageEnhancedFifoData(instance, data, true);
            if (STATUS_SUCCESS == result) {
                result = FLEXCAN_ProccessEnhancedRxFifo(instance, timeout_ms);
            }
        } else {
            result = FLEXCAN_StartRxMessageFifoData(instance, data, true);
            if (STATUS_SUCCESS == result) {
                result = FLEXCAN_ProccessLegacyRxFIFO(instance, timeout_ms);
            }
        }
    } else {
        result = FLEXCAN_StartRxMessageFifoData(instance, data, true);
        if (result == STATUS_SUCCESS) {
            result = FLEXCAN_ProccessLegacyRxFIFO(instance, timeout_ms);
        }
    }

    return result;
}

/*
 * Function Name : FLEXCAN_ProccessLegacyRxFIFO
 * Description   : This function receives a CAN frame using the Rx FIFO. The
 * function blocks until either a frame was received, or the specified timeout
 * expired.
 *
 */
static status_t FLEXCAN_ProccessLegacyRxFIFO(uint8_t instance, uint32_t timeout_ms)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    status_t result;
    flexcan_state_t *state = g_flexcanStatePtr[instance];
    can_type_t *base       = g_flexcanBase[instance];

    result = (status_t)OS_SemaphoreAcquire(state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mbSema, timeout_ms);
    if (FLEXCAN_RXFIFO_USING_INTERRUPTS == state->transferType) {
        /* Disable RX FIFO interrupts */
        (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_FRAME_AVAILABLE_SHIFT, false);
        (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_WARNING_SHIFT, false);
        (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_OVERFLOW_SHIFT, false);
    } else {
        /* This function always return status success */
        (void)PDMA_DRV_StopChannel(state->rxFifoDMAChannel);
    }

    if (STATUS_TIMEOUT == result) {
        /* If the status is updated reception successful else report TimeOut */
        if (state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state == FLEXCAN_MB_IDLE) {
            result = STATUS_SUCCESS;
            /* add dummy wait for reset state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mbSema at beginning of block functions */
            (void)OS_SemaphoreAcquire(state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mbSema, 0);

        } else {
            state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state = FLEXCAN_MB_IDLE;
        }
    }

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_RxFifo
 * Description   : This function receives a CAN frame using the Rx FIFO. The
 * function returns immediately. If a callback is installed, it will be invoked
 * after the frame was received and read into the specified buffer.
 *
 */

status_t FLEXCAN_DRV_RxFifo(uint8_t instance, flexcan_msgbuff_t *data)
{
    status_t result = STATUS_SUCCESS;

    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    const can_type_t *base = g_flexcanBase[instance];
    if (true == FLEXCAN_IsEnhancedRxFifoAvailable(base)) {
        if (true == FLEXCAN_IsEnhancedRxFifoEnabled(base)) {
            result = FLEXCAN_StartRxMessageEnhancedFifoData(instance, data, false);
        } else {
            result = FLEXCAN_StartRxMessageFifoData(instance, data, false);
        }
    } else {
        result = FLEXCAN_StartRxMessageFifoData(instance, data, false);
    }

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_Deinit
 * Description   : Shutdown a FlexCAN module.
 * This function will disable all FlexCAN interrupts, and disable the FlexCAN.
 *
 */
status_t FLEXCAN_DRV_Deinit(uint8_t instance)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base             = g_flexcanBase[instance];
    const flexcan_state_t *state = g_flexcanStatePtr[instance];
    status_t result              = STATUS_SUCCESS;
    OS_Status_t osifStat;
    uint32_t i;

    /* Disable FlexCAN interrupts. */
    if (NULL != state) {
        if (state->error_callback != NULL) {
            FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_ERR, false);
            FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_RX_WARNING, false);
            FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_TX_WARNING, false);
            FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_BUSOFF, false);
        }
    }

    if (NotAvail_IRQn != g_flexcanWakeUpMatchIrqId[instance]) {
        OS_DisableIrq(g_flexcanWakeUpMatchIrqId[instance]);
    }

    if (NotAvail_IRQn != g_flexcanWakeUpTimeOutIrqId[instance]) {
        OS_DisableIrq(g_flexcanWakeUpTimeOutIrqId[instance]);
    }

    OS_DisableIrq(g_flexcanErrorIrqId[instance]);
    OS_DisableIrq(g_flexcanBusOffIrqId[instance]);
    OS_DisableIrq(g_flexcanOredMessageBufferIrqId[instance]);

    if (FLEXCAN_IsEnabled(g_flexcanBase[instance])) {
        /* Enter Freeze Mode Required before to enter Disabled Mode */
        FLEXCAN_EnterFreezeMode(g_flexcanBase[instance]);
        /* Disable FlexCAN.*/
        FLEXCAN_Disable(g_flexcanBase[instance]);
        /* Check if the state have been initialized */
    }

    const clock_names_t canClocks[CAN_INSTANCE_COUNT] = FLEXCAN_CLOCKS;
    CLOCK_SYS_ClockEnable(canClocks[instance], false);

    if (NULL != state) {
        uint32_t maxNumMB = FLEXCAN_GetMaxMbNum(base);
        for (i = 0; i < maxNumMB; i++) {
            osifStat = OS_SemaphoreDelete(state->mbs[i].mbSema);
            if (OS_OK != osifStat) {
                result = STATUS_ERROR;
            }
        }
        OS_SemaphoreDelete(state->enhancedFifoOutput.mbSema);
    }

    if (STATUS_SUCCESS == result) {
        /* Clear state pointer that is checked by FLEXCAN_DRV_Init */
        g_flexcanStatePtr[instance] = NULL;
    }

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_SetTDCOffset
 * Description   : Enables/Disables the Transceiver Delay Compensation feature and sets
 * the Transceiver Delay Compensation Offset.
 *
 */
void FLEXCAN_DRV_SetTDCOffset(uint8_t instance, bool enable, uint8_t offset)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base = g_flexcanBase[instance];

    bool freeze = FLEXCAN_GetFreezeMode(base);

    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    /* Enable/Disable TDC and set the TDC Offset */
    FLEXCAN_SetTDCOffset(base, enable, offset);

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_GetTDCValue
 * Description   : Gets the value of the Transceiver Delay Compensation.
 *
 */
uint8_t FLEXCAN_DRV_GetTDCValue(uint8_t instance)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    const can_type_t *base = g_flexcanBase[instance];

    return (uint8_t)((base->FDCTRL & CAN_FDCTRL_TDCVAL_MASK) >> CAN_FDCTRL_TDCVAL_SHIFT);
}

/*
 * Function Name : FLEXCAN_DRV_GetTDCFail
 * Description   : Gets the value of the TDC Fail flag.
 *
 */
bool FLEXCAN_DRV_GetTDCFail(uint8_t instance)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    const can_type_t *base = g_flexcanBase[instance];

    return (((base->FDCTRL & CAN_FDCTRL_TDCFAIL_MASK) >> CAN_FDCTRL_TDCFAIL_SHIFT) != 0U);
}

/*
 * Function Name : FLEXCAN_DRV_ClearTDCFail
 * Description   : Clears the TDC Fail flag.
 *
 */
void FLEXCAN_DRV_ClearTDCFail(uint8_t instance)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base = g_flexcanBase[instance];

    base->FDCTRL = base->FDCTRL | CAN_FDCTRL_TDCFAIL_MASK;
}

static void FLEXCAN_EnhancedRxFIFODataIRQHandler(void *data)
{
    uint8_t Instance = (uint8_t)(uint32_t)data;

    can_type_t *base             = g_flexcanBase[Instance];
    const flexcan_state_t *state = g_flexcanStatePtr[Instance];
    uint32_t u32intType          = 0U;

    if (NULL != state) {
        /* Get the interrupts that are enabled and ready */
        for (u32intType = FLEXCAN_ENHANCED_RXFIFO_WATERMARK; u32intType >= FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE;
             u32intType--) {
            if ((uint8_t)0U != FLEXCAN_GetEnhancedRxFIFOStatusFlag(base, u32intType)) {
                if ((uint8_t)0U != FLEXCAN_GetEnhancedRxFIFOIntStatusFlag(base, u32intType)) {
                    FLEXCAN_IRQHandlerEnhancedRxFIFO(Instance, u32intType);
                }
            }
        }
    } else {
        /* Clear status interrupt flag */
        FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_WATERMARK);
        FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE);
    }
}

/*
 * Function Name : FLEXCAN_ReadEnhancedRxFifo
 * Description   : Read Enhanced Rx FIFO data.
 * This function will copy Enhanced Rx FIFO data output into user's buffer.
 *
 */
static void FLEXCAN_ReadEnhancedRxFifo(const can_type_t *base, flexcan_msgbuff_t *rxFifo)
{
    uint32_t DataByte;
    uint32_t MbWord;
    uint8_t Idhit_Offset;
    volatile const uint32_t *Flexcan_Mb         = (uint32_t *)((uint32_t)base + (uint32_t)FEATURE_FLEXCAN_ENHANCED_FIFO_RAM_OFFSET);
    volatile const uint32_t *Flexcan_Mb_Id      = &Flexcan_Mb[1];
    volatile const uint32_t *Flexcan_Mb_Data_32 = &Flexcan_Mb[2];
    uint32_t *MsgData_32                        = NULL;

    /* Check if the buffer address is aligned */

    /* Compute payload size */
    uint8_t flexcan_mb_dlc_value = (uint8_t)(((*Flexcan_Mb) & FLEXCAN_CS_DLC_MASK) >> FLEXCAN_CS_DLC_SHIFT);
    uint8_t can_real_payload     = FLEXCAN_ComputePayloadSize(flexcan_mb_dlc_value);
    OS_ASSERT(rxFifo != NULL);

    MsgData_32      = (uint32_t *)(rxFifo->data);
    rxFifo->dataLen = can_real_payload;
    rxFifo->cs      = *Flexcan_Mb;

    if ((rxFifo->cs & FLEXCAN_CS_IDE_MASK) != 0U) {
        rxFifo->msgId = *Flexcan_Mb_Id;
    } else {
        rxFifo->msgId = (*Flexcan_Mb_Id) >> FLEXCAN_ID_STD_SHIFT;
    }

    if ((rxFifo->cs & FLEXCAN_CS_RTR_MASK) != 0U) {
        can_real_payload = 0U;
    }

    Idhit_Offset = (can_real_payload >> 2U) + (((can_real_payload % 4U) != 0U) ? 1U : 0U);

    /* Extract the IDHIT */
    rxFifo->id_hit =
        (uint8_t)(((Flexcan_Mb_Data_32[Idhit_Offset]) & FLEXCAN_ENHANCED_IDHIT_MASK) >> FLEXCAN_ENHANCED_IDHIT_SHIFT);
    /* Extract the Time Stamp */
    if (FLEXCAN_IsHRTimeStampEnabled(base)) {
        rxFifo->time_stamp = (uint32_t)(Flexcan_Mb_Data_32[Idhit_Offset + 1U]);
    } else {
        rxFifo->time_stamp = (uint32_t)((rxFifo->cs & FLEXCAN_CS_TIME_STAMP_MASK) >> FLEXCAN_CS_TIME_STAMP_SHIFT);
    }

    /* Copy EnhancedRxFIFO data field into user's buffer */
    for (DataByte = 0U; DataByte < can_real_payload; DataByte += 4U) {
        MbWord = Flexcan_Mb_Data_32[DataByte >> 2U];
        FlexcanSwapBytesInWord((MbWord), (MsgData_32[DataByte >> 2U]));
    }
}

/*
 * Function Name : FLEXCAN_ProcessIRQHandlerEnhancedRxFIFO
 * Description   : Process IRQHandler in case of Enhanced RxFIFO mode selection for CAN interface.
 * note: just use in FlexCAN_IRQHandler
 */
static inline void FLEXCAN_ProcessIRQHandlerEnhancedRxFIFO(uint8_t Instance)
{
    const can_type_t *base = g_flexcanBase[Instance];
    uint32_t u32intType    = 0U;

    /* Get the interrupts that are enabled and ready */
    for (u32intType = FLEXCAN_ENHANCED_RXFIFO_UNDERFLOW; u32intType >= FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE;
         u32intType--) {
        if ((uint8_t)0U != FLEXCAN_GetEnhancedRxFIFOStatusFlag(base, u32intType)) {
            if ((uint8_t)0U != FLEXCAN_GetEnhancedRxFIFOIntStatusFlag(base, u32intType)) {
                FLEXCAN_IRQHandlerEnhancedRxFIFO(Instance, u32intType);
            }
        }
    }
}

/*
 * Function Name : FLEXCAN_IRQHandlerEnhancedRxFIFO
 * Description   : Process IRQHandler in case of Enhanced RxFIFO mode selection for CAN interface.
 *
 */
static void FLEXCAN_IRQHandlerEnhancedRxFIFO(uint8_t Instance, uint32_t intType)
{
    can_type_t *base       = g_flexcanBase[Instance];
    flexcan_state_t *state = g_flexcanStatePtr[Instance];
    flexcan_msgbuff_t data;

    switch (intType) {
    case FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE:
        if (FLEXCAN_MB_RX_BUSY == state->enhancedFifoOutput.state) {
            /* If use pass NULL_PTR, they can get data in callback function by getting
             * state->mbs[FLEXCAN_IP_MB_HANDLE_RXFIFO].pMBmessage */
            if (NULL == state->enhancedFifoOutput.mb_message) {
                state->enhancedFifoOutput.mb_message = &data;
            }

            /* Get Enhanced RX FIFO field values */
            FLEXCAN_ReadEnhancedRxFifo(base, state->enhancedFifoOutput.mb_message);

            FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, intType);
            FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_WATERMARK);
            FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_OVERFLOW);
            state->enhancedFifoOutput.state = FLEXCAN_MB_IDLE;

            /* Invoke callback */
            if (NULL != state->callback) {
                state->callback(Instance, FLEXCAN_EVENT_ENHANCED_RXFIFO_COMPLETE, FLEXCAN_MB_ENHANCED_RXFIFO, state);
            }

            if (FLEXCAN_MB_IDLE == state->enhancedFifoOutput.state) {
                /* Complete receive data */
                FLEXCAN_CompleteRxMessageEnhancedFifoData(Instance);
            }
        }

        break;
    case FLEXCAN_ENHANCED_RXFIFO_WATERMARK:
        FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, intType);

        /* Invoke callback */
        if (NULL != state->callback) {
            state->callback(Instance, FLEXCAN_EVENT_ENHANCED_RXFIFO_WATERMARK, FLEXCAN_MB_ENHANCED_RXFIFO, state);
        }

        break;
    case FLEXCAN_ENHANCED_RXFIFO_OVERFLOW:
        FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, intType);

        /* Invoke callback */
        if (NULL != state->callback) {
            state->callback(Instance, FLEXCAN_EVENT_ENHANCED_RXFIFO_OVERFLOW, FLEXCAN_MB_ENHANCED_RXFIFO, state);
        }

        break;
    case FLEXCAN_ENHANCED_RXFIFO_UNDERFLOW:
        FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, intType);

        /* Invoke callback */
        if (NULL != state->callback) {
            state->callback(Instance, FLEXCAN_EVENT_ENHANCED_RXFIFO_UNDERFLOW, FLEXCAN_MB_ENHANCED_RXFIFO, state);
        }

        break;
    default:
        /* Do Nothing */
        break;
    }
}

/*
 * Function Name : FLEXCAN_CompleteRxFifoDataDMA
 * Description   : Finish up a PDMA transfer (this is just a wrapper over
 * FLEXCAN_DRV_CompleteRxMessageFifoData).
 * This is not a public API as it is called from other driver functions.
 *
 */
static void FLEXCAN_CompleteEnhanceRxFifoDataDMA(void *parameter, pdma_chn_status_t status)
{
    uint32_t instance = (uint32_t)parameter;

    if (PDMA_CHN_ERROR == status) {
        flexcan_state_t *state          = g_flexcanStatePtr[instance];
        state->enhancedFifoOutput.state = FLEXCAN_MB_DMA_ERROR;

        if (NULL != state->callback) {
            state->callback((uint8_t)instance, FLEXCAN_EVENT_DMA_ERROR, FLEXCAN_MB_ENHANCED_RXFIFO, state);
        }
    }

    FLEXCAN_CompleteRxMessageEnhancedFifoData((uint8_t)instance);
}

/*
 * Function Name : FLEXCAN_CompleteRxMessageEnhancedFifoData
 * Description   : Finish up a receive by completing the process of receiving
 * data and disabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 */
static void FLEXCAN_CompleteRxMessageEnhancedFifoData(uint8_t instance)
{
    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    uint32_t u32MbCnt = 0U;
    uint8_t j;
    flexcan_msgbuff_t *fifo_message = NULL;
    uint32_t *msgData_32            = NULL;
    uint8_t flexcan_mb_dlc_value    = 0U;
    uint8_t can_dlc_payload         = 0U;
    uint8_t can_real_payload        = 0U;

    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(FLEXCAN_IsEnhancedRxFifoAvailable(base));

    if (false == state->enhancedFifoOutput.isPolling) {
        /* Reset to default value to avoid re-enable when calling EnableInterrupt */
        state->enhancedFifoOutput.isPolling = true;
        /* Disable Enhanced RX FIFO interrupts*/
        FLEXCAN_SetEnhancedRxFifoIntAll(base, false);
    } else if (FLEXCAN_RXFIFO_USING_DMA == state->transferType) {

        if (state->enhancedFifoOutput.state != FLEXCAN_MB_DMA_ERROR) {
            fifo_message = state->enhancedFifoOutput.mb_message;

            (void)PDMA_DRV_StopChannel(state->rxFifoDMAChannel);
            for (u32MbCnt = 0U; u32MbCnt < state->u32NumOfMbTransferByDMA; u32MbCnt++) {
                msgData_32 = &((uint32_t *)((uint32_t)fifo_message))[2U];
                /* Adjust the ID if it is not extended */
                if (0U == ((fifo_message->cs) & FLEXCAN_CS_IDE_MASK)) {
                    fifo_message->msgId = fifo_message->msgId >> FLEXCAN_ID_STD_SHIFT;
                }

                /* Extract the data length */
                flexcan_mb_dlc_value = (uint8_t)((fifo_message->cs & FLEXCAN_CS_DLC_MASK) >> FLEXCAN_CS_DLC_SHIFT);
                can_dlc_payload      = FLEXCAN_ComputePayloadSize(flexcan_mb_dlc_value);
                can_real_payload     = 0U;
                /* Extract the IDHIT and Time Stamp */
                if ((fifo_message->cs & FLEXCAN_CS_RTR_MASK) != 0U) {
                    can_real_payload = 0U;
                } else {
                    can_real_payload = can_dlc_payload;
                }

                uint8_t idhit_offset = (can_real_payload >> 2U) + (((can_real_payload % 4U) != 0U) ? 1U : 0U);
                /* Extract the Time Stamp */
                if (FLEXCAN_IsHRTimeStampEnabled(base)) {
                    fifo_message->time_stamp = (uint32_t)(msgData_32[idhit_offset + 1U]);
                } else {
                    fifo_message->time_stamp =
                        (uint32_t)((fifo_message->cs & FLEXCAN_CS_TIME_STAMP_MASK) >> FLEXCAN_CS_TIME_STAMP_SHIFT);
                }

                /* Extract the IDHIT */
                fifo_message->id_hit = (uint8_t)(((msgData_32[idhit_offset]) & FLEXCAN_ENHANCED_IDHIT_MASK) >>
                                                 FLEXCAN_ENHANCED_IDHIT_SHIFT);
                /* Extract the dataLen */
                fifo_message->dataLen = can_dlc_payload;
                /* Reverse the endianness */
                for (j = 0U; j < idhit_offset; j++) {
                    FlexcanSwapBytesInWord(msgData_32[j], msgData_32[j]);
                }

                fifo_message++;
            }
        } else {
            /* If Enhanced Rx FIFO has Pending Request that generated error,
             * the EnhancedRxFIFO need to be empty to activate PDMA */
            FLEXCAN_ClearOutputEnhanceFIFO(base);
        }
    } else {
        /* avoid misra */
    }

    /* Clear enhanced rx fifo message */
    state->enhancedFifoOutput.mb_message = NULL;

    if (FLEXCAN_MB_DMA_ERROR != state->enhancedFifoOutput.state) {
        state->enhancedFifoOutput.state = FLEXCAN_MB_IDLE;
        if ((state->callback != NULL) && (FLEXCAN_RXFIFO_USING_DMA == state->transferType)) {
            state->callback(instance, FLEXCAN_EVENT_DMA_COMPLETE, FLEXCAN_MB_ENHANCED_RXFIFO, state);
        }
    }

    /* Update status for receive by using fifo */
    if (true == state->enhancedFifoOutput.isBlocking) {
        if (FLEXCAN_MB_IDLE == state->enhancedFifoOutput.state) {
            OS_Status_t status = OS_SemaphoreRelease(state->enhancedFifoOutput.mbSema);
            OS_ASSERT(status == OS_OK);
            (void)status;
        }
    }
}

/*
 * Function Name : FLEXCAN_StartRxMessageEnhancedFifoData
 * Description   : Initiate (start) a receive by beginning the process of
 * receiving data and enabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 */
static status_t FLEXCAN_StartRxMessageEnhancedFifoData(uint8_t instance, flexcan_msgbuff_t *data, bool isBlocking)
{
    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];
    status_t eResult       = STATUS_SUCCESS;
    pdma_loop_transfer_config_t loopTransferConfig;
    pdma_transfer_config_t transferConfig;
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    /* Start receiving fifo */
    if (FLEXCAN_MB_RX_BUSY == state->enhancedFifoOutput.state) {
        eResult = FLEXCAN_STATUS_BUSY;
    } else {
        state->enhancedFifoOutput.state      = FLEXCAN_MB_RX_BUSY;
        state->enhancedFifoOutput.isBlocking = isBlocking;
        /* This will get filled by the interrupt handler */
        state->enhancedFifoOutput.mb_message = data;

        if (FLEXCAN_RXFIFO_USING_INTERRUPTS == state->transferType) {
            state->enhancedFifoOutput.isPolling = false;
            if (true == state->isIntActive) {
                /* Enable All Enhanced RX FIFO interrupts*/
                FLEXCAN_SetEnhancedRxFifoIntAll(base, true);
            }
        }

        if (FLEXCAN_RXFIFO_USING_POLLING == state->transferType) {
            state->enhancedFifoOutput.isPolling = true;
        }

        if (FLEXCAN_RXFIFO_USING_DMA == state->transferType) {
            /* reset to default to avoid enabling interrupt */
            state->enhancedFifoOutput.isPolling = true;
            status_t pdmaStatus;

            pdmaStatus = PDMA_DRV_InstallCallback(state->rxFifoDMAChannel, FLEXCAN_CompleteEnhanceRxFifoDataDMA,
                                                  (void *)((uint32_t)instance));

            if (pdmaStatus != STATUS_SUCCESS) {
                state->enhancedFifoOutput.state = FLEXCAN_MB_IDLE;
                return STATUS_ERROR;
            }

            /* Initiate pDMA Loop Transfer */
            loopTransferConfig.majorLoopIterationCount = state->u32NumOfMbTransferByDMA;
            loopTransferConfig.minorLoopOffset         = (-80);
            loopTransferConfig.srcOffsetEnable         = true;
            loopTransferConfig.dstOffsetEnable         = false;
            loopTransferConfig.majorLoopChnLinkEnable  = false;
            loopTransferConfig.majorLoopChnLinkNumber  = 0U;
            loopTransferConfig.minorLoopChnLinkEnable  = false;
            loopTransferConfig.minorLoopChnLinkNumber  = 0U;

            transferConfig.srcAddr                = ((uint32_t)base + (uint32_t)FEATURE_FLEXCAN_ENHANCED_FIFO_RAM_OFFSET);
            transferConfig.destAddr               = (uint32_t)(state->enhancedFifoOutput.mb_message);
            transferConfig.destOffset             = 4; // Add offset to destination address after each transfer
            transferConfig.srcOffset              = 4; // Add offset to source address after each transfer
            transferConfig.srcTransferSize        = PDMA_TRANSFER_SIZE_4B;
            transferConfig.destTransferSize       = PDMA_TRANSFER_SIZE_4B;
            transferConfig.srcLastAddrAdjust      = 0;
            transferConfig.destLastAddrAdjust     = 0;
            transferConfig.srcModulo              = PDMA_MODULO_OFF;
            transferConfig.destModulo             = PDMA_MODULO_OFF;
            transferConfig.minorByteTransferCount = 80; // Number of bytes that form a minor loop
            transferConfig.loopTransferConfig     = &loopTransferConfig;
            transferConfig.interruptEnable        = true;

            pdmaStatus = PDMA_DRV_ConfigLoopTransfer(state->rxFifoDMAChannel, &transferConfig);
            if (STATUS_SUCCESS != pdmaStatus) {
                state->enhancedFifoOutput.state = FLEXCAN_MB_IDLE;
                eResult                         = STATUS_ERROR;
            } else {
                PDMA_DRV_DisableRequestsOnTransferComplete(state->rxFifoDMAChannel, true);
                pdmaStatus = PDMA_DRV_StartChannel(state->rxFifoDMAChannel);
                if (STATUS_SUCCESS != pdmaStatus) {
                    state->enhancedFifoOutput.state = FLEXCAN_MB_IDLE;
                    eResult                         = STATUS_ERROR;
                }
            }
        }
    }

    return eResult;
}

/*
 * Function Name : FLEXCAN_ProccessEnhancedRxFifo
 * Description   : This function will process the enhanced RxFIFO in blocking mode.
 * This is not a public API as it is called from other driver functions.
 *
 */
static status_t FLEXCAN_ProccessEnhancedRxFifo(uint8_t Instance, uint32_t u32TimeoutMs)
{
    status_t eResult        = STATUS_SUCCESS;
    can_type_t *base        = g_flexcanBase[Instance];
    flexcan_state_t *pState = g_flexcanStatePtr[Instance];
    uint32_t timeStart      = 0U;
    uint32_t timeElapsed    = 0U;
    uint32_t mS2Ticks       = OS_Msec2Tick(u32TimeoutMs * 1000U);
    uint32_t u32intType     = 0U;

    timeStart = OS_KernelGetTickCount();
    if (FLEXCAN_RXFIFO_USING_POLLING == pState->transferType) {
        while (FLEXCAN_MB_RX_BUSY == pState->enhancedFifoOutput.state) {

            for (u32intType = FLEXCAN_ENHANCED_RXFIFO_UNDERFLOW; u32intType >= FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE;
                 u32intType--) {
                if (FLEXCAN_GetEnhancedRxFIFOStatusFlag(base, u32intType) != 0U) {
                    FLEXCAN_IRQHandlerEnhancedRxFIFO(Instance, u32intType);
                }
            }

            timeElapsed = OS_KernelGetTickCount() - timeStart;
            if (timeElapsed >= mS2Ticks) {
                return STATUS_TIMEOUT;
            }
        }
    } else {

        eResult = (status_t)OS_SemaphoreAcquire(pState->enhancedFifoOutput.mbSema, u32TimeoutMs);
        if (pState->transferType == FLEXCAN_RXFIFO_USING_INTERRUPTS) {
            /* Disable Enhanced RX FIFO interrupts*/
            FLEXCAN_SetEnhancedRxFifoIntAll(base, false);

        } else { /* FLEXCAN_RXFIFO_USING_DMA*/
            /* This function always return status success */
            (void)PDMA_DRV_StopChannel(pState->rxFifoDMAChannel);
        }
    }

    if (STATUS_TIMEOUT == eResult) {
        /* If the status is updated reception successful else report TimeOut */
        if (FLEXCAN_MB_IDLE == pState->enhancedFifoOutput.state) {
            eResult = STATUS_SUCCESS;
            /* add dummy wait for reset pState->enhancedFifoOutput.mbSema at beginning of block functions */
            (void)OS_SemaphoreAcquire(pState->enhancedFifoOutput.mbSema, 0);
        } else {
            pState->enhancedFifoOutput.state = FLEXCAN_MB_IDLE;
            FLEXCAN_ClearOutputEnhanceFIFO(base);
        }
    }

    return eResult;
}

/*
 * Function Name : FLEXCAN_IRQHandlerRxFIFO
 * Description   : Process IRQHandler in case of RxFIFO mode selection for CAN interface.
 *
 */
static inline void FLEXCAN_IRQHandlerRxFIFO(uint8_t instance, uint32_t mb_idx)
{
    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    if (FLEXCAN_RXFIFO_FRAME_AVAILABLE_SHIFT == mb_idx) {
        if (FLEXCAN_MB_RX_BUSY == state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state) {
            /* Get RX FIFO field values */
            FLEXCAN_ReadRxFifo(base, state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mb_message);

            FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);

            state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state = FLEXCAN_MB_IDLE;

            /* Invoke callback */
            if (NULL != state->callback) {
                state->callback(instance, FLEXCAN_EVENT_RXFIFO_COMPLETE, FLEXCAN_MB_HANDLE_RXFIFO, state);
            }

            if (FLEXCAN_MB_IDLE == state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state) {
                /* Complete receive data */
                FLEXCAN_CompleteRxMessageFifoData(instance);
            }
        }
    } else if (FLEXCAN_RXFIFO_WARNING_SHIFT == mb_idx) {
        FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);

        /* Invoke callback */
        if (NULL != state->callback) {
            state->callback(instance, FLEXCAN_EVENT_RXFIFO_WARNING, FLEXCAN_MB_HANDLE_RXFIFO, state);
        }
    } else if (FLEXCAN_RXFIFO_OVERFLOW_SHIFT == mb_idx) {
        FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);

        /* Invoke callback */
        if (NULL != state->callback) {
            state->callback(instance, FLEXCAN_EVENT_RXFIFO_OVERFLOW, FLEXCAN_MB_HANDLE_RXFIFO, state);
        }
    } else {
        /* Do Nothing */
    }
}

/*
 * Function Name : FLEXCAN_IRQHandlerRxMB
 * Description   : Process IRQHandler in case of Rx MessageBuffer selection
 * for CAN interface.
 *
 * This is not a public API as it is called whenever an interrupt and receive
 * individual MB occurs
 */
static void FLEXCAN_IRQHandlerRxMB(uint8_t instance, uint32_t mb_idx)
{
    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    /* Lock RX message buffer and RX FIFO */
    FLEXCAN_LockRxMsgBuff(base, mb_idx);

    /* Get RX MB field values */
    FLEXCAN_GetMsgBuff(base, mb_idx, state->mbs[mb_idx].mb_message);

    /* Clear the proper flag in the IFLAG register */
    FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
    /* the CODE field is updated with an incorrect value when MBx is locked by software for more than 20 CAN bit times
     * and FIFO enable */
    if ((FLEXCAN_IsRxFifoEnabled(base)) && (((state->mbs[mb_idx].mb_message->cs & FLEXCAN_CS_CODE_MASK) >>
                                             FLEXCAN_CS_CODE_SHIFT) == (uint32_t)FLEXCAN_RX_INACTIVE)) {
        /* Update the cs code for next sequence move in MB.
        A CPU write into the C/S word also unlocks the MB */
        volatile uint32_t *flexcan_mb = FLEXCAN_GetMsgBuffRegion(base, mb_idx);
        *flexcan_mb &= ~FLEXCAN_CS_CODE_MASK;
        *flexcan_mb |= (((uint32_t)FLEXCAN_RX_EMPTY) << FLEXCAN_CS_CODE_SHIFT) & FLEXCAN_CS_CODE_MASK;
    } else {
        /* Unlock RX message buffer and RX FIFO*/
        FLEXCAN_UnlockRxMsgBuff(base);
    }

    state->mbs[mb_idx].state = FLEXCAN_MB_IDLE;

    /* Invoke callback */
    if (NULL != state->callback) {
        state->callback(instance, FLEXCAN_EVENT_RX_COMPLETE, mb_idx, state);
    }

    if (FLEXCAN_MB_IDLE == state->mbs[mb_idx].state) {
        /* Complete receive data */
        FLEXCAN_CompleteTransfer(instance, mb_idx);
    }
}

/*
 * Function Name : FLEXCAN_BusOff_IRQHandler
 * Description   : BusOff and Tx/Rx Warning interrupt handler for FLEXCAN.
 * This handler only provides a error status report and invokes the user callback,
 * and then clears the interrupt flags.
 * This is not a public API as it is called whenever an interrupt occurs.
 *
 */
static void FLEXCAN_BusOff_IRQHandler(void *data)
{
    uint8_t instance = (uint8_t)(uint32_t)data;

    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    /* Invoke callback */
    if (NULL != state->error_callback) {
        state->error_callback(instance, FLEXCAN_EVENT_BUSOFF, state);
    }

    /* Clear all BusOff and Tx/Rx Warning interrupts in ESR1 register */
    FLEXCAN_ClearBusOffIntStatusFlag(base);
}

/*
 * Function Name : FLEXCAN_IRQHandler
 * Description   : Interrupt handler for FLEXCAN.
 * This handler read data from MB or FIFO, and then clear the interrupt flags.
 * This is not a public API as it is called whenever an interrupt occurs.
 *
 */
static void FLEXCAN_IRQHandler(void *data)
{
    uint8_t instance = (uint8_t)(uint32_t)data;

    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    uint32_t flag_reg      = 0;
    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];
    uint32_t maxNumMB      = FLEXCAN_GetMaxMbNum(base);

    /* Get the interrupts that are enabled and ready */
    uint32_t mb_idx = 0;
    flag_reg        = FLEXCAN_GetMsgBuffIntStatusFlag(base, mb_idx);

    while ((flag_reg & 1U) == 0U) {
        mb_idx++;
        flag_reg = FLEXCAN_GetMsgBuffIntStatusFlag(base, mb_idx);

        if (mb_idx >= maxNumMB) {
            break;
        }
    }

    /* Check Tx/Rx interrupt flag and clear the interrupt */
    if (flag_reg != 0U) {
        if (FLEXCAN_IsRxFifoEnabled(base) && (mb_idx <= FLEXCAN_RXFIFO_OVERFLOW_SHIFT)) {
            FLEXCAN_IRQHandlerRxFIFO(instance, mb_idx);
        } else {
            /* Check mailbox completed reception */
            if (FLEXCAN_MB_RX_BUSY == state->mbs[mb_idx].state) {
                FLEXCAN_IRQHandlerRxMB(instance, mb_idx);
            }
        }

        /* Check mailbox completed transmission */
        if (FLEXCAN_MB_TX_BUSY == state->mbs[mb_idx].state) {
            if (state->mbs[mb_idx].isRemote) {
                /* If the frame was a remote frame, clear the flag only if the response was
                 * not received yet. If the response was received, leave the flag set in order
                 * to be handled when the user calls FLEXCAN_DRV_RxMessageBuffer. */
                flexcan_msgbuff_t mb;
                FLEXCAN_LockRxMsgBuff(base, mb_idx);
                FLEXCAN_GetMsgBuff(base, mb_idx, &mb);
                FLEXCAN_UnlockRxMsgBuff(base);
                state->mbs[mb_idx].time_stamp = mb.time_stamp;

                if (((mb.cs & FLEXCAN_CS_CODE_MASK) >> FLEXCAN_CS_CODE_SHIFT) == (uint32_t)FLEXCAN_RX_EMPTY) {
                    FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
                }
            } else {
                state->mbs[mb_idx].time_stamp = FLEXCAN_GetMsgBuffTimestamp(base, mb_idx);
                FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
            }

            state->mbs[mb_idx].state = FLEXCAN_MB_IDLE;

            /* Invoke callback */
            if (NULL != state->callback) {
                state->callback(instance, FLEXCAN_EVENT_TX_COMPLETE, mb_idx, state);
            }

            if (FLEXCAN_MB_IDLE == state->mbs[mb_idx].state) {
                /* Complete transmit data */
                FLEXCAN_CompleteTransfer(instance, mb_idx);
            }
        }

        if (FLEXCAN_GetMsgBuffIntStatusFlag(base, mb_idx) != 0U) {
            if (FLEXCAN_IsRxFifoEnabled(base) && (mb_idx <= FLEXCAN_RXFIFO_OVERFLOW_SHIFT)) {
                if (state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state == FLEXCAN_MB_IDLE) {
                    /* In case of desynchronizing status of the MB to avoid trapping in ISR
                     * clear the MB flag */
                    FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
                }
            } else {
                if (state->mbs[mb_idx].state == FLEXCAN_MB_IDLE) {
                    /* In case of desynchronizing status of the MB to avoid trapping in ISR
                     * clear the MB flag */
                    FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
                }
            }
        }
    }

    if (FLEXCAN_IsEnhancedRxFifoAvailable(base)) {
        if ((true == FLEXCAN_IsEnhancedRxFifoEnabled(base)) &&
            (FLEXCAN_RXFIFO_USING_INTERRUPTS == state->transferType)) {
            FLEXCAN_ProcessIRQHandlerEnhancedRxFIFO(instance);
        }
    }

    return;
}

/*
 * Function Name : FLEXCAN_TxWarning_IRQHandler
 * Description   : Tx warning interrupt handler for FLEXCAN.
 * This handler read data from MB or FIFO, and then clear the interrupt flags.
 * This is not a public API as it is called whenever an interrupt occurs.
 *
 */
static void FLEXCAN_TxWarning_IRQHandler(void *data)
{
    uint8_t instance = (uint8_t)(uint32_t)data;

    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    /* Invoke callback */
    if (NULL != state->error_callback) {
        state->error_callback(instance, FLEXCAN_EVENT_TX_WARNING, state);
    }

    /* Clear all other interrupts in ESR1 register (Error, Wakeup) */
    // FLEXCAN_ClearErrIntStatusFlag(base);
    /* Clear all BusOff and Tx/Rx Warning interrupts in ESR1 register */
    FLEXCAN_ClearBusOffIntStatusFlag(base);

    return;
}

/*
 * Function Name : FLEXCAN_Error_IRQHandler
 * Description   : Error interrupt handler for FLEXCAN.
 * This handler read data from MB or FIFO, and then clear the interrupt flags.
 * This is not a public API as it is called whenever an interrupt occurs.
 *
 */
static void FLEXCAN_Error_IRQHandler(void *data)
{
    uint8_t instance = (uint8_t)(uint32_t)data;

    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    /* Invoke callback */
    if (NULL != state->error_callback) {
        state->error_callback(instance, FLEXCAN_EVENT_ERROR, state);
    }

    /* Clear all other interrupts in ESR1 register (Error, Wakeup) */
    FLEXCAN_ClearErrIntStatusFlag(base);

    return;
}

/*
 * Function Name : FLEXCAN_WakeUpHandler
 * Description   : Wake up handler for FLEXCAN.
 * This handler verifies the event which caused the wake up and invokes the
 * user callback, if configured.
 * This is not a public API as it is called whenever an wake up event occurs.
 *
 */
static void FLEXCAN_WakeUpHandler(void *data)
{
    uint8_t instance = (uint8_t)(uint32_t)data;

    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    if (FLEXCAN_IsPNEnabled(base)) {
        if (FLEXCAN_GetWTOF(base) != 0U) {
            FLEXCAN_ClearWTOF(base);
            /* Invoke callback */
            if (NULL != state->callback) {
                state->callback(instance, FLEXCAN_EVENT_WAKEUP_TIMEOUT, 0U, state);
            }
        }

        if (FLEXCAN_GetWUMF(base) != 0U) {
            FLEXCAN_ClearWUMF(base);
            /* Invoke callback */
            if (NULL != state->callback) {
                state->callback(instance, FLEXCAN_EVENT_WAKEUP_MATCH, 0U, state);
            }
        }
    }
}

/*
 * Function Name : FLEXCAN_DRV_GetTransferStatus
 * Description   : This function returns whether the previous FLEXCAN receive is
 *                 completed.
 * When performing a non-blocking receive, the user can call this function to
 * ascertain the state of the current receive progress: in progress (or busy)
 * or complete (success).
 *
 */
status_t FLEXCAN_DRV_GetTransferStatus(uint8_t instance, uint8_t mb_idx)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    const flexcan_state_t *state = g_flexcanStatePtr[instance];
    status_t result              = STATUS_SUCCESS;

    const can_type_t *base = g_flexcanBase[instance];
    if (true == FLEXCAN_IsEnhancedRxFifoAvailable(base)) {
        if (true == FLEXCAN_IsEnhancedRxFifoEnabled(base)) {
            if (FLEXCAN_MB_IDLE == state->enhancedFifoOutput.state) {
                result = STATUS_SUCCESS;
            } else if (FLEXCAN_MB_DMA_ERROR == state->enhancedFifoOutput.state) {
                result = STATUS_ERROR;
            } else {
                result = STATUS_BUSY;
            }
        } else {
            if (FLEXCAN_MB_IDLE == state->mbs[mb_idx].state) {
                result = STATUS_SUCCESS;
            } else if (FLEXCAN_MB_DMA_ERROR == state->mbs[mb_idx].state) {
                result = STATUS_ERROR;
            } else {
                result = STATUS_BUSY;
            }
        }
    } else {
        if (FLEXCAN_MB_IDLE == state->mbs[mb_idx].state) {
            result = STATUS_SUCCESS;
        } else if (FLEXCAN_MB_DMA_ERROR == state->mbs[mb_idx].state) {
            result = STATUS_ERROR;
        } else {
            result = STATUS_BUSY;
        }
    }

    return result;
}

/*
 * Function Name : FLEXCAN_DRV_GetErrorStatus
 * Description   : Reports various error conditions detected in the reception and
 *                 transmission of a CAN frame and some general status of the device.
 *
 */
uint32_t FLEXCAN_DRV_GetErrorStatus(uint8_t instance)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    const can_type_t *base = g_flexcanBase[instance];

    return ((uint32_t)(base->ESR1));
}

/*
 * Function Name : FLEXCAN_DRV_AbortTransfer
 * Description   : This function shuts down the FLEXCAN by disabling interrupts and
 *                 the transmitter/receiver.
 * This function disables the FLEXCAN interrupts, disables the transmitter and
 * receiver.
 *
 */
status_t FLEXCAN_DRV_AbortTransfer(uint8_t instance, uint8_t mb_idx)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    flexcan_state_t *state = g_flexcanStatePtr[instance];
    can_type_t *base       = g_flexcanBase[instance];

    /* Check if a transfer is running. */
    if (FLEXCAN_MB_IDLE == state->mbs[mb_idx].state) {
        return STATUS_CAN_NO_TRANSFER_IN_PROGRESS;
    }

    if (FLEXCAN_MB_TX_BUSY == state->mbs[mb_idx].state) {
        status_t result = STATUS_SUCCESS;
        /* Stop the running transfer. */
        FLEXCAN_CompleteTransfer(instance, mb_idx);
        /* Clear message buffer flag */
        FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
        FLEXCAN_AbortTxMsgBuff(base, mb_idx);
        uint32_t flexcan_mb_config;

        /* Wait to finish abort operation */
        while (FLEXCAN_GetBuffStatusFlag(base, mb_idx) == 0U) {
            /* Do Nothing wait for the flag */
        }

        volatile const uint32_t *flexcan_mb = FLEXCAN_GetMsgBuffRegion(base, mb_idx);
        flexcan_mb_config                   = *flexcan_mb;
        /* Check if the MBs have been safely Inactivated */
        if (((flexcan_mb_config & FLEXCAN_CS_CODE_MASK) >> FLEXCAN_CS_CODE_SHIFT) == (uint32_t)FLEXCAN_TX_INACTIVE) {
            /* Transmission have occurred */
            result = STATUS_CAN_NO_TRANSFER_IN_PROGRESS;
        }
        if (((flexcan_mb_config & FLEXCAN_CS_CODE_MASK) >> FLEXCAN_CS_CODE_SHIFT) == (uint32_t)FLEXCAN_TX_ABORT) {
            /* Transmission have been aborted */
            result = STATUS_SUCCESS;
        }
        /* Clear message buffer flag */
        FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
        state->mbs[mb_idx].state = FLEXCAN_MB_IDLE;
        return result;
    }

    if (FLEXCAN_MB_RX_BUSY == state->mbs[mb_idx].state) {
        /* Stop the running transfer. */
        FLEXCAN_CompleteTransfer(instance, mb_idx);

        if (true == FLEXCAN_IsRxFifoEnabled(base)) {

            /* Get the number of RX FIFO Filters*/
            uint32_t val1 = (((base->CTRL2) & CAN_CTRL2_RFFN_MASK) >> CAN_CTRL2_RFFN_SHIFT);
            /* Get the number if MBs occupied by RX FIFO and ID filter table*/
            /* the Rx FIFO occupies the memory space originally reserved for MB0-5*/
            /* Every number of RFFN means 8 number of RX FIFO filters*/
            /* and every 4 number of RX FIFO filters occupied one MB*/
            uint32_t val2 = RxFifoOccupiedLastMsgBuff(val1);
            if (mb_idx > val2) {
                /* This operation is not allowed for MB that are part of RxFIFO */
                FLEXCAN_ResetRxMsgBuff(base, mb_idx);
            }

            if (FLEXCAN_MB_HANDLE_RXFIFO == mb_idx) {
                /* Disable RX FIFO interrupts*/
                (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_FRAME_AVAILABLE_SHIFT, false);
                if (FLEXCAN_RXFIFO_USING_DMA == state->transferType) {
                    (void)PDMA_DRV_StopChannel(state->rxFifoDMAChannel);
                }
            }
        } else {
            /* This operation is not allowed for MB that are part of RxFIFO */
            FLEXCAN_ResetRxMsgBuff(base, mb_idx);
        }
    }

    if (1U == FLEXCAN_GetMsgBuffIntStatus(base, mb_idx)) {
        /* Stop the running transfer. */
        FLEXCAN_CompleteTransfer(instance, mb_idx);
        /* Clear message buffer flag */
        FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
    }

    return STATUS_SUCCESS;
}

/*
 * Function Name : FLEXCAN_StartSendData
 * Description   : Initiate (start) a transmit by beginning the process of
 * sending data.
 * This is not a public API as it is called from other driver functions.
 *
 */
static status_t FLEXCAN_StartSendData(uint8_t instance, uint8_t mb_idx, const flexcan_data_info_t *tx_info,
                                      uint32_t msg_id, const uint8_t *mb_data, bool isBlocking)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(tx_info != NULL);

    status_t result;
    flexcan_msgbuff_code_status_t cs;
    flexcan_state_t *state = g_flexcanStatePtr[instance];
    can_type_t *base       = g_flexcanBase[instance];
    bool instHasFd         = FLEXCAN_InstanceHasFD(instance);

    if (mb_idx >= FLEXCAN_GetMaxMbNum(base)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    if (state->mbs[mb_idx].state != FLEXCAN_MB_IDLE) {
        return STATUS_BUSY;
    }

    /* Clear message buffer flag */
    FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);

    state->mbs[mb_idx].state      = FLEXCAN_MB_TX_BUSY;
    state->mbs[mb_idx].time_stamp = 0U;
    state->mbs[mb_idx].isBlocking = isBlocking;
    state->mbs[mb_idx].isRemote   = tx_info->is_remote;

    cs.dataLen   = tx_info->data_length;
    cs.msgIdType = tx_info->msg_id_type;
    cs.enable_brs = false;
    cs.fd_enable  = false;
    cs.fd_padding = tx_info->fd_padding;

    if (instHasFd) {
        cs.fd_enable  = tx_info->fd_enable;
        cs.fd_padding = tx_info->fd_padding;
        cs.enable_brs = tx_info->enable_brs;
    }

    if (tx_info->is_remote) {
        cs.code = (uint32_t)FLEXCAN_TX_REMOTE;
    } else {
        cs.code = (uint32_t)FLEXCAN_TX_DATA;
    }
    result = FLEXCAN_SetTxMsgBuff(base, mb_idx, &cs, msg_id, mb_data, false);

    if (STATUS_SUCCESS != result) {
        state->mbs[mb_idx].state = FLEXCAN_MB_IDLE;
    }

    return result;
}

/*
 * Function Name : FLEXCAN_StartRxMessageBufferData
 * Description   : Initiate (start) a receive by beginning the process of
 * receiving data and enabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 */
static status_t FLEXCAN_StartRxMessageBufferData(uint8_t instance, uint8_t mb_idx, flexcan_msgbuff_t *data,
                                                 bool isBlocking)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    status_t result        = STATUS_SUCCESS;
    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    /* Check if the MB index is in range */
    if (mb_idx >= FLEXCAN_GetMaxMbNum(base)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    /* Check if RX FIFO is enabled */
    if (true == FLEXCAN_IsRxFifoEnabled(base)) {
        uint32_t val1, val2;
        /* Get the number of RX FIFO Filters*/
        val1 = (((base->CTRL2) & CAN_CTRL2_RFFN_MASK) >> CAN_CTRL2_RFFN_SHIFT);
        /* Get the number if MBs occupied by RX FIFO and ID filter table*/
        /* the Rx FIFO occupies the memory space originally reserved for MB0-5*/
        /* Every number of RFFN means 8 number of RX FIFO filters*/
        /* and every 4 number of RX FIFO filters occupied one MB*/
        val2 = RxFifoOccupiedLastMsgBuff(val1);

        if (mb_idx <= val2) {
            return STATUS_CAN_BUFF_OUT_OF_RANGE;
        }
    }

    /* Start receiving mailbox */
    if (state->mbs[mb_idx].state != FLEXCAN_MB_IDLE) {
        return STATUS_BUSY;
    }
    state->mbs[mb_idx].state      = FLEXCAN_MB_RX_BUSY;
    state->mbs[mb_idx].mb_message = data;
    state->mbs[mb_idx].isBlocking = isBlocking;

    /* Enable MB interrupt */
    result = FLEXCAN_SetMsgBuffIntCmd(base, mb_idx, true);

    if (STATUS_SUCCESS != result) {
        state->mbs[mb_idx].state = FLEXCAN_MB_IDLE;
    }

    return result;
}

/*
 * Function Name : FLEXCAN_StartRxMessageFifoData
 * Description   : Initiate (start) a receive by beginning the process of
 * receiving data and enabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 */
static status_t FLEXCAN_StartRxMessageFifoData(uint8_t instance, flexcan_msgbuff_t *data, bool isBlocking)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];
    status_t pdmaStat;

    if (FLEXCAN_RXFIFO_USING_DMA == state->transferType) {
        if (FLEXCAN_MB_DMA_ERROR == state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state) {
            /* Check if FIFO has Pending Request that generated error,
             * the RxFIFO need to be empty to activate PDMA */
            if ((uint8_t)1U == FLEXCAN_GetBuffStatusFlag(base, FLEXCAN_RXFIFO_FRAME_AVAILABLE_SHIFT)) {
                /* Enter CAN in freeze Mode to allow FIFO Clear */
                FLEXCAN_EnterFreezeMode(base);

                FLEXCAN_ClearFIFO(base);
                do { /* Read Offset 0x8C to clear PDMA pending request */
                    (void)base->RAMn[3];
                } while ((uint8_t)1U == FLEXCAN_GetBuffStatusFlag(base, FLEXCAN_RXFIFO_FRAME_AVAILABLE_SHIFT));

                /* Return CAN to normal Mode */
                FLEXCAN_ExitFreezeMode(base);
            }
            /* Change status of MB to be reconfigured with PDMA transfer */
            state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state = FLEXCAN_MB_IDLE;
        }
    }

    /* Start receiving fifo */
    if (FLEXCAN_MB_IDLE != state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state) {
        return STATUS_BUSY;
    }
    /* Check if RxFIFO feature is enabled */
    if (!FLEXCAN_IsRxFifoEnabled(base)) {
        return STATUS_ERROR;
    }

    state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state = FLEXCAN_MB_RX_BUSY;

    state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].isBlocking = isBlocking;

    /* This will get filled by the interrupt handler */
    state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mb_message = data;

    if (FLEXCAN_RXFIFO_USING_INTERRUPTS == state->transferType) {
        /* Enable RX FIFO interrupts */
        (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_WARNING_SHIFT, true);
        (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_OVERFLOW_SHIFT, true);
        (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_FRAME_AVAILABLE_SHIFT, true);
    }

    if (FLEXCAN_RXFIFO_USING_DMA == state->transferType) {
        status_t pdmaStatus;

        pdmaStatus = PDMA_DRV_InstallCallback(state->rxFifoDMAChannel, FLEXCAN_CompleteRxFifoDataDMA,
                                              (void *)((uint32_t)instance));

        if (STATUS_SUCCESS != pdmaStatus) {
            state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state = FLEXCAN_MB_IDLE;
            return STATUS_ERROR;
        }

        pdmaStatus = PDMA_DRV_ConfigSingleBlockTransfer(
            state->rxFifoDMAChannel, PDMA_TRANSFER_MEM2MEM, (uint32_t)(base->RAMn),
            (uint32_t)(state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mb_message), PDMA_TRANSFER_SIZE_4B, 16U);

        if (STATUS_SUCCESS != pdmaStatus) {
            state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state = FLEXCAN_MB_IDLE;
            return STATUS_ERROR;
        }

        PDMA_DRV_DisableRequestsOnTransferComplete(state->rxFifoDMAChannel, true);

        pdmaStat = PDMA_DRV_StartChannel(state->rxFifoDMAChannel);
        if (STATUS_SUCCESS != pdmaStat) {
            state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state = FLEXCAN_MB_IDLE;
            return STATUS_ERROR;
        }
    }

    return STATUS_SUCCESS;
}

/*
 * Function Name : FLEXCAN_CompleteTransfer
 * Description   : Finish up a transmit by completing the process of sending
 * data and disabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 */
static void FLEXCAN_CompleteTransfer(uint8_t instance, uint32_t mb_idx)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    /* Disable the transmitter data register empty interrupt */
    (void)FLEXCAN_SetMsgBuffIntCmd(base, mb_idx, false);

    /* Update the information of the module driver state */
    if (state->mbs[mb_idx].isBlocking) {
        (void)OS_SemaphoreRelease(state->mbs[mb_idx].mbSema);
    }
    state->mbs[mb_idx].state = FLEXCAN_MB_IDLE;
}

/*
 * Function Name : FLEXCAN_CompleteRxFifoDataDMA
 * Description   : Finish up a PDMA transfer (this is just a wrapper over
 * FLEXCAN_DRV_CompleteRxMessageFifoData).
 * This is not a public API as it is called from other driver functions.
 *
 */
static void FLEXCAN_CompleteRxFifoDataDMA(void *parameter, pdma_chn_status_t status)
{
    uint32_t instance = (uint32_t)parameter;

    if (PDMA_CHN_ERROR == status) {
        flexcan_state_t *state                     = g_flexcanStatePtr[instance];
        state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state = FLEXCAN_MB_DMA_ERROR;

        if (NULL != state->callback) {
            state->callback((uint8_t)instance, FLEXCAN_EVENT_DMA_ERROR, FLEXCAN_MB_HANDLE_RXFIFO, state);
        }
    }
    FLEXCAN_CompleteRxMessageFifoData((uint8_t)instance);
}

/*
 * Function Name : FLEXCAN_CompleteRxMessageFifoData
 * Description   : Finish up a receive by completing the process of receiving
 * data and disabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 */
static void FLEXCAN_CompleteRxMessageFifoData(uint8_t instance)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];
    /* if using interrupt , data read in FLEXCAN_ReadRxFifo */
    if (FLEXCAN_RXFIFO_USING_INTERRUPTS == state->transferType) {
        /* Disable RX FIFO interrupts */
        (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_FRAME_AVAILABLE_SHIFT, false);
        (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_WARNING_SHIFT, false);
        (void)FLEXCAN_SetMsgBuffIntCmd(base, FLEXCAN_RXFIFO_OVERFLOW_SHIFT, false);
    } else { /* if using dma, data read in here */
        if (state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state != FLEXCAN_MB_DMA_ERROR) {
            flexcan_msgbuff_t *fifo_message = state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mb_message;
            uint32_t *msgData_32            = (uint32_t *)fifo_message->data;

            (void)PDMA_DRV_StopChannel(state->rxFifoDMAChannel);
            /* Adjust the ID if it is not extended */
            if (((fifo_message->cs) & FLEXCAN_CS_IDE_MASK) == 0U) {
                fifo_message->msgId = fifo_message->msgId >> FLEXCAN_ID_STD_SHIFT;
            }
            /* Extract the data length */
            fifo_message->dataLen = (uint8_t)((fifo_message->cs & FLEXCAN_CS_DLC_MASK) >> FLEXCAN_CS_DLC_SHIFT);
            /* Extract the Time Stamp */
            fifo_message->time_stamp =
                (uint32_t)((fifo_message->cs & FLEXCAN_CS_TIME_STAMP_MASK) >> FLEXCAN_CS_TIME_STAMP_SHIFT);
            /* Reverse the endianess */
            FlexcanSwapBytesInWord(msgData_32[0], msgData_32[0]);
            FlexcanSwapBytesInWord(msgData_32[1], msgData_32[1]);
        }
    }
    /* Clear fifo message*/
    state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mb_message = NULL;

    if (FLEXCAN_MB_DMA_ERROR != state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state) {
        state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state = FLEXCAN_MB_IDLE;
        if ((NULL != state->callback) && (FLEXCAN_RXFIFO_USING_DMA == state->transferType)) {
            state->callback(instance, FLEXCAN_EVENT_DMA_COMPLETE, FLEXCAN_MB_HANDLE_RXFIFO, state);
        }
    }

    /* Update status for receive by using fifo */
    if (true == state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].isBlocking) {
        if (FLEXCAN_MB_IDLE == state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].state) {
            OS_Status_t status = OS_SemaphoreRelease(state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mbSema);
            OS_ASSERT(status == OS_OK);
            (void)status;
        }
    }
}

/*
 * Function Name : FLEXCAN_DRV_InstallEventCallback
 * Description   : Installs a callback function for the IRQ handler.
 *
 */
void FLEXCAN_DRV_InstallEventCallback(uint8_t instance, flexcan_callback_t callback, void *callbackParam)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    flexcan_state_t *state = g_flexcanStatePtr[instance];

    state->callback      = callback;
    state->callbackParam = callbackParam;
}

/*
 * Function Name : FLEXCAN_DRV_InstallErrorCallback
 * Description   : Installs an error callback function for the IRQ handler and enables/disables
 *                 error interrupts.
 *
 */
void FLEXCAN_DRV_InstallErrorCallback(uint8_t instance, flexcan_error_callback_t callback, void *callbackParam)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    state->error_callback     = callback;
    state->errorCallbackParam = callbackParam;

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    if (NULL != callback) {
        FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_ERR, true);
        FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_RX_WARNING, true);
        FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_TX_WARNING, true);
        FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_BUSOFF, true);
    } else {
        FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_ERR, false);
        FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_RX_WARNING, false);
        FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_TX_WARNING, false);
        FLEXCAN_SetErrIntCmd(base, FLEXCAN_INT_BUSOFF, false);
    }

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_ConfigPN
 * Description   : Configures Pretended Networking settings.
 *
 */
void FLEXCAN_DRV_ConfigPN(uint8_t instance, bool enable, const flexcan_pn_config_t *pnConfig)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    can_type_t *base = g_flexcanBase[instance];

    bool freeze = FLEXCAN_GetFreezeMode(base);
    if (false == freeze) {
        FLEXCAN_EnterFreezeMode(base);
    }

    if (enable) {
        FLEXCAN_ConfigPN(base, pnConfig);
    }

    FLEXCAN_SetPN(base, enable);

    if (false == freeze) {
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_DRV_GetWMB
 * Description   : Extracts one of the frames which triggered the wake up event.
 *
 */
void FLEXCAN_DRV_GetWMB(uint8_t instance, uint8_t wmbIndex, flexcan_msgbuff_t *wmb)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    OS_ASSERT(wmb != NULL);
    const can_type_t *base = g_flexcanBase[instance];
    uint32_t *tmp, wmbData;

    tmp     = (uint32_t *)&wmb->data[0];
    wmbData = base->WMB[wmbIndex].WMBn_D03;
    FlexcanSwapBytesInWord(wmbData, *tmp);

    tmp     = (uint32_t *)&wmb->data[4];
    wmbData = base->WMB[wmbIndex].WMBn_D47;
    FlexcanSwapBytesInWord(wmbData, *tmp);

    wmb->cs = base->WMB[wmbIndex].WMBn_CS;

    if ((wmb->cs & FLEXCAN_CS_IDE_MASK) != 0U) {
        wmb->msgId = base->WMB[wmbIndex].WMBn_ID;
    } else {
        wmb->msgId = base->WMB[wmbIndex].WMBn_ID >> FLEXCAN_ID_STD_SHIFT;
    }

    wmb->dataLen = (uint8_t)((wmb->cs & FLEXCAN_CS_DLC_MASK) >> 16);
}

/*
 * Function Name : FLEXCAN_CheckSample
 * Description   : Check the Sample value
 *
 */
static inline uint32_t FLEXCAN_CheckSample(uint32_t tmpSample, uint32_t samplePoint)
{
    if (tmpSample > samplePoint) {
        return (tmpSample - samplePoint);
    }
    return (samplePoint - tmpSample);
}

/*
 * Function Name : FLEXCAN_CheckBitrate
 * Description   : Check the Bitrate value
 *
 */
static inline uint32_t FLEXCAN_CheckBitrate(uint32_t tmpBitrate, uint32_t bitrate)
{
    if (tmpBitrate > bitrate) {
        return (tmpBitrate - bitrate);
    }
    return (bitrate - tmpBitrate);
}

/*
 * Function Name : FLEXCAN_CheckJumpWidth
 * Description   : Check the JumpWidth value
 *
 */
static inline uint32_t FLEXCAN_CheckJumpWidth(uint32_t pseg1)
{
    if (pseg1 < FLEXCAN_RJW_MAX) {
        return pseg1;
    }
    return FLEXCAN_RJW_MAX;
}

/*
 * Function Name : FLEXCAN_ProccessTSeg
 * Description   : Calculate Tseg value
 *
 */
static inline void FLEXCAN_ProccessTSeg(uint32_t *tSeg1, uint32_t *tSeg2)
{
    /* Adjust time segment 1 and time segment 2 */
    while ((*tSeg1 >= FLEXCAN_TSEG1_MAX) || (*tSeg2 < FLEXCAN_TSEG2_MIN)) {
        *tSeg2 = *tSeg2 + 1U;
        *tSeg1 = *tSeg1 - 1U;
    }
}

/*
 * Function Name : FLEXCAN_ProccessPSeg
 * Description   : Calculate Pseg value
 *
 */
static inline void FLEXCAN_ProccessPSeg(uint32_t *tmpPropseg, uint32_t *tmpPseg1)
{
    while (*tmpPropseg <= 0U) {
        *tmpPropseg = *tmpPropseg + 1U;
        *tmpPseg1   = *tmpPseg1 - 1U;
    }

    while (*tmpPropseg >= FLEXCAN_PROPSEG_MAX) {
        *tmpPropseg = *tmpPropseg - 1U;
        *tmpPseg1   = *tmpPseg1 + 1U;
    }
}

/*
 * Function Name : FLEXCAN_BitrateToTimeSeg
 * Description   : Converts a bitrate (kbit/s) in time segment values for
 *                 standard CAN frame.
 *
 */
static uint32_t FLEXCAN_BitrateToTimeSeg(uint32_t bitrate, uint32_t clkFreq, flexcan_time_segment_t *timeSeg)
{
    uint32_t dBitrate, tmpBitrate, dBitrateMin, tmpPresdiv, tmpSample, dSampleMin, dSample, samplePoint, numTq, pseg1,
        pseg2, propseg, presdiv, tSeg1, tSeg2, tmpPseg1, tmpPseg2, tmpPropseg;

    presdiv = 0U;
    propseg = 0U;
    pseg1   = 0U;
    pseg2   = 0U;

    dSampleMin  = 100U;
    dBitrateMin = 1000000U;
    samplePoint = 88U;

    for (tmpPresdiv = 0U; tmpPresdiv < FLEXCAN_PRESDIV_MAX; tmpPresdiv++) {

        /* Compute the number of time quanta in 1 bit time */
        numTq = clkFreq / ((tmpPresdiv + 1U) * bitrate);
        /* Compute the real bitrate resulted */
        tmpBitrate = clkFreq / ((tmpPresdiv + 1U) * numTq);

        /* The number of time quanta in 1 bit time must be lower than the one supported */
        if ((numTq >= FLEXCAN_NUM_TQ_MIN) && (numTq < FLEXCAN_NUM_TQ_MAX)) {
            /* Compute time segments based on the value of the sampling point */
            tSeg1 = (numTq * samplePoint / 100U) - 1U;
            tSeg2 = numTq - 1U - tSeg1;

            /* Adjust time segment 1 and time segment 2 */
            FLEXCAN_ProccessTSeg(&tSeg1, &tSeg2);

            tmpPseg2 = tSeg2 - 1U;

            /* Start from pseg1 = pseg2 and adjust until propseg is valid */
            tmpPseg1   = tmpPseg2;
            tmpPropseg = tSeg1 - tmpPseg1 - 2U;

            FLEXCAN_ProccessPSeg(&tmpPropseg, &tmpPseg1);

            if (((tSeg1 >= FLEXCAN_TSEG1_MAX) || (tSeg2 >= FLEXCAN_TSEG2_MAX) || (tSeg2 < FLEXCAN_TSEG2_MIN) ||
                 (tSeg1 < FLEXCAN_TSEG1_MIN)) ||
                ((tmpPropseg >= FLEXCAN_PROPSEG_MAX) || (tmpPseg1 >= FLEXCAN_PSEG1_MAX) ||
                 (tmpPseg2 < FLEXCAN_PSEG2_MIN) || (tmpPseg2 >= FLEXCAN_PSEG2_MAX))) {
                continue;
            }

            tmpSample = ((tSeg1 + 1U) * 100U) / numTq;
            dSample   = FLEXCAN_CheckSample(tmpSample, samplePoint);
            dBitrate  = FLEXCAN_CheckBitrate(tmpBitrate, bitrate);

            if ((dBitrate < dBitrateMin) || ((dBitrate == dBitrateMin) && (dSample < dSampleMin))) {
                dSampleMin  = dSample;
                dBitrateMin = dBitrate;
                pseg1       = tmpPseg1;
                pseg2       = tmpPseg2;
                presdiv     = tmpPresdiv;
                propseg     = tmpPropseg;

                if ((dBitrate == 0U) && (dSample <= 1U)) {
                    break;
                }
            }
        }
    }

    timeSeg->phaseSeg1  = pseg1;
    timeSeg->phaseSeg2  = pseg2;
    timeSeg->preDivider = presdiv;
    timeSeg->propSeg    = propseg;
    timeSeg->rJumpwidth = FLEXCAN_CheckJumpWidth(pseg1);

    return tmpBitrate;
}

/*
 * Function Name : FLEXCAN_DRV_GetDefaultConfig
 * Description   : Gets the default configuration structure
 *
 */
uint32_t FLEXCAN_DRV_GetDefaultConfig(flexcan_user_config_t *config)
{
    /* Checks input parameter. */
    OS_ASSERT(config != NULL);

    uint32_t clkFreq, bitrate;
    flexcan_time_segment_t timeSeg;

    /* Get the PE clock frequency */
    (void)CLOCK_SYS_GetFreq(FEATURE_FLEXCAN_PE_OSC_CLK_NAME, &clkFreq);
    /* Means Clock Gate is off */
    OS_ASSERT(clkFreq > 0U);
    /* Time segments computed for PE bitrate = 500 Kbit/s, sample point = 87.5 */
    bitrate = FLEXCAN_BitrateToTimeSeg(500000U, clkFreq, &timeSeg);

    /* Maximum number of message buffers */
    config->max_num_mb = 16;
    /* Rx FIFO is disabled */
    config->is_rx_fifo_needed          = false;
    config->is_enhanced_rx_fifo_needed = false;
    /* Number of Rx FIFO ID filters */
    config->num_id_filters = FLEXCAN_RX_FIFO_ID_FILTERS_8;
    /* Normal operation mode */
    config->flexcanMode = FLEXCAN_NORMAL_MODE;
    /* Protocol engine clock is System Oscillator div 2 */
    config->pe_clock = FLEXCAN_CLK_SOURCE_OSC;
    /* Time segments for the arbitration phase */
    config->bitrate = timeSeg;
    /* Rx FIFO transfer type */
    config->transfer_type = FLEXCAN_RXFIFO_USING_INTERRUPTS;
    /* Rx FIFO PDMA channel */
    config->rxFifoDMAChannel = 0U;
    /* Payload size */
    config->payload.blockR0 = FLEXCAN_PAYLOAD_SIZE_8;
    config->payload.blockR1 = FLEXCAN_PAYLOAD_SIZE_8;
    config->payload.blockR2 = FLEXCAN_PAYLOAD_SIZE_8;
    config->payload.blockR3 = FLEXCAN_PAYLOAD_SIZE_8;
    /* Flexible data rate is disabled */
    config->fd_enable = false;
    /* Time segments for the data phase of FD frames */
    config->bitrate_cbt = timeSeg;

    config->time_stamp.timeStampSource      = FLEXCAN_CAN_CLK_TIMESTAMP_SRC;
    config->time_stamp.msgBuffTimeStampType = FLEXCAN_MSGBUFFTIMESTAMP_TIMER;
    config->time_stamp.hrConfigType         = FLEXCAN_TIMESTAMPCAPTURE_DISABLE;

    return bitrate;
}

/*
 * Function Name : FLEXCAN_DRV_TcsrStopZoneMode
 * Description   : Enter/exit stop mode according the TCSR
 *
 */
void FLEXCAN_DRV_TcsrStopZoneMode(uint8_t instance, bool en)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    uint32_t doze_enable = 0, stop_enable = 0, enable = 0;
    uint32_t val;

    if (3 > instance) { // can0/1/2
        // doze_enable = 1 << (instance + 12);
        stop_enable = 1 << (instance + TCSR_CANCONFIG_IPG_STOP_SHIFT);
    } else { // can3 in software means can4 in hardware
        // doze_enable = 1 << (instance + 12 + 1);
        stop_enable = 1 << (instance + TCSR_CANCONFIG_IPG_STOP_SHIFT + 1);
    }

    enable = doze_enable | stop_enable;

    tcsr_type_t *tcsr = (tcsr_type_t *)TCSR_BASEADDR;
    val               = tcsr->CANCONFIG;

    if (en) {
        val = val | enable;
    } else {
        val = val & (~enable);
    }

    tcsr->CANCONFIG = val;
}

/*
 * Function Name : FLEXCAN_DRV_CheckTcsrStopACK
 * Description   : Check stop ack according the TCSR
 *
 */
bool FLEXCAN_DRV_CheckTcsrStopACK(uint8_t instance)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);

    uint32_t stop_ack = 0, val = 0;

    if (3 > instance) // can0/1/2
        stop_ack = 1 << (31 - instance);
    else if (3 == instance) // can3 in software means can4 in hardware
        stop_ack = 1 << (31 - (instance + 1));

    val = *(volatile uint32_t *)(0x4404001c);
    if (val & stop_ack)
        return true;
    else
        return false;
}

/*
 * Function Name : FLEXCAN_DRV_GetMsgBuffTimestamp
 * Description   : Gets the time stamp of MB
 *
 */
uint32_t FLEXCAN_DRV_GetMsgBuffTimestamp(uint8_t instance, uint32_t mb_idx)
{
    can_type_t *base = g_flexcanBase[instance];
    return FLEXCAN_GetMsgBuffTimestamp(base, mb_idx);
}

/*
 * Function Name : FLEXCAN_DRV_RxMessageBuffer
 * Description   : Gets the frame data of the MB
 *
 */
status_t FLEXCAN_DRV_RxMessageBuffer(uint8_t instance, uint32_t mb_idx, flexcan_msgbuff_t *data)
{
    can_type_t *base       = g_flexcanBase[instance];
    flexcan_state_t *state = g_flexcanStatePtr[instance];

    /* Check if the MB index is in range */
    if (mb_idx >= FLEXCAN_GetMaxMbNum(base)) {
        return STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    /* Check if RX FIFO is enabled */
    if (true == FLEXCAN_IsRxFifoEnabled(base)) {
        uint32_t val1, val2;
        /* Get the number of RX FIFO Filters*/
        val1 = (((base->CTRL2) & CAN_CTRL2_RFFN_MASK) >> CAN_CTRL2_RFFN_SHIFT);
        /* Get the number if MBs occupied by RX FIFO and ID filter table*/
        /* the Rx FIFO occupies the memory space originally reserved for MB0-5*/
        /* Every number of RFFN means 8 number of RX FIFO filters*/
        /* and every 4 number of RX FIFO filters occupied one MB*/
        val2 = RxFifoOccupiedLastMsgBuff(val1);

        if (mb_idx <= val2) {
            return STATUS_CAN_BUFF_OUT_OF_RANGE;
        }
    }

    /* Start receiving mailbox */
    if (state->mbs[mb_idx].state != FLEXCAN_MB_IDLE) {
        return STATUS_BUSY;
    }

    FLEXCAN_LockRxMsgBuff(base, mb_idx);
    FLEXCAN_GetMsgBuff(base, mb_idx, data);
    FLEXCAN_UnlockRxMsgBuff(base);

    if (((data->cs & FLEXCAN_CS_CODE_MASK) >> FLEXCAN_CS_CODE_SHIFT) == (uint32_t)FLEXCAN_RX_FULL) {
        return STATUS_SUCCESS;
    } else {
        return STATUS_ERROR;
    }
}

/* temporarily, to test soft reset function */
bool FLEXCAN_Tmp_SoftRst_test(uint8_t instance)
{
    OS_ASSERT(instance < CAN_INSTANCE_COUNT);
    can_type_t *base = g_flexcanBase[instance];

    uint32_t freeRunningTimer_before = base->TIMER;
    if (0 == freeRunningTimer_before) {
        OS_DelayMs(2);
        freeRunningTimer_before = base->TIMER;
    }

    /* Reset the FLEXCAN */
    base->MCR = (base->MCR & ~CAN_MCR_SOFTRST_MASK) | CAN_MCR_SOFTRST(1U);
    /* Wait for reset cycle to complete */
    while (((base->MCR & CAN_MCR_SOFTRST_MASK) >> CAN_MCR_SOFTRST_SHIFT) != 0U) {
    }

    uint32_t freeRunningTimer_after = base->TIMER;
    if (freeRunningTimer_before > 0 && 0 == freeRunningTimer_after)
        return true;
    else
        return false;
}
