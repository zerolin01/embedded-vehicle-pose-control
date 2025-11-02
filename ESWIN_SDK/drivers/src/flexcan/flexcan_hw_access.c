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
 * @file flexcan_hw_access.c
 * @brief FLEXCAN driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "flexcan_hw_access.h"

#define FLEXCAN_ALL_INT (0x3B0006U) /*< Masks for wakeup, error, bus off */
#define BUS_OFF_INT     (0xB0004U)  /*< Masks for busOff, Tx/Rx Warning */
#define ERROR_INT       (0x300002U) /*< Masks for ErrorOvr, ErrorFast, Error */

#define FLEXCAN_ENHANCED_RX_FIFO_FILTER_TABLE_BASE (0x0U)

#define FLEXCAN_RX_FIFO_ACCEPT_REMOTE_FRAME (1UL)
#define FLEXCAN_RX_FIFO_ACCEPT_EXT_FRAME    (1UL)
/* CAN FD extended data length DLC encoding */
#define CAN_DLC_VALUE_12_BYTES (9U)
#define CAN_DLC_VALUE_16_BYTES (10U)
#define CAN_DLC_VALUE_20_BYTES (11U)
#define CAN_DLC_VALUE_24_BYTES (12U)
#define CAN_DLC_VALUE_32_BYTES (13U)
#define CAN_DLC_VALUE_48_BYTES (14U)
#define CAN_DLC_VALUE_64_BYTES (15U)

#define RXFIFO_FILTER_TABLE_OFFSET (0x18U)

static uint8_t FLEXCAN_ComputeDLCValue(uint8_t payloadSize);
static void FLEXCAN_ClearRAM(can_type_t *base);
static uint8_t FLEXCAN_GetPayloadSize(const can_type_t *base, uint8_t blockIdx);

/* Determines the RxFIFO Filter element number */
#define RxFifoFilterElementNum(x) (((x) + 1U) * 8U)

/*
 * Function Name : FLEXCAN_GetRxFifoMask
 * Description   : Calculate the Global Mask as format ID type in FIFO Mode.
 * Based on the ID format type and Mask Type will calculate the and set accordingly
 * the Rx FIFO Individual/Global Mask register.
 *
 */
uint32_t FLEXCAN_GetRxFifoMask(flexcan_msgbuff_id_type_t id_type, flexcan_rx_fifo_id_element_format_t formatType,
                               uint32_t mask)
{
    uint32_t val = 0;

    switch (formatType) {
    case (FLEXCAN_RX_FIFO_ID_FORMAT_A):
        /* Set RTR bit encoded as bit 31 and IDE bit encoded as bit 30 in mask */
        val = mask & ((1UL << FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_RTR_SHIFT) |
                      (1UL << FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_IDE_SHIFT));
        if (id_type == FLEXCAN_MSG_ID_STD) {
            /* Set standard global mask for RX FIFO and IDE will be 1 and check the FIFO filter ide */
            val |= ((mask << FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_SHIFT) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_MASK);

        } else if (id_type == FLEXCAN_MSG_ID_EXT) {
            /* Set extended global mask for RX FIFO and IDE will be 0 and don't check the FIFO filter ide */
            val |= ((mask << FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_SHIFT) & FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_MASK);
        } else {
            /* Should not get here */
        }
        break;
    case (FLEXCAN_RX_FIFO_ID_FORMAT_B):
        /* Set RTR bit encoded as bit 31 and IDE bit encoded as bit 30 in mask */
        val = mask & ((1UL << FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_RTR_SHIFT) |
                      (1UL << FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_IDE_SHIFT));
        if (id_type == FLEXCAN_MSG_ID_STD) {
            /* Set standard global mask for RX FIFO  */
            val |=
                ((mask & FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK) << FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT1);

        } else if (id_type == FLEXCAN_MSG_ID_EXT) {
            /* Set extended global mask for RX FIFO  */
            val |=
                ((mask & FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK1) << FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1);
        } else {
            /* Should not get here */
        }
        break;
    case (FLEXCAN_RX_FIFO_ID_FORMAT_C):
        if ((id_type == FLEXCAN_MSG_ID_EXT) || (id_type == FLEXCAN_MSG_ID_STD)) {
            val |= ((mask & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_MASK) << FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1);
        } else {
            /* Should not get here */
        }
        break;
    case (FLEXCAN_RX_FIFO_ID_FORMAT_D):
        /* Format not supported. */
        break;
    default:
        /* Should not get here */
        break;
    } /* End Switch */

    return val;
}

/*
 * Function Name: FLEXCAN_ComputeDLCValue
 * Description  : Computes the DLC field value, given a payload size (in bytes).
 *
 */
static uint8_t FLEXCAN_ComputeDLCValue(uint8_t payloadSize)
{
    uint32_t ret                          = 0xFFU; // 0,  1,  2,  3,  4,  5,  6,  7,  8,
    static const uint8_t payload_code[65] = {
        0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U,
        /* 9 to 12 payload have DLC Code 12 Bytes */
        CAN_DLC_VALUE_12_BYTES, CAN_DLC_VALUE_12_BYTES, CAN_DLC_VALUE_12_BYTES, CAN_DLC_VALUE_12_BYTES,
        /* 13 to 16 payload have DLC Code 16 Bytes */
        CAN_DLC_VALUE_16_BYTES, CAN_DLC_VALUE_16_BYTES, CAN_DLC_VALUE_16_BYTES, CAN_DLC_VALUE_16_BYTES,
        /* 17 to 20 payload have DLC Code 20 Bytes */
        CAN_DLC_VALUE_20_BYTES, CAN_DLC_VALUE_20_BYTES, CAN_DLC_VALUE_20_BYTES, CAN_DLC_VALUE_20_BYTES,
        /* 21 to 24 payload have DLC Code 24 Bytes */
        CAN_DLC_VALUE_24_BYTES, CAN_DLC_VALUE_24_BYTES, CAN_DLC_VALUE_24_BYTES, CAN_DLC_VALUE_24_BYTES,
        /* 25 to 32 payload have DLC Code 32 Bytes */
        CAN_DLC_VALUE_32_BYTES, CAN_DLC_VALUE_32_BYTES, CAN_DLC_VALUE_32_BYTES, CAN_DLC_VALUE_32_BYTES,
        CAN_DLC_VALUE_32_BYTES, CAN_DLC_VALUE_32_BYTES, CAN_DLC_VALUE_32_BYTES, CAN_DLC_VALUE_32_BYTES,
        /* 33 to 48 payload have DLC Code 48 Bytes */
        CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES,
        CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES,
        CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES,
        CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES, CAN_DLC_VALUE_48_BYTES,
        /* 49 to 64 payload have DLC Code 64 Bytes */
        CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES,
        CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES,
        CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES,
        CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES, CAN_DLC_VALUE_64_BYTES};

    if (payloadSize <= 64U) {
        ret = payload_code[payloadSize];
    } else {
        /* The argument is not a valid payload size will return 0xFF*/
    }

    return (uint8_t)ret;
}

/*
 * Function Name : FLEXCAN_ComputePayloadSize
 * Description   : Given a DLC field value, computes the maximum payload size (in bytes).
 *
 */
uint8_t FLEXCAN_ComputePayloadSize(uint8_t dlcValue)
{
    uint8_t ret = 0U;

    if (dlcValue <= 8U) {
        ret = dlcValue;
    } else {
        switch (dlcValue) {
        case CAN_DLC_VALUE_12_BYTES:
            ret = 12U;
            break;
        case CAN_DLC_VALUE_16_BYTES:
            ret = 16U;
            break;
        case CAN_DLC_VALUE_20_BYTES:
            ret = 20U;
            break;
        case CAN_DLC_VALUE_24_BYTES:
            ret = 24U;
            break;
        case CAN_DLC_VALUE_32_BYTES:
            ret = 32U;
            break;
        case CAN_DLC_VALUE_48_BYTES:
            ret = 48U;
            break;
        case CAN_DLC_VALUE_64_BYTES:
            ret = 64U;
            break;
        default:
            /* The argument is not a valid DLC size */
            break;
        }
    }

    return ret;
}

/*
 * Function Name : FLEXCAN_GetMaxMbNum
 * Description   : Computes the maximum RAM size occupied by MBs.
 *
 */
uint32_t FLEXCAN_GetMaxMbNum(const can_type_t *base)
{
    uint32_t i, ret = 0;
    static can_type_t *const flexcanBase[] = CAN_BASE_PTRS;
    static const uint32_t maxMbNum[]       = FEATURE_FLEXCAN_MAX_MB_NUM_ARRAY;

    for (i = 0; i < CAN_INSTANCE_COUNT; i++) {
        if (base == flexcanBase[i]) {
            ret = maxMbNum[i];
        }
    }

    return ret;
}

/*
 * Function Name : FLEXCAN_ClearRAM
 * Description   : Clears FlexCAN memory positions that require initialization.
 *
 */
static void FLEXCAN_ClearRAM(can_type_t *base)
{
    uint32_t dataByte;
    uint32_t RAM_size   = FLEXCAN_GetMaxMbNum(base) * 4U;
    uint32_t RXIMR_size = FLEXCAN_GetMaxMbNum(base);

    /* Point to the MB start address */
    volatile uint32_t *RAM = base->RAMn;

    /* Clear MB region */
    for (dataByte = 0U; dataByte < RAM_size; dataByte++) {
        RAM[dataByte] = 0x0U;
    }

    RAM = base->RXIMR;
    /* Clear RXIMR region */
    for (dataByte = 0U; dataByte < RXIMR_size; dataByte++) {
        RAM[dataByte] = 0x0U;
    }

    if (FLEXCAN_IsExpandableMemoryAvailable(base)) {
        /* Clear Enhance MB ram region */
        for (dataByte = 0; dataByte < CAN_RAMn_COUNT; dataByte++) {
            base->EMBn[dataByte] = 0;
        }
    }

    /* Set WRMFRZ bit in CTRL2 Register to grant write access to memory */
    base->CTRL2 = (base->CTRL2 & ~CAN_CTRL2_WRMFRZ_MASK) | CAN_CTRL2_WRMFRZ(1U);

    /*  Clear RXFIR RAM  */
    for (dataByte = 0; dataByte < 6; dataByte++) {
        base->RXFIR_RAM[dataByte] = 0;
    }

    /* Clear RXMGMASK, RXFGMASK, RX14MASK, RX15MASK RAM mapping */
    for (dataByte = 0; dataByte < 4; dataByte++) {
        base->RXMASK_RAM[dataByte] = 0;
    }

    if (FLEXCAN_IsEnhancedRxFifoAvailable(base)) {
        /* Clear SMB FD region */
        for (dataByte = 0; dataByte < 54; dataByte++) {
            base->FDRMB[dataByte] = 0;
        }
    }

    /* Clear SMB nonFD region */
    for (dataByte = 0; dataByte < 12; dataByte++) {
        base->SMB_RAM[dataByte] = 0;
    }

    if (FLEXCAN_IsEnhancedRxFifoAvailable(base)) {
        /* Clear Enhance RX FIFO */
        for (dataByte = 0; dataByte < (CAN_0_ERFFELn_COUNT * 20); dataByte++) {
            base->ERX_FIFOn[dataByte] = 0;
        }

        /* Clear Enhance RX FIFO Filter*/
        for (dataByte = 0; dataByte < CAN_0_ERFFELn_COUNT; dataByte++) {
            base->ERFFELn[dataByte] = 0;
        }

        /* Clear RX SMB time stamps*/
        for (dataByte = 0; dataByte < 2; dataByte++) {
            base->RSMB_TIME_STAMP[dataByte] = 0;
        }

        /* Clear HR time stamps*/
        for (dataByte = 0; dataByte < CAN_0_HR_TIME_STAMPn_COUNT; dataByte++) {
            base->HR_TIME_STAMPn[dataByte] = 0;
        }
    }

    /* Clear WRMFRZ bit in CTRL2 Register to restrict write access to memory */
    base->CTRL2 = (base->CTRL2 & ~CAN_CTRL2_WRMFRZ_MASK) | CAN_CTRL2_WRMFRZ(0U);
}

/*
 *
 * Function Name : FLEXCAN_IsExpandableMemoryAvailable
 * Description   : Checks if this instance has an extra MB memory.
 * This function is private.
 *
 */
bool FLEXCAN_IsExpandableMemoryAvailable(const can_type_t *base)
{
    uint32_t i;
    static can_type_t *const flexcanBase[] = FLEXCAN_BASE_PTRS_HAS_EXPANDABLE_MEMORY;
    bool returnValue                       = false;
    for (i = 0U; i < FEATURE_FLEXCAN_EXPANDABLE_MEMORY_NUM; i++) {
        if (base == flexcanBase[i]) {
            returnValue = true;
            break;
        }
    }

    return returnValue;
}

/*
 * Function Name : FLEXCAN_GetMsgBuffRegion
 * Description   : Returns the start address of a MB area, based on its index.
 *
 */
volatile uint32_t *FLEXCAN_GetMsgBuffRegion(can_type_t *base, uint32_t msgBuffIdx)
{
    uint8_t arbitration_field_size = 8U;
    uint8_t mb_size                = 0U;
    uint32_t blockRegionSize       = 512U;
    uint32_t blockRegionOffset     = 0U;
    uint32_t msgBuffIdxTmp         = msgBuffIdx;
    uint8_t maxMbNum = 0U, i = 0U;
    uint32_t mb_index    = 0U;
    uint8_t payload_size = 8U;

    volatile uint32_t *RAM          = (uint32_t *)((uint32_t)base + (uint32_t)FEATURE_FLEXCAN_RAM_OFFSET);
    volatile uint32_t *RAM_ENHANCED = (uint32_t *)((uint32_t)base + (uint32_t)FEATURE_FLEXCAN_EXP_RAM_OFFSET);
    volatile uint32_t *pAddressRet  = NULL;

    uint8_t instance   = FLEXCAN_Base2Instance(base);
    bool instHasFd     = FLEXCAN_InstanceHasFD(instance);
    bool enhancedMbSup = FLEXCAN_IsExpandableMemoryAvailable(base);

    for (i = 0; i < (uint8_t)FLEXCAN_MBDSR_COUNT_OF_NORMAL_RAM_BLOCK; i++) {
        if (instHasFd) {
            payload_size = FLEXCAN_GetPayloadSize(base, i);
        } else {
            payload_size = 8U;
        }

        mb_size  = (uint8_t)(payload_size + arbitration_field_size);
        maxMbNum = (uint8_t)(blockRegionSize / mb_size);
        if (maxMbNum > msgBuffIdxTmp) {
            break;
        }

        blockRegionOffset += 128U;
        msgBuffIdxTmp -= maxMbNum;
    }

    if (((uint8_t)FLEXCAN_MBDSR_COUNT_OF_NORMAL_RAM_BLOCK == i) && (true == enhancedMbSup)) {
        /* Multiply the MB index by the MB size (in words).
         * For enhanced ram block region:
         * maxMbNum per one block region is 7. mb_size per one MB is 72 (64 + 8)
         * 128  = 512/4, because RANn is unit32_t.
         */
        mb_index    = ((msgBuffIdxTmp / 7U) * 128U) + ((msgBuffIdxTmp % 7U) * (72U >> 2U));
        pAddressRet = &(RAM_ENHANCED[mb_index]);
    } else {
        /* Multiply the MB index by the MB size (in words) */
        mb_index    = blockRegionOffset + ((msgBuffIdxTmp % (uint32_t)maxMbNum) * ((uint32_t)mb_size >> 2U));
        pAddressRet = &(RAM[mb_index]);
    }

    return pAddressRet;
}

/*
 * Function Name : FLEXCAN_Enable
 * Description   : Enable FlexCAN module.
 * This function will enable FlexCAN module.
 *
 */
void FLEXCAN_Enable(can_type_t *base)
{
    uint32_t tmpReg = 0;
    tmpReg |= CAN_MCR_MAXMB(0x7F);
    tmpReg |= CAN_MCR_IDAM(0) | CAN_MCR_FDEN(0);
    tmpReg |= CAN_MCR_AEN(1) | CAN_MCR_LPRIOEN(0) | CAN_MCR_PNET_EN(0) | CAN_MCR_DMA(0);
    tmpReg |= CAN_MCR_IRMQ(1) | CAN_MCR_SRXDIS(1);
    tmpReg |= CAN_MCR_LPMACK(0) | CAN_MCR_WRNEN(1);
    tmpReg |= CAN_MCR_HALT(1) | CAN_MCR_RFEN(1) | CAN_MCR_FRZ(1);
    base->MCR = tmpReg; // 0x7023107F

    OS_DelayMs(1);

    /* Check for low power mode */
    if (((base->MCR & CAN_MCR_LPMACK_MASK) >> CAN_MCR_LPMACK_SHIFT) == 1U) {
        /* Enable clock , when Module enable , PE clock is on*/
        base->MCR = (base->MCR & ~CAN_MCR_MDIS_MASK) | CAN_MCR_MDIS(0U);
        base->MCR = (base->MCR & ~CAN_MCR_FRZ_MASK) | CAN_MCR_FRZ(0U);
        base->MCR = (base->MCR & ~CAN_MCR_HALT_MASK) | CAN_MCR_HALT(0U);
        /* Disable ECC Error */
        base->MECR = 0;
        /* Wait until enabled */
        while (((base->MCR & CAN_MCR_NOTRDY_MASK) >> CAN_MCR_NOTRDY_SHIFT) != 0U) {
        }
    }
}

/*
 * Function Name : FLEXCAN_Disable
 * Description   : Disable FlexCAN module.
 * This function will disable FlexCAN module.
 *
 */
void FLEXCAN_Disable(can_type_t *base)
{
    /* To access the memory mapped registers */
    /* Entre disable mode (hard reset). */
    if (((base->MCR & CAN_MCR_MDIS_MASK) >> CAN_MCR_MDIS_SHIFT) == 0U) {
        /* Clock disable (module) */
        base->MCR = (base->MCR & ~CAN_MCR_MDIS_MASK) | CAN_MCR_MDIS(1U);

        /* Wait until disable mode acknowledged */
        while (((base->MCR & CAN_MCR_LPMACK_MASK) >> CAN_MCR_LPMACK_SHIFT) == 0U) {
        }
    }
}

/*
 * Function Name : dataByte FLEXCAN_Init
 * Description   : Initialize FlexCAN module.
 * This function will reset FlexCAN module, set maximum number of message
 * buffers, initialize all message buffers as inactive, enable RX FIFO
 * if needed, mask all mask bits, and disable all MB interrupts.
 *
 */
void FLEXCAN_Init(can_type_t *base)
{
    /* Reset the FLEXCAN */
    base->MCR = (base->MCR & ~CAN_MCR_SOFTRST_MASK) | CAN_MCR_SOFTRST(1U);

    /* Wait for reset cycle to complete */
    while (((base->MCR & CAN_MCR_SOFTRST_MASK) >> CAN_MCR_SOFTRST_SHIFT) != 0U) {
    }

    /* Enable abort */
    /* Avoid Abort Transmission, use Inactive MB */
    base->MCR = (base->MCR & ~CAN_MCR_AEN_MASK) | CAN_MCR_AEN(1U);

    /* Clear FlexCAN memory */
    FLEXCAN_ClearRAM(base);

    /* Rx global mask*/
    (base->RXMGMASK) = (uint32_t)(CAN_RXMGMASK_MG_MASK);

    /* Rx reg 14 mask*/
    (base->RX14MASK) = (uint32_t)(CAN_RX14MASK_RX14M_MASK);

    /* Rx reg 15 mask*/
    (base->RX15MASK) = (uint32_t)(CAN_RX15MASK_RX15M_MASK);

    /* Disable all MB interrupts */
    (base->IMASK1) = 0x0;
    /* Clear all MB interrupt flags */
    (base->IFLAG1) = CAN_IMASK1_BUF31TO0M_MASK;

    if (FLEXCAN_GetMaxMbNum(base) > 32U) {
        (base->IMASK2) = 0x0;
        (base->IFLAG2) = CAN_IMASK2_BUF63TO32M_MASK;
    }

    if (FLEXCAN_GetMaxMbNum(base) > 64U) {
        (base->IMASK3) = 0x0;
        (base->IFLAG3) = CAN_IMASK3_BUF95TO64M_MASK;
    }

    if (FLEXCAN_GetMaxMbNum(base) > 96U) {
        (base->IMASK4) = 0x0;
        (base->IFLAG4) = CAN_IMASK4_BUF127TO96M_MASK;
    }

    /* Clear all error interrupt flags */
    (base->ESR1) = FLEXCAN_ALL_INT;
}

/*
 * Function Name : FLEXCAN_SetTxMsgBuff
 * Description   : Configure a message buffer for transmission.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will copy user's buffer into the
 * message buffer data area and configure the message buffer as required for
 * transmission.
 *
 */
status_t FLEXCAN_SetTxMsgBuff(can_type_t *base, uint32_t msgBuffIdx, const flexcan_msgbuff_code_status_t *cs,
                              uint32_t msgId, const uint8_t *msgData, const bool isRemote)
{
    OS_ASSERT(cs != NULL);
    OS_ASSERT(((uint32_t)msgData % 4) == 0);

    uint32_t val1, val2 = 1;
    uint32_t flexcan_mb_config = 0;
    uint32_t dataByte;
    uint8_t dlc_value;
    status_t stat    = STATUS_SUCCESS;
    uint8_t instance = FLEXCAN_Base2Instance(base);
    bool instHasFd   = FLEXCAN_InstanceHasFD(instance);

    volatile uint32_t *flexcan_mb = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);

    volatile uint32_t *flexcan_mb_id      = &flexcan_mb[1];
    volatile uint8_t *flexcan_mb_data     = (volatile uint8_t *)(&flexcan_mb[2]);
    volatile uint32_t *flexcan_mb_data_32 = &flexcan_mb[2];
    const uint32_t *msgData_32            = (const uint32_t *)msgData;

    if (msgBuffIdx > (((base->MCR) & CAN_MCR_MAXMB_MASK) >> CAN_MCR_MAXMB_SHIFT)) {
        stat = STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    /* Check if RX FIFO is enabled*/
    if (((base->MCR & CAN_MCR_RFEN_MASK) >> CAN_MCR_RFEN_SHIFT) != 0U) {
        /* Get the number of RX FIFO Filters*/
        val1 = (((base->CTRL2) & CAN_CTRL2_RFFN_MASK) >> CAN_CTRL2_RFFN_SHIFT);
        /* Get the number if MBs occupied by RX FIFO and ID filter table*/
        /* the Rx FIFO occupies the memory space originally reserved for MB0-5*/
        /* Every number of RFFN means 8 number of RX FIFO filters*/
        /* and every 4 number of RX FIFO filters occupied one MB*/
        val2 = RxFifoOccupiedLastMsgBuff(val1);

        if (msgBuffIdx <= val2) {
            stat = STATUS_CAN_BUFF_OUT_OF_RANGE;
        }
    }

    if (stat == STATUS_SUCCESS) {
        if (instHasFd) {
            /* Make sure the BRS bit will not be ignored */
            if (FLEXCAN_IsFDEnabled(base) && cs->enable_brs) {
                base->FDCTRL = (base->FDCTRL & ~CAN_FDCTRL_FDRATE_MASK) | CAN_FDCTRL_FDRATE(1U);
            }
            /* Check if the Payload Size is smaller than the payload configured */
            OS_ASSERT((uint8_t)cs->dataLen <= FLEXCAN_GetMbPayloadSize(base, msgBuffIdx));
        } else {
            OS_ASSERT((uint8_t)cs->dataLen <= 8U);
        }

        /* Compute the value of the DLC field */
        dlc_value = FLEXCAN_ComputeDLCValue((uint8_t)cs->dataLen);

        /* Copy user's buffer into the message buffer data area */
        if (NULL != msgData) {

            for (dataByte = 0; dataByte < (cs->dataLen & ~3U); dataByte += 4U) {
                FlexcanSwapBytesInWord(msgData_32[dataByte >> 2U], flexcan_mb_data_32[dataByte >> 2U]);
            }

            for (; dataByte < cs->dataLen; dataByte++) {
                flexcan_mb_data[FlexcanSwapBytesInWordIndex(dataByte)] = msgData[dataByte];
            }

            uint8_t payload_size = FLEXCAN_ComputePayloadSize(dlc_value);

            /* Add padding, if needed */
            for (dataByte = cs->dataLen; dataByte < payload_size; dataByte++) {
                flexcan_mb_data[FlexcanSwapBytesInWordIndex(dataByte)] = cs->fd_padding;
            }
        }

        /* Clean up the arbitration field area */
        *flexcan_mb    = 0;
        *flexcan_mb_id = 0;

        /* Set the ID according the format structure */
        if (FLEXCAN_MSG_ID_EXT == cs->msgIdType) {
            /* ID [28-0] */
            *flexcan_mb_id &= ~(FLEXCAN_ID_STD_MASK | FLEXCAN_ID_EXT_MASK);
            *flexcan_mb_id |= (msgId & (FLEXCAN_ID_STD_MASK | FLEXCAN_ID_EXT_MASK));

            /* Set IDE */
            flexcan_mb_config |= FLEXCAN_CS_IDE_MASK;

            /* Set SRR bit */
            flexcan_mb_config |= FLEXCAN_CS_SRR_MASK;
        }

        if (FLEXCAN_MSG_ID_STD == cs->msgIdType) {
            /* ID[28-18] */
            *flexcan_mb_id &= ~FLEXCAN_ID_STD_MASK;
            *flexcan_mb_id |= (msgId << FLEXCAN_ID_STD_SHIFT) & FLEXCAN_ID_STD_MASK;

            /* make sure IDE and SRR are not set */
            flexcan_mb_config &= ~(FLEXCAN_CS_IDE_MASK | FLEXCAN_CS_SRR_MASK);
        }

        /* Set the length of data in bytes */
        flexcan_mb_config &= ~FLEXCAN_CS_DLC_MASK;
        flexcan_mb_config |= ((uint32_t)dlc_value << FLEXCAN_CS_DLC_SHIFT) & FLEXCAN_CS_DLC_MASK;

        /* Set MB CODE */
        if ((uint32_t)FLEXCAN_TX_NOT_USED != cs->code) {
            if ((uint32_t)FLEXCAN_TX_REMOTE == cs->code) {
                /* Set RTR bit */
                flexcan_mb_config |= FLEXCAN_CS_RTR_MASK;
            } else {
                if (true == isRemote) {
                    /* Set RTR bit */
                    flexcan_mb_config |= FLEXCAN_CS_RTR_MASK;
                }
            }

            /* Reset the code */
            flexcan_mb_config &= ~FLEXCAN_CS_CODE_MASK;

            /* Set the code */
            if (cs->fd_enable) {
                flexcan_mb_config |= ((cs->code << FLEXCAN_CS_CODE_SHIFT) & FLEXCAN_CS_CODE_MASK) | FLEXCAN_MB_EDL_MASK;
            } else {
                flexcan_mb_config |= (cs->code << FLEXCAN_CS_CODE_SHIFT) & FLEXCAN_CS_CODE_MASK;
            }

            if (cs->enable_brs) {
                flexcan_mb_config |= FLEXCAN_MB_BRS_MASK;
            }

            *flexcan_mb |= flexcan_mb_config;
        }
    }

    return stat;
}

/*
 * Function Name : FLEXCAN_AbortTxMsgBuff
 * Description   : Writes the abort code into the CODE field of the requested
 * Tx message buffer.
 *
 */
void FLEXCAN_AbortTxMsgBuff(can_type_t *base, uint32_t msgBuffIdx)
{
    uint32_t flexcan_mb_config = 0;
    uint32_t code              = FLEXCAN_TX_ABORT;

    volatile uint32_t *flexcan_mb = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);
    flexcan_mb_config             = *flexcan_mb;

    /* Reset the code */
    flexcan_mb_config &= (~FLEXCAN_CS_CODE_MASK);

    /* Write the abort code */
    flexcan_mb_config |= (code << FLEXCAN_CS_CODE_SHIFT) & FLEXCAN_CS_CODE_MASK;
    *flexcan_mb = flexcan_mb_config;
}

/*
 * Function Name : FLEXCAN_ResetRxMsgBuff
 * Description   : Writes the Inactive Rx code into the CODE field of the requested
 * Rx message buffer and restore the MB to active Rx. This will force even the unlock of the RxMB.
 *
 */
void FLEXCAN_ResetRxMsgBuff(can_type_t *base, uint32_t msgBuffIdx)
{
    uint32_t flexcan_mb_config = 0;
    uint32_t code              = FLEXCAN_RX_INACTIVE;

    volatile uint32_t *flexcan_mb = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);
    flexcan_mb_config             = *flexcan_mb;

    /* Reset the code and unlock the MB */
    flexcan_mb_config &= (~FLEXCAN_CS_CODE_MASK);
    flexcan_mb_config |= (code << FLEXCAN_CS_CODE_SHIFT) & FLEXCAN_CS_CODE_MASK;
    *flexcan_mb = flexcan_mb_config;

    /* Reconfigure The MB as left by RxMBconfig */
    code = FLEXCAN_RX_EMPTY;
    flexcan_mb_config &= (~FLEXCAN_CS_CODE_MASK);
    flexcan_mb_config |= (code << FLEXCAN_CS_CODE_SHIFT) & FLEXCAN_CS_CODE_MASK;
    *flexcan_mb = flexcan_mb_config;
}

/*
 * Function Name : FLEXCAN_SetRxMsgBuff
 * Description   : Configure a message buffer for receiving.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will configure the message buffer as
 * required for receiving.
 *
 */
status_t FLEXCAN_SetRxMsgBuff(can_type_t *base, uint32_t msgBuffIdx, const flexcan_msgbuff_code_status_t *cs,
                              uint32_t msgId)
{
    OS_ASSERT(cs != NULL);

    uint32_t val1, val2 = 1;

    volatile uint32_t *flexcan_mb    = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);
    volatile uint32_t *flexcan_mb_id = &flexcan_mb[1];
    status_t stat                    = STATUS_SUCCESS;

    if (msgBuffIdx > (((base->MCR) & CAN_MCR_MAXMB_MASK) >> CAN_MCR_MAXMB_SHIFT)) {
        stat = STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    /* Check if RX FIFO is enabled */
    if (((base->MCR & CAN_MCR_RFEN_MASK) >> CAN_MCR_RFEN_SHIFT) != 0U) {
        /* Get the number of RX FIFO Filters*/
        val1 = (((base->CTRL2) & CAN_CTRL2_RFFN_MASK) >> CAN_CTRL2_RFFN_SHIFT);
        /* Get the number if MBs occupied by RX FIFO and ID filter table*/
        /* the Rx FIFO occupies the memory space originally reserved for MB0-5*/
        /* Every number of RFFN means 8 number of RX FIFO filters*/
        /* and every 4 number of RX FIFO filters occupied one MB*/
        val2 = RxFifoOccupiedLastMsgBuff(val1);

        if (msgBuffIdx <= val2) {
            stat = STATUS_CAN_BUFF_OUT_OF_RANGE;
        }
    }

    if (stat == STATUS_SUCCESS) {
        /* Clean up the arbitration field area */
        *flexcan_mb    = 0;
        *flexcan_mb_id = 0;

        /* Set the ID according the format structure */
        if (cs->msgIdType == FLEXCAN_MSG_ID_EXT) {
            /* Set IDE */
            *flexcan_mb |= FLEXCAN_CS_IDE_MASK;

            /* Clear SRR bit */
            *flexcan_mb &= ~FLEXCAN_CS_SRR_MASK;

            /* ID [28-0] */
            *flexcan_mb_id &= ~(FLEXCAN_ID_STD_MASK | FLEXCAN_ID_EXT_MASK);
            *flexcan_mb_id |= (msgId & (FLEXCAN_ID_STD_MASK | FLEXCAN_ID_EXT_MASK));
        }
        if (cs->msgIdType == FLEXCAN_MSG_ID_STD) {
            /* Make sure IDE and SRR are not set */
            *flexcan_mb &= ~(FLEXCAN_CS_IDE_MASK | FLEXCAN_CS_SRR_MASK);

            /* ID[28-18] */
            *flexcan_mb_id &= ~FLEXCAN_ID_STD_MASK;
            *flexcan_mb_id |= (msgId << FLEXCAN_ID_STD_SHIFT) & FLEXCAN_ID_STD_MASK;
        }

        /* Set MB CODE */
        if (cs->code != (uint32_t)FLEXCAN_RX_NOT_USED) {
            *flexcan_mb &= ~FLEXCAN_CS_CODE_MASK;
            *flexcan_mb |= (cs->code << FLEXCAN_CS_CODE_SHIFT) & FLEXCAN_CS_CODE_MASK;
        }
    }

    return stat;
}

/*
 * Function Name : FLEXCAN_GetMsgBuffTimestamp
 * Description   : Get a message buffer timestamp value.
 *
 */
uint32_t FLEXCAN_GetMsgBuffTimestamp(can_type_t *base, uint32_t msgBuffIdx)
{
    uint32_t TimeStamp                  = 0U;
    volatile const uint32_t *Flexcan_Mb = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);

    if (FLEXCAN_IsHRTimeStampEnabled(base)) {
        /* Extract the Time Stamp */
        TimeStamp = (uint32_t)base->HR_TIME_STAMPn[msgBuffIdx];
    } else {
        TimeStamp = (uint32_t)((*Flexcan_Mb & FLEXCAN_CS_TIME_STAMP_MASK) >> FLEXCAN_CS_TIME_STAMP_SHIFT);
    }

    return TimeStamp;
}

/*
 * Function Name : FLEXCAN_GetMsgBuff
 * Description   : Get a message buffer field values.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will get the message buffer field
 * values and copy the MB data field into user's buffer.
 *
 */
void FLEXCAN_GetMsgBuff(can_type_t *base, uint32_t msgBuffIdx, flexcan_msgbuff_t *msgBuff)
{
    OS_ASSERT(msgBuff != NULL);

    uint8_t i;
    uint8_t instance                       = FLEXCAN_Base2Instance(base);
    bool instHasFd                         = FLEXCAN_InstanceHasFD(instance);
    volatile const uint32_t *flexcan_mb    = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);
    volatile const uint32_t *flexcan_mb_id = &flexcan_mb[1];
    // volatile const uint8_t *flexcan_mb_data     = (volatile const uint8_t *)(&flexcan_mb[2]);
    volatile const uint32_t *flexcan_mb_data_32 = &flexcan_mb[2];
    uint32_t *msgBuff_data_32                   = (uint32_t *)(msgBuff->data);
    uint32_t mbWord;

    uint8_t flexcan_mb_dlc_value = (uint8_t)(((*flexcan_mb) & FLEXCAN_CS_DLC_MASK) >> 16);
    uint8_t payload_size         = FLEXCAN_ComputePayloadSize(flexcan_mb_dlc_value);

    if (instHasFd) {
        if (payload_size > FLEXCAN_GetMbPayloadSize(base, msgBuffIdx)) {
            payload_size = FLEXCAN_GetMbPayloadSize(base, msgBuffIdx);
        }
    }

    msgBuff->dataLen = payload_size;

    /* Get a MB field values */
    msgBuff->cs = *flexcan_mb;

    if ((msgBuff->cs & FLEXCAN_CS_IDE_MASK) != 0U) {
        msgBuff->msgId = (*flexcan_mb_id);
    } else {
        msgBuff->msgId = (*flexcan_mb_id) >> FLEXCAN_ID_STD_SHIFT;
    }

    if (FLEXCAN_IsHRTimeStampEnabled(base)) {
        /* Extract the Time Stamp */
        msgBuff->time_stamp = (uint32_t)base->HR_TIME_STAMPn[msgBuffIdx];
    } else {
        msgBuff->time_stamp = (uint32_t)((msgBuff->cs & FLEXCAN_CS_TIME_STAMP_MASK) >> FLEXCAN_CS_TIME_STAMP_SHIFT);
    }

    for (i = 0U; i < (payload_size & ~3U); i += 4U) {
        mbWord = flexcan_mb_data_32[i >> 2U];
        FlexcanSwapBytesInWord(mbWord, msgBuff_data_32[i >> 2U]);
    }
}

/*
 * Function Name : FLEXCAN_LockRxMsgBuff
 * Description   : Lock the RX message buffer.
 * This function will lock the RX message buffer.
 *
 */
void FLEXCAN_LockRxMsgBuff(can_type_t *base, uint32_t msgBuffIdx)
{
    volatile const uint32_t *flexcan_mb = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);

    /* Lock the mailbox by reading it */
    (void)*flexcan_mb;
}

/*
 * Function Name : FLEXCAN_EnableRxFifo
 * Description   : Enable Rx FIFO feature.
 * This function will enable the Rx FIFO feature.
 *
 */
status_t FLEXCAN_EnableRxFifo(can_type_t *base, uint32_t numOfFilters)
{
    uint32_t i;
    uint16_t noOfMbx = (uint16_t)FLEXCAN_GetMaxMbNum(base);
    uint8_t instance = FLEXCAN_Base2Instance(base);
    bool instHasFd   = FLEXCAN_InstanceHasFD(instance);
    status_t stat    = STATUS_SUCCESS;

    if (instHasFd) {
        /* RxFIFO cannot be enabled if FD is enabled */
        if (FLEXCAN_IsFDEnabled(base)) {
            stat = STATUS_ERROR;
        }
    }

    if (stat == STATUS_SUCCESS) {
        /* Enable RX FIFO */
        base->MCR = (base->MCR & ~CAN_MCR_RFEN_MASK) | CAN_MCR_RFEN(1U);
        /* Set the number of the RX FIFO filters needed */
        base->CTRL2 =
            (base->CTRL2 & ~CAN_CTRL2_RFFN_MASK) | ((numOfFilters << CAN_CTRL2_RFFN_SHIFT) & CAN_CTRL2_RFFN_MASK);
        /* RX FIFO global mask, take in consideration all filter fields*/
        (base->RXFGMASK) = CAN_RXFGMASK_FGM_MASK;

        for (i = 0; i < noOfMbx; i++) {
            /* RX individual mask */
            base->RXIMR[i] = (CAN_RXIMR_MI_MASK << FLEXCAN_ID_EXT_SHIFT) & (FLEXCAN_ID_STD_MASK | FLEXCAN_ID_EXT_MASK);
        }
    }

    return stat;
}

/*
 * Function Name : FLEXCAN_SetMaxMsgBuffNum
 * Description   : Set the number of the last Message Buffers.
 * This function will config the number of the last Message Buffers. 
 * Maybe the real number of used Message Buffers is smaller than the last Message Buffers.
 *
 */
status_t FLEXCAN_SetMaxMsgBuffNum(can_type_t *base, uint32_t maxMsgBuffNum)
{
    OS_ASSERT(maxMsgBuffNum > 0U);

    uint8_t msgBuffIdx;
    uint32_t dataByte;
    const volatile uint32_t *RAM          = (uint32_t *)((uint32_t)base + (uint32_t)FEATURE_FLEXCAN_RAM_OFFSET);
    const volatile uint32_t *RAM_ENHANCED = (uint32_t *)((uint32_t)base + (uint32_t)FEATURE_FLEXCAN_EXP_RAM_OFFSET);

    const volatile uint32_t *valEndMbPointer = NULL;
    uint32_t valEndMb                        = 0U;
    uint32_t valEndRam                       = 0U;
    status_t status                          = STATUS_SUCCESS;

    uint8_t arbitration_field_size = 8U;
    uint8_t can_real_payload       = FLEXCAN_GetMbPayloadSize(base, maxMsgBuffNum - (uint32_t)1U);

    valEndMbPointer = FLEXCAN_GetMsgBuffRegion(base, (maxMsgBuffNum - (uint32_t)1U));
    valEndMb        = (uint32_t)valEndMbPointer + can_real_payload + arbitration_field_size;

    if (FLEXCAN_IsExpandableMemoryAvailable(base)) {
        valEndRam = (uint32_t)&RAM_ENHANCED[CAN_RAMn_COUNT];
    } else {
        valEndRam = (uint32_t)&RAM[(FLEXCAN_GetMaxMbNum(base) * 4U)];
    }

    if ((valEndMb > valEndRam) || (maxMsgBuffNum > FLEXCAN_GetMaxMbNum(base))) {
        status = STATUS_CAN_BUFF_OUT_OF_RANGE;
    }

    if (STATUS_SUCCESS == status) {
        /* Set the maximum number of MBs*/
        base->MCR = (base->MCR & ~CAN_MCR_MAXMB_MASK) | (((maxMsgBuffNum - 1U) << CAN_MCR_MAXMB_SHIFT) & CAN_MCR_MAXMB_MASK);
        if (!FLEXCAN_IsRxFifoEnabled(base)) {
            /* Initialize all message buffers as inactive */
            for (msgBuffIdx = 0; msgBuffIdx < maxMsgBuffNum; msgBuffIdx++) {
                volatile uint32_t *flexcan_mb     = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);
                volatile uint32_t *flexcan_mb_id  = &flexcan_mb[1];
                volatile uint8_t *flexcan_mb_data = (volatile uint8_t *)(&flexcan_mb[2]);

                *flexcan_mb    = 0x0;
                *flexcan_mb_id = 0x0;

                can_real_payload = FLEXCAN_GetMbPayloadSize(base, msgBuffIdx);
                for (dataByte = 0; dataByte < can_real_payload; dataByte++) {
                    flexcan_mb_data[dataByte] = 0x0;
                }
            }
        }
    }

    return status;
}

/*
 * Function Name : FLEXCAN_SetRxFifoFilter
 * Description   : Configure RX FIFO ID filter table elements.
 *
 */
void FLEXCAN_SetRxFifoFilter(can_type_t *base, flexcan_rx_fifo_id_element_format_t idFormat,
                             const flexcan_id_table_t *idFilterTable)
{
    OS_ASSERT(idFilterTable != NULL);

    /* Set RX FIFO ID filter table elements*/
    uint32_t i, j, numOfFilters;
    uint32_t val1 = 0, val2 = 0, val = 0;

    volatile uint32_t *filterTable = &base->RAMn[RXFIFO_FILTER_TABLE_OFFSET];

    numOfFilters = (((base->CTRL2) & CAN_CTRL2_RFFN_MASK) >> CAN_CTRL2_RFFN_SHIFT);

    switch (idFormat) {
    case (FLEXCAN_RX_FIFO_ID_FORMAT_A):
        /* One full ID (standard and extended) per ID Filter Table element.*/
        (base->MCR) =
            (((base->MCR) & ~(CAN_MCR_IDAM_MASK)) |
             ((((uint32_t)(((uint32_t)(FLEXCAN_RX_FIFO_ID_FORMAT_A)) << CAN_MCR_IDAM_SHIFT)) & CAN_MCR_IDAM_MASK)));

        for (i = 0; i < RxFifoFilterElementNum(numOfFilters); i++) {
            val = 0;

            if (idFilterTable[i].isRemoteFrame) {
                val = FLEXCAN_RX_FIFO_ACCEPT_REMOTE_FRAME << FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_RTR_SHIFT;
            }
            if (idFilterTable[i].isExtendedFrame) {
                val |= FLEXCAN_RX_FIFO_ACCEPT_EXT_FRAME << FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_IDE_SHIFT;
                filterTable[i] = val + ((idFilterTable[i].id << FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_SHIFT) &
                                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_EXT_MASK);
            } else {
                filterTable[i] = val + ((idFilterTable[i].id << FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_SHIFT) &
                                        FLEXCAN_RX_FIFO_ID_FILTER_FORMATA_STD_MASK);
            }
        }
        break;
    case (FLEXCAN_RX_FIFO_ID_FORMAT_B):
        /* Two full standard IDs or two partial 14-bit (standard and extended) IDs*/
        /* per ID Filter Table element.*/
        (base->MCR) =
            (((base->MCR) & ~(CAN_MCR_IDAM_MASK)) |
             ((((uint32_t)(((uint32_t)(FLEXCAN_RX_FIFO_ID_FORMAT_B)) << CAN_MCR_IDAM_SHIFT)) & CAN_MCR_IDAM_MASK)));

        j = 0;
        for (i = 0; i < RxFifoFilterElementNum(numOfFilters); i++) {
            val1 = 0;
            val2 = 0;

            if (idFilterTable[j].isRemoteFrame) {
                val1 = FLEXCAN_RX_FIFO_ACCEPT_REMOTE_FRAME << FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_RTR_SHIFT;
            }
            if (idFilterTable[j + 1U].isRemoteFrame) {
                val2 = FLEXCAN_RX_FIFO_ACCEPT_REMOTE_FRAME << FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_RTR_SHIFT;
            }
            if (idFilterTable[j].isExtendedFrame) {
                val1 |= FLEXCAN_RX_FIFO_ACCEPT_EXT_FRAME << FLEXCAN_RX_FIFO_ID_FILTER_FORMATAB_IDE_SHIFT;

                filterTable[i] = val1 + (((idFilterTable[j].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK) >>
                                          FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_CMP_SHIFT)
                                         << FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT1);
            } else {
                filterTable[i] = val1 + ((idFilterTable[j].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK)
                                         << FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT1);
            }
            if (idFilterTable[j + 1U].isExtendedFrame) {
                val2 |= FLEXCAN_RX_FIFO_ACCEPT_EXT_FRAME << FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_IDE_SHIFT;

                filterTable[i] |= val2 + (((idFilterTable[j + 1U].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_MASK) >>
                                           FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_CMP_SHIFT)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_EXT_SHIFT2);
            } else {
                filterTable[i] |= val2 + ((idFilterTable[j + 1U].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_MASK)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATB_STD_SHIFT2);
            }
            j = j + 2U;
        }
        break;
    case (FLEXCAN_RX_FIFO_ID_FORMAT_C):
        /* Four partial 8-bit Standard IDs per ID Filter Table element.*/
        (base->MCR) =
            (((base->MCR) & ~(CAN_MCR_IDAM_MASK)) |
             ((((uint32_t)(((uint32_t)(FLEXCAN_RX_FIFO_ID_FORMAT_C)) << CAN_MCR_IDAM_SHIFT)) & CAN_MCR_IDAM_MASK)));
        j = 0;
        for (i = 0; i < RxFifoFilterElementNum(numOfFilters); i++) {
            if (idFilterTable[j].isExtendedFrame) {
                filterTable[i] |= val1 + (((idFilterTable[j].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_MASK) >>
                                           FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_CMP_SHIFT)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1);
            } else {
                filterTable[i] |= val1 + (((idFilterTable[j].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_MASK) >>
                                           FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_CMP_SHIFT)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT1);
            }
            if (idFilterTable[j + 1U].isExtendedFrame) {
                filterTable[i] |= val1 + (((idFilterTable[j + 1U].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_MASK) >>
                                           FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_CMP_SHIFT)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2);
            } else {
                filterTable[i] |= val1 + (((idFilterTable[j + 1U].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_MASK) >>
                                           FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_CMP_SHIFT)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT2);
            }
            if (idFilterTable[j + 2U].isExtendedFrame) {
                filterTable[i] |= val1 + (((idFilterTable[j + 2U].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_MASK) >>
                                           FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_CMP_SHIFT)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3);
            } else {
                filterTable[i] |= val1 + (((idFilterTable[j + 2U].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_MASK) >>
                                           FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_CMP_SHIFT)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT3);
            }
            if (idFilterTable[j + 3U].isExtendedFrame) {
                filterTable[i] |= val1 + (((idFilterTable[j + 3U].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_MASK) >>
                                           FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_EXT_CMP_SHIFT)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT4);
            } else {
                filterTable[i] |= val1 + (((idFilterTable[j + 3U].id & FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_MASK) >>
                                           FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_STD_CMP_SHIFT)
                                          << FLEXCAN_RX_FIFO_ID_FILTER_FORMATC_SHIFT4);
            }
            j = j + 4U;
        }
        break;
    case (FLEXCAN_RX_FIFO_ID_FORMAT_D):
        /* All frames rejected.*/
        (base->MCR) =
            (((base->MCR) & ~(CAN_MCR_IDAM_MASK)) |
             ((((uint32_t)(((uint32_t)(FLEXCAN_RX_FIFO_ID_FORMAT_D)) << CAN_MCR_IDAM_SHIFT)) & CAN_MCR_IDAM_MASK)));
        break;
    default:
        /* Should not get here */
        break;
    }
}

/*
 * Function Name : FLEXCAN_SetMsgBuffIntCmd
 * Description   : Enable/Disable the corresponding Message Buffer interrupt.
 *
 */
status_t FLEXCAN_SetMsgBuffIntCmd(can_type_t *base, uint32_t msgBuffIdx, bool enable)
{
    uint32_t temp;
    status_t stat = STATUS_SUCCESS;

    /* Enable the corresponding message buffer Interrupt */
    temp = 1UL << (msgBuffIdx % 32U);
    if (msgBuffIdx < 32U) {
        if (enable) {
            (base->IMASK1) = ((base->IMASK1) | (temp));
        } else {
            (base->IMASK1) = ((base->IMASK1) & ~(temp));
        }
    }

    if ((msgBuffIdx >= 32U) && (msgBuffIdx < 64U)) {
        if (enable) {
            (base->IMASK2) = ((base->IMASK2) | (temp));
        } else {
            (base->IMASK2) = ((base->IMASK2) & ~(temp));
        }
    }

    if ((msgBuffIdx >= 64U) && (msgBuffIdx < 96U)) {
        if (enable) {
            (base->IMASK3) = ((base->IMASK3) | (temp));
        } else {
            (base->IMASK3) = ((base->IMASK3) & ~(temp));
        }
    }

    if (msgBuffIdx >= 96U) {
        if (enable) {
            (base->IMASK4) = ((base->IMASK4) | (temp));
        } else {
            (base->IMASK4) = ((base->IMASK4) & ~(temp));
        }
    }

    return stat;
}

/*
 * Function Name : FLEXCAN_SetErrIntCmd
 * Description   : Enable the error interrupts.
 * This function will enable Error interrupt.
 *
 */
void FLEXCAN_SetErrIntCmd(can_type_t *base, flexcan_int_type_t errType, bool enable)
{
    uint8_t instance = FLEXCAN_Base2Instance(base);
    bool instHasFd   = FLEXCAN_InstanceHasFD(instance);
    uint32_t temp    = (uint32_t)errType;
    if (enable) {
        if ((errType == FLEXCAN_INT_RX_WARNING) || (errType == FLEXCAN_INT_TX_WARNING)) {
            base->MCR = (base->MCR & ~CAN_MCR_WRNEN_MASK) | CAN_MCR_WRNEN(1U);
        }

        if (instHasFd) {

            if (errType == FLEXCAN_INT_ERR) {
                base->CTRL2 = (base->CTRL2 & ~CAN_CTRL2_ERRMSK_FAST_MASK) | CAN_CTRL2_ERRMSK_FAST(1U);
            }
        }

        (base->CTRL1) = ((base->CTRL1) | (temp));
    } else {
        (base->CTRL1) = ((base->CTRL1) & ~(temp));

        if (instHasFd) {

            if (errType == FLEXCAN_INT_ERR) {
                base->CTRL2 = (base->CTRL2 & ~CAN_CTRL2_ERRMSK_FAST_MASK) | CAN_CTRL2_ERRMSK_FAST(0U);
            }
        }

        temp = base->CTRL1;
        if (((temp & (uint32_t)FLEXCAN_INT_RX_WARNING) == 0U) && ((temp & (uint32_t)FLEXCAN_INT_TX_WARNING) == 0U)) {
            /* If WRNEN disabled then both FLEXCAN_INT_RX_WARNING and FLEXCAN_INT_TX_WARNING will be disabled */
            base->MCR = (base->MCR & ~CAN_MCR_WRNEN_MASK) | CAN_MCR_WRNEN(0U);
        }
    }
}

/*
 * Function Name : FLEXCAN_ExitFreezeMode
 * Description   : Exit of freeze mode.
 *
 */
void FLEXCAN_ExitFreezeMode(can_type_t *base)
{
    base->MCR = (base->MCR & ~CAN_MCR_HALT_MASK) | CAN_MCR_HALT(0U);
    base->MCR = (base->MCR & ~CAN_MCR_FRZ_MASK) | CAN_MCR_FRZ(0U);

    /* Wait till exit freeze mode */
    while (((base->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT) != 0U) {
    }
}

/*
 * Function Name : FLEXCAN_EnterFreezeMode
 * Description   : Enter the freeze mode.
 *
 */
void FLEXCAN_EnterFreezeMode(can_type_t *base)
{
    bool enabled     = false;
    uint8_t instance = FLEXCAN_Base2Instance(base);
    bool instHasFd   = FLEXCAN_InstanceHasFD(instance);
    base->MCR        = (base->MCR & ~CAN_MCR_FRZ_MASK) | CAN_MCR_FRZ(1U);
    base->MCR        = (base->MCR & ~CAN_MCR_HALT_MASK) | CAN_MCR_HALT(1U);
    if (((base->MCR & CAN_MCR_MDIS_MASK) >> CAN_MCR_MDIS_SHIFT) == 0U) {
        enabled = true;
    } else {
        base->MCR &= ~CAN_MCR_MDIS_MASK;
    }

    OS_DelayUs(1000);

    base->TIMER  = 0U;
    uint32_t aux = 0U;

    if (instHasFd) {
        /* MCR[FDEN] was reset to 0, wait for timeout */
        if (!FLEXCAN_IsFDEnabled(base)) {
            while (!FLEXCAN_GetFreezeMode(base) && (aux < 180U)) {
                /* Wait until finish counting 180 bit times and exit*/
                aux = (uint32_t)base->TIMER;
            }
        } else {
            while (!FLEXCAN_GetFreezeMode(base) && (aux < 730U)) {
                /* Wait until finish counting 730 bit times and exit*/
                aux = (uint32_t)base->TIMER;
            }
        }
    } else {
        while (!FLEXCAN_GetFreezeMode(base) && (aux < 180U)) {
            /* Wait until finish counting 180 bit times and exit*/
            aux = (uint32_t)base->TIMER;
        }
    }

    if (((base->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT) == 0U) {
        /* Save registers before Soft Reset */
        uint32_t tempIMSK1, tempMCR;

        /* Save IMASK1 value */
        tempIMSK1 = base->IMASK1;

        uint32_t tempIMSK2 = 0U;

        if (FLEXCAN_GetMaxMbNum(base) > 32U) {
            /* Save IMASK2 value */
            tempIMSK2 = base->IMASK2;
        }

        uint32_t tempIMSK3 = 0U;

        if (FLEXCAN_GetMaxMbNum(base) > 64U) {
            /* Save IMASK3 value */
            tempIMSK3 = base->IMASK3;
        }

        uint32_t tempIMSK4 = 0U;

        if (FLEXCAN_GetMaxMbNum(base) > 96U) {
            /* Save IMASK4 value */
            tempIMSK4 = base->IMASK4;
        }

        /* Save MCR value */
        tempMCR = base->MCR;

        /* Soft Reset FlexCan */
        base->MCR |= CAN_MCR_SOFTRST(1U);
        OS_DelayUs(1000);

        while (((base->MCR & CAN_MCR_SOFTRST_MASK) >> CAN_MCR_SOFTRST_SHIFT) != 0U) {
        }

        /* Restore IMASK1 value */
        base->IMASK1 = tempIMSK1;

        if (FLEXCAN_GetMaxMbNum(base) > 32U) {
            /* Restore IMASK2 value */
            base->IMASK2 = tempIMSK2;
        }

        if (FLEXCAN_GetMaxMbNum(base) > 64U) {
            /* Restore IMASK3 value */
            base->IMASK3 = tempIMSK3;
        }

        if (FLEXCAN_GetMaxMbNum(base) > 96U) {
            /* Restore IMASK4 value */
            base->IMASK4 = tempIMSK4;
        }

        /* Restore MCR value */
        base->MCR = tempMCR;
        OS_DelayUs(1000);
    }

    if (false == enabled) {
        base->MCR |= CAN_MCR_MDIS_MASK;
        OS_DelayUs(1000);
        /* Wait until disable mode acknowledged */
        while (((base->MCR & CAN_MCR_LPMACK_MASK) >> CAN_MCR_LPMACK_SHIFT) == 0U) {
        }
    }
}

/*
 * Function Name : FLEXCAN_ClearErrIntStatusFlag
 * Description   : Clear all error interrupt status.
 *
 */
void FLEXCAN_ClearErrIntStatusFlag(can_type_t *base)
{
    if ((base->ESR1 & FLEXCAN_ALL_INT) != 0U) {
        (base->ESR1) = ERROR_INT;
    }
}

/*
 * Function Name : FLEXCAN_ClearBusOffIntStatusFlag
 * Description   : Clear all busOff and Tx/Rx Warning interrupt status.
 *
 */
void FLEXCAN_ClearBusOffIntStatusFlag(can_type_t *base)
{
    if ((base->ESR1 & BUS_OFF_INT) != 0U) {
        base->ESR1 = BUS_OFF_INT;
    }
}

/*
 * Function Name : FLEXCAN_ReadRxFifo
 * Description   : Read Rx FIFO data.
 * This function will copy MB[0] data field into user's buffer.
 *
 */
void FLEXCAN_ReadRxFifo(const can_type_t *base, flexcan_msgbuff_t *rxFifo)
{
    OS_ASSERT(rxFifo != NULL);

    uint32_t dataByte;
    uint32_t mbWord;

    volatile const uint32_t *flexcan_mb         = base->RAMn;
    volatile const uint32_t *flexcan_mb_id      = &base->RAMn[1];
    volatile const uint32_t *flexcan_mb_data_32 = &flexcan_mb[2];
    uint32_t *msgData_32                        = (uint32_t *)(rxFifo->data);

    uint8_t flexcan_mb_dlc_value = (uint8_t)(((*flexcan_mb) & FLEXCAN_CS_DLC_MASK) >> 16);
    uint8_t can_real_payload     = FLEXCAN_ComputePayloadSize(flexcan_mb_dlc_value);

    rxFifo->dataLen = can_real_payload;
    rxFifo->cs      = *flexcan_mb;

    if ((rxFifo->cs & FLEXCAN_CS_IDE_MASK) != 0U) {
        rxFifo->msgId = *flexcan_mb_id;
    } else {
        rxFifo->msgId = (*flexcan_mb_id) >> FLEXCAN_ID_STD_SHIFT;
    }

    /* Extract the IDHIT */
    rxFifo->id_hit = (uint8_t)base->RXFIR;
    /* Extract the Time Stamp */
    rxFifo->time_stamp = (uint32_t)((rxFifo->cs & FLEXCAN_CS_TIME_STAMP_MASK) >> FLEXCAN_CS_TIME_STAMP_SHIFT);

    /* Copy MB[0] data field into user's buffer */
    for (dataByte = 0U; dataByte < can_real_payload; dataByte += 4U) {
        mbWord = flexcan_mb_data_32[dataByte >> 2U];
        FlexcanSwapBytesInWord(mbWord, msgData_32[dataByte >> 2U]);
    }
}

/*
 * Function Name : FLEXCAN_SetOperationMode
 * Description   : Enable a FlexCAN operation mode.
 * This function will enable one of the modes listed in flexcan_operation_modes_t.
 *
 */
void FLEXCAN_SetOperationMode(can_type_t *base, flexcan_operation_modes_t mode)
{

    switch (mode) {
    case FLEXCAN_FREEZE_MODE:
        /* Debug mode, Halt and Freeze*/
        FLEXCAN_EnterFreezeMode(base);
        break;
    case FLEXCAN_DISABLE_MODE:
        /* Debug mode, Halt and Freeze */
        base->MCR = (base->MCR & ~CAN_MCR_MDIS_MASK) | CAN_MCR_MDIS(1U);
        break;
    case FLEXCAN_NORMAL_MODE:
        base->MCR   = (base->MCR & ~CAN_MCR_SUPV_MASK) | CAN_MCR_SUPV(0U);
        base->CTRL1 = (base->CTRL1 & ~CAN_CTRL1_LOM_MASK) | CAN_CTRL1_LOM(0U);
        base->CTRL1 = (base->CTRL1 & ~CAN_CTRL1_LPB_MASK) | CAN_CTRL1_LPB(0U);
        break;
    case FLEXCAN_LISTEN_ONLY_MODE:
        base->CTRL1 = (base->CTRL1 & ~CAN_CTRL1_LOM_MASK) | CAN_CTRL1_LOM(1U);
        break;
    case FLEXCAN_LOOPBACK_MODE:
        base->CTRL1 = (base->CTRL1 & ~CAN_CTRL1_LPB_MASK) | CAN_CTRL1_LPB(1U);
        base->CTRL1 = (base->CTRL1 & ~CAN_CTRL1_LOM_MASK) | CAN_CTRL1_LOM(0U);
        /* Enable Self Reception */
        FLEXCAN_SetSelfReception(base, true);
        break;
    default:
        /* Should not get here */
        break;
    }
}

/*
 * Function Name : FLEXCAN_IsOutOfRangeMbIdx
 * Description   : Returns True if selected Message Buffer index is out of range.
 *
 */
bool FLEXCAN_IsOutOfRangeMbIdx(const can_type_t *base, uint32_t msgBuffIdx)
{
    /* Check if the MB index is in range */
    if (msgBuffIdx > (((base->MCR) & CAN_MCR_MAXMB_MASK) >> CAN_MCR_MAXMB_SHIFT)) {
        return true;
    }

    /* Check if RX FIFO is enabled*/
    if (FLEXCAN_IsRxFifoEnabled(base) == true) {
        uint32_t val1 = 0U, val2 = 0U;
        /* Get the number of RX FIFO Filters*/
        val1 = (((uint32_t)(base->CTRL2) & CAN_CTRL2_RFFN_MASK) >> CAN_CTRL2_RFFN_SHIFT);
        /* Get the number if MBs occupied by RX FIFO and ID filter table*/
        /* the Rx FIFO occupies the memory space originally reserved for MB0-5*/
        /* Every number of RFFN means 8 number of RX FIFO filters*/
        /* and every 4 number of RX FIFO filters occupied one MB*/
        val2 = RxFifoOccupiedLastMsgBuff(val1);

        if (msgBuffIdx <= val2) {
            return true;
        }
    }

    return false;
}

/*
 * Function Name : FLEXCAN_IsEnhancedRxFifoAvailable
 * Description   : Checks if FlexCAN has Enhanced Rx FIFO.
 * This function is private.
 *
 */
bool FLEXCAN_IsEnhancedRxFifoAvailable(const can_type_t *base)
{
    uint32_t i                             = 0U;
    static can_type_t *const FlexcanBase[] = FLEXCAN_BASE_PTRS_HAS_ENHANCED_RX_FIFO;
    bool ReturnValue                       = false;

    for (i = 0U; i < FEATURE_FLEXCAN_ENHANCED_RX_FIFO_NUM; i++) {
        if (base == FlexcanBase[i]) {
            ReturnValue = true;
            break;
        }
    }

    return ReturnValue;
}

/*
 * Function Name : FLEXCAN_EnableEnhancedRxFifo
 * Description   : Enable Enhanced Rx FIFO feature.
 * This function will enable the Enhanced Rx FIFO feature.
 *
 */
status_t FLEXCAN_EnableEnhancedRxFifo(can_type_t *base, uint32_t numOfStdIDFilters, uint32_t numOfExtIDFilters,
                                      uint32_t numOfWatermark)
{
    status_t Stat                 = STATUS_SUCCESS;
    uint32_t NumOfEnhancedFilters = 0U;

    if (FLEXCAN_IsEnhancedRxFifoAvailable(base)) {

        /* NumOfEnhancedFilters equals (numOfStdIDFilters/2) + numOfExtIDFilters - 1U */
        NumOfEnhancedFilters = (numOfStdIDFilters >> 1U) + numOfExtIDFilters - 1U;

        if ((0U == numOfStdIDFilters) && (0U == numOfExtIDFilters)) {
            Stat = STATUS_ERROR;
        }
        /* If the no of Std Filters is odd */
        if (1U == (numOfStdIDFilters & 1U)) {
            Stat = STATUS_ERROR;
        }
        /* Enhanced RX FIFO and Legacy RX FIFO cannot be enabled at the same time.*/
        if (CAN_MCR_RFEN_MASK == (base->MCR & CAN_MCR_RFEN_MASK)) {
            Stat = STATUS_ERROR;
        }

        if (STATUS_SUCCESS == Stat) {
            /* Enable Enhanced Rx FIFO */
            base->ERFCR = (base->ERFCR & ~CAN_ERFCR_ERFEN_MASK) | CAN_ERFCR_ERFEN(1U);
            /* Reset Enhanced Rx FIFO engine */
            base->ERFSR = (base->ERFSR & ~CAN_ERFSR_ERFCLR_MASK) | CAN_ERFSR_ERFCLR(1U);
            /* Clear the status bits of the Enhanced RX FIFO */
            base->ERFSR = base->ERFSR & ~(CAN_ERFSR_ERFUFW_MASK | CAN_ERFSR_ERFOVF_MASK | CAN_ERFSR_ERFWMI_MASK |
                                          CAN_ERFSR_ERFDA_MASK);
            /* Set the total number of enhanced Rx FIFO filter elements */
            base->ERFCR = (base->ERFCR & ~CAN_ERFCR_NFE_MASK) |
                          ((NumOfEnhancedFilters << CAN_ERFCR_NFE_SHIFT) & CAN_ERFCR_NFE_MASK);
            /* Set the number of extended ID filter elements */
            base->ERFCR = (base->ERFCR & ~CAN_ERFCR_NEXIF_MASK) |
                          ((numOfExtIDFilters << CAN_ERFCR_NEXIF_SHIFT) & CAN_ERFCR_NEXIF_MASK);
            /* Set the Enhanced Rx FIFO watermark */
            base->ERFCR = (base->ERFCR & ~CAN_ERFCR_ERFWM_MASK) |
                          ((numOfWatermark << CAN_ERFCR_ERFWM_SHIFT) & CAN_ERFCR_ERFWM_MASK);
        }
    } else {
        Stat = STATUS_ERROR;
    }

    return Stat;
}

/*
 * Function Name : FLEXCAN_SetEnhancedRxFifoFilter
 * Description   : Configure Enhanced RX FIFO ID filter table elements.
 *
 */
void FLEXCAN_SetEnhancedRxFifoFilter(can_type_t *base, const flexcan_enhancedIdTableType_t *idFilterTable)
{
    /* Set Enhanced RX FIFO ID filter table elements*/
    uint32_t i, j, n, NumOfEnhancedFilter, NumOfExtIDFilter, NumOfStdIDFilter;
    uint32_t Val2 = 0U, Val1 = 0U, Val = 0U;

    volatile uint32_t *FilterExtIDTable = NULL;
    volatile uint32_t *FilterStdIDTable = NULL;

    NumOfEnhancedFilter = (((base->ERFCR) & CAN_ERFCR_NFE_MASK) >> CAN_ERFCR_NFE_SHIFT);
    NumOfExtIDFilter    = (((base->ERFCR) & CAN_ERFCR_NEXIF_MASK) >> CAN_ERFCR_NEXIF_SHIFT);
    NumOfStdIDFilter    = 2U * (NumOfEnhancedFilter - NumOfExtIDFilter + 1U);
    FilterExtIDTable    = (volatile uint32_t *)&base->ERFFELn[FLEXCAN_ENHANCED_RX_FIFO_FILTER_TABLE_BASE];
    FilterStdIDTable    = (volatile uint32_t *)&base->ERFFELn[NumOfExtIDFilter * 2U];
    j                   = 0U;
    n                   = 0U;
    for (i = 0U; i < (NumOfExtIDFilter + NumOfStdIDFilter); i++) {
        if (!(idFilterTable[i].isExtendedFrame)) {
            Val = 0U;
            if (idFilterTable[i].rtr2) {
                Val = FLEXCAN_RX_FIFO_ACCEPT_REMOTE_FRAME << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_RTR2_SHIFT;
            }

            if (idFilterTable[i].rtr1) {
                Val |= FLEXCAN_RX_FIFO_ACCEPT_REMOTE_FRAME << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_RTR1_SHIFT;
            }

            FilterStdIDTable[j] = ((idFilterTable[i].id2 & FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_MASK)
                                   << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_SHIFT2) |
                                  ((idFilterTable[i].id1 & FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_MASK)
                                   << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_STD_SHIFT1) |
                                  Val;
            switch (idFilterTable[i].filterType) {
            case FLEXCAN_ENHANCED_RX_FIFO_ONE_ID_FILTER: {
                FilterStdIDTable[j] |= (uint32_t)FLEXCAN_ENHANCED_RX_FIFO_ONE_ID_FILTER
                                       << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_FSCH_SHIFT;
            } break;

            case FLEXCAN_ENHANCED_RX_FIFO_RANGE_ID_FILTER: {
                FilterStdIDTable[j] |= (uint32_t)FLEXCAN_ENHANCED_RX_FIFO_RANGE_ID_FILTER
                                       << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_FSCH_SHIFT;
            } break;

            case FLEXCAN_ENHANCED_RX_FIFO_TWO_ID_FILTER: {
                FilterStdIDTable[j] |= (uint32_t)FLEXCAN_ENHANCED_RX_FIFO_TWO_ID_FILTER
                                       << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_FSCH_SHIFT;
            } break;

            default: {
                OS_ASSERT(false);
                /* Should not get here */
            } break;
            }

            j++;
        } else {
            Val2 = 0U;
            Val1 = 0U;
            if (idFilterTable[i].rtr2) {
                Val2 = FLEXCAN_RX_FIFO_ACCEPT_REMOTE_FRAME << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_EXT_RTR_SHIFT;
            }

            if (idFilterTable[i].rtr1) {
                Val1 = FLEXCAN_RX_FIFO_ACCEPT_REMOTE_FRAME << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_EXT_RTR_SHIFT;
            }
            FilterExtIDTable[n] = ((idFilterTable[i].id2 & FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_EXT_MASK)
                                   << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_EXT_SHIFT) |
                                  Val2;

            FilterExtIDTable[n + 1U] = ((idFilterTable[i].id1 & FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_EXT_MASK)
                                        << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_EXT_SHIFT) |
                                       Val1;
            switch (idFilterTable[i].filterType) {
            case FLEXCAN_ENHANCED_RX_FIFO_ONE_ID_FILTER: {
                FilterExtIDTable[n] |= (uint32_t)FLEXCAN_ENHANCED_RX_FIFO_ONE_ID_FILTER
                                       << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_FSCH_SHIFT;
            } break;
            case FLEXCAN_ENHANCED_RX_FIFO_RANGE_ID_FILTER: {
                FilterExtIDTable[n] |= (uint32_t)FLEXCAN_ENHANCED_RX_FIFO_RANGE_ID_FILTER
                                       << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_FSCH_SHIFT;
            } break;
            case FLEXCAN_ENHANCED_RX_FIFO_TWO_ID_FILTER: {
                FilterExtIDTable[n] |= (uint32_t)FLEXCAN_ENHANCED_RX_FIFO_TWO_ID_FILTER
                                       << FLEXCAN_ENHANCED_RX_FIFO_ID_FILTER_FSCH_SHIFT;
            } break;
            default: {
                OS_ASSERT(false);
                /* Should not get here */
            } break;
            }
            n = n + 2U;
        }
    }
}

/*
 * Function Name : FLEXCAN_ClearOutputEnhanceFIFO
 * Description   : Clear output of enhance fifo.
 *
 */
void FLEXCAN_ClearOutputEnhanceFIFO(can_type_t *base)
{
    volatile const uint32_t *Ram = (uint32_t *)((uint32_t)base);
    uint32_t LastWord_Offset     = ((uint32_t)0x204C) / ((uint32_t)4U); // fixed, because DMALW is always = 19
    uint8_t u8TimeOut            = 0;

    /* If Enhanced Rx FIFO has Pending Request that generated error,
     * the EnhancedRxFIFO need to be empty to activate PDMA */
    if ((uint8_t)1U == FLEXCAN_GetEnhancedRxFIFOStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE)) {
        /* Enter CAN in freeze Mode to allow Enhanced Rx FIFO Clear */
        (void)FLEXCAN_EnterFreezeMode(base);
        FLEXCAN_ClearEnhancedFIFO(base);

        do {
            if ((uint32_t)CAN_MCR_DMA_MASK == (base->MCR & ((uint32_t)CAN_MCR_DMA_MASK))) {
                /* Read Enhanced Output to clear PDMA pending request */
                (void)Ram[LastWord_Offset];
            } else {
                FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE);
            }
            u8TimeOut++;
        } while (((uint8_t)1U == FLEXCAN_GetEnhancedRxFIFOStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE)) &&
                 (u8TimeOut <= ((uint8_t)64U))); // avoid blocking

        FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_OVERFLOW);
        FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_WATERMARK);
        FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_FRAME_AVAILABLE);
        FLEXCAN_ClearEnhancedRxFifoIntStatusFlag(base, FLEXCAN_ENHANCED_RXFIFO_UNDERFLOW);

        /* Return CAN to normal Mode */
        FLEXCAN_ExitFreezeMode(base);
    }
}

/*
 * Function Name : FLEXCAN_SetPayloadSize
 * Description   : Sets the payload size of the MBs of all ram block regions.
 *
 */
void FLEXCAN_SetPayloadSize(can_type_t *base, flexcan_payload_block_size_t payload)
{
    uint32_t tmpReg;

    /* If FD is not enabled, selected classic can. Only 8 bytes payload type used */
    if (FLEXCAN_IsFDEnabled(base)) {
        tmpReg = base->FDCTRL;
        tmpReg &= ~(CAN_FDCTRL_MBDSR0_MASK);
        tmpReg |= ((uint32_t)payload.blockR0) << CAN_FDCTRL_MBDSR0_SHIFT;
        tmpReg &= ~(CAN_FDCTRL_MBDSR1_MASK);
        tmpReg |= ((uint32_t)payload.blockR1) << CAN_FDCTRL_MBDSR1_SHIFT;
        tmpReg &= ~(CAN_FDCTRL_MBDSR2_MASK);
        tmpReg |= ((uint32_t)payload.blockR2) << CAN_FDCTRL_MBDSR2_SHIFT;
        tmpReg &= ~(CAN_FDCTRL_MBDSR3_MASK);
        tmpReg |= ((uint32_t)payload.blockR3) << CAN_FDCTRL_MBDSR3_SHIFT;
        base->FDCTRL = tmpReg;
    }
}

/*
 * Function Name : FLEXCAN_GetPayloadSize
 * Description   : Returns the MB's payload size of the block region (in bytes).
 *
 */
static uint8_t FLEXCAN_GetPayloadSize(const can_type_t *base, uint8_t blockIdx)
{
    uint32_t payloadSize = 8U;

    /* The classic can's payload size is 8 bytes */
    if (!FLEXCAN_IsFDEnabled(base)) {
        payloadSize = 8U;
    } else {
        if (0U == blockIdx) {
            payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR0_MASK) >> CAN_FDCTRL_MBDSR0_SHIFT) + 3U);
        } else if (1U == blockIdx) {
            payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR1_MASK) >> CAN_FDCTRL_MBDSR1_SHIFT) + 3U);
        } else if (2U == blockIdx) {
            payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR2_MASK) >> CAN_FDCTRL_MBDSR2_SHIFT) + 3U);
        } else if (3U == blockIdx) {
            payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR3_MASK) >> CAN_FDCTRL_MBDSR3_SHIFT) + 3U);
        } else {
            payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR0_MASK) >> CAN_FDCTRL_MBDSR0_SHIFT) + 3U);
        }
    }

    return payloadSize;
}

/*
 * Function Name : FLEXCAN_GetMbPayloadSize
 * Description   : Returns the payload size of the max MB number (in bytes).
 *
 */
uint8_t FLEXCAN_GetMbPayloadSize(const can_type_t *base, uint32_t maxMbNum)
{
    uint8_t arbitration_field_size = 8U;
    uint32_t blockRegionSize       = 512U;
    uint8_t real_payload_bytes     = 8U;
    uint8_t maxMbBlockNum          = 0U;
    uint8_t i                      = 0U;
    uint8_t mb_size                = 0U;

    uint8_t instance   = FLEXCAN_Base2Instance(base);
    bool instHasFd     = FLEXCAN_InstanceHasFD(instance);
    bool enhancedMbSup = FLEXCAN_IsExpandableMemoryAvailable(base);

    for (i = 0; i < (uint8_t)FLEXCAN_MBDSR_COUNT_OF_NORMAL_RAM_BLOCK; i++) {
        /* Check that the number of MBs is supported based on the payload size*/
        if (instHasFd) {
            real_payload_bytes = FLEXCAN_GetPayloadSize(base, i);
        } /* else, classic can's payload is a fixed value, 8 byte */

        mb_size = real_payload_bytes + arbitration_field_size;
        maxMbBlockNum += (uint8_t)(blockRegionSize / mb_size);
        if (maxMbBlockNum > maxMbNum) {
            break;
        }
    }

    if (enhancedMbSup) {
        /* exceeded normal MB ram block region */
        if ((uint8_t)FLEXCAN_MBDSR_COUNT_OF_NORMAL_RAM_BLOCK == i) {
            real_payload_bytes = 64U;
        }
    }

    return real_payload_bytes;
}

/*
 * Function Name : FLEXCAN_ConfigPN
 * Description   : Configures the Pretended Networking mode.
 *
 */
void FLEXCAN_ConfigPN(can_type_t *base, const flexcan_pn_config_t *pnConfig)
{
    OS_ASSERT(pnConfig != NULL);

    /* Configure specific pretended networking settings */
    FLEXCAN_SetPNFilteringSelection(base, pnConfig->wakeUpTimeout, pnConfig->wakeUpMatch, pnConfig->numMatches,
                                    pnConfig->filterComb, pnConfig->idFilterType, pnConfig->payloadFilterType);

    FLEXCAN_SetPNTimeoutValue(base, pnConfig->matchTimeout);

    /* Configure ID filtering */
    FLEXCAN_SetPNIdFilter1(base, pnConfig->idFilter1);

    /* Configure the second ID, if needed (as mask for exact matching or higher limit for range matching) */
    if ((pnConfig->idFilterType == FLEXCAN_FILTER_MATCH_EXACT) ||
        (pnConfig->idFilterType == FLEXCAN_FILTER_MATCH_RANGE)) {
        FLEXCAN_SetPNIdFilter2(base, pnConfig);
    } else {
        /* In other case need only to check the IDE and RTR match the ID_MASK is not considered */
        FLEXCAN_SetPNIdFilter2Check(base);
    }

    /* Configure payload filtering, if requested */
    if ((pnConfig->filterComb == FLEXCAN_FILTER_ID_PAYLOAD) ||
        (pnConfig->filterComb == FLEXCAN_FILTER_ID_PAYLOAD_NTIMES)) {
        FLEXCAN_SetPNDlcFilter(base, pnConfig->payloadFilter.dlcLow, pnConfig->payloadFilter.dlcHigh);

        FLEXCAN_SetPNPayloadHighFilter1(base, pnConfig->payloadFilter.payload1);
        FLEXCAN_SetPNPayloadLowFilter1(base, pnConfig->payloadFilter.payload1);

        /* Configure the second payload, if needed (as mask for exact matching or higher limit for range matching) */
        if ((pnConfig->payloadFilterType == FLEXCAN_FILTER_MATCH_EXACT) ||
            (pnConfig->payloadFilterType == FLEXCAN_FILTER_MATCH_RANGE)) {
            FLEXCAN_SetPNPayloadHighFilter2(base, pnConfig->payloadFilter.payload2);
            FLEXCAN_SetPNPayloadLowFilter2(base, pnConfig->payloadFilter.payload2);
        }
    }
}

/*
 * Function Name : FLEXCAN_ConfigTimestamp
 * Description   : Only configures the time stamp setting in the Freeze mode.
 *
 */
void FLEXCAN_ConfigTimestamp(can_type_t *base, const flexcan_timeStampConfigType_t *config)
{
    uint32_t Temp = 0U;

    Temp = CAN_CTRL2_TIMER_SRC(config->timeStampSource);
    Temp |= CAN_CTRL2_MBTSBASE(config->msgBuffTimeStampType);
    Temp |= CAN_CTRL2_TSTAMPCAP(config->hrConfigType);
    base->CTRL2 &= ~(CAN_CTRL2_TIMER_SRC_MASK | CAN_CTRL2_MBTSBASE_MASK | CAN_CTRL2_TSTAMPCAP_MASK);
    base->CTRL2 |= Temp;
}
