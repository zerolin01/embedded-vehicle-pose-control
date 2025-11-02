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
 * @file uart_hw_access.h
 * @brief UART driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __UART_HW_ACCESS_H__
#define __UART_HW_ACCESS_H__

#include "uart_driver.h"
#include "uart_register.h"

#define UART_SHIFT        (16U)
#define UART_BAUD_REG_ID  (1U)
#define UART_STAT_REG_ID  (2U)
#define UART_CTRL_REG_ID  (3U)
#define UART_DATA_REG_ID  (4U)
#define UART_MATCH_REG_ID (5U)
#define UART_MODIR_REG_ID (6U)
#define UART_FIFO_REG_ID  (7U)

/**
 *  @brief UART wakeup from standby method constants
 */
typedef enum {
    UART_IDLE_LINE_WAKE = 0x0U, /**< Idle-line wakes the UART receiver from standby. */
    UART_ADDR_MARK_WAKE = 0x1U  /**< Addr-mark wakes UART receiver from standby.*/
} uart_wakeup_method_t;

/**
 * @brief UART break character length settings for transmit/detect.
 *
 * The actual maximum bit times may vary depending on the UART instance.
 */
typedef enum {
    UART_BREAK_CHAR_10_BIT_MINIMUM = 0x0U, /**< UART break char length 10 bit times (if M = 0, SBNS = 0)
                                                  or 11 (if M = 1, SBNS = 0 or M = 0, SBNS = 1) or 12 (if M = 1,
                                                  SBNS = 1 or M10 = 1, SNBS = 0) or 13 (if M10 = 1, SNBS = 1) */
    UART_BREAK_CHAR_13_BIT_MINIMUM = 0x1U  /**< UART break char length 13 bit times (if M = 0, SBNS = 0
                                                  or M10 = 0, SBNS = 1) or 14 (if M = 1, SBNS = 0 or M = 1,
                                                  SBNS = 1) or 15 (if M10 = 1, SBNS = 1 or M10 = 1, SNBS = 0) */
} uart_break_char_length_t;

/**
 * @brief UART status flags.
 *
 * This provides constants for the UART status flags for use in the UART functions.
 */
typedef enum {
    UART_TX_DATA_REG_EMPTY          = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_TDRE_SHIFT),    /**< Tx data register empty flag, sets when Tx buffer is empty */
    UART_TX_COMPLETE                = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_TC_SHIFT),      /**< Transmission complete flag, sets when transmission activity complete */
    UART_RX_DATA_REG_FULL           = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_RDRF_SHIFT),    /**< Rx data register full flag, sets when the receive data buffer is full */
    UART_IDLE_LINE_DETECT           = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_IDLE_SHIFT),    /**< Idle line detect flag, sets when idle line detected */
    UART_RX_OVERRUN                 = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_OR_SHIFT),      /**< Rx Overrun sets if new data is received before data is read */
    UART_NOISE_DETECT               = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_NF_SHIFT),      /**< Rx takes 3 samples of each received bit. If these differ, the flag sets */
    UART_FRAME_ERR                  = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_FE_SHIFT),      /**< Frame error flag, sets if logic 0 was detected where stop bit expected */
    UART_PARITY_ERR                 = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_PF_SHIFT),      /**< If parity enabled, sets upon parity error detection */
    UART_LIN_BREAK_DETECT           = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_LBKDIF_SHIFT),  /**< LIN break detect interrupt flag, sets when LIN break char detected */
    UART_RX_ACTIVE_EDGE_DETECT      = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_RXEDGIF_SHIFT), /**< Rx pin active edge interrupt flag, sets when active edge detected */
    UART_RX_ACTIVE                  = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_RAF_SHIFT),     /**< Receiver Active Flag (RAF), sets at beginning of valid start bit */
    UART_NOISE_IN_CURRENT_WORD      = (((uint32_t)UART_DATA_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_DATA_NOISY_SHIFT),   /**< NOISY bit, sets if noise detected in current data word */
    UART_PARITY_ERR_IN_CURRENT_WORD = (((uint32_t)UART_DATA_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_DATA_PARITYE_SHIFT), /**< PARITYE bit, sets if noise detected in current data word */
    UART_MATCH_ADDR_ONE             = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_MA1F_SHIFT),    /**< Address one match flag */
    UART_MATCH_ADDR_TWO             = (((uint32_t)UART_STAT_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_STAT_MA2F_SHIFT),    /**< Address two match flag */
    UART_FIFO_TX_OF                 = (((uint32_t)UART_FIFO_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_FIFO_TXOF_SHIFT),    /**< Transmitter FIFO buffer overflow */
    UART_FIFO_RX_UF                 = (((uint32_t)UART_FIFO_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_FIFO_RXUF_SHIFT)     /**< Receiver FIFO buffer underflow */
} uart_status_flag_t;

/**
 *  @brief UART interrupt configuration structure, default settings are 0 (disabled)
 */
typedef enum {
    UART_INT_LIN_BREAK_DETECT  = (((uint32_t)UART_BAUD_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_BAUD_LBKDIE_SHIFT),  /**< LIN break detect. */
    UART_INT_RX_ACTIVE_EDGE    = (((uint32_t)UART_BAUD_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_BAUD_RXEDGIE_SHIFT), /**< RX Active Edge. */
    UART_INT_TX_DATA_REG_EMPTY = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_TIE_SHIFT),     /**< Transmit data register empty. */
    UART_INT_TX_COMPLETE       = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_TCIE_SHIFT),    /**< Transmission complete. */
    UART_INT_RX_DATA_REG_FULL  = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_RIE_SHIFT),     /**< Receiver data register full. */
    UART_INT_IDLE_LINE         = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_ILIE_SHIFT),    /**< Idle line. */
    UART_INT_RX_OVERRUN        = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_ORIE_SHIFT),    /**< Receiver Overrun. */
    UART_INT_NOISE_ERR_FLAG    = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_NEIE_SHIFT),    /**< Noise error flag. */
    UART_INT_FRAME_ERR_FLAG    = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_FEIE_SHIFT),    /**< Framing error flag. */
    UART_INT_PARITY_ERR_FLAG   = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_PEIE_SHIFT),    /**< Parity error flag. */
    UART_INT_MATCH_ADDR_ONE    = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_MA1IE_SHIFT),   /**< Match address one flag. */
    UART_INT_MATCH_ADDR_TWO    = (((uint32_t)UART_CTRL_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_CTRL_MA2IE_SHIFT),   /**< Match address two flag. */
    UART_INT_FIFO_TXOF         = (((uint32_t)UART_FIFO_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_FIFO_TXOFE_SHIFT),   /**< Transmitter FIFO buffer interrupt */
    UART_INT_FIFO_RXUF         = (((uint32_t)UART_FIFO_REG_ID << (uint32_t)UART_SHIFT) | (uint32_t)UART_FIFO_RXUFE_SHIFT)    /**< Receiver FIFO buffer interrupt */
} uart_interrupt_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initializes the UART controller.
 *
 * This function Initializes the UART controller to known state.
 *
 * @param base UART base pointer.
 */
void UART_Init(uart_type_t *base);

/**
 * @brief Get the UART base pointer
 *
 * This function return the base pointer of uart
 *
 * @param base UART instance.
 * @return UART base pointer
 */
uart_type_t *UART_GetBasePointer(uint32_t instance);

/**
 * @brief Initializes the UART fifo water
 *
 * This function Initializes the UART fifo water
 *
 * @param base UART base pointer.
 * @param water UART watermark
 */
void UART_FIFOWater_Init(uart_type_t *base, uint16_t water);

/**
 * @brief Disables the UART fifo.
 *
 * This function Disables the UART fifo water.
 *
 * @param base UART base pointer.
 */
void UART_FIFO_Disable(uart_type_t *base);

/**
 * @brief Enable/Disable the UART transmitter.
 *
 * This function enables or disables the UART transmitter, based on the
 * parameter received.
 *
 * @param base UART base pointer.
 * @param enable Enable(true) or disable(false) transmitter.
 */
static inline void UART_SetTransmitterCmd(uart_type_t *base, bool enable)
{
    base->CTRL = (base->CTRL & ~UART_CTRL_TE_MASK) | ((enable ? 1UL : 0UL) << UART_CTRL_TE_SHIFT);
    /* Wait for the register write operation to complete */
    while ((bool)((base->CTRL & UART_CTRL_TE_MASK) != 0U) != enable) {
        base->CTRL = (base->CTRL & ~UART_CTRL_TE_MASK) | ((enable ? 1UL : 0UL) << UART_CTRL_TE_SHIFT);
    }
}

/**
 * @brief Enable/Disable the UART receiver.
 *
 * This function enables or disables the UART receiver, based on the
 * parameter received.
 *
 * @param base UART base pointer
 * @param enable Enable(true) or disable(false) receiver.
 */
static inline void UART_SetReceiverCmd(uart_type_t *base, bool enable)
{
    base->CTRL = (base->CTRL & ~UART_CTRL_RE_MASK) | ((enable ? 1UL : 0UL) << UART_CTRL_RE_SHIFT);
    /* Wait for the register write operation to complete */
    while ((bool)((base->CTRL & UART_CTRL_RE_MASK) != 0U) != enable) {
    }
}

/**
 * @brief Sets the UART baud rate modulo divisor.
 *
 * This function sets the UART baud rate modulo divisor.
 *
 * @param base UART base pointer.
 * @param baudRateDivisor The baud rate modulo division "SBR"
 */
static inline void UART_SetBaudRateDivisor(uart_type_t *base, uint32_t baudRateDivisor)
{
    OS_ASSERT((baudRateDivisor <= 0x1FFFU) && (baudRateDivisor >= 1U));
    uint32_t baudRegValTemp;

    baudRegValTemp = base->BAUD;
    baudRegValTemp &= ~(UART_BAUD_SBR_MASK);
    /* Removed the shift operation as the SBR field position is zero; shifting with 0 violates MISRA */
    baudRegValTemp |= baudRateDivisor & UART_BAUD_SBR_MASK;
    base->BAUD = baudRegValTemp;
}

/**
 * @brief set loopback mode
 *
 * @param base UART base pointer.
 * @param en enable or disable loopback mode
 */
void UART_SetLoopback(uart_type_t *base, bool en);

/**
 * @brief Gets the UART baud rate modulo divisor.
 *
 * This function gets the UART baud rate modulo divisor.
 *
 * @param base UART base pointer.
 * @return The baud rate modulo division "SBR"
 */
static inline uint16_t UART_GetBaudRateDivisor(const uart_type_t *base)
{
    return ((uint16_t)((base->BAUD & UART_BAUD_SBR_MASK) >> UART_BAUD_SBR_SHIFT));
}

/**
 * @brief Sets the UART baud rate oversampling ratio
 *
 * This function sets the UART baud rate oversampling ratio.
 * (Note: Feature available on select UART instances used together with baud rate programming)
 * The oversampling ratio should be set between 4x (00011) and 32x (11111). Writing
 * an invalid oversampling ratio results in an error and is set to a default
 * 16x (01111) oversampling ratio.
 * Disable the transmitter/receiver before calling this function.
 *
 * @param base UART base pointer.
 * @param overSamplingRatio The oversampling ratio "OSR"
 */
static inline void UART_SetOversamplingRatio(uart_type_t *base, uint32_t overSamplingRatio)
{
    OS_ASSERT(overSamplingRatio <= 0x1FU);
    uint32_t baudRegValTemp;

    baudRegValTemp = base->BAUD;
    baudRegValTemp &= ~(UART_BAUD_OSR_MASK);
    baudRegValTemp |= UART_BAUD_OSR(overSamplingRatio);
    base->BAUD = baudRegValTemp;
}

/**
 * @brief Gets the UART baud rate oversampling ratio
 *
 * This function gets the UART baud rate oversampling ratio.
 * (Note: Feature available on select UART instances used together with baud rate programming)
 *
 * @param base UART base pointer.
 * @return The oversampling ratio "OSR"
 */
static inline uint8_t UART_GetOversamplingRatio(const uart_type_t *base)
{
    return ((uint8_t)((base->BAUD & UART_BAUD_OSR_MASK) >> UART_BAUD_OSR_SHIFT));
}

/**
 * @brief Configures the UART baud rate both edge sampling
 *
 * This function configures the UART baud rate both edge sampling.
 * (Note: Feature available on select UART instances used with baud rate programming)
 * When enabled, the received data is sampled on both edges of the baud rate clock.
 * This must be set when the oversampling ratio is between 4x and 7x.
 * This function should only be called when the receiver is disabled.
 *
 * @param base UART base pointer.
 * @param enable   Enable (1) or Disable (0) Both Edge Sampling
 */
static inline void UART_EnableBothEdgeSamplingCmd(uart_type_t *base)
{
    base->BAUD |= UART_BAUD_BOTHEDGE_MASK;
}

/**
 * @brief Configures the number of bits per character in the UART controller.
 *
 * This function configures the number of bits per character in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param bitCountPerChar  Number of bits per char (8, 9, or 10, depending on the UART instance)
 * @param parity  Specifies whether parity bit is enabled
 */
void UART_SetBitCountPerChar(uart_type_t *base, uart_bit_count_per_char_t bitCountPerChar, bool parity);

/**
 * @brief Configures MSB or LSB first mode in the UART controller.
 *
 * This function configures MSB or LSB first mode in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param msbModeType  MSB or LSB first mode(LSB or MSB first)
 */
void UART_MSBFMode(uart_type_t *base, uart_msbf_mode_t msbModeType);

/**
 * @brief Configures transmit data inverse in the UART controller.
 *
 * This function configures transmit data inverse in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param invModeType  inverse mode(inverse the transmit data or not)
 */
void UART_SetTxInv(uart_type_t *base, uart_tx_rx_inve_t invModeType);

/**
 * @brief Configures receive data inverse in the UART controller.
 *
 * This function configures receive data inverse in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param invModeType  inverse mode(inverse the receive data or not)
 */
void UART_SetRxInv(uart_type_t *base, uart_tx_rx_inve_t invModeType);

/**
 * @brief Enables the idle line interrupt
 *
 * This function enables the idle line interrupt.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param uart_idle_interrupt_type_t  Enables or disable the idle interrupt. 
 */
void UART_SetIdleIREnable(uart_type_t *base, uart_idle_interrupt_type_t idleIRType);

/**
 * @brief Configures the number of idle characters.
 *
 * This function Configures the number of idle characters that must be received 
 * before the IDLE flag is set.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param uart_character_type_t  idle character length type.
 */
void UART_SetIdleCharsLen(uart_type_t *base, uart_character_type_t idleCharsType);

/**
 * @brief Configures the idle line type.
 *
 * This function determines when the receiver starts counting logic 1s as idle 
 * character bits. The count begins either after a valid start bit or after the 
 * stop bit.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param idleLineType  idle line type.
 */
void UART_SetIdleLineType(uart_type_t *base, uart_idle_line_type_t idleLineType);

/**
 * @brief Returns the transmit Buffer/FIFO empty or not.
 *
 * This function asserts when there is no data in the Transmit FIFO/buffer.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 */
bool UART_IsTxFIFOEmpty(uart_type_t *base);

/**
 * @brief Returns the transmit Buffer/FIFO empty or not.
 *
 * If the function returns 1, it indicates that there is an active edge on the RXD pin. 
 * If it is 0, it indicates that there is no active edge on the RXD pin. 
 *
 * @param base UART base pointer.
 */
bool UART_IsRxActiveEage(uart_type_t *base);

uint32_t UART_TxFIFODepth(uart_type_t *base);

uint32_t UART_RxFIFODepth(uart_type_t *base);

/**
 * @brief Return whether there is a receiver overrun event.
 *
 * This function returns whether there is a receiver overrun event.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 */
bool UART_IsRxOverrun(uart_type_t *base);

/**
 * @brief Overrun Interrupt Enable or not;
 *
 * This bit enables the overrun flag (OR) to generate hardware interrupt requests.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param OverrunENType  Enable or disanables the overrun error interrupt.
 */
void UART_OverrunIREnable(uart_type_t *base, uart_overrrun_interrupt_type_t OverrunENType);

/**
 * @brief Configures parity mode in the UART controller.
 *
 * This function configures parity mode in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param parityModeType  Parity mode (enabled, disable, odd, even - see parity_mode_t struct)
 */
void UART_SetParityMode(uart_type_t *base, uart_parity_mode_t parityModeType);

/**
 * @brief Configures the number of stop bits in the UART controller.
 *
 * This function configures the number of stop bits in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param base UART base pointer.
 * @param stopBitCount Number of stop bits (1 or 2 - see uart_stop_bit_count_t struct)
 */
static inline void UART_SetStopBitCount(uart_type_t *base, uart_stop_bit_count_t stopBitCount)
{
    base->BAUD = (base->BAUD & ~UART_BAUD_SBNS_MASK) | ((uint32_t)stopBitCount << UART_BAUD_SBNS_SHIFT);
}

/**
 * @brief Configures the UART module interrupts.
 *
 * This function configures the UART module interrupts to enable/disable various interrupt sources.
 *
 * @param   base UART module base pointer.
 * @param   intSrc UART interrupt configuration data.
 * @param   enable   true: enable, false: disable.
 */
void UART_SetIntMode(uart_type_t *base, uart_interrupt_t intSrc, bool enable);

/**
 * @brief Returns UART module interrupts state.
 *
 * This function returns whether a certain UART module interrupt is enabled or disabled.
 *
 *
 * @param   base UART module base pointer.
 * @param   intSrc UART interrupt configuration data.
 * @return  true: enable, false: disable.
 */
bool UART_GetIntMode(const uart_type_t *base, uart_interrupt_t intSrc);

/**
 * @brief Configures PDMA requests.
 *
 * This function configures PDMA requests for UART Transmitter.
 *
 * @param base UART base pointer
 * @param enable Transmit PDMA request configuration (enable:1 /disable: 0)
 */
static inline void UART_SetTxDmaCmd(uart_type_t *base, bool enable)
{
    base->BAUD = (base->BAUD & ~UART_BAUD_TDMAE_MASK) | ((enable ? 1UL : 0UL) << UART_BAUD_TDMAE_SHIFT);
}

/**
 * @brief Configures PDMA requests.
 *
 * This function configures PDMA requests for UART Receiver.
 *
 *
 * @param base UART base pointer
 * @param enable Receive PDMA request configuration (enable: 1/disable: 0)
 */
static inline void UART_SetRxDmaCmd(uart_type_t *base, bool enable)
{
    base->BAUD = (base->BAUD & ~UART_BAUD_RDMAE_MASK) | ((enable ? 1UL : 0UL) << UART_BAUD_RDMAE_SHIFT);
}

/**
 * @brief Sends the UART 8-bit character.
 *
 * This functions sends an 8-bit character.
 *
 * @param base UART Instance
 * @param data     data to send (8-bit)
 */
static inline void UART_Putchar(uart_type_t *base, uint8_t data)
{
    volatile uint8_t *dataRegBytes = (volatile uint8_t *)(&(base->DATA));
    dataRegBytes[0]                = data;
}

/**
 * @brief Sends the UART 9-bit character.
 *
 * This functions sends a 9-bit character.
 *
 * @param base UART Instance
 * @param data     data to send (9-bit)
 */
void UART_Putchar9(uart_type_t *base, uint16_t data);

/**
 * @brief Sends the UART 10-bit character (Note: Feature available on select UART instances).
 *
 * This functions sends a 10-bit character.
 *
 * @param base UART Instance
 * @param data   data to send (10-bit)
 */
void UART_Putchar10(uart_type_t *base, uint16_t data);

/**
 * @brief Gets the UART 8-bit character.
 *
 * This functions receives an 8-bit character.
 *
 * @param base UART base pointer
 * @param readData Data read from receive (8-bit)
 */
static inline void UART_Getchar(const uart_type_t *base, uint8_t *readData)
{
    OS_ASSERT(readData != NULL);

    *readData = (uint8_t)base->DATA;
}

/**
 * @brief Gets the UART 9-bit character.
 *
 * This functions receives a 9-bit character.
 *
 * @param base UART base pointer
 * @param readData Data read from receive (9-bit)
 */
void UART_Getchar9(const uart_type_t *base, uint16_t *readData);

/**
 * @brief Gets the UART 10-bit character.
 *
 * This functions receives a 10-bit character.
 *
 * @param base UART base pointer
 * @param readData Data read from receive (10-bit)
 */
void UART_Getchar10(const uart_type_t *base, uint16_t *readData);

/**
 * @brief  UART get status flag
 *
 * This function returns the state of a status flag.
 *
 * @param base UART base pointer
 * @param statusFlag  The status flag to query
 * @return Whether the current status flag is set(true) or not(false).
 */
bool UART_GetStatusFlag(const uart_type_t *base, uart_status_flag_t statusFlag);

/**
 * @brief UART clears an individual status flag.
 *
 * This function clears an individual status flag (see uart_status_flag_t for list of status bits).
 *
 * @param base UART base pointer
 * @param statusFlag  Desired UART status flag to clear
 * @return STATUS_SUCCESS if successful or STATUS_ERROR if an error occurred
 */
status_t UART_ClearStatusFlag(uart_type_t *base, uart_status_flag_t statusFlag);

/**
 * @brief Enable or disable the UART error interrupts.
 *
 * This function configures the error interrupts (parity, noise, overrun, framing).
 *
 * @param base UART base pointer
 * @param enable true - enable, false - disable error interrupts
 */
void UART_SetErrorInterrupts(uart_type_t *base, bool enable);

/**
 * @brief  Clears the error flags treated by the driver
 *
 * This function clears the error flags treated by the driver.
 *
 * @param base UART base pointer
 */
static inline void UART_DRV_ClearErrorFlags(uart_type_t *base)
{
    uint32_t mask = UART_STAT_OR_MASK | UART_STAT_NF_MASK | UART_STAT_FE_MASK | UART_STAT_PF_MASK;

    base->STAT = (base->STAT & (~UART_STAT_REG_FLAGS_MASK)) | mask;
}

#if defined(__cplusplus)
}
#endif

#endif // __UART_HW_ACCESS_H__
