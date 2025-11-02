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
 * @file superio_driver.h
 * @brief SUPERIO driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERIO_DRIVER_H__
#define __SUPERIO_DRIVER_H__

#include "emps_platform.h"
#include "osal.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Driver type: interrupts/polling/PDMA.
 */
typedef enum {
    SUPERIO_DRIVER_TYPE_INTERRUPTS = 0U, /**< Driver uses interrupts for data transfers. */
    SUPERIO_DRIVER_TYPE_POLLING    = 1U, /**< Driver is based on polling. */
    SUPERIO_DRIVER_TYPE_DMA        = 2U, /**< Driver uses PDMA for data transfers. */
} superio_driver_type_t;

/**
 * @brief SuperIO interrupt service routine.
 */
typedef void (*superio_isr_t)(void *isrParam);

/**
 * @brief SuperIO common context structure.
 *
 * This is a common structure used by all SuperIO drivers as a part of their context structure.
 * It is needed for common operations such as interrupt handling.
 */
typedef struct {
    superio_isr_t isr;     /**< Interrupt handler for this driver instance. */
    uint32_t instance;     /**< SuperIO device instance number. */
    uint8_t resourceCount; /**< Count of internal resources used (shifters and timers). */
    uint8_t resourceIndex; /**< Index of first used internal resource instance (shifter and timer). */
} superio_common_state_t;

/**
 * @brief SuperIO device context structure.
 *
 * This is a structure containing data common to all drivers on one device.
 */
typedef struct {
    uint8_t resourceAllocation; /**< Mask to keep track of resources allocated on current device. */
    OS_MutexId_t resourceLock;  /**< Mutex for guarding channel allocation. */
    /**< Array of pointers to runtime state structures. Each SuperIO instance can have at most
       one driver instance per shifter. */
    superio_common_state_t *superioStatePtr[FEATURE_SUPERIO_MAX_SHIFTER_COUNT];
} superio_device_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initializes the SuperIO device.
 *
 * This function resets the SuperIO device, enables interrupts in interrupt manager and enables the device.
 *
 * @param instance     SuperIO peripheral instance number.
 * @param deviceState  Pointer to the SuperIO device context structure. The driver uses
 *                     this memory area for its internal logic. The application must make no
 *                     assumptions about the content of this structure, and must not free this
 *                     memory until the device is de-initialized using SUPERIO_DRV_DeinitDevice().
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_DRV_InitDevice(uint32_t instance, superio_device_state_t *deviceState);

/**
 * @brief De-initializes the SuperIO device.
 *
 * This function de-initializes the SuperIO device.
 *
 * @param instance  SuperIO peripheral instance number.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_DRV_DeinitDevice(uint32_t instance);

/**
 * @brief Resets the SuperIO device.
 *
 * This function resets the SuperIO device.
 *
 * @param instance  SuperIO peripheral instance number.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_DRV_Reset(uint32_t instance);

/*
 * Function Name : SUPERIO_DRV_TcsrStopZoneMode
 * Description   : Enter/exit stop mode according the TCSR
 *
 */
void SUPERIO_UART_DRV_TcsrStopZoneMode(void *state, bool en);

#if defined(__cplusplus)
}
#endif

#endif /* __SUPERIO_DRIVER_H__ */
