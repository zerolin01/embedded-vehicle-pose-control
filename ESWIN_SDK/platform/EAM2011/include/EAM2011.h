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
 * @file EAM2011.h
 * @brief Peripheral Access Layer for EAM2011
 * @author abu (abu@eswincomputing.com)
 * @date 2023-03-24
 * 
 * Modification History :
 * Date:               Version:                    Author:     
 * Changes: 
 * 
 */

/* ----------------------------------------------------------------------------
   -- MCU activation
   ---------------------------------------------------------------------------- */

/* Prevention from multiple including the same memory map */
#if !defined(__EAM2011_H__) /* Check if memory map has not been already included */
#define __EAM2011_H__

#ifndef __ASSEMBLY__
#include <stdint.h>
#endif

#include "EAM2011_TCSR.h"
#include "EAM2011_irqn.h"
#include "EAM2011_memory_map.h"

/* ----------------------------------------------------------------------------
  -- Peripheral Access Layer for EAM2011
  ---------------------------------------------------------------------------- */

/**
 * @addtogroup Peripheral_Access_Layer_EAM2011 Peripheral Access Layer for EAM2011
 * @{
 */

/* ----------------------------------------------------------------------------
   -- ADMA Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup ADMA_Peripheral_Access_Layer ADMA Peripheral Access Layer
 * @{
 */

/** Number of instances of the ADMA module. */
#define ADMA_INSTANCE_COUNT (1U)

/** ADMA - Size of Registers Arrays */
#define ADMA_CH_COUNT (8U)

/* ADMA - Peripheral instance base addresses */
/** Peripheral ADMA base address */
#define ADMA_BASE (ADMA_BASEADDR)
/** Peripheral ADMA base pointer */
#define ADMA ((adma_type_t *)ADMA_BASE)
/** Array initializer of ADC peripheral base addresses */
#define ADMA_BASE_ADDRS \
    {                   \
        ADMA_BASE       \
    }
/** Array initializer of ADC peripheral base pointers */
#define ADMA_BASE_PTRS \
    {                  \
        ADMA           \
    }
/** Number of interrupt vector arrays for the ADC module. */
#define ADMA_IRQS_ARR_COUNT (2U)
/** Number of interrupt channels for the ADC module. */
#define ADMA_IRQS_CH_COUNT (2U)
/** Interrupt vectors for the ADC peripheral type */
#define ADMA_IRQS                  \
    {                              \
        ADC0_Tx_IRQn, ADC1_Tx_IRQn \
    }

/**
 * @}
 */
/* end of group ADMA_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- ADC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup ADC_Peripheral_Access_Layer ADC Peripheral Access Layer
 * @{
 */

/** Number of instances of the ADC module. */
#define ADC_INSTANCE_COUNT (2U)

/** ADC - Size of Registers Arrays */
#define ADC_SC1_COUNT (24U)
#define ADC_R_COUNT   (24U)
#define ADC_CV_COUNT  (2U)

/* ADC - Peripheral instance base addresses */
/** Peripheral ADC0 base address */
#define ADC0_BASE (ADC_0_BASEADDR)
/** Peripheral ADC0 base pointer */
#define ADC0 ((adc_type_t *)ADC0_BASE)
/** Peripheral ADC1 base address */
#define ADC1_BASE (ADC_1_BASEADDR)
/** Peripheral ADC1 base pointer */
#define ADC1 ((adc_type_t *)ADC1_BASE)
/** Array initializer of ADC peripheral base addresses */
#define ADC_BASE_ADDRS       \
    {                        \
        ADC0_BASE, ADC1_BASE \
    }
/** Array initializer of ADC peripheral base pointers */
#define ADC_BASE_PTRS \
    {                 \
        ADC0, ADC1    \
    }
/** Number of interrupt vector arrays for the ADC module. */
#define ADC_IRQS_ARR_COUNT (1U)
/** Number of interrupt channels for the ADC module. */
#define ADC_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the ADC peripheral type */
#define ADC_IRQS             \
    {                        \
        ADC0_IRQn, ADC1_IRQn \
    }

/**
 * @}
 */
/* end of group ADC_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- CMP Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CMP_Peripheral_Access_Layer CMP Peripheral Access Layer
 * @{
 */

/** Number of instances of the CMP module. */
#define CMP_INSTANCE_COUNT (1U)

/* CMP - Peripheral instance base addresses */
/** Peripheral CMP0 base address */
#define CMP0_BASE (CMP_0_BASEADDR)
/** Peripheral CMP0 base pointer */
#define CMP0 ((cmp_type_t *)CMP0_BASE)
/** Array initializer of CMP peripheral base addresses */
#define CMP_BASE_ADDRS \
    {                  \
        CMP0_BASE      \
    }
/** Array initializer of CMP peripheral base pointers */
#define CMP_BASE_PTRS \
    {                 \
        CMP0          \
    }
/** Number of interrupt vector arrays for the CMP module. */
#define CMP_IRQS_ARR_COUNT (1U)
/** Number of interrupt channels for the CMP module. */
#define CMP_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the CMP peripheral type */
#define CMP_IRQS  \
    {             \
        CMP0_IRQn \
    }

/**
 * @}
 */
/* end of group DAC_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- DAC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup DAC_Peripheral_Access_Layer DAC Peripheral Access Layer
 * @{
 */

/** Number of instances of the DAC module. */
#define DAC_INSTANCE_COUNT (1U)

/* DAC - Peripheral instance base addresses */
/** Peripheral DAC0 base address */
#define DAC0_BASE (DAC_0_BASEADDR)
/** Peripheral DAC0 base pointer */
#define DAC0 ((dac_type_t *)DAC0_BASE)
/** Array initializer of DAC peripheral base addresses */
#define DAC_BASE_ADDRS \
    {                  \
        DAC_BASE       \
    }
/** Array initializer of DAC peripheral base pointers */
#define DAC_BASE_PTRS \
    {                 \
        DAC0          \
    }
/** Number of interrupt vector arrays for the DAC module. */
#define DAC_IRQS_ARR_COUNT (1U)
/** Number of interrupt channels for the DAC module. */
#define DAC_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the DAC peripheral type */
#define DAC_IRQS  \
    {             \
        DAC0_IRQn \
    }

/**
 * @}
 */
/* end of group DAC_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- CRC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CRC_Peripheral_Access_Layer CRC Peripheral Access Layer
 * @{
 */

/** Number of instances of the CRC module. */
#define CRC_INSTANCE_COUNT (1U)

/* CRC - Peripheral instance base addresses */
/** Peripheral CRC base address */
#define CRC_BASE (CRC_BASEADDR)
/** Peripheral CRC base pointer */
#define CRC ((crc_type_t *)CRC_BASE)
/** Array initializer of CRC peripheral base addresses */
#define CRC_BASE_ADDRS \
    {                  \
        CRC_BASE       \
    }
/** Array initializer of CRC peripheral base pointers */
#define CRC_BASE_PTRS \
    {                 \
        CRC           \
    }

/**
 * @}
 */
/* end of group CRC_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
  -- CRG Peripheral Access Layer
  ---------------------------------------------------------------------------- */

/**
 * @addtogroup CRG_Peripheral_Access_Layer CRG Peripheral Access Layer
 * @{
 */

/** Number of instances of the CRG module. */
#define CRG_INSTANCE_COUNT (1U)

/* CRG - Peripheral instance base addresses */
/** Peripheral CRG base address */
#define CRG_BASE (CRG_BASEADDR)
/** Peripheral CRG base pointer */
#define CRG ((crg_type_t *)CRG_BASE)
/** Array initializer of CRG peripheral base addresses */
#define CRG_BASE_ADDRS \
    {                  \
        CRG_BASE       \
    }
/** Array initializer of CRG peripheral base pointers */
#define CRG_BASE_PTRS \
    {                 \
        CRG           \
    }

/** Peripheral XTAL base address */
#define XTAL_BASE (PINMUX_BASEADDR + 0x2B0)
/** Peripheral CRG base pointer */
#define XTAL ((xtal_type_t *)XTAL_BASE)

/**
 * @}
 */
/* end of group CRG_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- PDMA Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PDMA_Peripheral_Access_Layer PDMA Peripheral Access Layer
 * @{
 */

/** PDMA - Size of Registers Arrays */
#define PDMA_DCHPRI_COUNT (32U)
#define PDMA_TCD_COUNT    (32U)

/** Number of instances of the PDMA module. */
#define PDMA_INSTANCE_COUNT (1U)

/* PDMA - Peripheral instance base addresses */
/** Peripheral PDMA base address */
#define PDMA_BASE (PDMA_BASEADDR)
/** Peripheral PDMA base pointer */
#define PDMA ((pdma_type_t *)PDMA_BASE)
/** Array initializer of PDMA peripheral base addresses */
#define PDMA_BASE_ADDRS \
    {                   \
        PDMA_BASE       \
    }
/** Array initializer of PDMA peripheral base pointers */
#define PDMA_BASE_PTRS \
    {                  \
        PDMA           \
    }
/** Number of interrupt vector arrays for the PDMA module. */
#define PDMA_IRQS_ARR_COUNT (2U)
/** Number of interrupt channels for the CHN type of PDMA module. */
#define PDMA_CHN_IRQS_CH_COUNT (32U)
/** Number of interrupt channels for the ERROR type of PDMA module. */
#define PDMA_ERROR_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the PDMA peripheral type */
#define PDMA_CHN_IRQS                                                                                                           \
    {                                                                                                                           \
        PDMA0_IRQn, PDMA1_IRQn, PDMA2_IRQn, PDMA3_IRQn, PDMA4_IRQn, PDMA5_IRQn, PDMA6_IRQn, PDMA7_IRQn, PDMA8_IRQn, PDMA9_IRQn, \
            PDMA10_IRQn, PDMA11_IRQn, PDMA12_IRQn, PDMA13_IRQn, PDMA14_IRQn, PDMA15_IRQn, PDMA16_IRQn, PDMA17_IRQn,             \
            PDMA18_IRQn, PDMA19_IRQn, PDMA20_IRQn, PDMA21_IRQn, PDMA22_IRQn, PDMA23_IRQn, PDMA24_IRQn, PDMA25_IRQn,             \
            PDMA26_IRQn, PDMA27_IRQn, PDMA28_IRQn, PDMA29_IRQn, PDMA30_IRQn, PDMA31_IRQn,                                       \
    }
#define PDMA_ERROR_IRQS \
    {                   \
        PDMA_Error_IRQn \
    }

/**
 * @}
 */
/* end of group PDMA_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- DMAMUX Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup DMAMUX_Peripheral_Access_Layer DMAMUX Peripheral Access Layer
 * @{
 */

/** DMAMUX - Size of Registers Arrays */
#define DMAMUX_CHCFG_COUNT (32U)

/** Number of instances of the DMAMUX module. */
#define DMAMUX_INSTANCE_COUNT (1U)

/* DMAMUX - Peripheral instance base addresses */
/** Peripheral DMAMUX base address */
#define DMAMUX_BASE (DMAMUX_BASEADDR)
/** Peripheral DMAMUX base pointer */
#define DMAMUX ((dmamux_type_t *)DMAMUX_BASE)
/** Array initializer of DMAMUX peripheral base addresses */
#define DMAMUX_BASE_ADDRS \
    {                     \
        DMAMUX_BASE       \
    }
/** Array initializer of DMAMUX peripheral base pointers */
#define DMAMUX_BASE_PTRS \
    {                    \
        DMAMUX           \
    }

/**
 * @}
 */
/* end of group DMAMUX_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- PINMUX Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PINMUX_Access_Layer PINMUX Access Layer
 * @{
 */

/** Number of Pinmux PORT. */
#define PINMUX_PORT_COUNT (4U)

/** Size of Pinmux PORT Control Register. */
#define PINMUX_PORT_PCR_COUNT (32U)

/** Pinmux base address */
#define PINMUX_BASE (PINMUX_BASEADDR)
/** Pinmux PORTA offset */
#define PORTA (0U)
/** Pinmux PTA offset */
#define PTA (0U)
/** Pinmux GPIOA offset */
#define GPIOA (0U)

/** Pinmux PORTB offset */
#define PORTB (1U)
/** Pinmux PTB offset */
#define PTB (1U)
/** Pinmux GPIOB offset */
#define GPIOB (1U)

/** Pinmux PORTC offset */
#define PORTC (2U)
/** Pinmux PTC offset */
#define PTC (2U)
/** Pinmux GPIOC offset */
#define GPIOC (2U)

/** Pinmux PORTD offset */
#define PORTD (3U)
/** Pinmux PTD offset */
#define PTD (3U)
/** Pinmux GPIOD offset */
#define GPIOD (3U)

/** Pinmux PORTE offset */
#define PORTE (4U)
/** Pinmux PTE offset */
#define PTE (4U)
/** Pinmux GPIOE offset */
#define GPIOE (4U)

/** Array initializer of PORT */
#define PORT_ARRAY                        \
    {                                     \
        PORTA, PORTB, PORTC, PORTD, PORTE \
    }

/**
 * @}
 */
/* end of group PINMUX_Access_Layer */

/* ----------------------------------------------------------------------------
   -- PDU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PDU_Peripheral_Access_Layer PDU Peripheral Access Layer
 * @{
 */

/** PDU - Size of Registers Arrays */

#define PDU_CH_COUNT     4u
#define PDU_DLY_COUNT    8u
#define PDU_POnDLY_COUNT 8u

/** Number of instances of the PDU module. */
#define PDU_INSTANCE_COUNT (2U)

/* PDU - Peripheral instance base addresses */
/** Peripheral PDU0 base address */
#define PDU0_BASE (PDU_0_BASEADDR)
/** Peripheral PDU0 base pointer */
#define PDU0 ((pdu_type_t *)PDU0_BASE)
/** Peripheral PDU1 base address */
#define PDU1_BASE (PDU_1_BASEADDR)
/** Peripheral PDU1 base pointer */
#define PDU1 ((pdu_type_t *)PDU1_BASE)
/** Array initializer of PDU peripheral base addresses */
#define PDU_BASE_ADDRS       \
    {                        \
        PDU0_BASE, PDU1_BASE \
    }
/** Array initializer of PDU peripheral base pointers */
#define PDU_BASE_PTRS \
    {                 \
        PDU0, PDU1    \
    }
/** Number of interrupt vector arrays for the PDU module. */
#define PDU_IRQS_ARR_COUNT (1U)
/** Number of interrupt channels for the PDU module. */
#define PDU_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the PDU peripheral type */
#define PDU_IRQS             \
    {                        \
        PDU0_IRQn, PDU1_IRQn \
    }

/** Interrupt vectors for the PDU Sequence Error Interrupt peripheral type */
#define PDU_ERR_IRQS                 \
    {                                \
        PDU0_ERR_IRQn, PDU1_ERR_IRQn \
    }

/**
 * @}
 */
/* end of group PDU_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- PMU Peripheral Access Layer
---------------------------------------------------------------------------- */

/**
 * @addtogroup PMU_Peripheral_Access_Layer PMU Peripheral Access Layer
 * @{
 */

/** Number of instances of the PMU module. */
#define PMU_INSTANCE_COUNT (1U)

/* PMU - Peripheral instance base addresses */
/** Peripheral PMU base address */
#define PMU_BASE (PMU_BASEADDR)
/** Peripheral PMU base pointer */
#define PMU ((pmu_type_t *)PMU_BASE)
/** Array initializer of PMU peripheral base addresses */
#define PMU_BASE_ADDRS \
    {                  \
        PMU_BASE       \
    }
/** Array initializer of PMU peripheral base pointers */
#define PMU_BASE_PTRS \
    {                 \
        PMU           \
    }
/** Interrupt vectors for the PMU peripheral type */
#define PMU_PWRUP_IRQS \
    {                  \
        POWERUP_IRQn   \
    }
#define PMU_PVD_IRQS  \
    {                 \
        PMU_VMON_IRQn \
    }
#define PMU_BOR_IRQS \
    {                \
        PMU_BOR_IRQn \
    }
#define PMU_REBOOT_FAIL_IRQS \
    {                        \
        BOOT_TIMEOUT_IRQn    \
    }

/**
 * @}
 */
/* end of group PMU_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- IOPMP Peripheral Access Layer
---------------------------------------------------------------------------- */

/**
 * @addtogroup IOPMP_Peripheral_Access_Layer IOPMP Peripheral Access Layer
 * @{
 */

/** Number of instances of the IOPMP module. */
#define IOPMP_INSTANCE_COUNT (1U)

#define IOPMP_MASTER_COUNT (4U)
#define IOPMP_REGION_COUNT (127U)

/* IOPMP - Peripheral instance base addresses */
/** Peripheral IOPMP base address */
#define IOPMP_BASE (IOPMP_BASEADDR)
/** Peripheral PMU base pointer */
#define IOPMP ((iopmp_type_t *)IOPMP_BASE)
/** Array initializer of IOPMP peripheral base addresses */
#define IOPMP_BASE_ADDRS \
    {                    \
        IOPMP_BASE       \
    }
/** Array initializer of IOPMP peripheral base pointers */
#define IOPMP_BASE_PTRS \
    {                   \
        IOPMP           \
    }
/** Interrupt vectors for the IOPMP peripheral type */
#define IOPMP_IRQS \
    {              \
        IOPMP_IRQn \
    }

/**
 * @}
 */
/* end of group IOPMP_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- UART Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup UART_Peripheral_Access_Layer UART Peripheral Access Layer
 * @{
 */

/** UART - Size of Registers Arrays */

/** Number of instances of the UART module. */
#define UART_INSTANCE_COUNT (6U)

/* UART - Peripheral instance base addresses */
/** Peripheral UART base address */
#define UART_BASE(n) (UART_0_BASEADDR + (n)*UART_0_SIZE)
/** Peripheral UART base pointer */
#define UART(n) ((uart_type_t *)UART_BASE(n))

/** Array initializer of UART peripheral base addresses */
#define UART_BASE_ADDRS                                                                    \
    {                                                                                      \
        UART_BASE(0), UART_BASE(1), UART_BASE(2), UART_BASE(3), UART_BASE(4), UART_BASE(5) \
    }
/** Array initializer of UART peripheral base pointers */
#define UART_BASE_PTRS                                       \
    {                                                        \
        UART(0), UART(1), UART(2), UART(3), UART(4), UART(5) \
    }
/** Number of interrupt vector arrays for the UART module. */
#define UART_IRQS_ARR_COUNT (1U)
/** Number of interrupt channels for the RX_TX type of UART module. */
#define UART_RX_TX_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the UART peripheral type */
#define UART_RX_IRQS                                                                             \
    {                                                                                            \
        UART0_Rx_IRQn, UART1_Rx_IRQn, UART2_Rx_IRQn, UART3_Rx_IRQn, UART4_Rx_IRQn, UART5_Rx_IRQn \
    }
#define UART_TX_IRQS                                                                             \
    {                                                                                            \
        UART0_Tx_IRQn, UART1_Tx_IRQn, UART2_Tx_IRQn, UART3_Tx_IRQn, UART4_Tx_IRQn, UART5_Tx_IRQn \
    }
/**
 * @}
 */
/* end of group UART_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- SPI Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup SPI_Peripheral_Access_Layer SPI Peripheral Access Layer
 * @{
 */

/** Number of instances of the SPI module. */
#define SPI_INSTANCE_COUNT (4U)
/* SPI - Peripheral instance base addresses */
/** Peripheral SPI0 base address */
#define SPI0_BASE (SPI_0_BASEADDR)
/** Peripheral SPI0 base pointer */
#define SPI0 ((spi_type_t *)SPI0_BASE)
/** Peripheral SPI1 base address */
#define SPI1_BASE (SPI_1_BASEADDR)
/** Peripheral SPI1 base pointer */
#define SPI1 ((spi_type_t *)SPI1_BASE)
/** Peripheral SPI2 base address */
#define SPI2_BASE (SPI_2_BASEADDR)
/** Peripheral SPI2 base pointer */
#define SPI2 ((spi_type_t *)SPI2_BASE)
/** Peripheral SPI3 base address */
#define SPI3_BASE (SPI_3_BASEADDR)
/** Peripheral SPI2 base pointer */
#define SPI3 ((spi_type_t *)SPI3_BASE)

/** Array initializer of SPI peripheral base addresses */
#define SPI_BASE_ADDRS                             \
    {                                              \
        SPI0_BASE, SPI1_BASE, SPI2_BASE, SPI3_BASE \
    }
/** Array initializer of SPI peripheral base pointers */
#define SPI_BASE_PTRS          \
    {                          \
        SPI0, SPI1, SPI2, SPI3 \
    }
/** Interrupt vectors for the SPI peripheral type */
#define SPI_IRQS                                                                \
    {                                                                           \
        SPI0_IRQn, SPI1_IRQn, SPI2_IRQn, SPI3_IRQn,                             \
            SPI0_ASYNC_IRQn, SPI1_ASYNC_IRQn, SPI2_ASYNC_IRQn, SPI3_ASYNC_IRQn, \
    }

/**
 * @}
 */
/* end of group SPI_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- PWM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PWM_Peripheral_Access_Layer PWM Peripheral Access Layer
 * @{
 */

/** Number of instances of the PWM module. */
#define PWM_INSTANCE_COUNT (5U)
/* PWM - Peripheral instance base addresses */
/** Peripheral PWM base address */
#define PWM0_BASE (PWM_0_BASEADDR)
/** Peripheral PWM base pointer */
#define PWM0 ((pwm_type_t *)PWM0_BASE)
/** Peripheral PWM base address */
#define PWM1_BASE (PWM_1_BASEADDR)
/** Peripheral PWM base pointer */
#define PWM1 ((pwm_type_t *)PWM1_BASE)
/** Peripheral PWM base address */
#define PWM2_BASE (PWM_2_BASEADDR)
/** Peripheral PWM base pointer */
#define PWM2 ((pwm_type_t *)PWM2_BASE)
/** Peripheral PWM base address */
#define PWM3_BASE (PWM_3_BASEADDR)
/** Peripheral PWM base pointer */
#define PWM3 ((pwm_type_t *)PWM3_BASE)
/** Peripheral PWM base address */
#define PWM4_BASE (PWM_4_BASEADDR)
/** Peripheral PWM base pointer */
#define PWM4 ((pwm_type_t *)PWM4_BASE)
/** Array initializer of PWM peripheral base pointers */
#define PWM_BASE_PTRS                \
    {                                \
        PWM0, PWM1, PWM2, PWM3, PWM4 \
    }
/** Interrupt vectors for the PWM peripheral type */
#define PWM_IRQS                                              \
    {                                                         \
        PWM0_IRQn, PWM1_IRQn, PWM2_IRQn, PWM3_IRQn, PWM4_IRQn \
    }

/**
 * @}
 */
/* end of group PWM_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- TRGMUX and TMRMUX Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup TRGMUX_and_TMRMUX_Peripheral_Access_Layer TRGMUX/TMRMUX Peripheral Access Layer
 * @{
 */

/** Number of instances of the TRGMUX module. */
#define TRGMUX_INSTANCE_COUNT (2U)

/* TMRMUX - Peripheral instance base addresses */
/** Peripheral TMRMUX base address */
#define TMRMUX_BASE (TMRMUX_BASEADDR)
/** Peripheral TMRMUX base pointer */
#define TMRMUX ((trgmux_type_t *)TMRMUX_BASE)

/* TRGMUX - Peripheral instance base addresses */
/** Peripheral TRGMUX base address */
#define TRGMUX_BASE (TRGMUX_BASEADDR)
/** Peripheral TRGMUX base pointer */
#define TRGMUX ((trgmux_type_t *)TRGMUX_BASE)

/** Array initializer of TRGMUX peripheral base pointers */
#define TRGMUX_BASE_PTRS \
    {                    \
        TRGMUX, TMRMUX   \
    }

/**
 * @}
 */
/* end of group TRGMUX_and_TMRMUX_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- PITMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PITMR_Peripheral_Access_Layer PITMR Peripheral Access Layer
 * @{
 */

/** PITMR - Size of Registers Arrays Motor PITMR*/
#define PITMR0_TMR_COUNT (4U)

/** PITMR - Size of Registers Arrays PDMA PITMR*/
#define PITMR1_TMR_COUNT (10U)

/** Number of instances of the PITMR module. */
#define PITMR_INSTANCE_COUNT (2U)

/* PITMR - Peripheral instance base addresses */
/** Peripheral PITMR0 base address */
#define PITMR0_BASE (PITMR_BASEADDR)
/** Peripheral PITMR0 base pointer */
#define PITMR0 ((pitmr_type_t *)PITMR0_BASE)

/** Peripheral PITMR1 base address */
#define PITMR1_BASE (DMAPITMR_BASEADDR)
#define PITMR1      ((pitmr_type_t *)PITMR1_BASE)

/** Array initializer of PITMR peripheral base addresses */
#define PITMR_BASE_ADDRS         \
    {                            \
        PITMR0_BASE, PITMR1_BASE \
    }

/** Array initializer of PITMR peripheral base pointers */
#define PITMR_BASE_PTRS \
    {                   \
        PITMR0, PITMR1  \
    }

/** Number of interrupt vector arrays for the PITMR module. */
#define PITMR_IRQS_ARR_COUNT (2U)

/** Number of interrupt channels for the PITMR0 module. */
#define PITMR0_IRQS_CH_COUNT (4U)

/** Number of interrupt channels for the PITMR1 module. */
#define PITMR1_IRQS_CH_COUNT (10U)

/** Interrupt vectors for the PITMR0 peripheral type */
#define PITMR_IRQS                                                                                                \
    {                                                                                                             \
        PITMR0_Ch0_IRQn, PITMR0_Ch1_IRQn, PITMR0_Ch2_IRQn, PITMR0_Ch3_IRQn, PITMR1_Ch0_IRQn, PITMR1_Ch1_IRQn,     \
            PITMR1_Ch2_IRQn, PITMR1_Ch3_IRQn, PITMR1_Ch4_IRQn, PITMR1_Ch5_IRQn, PITMR1_Ch6_IRQn, PITMR1_Ch7_IRQn, \
            PITMR1_Ch8_IRQn, PITMR1_Ch9_IRQn                                                                      \
    }

/**
 * @}
 */
/* end of group PITMR_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- PCTMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PCTMR_Peripheral_Access_Layer PCTMR Peripheral Access Layer
 * @{
 */

/* PCTMR - Peripheral instance base addresses */
/** Peripheral PCTMR0 base address */
#define PCTMR0_BASE (PCTMR0_BASEADDR)
/** Peripheral PCTMR0 base pointer */
#define PCTMR0 ((pctmr_type_t *)PCTMR0_BASE)

/** Peripheral PCTMR1 base address */
#define PCTMR1_BASE (PCTMR1_BASEADDR)
/** Peripheral PCTMR1 base pointer */
#define PCTMR1 ((pctmr_type_t *)PCTMR1_BASE)

/** Peripheral PCTMR2 base address */
#define PCTMR2_BASE (PCTMR2_BASEADDR)
/** Peripheral PCTMR2 base pointer */
#define PCTMR2 ((pctmr_type_t *)PCTMR2_BASE)

/** Peripheral PCTMR3 base address */
#define PCTMR3_BASE (PCTMR3_BASEADDR)
/** Peripheral PCTMR3 base pointer */
#define PCTMR3 ((pctmr_type_t *)PCTMR3_BASE)

/** Peripheral PCTMR4 base address */
#define PCTMR4_BASE (PCTMR4_BASEADDR)
/** Peripheral PCTMR4 base pointer */
#define PCTMR4 ((pctmr_type_t *)PCTMR4_BASE)

/** Peripheral PCTMR5 base address */
#define PCTMR5_BASE (PCTMR5_BASEADDR)
/** Peripheral PCTMR5 base pointer */
#define PCTMR5 ((pctmr_type_t *)PCTMR5_BASE)

/** Peripheral PCTMR6 base address */
#define PCTMR6_BASE (PCTMR6_BASEADDR)
/** Peripheral PCTMR6 base pointer */
#define PCTMR6 ((pctmr_type_t *)PCTMR6_BASE)

/** Peripheral PCTMR7 base address */
#define PCTMR7_BASE (PCTMR7_BASEADDR)
/** Peripheral PCTMR7 base pointer */
#define PCTMR7 ((pctmr_type_t *)PCTMR7_BASE)

/** Peripheral PCTMR8 base address */
#define PCTMR8_BASE (PCTMR8_BASEADDR)
/** Peripheral PCTMR8 base pointer */
#define PCTMR8 ((pctmr_type_t *)PCTMR8_BASE)

/**
 * @}
 */
/* end of group PCTMR_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- PCTMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PCTMR_Peripheral_Access_Layer PCTMR Peripheral Access Layer
 * @{
 */

/** Number of instances of the PCTMR module. */
#define PCTMR_INSTANCE_COUNT (9U)

/** Array initializer of PCTMR peripheral base addresses */
#define PCTMR_BASE_ADDRS                                                                                        \
    {                                                                                                           \
        PCTMR0_BASE, PCTMR1_BASE, PCTMR2_BASE, PCTMR3_BASE, PCTMR4_BASE, PCTMR5_BASE, PCTMR6_BASE, PCTMR7_BASE, \
            PCTMR8_BASE                                                                                         \
    }
/** Array initializer of PCTMR peripheral base pointers */
#define PCTMR_BASE_PTRS                                                        \
    {                                                                          \
        PCTMR0, PCTMR1, PCTMR2, PCTMR3, PCTMR4, PCTMR5, PCTMR6, PCTMR7, PCTMR8 \
    }
/** Number of interrupt vector arrays for the PCTMR module. */
#define PCTMR_IRQS_ARR_COUNT (9U)
/** Number of interrupt channels for the PCTMR module. */
#define PCTMR_IRQS_CH_COUNT (9U)
/** Interrupt vectors for the PCTMR peripheral type */
#define PCTMR_IRQS                                                                                              \
    {                                                                                                           \
        PCTMR0_IRQn, PCTMR1_IRQn, PCTMR2_IRQn, PCTMR3_IRQn, PCTMR4_IRQn, PCTMR5_IRQn, PCTMR6_IRQn, PCTMR7_IRQn, \
            PCTMR8_IRQn                                                                                         \
    }
/**
 * @}
 */
/* end of group PCTMR_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- RTC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup RTC_Peripheral_Access_Layer RTC Peripheral Access Layer
 * @{
 */

/** Number of instances of the RTC module. */
#define RTC_INSTANCE_COUNT (1U)

/* RTC - Peripheral instance base addresses */

/** Peripheral RTC base address */
#define RTC_BASE (RTC_BASEADDR)
/** Peripheral RTC base pointer */
#define RTC ((rtc_type_t *)RTC_BASE)
/** Array initializer of RTC peripheral base addresses */
#define RTC_BASE_ADDRS \
    {                  \
        RTC_BASE       \
    }

/** Array initializer of RTC peripheral base pointers */
#define RTC_BASE_PTRS \
    {                 \
        RTC           \
    }

/** Interrupt vectors for the RTC peripheral type */
#define RTC_IRQS \
    {            \
        RTC_IRQn \
    }

/**
 * @}
 */
/* end of group RTC_Peripheral_Access_Layer */

/**
 * @addtogroup WAKEUP_INTC_Peripheral_Access_Layer WAKEUP_INTC Peripheral Access Layer
 * @{
 */

/* ----------------------------------------------------------------------------
   -- WAKEUP_INTC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/** Number of instances of the WAKEUP_INTC module. */
#define WAKEUP_INSTANCE_COUNT (1U)

/* WAKEUP - Peripheral instance base addresses */
/** Peripheral WAKEUP_INTC base address */
#define WAKEUP_BASE (WAKEUP_INTC_BASEADDR)
/** Peripheral WAKEUP_INTC base pointer */
#define WAKEUP ((wakeup_type_t *)WAKEUP_BASE)
/** Array initializer of WAKEUP_INTC peripheral base addresses */
#define WAKEUP_BASE_ADDRS \
    {                     \
        WAKEUP_BASE       \
    }
/** Array initializer of WAKEUP_INTC peripheral base pointers */
#define WAKEUP_BASE_PTRS \
    {                    \
        WAKEUP           \
    }
/** Number of interrupt vector arrays for the WAKEUP_INTC module. */
#define WAKEUP_IRQS_ARR_COUNT (1U)
/** Number of interrupt channels for the WAKEUP_INTC module. */
#define WAKEUP_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the WAKEUP_INTC peripheral type */
#define WAKEUP_IRQS \
    {               \
        WAKEUP_IRQn \
    }

/**
 * @}
 */
/* end of group WAKEUP_INTC_Peripheral_Access_Layer */

/**
 * @addtogroup WDOG_Peripheral_Access_Layer WDOG Peripheral Access Layer
 * @{
 */

/* ----------------------------------------------------------------------------
   -- WDOG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/** Number of instances of the WDOG module. */
#define WDOG_INSTANCE_COUNT (1U)

/* WDOG - Peripheral instance base addresses */
/** Peripheral WDOG base address */
#define WDOG_BASE (WDOG_BASEADDR)
/** Peripheral WDOG base pointer */
#define WDOG ((wdog_type_t *)WDOG_BASE)
/** Array initializer of WDOG peripheral base addresses */
#define WDOG_BASE_ADDRS \
    {                   \
        WDOG_BASE       \
    }
/** Array initializer of WDOG peripheral base pointers */
#define WDOG_BASE_PTRS \
    {                  \
        WDOG           \
    }
/** Number of interrupt vector arrays for the WDOG module. */
#define WDOG_IRQS_ARR_COUNT (1U)
/** Number of interrupt channels for the WDOG module. */
#define WDOG_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the WDOG peripheral type */
#define WDOG_IRQS \
    {             \
        WDOG_IRQn \
    }
#define WDOG_ASYNC_IRQS     \
    {                       \
        WDOG_EWM_ASYNC_IRQn \
    }

/**
 * @}
 */
/* end of group WDOG_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- EWM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup EWM_Peripheral_Access_Layer EWM Peripheral Access Layer
 * @{
 */

/** Number of instances of the WDOG module. */
#define EWM_INSTANCE_COUNT (1U)

/* EWM - Peripheral instance base addresses */
/** Peripheral EWM base address */
#define EWM_BASE (EWM_BASEADDR)
/** Peripheral EWM base pointer */
#define EWM ((ewm_type_t *)EWM_BASE)
/** Array initializer of EWM peripheral base addresses */
#define EWM_BASE_ADDRS \
    {                  \
        EWM_BASE       \
    }
/** Array initializer of EWM peripheral base pointers */
#define EWM_BASE_PTRS \
    {                 \
        EWM           \
    }
/** Number of interrupt vector arrays for the EWM module. */
#define EWM_IRQS_ARR_COUNT (1U)
/** Number of interrupt channels for the EWM module. */
#define EWM_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the EWM peripheral type */
#define EWM_IRQS \
    {            \
        EWM_IRQ  \
    }

/**
 * @}
 */
/* end of group EWM_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- GTMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup GTMR_Peripheral_Access_Layer GTMR Peripheral Access Layer
 * @{
 */

/** Number of instances of the GTMR module. */
#define GTMR_INSTANCE_COUNT (1U)

/* GTMR - Peripheral instance base addresses */
/** Peripheral WDOG base address */
#define GTMR_BASE (GTMR_BASEADDR)
/** Peripheral WDOG base pointer */
#define GTMR ((gtmr_type_t *)GTMR_BASE)
/** Array initializer of WDOG peripheral base addresses */
#define GTMR_BASE_ADDRS \
    {                   \
        GTMR_BASE       \
    }
/** Array initializer of WDOG peripheral base pointers */
#define GTMR_BASE_PTRS \
    {                  \
        GTMR           \
    }
/** Number of interrupt vector arrays for the WDOG module. */
#define GTMR_IRQS_ARR_COUNT (1U)
/** Number of interrupt channels for the WDOG module. */
#define GTMR_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the WDOG peripheral type */
#define GTMR_IRQS \
    {             \
        GTMR_IRQ  \
    }

/**
 * @}
 */
/* end of group GTMR_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- SUPERTMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup SUPERTMR_Peripheral_Access_Layer SUPERTMR Peripheral Access Layer
 * @{
 */

/** Number of instances of the SUPERTMR module. */
#define SUPERTMR_INSTANCE_COUNT (6U)
/* @brief Number of PWM channels */
#define SUPERTMR_CHANNEL_COUNT (8U)
/* @brief Number of fault channels */
#define SUPERTMR_FAULT_CHANNELS (4U)

/* SUPERTMR - Peripheral instance base addresses */
/** Peripheral SUPERTMR0 base address */
#define SUPERTMR0_BASE (SUPERTIMER_0_BASEADDR)
/** Peripheral SUPERTMR0 base pointer */
#define SUPERTMR0 ((supertmr_type_t *)SUPERTMR0_BASE)
/** Peripheral SUPERTMR1 base address */
#define SUPERTMR1_BASE (SUPERTIMER_1_BASEADDR)
/** Peripheral SUPERTMR1 base pointer */
#define SUPERTMR1 ((supertmr_type_t *)SUPERTMR1_BASE)
/** Peripheral SUPERTMR2 base address */
#define SUPERTMR2_BASE (SUPERTIMER_2_BASEADDR)
/** Peripheral SUPERTMR2 base pointer */
#define SUPERTMR2 ((supertmr_type_t *)SUPERTMR2_BASE)
/** Peripheral SUPERTMR3 base address */
#define SUPERTMR3_BASE (SUPERTIMER_3_BASEADDR)
/** Peripheral SUPERTMR3 base pointer */
#define SUPERTMR3 ((supertmr_type_t *)SUPERTMR3_BASE)
/** Peripheral SUPERTMR4 base address */
#define SUPERTMR4_BASE (SUPERTIMER_4_BASEADDR)
/** Peripheral SUPERTMR4 base pointer */
#define SUPERTMR4 ((supertmr_type_t *)SUPERTMR4_BASE)
/** Peripheral SUPERTMR5 base address */
#define SUPERTMR5_BASE (SUPERTIMER_5_BASEADDR)
/** Peripheral SUPERTMR5 base pointer */
#define SUPERTMR5 ((supertmr_type_t *)SUPERTMR5_BASE)
/** Array initializer of SUPERTMR peripheral base addresses */
#define SUPERTMR_BASE_ADDRS                                                                            \
    {                                                                                                  \
        SUPERTMR0_BASE, SUPERTMR1_BASE, SUPERTMR2_BASE, SUPERTMR3_BASE, SUPERTMR4_BASE, SUPERTMR5_BASE \
    }
/** Array initializer of SUPERTMR peripheral base pointers */
#define SUPERTMR_BASE_PTRS                                               \
    {                                                                    \
        SUPERTMR0, SUPERTMR1, SUPERTMR2, SUPERTMR3, SUPERTMR4, SUPERTMR5 \
    }
/** Number of interrupt vector arrays for the SUPERTMR module. */
#define SUPERTMR_IRQS_ARR_COUNT (6U)
/** Number of interrupt channels for the SUPERTMR module. */
#define SUPERTMR_IRQS_CH_COUNT (8U)
/** Number of interrupt channels for the Fault type of SUPERTMR module. */
#define SUPERTMR_Fault_IRQS_CH_COUNT (1U)
/** Number of interrupt channels for the Overflow type of SUPERTMR module. */
#define SUPERTMR_Overflow_IRQS_CH_COUNT (1U)
/** Number of interrupt channels for the Reload type of SUPERTMR module. */
#define SUPERTMR_Reload_IRQS_CH_COUNT (1U)
/** Interrupt vectors for the SUPERTMR peripheral type */
#define SUPERTMR_IRQS                                                                                           \
    {                                                                                                           \
        {SUPERTMR0_Ch0_IRQn, SUPERTMR0_Ch1_IRQn, SUPERTMR0_Ch2_IRQn, SUPERTMR0_Ch3_IRQn,                        \
         SUPERTMR0_Ch4_IRQn, SUPERTMR0_Ch5_IRQn, SUPERTMR0_Ch6_IRQn, SUPERTMR0_Ch7_IRQn},                       \
            {SUPERTMR1_Ch0_IRQn, SUPERTMR1_Ch1_IRQn, SUPERTMR1_Ch2_IRQn, SUPERTMR1_Ch3_IRQn,                    \
             SUPERTMR1_Ch4_IRQn, SUPERTMR1_Ch5_IRQn, SUPERTMR1_Ch6_IRQn, SUPERTMR1_Ch7_IRQn},                   \
            {SUPERTMR2_Ch0_IRQn, SUPERTMR2_Ch1_IRQn, SUPERTMR2_Ch2_IRQn, SUPERTMR2_Ch3_IRQn,                    \
             SUPERTMR2_Ch4_IRQn, SUPERTMR2_Ch5_IRQn, SUPERTMR2_Ch6_IRQn, SUPERTMR2_Ch7_IRQn},                   \
            {SUPERTMR3_Ch0_IRQn, SUPERTMR3_Ch1_IRQn, SUPERTMR3_Ch2_IRQn, SUPERTMR3_Ch3_IRQn,                    \
             SUPERTMR3_Ch4_IRQn, SUPERTMR3_Ch5_IRQn, SUPERTMR3_Ch6_IRQn, SUPERTMR3_Ch7_IRQn},                   \
            {SUPERTMR4_Ch0_IRQn, SUPERTMR4_Ch1_IRQn, SUPERTMR4_Ch2_IRQn, SUPERTMR4_Ch3_IRQn,                    \
             SUPERTMR4_Ch4_IRQn, SUPERTMR4_Ch5_IRQn, SUPERTMR4_Ch6_IRQn, SUPERTMR4_Ch7_IRQn},                   \
        {                                                                                                       \
            SUPERTMR5_Ch0_IRQn, SUPERTMR5_Ch1_IRQn, SUPERTMR5_Ch2_IRQn, SUPERTMR5_Ch3_IRQn, SUPERTMR5_Ch4_IRQn, \
                SUPERTMR5_Ch5_IRQn, SUPERTMR5_Ch6_IRQn, SUPERTMR5_Ch7_IRQn                                      \
        }                                                                                                       \
    }
#define SUPERTMR_Fault_IRQS                                                                                           \
    {                                                                                                                 \
        SUPERTMR0_Fault_IRQn, SUPERTMR1_Fault_IRQn, SUPERTMR2_Fault_IRQn, SUPERTMR3_Fault_IRQn, SUPERTMR4_Fault_IRQn, \
            SUPERTMR5_Fault_IRQn                                                                                      \
    }
#define SUPERTMR_Overflow_IRQS                                                                              \
    {                                                                                                       \
        SUPERTMR0_Ovf_IRQn, SUPERTMR1_Ovf_IRQn, SUPERTMR2_Ovf_IRQn, SUPERTMR3_Ovf_IRQn, SUPERTMR4_Ovf_IRQn, \
            SUPERTMR5_Ovf_IRQn                                                                              \
    }
#define SUPERTMR_Reload_IRQS                                                                        \
    {                                                                                               \
        SUPERTMR0_Reload_IRQn, SUPERTMR1_Reload_IRQn, SUPERTMR2_Reload_IRQn, SUPERTMR3_Reload_IRQn, \
            SUPERTMR4_Reload_IRQn, SUPERTMR5_Reload_IRQn                                            \
    }

/**
 * @}
 */
/* end of group SUPERTMR_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- TCSR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup TCSR_Peripheral_Access_Layer TCSR Peripheral Access Layer
 * @{
 */

/** Number of instances of the SIM module. */
#define TCSR_INSTANCE_COUNT (1U)

/* SIM - Peripheral instance base addresses */
/** Peripheral SIM base address */
#define TCSR_BASE (TCSR_BASEADDR)
/** Peripheral SIM base pointer */
#define TCSR ((tcsr_type_t *)TCSR_BASE)
/** Array initializer of SIM peripheral base addresses */
#define TCSR_BASE_ADDRS \
    {                   \
        TCSR_BASE       \
    }
/** Array initializer of SIM peripheral base pointers */
#define TCSR_BASE_PTRS \
    {                  \
        TCSR           \
    }

/**
 * @}
 */
/* end of group TCSR_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- TSENSOR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup TSENSOR_Peripheral_Access_Layer TSENSOR Peripheral Access Layer
 * @{
 */

/** Number of instances of the TSENSOR module. */
#define TSENSOR_INSTANCE_COUNT (1U)

/* TSENSOR - Peripheral instance base addresses */
/** Peripheral TSENSOR base address */
#define TSENSOR_BASE (TSENSOR_BASEADDR)
/** Peripheral TSENSOR base pointer */
#define TSENSOR ((tsensor_type_t *)TSENSOR_BASE)
/** Array initializer of TSENSOR peripheral base addresses */
#define TSENSOR_BASE_ADDRS \
    {                      \
        TSENSOR_BASE       \
    }
/** Array initializer of TSENSOR peripheral base pointers */
#define TSENSOR_BASE_PTRS \
    {                     \
        TSENSOR           \
    }

/**
 * @}
 */
/* end of group TSENSOR_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- CMU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CMU_Peripheral_Access_Layer CMU Peripheral Access Layer
 * @{
 */

/** Number of instances of the CMU module. */
#define CMU_INSTANCE_COUNT (5U)
/* CMU - Peripheral instance base addresses */
/** Peripheral CMU base address */
#define CMU0_BASE (CMU_0_BASEADDR)
/** Peripheral CMU base pointer */
#define CMU0 ((cmu_type_t *)CMU0_BASE)
/** Peripheral CMU base address */
#define CMU1_BASE (CMU_1_BASEADDR)
/** Peripheral CMU base pointer */
#define CMU1 ((cmu_type_t *)CMU1_BASE)
/** Peripheral CMU base address */
#define CMU2_BASE (CMU_2_BASEADDR)
/** Peripheral CMU base pointer */
#define CMU2 ((cmu_type_t *)CMU2_BASE)
/** Peripheral CMU base address */
#define CMU3_BASE (CMU_3_BASEADDR)
/** Peripheral CMU base pointer */
#define CMU3 ((cmu_type_t *)CMU3_BASE)
/** Peripheral CMU base address */
#define CMU4_BASE (CMU_4_BASEADDR)
/** Peripheral CMU base pointer */
#define CMU4 ((cmu_type_t *)CMU4_BASE)
/** Peripheral CMU common base pointer */
#define CMU_COMMON ((cmu_common_type_t *)CMU_COMMON_BASEADDR)

/** Array initializer of CMU peripheral base pointers */
#define CMU_BASE_PTRS                \
    {                                \
        CMU0, CMU1, CMU2, CMU3, CMU4 \
    }
/** Interrupt vectors for the CMU peripheral type */
#define CMU_IRQS                                              \
    {                                                         \
        CMU0_IRQn, CMU1_IRQn, CMU2_IRQn, CMU3_IRQn, CMU4_IRQn \
    }

/**
 * @}
 */
/* end of group CMU_Peripheral_Access_Layer */

/**
 * @}
 */
/* end of group Peripheral_Access_Layer_EAM2011 */

#endif /* __EAM2011_H__ */
