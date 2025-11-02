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
 * @file i2c_hw_access.h
 * @brief I2C driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#ifndef __I2C_HW_ACCESS_H__
#define __I2C_HW_ACCESS_H__

#include "emps_platform.h"
#include "i2c_driver.h"
#include "i2c_register.h"
#include <stdbool.h>

/** I2C master interrupts. */
#define I2C_MASTER_DATA_MATCH_INT       0x4000U /**< Data Match Interrupt.       */
#define I2C_MASTER_PIN_LOW_TIMEOUT_INT  0x2000U /**< Pin Low Timeout Interrupt.  */
#define I2C_MASTER_FIFO_ERROR_INT       0x1000U /**< FIFO Error Interrupt .      */
#define I2C_MASTER_ARBITRATION_LOST_INT 0x800U  /**< Arbitration Lost Interrupt. */
#define I2C_MASTER_NACK_DETECT_INT      0x400U  /**< NACK Detect Interrupt.      */
#define I2C_MASTER_STOP_DETECT_INT      0x200U  /**< STOP Detect Interrupt.      */
#define I2C_MASTER_END_PACKET_INT       0x100U  /**< End Packet Interrupt.       */
#define I2C_MASTER_RECEIVE_DATA_INT     0x2U    /**< Receive Data Interrupt.     */
#define I2C_MASTER_TRANSMIT_DATA_INT    0x1U    /**< Transmit Data Interrupt.    */

/** I2C slave interrupts. */
#define I2C_SLAVE_SMBUS_ALERT_RESPONSE_INT 0x8000U /**< SMBus Alert Response Interrupt. */
#define I2C_SLAVE_GENERAL_CALL_INT         0x4000U /**< General Call Interrupt.         */
#define I2C_SLAVE_ADDRESS_MATCH_1_INT      0x2000U /**< Address Match 1 Interrupt.      */
#define I2C_SLAVE_ADDRESS_MATCH_0_INT      0x1000U /**< Address Match 0 Interrupt.      */
#define I2C_SLAVE_FIFO_ERROR_INT           0x800U  /**< FIFO Error Interrupt.           */
#define I2C_SLAVE_BIT_ERROR_INT            0x400U  /**< Bit Error Interrupt.            */
#define I2C_SLAVE_STOP_DETECT_INT          0x200U  /**< STOP Detect Interrupt.          */
#define I2C_SLAVE_REPEATED_START_INT       0x100U  /**< Repeated Start Interrupt.       */
#define I2C_SLAVE_TRANSMIT_ACK_INT         0x8U    /**< Transmit ACK Interrupt.         */
#define I2C_SLAVE_ADDRESS_VALID_INT        0x4U    /**< Address Valid Interrupt.        */
#define I2C_SLAVE_RECEIVE_DATA_INT         0x2U    /**< Receive Data Interrupt.         */
#define I2C_SLAVE_TRANSMIT_DATA_INT        0x1U    /**< Transmit Data Interrupt.        */

/** Pin configuration selection. */
typedef enum {
    I2C_CFG_2PIN_OPEN_DRAIN         = 0U, /**< 2-pin open drain mode. */
    I2C_CFG_2PIN_OUTPUT_ONLY        = 1U, /**< 2-pin output only mode (ultra-fast mode). */
    I2C_CFG_2PIN_PUSH_PULL          = 2U, /**< 2-pin push-pull mode. */
    I2C_CFG_4PIN_PUSH_PULL          = 3U, /**< 4-pin push-pull mode. */
    I2C_CFG_2PIN_OPEN_DRAIN_SLAVE   = 4U, /**< 2-pin open drain mode with separate I2C slave. */
    I2C_CFG_2PIN_OUTPUT_ONLY_SLAVE  = 5U, /**< 2-pin output only mode (ultra-fast mode) with separate I2C slave. */
    I2C_CFG_2PIN_PUSH_PULL_SLAVE    = 6U, /**< 2-pin push-pull mode with separate I2C slave. */
    I2C_CFG_4PIN_PUSH_PULL_INVERTED = 7U, /**< 4-pin push-pull mode (inverted outputs). */
} i2c_pin_config_t;

/** Master NACK reaction configuration. */
typedef enum {
    I2C_NACK_RECEIVE = 0U, /**< Receive ACK and NACK normally. */
    I2C_NACK_IGNORE  = 1U, /**< Treat a received NACK as if it was an ACK. */
} i2c_nack_config_t;

/** I2C master prescaler options. */
typedef enum {
    I2C_MASTER_PRESC_DIV_1   = 0U, /**< Divide by 1.   */
    I2C_MASTER_PRESC_DIV_2   = 1U, /**< Divide by 2.   */
    I2C_MASTER_PRESC_DIV_4   = 2U, /**< Divide by 4.   */
    I2C_MASTER_PRESC_DIV_8   = 3U, /**< Divide by 8.   */
    I2C_MASTER_PRESC_DIV_16  = 4U, /**< Divide by 16.  */
    I2C_MASTER_PRESC_DIV_32  = 5U, /**< Divide by 32.  */
    I2C_MASTER_PRESC_DIV_64  = 6U, /**< Divide by 64.  */
    I2C_MASTER_PRESC_DIV_128 = 7U, /**< Divide by 128. */
} i2c_master_prescaler_t;

/** Slave address configuration. */
typedef enum {
    I2C_SLAVE_ADDR_MATCH_0_7BIT             = 0U, /**< Address match 0 (7-bit). */
    I2C_SLAVE_ADDR_MATCH_0_10BIT            = 1U, /**< Address match 0 (10-bit). */
    I2C_SLAVE_ADDR_MATCH_0_7BIT_OR_1_7BIT   = 2U, /**< Address match 0 (7-bit) or Address match 1 (7-bit). */
    I2C_SLAVE_ADDR_MATCH_0_10BIT_OR_1_10BIT = 3U, /**< Address match 0 (10-bit) or Address match 1 (10-bit). */
    I2C_SLAVE_ADDR_MATCH_0_7BIT_OR_1_10BIT  = 4U, /**< Address match 0 (7-bit) or Address match 1 (10-bit). */
    I2C_SLAVE_ADDR_MATCH_0_10BIT_OR_1_7BIT  = 5U, /**< Address match 0 (10-bit) or Address match 1 (7-bit). */
    I2C_SLAVE_ADDR_MATCH_RANGE_7BIT         = 6U, /**< From Address match 0 (7-bit) to Address match 1 (7-bit). */
    I2C_SLAVE_ADDR_MATCH_RANGE_10BIT        = 7U, /**< From Address match 0 (10-bit) to Address match 1 (10-bit). */
} i2c_slave_addr_config_t;

/** Slave NACK reaction configuration. */
typedef enum {
    I2C_SLAVE_NACK_END_TRANSFER      = 0U, /**< Slave will end transfer when NACK detected. */
    I2C_SLAVE_NACK_CONTINUE_TRANSFER = 1U, /**< Slave will not end transfer when NACK detected. */
} i2c_slave_nack_config_t;

/** Slave ACK transmission options. */
typedef enum {
    I2C_SLAVE_TRANSMIT_ACK  = 0U, /**< Transmit ACK for received word.  */
    I2C_SLAVE_TRANSMIT_NACK = 1U, /**< Transmit NACK for received word. */
} i2c_slave_nack_transmit_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Get the size of the Master Receive FIFO.
 *
 * This function returns the size of the Master Receive FIFO, always a power of 2.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint16_t Master Receive FIFO Size.
 */
static inline uint16_t I2C_GetMasterRxFIFOSize(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->PARAM;
    tmp          = (tmp & I2C_PARAM_MRXFIFO_MASK) >> I2C_PARAM_MRXFIFO_SHIFT;
    tmp          = 1UL << tmp; /**< RX FIFO size = 2^MRXFIFO */
    return (uint16_t)tmp;
}

/**
 * @brief Get the size of the Master Transmit FIFO.
 *
 * This function returns the size of the Master Transmit FIFO, always a power of 2.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint16_t Master Transmit FIFO Size.
 */
static inline uint16_t I2C_GetMasterTxFIFOSize(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->PARAM;
    tmp          = (tmp & I2C_PARAM_MTXFIFO_MASK) >> I2C_PARAM_MTXFIFO_SHIFT;
    tmp          = 1UL << tmp; /**< TX FIFO size = 2^MTXFIFO */
    return (uint16_t)tmp;
}

/**
 * @brief Reset the master receive FIFO.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ResetMasterRxFIFOCmd(i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~(I2C_MCR_RRF_MASK));
    regValue |= I2C_MCR_RRF(1U);
    baseAddr->MCR = (uint32_t)regValue;
}

/**
 * @brief Reset the master transmit FIFO.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ResetMasterTxFIFOCmd(i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~(I2C_MCR_RTF_MASK));
    regValue |= I2C_MCR_RTF(1U);
    baseAddr->MCR = (uint32_t)regValue;
}

/**
 * @brief Set/clear the master reset command.
 *
 * Calling this function with enable parameter set to true resets all internal
 * master logic and registers, except the Master Control Register. The reset state
 * persists until this function is called with enable parameter set to false.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Specifies the reset state of the I2C master logic.
 */
static inline void I2C_SetMasterSoftwareReset(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~(I2C_MCR_RST_MASK));
    regValue |= I2C_MCR_RST(enable);
    baseAddr->MCR = (uint32_t)regValue;
}

/**
 * @brief Enable or disable the I2C master.
 *
 * This function enables or disables the I2C module in master mode. If the module
 * is enabled, the transmit FIFO  is not empty and the I2C bus is idle, then
 * the I2C master will immediately initiate a transfer on the I2C bus.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Specifies whether to enable or disable the I2C master.
 */
static inline void I2C_SetMasterEnable(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~(I2C_MCR_MEN_MASK));
    regValue |= I2C_MCR_MEN(enable);
    baseAddr->MCR = (uint32_t)regValue;
}

/**
 * @brief Indicate the availability of receive data.
 *
 * This function returns true when the number of words in the receive FIFO is greater
 * than the receive FIFO watermark. See function I2C_MasterSetRxFIFOWatermark()
 * for configuring the receive FIFO watermark.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Receive data is ready.
 * @return false: Receive Data is not ready.
 */
static inline bool I2C_GetMasterReceiveDataReadyEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue          = (regValue & I2C_MSR_RDF_MASK) >> I2C_MSR_RDF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Indicate if the I2C master requests more data.
 *
 * This function returns true when the number of words in the transmit FIFO is equal
 * or less than the transmit FIFO watermark. See function I2C_SetMasterTxFIFOWatermark()
 * for configuring the transmit FIFO watermark.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Transmit data is requested.
 * @return false: Transmit data is not requested.
 */
static inline bool I2C_GetMasterTransmitDataRequestEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue          = (regValue & I2C_MSR_TDF_MASK) >> I2C_MSR_TDF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Check the occurrence of a FIFO error event.
 *
 * This function returns true if the I2C master detects an attempt to send or
 * receive data without first generating a (repeated) START condition. This can
 * occur if the transmit FIFO underflows when the AUTOSTOP bit is set. When this
 * flag is set, the I2C master will send a STOP condition (if busy) and will
 * not initiate a new START condition until this flag has been cleared.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Master sending or receiving data without a START condition.
 * @return false: No error.
 */
static inline bool I2C_GetMasterFIFOErrorEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue          = (regValue & I2C_MSR_FEF_MASK) >> I2C_MSR_FEF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Check the occurrence of an arbitration lost event.
 *
 * This function returns true if the I2C master detects an arbitration lost
 * condition, as defined by the I2C standard. When this flag sets, the I2C
 * master will release the bus (go idle) and will not initiate a new START
 * condition until this flag has been cleared.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Master has lost arbitration.
 * @return false: Master has not lost arbitration.
 */
static inline bool I2C_GetMasterArbitrationLostEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue          = (regValue & I2C_MSR_ALF_MASK) >> I2C_MSR_ALF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Get the occurrence of an unexpected NACK event.
 *
 * This function returns true if the I2C master detects a NACK when
 * transmitting an address or data. If a NACK is expected for a given address
 * (as configured by the command word) then the flag will set if a NACK is not
 * generated. When set, the master will transmit a STOP condition and will not
 * initiate a new START condition until this flag has been cleared.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Unexpected NACK was detected.
 * @return false: Unexpected NACK was not detected.
 */
static inline bool I2C_GetMasterNACKDetectEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue          = (regValue & I2C_MSR_NDF_MASK) >> I2C_MSR_NDF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Get the I2C master STOP Detect Flag.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Master has generated a STOP condition.
 * @return false: Master has not generated a STOP condition.
 */
static inline bool I2C_GetMasterStopDetectEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue          = (regValue & I2C_MSR_SDF_MASK) >> I2C_MSR_SDF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Clear the FIFO error event flag.
 *
 * This function clears the FIFO error event. This event must be cleared before
 * the I2C master can initiate a START condition.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ClearMasterFIFOErrorEvent(i2c_type_t *baseAddr)
{
    baseAddr->MSR = ((uint32_t)1U << I2C_MSR_FEF_SHIFT);
}

/**
 * @brief Clear the arbitration lost event flag.
 *
 * This function clears the arbitration lost event. This event must be cleared
 * before the I2C master can initiate a START condition.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ClearMasterArbitrationLostEvent(i2c_type_t *baseAddr)
{
    baseAddr->MSR = ((uint32_t)1U << I2C_MSR_ALF_SHIFT);
}

/**
 * @brief Clear the unexpected NACK event flag.
 *
 * This function clears the unexpected NACK event. This event must be cleared
 * before the I2C master can initiate a START condition.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ClearMasterNACKDetectEvent(i2c_type_t *baseAddr)
{
    baseAddr->MSR = ((uint32_t)1U << I2C_MSR_NDF_SHIFT);
}

/**
 * @brief Clear the I2C master STOP Detect Flag.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ClearMasterStopDetectEvent(i2c_type_t *baseAddr)
{
    baseAddr->MSR = ((uint32_t)1U << I2C_MSR_SDF_SHIFT);
}

/**
 * @brief Enable/disable receive data PDMA requests.
 *
 * This function enables or disables generation of Rx PDMA requests when data
 * can be read from the receive FIFO, as configured by the receive FIFO watermark.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Specifies whether to enable or disable PDMA requests.
 */
static inline void I2C_SetMasterRxDMA(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->MDER;
    regValue &= (uint32_t)(~(I2C_MDER_RDDE_MASK));
    regValue |= I2C_MDER_RDDE(enable);
    baseAddr->MDER = (uint32_t)regValue;
}

/**
 * @brief Enable/disable transmit data PDMA requests.
 *
 * This function enables or disables generation of Tx PDMA requests when data
 * can be written to the transmit FIFO, as configured by the transmit FIFO watermark.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Specifies whether to enable or disable PDMA requests.
 */
static inline void I2C_SetMasterTxDMA(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->MDER;
    regValue &= (uint32_t)(~(I2C_MDER_TDDE_MASK));
    regValue |= I2C_MDER_TDDE(enable);
    baseAddr->MDER = (uint32_t)regValue;
}

/**
 * @brief Enable or disable specified I2C master interrupts
 *
 * This function can enable or disable one or more master interrupt sources
 * specified by the interrupts parameter.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  interrupts Interrupts to be enabled or disabled.
 *  Must be a bitwise or between one or more of the following constants:
 *  - I2C_MASTER_DATA_MATCH_INT          - Data Match Interrupt
 *  - I2C_MASTER_PIN_LOW_TIMEOUT_INT     - Pin Low Timeout Interrupt
 *  - I2C_MASTER_FIFO_ERROR_INT          - FIFO Error Interrupt
 *  - I2C_MASTER_ARBITRATION_LOST_INT    - Arbitration Lost Interrupt
 *  - I2C_MASTER_NACK_DETECT_INT         - NACK Detect Interrupt
 *  - I2C_MASTER_STOP_DETECT_INT         - STOP Detect Interrupt
 *  - I2C_MASTER_END_PACKET_INT          - End Packet Interrupt
 *  - I2C_MASTER_RECEIVE_DATA_INT        - Receive Data Interrupt
 *  - I2C_MASTER_TRANSMIT_DATA_INT       - Transmit Data Interrupt
 * @param  enable Specifies whether to enable or disable specified interrupts.
 */
static inline void I2C_SetMasterInt(i2c_type_t *baseAddr, uint32_t interrupts, bool enable)
{
    uint32_t tmp = baseAddr->MIER;

    if (true == enable) {
        tmp |= interrupts;
    } else {
        tmp &= ~interrupts;
    }
    baseAddr->MIER = tmp;
}

/**
 * @brief Configures the I2C Host Request Enable status.
 *
 * When enabled, the I2C master will only initiate a START condition
 * if the Host Request Enable is asserted and the bus is idle.
 * A repeated START is not affected by the Host Request.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  configuration true: Host Request input is enabled.
 *                       false: Host Request input is disabled.
 */
static inline void I2C_SetMasterHostRequestEnable(i2c_type_t *baseAddr, bool configuration)
{
    uint32_t tmp = baseAddr->MCFGR0;
    tmp &= ~(I2C_MCFGR0_HREN_MASK);
    tmp |= I2C_MCFGR0_HREN(configuration);
    baseAddr->MCFGR0 = tmp;
}

/**
 * @brief Configures the I2C Host Request Polarity status.
 *
 * When host request input is enabled, the Host Request Polarity field should remain static.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  configuration true: Host Request Polarity is Active high.
 *                       false: Host Request Polarity is Active low.
 */
static inline void I2C_SetMasterHostRequestPolarity(i2c_type_t *baseAddr, bool configuration)
{
    uint32_t tmp = baseAddr->MCFGR0;
    tmp &= ~(I2C_MCFGR0_HRPOL_MASK);
    tmp |= I2C_MCFGR0_HRPOL(configuration);
    baseAddr->MCFGR0 = tmp;
}

/**
 * @brief Configures the source of the Host Request Select input.
 *
 * When Host Request Select input is enabled, the Host Request Select field should remain static.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  configuration true: Host Request Select input is input trigger.
 *                       false: Host Request Select is pin HREQ.
 */
static inline void I2C_SetMasterHostRequestSelect(i2c_type_t *baseAddr, bool configuration)
{
    uint32_t tmp = baseAddr->MCFGR0;
    tmp &= ~(I2C_MCFGR0_HRSEL_MASK);
    tmp |= I2C_MCFGR0_HRSEL(configuration);
    baseAddr->MCFGR0 = tmp;
}

/**
 * @brief Set the pin mode of the module.
 *
 * This function sets the pin mode of the module. See type i2c_pin_config_t for
 * a description of available modes.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  configuration Pin mode of the module.
 */
static inline void I2C_SetMasterPinConfig(i2c_type_t *baseAddr, i2c_pin_config_t configuration)
{
    uint32_t tmp = baseAddr->MCFGR1;
    tmp &= ~(I2C_MCFGR1_PINCFG_MASK);
    tmp |= I2C_MCFGR1_PINCFG(configuration);
    baseAddr->MCFGR1 = tmp;
}

/**
 * @brief Configure the reaction of the module on NACK reception.
 *
 * This function configures how the I2C master reacts when receiving a NACK. NACK responses can
 * be treated normally or ignored. In Ultra-Fast mode it is necessary to configure the module to
 * ignore NACK responses.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  configuration Set reaction of the module on NACK reception.
 */
static inline void I2C_SetMasterNACKConfig(i2c_type_t *baseAddr, i2c_nack_config_t configuration)
{
    uint32_t regValue = (uint32_t)baseAddr->MCFGR1;
    regValue &= (uint32_t)(~(I2C_MCFGR1_IGNACK_MASK));
    regValue |= I2C_MCFGR1_IGNACK(configuration);
    baseAddr->MCFGR1 = (uint32_t)regValue;
}

/**
 * @brief Configure the I2C master prescaler.
 *
 * This function configures the clock prescaler used for all I2C master logic,
 * except the digital glitch filters.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  prescaler I2C master prescaler.
 */
static inline void I2C_SetMasterPrescaler(i2c_type_t *baseAddr, i2c_master_prescaler_t prescaler)
{
    uint32_t tmp = baseAddr->MCFGR1;
    tmp &= ~(I2C_MCFGR1_PRESCALE_MASK);
    tmp |= I2C_MCFGR1_PRESCALE(prescaler);
    baseAddr->MCFGR1 = tmp;
}

/**
 * @brief Return the I2C master prescaler.
 *
 * This function returns the currently configured clock prescaler.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return i2c_master_prescaler_t I2C master prescaler
 */
static inline i2c_master_prescaler_t I2C_GetMasterPrescaler(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->MCFGR1;
    tmp          = (tmp & I2C_MCFGR1_PRESCALE_MASK) >> I2C_MCFGR1_PRESCALE_SHIFT;
    return (i2c_master_prescaler_t)tmp;
}

/**
 * @brief Configures the I2C master digital glitch filters for SCL input.
 *
 * A configuration of 0 will disable the glitch filter.
 * Glitches equal to or less than FILTSCL cycles long will be filtered out and ignored.
 * The latency through the glitch filter is equal to FILTSCL cycles, and must be configured to be less
 * than the minimum SCL low or high period.
 * The glitch filter cycle count is not affected by the PRESCALE configuration, and the glitch filter
 * cycle count is automatically bypassed in High Speed mode.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  value I2C master glitch filter cycle count
 */
static inline void I2C_SetMasterGlitchFilterSCL(i2c_type_t *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCFGR2;
    tmp &= ~(I2C_MCFGR2_FILTSCL_MASK);
    tmp |= I2C_MCFGR2_FILTSCL(value);
    baseAddr->MCFGR2 = tmp;
}

/**
 * @brief Return the configured minimum clock high period.
 *
 * This function returns the currently configured value for clock high period.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint8_t Minimum clock high period.
 */
static inline uint8_t I2C_GetMasterClockHighPeriod(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp          = (tmp & I2C_MCCR0_CLKHI_MASK) >> I2C_MCCR0_CLKHI_SHIFT;
    return (uint8_t)tmp;
}

/**
 * @brief Set the minimum clock high period.
 *
 * This function configures the minimum number of cycles (minus one) that the
 * SCL clock is driven high by the master. The SCL high time is extended by the
 * time it takes to detect a rising edge on the external SCL pin. Ignoring any
 * additional board delay due to external loading, this is equal to
 * (2 + FILTSCL) / 2^PRESCALE cycles.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  value Minimum clock high period.
 */
static inline void I2C_SetMasterClockHighPeriod(i2c_type_t *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp &= ~(I2C_MCCR0_CLKHI_MASK);
    tmp |= I2C_MCCR0_CLKHI(value);
    baseAddr->MCCR0 = tmp;
}

/**
 * @brief Set the data hold time for SDA.
 *
 * This function sets the minimum number of cycles (minus one) that is used as the
 * data hold time for SDA. Must be configured less than the minimum SCL low period.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  value Value of the data hold time for SDA.
 */
static inline void I2C_SetMasterDataValidDelay(i2c_type_t *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp &= ~(I2C_MCCR0_DATAVD_MASK);
    tmp |= I2C_MCCR0_DATAVD(value);
    baseAddr->MCCR0 = tmp;
}

/**
 * @brief Set the setup and hold delay for a START / STOP condition.
 *
 * This function configures the Minimum number of cycles (minus one) that is used
 * by the master as the setup and hold time for a (repeated) START condition and setup
 * time for a STOP condition. The setup time is extended by the time it takes to detect
 * a rising edge on the external SCL pin. Ignoring any additional board delay due to
 * external loading, this is equal to (2 + FILTSCL) / 2^PRESCALE cycles.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  value Setup and hold time for a START / STOP condition.
 */
static inline void I2C_SetMasterSetupHoldDelay(i2c_type_t *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp &= ~(I2C_MCCR0_SETHOLD_MASK);
    tmp |= I2C_MCCR0_SETHOLD(value);
    baseAddr->MCCR0 = tmp;
}

/**
 * @brief Set the minimum clock low period.
 *
 * This function configures the minimum number of cycles (minus one) that the
 * SCL clock is driven low by the master. This value is also used for the
 * minimum bus free time between a STOP and a START condition.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  value Minimum clock low period.
 */
static inline void I2C_SetMasterClockLowPeriod(i2c_type_t *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp &= ~(I2C_MCCR0_CLKLO_MASK);
    tmp |= I2C_MCCR0_CLKLO(value);
    baseAddr->MCCR0 = tmp;
}

/**
 * @brief Return the configured minimum clock low period.
 *
 * This function returns the currently configured value for clock low period.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint8_t Minimum clock low period.
 */
static inline uint8_t I2C_GetMasterClockLowPeriod(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp          = (tmp & I2C_MCCR0_CLKLO_MASK) >> I2C_MCCR0_CLKLO_SHIFT;
    return (uint8_t)tmp;
}

/**
 * @brief Set the receive FIFO watermark.
 *
 * This function configures the receive FIFO watermark. Whenever the number of words in the receive
 * FIFO is greater than the receive FIFO watermark, a receive data ready event is generated.
 * Writing a value equal or greater than the FIFO size will be truncated.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  value Number of words in the receive FIFO that will cause the receive data flag to be set.
 */
static inline void I2C_SetMasterRxFIFOWatermark(i2c_type_t *baseAddr, uint16_t value)
{
    uint32_t tmp = baseAddr->MFCR;
    tmp &= ~(I2C_MFCR_RXWATER_MASK);
    tmp |= I2C_MFCR_RXWATER(value);
    baseAddr->MFCR = tmp;
}

/**
 * @brief Return the configured receive FIFO watermark.
 *
 * This function returns the currently configured value for receive FIFO watermark.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint16_t Number of words in the receive FIFO that will cause the receive data flag to be set.
 */
static inline uint16_t I2C_GetMasterRxFIFOWatermark(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->MFCR;
    tmp          = (tmp & I2C_MFCR_RXWATER_MASK) >> I2C_MFCR_RXWATER_SHIFT;
    return (uint16_t)tmp;
}

/**
 * @brief Set the transmit FIFO watermark.
 *
 * This function configures the transmit FIFO watermark. Whenever the number of words in the transmit
 * FIFO is greater than the transmit FIFO watermark, a transmit data request event is generated.
 * Writing a value equal or greater than the FIFO size will be truncated.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  value Number of words in the transmit FIFO that will cause the transmit data flag to be set.
 */
static inline void I2C_SetMasterTxFIFOWatermark(i2c_type_t *baseAddr, uint16_t value)
{
    uint32_t tmp = baseAddr->MFCR;
    tmp &= ~(I2C_MFCR_TXWATER_MASK);
    tmp |= I2C_MFCR_TXWATER(value);
    baseAddr->MFCR = tmp;
}

/**
 * @brief Return the number of words in the receive FIFO.
 *
 * This function returns the number of words currently available in the receive FIFO.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint16_t The number of words in the receive FIFO.
 */
static inline uint16_t I2C_GetMasterRxFIFOCount(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->MFSR;
    tmp          = (tmp & I2C_MFSR_RXCOUNT_MASK) >> I2C_MFSR_RXCOUNT_SHIFT;
    return (uint16_t)tmp;
}

/**
 * @brief Return the number of words in the transmit FIFO.
 *
 * This function returns the number of words currently available in the transmit FIFO.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint16_t The number of words in the transmit FIFO.
 */
static inline uint16_t I2C_GetMasterTxFIFOCount(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->MFSR;
    tmp          = (tmp & I2C_MFSR_TXCOUNT_MASK) >> I2C_MFSR_TXCOUNT_SHIFT;
    return (uint16_t)tmp;
}

/**
 * @brief Provide commands and data for the I2C master.
 *
 * This function stores commands and data in the transmit FIFO and increments the FIFO
 * write pointer.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  cmd Command for the I2C master.
 * @param  data Data for the I2C master.
 */
static inline void I2C_CmdMasterTransmit(i2c_type_t *baseAddr, i2c_master_command_t cmd, uint8_t data)
{
    baseAddr->MTDR = ((uint32_t)cmd << 8U) + (uint32_t)data;
}

/**
 * @brief Return the received data.
 *
 * This function returns data received by the I2C master that has not been discarded
 * due to data match settings or active command, and increments the FIFO read pointer.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint8_t Data received by the I2C master.
 */
static inline uint8_t I2C_GetMasterRxData(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->MRDR;
    tmp          = (tmp & I2C_MRDR_DATA_MASK) >> I2C_MRDR_DATA_SHIFT;
    return (uint8_t)tmp;
}

/**
 * @brief Enable or disable the I2C slave.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Specifies whether to enable or disable the I2C slave.
 */
static inline void I2C_SetSlaveEnable(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCR;
    regValue &= (uint32_t)(~(I2C_SCR_SEN_MASK));
    regValue |= I2C_SCR_SEN(enable);
    baseAddr->SCR = (uint32_t)regValue;
}

/**
 * @brief Set/clear the slave reset command.
 *
 * Calling this function with enable parameter set to true will perform a software
 * reset of the I2C slave.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Specifies the reset state of the I2C slave logic.
 */
static inline void I2C_SetSlaveSoftwareReset(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCR;
    regValue &= (uint32_t)(~(I2C_SCR_RST_MASK));
    regValue |= I2C_SCR_RST(enable);
    baseAddr->SCR = (uint32_t)regValue;
}

/**
 * @brief Enable/Disable the slave Filter.
 *
 * Enable/Disable digital filter and output delay counter for slave mode.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Specifies the Enable/Disable state of the I2C slave Filter.
 */
static inline void I2C_SetSlaveFilterEnable(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCR;
    regValue &= (uint32_t)(~(I2C_SCR_FILTEN_MASK));
    regValue |= I2C_SCR_FILTEN(enable);
    baseAddr->SCR = (uint32_t)regValue;
}

#if (I2C_HAS_ULTRA_FAST_MODE)
/**
 * @brief Check the detection of a FIFO overflow or underflow
 *
 * This function checks for the occurrence of a slave FIFO overflow or underflow.
 * This event can only occur if clock stretching is disabled.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: FIFO underflow or overflow was detected.
 * @return false: FIFO underflow or overflow was not detected.
 */
static inline bool I2C_GetSlaveFIFOErrorEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue          = (regValue & I2C_SSR_FEF_MASK) >> I2C_SSR_FEF_SHIFT;
    return (bool)regValue;
}
#endif

/**
 * @brief Check the detection of a bit error.
 *
 * This function checks for the occurrence of a bit error event. This event occurs
 * if the I2C slave transmits a logic one and detects a logic zero on the I2C bus. The
 * slave will ignore the rest of the transfer until the next (repeated) START condition.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Slave has detected a bit error.
 * @return false: Slave has not detected a bit error.
 */
static inline bool I2C_GetSlaveBitErrorEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue          = (regValue & I2C_SSR_BEF_MASK) >> I2C_SSR_BEF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Check the detection of a STOP condition.
 *
 * This function checks for the detection of a STOP condition, after the I2C slave
 * matched the last address byte.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Slave has detected a STOP condition.
 * @return false: Slave has not detected a STOP condition.
 */
static inline bool I2C_GetSlaveSTOPDetectEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue          = (regValue & I2C_SSR_SDF_MASK) >> I2C_SSR_SDF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Check the detection of a repeated START condition.
 *
 * This function checks for the detection of a repeated START condition, after
 * the I2C slave matched the last address byte. This event does not occur
 * when the slave first detects a START condition.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Slave has detected a Repeated START condition.
 * @return false: Slave has not detected a Repeated START condition.
 */
static inline bool I2C_GetSlaveRepeatedStartEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue          = (regValue & I2C_SSR_RSF_MASK) >> I2C_SSR_RSF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Check the validity of the Address Status Register.
 *
 * This function checks for the detection of a valid address. The event is
 * cleared by reading the address - see function I2C_GetSlaveReceivedAddr().
 * It can also be cleared by reading the data register, when data register has
 * been configured to allow address reads.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Address Status Register is valid
 * @return false: Address Status Register is not valid
 */
static inline bool I2C_GetSlaveAddressValidEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue          = (regValue & I2C_SSR_AVF_MASK) >> I2C_SSR_AVF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Check the availability of receive data.
 *
 * This function checks for the availability of data received by the I2C slave.
 * The event is cleared by reading the received data - see function
 * I2C_GetSlaveData(). The event is not cleared by calling
 * I2C_GetSlaveData() if the data register is configured to allow address
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Receive data is ready.
 * @return false: Receive data is not ready.
 */
static inline bool I2C_GetSlaveReceiveDataEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue          = (regValue & I2C_SSR_RDF_MASK) >> I2C_SSR_RDF_SHIFT;
    return (bool)regValue;
}

/**
 * @brief Check if transmit data is requested.
 *
 * This function checks if the I2C slave requests data to transmit. The
 * event is cleared by providing transmit data - see function
 * I2C_TransmitSlaveData(). The event can also be automatically cleared
 * if the I2C module detects a NACK or a repeated START or STOP condition
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return true: Transmit data is requested.
 * @return false: Transmit data not requested.
 */
static inline bool I2C_GetSlaveTransmitDataEvent(const i2c_type_t *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue          = (regValue & I2C_SSR_TDF_MASK) >> I2C_SSR_TDF_SHIFT;
    return (bool)regValue;
}

#if (I2C_HAS_ULTRA_FAST_MODE)
/**
 * @brief Clear the FIFO overflow or underflow flag.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ClearSlaveFIFOErrorEvent(i2c_type_t *baseAddr)
{
    baseAddr->SSR = ((uint32_t)1U << I2C_SSR_FEF_SHIFT);
}
#endif

/**
 * @brief Clear bit error flag.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ClearSlaveBitErrorEvent(i2c_type_t *baseAddr)
{
    baseAddr->SSR = ((uint32_t)1U << I2C_SSR_BEF_SHIFT);
}

/**
 * @brief Clear the STOP detect flag.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ClearSlaveSTOPDetectEvent(i2c_type_t *baseAddr)
{
    baseAddr->SSR = ((uint32_t)1U << I2C_SSR_SDF_SHIFT);
}

/**
 * @brief Clear the repeated START detect flag.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
static inline void I2C_ClearSlaveRepeatedStartEvent(i2c_type_t *baseAddr)
{
    baseAddr->SSR = ((uint32_t)1U << I2C_SSR_RSF_SHIFT);
}

/**
 * @brief Enable or disable specified I2C slave interrupts
 *
 * This function can enable or disable one or more slave interrupt sources
 * specified by the interrupts parameter.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  interrupts Interrupts to be enabled or disabled.
 *  Must be a bitwise or between one or more of the following constants:
 *  - I2C_SLAVE_SMBUS_ALERT_RESPONSE  - SMBus Alert Response Interrupt
 *  - I2C_SLAVE_GENERAL_CALL          - General Call Interrupt
 *  - I2C_SLAVE_ADDRESS_MATCH_1       - Address Match 1 Interrupt
 *  - I2C_SLAVE_ADDRESS_MATCH_0       - Address Match 0 Interrupt
 *  - I2C_SLAVE_FIFO_ERROR            - FIFO Error Interrupt
 *  - I2C_SLAVE_BIT_ERROR             - Bit Error Interrupt
 *  - I2C_SLAVE_STOP_DETECT           - STOP Detect Interrupt
 *  - I2C_SLAVE_REPEATED_START        - Repeated Start Interrupt
 *  - I2C_SLAVE_TRANSMIT_ACK          - Transmit ACK Interrupt
 *  - I2C_SLAVE_ADDRESS_VALID         - Address Valid Interrupt
 *  - I2C_SLAVE_RECEIVE_DATA          - Receive Data Interrupt
 *  - I2C_SLAVE_TRANSMIT_DATA         - Transmit Data Interrupt
 * @param  enable Specifies whether to enable or disable specified interrupts.
 */
static inline void I2C_SetSlaveInt(i2c_type_t *baseAddr, uint32_t interrupts, bool enable)
{
    uint32_t tmp = baseAddr->SIER;

    if (true == enable) {
        tmp |= interrupts;
    } else {
        tmp &= ~interrupts;
    }
    baseAddr->SIER = tmp;
}

/**
 * @brief Return the state of the specified I2C slave interrupt
 *
 * This function returns the enabled/disabled state of the slave interrupt
 * source specified by the interrupt parameter.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  interrupts Interrupt for which the check is made.
 *  Must be one of the following constants:
 *  - I2C_SLAVE_SMBUS_ALERT_RESPONSE  - SMBus Alert Response Interrupt
 *  - I2C_SLAVE_GENERAL_CALL          - General Call Interrupt
 *  - I2C_SLAVE_ADDRESS_MATCH_1       - Address Match 1 Interrupt
 *  - I2C_SLAVE_ADDRESS_MATCH_0       - Address Match 0 Interrupt
 *  - I2C_SLAVE_FIFO_ERROR            - FIFO Error Interrupt
 *  - I2C_SLAVE_BIT_ERROR             - Bit Error Interrupt
 *  - I2C_SLAVE_STOP_DETECT           - STOP Detect Interrupt
 *  - I2C_SLAVE_REPEATED_START        - Repeated Start Interrupt
 *  - I2C_SLAVE_TRANSMIT_ACK          - Transmit ACK Interrupt
 *  - I2C_SLAVE_ADDRESS_VALID         - Address Valid Interrupt
 *  - I2C_SLAVE_RECEIVE_DATA          - Receive Data Interrupt
 *  - I2C_SLAVE_TRANSMIT_DATA         - Transmit Data Interrupt
 * @return true: Enable state of specified interrupt.
 * @return false: Disable state of specified interrupt.
 */
static inline bool I2C_GetSlaveInt(const i2c_type_t *baseAddr, uint32_t interrupts)
{
    uint32_t tmp       = baseAddr->SIER;
    bool hasInterrupts = false;

    if ((tmp & interrupts) != (uint32_t)0U) {
        hasInterrupts = true;
    }

    return hasInterrupts;
}

/**
 * @brief Enable/disable slave receive data PDMA requests
 *
 * This function enables or disables generation of Rx PDMA requests when received
 * data is available.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Specifies whether to enable or disable receive data PDMA requests.
 */
static inline void I2C_SetSlaveRxDMA(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SDER;
    regValue &= (uint32_t)(~(I2C_SDER_RDDE_MASK));
    regValue |= I2C_SDER_RDDE(enable);
    baseAddr->SDER = (uint32_t)regValue;
}

/**
 * @brief Enable/disable slave transmit data PDMA requests
 *
 * This function enables or disables generation of Tx PDMA requests when the module
 * requires more data to transmit.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Specifies whether to enable or disable transmit data PDMA requests.
 */
static inline void I2C_SetSlaveTxDMA(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SDER;
    regValue &= (uint32_t)(~(I2C_SDER_TDDE_MASK));
    regValue |= I2C_SDER_TDDE(enable);
    baseAddr->SDER = (uint32_t)regValue;
}

/**
 * @brief Control address match configuration
 *
 * This function configures the condition that will cause an address match to
 * occur. See type i2c_slave_addr_config_t for a description of available options.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  configuration Configures the condition that will cause an address to match.
 */
static inline void I2C_SetSlaveAddrConfig(i2c_type_t *baseAddr, i2c_slave_addr_config_t configuration)
{
    uint32_t tmp = baseAddr->SCFGR1;
    tmp &= ~(I2C_SCFGR1_ADDRCFG_MASK);
    tmp |= I2C_SCFGR1_ADDRCFG(configuration);
    baseAddr->SCFGR1 = tmp;
}

/**
 * @brief Control detection of the High-speed Mode master code
 *
 * This function enables or disables the detection of the High-speed Mode
 * master code of slave address 0000_1XX, but does not cause an address match
 * on this code. When set and any Hs-mode master code is detected, the slave
 * filter and ACK stalls are disabled until the next STOP condition is detected.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Enable/disable the detection of the High-speed Mode master code.
 */
static inline void I2C_SetSlaveHighSpeedModeDetect(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(I2C_SCFGR1_HSMEN_MASK));
    regValue |= I2C_SCFGR1_HSMEN(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/**
 * @brief Control slave behaviour when NACK is detected
 *
 * This function controls the option to ignore received NACKs. When enabled, the
 * I2C slave will continue transfers after a NACK is detected. This option is needed
 * for Ultra-Fast mode.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  nack_config Slave behaviour when NACK is detected.
 */
static inline void I2C_SetSlaveIgnoreNACK(i2c_type_t *baseAddr, i2c_slave_nack_config_t nack_config)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(I2C_SCFGR1_IGNACK_MASK));
    regValue |= I2C_SCFGR1_IGNACK(nack_config);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/**
 * @brief Enable or disable clock stretching for the sending of the ACK bit
 *
 * This function enables or disables SCL clock stretching during slave-transmit address
 * byte(s) and slave-receiver address and data byte(s) to allow software to write the
 * Transmit ACK Register before the ACK or NACK is transmitted. Clock stretching occurs
 * when transmitting the 9th bit and is therefore not compatible with high speed mode.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Enable or disable clock stretching.
 */
static inline void I2C_SetSlaveACKStall(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(I2C_SCFGR1_ACKSTALL_MASK));
    regValue |= I2C_SCFGR1_ACKSTALL(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/**
 * @brief Enable or disable clock stretching for data transmission
 *
 * This function enables or disables SCL clock stretching when the transmit data
 * flag is set during a slave-transmit transfer. Clock stretching occurs following
 * the 9th bit and is therefore compatible with high speed mode.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Enable or disable clock stretching.
 */
static inline void I2C_SetSlaveTXDStall(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(I2C_SCFGR1_TXDSTALL_MASK));
    regValue |= I2C_SCFGR1_TXDSTALL(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/**
 * @brief Enable or disable clock stretching for data reception
 *
 * This function enables or disables SCL clock stretching when receive data flag
 * is set during a slave-receive transfer. Clock stretching occurs following the 9th
 * bit and is therefore compatible with high speed mode.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Enable or disable clock stretching.
 */
static inline void I2C_SetSlaveRXStall(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(I2C_SCFGR1_RXSTALL_MASK));
    regValue |= I2C_SCFGR1_RXSTALL(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/**
 * @brief Enable or disable clock stretching for valid address reception
 *
 * This function enables or disables SCL clock stretching when the address valid
 * flag is asserted. Clock stretching only occurs following the 9th bit and is
 * therefore compatible with high speed mode.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  enable Enable or disable clock stretching.
 */
static inline void I2C_SetSlaveAddrStall(i2c_type_t *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(I2C_SCFGR1_ADRSTALL_MASK));
    regValue |= I2C_SCFGR1_ADRSTALL(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/**
 * @brief Configures the minimum clock hold time for the I2C slave, when clock stretching is enabled.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  data Hold time for the I2C slave minimum clock.
 */
static inline void I2C_SetSlaveClockHoldTime(i2c_type_t *baseAddr, uint8_t data)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR2;
    regValue &= (uint32_t)(~(I2C_SCFGR2_CLKHOLD_MASK));
    regValue |= I2C_SCFGR2_CLKHOLD(data);
    baseAddr->SCFGR2 = (uint32_t)regValue;
}

/**
 * @brief Configures the SDA data valid delay time for the I2C slave,
 * and is equal to FILTSCL + DATAVD + 3 cycles.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  data DATAVD data value [0-63].
 */
static inline void I2C_SetSlaveDataValidDelay(i2c_type_t *baseAddr, uint8_t data)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR2;
    regValue &= (uint32_t)(~(I2C_SCFGR2_DATAVD_MASK));
    regValue |= I2C_SCFGR2_DATAVD(data);
    baseAddr->SCFGR2 = (uint32_t)regValue;
}

/**
 * @brief Configure the ADDR0 address for slave address match
 *
 * This function configures the ADDR0 value which is used to validate the received
 * slave address. In 10-bit mode, the first address byte is compared to
 * { 11110, ADDR0[10:9] } and the second address byte is compared to ADDR0[8:1].
 * In 7-bit mode, the address is compared to ADDR0[7:1]
 * The formula used for address validation is configured with function
 * I2C_SetSlaveAddrConfig().
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  addr ADDR0 Address for slave address match.
 */
static inline void I2C_SetSlaveAddr0(i2c_type_t *baseAddr, uint16_t addr)
{
    uint32_t tmp = baseAddr->SAMR;
    tmp &= ~(I2C_SAMR_ADDR0_MASK);
    tmp |= I2C_SAMR_ADDR0(addr);
    baseAddr->SAMR = tmp;
}

/**
 * @brief Return the received slave address
 *
 * This function returns the received slave address. Reading the address clears
 * the address valid event. The address can be 7-bit or 10-bit (10-bit addresses
 * are prefixed by 11110) and includes the R/W bit in the least significant position.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint16_t Received address
 */
static inline uint16_t I2C_GetSlaveReceivedAddr(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->SASR;
    tmp          = (tmp & I2C_SASR_RADDR_MASK) >> I2C_SASR_RADDR_SHIFT;
    return (uint16_t)tmp;
}

/**
 * @brief Configure the ACK/NACK transmission after a received byte
 *
 * This function can be used to instruct the I2C slave whether to send an ACK or
 * a NACK after receiving a byte. When ACK stall is enabled this function must be
 * called after each matching address and after each received data byte. It can also
 * be called when I2C Slave is disabled or idle to configure the default ACK/NACK.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  nack Specifies whether to transmit ACK or NACK.
 */
static inline void I2C_SetSlaveTransmitNACK(i2c_type_t *baseAddr, i2c_slave_nack_transmit_t nack)
{
    uint32_t regValue = (uint32_t)baseAddr->STAR;
    regValue &= (uint32_t)(~(I2C_STAR_TXNACK_MASK));
    regValue |= I2C_STAR_TXNACK(nack);
    baseAddr->STAR = (uint32_t)regValue;
}

/**
 * @brief Provide data for the I2C slave transmitter
 *
 * This function provides one byte of data for the I2C slave to transmit.
 * Calling this function clears the transmit data event.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  data Data for the I2C slave transmitter.
 */
static inline void I2C_TransmitSlaveData(i2c_type_t *baseAddr, uint8_t data)
{
    baseAddr->STDR = (uint32_t)data;
}

/**
 * @brief Return the data received by the I2C slave receiver
 *
 * This function returns the data received by the I2C slave.
 * Calling this function clears the receive data event.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @return uint8_t Data received by the I2C slave receiver.
 */
static inline uint8_t I2C_GetSlaveData(const i2c_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->SRDR;
    tmp          = (tmp & I2C_SRDR_DATA_MASK) >> I2C_SRDR_DATA_SHIFT;
    return (uint8_t)tmp;
}

/**
 * @brief Set the timeout for bus idle for Master
 *
 * This function sets time out for bus idle for Master.If both SCL and SDA are high for longer than
 * Timeout cycles, then the I2C bus is assumed to be idle and the master can generate a START condition.
 *
 * @param  baseAddr The I2C peripheral base address.
 * @param  timeout Bus idle timeout period. Zero means no bus idle timeout
 */
static inline void I2C_SetMasterBusIdleTimeout(i2c_type_t *baseAddr, uint16_t timeout)
{
    uint32_t tmp = baseAddr->MCFGR2;
    tmp &= ~(I2C_MCFGR2_BUSIDLE_MASK);
    tmp |= I2C_MCFGR2_BUSIDLE(timeout);
    baseAddr->MCFGR2 = tmp;
}

/**
 * @brief Initializes the I2C module to a known state.
 *
 * This function initializes all the registers of the I2C module to their reset value.
 *
 * @param  baseAddr The I2C peripheral base address.
 */
void I2C_Init(i2c_type_t *baseAddr);

#if defined(__cplusplus)
}
#endif

#endif /* __I2C_HW_ACCESS_H__ */
