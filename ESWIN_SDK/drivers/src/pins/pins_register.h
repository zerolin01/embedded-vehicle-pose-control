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
 * @file pins_register.h
 * @brief PINS driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PINS_REGISTER_H__
#define __PINS_REGISTER_H__

#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- PINMUX Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PINMUX_Access_Layer PINMUX Access Layer
 * @{
 */

/**
 * @brief GPIO Register Layout Typedef
 */

typedef struct {
  __IO uint32_t PORTA_PCR[PINMUX_PORT_PCR_COUNT];                            /**< PortA Control Register */
  __IO uint32_t PORTB_PCR[PINMUX_PORT_PCR_COUNT];                            /**< PortB Control Register */
  __IO uint32_t PORTC_PCR[PINMUX_PORT_PCR_COUNT];                            /**< PortC Control Register */
  __IO uint32_t PORTD_PCR[PINMUX_PORT_PCR_COUNT];                            /**< PortD Control Register */
  __IO uint32_t DR[PINMUX_PORT_COUNT];                                       /**< Port Data Register */
  __IO uint32_t DIR[PINMUX_PORT_COUNT];                                      /**< Port Data Direction Register */
  __O  uint32_t PSR[PINMUX_PORT_COUNT];                                      /**< Port Pad Status Register */
  __IO uint32_t ICR[PINMUX_PORT_COUNT * 2];                                  /**< Port Interrupt Configuration Register */
  __IO uint32_t IMR[PINMUX_PORT_COUNT];                                      /**< Port Interrupt Mask Register */
  __IO uint32_t ISR[PINMUX_PORT_COUNT];                                      /**< Port Interrupt Status Register */
  __IO uint32_t EDGE_SEL[PINMUX_PORT_COUNT];                                 /**< Port Edge Select Register */
  __IO uint32_t DEB_SEL[PINMUX_PORT_COUNT];                                  /**< Port Debounce Enable Register */
  __IO uint32_t DMA_EN[PINMUX_PORT_COUNT];                                   /**< Dma Enable Register 0-31 used */
  __IO uint32_t DMA_SEL[PINMUX_PORT_COUNT];                                  /**< Dma Selected Register 0-6 used, 7-31 reserved */
} pins_type_t;

/* ----------------------------------------------------------------------------
   -- PINMUX Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PINMUX_Register_Masks PINMUX Register Masks
 * @{
 */

/** PORT PCR Bit Fields. */
/** PORT PCR FS Bit Fields, Pin Function Select Register. */
#define PORT_PCR_FS_MASK                        0x7U
#define PORT_PCR_FS_SHIFT                       0U
#define PORT_PCR_FS_WIDTH                       3U
#define PORT_PCR_FS(x)                          (((uint32_t)(((uint32_t)(x))<<PORT_PCR_FS_SHIFT))&PORT_PCR_FS_MASK)
/** PORT PCR PE Bit Fields, Pull resistor enable signal. */
#define PORT_PCR_PE_MASK                         0x8U
#define PORT_PCR_PE_SHIFT                        3U
#define PORT_PCR_PE_WIDTH                        1U
#define PORT_PCR_PE(x)                           (((uint32_t)(((uint32_t)(x))<<PORT_PCR_PE_SHIFT))&PORT_PCR_PE_MASK)
/** PORT PCR PS Bit Fields, Pull resistor selection signal. */
#define PORT_PCR_PS_MASK                         0x10U
#define PORT_PCR_PS_SHIFT                        4U
#define PORT_PCR_PS_WIDTH                        1U
#define PORT_PCR_PS(x)                           (((uint32_t)(((uint32_t)(x))<<PORT_PCR_PS_SHIFT))&PORT_PCR_PS_MASK)
/** PORT PCR DS Bit Fields, Used to select output drive strength. */
#define PORT_PCR_DS_MASK                         0x60U
#define PORT_PCR_DS_SHIFT                        5U
#define PORT_PCR_DS_WIDTH                        2U
#define PORT_PCR_DS(x)                           (((uint32_t)(((uint32_t)(x))<<PORT_PCR_DS_SHIFT))&PORT_PCR_DS_MASK)
/** PORT PCR IER Bit Fields, Used to configure register input enable or not. */
#define PORT_PCR_IER_MASK                        0x80U
#define PORT_PCR_IER_SHIFT                       7U
#define PORT_PCR_IER_WIDTH                       1U
#define PORT_PCR_IER(x)                          (((uint32_t)(((uint32_t)(x))<<PORT_PCR_IER_SHIFT))&PORT_PCR_IER_MASK)
/** PORT PCR IES Bit Fields, Used to select input function configuration or input enable configuration. */
#define PORT_PCR_IES_MASK                        0x100U
#define PORT_PCR_IES_SHIFT                       8U
#define PORT_PCR_IES_WIDTH                       1U
#define PORT_PCR_IES(x)                          (((uint32_t)(((uint32_t)(x))<<PORT_PCR_IES_SHIFT))&PORT_PCR_IES_MASK)
/** PORT PCR OER Bit Fields, Used to configure register output enable or not. */
#define PORT_PCR_OER_MASK                        0x200U
#define PORT_PCR_OER_SHIFT                       9U
#define PORT_PCR_OER_WIDTH                       1U
#define PORT_PCR_OER(x)                          (((uint32_t)(((uint32_t)(x))<<PORT_PCR_OER_SHIFT))&PORT_PCR_OER_MASK)
/** PORT PCR OES Bit Fields, Used to select output function configuration or output enable configuration. */
#define PORT_PCR_OES_MASK                        0x400U
#define PORT_PCR_OES_SHIFT                       10U
#define PORT_PCR_OES_WIDTH                       1U
#define PORT_PCR_OES(x)                          (((uint32_t)(((uint32_t)(x))<<PORT_PCR_OES_SHIFT))&PORT_PCR_OES_MASK)

/** GPIO DR Bit Fields. */
#define GPIO_DR_MASK                             0xFFFFFFFFU
#define GPIO_DR_SHIFT                            0U
#define GPIO_DR_WIDTH                            32U
#define GPIO_DR(x)                               (((uint32_t)(((uint32_t)(x))<<GPIO_DR_SHIFT))&GPIO_DR_MASK)
/** GPIO DIR Bit Fields. */
#define GPIO_DIR_MASK                            0xFFFFFFFFU
#define GPIO_DIR_SHIFT                           0U
#define GPIO_DIR_WIDTH                           32U
#define GPIO_DIR(x)                              (((uint32_t)(((uint32_t)(x))<<GPIO_DIR_SHIFT))&GPIO_DIR_MASK)
/** GPIO PSR Bit Fields. */
#define GPIO_PSR_PS_MASK                         0xFFFFFFFFU
#define GPIO_PSR_PS_SHIFT                        0U
#define GPIO_PSR_PS_WIDTH                        32U
#define GPIO_PSR_PS(x)                           (((uint32_t)(((uint32_t)(x))<<GPIO_PSR_PS_SHIFT))&GPIO_PSR_PS_MASK)
/** GPIO ICR1 Bit Fields. */
#define GPIO_ICR1_IC_MASK                        0xFFFFFFFFU
#define GPIO_ICR1_IC_SHIFT                       0U
#define GPIO_ICR1_IC_WIDTH                       32U
#define GPIO_ICR1_IC(x)                          (((uint32_t)(((uint32_t)(x))<<GPIO_ICR1_IC_SHIFT))&GPIO_ICR1_IC_MASK)
/** GPIO ICR2 Bit Fields. */
#define GPIO_ICR2_IC_MASK                        0xFFFFFFFFU
#define GPIO_ICR2_IC_SHIFT                       0U
#define GPIO_ICR2_IC_WIDTH                       32U
#define GPIO_ICR2_IC(x)                          (((uint32_t)(((uint32_t)(x))<<GPIO_ICR2_IC_SHIFT))&GPIO_ICR2_IC_MASK)
/** GPIO IMR Bit Fields. */
#define GPIO_IMR_IM_MASK                         0xFFFFFFFFU
#define GPIO_IMR_IM_SHIFT                        0U
#define GPIO_IMR_IM_WIDTH                        32U
#define GPIO_IMR_IM(x)                           (((uint32_t)(((uint32_t)(x))<<GPIO_IMR_IM_SHIFT))&GPIO_IMR_IM_MASK)
/** GPIO ISR Bit Fields. */
#define GPIO_ISR_IS_MASK                         0xFFFFFFFFU
#define GPIO_ISR_IS_SHIFT                        0U
#define GPIO_ISR_IS_WIDTH                        32U
#define GPIO_ISR_IS(x)                           (((uint32_t)(((uint32_t)(x))<<GPIO_ISR_IS_SHIFT))&GPIO_ISR_IS_MASK)
/** GPIO EDGE_SEL Bit Fields. */
#define GPIO_EDGE_SEL_ES_MASK                    0xFFFFFFFFU
#define GPIO_EDGE_SEL_ES_SHIFT                   0U
#define GPIO_EDGE_SEL_ES_WIDTH                   32U
#define GPIO_EDGE_SEL_ES(x)                      (((uint32_t)(((uint32_t)(x))<<GPIO_EDGE_SEL_ES_SHIFT))&GPIO_EDGE_SEL_ES_MASK)
/** GPIO DEB_SEL Bit Fields. */
#define GPIO_DEB_SEL_DS_MASK                     0xFFFFFFFFU
#define GPIO_DEB_SEL_DS_SHIFT                    0U
#define GPIO_DEB_SEL_DS_WIDTH                    32U
#define GPIO_DEB_SEL_DS(x)                       (((uint32_t)(((uint32_t)(x))<<GPIO_DEB_SEL_DS_SHIFT))&GPIO_DEB_SEL_DS_MASK)
/** GPIO DMA_EN Bit Fields. */
#define GPIO_DMA_EN_MASK                         0xFFFFFFFFU
#define GPIO_DMA_EN_SHIFT                        0U
#define GPIO_DMA_EN_WIDTH                        32U
#define GPIO_DMA_EN(x)                           (((uint32_t)(((uint32_t)(x))<<GPIO_DIR_SHIFT))&GPIO_DIR_MASK)
/** GPIO DMA_SEL Bit Fields. */
#define GPIO_DMA_SEL_MASK                        0x7FU
#define GPIO_DMA_SEL_SHIFT                       0U
#define GPIO_DMA_SEL_WIDTH                       7U
#define GPIO_DMA_SEL(x)                          (((uint32_t)(((uint32_t)(x))<<GPIO_DIR_SHIFT))&GPIO_DIR_MASK)

/** GPIO pin value get and set. */
#define GPIO_PIN_VALUE_GET(port, Type)           ((uint32_t)((pins_type_t *)PINMUX_BASE)->Type[(uint32_t)port])
#define GPIO_PIN_VALUE_SET(port, Type)           (((pins_type_t *)PINMUX_BASE)->Type[(uint32_t)port])
/** GPIO ICR1 value get and set. */
#define GPIO_ICR1_VALUE_GET(port)                ((uint32_t)((pins_type_t *)PINMUX_BASE)->ICR[((uint32_t)2 * port)])
#define GPIO_ICR1_VALUE_SET(port)                (((pins_type_t *)PINMUX_BASE)->ICR[((uint32_t)2 * port)])
/** GPIO ICR2 value get and set. */
#define GPIO_ICR2_VALUE_GET(port)                ((uint32_t)((pins_type_t *)PINMUX_BASE)->ICR[((uint32_t)2 * port + 1)])
#define GPIO_ICR2_VALUE_SET(port)                (((pins_type_t *)PINMUX_BASE)->ICR[((uint32_t)2 * port + 1)])
/** GPIO PDMA Channel value get and set. */
#define GPIO_DMA_SEL_CHANNEL_GET(channel)        ((uint32_t)((pins_type_t *)PINMUX_BASE)->DMA_SEL[((uint32_t)channel)])
#define GPIO_DMA_SEL_CHANNEL_SET(channel)        (((pins_type_t *)PINMUX_BASE)->DMA_SEL[((uint32_t)channel)])

/**
 * @}
 */ /* end of group PINMUX_Register_Masks */

/**
 * @}
 */ /* end of group PINMUX_Access_Layer */
// clang-format on

#endif /* __PINS_REGISTER_H__ */
