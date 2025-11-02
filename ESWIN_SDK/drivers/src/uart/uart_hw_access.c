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
 * @file uart_hw_access.c
 * @brief UART driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "uart_hw_access.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_Init
 * Description   : Initializes the UART controller to known state, using
 *                 register reset values defined in the reference manual.
 *
 *END**************************************************************************/
void UART_Init(uart_type_t *base)
{
    /* reset UART */
    base->GLOBAL = UART_GLOBAL_RST(1);
    // OS_DelayMs(1);
    base->GLOBAL = UART_GLOBAL_RST(0);

    /* Set the default oversampling ratio (16) and baud-rate divider (4) */
    base->BAUD = ((uint32_t)((UART_DEFAULT_OSR << UART_BAUD_OSR_SHIFT) |
                             (UART_DEFAULT_SBR << UART_BAUD_SBR_SHIFT)));
    /* Clear the error/interrupt flags */
    base->STAT = UART_STAT_REG_FLAGS_MASK;
    /* Reset all features/interrupts by default */
    base->CTRL = 0x00000000;
    /* Reset match addresses */
    base->MATCH = 0x00000000;

    /* Reset IrDA modem features */
    base->MODIR = 0x00000000;
    // base->MODIR = 0x00000009; // TODO:zhoulei RTS CTS

    /* Reset FIFO feature */
    base->FIFO = UART_FIFO_EN_RESET_MASK;
    /* Reset FIFO Watermark values */
    base->WATER = 0x00000000;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_FIFOWater_Init
 *
 *END**************************************************************************/
void UART_FIFOWater_Init(uart_type_t *base, uint16_t water)
{
    uint32_t val = 0;
    /* Watermark values */
    val         = water | (water << 16);
    base->WATER = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_FIFO_Disable
 *
 *END**************************************************************************/
void UART_FIFO_Disable(uart_type_t *base)
{
    base->FIFO = UART_FIFO_DISEN_RESET_MASK;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetLoopback
 * Description   : set loop back mode
 *
 *END**************************************************************************/
void UART_SetLoopback(uart_type_t *base, bool en)
{
    base->CTRL &= (~UART_CTRL_LOOPS_MASK);
    base->CTRL |= (UART_CTRL_LOOPS(en));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetBitCountPerChar
 * Description   : Configures the number of bits per char in UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_SetBitCountPerChar(uart_type_t *base, uart_bit_count_per_char_t bitCountPerChar, bool parity)
{
    uint32_t tmpBitCountPerChar = (uint32_t)bitCountPerChar;

    if ((uint32_t)UART_7_BITS_PER_CHAR == tmpBitCountPerChar) {
        base->CTRL |= UART_CTRL_M7_MASK;
        base->BAUD &= ~UART_BAUD_M10_MASK;
    } else {
        if (parity) {
            tmpBitCountPerChar += 1U;
        }

        if ((uint32_t)UART_10_BITS_PER_CHAR == tmpBitCountPerChar) {
            base->BAUD = (base->BAUD & ~UART_BAUD_M10_MASK) | ((uint32_t)1U << UART_BAUD_M10_SHIFT);
        } else {
            /* config 8-bit (M=0) or 9-bits (M=1) */
            base->CTRL = (base->CTRL & ~UART_CTRL_M_MASK) | (tmpBitCountPerChar << UART_CTRL_M_SHIFT);
            /* clear M10 to make sure not 10-bit mode */
            base->BAUD &= ~UART_BAUD_M10_MASK;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_MSBFMode
 * Description   : Configures MSB first or LSB first mode in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_MSBFMode(uart_type_t *base, uart_msbf_mode_t msbModeType)
{
    base->STAT = (base->STAT & ~UART_STAT_MSBF_MASK) | ((uint32_t)msbModeType << UART_STAT_MSBF_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_OverrunIREnable
 * Description   : Enables overrun interupt.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_OverrunIREnable(uart_type_t *base, uart_overrrun_interrupt_type_t OverrunIrqType)
{
    base->CTRL = (base->CTRL & ~UART_CTRL_ORIE_MASK) | ((uint32_t)OverrunIrqType << UART_CTRL_ORIE_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetTxInv
 * Description   : Configures transmit data inverse in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_SetTxInv(uart_type_t *base, uart_tx_rx_inve_t invModeType)
{
    base->CTRL = (base->CTRL & ~UART_CTRL_TXINV_MASK) | ((uint32_t)invModeType << UART_CTRL_TXINV_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetTxInv
 * Description   : Configures receive data inverse in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_SetRxInv(uart_type_t *base, uart_tx_rx_inve_t invModeType)
{
    base->STAT = (base->STAT & ~UART_STAT_RXINV_MASK) | ((uint32_t)invModeType << UART_STAT_RXINV_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetIdleIREnable
 * Description   : This function enables the idle line interrupt.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_SetIdleIREnable(uart_type_t *base, uart_idle_interrupt_type_t idleIRType)
{
    base->CTRL = (base->CTRL & ~UART_CTRL_ILIE_MASK) | ((uint32_t)idleIRType << UART_CTRL_ILIE_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetIdleCharsLen
 * Description   : Configures the number of idle characters that must be received before the IDLE flag is set.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_SetIdleCharsLen(uart_type_t *base, uart_character_type_t idleCharsType)
{
    UART_SetIdleLineType(base, UART_LINE_AFTER_STOP);
    base->CTRL = (base->CTRL & ~UART_CTRL_IDLECFG_MASK) | ((uint32_t)idleCharsType << UART_CTRL_IDLECFG_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetIdleLineType
 * Description   : Determines when the receiver starts counting logic 1s as idle character bits. 
 * The count begins either after a valid start bit or after the stop bit.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_SetIdleLineType(uart_type_t *base, uart_idle_line_type_t idleLineType)
{
    base->CTRL = (base->CTRL & ~UART_CTRL_ILT_MASK) | ((uint32_t)idleLineType << UART_CTRL_ILT_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_IsTxFIFOEmpty
 * Description   : Asserts when there is no data in the Transmit FIFO/buffer. 
 * This field does not take into account data that is in the transmit shift register.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
bool UART_IsTxFIFOEmpty(uart_type_t *base)
{
    uint32_t isEmpty = (base->FIFO & UART_FIFO_TXEMPT_MASK);
    return !!isEmpty;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_IsRxOverrun
 * Description   : Indicates whether there is a receiver overrun event. 
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
bool UART_IsRxOverrun(uart_type_t *base)
{
    uint32_t isOverrun = (base->STAT & UART_STAT_OR_MASK);
    return isOverrun == 0x80000U ? true : false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_IsRxActiveEage
 * Description   : If the function returns 1, it indicates that there is an active edge on the RXD pin. 
 * If it is 0, it indicates that there is no active edge on the RXD pin. 
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
bool UART_IsRxActiveEage(uart_type_t *base)
{
    uint32_t isRxActive = (base->STAT & UART_STAT_RXEDGIF_MASK);
    return isRxActive == 40000000U ? true : false;
}

uint32_t UART_TxFIFODepth(uart_type_t *base)
{
    uint32_t depth = (base->FIFO & UART_FIFO_TXFIFOSIZE_MASK);
    return depth;
}

uint32_t UART_RxFIFODepth(uart_type_t *base)
{
    uint32_t depth = (base->FIFO & UART_FIFO_RXFIFOSIZE_MASK);
    return depth;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetParityMode
 * Description   : Configures parity mode in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_SetParityMode(uart_type_t *base, uart_parity_mode_t parityModeType)
{
    base->CTRL = (base->CTRL & ~UART_CTRL_PE_MASK) | (((uint32_t)parityModeType >> 1U) << UART_CTRL_PE_SHIFT);
    base->CTRL = (base->CTRL & ~UART_CTRL_PT_MASK) | (((uint32_t)parityModeType & 1U) << UART_CTRL_PT_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_Putchar9
 * Description   : Sends the UART 9-bit character.
 *
 *END**************************************************************************/
void UART_Putchar9(uart_type_t *base, uint16_t data)
{
    uint8_t ninthDataBit;
    volatile uint8_t *dataRegBytes = (volatile uint8_t *)(&(base->DATA));

    ninthDataBit = (uint8_t)((data >> 8U) & 0x1U);

    /* write to ninth data bit T8(where T[0:7]=8-bits, T8=9th bit) */
    base->CTRL = (base->CTRL & ~UART_CTRL_R9T8_MASK) | ((uint32_t)(ninthDataBit) << UART_CTRL_R9T8_SHIFT);

    /* write 8-bits to the data register*/
    dataRegBytes[0] = (uint8_t)data;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_Putchar10
 * Description   : Sends the UART 10-bit character.
 *
 *END**************************************************************************/
void UART_Putchar10(uart_type_t *base, uint16_t data)
{
    uint8_t ninthDataBit, tenthDataBit;
    uint32_t ctrlRegVal;
    volatile uint8_t *dataRegBytes = (volatile uint8_t *)(&(base->DATA));

    ninthDataBit = (uint8_t)((data >> 8U) & 0x1U);
    tenthDataBit = (uint8_t)((data >> 9U) & 0x1U);

    /* write to ninth/tenth data bit (T[0:7]=8-bits, T8=9th bit, T9=10th bit) */
    ctrlRegVal = base->CTRL;
    ctrlRegVal = (ctrlRegVal & ~UART_CTRL_R9T8_MASK) | ((uint32_t)ninthDataBit << UART_CTRL_R9T8_SHIFT);
    ctrlRegVal = (ctrlRegVal & ~UART_CTRL_R8T9_MASK) | ((uint32_t)tenthDataBit << UART_CTRL_R8T9_SHIFT);
    base->CTRL = ctrlRegVal;

    /* write to 8-bits to the data register */
    dataRegBytes[0] = (uint8_t)data;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_Getchar9
 * Description   : Gets the UART 9-bit character.
 *
 *END**************************************************************************/
void UART_Getchar9(const uart_type_t *base, uint16_t *readData)
{
    OS_ASSERT(NULL != readData);

    /* get ninth bit from uart data register */
    *readData = (uint16_t)(((base->CTRL >> UART_CTRL_R8T9_SHIFT) & 1U) << 8);

    /* get 8-bit data from the uart data register */
    *readData |= (uint8_t)base->DATA;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_Getchar10
 * Description   : Gets the UART 10-bit character
 *
 *END**************************************************************************/
void UART_Getchar10(const uart_type_t *base, uint16_t *readData)
{
    OS_ASSERT(NULL != readData);

    /* read tenth data bit */
    *readData = (uint16_t)(((base->CTRL >> UART_CTRL_R9T8_SHIFT) & 1U) << 9);
    /* read ninth data bit */
    *readData |= (uint16_t)(((base->CTRL >> UART_CTRL_R8T9_SHIFT) & 1U) << 8);

    /* get 8-bit data */
    *readData |= (uint8_t)base->DATA;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetIntMode
 * Description   : Configures the UART module interrupts to enable/disable
 * various interrupt sources.
 *
 *END**************************************************************************/
void UART_SetIntMode(uart_type_t *base, uart_interrupt_t intSrc, bool enable)
{
    uint32_t reg          = (uint32_t)(intSrc) >> UART_SHIFT;
    uint32_t intRegOffset = (uint16_t)(intSrc);

    switch (reg) {
    case UART_BAUD_REG_ID:
        base->BAUD = (base->BAUD & ~(1UL << intRegOffset)) | ((enable ? 1U : 0U) << intRegOffset);
        break;
    case UART_CTRL_REG_ID:
        base->CTRL = (base->CTRL & ~(1UL << intRegOffset)) | ((enable ? 1U : 0U) << intRegOffset);
        break;
    case UART_MODIR_REG_ID:
        base->MODIR = (base->MODIR & ~(1UL << intRegOffset)) | ((enable ? 1U : 0U) << intRegOffset);
        break;
    case UART_FIFO_REG_ID:
        base->FIFO = (base->FIFO & (~UART_FIFO_REG_FLAGS_MASK & ~(1UL << intRegOffset))) |
                     ((enable ? 1U : 0U) << intRegOffset);
        break;
    default:
        /* Invalid parameter: return */
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_GetIntMode
 * Description   : Returns whether UART module interrupt is enabled/disabled.
 *
 *END**************************************************************************/
bool UART_GetIntMode(const uart_type_t *base, uart_interrupt_t intSrc)
{
    uint32_t reg = (uint32_t)(intSrc) >> UART_SHIFT;
    bool retVal  = false;

    switch (reg) {
    case UART_BAUD_REG_ID:
        retVal = (((base->BAUD >> (uint16_t)(intSrc)) & 1U) > 0U);
        break;
    case UART_CTRL_REG_ID:
        retVal = (((base->CTRL >> (uint16_t)(intSrc)) & 1U) > 0U);
        break;
    case UART_MODIR_REG_ID:
        retVal = (((base->MODIR >> (uint16_t)(intSrc)) & 1U) > 0U);
        break;
    case UART_FIFO_REG_ID:
        retVal = (((base->FIFO >> (uint16_t)(intSrc)) & 1U) > 0U);
        break;
    default:
        /* Invalid parameter: return */
        break;
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_GetStatusFlag
 * Description   : UART get status flag by passing flag enum.
 *
 *END**************************************************************************/
bool UART_GetStatusFlag(const uart_type_t *base, uart_status_flag_t statusFlag)
{
    uint32_t reg = (uint32_t)(statusFlag) >> UART_SHIFT;
    bool retVal  = false;

    switch (reg) {
    case UART_STAT_REG_ID:
        retVal = (((base->STAT >> (uint16_t)(statusFlag)) & 1U) > 0U);
        break;
    case UART_DATA_REG_ID:
        retVal = (((base->DATA >> (uint16_t)(statusFlag)) & 1U) > 0U);
        break;
    case UART_MODIR_REG_ID:
        retVal = (((base->MODIR >> (uint16_t)(statusFlag)) & 1U) > 0U);
        break;
    case UART_FIFO_REG_ID:
        retVal = (((base->FIFO >> (uint16_t)(statusFlag)) & 1U) > 0U);
        break;
    default:
        /* Invalid parameter: return */
        break;
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_ClearStatusFlag
 * Description   : UART clears an individual status flag
 * (see uart_status_flag_t for list of status bits).
 *
 *END**************************************************************************/
status_t UART_ClearStatusFlag(uart_type_t *base, uart_status_flag_t statusFlag)
{
    status_t returnCode = STATUS_SUCCESS;

    switch (statusFlag) {
    /* These flags are cleared automatically by other uart operations
     * and cannot be manually cleared, return error code */
    case UART_TX_DATA_REG_EMPTY:
    case UART_TX_COMPLETE:
    case UART_RX_DATA_REG_FULL:
    case UART_RX_ACTIVE:
    case UART_NOISE_IN_CURRENT_WORD:
    case UART_PARITY_ERR_IN_CURRENT_WORD:
        returnCode = STATUS_ERROR;
        break;

    case UART_IDLE_LINE_DETECT:
        base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | UART_STAT_IDLE_MASK;
        break;

    case UART_RX_OVERRUN:
        base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | UART_STAT_OR_MASK;
        break;

    case UART_NOISE_DETECT:
        base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | UART_STAT_NF_MASK;
        break;

    case UART_FRAME_ERR:
        base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | UART_STAT_FE_MASK;
        break;

    case UART_PARITY_ERR:
        base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | UART_STAT_PF_MASK;
        break;

    case UART_LIN_BREAK_DETECT:
        base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | UART_STAT_LBKDIF_MASK;
        break;

    case UART_RX_ACTIVE_EDGE_DETECT:
        base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | UART_STAT_RXEDGIF_MASK;
        break;

    case UART_MATCH_ADDR_ONE:
        base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | UART_STAT_MA1F_MASK;
        break;
    case UART_MATCH_ADDR_TWO:
        base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | UART_STAT_MA2F_MASK;
        break;
    case UART_FIFO_TX_OF:
        base->FIFO = (base->FIFO & (~UART_FIFO_REG_FLAGS_MASK)) | UART_FIFO_TXOF_MASK;
        break;
    case UART_FIFO_RX_UF:
        base->FIFO = (base->FIFO & (~UART_FIFO_REG_FLAGS_MASK)) | UART_FIFO_RXUF_MASK;
        break;
    default:
        returnCode = STATUS_ERROR;
        break;
    }

    return (returnCode);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetErrorInterrupts
 * Description   : Enable or disable the UART error interrupts.
 *
 *END**************************************************************************/
void UART_SetErrorInterrupts(uart_type_t *base, bool enable)
{
    /* Configure the error interrupts */
    UART_SetIntMode(base, UART_INT_RX_OVERRUN, enable);
    UART_SetIntMode(base, UART_INT_PARITY_ERR_FLAG, enable);
    UART_SetIntMode(base, UART_INT_NOISE_ERR_FLAG, enable);
    UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, enable);
}
