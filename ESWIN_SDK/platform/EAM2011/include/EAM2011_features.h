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
 * @file EAM2011_features.h
 * @brief
 * @author abu (abu@eswincomputing.com)
 * @date 2023-03-24
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#if !defined(__EAM2011_FEATURES_H__)
#define __EAM2011_FEATURES_H__

#include "EAM2011_clock.h"

/* EAM2011 Common features */

/** \brief  Endianness.
 */
#define CORE_LITTLE_ENDIAN

#define NUMBER_OF_TCLK_INPUTS       (3U)
#define SPLL_STABILIZATION_TIMEOUT  (10000U)
#define SROSC_STABILIZATION_TIMEOUT (10000U)
#define FROSC_STABILIZATION_TIMEOUT (10000U)

/* ADMA module features */

/* @brief Number of ADMA channels. */
#define FEATURE_ADMA_CHANNELS (3U)
typedef enum {
    ADMA_PERIPH_MEMORY = 0U,
    ADMA_PERIPH_ADC0   = 1U,
    ADMA_PERIPH_ADC1   = 2U,
} adma_periph_num_t;

typedef enum {
    ADMA_ADC0_RDADDR = 0x100U,
    ADMA_ADC1_RDADDR = 0x200U,
} adma_periph_addr_t;

/* ADC module features */

/** @brief ADC feature flag for defining number of external ADC channels.
 * If each ADC instance has different number of external channels, then
 * this define is set with the maximum value. */
#define FEATURE_ADC_DMA_ADMA              (1)
#define FEATURE_ADC_MAX_NUM_EXT_CHANS     (24)
#define FEATURE_ADC_INPUTCHAN_DISABLED    (0x3FU)
#define FEATURE_ADC_CALIBRATION_VALUE     (0x800)
#define FEATURE_ADC_SAMPLE_CONV_CYCLE     (14)
#define FEATURE_ADC_CALC_SAMPLERATE_CONST (0xE00000000) /* FEATURE_ADC_SAMPLE_CONV_CYCLE * (2**32) */

#define ADC_CLOCKS         \
    {                      \
        ADC0_CLK, ADC1_CLK \
    }

/** @brief ADC number of control channels */
#define ADC_CTRL_CHANS_COUNT (24)

/* @brief Max of adc clock frequency */
#define ADC_CLOCK_FREQ_MAX_RUNTIME (60000000U)
/* @brief Min of adc clock frequency */
#define ADC_CLOCK_FREQ_MIN_RUNTIME (3000000U)

/* PDMA module features */

/* @brief Number of PDMA interrupt lines. */
#define DMA_CHANNELS_INTERRUPT_LINES (32U)

/* @brief Number of PDMA virtual error interrupt lines. */
#define FEATURE_DMA_VIRTUAL_ERROR_INTERRUPT_LINES \
    ((uint32_t)FEATURE_DMA_ERROR_INTERRUPT_LINES * (uint32_t)PDMA_INSTANCE_COUNT)

/* @brief PDMA module separate interrupt lines for each channel */
#define FEATURE_DMA_SEPARATE_IRQ_LINES_PER_CHN
/* @brief Conversion from channel index to DCHPRI index. */
#define FEATURE_DMA_CHN_TO_DCHPRI_INDEX(x) ((x) ^ 3U)

/* @brief Clock name for PDMA */
#define FEATURE_DMA_CLOCK_NAMES \
    {                           \
        SIM_DMA_CLK             \
    }
/* @brief PDMA channel width based on number of TCDs: 2^N, N=4,5,... */
#define FEATURE_DMA_CH_WIDTH (5U)

/* @brief PDMA supports the following particular transfer size: */
#define FEATURE_DMA_TRANSFER_SIZE_16B
#define FEATURE_DMA_TRANSFER_SIZE_32B

/* I2C module features */

/* @brief PDMA instance used for I2C module */
#define I2C_DMA_INSTANCE (0U)

/* @brief PDMA requests for I2C module. */
#define I2C_PDMA_REQ                                        \
    {                                                       \
        {(uint8_t)PDMA_REQ_I2C0, (uint8_t)PDMA_REQ_I2C0},   \
        {                                                   \
            (uint8_t) PDMA_REQ_I2C1, (uint8_t)PDMA_REQ_I2C1 \
        }                                                   \
    }
// TODO: need to confirm real clock source
/* @brief PCC clocks for I2C module. */
#define I2C_PCC_CLOCKS     \
    {                      \
        I2C0_CLK, I2C1_CLK \
    }

/* I2C ultra-fast operating mode */
#define I2C_HAS_ULTRA_FAST_MODE (1U)

/* DMAMUX module features */

/* @brief Clock names for DMAMUX. */
#define FEATURE_DMAMUX_CLOCK_NAMES \
    {                              \
        DMAMUX0_CLK                \
    }
/**
 * @brief Structure for the PDMA hardware request
 *
 * Defines the structure for the PDMA hardware request collections. The user can configure the
 * hardware request into DMAMUX to trigger the PDMA transfer accordingly. The index
 * of the hardware request varies according  to the to SoC.
 */
typedef enum {
    PDMA_REQ_ADC0                = 0U,
    PDMA_REQ_ADC1                = 1U,
    PDMA_REQ_CMP0                = 2U,
    PDMA_REQ_PDU0                = 4U,
    PDMA_REQ_PDU1                = 5U,
    PDMA_REQ_SUPERTMR0_CHANNEL_0 = 6U,
    PDMA_REQ_SUPERTMR0_CHANNEL_1 = 7U,
    PDMA_REQ_SUPERTMR0_CHANNEL_2 = 8U,
    PDMA_REQ_SUPERTMR0_CHANNEL_3 = 9U,
    PDMA_REQ_SUPERTMR0_CHANNEL_4 = 10U,
    PDMA_REQ_SUPERTMR0_CHANNEL_5 = 11U,
    PDMA_REQ_SUPERTMR0_CHANNEL_6 = 12U,
    PDMA_REQ_SUPERTMR0_CHANNEL_7 = 13U,
    PDMA_REQ_SUPERTMR1_CHANNEL_0 = 14U,
    PDMA_REQ_SUPERTMR1_CHANNEL_1 = 15U,
    PDMA_REQ_SUPERTMR1_CHANNEL_2 = 16U,
    PDMA_REQ_SUPERTMR1_CHANNEL_3 = 17U,
    PDMA_REQ_SUPERTMR1_CHANNEL_4 = 18U,
    PDMA_REQ_SUPERTMR1_CHANNEL_5 = 19U,
    PDMA_REQ_SUPERTMR1_CHANNEL_6 = 20U,
    PDMA_REQ_SUPERTMR1_CHANNEL_7 = 21U,
    PDMA_REQ_SUPERTMR2_CHANNEL_0 = 22U,
    PDMA_REQ_SUPERTMR2_CHANNEL_1 = 23U,
    PDMA_REQ_SUPERTMR2_CHANNEL_2 = 24U,
    PDMA_REQ_SUPERTMR2_CHANNEL_3 = 25U,
    PDMA_REQ_SUPERTMR2_CHANNEL_4 = 26U,
    PDMA_REQ_SUPERTMR2_CHANNEL_5 = 27U,
    PDMA_REQ_SUPERTMR2_CHANNEL_6 = 28U,
    PDMA_REQ_SUPERTMR2_CHANNEL_7 = 29U,
    PDMA_REQ_SUPERTMR3_CHANNEL_0 = 30U,
    PDMA_REQ_SUPERTMR3_CHANNEL_1 = 31U,
    PDMA_REQ_SUPERTMR3_CHANNEL_2 = 32U,
    PDMA_REQ_SUPERTMR3_CHANNEL_3 = 33U,
    PDMA_REQ_SUPERTMR3_CHANNEL_4 = 34U,
    PDMA_REQ_SUPERTMR3_CHANNEL_5 = 35U,
    PDMA_REQ_SUPERTMR3_CHANNEL_6 = 36U,
    PDMA_REQ_SUPERTMR3_CHANNEL_7 = 37U,
    PDMA_REQ_SUPERTMR4_CHANNEL_0 = 38U,
    PDMA_REQ_SUPERTMR4_CHANNEL_1 = 39U,
    PDMA_REQ_SUPERTMR4_CHANNEL_2 = 40U,
    PDMA_REQ_SUPERTMR4_CHANNEL_3 = 41U,
    PDMA_REQ_SUPERTMR4_CHANNEL_4 = 42U,
    PDMA_REQ_SUPERTMR4_CHANNEL_5 = 43U,
    PDMA_REQ_SUPERTMR4_CHANNEL_6 = 44U,
    PDMA_REQ_SUPERTMR4_CHANNEL_7 = 45U,
    PDMA_REQ_SUPERTMR5_CHANNEL_0 = 46U,
    PDMA_REQ_SUPERTMR5_CHANNEL_1 = 47U,
    PDMA_REQ_SUPERTMR5_CHANNEL_2 = 48U,
    PDMA_REQ_SUPERTMR5_CHANNEL_3 = 49U,
    PDMA_REQ_SUPERTMR5_CHANNEL_4 = 50U,
    PDMA_REQ_SUPERTMR5_CHANNEL_5 = 51U,
    PDMA_REQ_SUPERTMR5_CHANNEL_6 = 52U,
    PDMA_REQ_SUPERTMR5_CHANNEL_7 = 53U,
    PDMA_REQ_UART0_TX            = 54U,
    PDMA_REQ_UART0_RX            = 55U,
    PDMA_REQ_UART1_TX            = 56U,
    PDMA_REQ_UART1_RX            = 57U,
    PDMA_REQ_UART2_TX            = 58U,
    PDMA_REQ_UART2_RX            = 59U,
    PDMA_REQ_UART3_TX            = 60U,
    PDMA_REQ_UART3_RX            = 61U,
    PDMA_REQ_UART4_TX            = 62U,
    PDMA_REQ_UART4_RX            = 63U,
    PDMA_REQ_UART5_TX            = 64U,
    PDMA_REQ_UART5_RX            = 65U,
    PDMA_REQ_I2C0                = 74U,
    PDMA_REQ_I2C1                = 75U,
    PDMA_REQ_SUPERIO_SHIFTER0    = 76U,
    PDMA_REQ_SUPERIO_SHIFTER1    = 77U,
    PDMA_REQ_SUPERIO_SHIFTER2    = 78U,
    PDMA_REQ_SUPERIO_SHIFTER3    = 79U,
    PDMA_REQ_PCTMR0              = 80U,
    PDMA_REQ_PCTMR1              = 81U,
    PDMA_REQ_PCTMR2              = 82U,
    PDMA_REQ_PCTMR3              = 83U,
    PDMA_REQ_PCTMR4              = 84U,
    PDMA_REQ_PCTMR5              = 85U,
    PDMA_REQ_PCTMR6              = 86U,
    PDMA_REQ_PCTMR7              = 87U,
    PDMA_REQ_PCTMR8              = 88U,
    PDMA_REQ_SPI0_TX             = 89U,
    PDMA_REQ_SPI0_RX             = 90U,
    PDMA_REQ_SPI1_TX             = 91U,
    PDMA_REQ_SPI1_RX             = 92U,
    PDMA_REQ_SPI2_TX             = 93U,
    PDMA_REQ_SPI2_RX             = 94U,
    PDMA_REQ_SPI3_TX             = 95U,
    PDMA_REQ_SPI3_RX             = 96U,
    PDMA_REQ_CAN0                = 101U,
    PDMA_REQ_CAN1                = 102U,
    PDMA_REQ_CAN2                = 103U,
    PDMA_REQ_CAN3                = 105U,
    PDMA_REQ_GPIO0               = 107U,
    PDMA_REQ_GPIO1               = 108U,
    PDMA_REQ_GPIO2               = 109U,
    PDMA_REQ_GPIO3               = 110U,
    PDMA_REQ_DISABLED            = 0xFFU,
} dma_request_source_t;

/* CRC module features */

/* @brief Default CRC bit width */
#define FEATURE_CRC_DEFAULT_WIDTH CRC_BITS_16
/* @brief Default polynomial 0x1021U */
#define FEATURE_CRC_DEFAULT_POLYNOMIAL (0x1021U)
/* @brief Default CRC read transpose */
#define FEATURE_CRC_DEFAULT_READ_TRANSPOSE CRC_TRANSPOSE_NONE
/* @brief Default CRC write transpose */
#define FEATURE_CRC_DEFAULT_WRITE_TRANSPOSE CRC_TRANSPOSE_NONE
/* @brief Default CRC complement or inversion checksum */
#define FEATURE_CRC_DEFAULT_COMPLEMENT_CHECKSUM false
/* @brief Default seed value is 0xFFFFU */
#define FEATURE_CRC_DEFAULT_SEED (0xFFFFU)

/**********************************************************************
 *           CMP module features
 **********************************************************************/
#define CMP_MUX_SOURCE   (0U)
#define CMP_VREFB_SOURCE (1U)
#define CMP_1_2V_SOURCE  (2U)
#define CMP_DAC_SOURCE   (3U)

#define CMP_CLOCKS \
    {              \
        CMP_CLK    \
    }

/**********************************************************************
 *           DAC module features
 **********************************************************************/
#define DAC_CRG_CLOCKS \
    {                  \
        DAC_CLK        \
    }
#define FEATURE_DAC_RESOLUTION          (12)
#define FEATURE_DAC_CLK_FREQUENCY       (20000000)
#define FEATURE_DAC_CALC_FREQWORD_CONST (0x100000000) /* 2**32 */

/* SUPERIO module features */

/* @brief Define the maximum number of shifters for any SUPERIO instance. */
#define FEATURE_SUPERIO_MAX_SHIFTER_COUNT (4U)
/* @brief Define PDMA request names for SUPERIO. */
#define FEATURE_SUPERIO_DMA_REQ_0 PDMA_REQ_SUPERIO_SHIFTER0
#define FEATURE_SUPERIO_DMA_REQ_1 PDMA_REQ_SUPERIO_SHIFTER1
#define FEATURE_SUPERIO_DMA_REQ_2 PDMA_REQ_SUPERIO_SHIFTER2
#define FEATURE_SUPERIO_DMA_REQ_3 PDMA_REQ_SUPERIO_SHIFTER3

/* UART module features */

/* @brief Clock names for UART. */
#define UART_CLOCK_NAMES                                                 \
    {                                                                    \
        UART0_CLK, UART1_CLK, UART2_CLK, UART3_CLK, UART4_CLK, UART5_CLK \
    }

/* FMC module features */
#define FEATURE_FMC_CLOCK_NAMES \
    {                           \
        FMC_CLK                 \
    }

/* TRGMUX module features */
/**
 * @brief Enumeration for trigger source module of TRGMUX
 *
 * Describes all possible inputs (trigger sources) of the TRGMUX IP
 * This enumeration depends on the supported instances in device
 */
typedef enum {
    TRGMUX_TRIG_SOURCE_GND                 = 0x00U,
    TRGMUX_TRIG_SOURCE_VDD                 = 0x01U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN0          = 0x02U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN1          = 0x03U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN2          = 0x04U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN3          = 0x05U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN4          = 0x06U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN5          = 0x07U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN6          = 0x08U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN7          = 0x09U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN8          = 0x0AU,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN9          = 0x0BU,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN10         = 0x0CU,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN11         = 0x0DU,
    TRGMUX_TRIG_SOURCE_CMP_OUT             = 0x0EU,
    TRGMUX_TRIG_SOURCE_GTMR                = 0x0FU,
    TRGMUX_TRIG_SOURCE_MOTO_PITMR_CH0      = 0x11U,
    TRGMUX_TRIG_SOURCE_MOTO_PITMR_CH1      = 0x12U,
    TRGMUX_TRIG_SOURCE_MOTO_PITMR_CH2      = 0x13U,
    TRGMUX_TRIG_SOURCE_MOTO_PITMR_CH3      = 0x14U,
    TRGMUX_TRIG_SOURCE_SUPERTMR0_INIT_TRIG = 0x16U,
    TRGMUX_TRIG_SOURCE_SUPERTMR0_EXT_TRIG  = 0x17U,
    TRGMUX_TRIG_SOURCE_SUPERTMR1_INIT_TRIG = 0x18U,
    TRGMUX_TRIG_SOURCE_SUPERTMR1_EXT_TRIG  = 0x19U,
    TRGMUX_TRIG_SOURCE_SUPERTMR2_INIT_TRIG = 0x1AU,
    TRGMUX_TRIG_SOURCE_SUPERTMR2_EXT_TRIG  = 0x1BU,
    TRGMUX_TRIG_SOURCE_SUPERTMR3_INIT_TRIG = 0x1CU,
    TRGMUX_TRIG_SOURCE_SUPERTMR3_EXT_TRIG  = 0x1DU,
    TRGMUX_TRIG_SOURCE_ADC0_SC1A_COCO      = 0x1EU,
    TRGMUX_TRIG_SOURCE_ADC0_SC1B_COCO      = 0x1FU,
    TRGMUX_TRIG_SOURCE_ADC1_SC1A_COCO      = 0x20U,
    TRGMUX_TRIG_SOURCE_ADC1_SC1B_COCO      = 0x21U,
    TRGMUX_TRIG_SOURCE_PDU0_CH0_TRIG       = 0x22U,
    TRGMUX_TRIG_SOURCE_PDU0_PULSE_OUT      = 0x24U,
    TRGMUX_TRIG_SOURCE_PDU1_CH0_TRIG       = 0x25U,
    TRGMUX_TRIG_SOURCE_PDU1_PULSE_OUT      = 0x27U,
    TRGMUX_TRIG_SOURCE_RTC_ALARM           = 0x2BU,
    TRGMUX_TRIG_SOURCE_RTC_SECOND          = 0x2CU,
    TRGMUX_TRIG_SOURCE_SUPERIO_TRIG0       = 0x2DU,
    TRGMUX_TRIG_SOURCE_SUPERIO_TRIG1       = 0x2EU,
    TRGMUX_TRIG_SOURCE_SUPERIO_TRIG2       = 0x2FU,
    TRGMUX_TRIG_SOURCE_SUPERIO_TRIG3       = 0x30U,
    TRGMUX_TRIG_SOURCE_UART0_RX_DATA       = 0x31U,
    TRGMUX_TRIG_SOURCE_UART0_TX_DATA       = 0x32U,
    TRGMUX_TRIG_SOURCE_UART0_RX_IDLE       = 0x33U,
    TRGMUX_TRIG_SOURCE_UART1_RX_DATA       = 0x34U,
    TRGMUX_TRIG_SOURCE_UART1_TX_DATA       = 0x35U,
    TRGMUX_TRIG_SOURCE_UART1_RX_IDLE       = 0x36U,
    TRGMUX_TRIG_SOURCE_I2C0_MASTER_TRIGGER = 0x37U,
    TRGMUX_TRIG_SOURCE_I2C0_SLAVE_TRIGGER  = 0x38U,
    TRGMUX_TRIG_SOURCE_SPI0_FRAME          = 0x3BU,
    TRGMUX_TRIG_SOURCE_SPI0_RX_DATA        = 0x3CU,
    TRGMUX_TRIG_SOURCE_SPI1_FRAME          = 0x3DU,
    TRGMUX_TRIG_SOURCE_SPI1_RX_DATA        = 0x3EU,
    TRGMUX_TRIG_SOURCE_SIM_SW_TRIG         = 0x3FU,
    TRGMUX_TRIG_SOURCE_I2C1_MASTER_TRIGGER = 0x43U,
    TRGMUX_TRIG_SOURCE_I2C1_SLAVE_TRIGGER  = 0x44U,
    TRGMUX_TRIG_SOURCE_SUPERTMR4_INIT_TRIG = 0x45U,
    TRGMUX_TRIG_SOURCE_SUPERTMR4_EXT_TRIG  = 0x46U,
    TRGMUX_TRIG_SOURCE_SUPERTMR5_INIT_TRIG = 0x47U,
    TRGMUX_TRIG_SOURCE_SUPERTMR5_EXT_TRIG  = 0x48U,
    TRGMUX_TRIG_SOURCE_UART2_RX_DATA       = 0x51U,
    TRGMUX_TRIG_SOURCE_UART2_TX_DATA       = 0x52U,
    TRGMUX_TRIG_SOURCE_UART2_RX_IDLE       = 0x53U,
    TRGMUX_TRIG_SOURCE_UART3_RX_DATA       = 0x54U,
    TRGMUX_TRIG_SOURCE_UART3_TX_DATA       = 0x55U,
    TRGMUX_TRIG_SOURCE_UART3_RX_IDLE       = 0x56U,
    TRGMUX_TRIG_SOURCE_UART4_RX_DATA       = 0x57U,
    TRGMUX_TRIG_SOURCE_UART4_TX_DATA       = 0x58U,
    TRGMUX_TRIG_SOURCE_UART4_RX_IDLE       = 0x59U,
    TRGMUX_TRIG_SOURCE_UART5_RX_DATA       = 0x5AU,
    TRGMUX_TRIG_SOURCE_UART5_TX_DATA       = 0x5BU,
    TRGMUX_TRIG_SOURCE_UART5_RX_IDLE       = 0x5CU,
    TRGMUX_TRIG_SOURCE_SPI2_FRAME          = 0x5DU,
    TRGMUX_TRIG_SOURCE_SPI2_RX_DATA        = 0x5EU,
    TRGMUX_TRIG_SOURCE_SPI3_FRAME          = 0x5FU,
    TRGMUX_TRIG_SOURCE_SPI3_RX_DATA        = 0x60U,
    TRGMUX_TRIG_SOURCE_PCTMR0_ASYNC        = 0x61U,
    TRGMUX_TRIG_SOURCE_PCTMR0_DELAY        = 0x62U,
    TRGMUX_TRIG_SOURCE_PCTMR1_ASYNC        = 0x63U,
    TRGMUX_TRIG_SOURCE_PCTMR1_DELAY        = 0x64U,
    TRGMUX_TRIG_SOURCE_PCTMR2_ASYNC        = 0x65U,
    TRGMUX_TRIG_SOURCE_PCTMR2_DELAY        = 0x66U,
    TRGMUX_TRIG_SOURCE_PCTMR3_ASYNC        = 0x67U,
    TRGMUX_TRIG_SOURCE_PCTMR3_DELAY        = 0x68U,
    TRGMUX_TRIG_SOURCE_PCTMR4_ASYNC        = 0x69U,
    TRGMUX_TRIG_SOURCE_PCTMR4_DELAY        = 0x6AU,
    TRGMUX_TRIG_SOURCE_PCTMR5_ASYNC        = 0x6BU,
    TRGMUX_TRIG_SOURCE_PCTMR5_DELAY        = 0x6CU,
    TRGMUX_TRIG_SOURCE_PCTMR6_ASYNC        = 0x6DU,
    TRGMUX_TRIG_SOURCE_PCTMR6_DELAY        = 0x6EU,
    TRGMUX_TRIG_SOURCE_PCTMR7_ASYNC        = 0x6FU,
    TRGMUX_TRIG_SOURCE_PCTMR7_DELAY        = 0x70U,
    TRGMUX_TRIG_SOURCE_PCTMR8_DELAY        = 0x71U
} trgmux_trigger_source_t;

/**
 * @brief Enumeration for target module of TRGMUX
 *
 * Describes all possible outputs (target modules) of the TRGMUX IP
 * This enumeration depends on the supported instances in device
 */
typedef enum {
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH0     = 0x00U,
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH1     = 0x01U,
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH2     = 0x02U,
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH3     = 0x03U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT0        = 0x04U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT1        = 0x05U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT2        = 0x06U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT3        = 0x07U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT4        = 0x08U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT5        = 0x09U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT6        = 0x0AU,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT7        = 0x0BU,
    TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA0    = 0x0CU,
    TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA1    = 0x0DU,
    TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA2    = 0x0EU,
    TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA3    = 0x0FU,
    TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA0    = 0x10U,
    TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA1    = 0x11U,
    TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA2    = 0x12U,
    TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA3    = 0x13U,
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH4     = 0x14U,
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH5     = 0x15U,
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH6     = 0x16U,
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH7     = 0x17U,
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH8     = 0x18U,
    TRGMUX_TARGET_MODULE_PDMA_PITMR_CH9     = 0x19U,
    TRGMUX_TARGET_MODULE_CMP_SAMPLE         = 0x1CU,
    TRGMUX_TARGET_MODULE_SUPERTMR0_HWTRIG0  = 0x28U,
    TRGMUX_TARGET_MODULE_SUPERTMR0_FAULT0   = 0x29U,
    TRGMUX_TARGET_MODULE_SUPERTMR0_FAULT1   = 0x2AU,
    TRGMUX_TARGET_MODULE_SUPERTMR0_FAULT2   = 0x2BU,
    TRGMUX_TARGET_MODULE_SUPERTMR1_HWTRIG0  = 0x2CU,
    TRGMUX_TARGET_MODULE_SUPERTMR1_FAULT0   = 0x2DU,
    TRGMUX_TARGET_MODULE_SUPERTMR1_FAULT1   = 0x2EU,
    TRGMUX_TARGET_MODULE_SUPERTMR1_FAULT2   = 0x2FU,
    TRGMUX_TARGET_MODULE_SUPERTMR2_HWTRIG0  = 0x30U,
    TRGMUX_TARGET_MODULE_SUPERTMR2_FAULT0   = 0x31U,
    TRGMUX_TARGET_MODULE_SUPERTMR2_FAULT1   = 0x32U,
    TRGMUX_TARGET_MODULE_SUPERTMR2_FAULT2   = 0x33U,
    TRGMUX_TARGET_MODULE_SUPERTMR3_HWTRIG0  = 0x34U,
    TRGMUX_TARGET_MODULE_SUPERTMR3_FAULT0   = 0x35U,
    TRGMUX_TARGET_MODULE_SUPERTMR3_FAULT1   = 0x36U,
    TRGMUX_TARGET_MODULE_SUPERTMR3_FAULT2   = 0x37U,
    TRGMUX_TARGET_MODULE_PDU0_TRG_IN        = 0x38U,
    TRGMUX_TARGET_MODULE_PDU1_TRG_IN        = 0x3CU,
    TRGMUX_TARGET_MODULE_SUPERIO0_TRG_TIM0  = 0x44U,
    TRGMUX_TARGET_MODULE_SUPERIO0_TRG_TIM1  = 0x45U,
    TRGMUX_TARGET_MODULE_SUPERIO0_TRG_TIM2  = 0x46U,
    TRGMUX_TARGET_MODULE_SUPERIO0_TRG_TIM3  = 0x47U,
    TRGMUX_TARGET_MODULE_MOTO_PITMR_TRG_CH0 = 0x48U,
    TRGMUX_TARGET_MODULE_MOTO_PITMR_TRG_CH1 = 0x49U,
    TRGMUX_TARGET_MODULE_MOTO_PITMR_TRG_CH2 = 0x4AU,
    TRGMUX_TARGET_MODULE_MOTO_PITMR_TRG_CH3 = 0x4BU,
    TRGMUX_TARGET_MODULE_UART0_TRG          = 0x4CU,
    TRGMUX_TARGET_MODULE_UART1_TRG          = 0x50U,
    TRGMUX_TARGET_MODULE_I2C0_TRG           = 0x54U,
    TRGMUX_TARGET_MODULE_SPI0_TRG           = 0x5CU,
    TRGMUX_TARGET_MODULE_SPI1_TRG           = 0x60U,
    TRGMUX_TARGET_MODULE_PCTMR8_ALT0        = 0x64U,
    TRGMUX_TARGET_MODULE_I2C1_TRG           = 0x6CU,
    TRGMUX_TARGET_MODULE_SUPERTMR4_HWTRIG0  = 0x70U,
    TRGMUX_TARGET_MODULE_SUPERTMR5_HWTRIG0  = 0x74U,
    TRGMUX_TARGET_MODULE_UART2_TRG          = 0x80U,
    TRGMUX_TARGET_MODULE_UART3_TRG          = 0x84U,
    TRGMUX_TARGET_MODULE_UART4_TRG          = 0x88U,
    TRGMUX_TARGET_MODULE_UART5_TRG          = 0x8CU,
    TRGMUX_TARGET_MODULE_SPI2_TRG           = 0x90U,
    TRGMUX_TARGET_MODULE_SPI3_TRG           = 0x94U,
    TRGMUX_TARGET_MODULE_SUPERIO1_TRG_TIM0  = 0x98U,
    TRGMUX_TARGET_MODULE_SUPERIO1_TRG_TIM1  = 0x99U,
    TRGMUX_TARGET_MODULE_SUPERIO1_TRG_TIM2  = 0x9AU,
    TRGMUX_TARGET_MODULE_SUPERIO1_TRG_TIM3  = 0x9BU,
    TRGMUX_TARGET_MODULE_SUPERIO2_TRG_TIM0  = 0x9CU,
    TRGMUX_TARGET_MODULE_SUPERIO2_TRG_TIM1  = 0x9DU,
    TRGMUX_TARGET_MODULE_SUPERIO2_TRG_TIM2  = 0x9EU,
    TRGMUX_TARGET_MODULE_SUPERIO2_TRG_TIM3  = 0x9FU,
    TRGMUX_TARGET_MODULE_SUPERIO3_TRG_TIM0  = 0xA0U,
    TRGMUX_TARGET_MODULE_SUPERIO3_TRG_TIM1  = 0xA1U,
    TRGMUX_TARGET_MODULE_SUPERIO3_TRG_TIM2  = 0xA2U,
    TRGMUX_TARGET_MODULE_SUPERIO3_TRG_TIM3  = 0xA3U,
    TRGMUX_TARGET_MODULE_GTMR_ALT0          = 0xA4U
} trgmux_target_module_t;

/* TRGMUX index offsets */
#define TRGMUX_PDMA_PITMR0_INDEX (00U)
#define TRGMUX_OUT0_INDEX        (01U)
#define TRGMUX_OUT1_INDEX        (02U)
#define TRGMUX_ADC0_INDEX        (03U)
#define TRGMUX_ADC1_INDEX        (04U)
#define TRGMUX_PDMA_PITMR1_INDEX (05U)
#define TRGMUX_PDMA_PITMR2_INDEX (06U)
#define TRGMUX_CMP0_INDEX        (07U)
#define TRGMUX_SUPERTMR0_INDEX   (10U)
#define TRGMUX_SUPERTMR1_INDEX   (11U)
#define TRGMUX_SUPERTMR2_INDEX   (12U)
#define TRGMUX_SUPERTMR3_INDEX   (13U)
#define TRGMUX_SUPERTMR4_INDEX   (28U)
#define TRGMUX_SUPERTMR5_INDEX   (29U)
#define TRGMUX_PDU0_INDEX        (14U)
#define TRGMUX_PDU1_INDEX        (15U)
#define TRGMUX_SUPERIO0_INDEX    (17U)
#define TRGMUX_SUPERIO1_INDEX    (38U)
#define TRGMUX_SUPERIO2_INDEX    (39U)
#define TRGMUX_SUPERIO3_INDEX    (40U)
#define TRGMUX_MOTO_PITMR_INDEX  (18U)
#define TRGMUX_UART0_INDEX       (19U)
#define TRGMUX_UART1_INDEX       (20U)
#define TRGMUX_UART2_INDEX       (32U)
#define TRGMUX_UART3_INDEX       (33U)
#define TRGMUX_UART4_INDEX       (34U)
#define TRGMUX_UART5_INDEX       (35U)
#define TRGMUX_I2C0_INDEX        (21U)
#define TRGMUX_I2C1_INDEX        (27U)
#define TRGMUX_SPI0_INDEX        (23U)
#define TRGMUX_SPI1_INDEX        (24U)
#define TRGMUX_SPI2_INDEX        (36U)
#define TRGMUX_SPI3_INDEX        (37U)
#define TRGMUX_PCTMR8_INDEX      (25U)
#define TRGMUX_GTMR_INDEX        (41U)

/* @brief Constant array storing the value of all TRGMUX output(target module) identifiers */
#define FEATURE_TRGMUX_TARGET_MODULE                                                                                       \
    {                                                                                                                      \
        TRGMUX_TARGET_MODULE_PDMA_PITMR_CH0, TRGMUX_TARGET_MODULE_PDMA_PITMR_CH1, TRGMUX_TARGET_MODULE_PDMA_PITMR_CH2,     \
            TRGMUX_TARGET_MODULE_PDMA_PITMR_CH3, TRGMUX_TARGET_MODULE_TRGMUX_OUT0, TRGMUX_TARGET_MODULE_TRGMUX_OUT1,       \
            TRGMUX_TARGET_MODULE_TRGMUX_OUT2, TRGMUX_TARGET_MODULE_TRGMUX_OUT3, TRGMUX_TARGET_MODULE_TRGMUX_OUT4,          \
            TRGMUX_TARGET_MODULE_TRGMUX_OUT5, TRGMUX_TARGET_MODULE_TRGMUX_OUT6, TRGMUX_TARGET_MODULE_TRGMUX_OUT7,          \
            TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA0, TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA1,                                    \
            TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA2, TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA3,                                    \
            TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA0, TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA1,                                    \
            TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA2, TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA3,                                    \
            TRGMUX_TARGET_MODULE_PDMA_PITMR_CH4, TRGMUX_TARGET_MODULE_PDMA_PITMR_CH5, TRGMUX_TARGET_MODULE_PDMA_PITMR_CH6, \
            TRGMUX_TARGET_MODULE_PDMA_PITMR_CH7, TRGMUX_TARGET_MODULE_PDMA_PITMR_CH8, TRGMUX_TARGET_MODULE_PDMA_PITMR_CH9, \
            TRGMUX_TARGET_MODULE_CMP_SAMPLE, TRGMUX_TARGET_MODULE_SUPERTMR0_HWTRIG0,                                       \
            TRGMUX_TARGET_MODULE_SUPERTMR0_FAULT0, TRGMUX_TARGET_MODULE_SUPERTMR0_FAULT1,                                  \
            TRGMUX_TARGET_MODULE_SUPERTMR0_FAULT2, TRGMUX_TARGET_MODULE_SUPERTMR1_HWTRIG0,                                 \
            TRGMUX_TARGET_MODULE_SUPERTMR1_FAULT0, TRGMUX_TARGET_MODULE_SUPERTMR1_FAULT1,                                  \
            TRGMUX_TARGET_MODULE_SUPERTMR1_FAULT2, TRGMUX_TARGET_MODULE_SUPERTMR2_HWTRIG0,                                 \
            TRGMUX_TARGET_MODULE_SUPERTMR2_FAULT0, TRGMUX_TARGET_MODULE_SUPERTMR2_FAULT1,                                  \
            TRGMUX_TARGET_MODULE_SUPERTMR2_FAULT2, TRGMUX_TARGET_MODULE_SUPERTMR3_HWTRIG0,                                 \
            TRGMUX_TARGET_MODULE_SUPERTMR3_FAULT0, TRGMUX_TARGET_MODULE_SUPERTMR3_FAULT1,                                  \
            TRGMUX_TARGET_MODULE_SUPERTMR3_FAULT2, TRGMUX_TARGET_MODULE_SUPERTMR4_HWTRIG0,                                 \
            TRGMUX_TARGET_MODULE_SUPERTMR5_HWTRIG0, TRGMUX_TARGET_MODULE_PDU0_TRG_IN,                                      \
            TRGMUX_TARGET_MODULE_PDU1_TRG_IN, TRGMUX_TARGET_MODULE_SUPERIO0_TRG_TIM0,                                      \
            TRGMUX_TARGET_MODULE_SUPERIO0_TRG_TIM1, TRGMUX_TARGET_MODULE_SUPERIO0_TRG_TIM2,                                \
            TRGMUX_TARGET_MODULE_SUPERIO0_TRG_TIM3, TRGMUX_TARGET_MODULE_SUPERIO1_TRG_TIM0,                                \
            TRGMUX_TARGET_MODULE_SUPERIO1_TRG_TIM1, TRGMUX_TARGET_MODULE_SUPERIO1_TRG_TIM2,                                \
            TRGMUX_TARGET_MODULE_SUPERIO1_TRG_TIM3, TRGMUX_TARGET_MODULE_SUPERIO2_TRG_TIM0,                                \
            TRGMUX_TARGET_MODULE_SUPERIO2_TRG_TIM1, TRGMUX_TARGET_MODULE_SUPERIO2_TRG_TIM2,                                \
            TRGMUX_TARGET_MODULE_SUPERIO2_TRG_TIM3, TRGMUX_TARGET_MODULE_SUPERIO3_TRG_TIM0,                                \
            TRGMUX_TARGET_MODULE_SUPERIO3_TRG_TIM1, TRGMUX_TARGET_MODULE_SUPERIO3_TRG_TIM2,                                \
            TRGMUX_TARGET_MODULE_SUPERIO3_TRG_TIM3, TRGMUX_TARGET_MODULE_MOTO_PITMR_TRG_CH0,                               \
            TRGMUX_TARGET_MODULE_MOTO_PITMR_TRG_CH1, TRGMUX_TARGET_MODULE_MOTO_PITMR_TRG_CH2,                              \
            TRGMUX_TARGET_MODULE_MOTO_PITMR_TRG_CH3, TRGMUX_TARGET_MODULE_UART0_TRG, TRGMUX_TARGET_MODULE_UART1_TRG,       \
            TRGMUX_TARGET_MODULE_UART2_TRG, TRGMUX_TARGET_MODULE_UART3_TRG, TRGMUX_TARGET_MODULE_UART4_TRG,                \
            TRGMUX_TARGET_MODULE_UART5_TRG, TRGMUX_TARGET_MODULE_I2C0_TRG, TRGMUX_TARGET_MODULE_I2C1_TRG,                  \
            TRGMUX_TARGET_MODULE_SPI0_TRG, TRGMUX_TARGET_MODULE_SPI1_TRG, TRGMUX_TARGET_MODULE_SPI2_TRG,                   \
            TRGMUX_TARGET_MODULE_SPI3_TRG, TRGMUX_TARGET_MODULE_PCTMR8_ALT0, TRGMUX_TARGET_MODULE_GTMR_ALT0                \
    }

/* TMRMUX module features */
/**
 * @brief Enumeration for trigger source module of TMRMUX
 *
 * Describes all possible inputs (trigger sources) of the TMRMUX IP
 * This enumeration depends on the supported instances in device
 */
typedef enum {
    TMRMUX_TRIG_SOURCE_SUPERTMR0_EXT_TRIG0 = 0U,
    TMRMUX_TRIG_SOURCE_SUPERTMR0_EXT_TRIG1 = 1U,
    TMRMUX_TRIG_SOURCE_SUPERTMR0_EXT_TRIG2 = 2U,
    TMRMUX_TRIG_SOURCE_SUPERTMR0_EXT_TRIG3 = 3U,
    TMRMUX_TRIG_SOURCE_SUPERTMR0_EXT_TRIG4 = 4U,
    TMRMUX_TRIG_SOURCE_SUPERTMR0_EXT_TRIG5 = 5U,
    TMRMUX_TRIG_SOURCE_SUPERTMR0_EXT_TRIG6 = 6U,
    TMRMUX_TRIG_SOURCE_SUPERTMR0_EXT_TRIG7 = 7U,
    TMRMUX_TRIG_SOURCE_SUPERTMR1_EXT_TRIG0 = 8U,
    TMRMUX_TRIG_SOURCE_SUPERTMR1_EXT_TRIG1 = 9U,
    TMRMUX_TRIG_SOURCE_SUPERTMR1_EXT_TRIG2 = 10U,
    TMRMUX_TRIG_SOURCE_SUPERTMR1_EXT_TRIG3 = 11U,
    TMRMUX_TRIG_SOURCE_SUPERTMR1_EXT_TRIG4 = 12U,
    TMRMUX_TRIG_SOURCE_SUPERTMR1_EXT_TRIG5 = 13U,
    TMRMUX_TRIG_SOURCE_SUPERTMR1_EXT_TRIG6 = 14U,
    TMRMUX_TRIG_SOURCE_SUPERTMR1_EXT_TRIG7 = 15U,
    TMRMUX_TRIG_SOURCE_SUPERTMR2_EXT_TRIG0 = 16U,
    TMRMUX_TRIG_SOURCE_SUPERTMR2_EXT_TRIG1 = 17U,
    TMRMUX_TRIG_SOURCE_SUPERTMR2_EXT_TRIG2 = 18U,
    TMRMUX_TRIG_SOURCE_SUPERTMR2_EXT_TRIG3 = 19U,
    TMRMUX_TRIG_SOURCE_SUPERTMR2_EXT_TRIG4 = 20U,
    TMRMUX_TRIG_SOURCE_SUPERTMR2_EXT_TRIG5 = 21U,
    TMRMUX_TRIG_SOURCE_SUPERTMR2_EXT_TRIG6 = 22U,
    TMRMUX_TRIG_SOURCE_SUPERTMR2_EXT_TRIG7 = 23U,
    TMRMUX_TRIG_SOURCE_SUPERTMR3_EXT_TRIG0 = 24U,
    TMRMUX_TRIG_SOURCE_SUPERTMR3_EXT_TRIG1 = 25U,
    TMRMUX_TRIG_SOURCE_SUPERTMR3_EXT_TRIG2 = 26U,
    TMRMUX_TRIG_SOURCE_SUPERTMR3_EXT_TRIG3 = 27U,
    TMRMUX_TRIG_SOURCE_SUPERTMR3_EXT_TRIG4 = 28U,
    TMRMUX_TRIG_SOURCE_SUPERTMR3_EXT_TRIG5 = 29U,
    TMRMUX_TRIG_SOURCE_SUPERTMR3_EXT_TRIG6 = 30U,
    TMRMUX_TRIG_SOURCE_SUPERTMR3_EXT_TRIG7 = 31U,
    TMRMUX_TRIG_SOURCE_SUPERTMR4_EXT_TRIG0 = 32U,
    TMRMUX_TRIG_SOURCE_SUPERTMR4_EXT_TRIG1 = 33U,
    TMRMUX_TRIG_SOURCE_SUPERTMR4_EXT_TRIG2 = 34U,
    TMRMUX_TRIG_SOURCE_SUPERTMR4_EXT_TRIG3 = 35U,
    TMRMUX_TRIG_SOURCE_SUPERTMR4_EXT_TRIG4 = 36U,
    TMRMUX_TRIG_SOURCE_SUPERTMR4_EXT_TRIG5 = 37U,
    TMRMUX_TRIG_SOURCE_SUPERTMR4_EXT_TRIG6 = 38U,
    TMRMUX_TRIG_SOURCE_SUPERTMR4_EXT_TRIG7 = 39U,
    TMRMUX_TRIG_SOURCE_SUPERTMR5_EXT_TRIG0 = 40U,
    TMRMUX_TRIG_SOURCE_SUPERTMR5_EXT_TRIG1 = 41U,
    TMRMUX_TRIG_SOURCE_SUPERTMR5_EXT_TRIG2 = 42U,
    TMRMUX_TRIG_SOURCE_SUPERTMR5_EXT_TRIG3 = 43U,
    TMRMUX_TRIG_SOURCE_SUPERTMR5_EXT_TRIG4 = 44U,
    TMRMUX_TRIG_SOURCE_SUPERTMR5_EXT_TRIG5 = 45U,
    TMRMUX_TRIG_SOURCE_SUPERTMR5_EXT_TRIG6 = 46U,
    TMRMUX_TRIG_SOURCE_SUPERTMR5_EXT_TRIG7 = 47U
} tmrmux_trigger_source_t;

/**
 * @brief Enumeration for target module of TMRMUX
 *
 * Describes all possible outputs (target modules) of the TMRMUX IP
 * This enumeration depends on the supported instances in device
 */
typedef enum {
    TMRMUX_TARGET_MODULE_PCTMR0 = 0U,
    TMRMUX_TARGET_MODULE_PCTMR1 = 1U,
    TMRMUX_TARGET_MODULE_PCTMR2 = 2U,
    TMRMUX_TARGET_MODULE_PCTMR3 = 3U,
    TMRMUX_TARGET_MODULE_PCTMR4 = 4U,
    TMRMUX_TARGET_MODULE_PCTMR5 = 5U,
    TMRMUX_TARGET_MODULE_PCTMR6 = 6U,
    TMRMUX_TARGET_MODULE_PCTMR7 = 7U
} tmrmux_target_module_t;

/* TMRMUX index offsets */
#define TMRMUX_PCTMR_INDEX0 (0)
#define TMRMUX_PCTMR_INDEX1 (1)

/* @brief Constant array storing the value of all TMRMUX output(target module) identifiers */
#define FEATURE_TMRMUX_TARGET_MODULE                                                               \
    {                                                                                              \
        TMRMUX_TARGET_MODULE_PCTMR0, TMRMUX_TARGET_MODULE_PCTMR1, TMRMUX_TARGET_MODULE_PCTMR2,     \
            TMRMUX_TARGET_MODULE_PCTMR3, TMRMUX_TARGET_MODULE_PCTMR4, TMRMUX_TARGET_MODULE_PCTMR5, \
            TMRMUX_TARGET_MODULE_PCTMR6, TMRMUX_TARGET_MODULE_PCTMR7                               \
    }

/* PITMR module features */

/** @brief Number of interrupt vector for channels of the PITMR module. */
#define FEATURE_PITMR_HAS_NUM_IRQS_CHANS (4U)
/** @brief Clock names for PITMR. */
#define PITMR_CLOCK_NAMES      \
    {                          \
        PITMR0_CLK, PITMR1_CLK \
    }

/* PCTMR module features */

/* @brief PCTMR pulse counter input options */
#define FEATURE_PCTMR_CLOCKS_NAMES \
    {PCTMR0_CLK, PCTMR1_CLK, PCTMR2_CLK, PCTMR3_CLK, PCTMR4_CLK, PCTMR5_CLK, PCTMR6_CLK, PCTMR7_CLK, PCTMR8_CLK};

/* @brief Supports high speed run mode. */
#define FEATURE_HAS_HIGH_SPEED_RUN_MODE (1U)
/* @brief Supports SPLL clock source. */
#define FEATURE_HAS_SPLL_CLK (1U)

/* SUPERTMR module features */

/* @brief Width of control channel */
#define SUPERTMR_FEATURE_COMBINE_CHAN_CTRL_WIDTH (8U)
/* @brief Output channel offset */
#define SUPERTMR_FEATURE_OUTPUT_CHANNEL_OFFSET (16U)
/* @brief Max counter value */
#define SUPERTMR_FEATURE_CNT_MAX_VALUE_U32 (0x0000FFFFU)
/* @brief Input capture for single shot */
#define SUPERTMR_FEATURE_INPUT_CAPTURE_SINGLE_SHOT (2U)

/* Pins module features */

/* @brief Has open drain control  */
#define FEATURE_PINS_HAS_OPEN_DRAIN (0)
/* @brief Has pull resistor selection available. */
#define FEATURE_PINS_HAS_PULL_SELECTION (1)
/* @brief Has drive strength */
#define FEATURE_PINS_HAS_DRIVE_STRENGTH (1)
/* @brief Has drive strength control bits*/
#define FEATURE_PINS_HAS_DRIVE_STRENGTH_CONTROL (0)

/* FlexCAN module features */

/* @brief The list contains definitions of the FD feature support on all instances */
#define FEATURE_FLEXCAN_INSTANCES_HAS_FD \
    {                                    \
        true, false, true, true          \
    }

/* @brief Maximum number of Message Buffers supported for payload size 8 for FlexCAN0 */
#define FEATURE_FLEXCAN0_MAX_MB_NUM (128U)
/* @brief Maximum number of Message Buffers supported for payload size 8 for FlexCAN1 */
#define FEATURE_FLEXCAN1_MAX_MB_NUM (16U)
/* @brief Maximum number of Message Buffers supported for payload size 8 for FlexCAN2 */
#define FEATURE_FLEXCAN2_MAX_MB_NUM (128U)
/* @brief Maximum number of Message Buffers supported for payload size 8 for FlexCAN3 */
#define FEATURE_FLEXCAN3_MAX_MB_NUM (128U)

/* @brief Maximum number of Message Buffers supported for payload size 8 for any of the FlexCAN instances */
#define FEATURE_FLEXCAN_MAX_MB_NUM (128U)
/* @brief Maximum number of Message Buffers supported for payload size 8 for any of the FlexCAN instances */
#define FEATURE_FLEXCAN_MAX_MB_NUM_ARRAY                                                                                   \
    {                                                                                                                      \
        FEATURE_FLEXCAN0_MAX_MB_NUM, FEATURE_FLEXCAN1_MAX_MB_NUM, FEATURE_FLEXCAN2_MAX_MB_NUM, FEATURE_FLEXCAN3_MAX_MB_NUM \
    }

/** @brief FlexCAN legacy Fifo Embedded RAM address offset */
#define FEATURE_FLEXCAN_RAM_OFFSET (0x00000080U)
/** @brief FlexCAN Enhanced Fifo Embedded RAM address offset */
#define FEATURE_FLEXCAN_ENHANCED_FIFO_RAM_OFFSET (0x00002000U)
/* @brief number of FlexCAN peripheral has Enhanced Rx FIFO mode */
#define FEATURE_FLEXCAN_ENHANCED_RX_FIFO_NUM (3U)

/* @brief the number of normal MB ram block region */
#define FLEXCAN_MBDSR_COUNT_OF_NORMAL_RAM_BLOCK (4U)

/** @brief FlexCAN extra or enhanced MB Embedded RAM address offset */
#define FEATURE_FLEXCAN_EXP_RAM_OFFSET        (0x00001000U)
#define FEATURE_FLEXCAN_EXPANDABLE_MEMORY_NUM (3U)

/* @brief Message Buffers IRQs */
#define FEATURE_FLEXCAN_MB_IRQS CAN_ORed_0_127_MB_IRQS

/* @brief Clock name for the PE oscillator clock source */
#define FEATURE_FLEXCAN_PE_OSC_CLK_NAME SOSC_CLK

#define FLEXCAN_CLOCKS                         \
    {                                          \
        CAN0_CLK, CAN1_CLK, CAN2_CLK, CAN3_CLK \
    }

/* PWM module features */
#define PWM_CLOCKS                                       \
    {                                                    \
        PWM0_CLK, PWM1_CLK, PWM2_CLK, PWM3_CLK, PWM4_CLK \
    }

#define PWM_CLOCKS_FOR_GET_FREQ                          \
    {                                                    \
        CLOCK_NAME_COUNT, MAIN_BUS_MOTOR_CTRL_CLK, MAIN_BUS_AUX_CLK, AON_CLK \
    }

#endif /* __EAM2011_FEATURES_H__ */
