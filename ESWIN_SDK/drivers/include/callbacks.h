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
 * @file callbacks.h
 * @brief Callback driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <stdint.h>

/**
 * @brief Define the enum of the events which can trigger I2C slave callback.
 *
 * This enum should include the events for all platforms.
 */
typedef enum {
    I2C_SLAVE_EVENT_RX_FULL  = 0x00U,
    I2C_SLAVE_EVENT_TX_EMPTY = 0x01U,
    I2C_SLAVE_EVENT_TX_REQ   = 0x02U,
    I2C_SLAVE_EVENT_RX_REQ   = 0x03U,
    I2C_SLAVE_EVENT_STOP     = 0X04U,
} i2c_slave_event_t;

/**
 * @brief Define the enum of the events which can trigger I2C master callback.
 *
 * This enum should include the events for all platforms.
 */
typedef enum {
    I2C_MASTER_EVENT_END_TRANSFER = 0x4U,
} i2c_master_event_t;

/**
 * @brief Callback for all peripherals which supports I2C features for slave mode.
 */
typedef void (*i2c_slave_callback_t)(i2c_slave_event_t event, void *userData);

/**
 * @brief Callback for all peripherals which supports I2C features for master mode.
 */
typedef void (*i2c_master_callback_t)(i2c_master_event_t event, void *userData);

/**
 * @brief  Define the enum of the events which can trigger SPI callback.
 *
 * This enum should include the events for all platforms.
 */
typedef enum {
    SPI_EVENT_END_TRANSFER = 0,
    SPI_EVENT_DATA_MATCH   = 1,
} spi_event_t;

/**
 * @brief Callback for all peripherals which supports SPI features.
 */
typedef void (*spi_callback_t)(void *driverState, spi_event_t event, void *userData);

/**
 * @brief  Define the enum of the events which can trigger PWM callback.
 *
 * This enum should include the events for all platforms.
 */
typedef enum {
    EVENT_FIFO_EMPTY = 0,
    EVENT_ROLL_OVER  = 1,
    EVENT_COMPARE    = 3,
    EVENT_FIFO_ERROR = 4,
} pwm_event_t;

/**
 * @brief Callback for all peripherals which supports PWM features.
 */
typedef void (*pwm_callback_t)(void *driverState, pwm_event_t event, void *userData);

/**
 * @brief Define the enum of the events which can trigger UART callback.
 *
 * This enum should include the events for all platforms.
 */
typedef enum {
    UART_EVENT_RX_FULL      = 0x00U, /**< Rx buffer is full. */
    UART_EVENT_TX_EMPTY     = 0x01U, /**< Tx buffer is empty. */
    UART_EVENT_END_TRANSFER = 0x02U, /**< The current transfer is ending. */
    UART_EVENT_RX_IDLE      = 0x03U, /**< Rx line is idle */
    UART_EVENT_ERROR        = 0x04U, /**< An error occurred during transfer. */
} uart_event_t;

/**
 * @brief Callback for all peripherals which support UART features.
 */
typedef void (*uart_callback_t)(void *driverState, uart_event_t event, void *userData);

/**
 * @brief Callback for all peripherals which support TIMING features.
 */
typedef void (*timer_callback_t)(void *userData);

/**
 * @brief Events for all peripherals which support i2s.
 */
typedef enum {
    I2S_EVENT_RX_FULL  = 0x00U, /**< Rx buffer is full. */
    I2S_EVENT_TX_EMPTY = 0x01U, /**< Tx buffer is empty. */
    I2S_EVENT_END_TRANSFER =
        0x02U,               /**< The current transfer is ending. Only SUPERIO instance uses this event. The difference between this and
                * event TX_EMPTY is: TX_EMPTY is generated when all data has been pushed to hardware fifo, users should
                * not call DeInit here or some last data will be lost; END_TRANSFER is generated when all data has been
                * pushed to line, the transmission will be stopped before users can start transmit again, user can call
                * DeInit here. For receiving case, this event is the same as RX_FULL.
                */
    I2S_EVENT_ERROR = 0x03U, /**An error occurred during transfer */
} i2s_event_t;

/**
 * @brief Callback for all peripherals which support i2s.
 */
typedef void (*i2s_callback_t)(i2s_event_t event, void *userData);

/**
 * @brief Define the enum of the events which can trigger CAN callback.
 *
 * This enum should include the events for all platforms.
 */
typedef enum {
    CAN_EVENT_RX_COMPLETE, /**< A frame was received in the configured Rx buffer. */
    CAN_EVENT_TX_COMPLETE, /**< A frame was sent from the configured Tx buffer. */
} can_event_t;

/**
 * @brief Callback for all peripherals which support CAN features.
 */
typedef void (*can_callback_t)(uint32_t instance, can_event_t eventType, uint32_t objIdx, void *driverState);

/**
 * @brief Callback for security modules.
 */
typedef void (*security_callback_t)(uint32_t completedCmd, void *callbackParam);

/**
 * @brief Define the enum of the events which can trigger the output compare callback.
 */
typedef enum {
    OC_EVENT_GENERATION_OUTPUT_COMPLETE = 0x00U /**< Generation output signal is completed. */
} oc_event_t;

/**
 * @brief Callback for all peripherals which support OC feature.
 */
typedef void (*oc_callback_t)(oc_event_t event, void *userData);

/**
 * @brief  Define the enum of the events which can trigger the input capture callback.
 */
typedef enum {
    IC_EVENT_MEASUREMENT_COMPLETE = 0x00U /**< Capture input signal is completed. */
} ic_event_t;

/**
 * @brief Callback for all peripherals which support IC feature.
 */
typedef void (*ic_callback_t)(ic_event_t event, void *userData);

/**
 * @brief  Define the enum of the events which supertmr callback.
 */
typedef enum {
    SUPERTMR_EVENT_TIMER_OVERFLOW = 0x00U, /**< Timer overflow, the counter passes the value int the MOD register. */
    SUPERTMR_EVENT_RELOAD_POINT   = 0x01U, /**< Reload Point */
    SUPERTMR_EVENT_FAULT          = 0x02U, /**< Fault Detect. */
} supertmr_event_t;

/**
 * @brief Callback for supertmr event.
 */
typedef void (*supertmr_callback_t)(supertmr_event_t event, void *userData);

/**
 * @brief Define the enum of the events which can trigger the cmu callback.
 */
typedef enum {
    CMU_EVENT_NORMAL   = 0x00U,
    CMU_EVENT_ABNORMAL = 0x01U,
} cmu_event_t;

/**
 * @brief Callback for all peripherals which support CMU feature.
 */
typedef void (*cmu_callback_t)(cmu_event_t event, void *userData);

#endif /* __CALLBACKS_H__ */
