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
 * @file status.h
 * @brief emps status
 * @author abu (abu@eswincomputing.com)
 * @date 2023-03-24
 * 
 * Modification History :
 * Date:               Version:                    Author:     
 * Changes: 
 * 
 */

#ifndef STATUS_H
#define STATUS_H

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * status_t is referenced from all drivers.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Local macro not referenced.
 * The defined macro is used as include guard.
 *
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/** @brief Status return codes.
 * Common error codes will be a unified enumeration (C enum) that will contain all error codes
 * (common and specific). There will be separate "error values spaces" (or slots), each of 256
 * positions, allocated per functionality.
 */
typedef enum {
    /* Generic error codes */
    STATUS_SUCCESS  = 0,  /** Generic operation success status */
    STATUS_ERROR    = -1, /** Generic operation failure status */
    STATUS_TIMEOUT  = -2, /** Generic operation timeout status */
    STATUS_ERRORRES = -3, /*< Resource not available.*/
    STATUS_ERRORPAR = -4, /*< Parameter error.*/
    STATUS_ERRORNOMEM =
        -5, /*< System is out of memory: it was impOS_sible to allocate or reserve memory for the operation.*/
    STATUS_ERRORISR =
        -6,                          /*< Not allowed in ISR context: the function cannot be called from interrupt service routines.*/
    STATUS_UNSUPPORTED = -7,         /** Generic operation unsupported status */
    STATUS_BUSY        = -8,         /** Generic operation busy status */
    STATUS_ERRORCLK    = -9,         /**CLK error*/
    Status_RESERVED    = 0x7FFFFFFF, /*< Prevents enum down-size compiler optimization.*/

    /* MCU specific error codes */
    STATUS_MCU_GATED_OFF           = 0x100U, /** Module is gated off */
    STATUS_MCU_TRANSITION_FAILED   = 0x101U, /** Error occurs during transition. */
    STATUS_MCU_INVALID_STATE       = 0x102U, /** Unsupported in current state. */
    STATUS_MCU_NOTIFY_BEFORE_ERROR = 0x103U, /** Error occurs during send "BEFORE" notification. */
    STATUS_MCU_NOTIFY_AFTER_ERROR  = 0x104U, /** Error occurs during send "AFTER" notification. */
    /* I2C specific error codes */
    STATUS_I2C_RECEIVED_NACK    = 0x200U, /** NACK signal received  */
    STATUS_I2C_TX_UNDERRUN      = 0x201U, /** TX underrun error */
    STATUS_I2C_RX_OVERRUN       = 0x202U, /** RX overrun error */
    STATUS_I2C_ARBITRATION_LOST = 0x203U, /** Arbitration lost */
    STATUS_I2C_ABORTED          = 0x204U, /** A transfer was aborted */
    STATUS_I2C_BUS_BUSY         = 0x205U, /** I2C bus is busy, cannot start transfer */
    /* CAN specific error codes */
    STATUS_CAN_BUFF_OUT_OF_RANGE       = 0x300U, /** The specified MB index is out of the configurable range */
    STATUS_CAN_NO_TRANSFER_IN_PROGRESS = 0x301U, /** There is no transmission or reception in progress */
    FLEXCAN_STATUS_BUSY                = 0x302U, /** Busy Operation Completed */
    FLEXCAN_STATUS_TIMEOUT             = 0x303U, /** TimeOut Operation Completed */
    /* Security specific error codes */
    STATUS_SEC_SEQUENCE_ERROR            = 0x402U, /** The sequence of commands or subcommands is out of
                                                         sequence */
    STATUS_SEC_KEY_NOT_AVAILABLE         = 0x403U, /** A key is locked due to failed boot measurement or
                                                         an active debugger */
    STATUS_SEC_KEY_INVALID               = 0x404U, /** A function is called to perform an operation with
                                                         a key that is not allowed for the given operation */
    STATUS_SEC_KEY_EMPTY                 = 0x405U, /** Attempt to use a key that has not been initialized yet */
    STATUS_SEC_NO_SECURE_BOOT            = 0x406U, /** The conditions for a secure boot process are not met */
    STATUS_SEC_KEY_WRITE_PROTECTED       = 0x407U, /** Request for updating a write protected key slot,
                                                         or activating debugger with write protected key(s) */
    STATUS_SEC_KEY_UPDATE_ERROR          = 0x408U, /** Key update did not succeed due to errors in
                                                         verification of the messages */
    STATUS_SEC_RNG_SEED                  = 0x409U, /** Returned by CMD_RND and CMD_DEBUG if the seed has not
                                                         been initialized before */
    STATUS_SEC_NO_DEBUGGING              = 0x40AU, /** DEBUG command authentication failed */
    STATUS_SEC_MEMORY_FAILURE            = 0x40CU, /** General memory technology failure
                                                         (multibit ECC error, common fault detected) */
    STATUS_SEC_HSM_INTERNAL_MEMORY_ERROR = 0x410U, /** An internal memory error encountered while
                                                        executing the command */
    STATUS_SEC_INVALID_COMMAND           = 0x411U, /** Command value out of range */
    STATUS_SEC_TRNG_ERROR                = 0x412U, /** One or more statistical tests run on the TRNG output failed */
    STATUS_SEC_HSM_FLASH_BLOCK_ERROR     = 0x413U, /** Error reading, programming or erasing one of the HSM flash blocks */
    STATUS_SEC_INTERNAL_CMD_ERROR        = 0x414U, /** An internal command processor error while executing a command */
    STATUS_SEC_MAC_LENGTH_ERROR          = 0x415U, /** MAC/Message length out of range */
    STATUS_SEC_INVALID_ARG               = 0x421U, /** Invalid command argument */
    STATUS_SEC_TRNG_CLOCK_ERROR          = 0x423U, /** TRNG not provided with a stable clock */
    /* SPI specific error codes */
    STATUS_SPI_TX_UNDERRUN = 0x500U, /** TX underrun error */
    STATUS_SPI_RX_OVERRUN  = 0x501U, /** RX overrun error */
    STATUS_SPI_ABORTED     = 0x502U, /** A transfer was aborted */
    /* UART specific error codes */
    STATUS_UART_TX_UNDERRUN   = 0x600U, /** TX underrun error */
    STATUS_UART_RX_OVERRUN    = 0x601U, /** RX overrun error */
    STATUS_UART_ABORTED       = 0x602U, /** A transfer was aborted */
    STATUS_UART_FRAMING_ERROR = 0x603U, /** Framing error */
    STATUS_UART_PARITY_ERROR  = 0x604U, /** Parity error */
    STATUS_UART_NOISE_ERROR   = 0x605U, /** Noise error */

    /* FLASH specific error codes */
    STATUS_FLASH_ERROR_ENABLE   = 0x701U, /** It's impossible to enable an operation */
    STATUS_FLASH_ERROR_NO_BLOCK = 0x702U, /** No blocks have been enabled for Array Integrity check */
    STATUS_FLASH_INPROGRESS     = 0x703U, /** InProgress status */
    STATUS_FLASH_OUT_OF_RANGE   = 0x704U, /** Flash address out of range status */
    STATUS_FLASH_PROFRAM_FAIL   = 0x705U, /** Flash program fail status */
    STATUS_FLASH_ERASE_FAIL     = 0x706U, /** Flash erase fail status */
    STATUS_FLASH_READ_FAIL      = 0x707U, /** Flash read fail status */

    /* I2S specific error codes */
    STATUS_I2S_TX_UNDERRUN = 0x800U, /** TX underrun error */
    STATUS_I2S_RX_OVERRUN  = 0x801U, /** RX overrun error */
    STATUS_I2S_ABORTED     = 0x802U, /** A transfer was aborted */
} status_t;

#endif /* STATUS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
