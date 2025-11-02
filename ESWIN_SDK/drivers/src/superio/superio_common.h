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
 * @file superio_common.h
 * @brief SUPERIO common driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERIO_COMMON_DRIVER_H__
#define __SUPERIO_COMMON_DRIVER_H__

#include "pdma_driver.h"
#include "superio_driver.h"
#include "superio_hw_access.h"
#include "superio_register.h"
#include <stdbool.h>
#include <stddef.h>

#define DMA_TCD_CSR_START_MASK  0x1U
#define DMA_TCD_CSR_START_SHIFT 0U
#define DMA_TCD_CSR_START_WIDTH 1U
#define DMA_TCD_CSR_START(x)    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_START_SHIFT)) & DMA_TCD_CSR_START_MASK)

#define DMA_TCD_CSR_DREQ_MASK  0x8U
#define DMA_TCD_CSR_DREQ_SHIFT 3U
#define DMA_TCD_CSR_DREQ_WIDTH 1U
#define DMA_TCD_CSR_DREQ(x)    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_DREQ_SHIFT)) & DMA_TCD_CSR_DREQ_MASK)

/** Table of base addresses for SUPERIO instances. */
extern superio_type_t *const g_superioBase[SUPERIO_INSTANCE_COUNT];

/** Pointer to device state structures. This structure contains data common to all drivers on one device */
extern superio_device_state_t *g_superioDeviceStatePtr[SUPERIO_INSTANCE_COUNT];

/** Table for SUPERIO IRQ numbers */
extern const IRQn_Type g_superioIrqId[SUPERIO_INSTANCE_COUNT];

/** PCC clock sources, for getting the input clock frequency */
extern const clock_names_t g_superioClock[SUPERIO_INSTANCE_COUNT];

/** SuperIO PDMA request sources */
extern const uint8_t g_superioDMASrc[SUPERIO_INSTANCE_COUNT][FEATURE_SUPERIO_MAX_SHIFTER_COUNT];

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Function Name : SUPERIO_DRV_InitDriver
 * Description   : Initializes an instance of SuperIO driver
 *
 */
status_t SUPERIO_DRV_InitDriver(uint32_t instance, superio_common_state_t *driver);

/**
 * Function Name : SUPERIO_DRV_DeinitDriver
 * Description   : De-initializes an instance of SuperIO driver
 *
 */
status_t SUPERIO_DRV_DeinitDriver(const superio_common_state_t *driver);

#if defined(__cplusplus)
}
#endif

#endif /* __SUPERIO_COMMON_DRIVER_H__ */
