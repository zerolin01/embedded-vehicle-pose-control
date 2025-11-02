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
 * @file w5500_driver.h
 * @brief w5500 driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#ifndef __W5500_DRIVER_H__
#define __W5500_DRIVER_H__

#include "callbacks.h"
#include "emps_platform.h"
#include "osal.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>

/* do ping options numbers of retries. */
extern uint8_t req;
/* do ping options numbers of reply. */
extern uint8_t rep;
/* send buffer. */
extern uint8_t txsize[];
/* receive buffer. */
extern uint8_t rxsize[];

/**
 * @brief Initializes an w5500 operation instance.
 */
void W5500_DRV_Reset(void);

/**
 * @brief Set mac address for an w5500 operation instance.
 */
void W5500_DRV_SetMAC(void);

/**
 * @brief Set ip address for an w5500 operation instance.
 */
void W5500_DRV_SetIP(void);

/**
 * @brief Init socket buffer for an w5500 operation instance.
 *
 * The caller provides memory for the driver state structures during initialization.
 * The user must select the UART clock source in the application to initialize the UART.
 *
 * @param tx_size  The send buffer.
 * @param rx_size  The receive buffer.
 */
void W5500_DRV_InitSocketBuffer(uint8_t * tx_size, uint8_t * rx_size);

/**
 * @brief Ping master machine for an w5500 operation instance.
 */
uint8_t W5500_DRV_DoPing(void);

#endif /* __W5500_DRIVER_H__ */
