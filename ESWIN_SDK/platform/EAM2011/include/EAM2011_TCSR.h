#ifndef __EAM2011_TCSR_H__
#define __EAM2011_TCSR_H__

#ifndef __ASSEMBLY__
#include <stdint.h>
#endif

/* ----------------------------------------------------------------------------
   -- Generic macros
   ---------------------------------------------------------------------------- */

/* IO definitions (access restrictions to peripheral registers) */
/**
 *   IO Type Qualifiers are used
 *   \li to specify the access to peripheral variables.
 *   \li for automatic generation of peripheral register debug information.
 */
#ifndef __IO
#ifdef __cplusplus
#define __I volatile /** Defines 'read only' permissions                 */
#else
#define __I volatile const /** Defines 'read only' permissions                 */
#endif
#define __O  volatile /** Defines 'write only' permissions                */
#define __IO volatile /** Defines 'read / write' permissions              */
#endif

// clang-format off
/* ----------------------------------------------------------------------------
   -- TCSR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup TCSR_Peripheral_Access_Layer TCSR Peripheral Access Layer
 * @{
 */

/** TCSR - Size of Registers Arrays */
#ifndef __ASSEMBLY__
/**
 * @brief TCSR Register Layout Typedef
 *
 */
typedef struct {
  __IO uint32_t STOP;
  __IO uint32_t STOPMODE;
  __IO uint32_t STOPACK;
  __IO uint32_t DOZE;
  __IO uint32_t DEBUG;
  __IO uint32_t WAIT;
  __IO uint32_t BDM;
  __IO uint32_t CANCONFIG;
  __IO uint32_t ADCCONFIG;
  __IO uint32_t SUPERTMROPT0;
  __IO uint32_t SUPERTMROPT1;
  __IO uint32_t RESERVED_0;
  __IO uint32_t CPUSRAM;
  __IO uint32_t CPUROM;
  __IO uint32_t CPUICACHETAG;
  __IO uint32_t CPUICACHEWAY0;
  __IO uint32_t CPUICACHEWAY1;
  __IO uint32_t CPUDCACHETAG;
  __IO uint32_t CPUDCACHEWAY0;
  __IO uint32_t CPUDCACHEWAY1;
  __IO uint32_t RESERVED_1[3];
  __IO uint32_t CAN0RAM;
  __IO uint32_t CAN1RAM;
  __IO uint32_t CAN2RAM;
  __IO uint32_t PDMARAM;
  __IO uint32_t PDMASTATUS0;
  __IO uint32_t PDMASTATUS1;
  __IO uint32_t PDMASTATUS2;
  __IO uint32_t PDMASTATUS3;
  __IO uint32_t RESERVED_2;
  __IO uint32_t CPUBOOTADDR;
  __IO uint32_t FMCTMEN;
  __IO uint32_t RESERVED_3;
  __IO uint32_t JTAGDEVID;
  __IO uint32_t MPUMASTERID;
  __IO uint32_t HARDID;
  __IO uint32_t CAN0ECCERRADR;
  __IO uint32_t CAN1ECCERRADR;
  __IO uint32_t CAN2ECCERRADR;
  __IO uint32_t CAN3ECCERRADR; // just reserved, no meaning
  __IO uint32_t CAN4ECCERRADR;
  __IO uint32_t CAN5ECCERRADR; // just reserved, no meaning
  __IO uint32_t CANERRORS;
  __IO uint32_t STATUSREG;
  __IO uint32_t BISTREG0;
  __IO uint32_t BISTREG1;
  __IO uint32_t BISTREG2;
  __IO uint32_t CONCTRREG;
  __IO uint32_t RESERVED_4;
  __IO uint32_t RESERVED_5;
  __IO uint32_t MEMORYOCM;
  __IO uint32_t MEMORYNVRAM;
  __IO uint32_t MISCTRL1;
  __IO uint32_t LBISTROM;
  __IO uint32_t LBISTSTATUS;
  __IO uint32_t LBISTECC;
  __IO uint32_t RESERVED_6[2];
  __IO uint32_t CJTAGCTRL;
  __IO uint32_t RESERVED_7[3];
  __IO uint32_t CRCSTATUS;
  __IO uint32_t NVRLOCK;
  __IO uint32_t DFLASH;
  __IO uint32_t P0FLASH;
  __IO uint32_t P1FLASH;
  __IO uint32_t RESERVED_8[3];
  __IO uint32_t CAN3RAM; // just reserved, no meaning
  __IO uint32_t CAN4RAM;
  __IO uint32_t CAN5RAM; // just reserved, no meaning
  __IO uint32_t RESERVED_9;
  __IO uint32_t HWCRCCALVAL;
  __IO uint32_t RESERVED_10[3];
  __IO uint32_t MODECODE;
  __IO uint32_t INTCCONFIG;
  __IO uint32_t MPUCONCTRL;
  __IO uint32_t RESERVED_11[237];
  __IO uint32_t NVRSTATUS0;
  __IO uint32_t NVRSTATUS1;
  __IO uint32_t NVRSTATUS2;
  __IO uint32_t NVRSTATUS3;
  __IO uint32_t NVRSTATUS4;
  __IO uint32_t RESERVED_12;
  __IO uint32_t NVRSTATUS5;
  __IO uint32_t RESERVED_13;
  __IO uint32_t NVRSTATUS6;
  __IO uint32_t NVRSTATUS7;
} tcsr_type_t, *tcsr_mem_map_ptr_t;
#endif

/* ----------------------------------------------------------------------------
   -- TCSR Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup TCSR_Register_Masks TCSR Register Masks
 * @{
 */

/* STOP Bit Fields */
#define TCSR_STOP_SUPERIO_MASK                   0x1U
#define TCSR_STOP_SUPERIO_SHIFT                  0U
#define TCSR_STOP_SUPERIO_WIDTH                  1U
#define TCSR_STOP_SUPERIO(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_SUPERIO_SHIFT))&TCSR_STOP_SUPERIO_MASK)
#define TCSR_STOP_I2C0_MASK                      0x2U
#define TCSR_STOP_I2C0_SHIFT                     1U
#define TCSR_STOP_I2C0_WIDTH                     1U
#define TCSR_STOP_I2C0(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_I2C0_SHIFT))&TCSR_STOP_I2C0_MASK)
#define TCSR_STOP_I2C1_MASK                      0x4U
#define TCSR_STOP_I2C1_SHIFT                     2U
#define TCSR_STOP_I2C1_WIDTH                     1U
#define TCSR_STOP_I2C1(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_I2C1_SHIFT))&TCSR_STOP_I2C1_MASK)
#define TCSR_STOP_UART0_MASK                     0x8U
#define TCSR_STOP_UART0_SHIFT                    3U
#define TCSR_STOP_UART0_WIDTH                    1U
#define TCSR_STOP_UART0(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_UART0_SHIFT))&TCSR_STOP_UART0_MASK)
#define TCSR_STOP_UART1_MASK                     0x10U
#define TCSR_STOP_UART1_SHIFT                    4U
#define TCSR_STOP_UART1_WIDTH                    1U
#define TCSR_STOP_UART1(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_UART1_SHIFT))&TCSR_STOP_UART1_MASK)
#define TCSR_STOP_UART2_MASK                     0x20U
#define TCSR_STOP_UART2_SHIFT                    5U
#define TCSR_STOP_UART2_WIDTH                    1U
#define TCSR_STOP_UART2(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_UART2_SHIFT))&TCSR_STOP_UART2_MASK)
#define TCSR_STOP_UART3_MASK                     0x40U
#define TCSR_STOP_UART3_SHIFT                    6U
#define TCSR_STOP_UART3_WIDTH                    1U
#define TCSR_STOP_UART3(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_UART3_SHIFT))&TCSR_STOP_UART3_MASK)
#define TCSR_STOP_UART4_MASK                     0x80U
#define TCSR_STOP_UART4_SHIFT                    7U
#define TCSR_STOP_UART4_WIDTH                    1U
#define TCSR_STOP_UART4(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_UART4_SHIFT))&TCSR_STOP_UART4_MASK)
#define TCSR_STOP_UART5_MASK                     0x100U
#define TCSR_STOP_UART5_SHIFT                    8U
#define TCSR_STOP_UART5_WIDTH                    1U
#define TCSR_STOP_UART5(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_UART5_SHIFT))&TCSR_STOP_UART5_MASK)
#define TCSR_STOP_SPI0_MASK                      0x200U
#define TCSR_STOP_SPI0_SHIFT                     9U
#define TCSR_STOP_SPI0_WIDTH                     1U
#define TCSR_STOP_SPI0(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_SPI0_SHIFT))&TCSR_STOP_SPI0_MASK)
#define TCSR_STOP_SPI1_MASK                      0x400U
#define TCSR_STOP_SPI1_SHIFT                     10U
#define TCSR_STOP_SPI1_WIDTH                     1U
#define TCSR_STOP_SPI1(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_SPI1_SHIFT))&TCSR_STOP_SPI1_MASK)
#define TCSR_STOP_SPI2_MASK                      0x800U
#define TCSR_STOP_SPI2_SHIFT                     11U
#define TCSR_STOP_SPI2_WIDTH                     1U
#define TCSR_STOP_SPI2(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_SPI2_SHIFT))&TCSR_STOP_SPI2_MASK)
#define TCSR_STOP_SPI3_MASK                      0x1000U
#define TCSR_STOP_SPI3_SHIFT                     12U
#define TCSR_STOP_SPI3_WIDTH                     1U
#define TCSR_STOP_SPI3(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_SPI3_SHIFT))&TCSR_STOP_SPI3_MASK)
#define TCSR_STOP_PWM0_MASK                      0x2000U
#define TCSR_STOP_PWM0_SHIFT                     13U
#define TCSR_STOP_PWM0_WIDTH                     1U
#define TCSR_STOP_PWM0(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_SPI0_SHIFT))&TCSR_STOP_SPI0_MASK)
#define TCSR_STOP_PWM1_MASK                      0x4000U
#define TCSR_STOP_PWM1_SHIFT                     14U
#define TCSR_STOP_PWM1_WIDTH                     1U
#define TCSR_STOP_PWM1(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_SPI1_SHIFT))&TCSR_STOP_SPI1_MASK)
#define TCSR_STOP_PWM2_MASK                      0x8000U
#define TCSR_STOP_PWM2_SHIFT                     15U
#define TCSR_STOP_PWM2_WIDTH                     1U
#define TCSR_STOP_PWM2(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_SPI2_SHIFT))&TCSR_STOP_SPI2_MASK)
#define TCSR_STOP_PWM3_MASK                      0x10000U
#define TCSR_STOP_PWM3_SHIFT                     16U
#define TCSR_STOP_PWM3_WIDTH                     1U
#define TCSR_STOP_PWM3(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_SPI3_SHIFT))&TCSR_STOP_SPI3_MASK)
#define TCSR_STOP_PWM4_MASK                      0x20000U
#define TCSR_STOP_PWM4_SHIFT                     17U
#define TCSR_STOP_PWM4_WIDTH                     1U
#define TCSR_STOP_PWM4(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_PWM4_SHIFT))&TCSR_STOP_PWM4_MASK)
#define TCSR_STOP_CMP0_MASK                      0x10000000U
#define TCSR_STOP_CMP0_SHIFT                     28U
#define TCSR_STOP_CMP0_WIDTH                     1U
#define TCSR_STOP_CMP0(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_CMP0_SHIFT))&TCSR_STOP_CMP0_MASK)
#define TCSR_STOP_EWM_MASK                       0x40000000U
#define TCSR_STOP_EWM_SHIFT                      30U
#define TCSR_STOP_EWM_WIDTH                      1U
#define TCSR_STOP_EWM(x)                         (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_EWM_SHIFT))&TCSR_STOP_EWM_MASK)
#define TCSR_STOP_WDG_MASK                       0x80000000U
#define TCSR_STOP_WDG_SHIFT                      31U
#define TCSR_STOP_WDG_WIDTH                      1U
#define TCSR_STOP_WDG(x)                         (((uint32_t)(((uint32_t)(x))<<TCSR_STOP_WDG_SHIFT))&TCSR_STOP_WDG_MASK)

/* STOPMODE Bit Fields */
#define TCSR_STOPMODE_SUPERIO_MASK                0x1U
#define TCSR_STOPMODE_SUPERIO_SHIFT               0U
#define TCSR_STOPMODE_SUPERIO_WIDTH               1U
#define TCSR_STOPMODE_SUPERIO(x)                  (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_SUPERIO_SHIFT))&TCSR_STOPMODE_SUPERIO_MASK)
#define TCSR_STOPMODE_I2C0_MASK                   0x2U
#define TCSR_STOPMODE_I2C0_SHIFT                  1U
#define TCSR_STOPMODE_I2C0_WIDTH                  1U
#define TCSR_STOPMODE_I2C0(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_I2C0_SHIFT))&TCSR_STOPMODE_I2C0_MASK)
#define TCSR_STOPMODE_I2C1_MASK                   0x4U
#define TCSR_STOPMODE_I2C1_SHIFT                  2U
#define TCSR_STOPMODE_I2C1_WIDTH                  1U
#define TCSR_STOPMODE_I2C1(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_I2C1_SHIFT))&TCSR_STOPMODE_I2C1_MASK)
#define TCSR_STOPMODE_UART0_MASK                  0x8U
#define TCSR_STOPMODE_UART0_SHIFT                 3U
#define TCSR_STOPMODE_UART0_WIDTH                 1U
#define TCSR_STOPMODE_UART0(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_UART0_SHIFT))&TCSR_STOPMODE_UART0_MASK)
#define TCSR_STOPMODE_UART1_MASK                  0x10U
#define TCSR_STOPMODE_UART1_SHIFT                 4U
#define TCSR_STOPMODE_UART1_WIDTH                 1U
#define TCSR_STOPMODE_UART1(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_UART1_SHIFT))&TCSR_STOPMODE_UART1_MASK)
#define TCSR_STOPMODE_UART2_MASK                  0x20U
#define TCSR_STOPMODE_UART2_SHIFT                 5U
#define TCSR_STOPMODE_UART2_WIDTH                 1U
#define TCSR_STOPMODE_UART2(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_UART2_SHIFT))&TCSR_STOPMODE_UART2_MASK)
#define TCSR_STOPMODE_UART3_MASK                  0x40U
#define TCSR_STOPMODE_UART3_SHIFT                 6U
#define TCSR_STOPMODE_UART3_WIDTH                 1U
#define TCSR_STOPMODE_UART3(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_UART3_SHIFT))&TCSR_STOPMODE_UART3_MASK)
#define TCSR_STOPMODE_UART4_MASK                  0x80U
#define TCSR_STOPMODE_UART4_SHIFT                 7U
#define TCSR_STOPMODE_UART4_WIDTH                 1U
#define TCSR_STOPMODE_UART4(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_UART4_SHIFT))&TCSR_STOPMODE_UART4_MASK)
#define TCSR_STOPMODE_UART5_MASK                  0x100U
#define TCSR_STOPMODE_UART5_SHIFT                 8U
#define TCSR_STOPMODE_UART5_WIDTH                 1U
#define TCSR_STOPMODE_UART5(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_UART5_SHIFT))&TCSR_STOPMODE_UART5_MASK)
#define TCSR_STOPMODE_SPI0_MASK                   0x200U
#define TCSR_STOPMODE_SPI0_SHIFT                  9U
#define TCSR_STOPMODE_SPI0_WIDTH                  1U
#define TCSR_STOPMODE_SPI0(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_SPI0_SHIFT))&TCSR_STOPMODE_SPI0_MASK)
#define TCSR_STOPMODE_SPI1_MASK                   0x400U
#define TCSR_STOPMODE_SPI1_SHIFT                  10U
#define TCSR_STOPMODE_SPI1_WIDTH                  1U
#define TCSR_STOPMODE_SPI1(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_SPI1_SHIFT))&TCSR_STOPMODE_SPI1_MASK)
#define TCSR_STOPMODE_SPI2_MASK                   0x800U
#define TCSR_STOPMODE_SPI2_SHIFT                  11U
#define TCSR_STOPMODE_SPI2_WIDTH                  1U
#define TCSR_STOPMODE_SPI2(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_SPI2_SHIFT))&TCSR_STOPMODE_SPI2_MASK)
#define TCSR_STOPMODE_SPI3_MASK                   0x1000U
#define TCSR_STOPMODE_SPI3_SHIFT                  12U
#define TCSR_STOPMODE_SPI3_WIDTH                  1U
#define TCSR_STOPMODE_SPI3(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_SPI3_SHIFT))&TCSR_STOPMODE_SPI3_MASK)
#define TCSR_STOPMODE_DMA_MASK                    0x20000000U
#define TCSR_STOPMODE_DMA_SHIFT                   29U
#define TCSR_STOPMODE_DMA_WIDTH                   1U
#define TCSR_STOPMODE_DMA(x)                      (((uint32_t)(((uint32_t)(x))<<TCSR_STOPMODE_DMA_SHIFT))&TCSR_STOPMODE_DMA_MASK)

/* DZONE Bit Fields */
#define TCSR_DZONE_SUPERIO_MASK                      0x1U
#define TCSR_DZONE_SUPERIO_SHIFT                     0U
#define TCSR_DZONE_SUPERIO_WIDTH                     1U
#define TCSR_DZONE_SUPERIO(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SUPERIO_SHIFT))&TCSR_DZONE_SUPERIO_MASK)
#define TCSR_DZONE_I2C0_MASK                        0x2U
#define TCSR_DZONE_I2C0_SHIFT                       1U
#define TCSR_DZONE_I2C0_WIDTH                       1U
#define TCSR_DZONE_I2C0(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_I2C0_SHIFT))&TCSR_DZONE_I2C0_MASK)
#define TCSR_DZONE_I2C1_MASK                        0x4U
#define TCSR_DZONE_I2C1_SHIFT                       2U
#define TCSR_DZONE_I2C1_WIDTH                       1U
#define TCSR_DZONE_I2C1(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_I2C1_SHIFT))&TCSR_DZONE_I2C1_MASK)
#define TCSR_DZONE_UART0_MASK                     0x8U
#define TCSR_DZONE_UART0_SHIFT                    3U
#define TCSR_DZONE_UART0_WIDTH                    1U
#define TCSR_DZONE_UART0(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_UART0_SHIFT))&TCSR_DZONE_UART0_MASK)
#define TCSR_DZONE_UART1_MASK                     0x10U
#define TCSR_DZONE_UART1_SHIFT                    4U
#define TCSR_DZONE_UART1_WIDTH                    1U
#define TCSR_DZONE_UART1(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_UART1_SHIFT))&TCSR_DZONE_UART1_MASK)
#define TCSR_DZONE_UART2_MASK                     0x20U
#define TCSR_DZONE_UART2_SHIFT                    5U
#define TCSR_DZONE_UART2_WIDTH                    1U
#define TCSR_DZONE_UART2(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_UART2_SHIFT))&TCSR_DZONE_UART2_MASK)
#define TCSR_DZONE_UART3_MASK                     0x40U
#define TCSR_DZONE_UART3_SHIFT                    6U
#define TCSR_DZONE_UART3_WIDTH                    1U
#define TCSR_DZONE_UART3(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_UART3_SHIFT))&TCSR_DZONE_UART3_MASK)
#define TCSR_DZONE_UART4_MASK                     0x80U
#define TCSR_DZONE_UART4_SHIFT                    7U
#define TCSR_DZONE_UART4_WIDTH                    1U
#define TCSR_DZONE_UART4(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_UART4_SHIFT))&TCSR_DZONE_UART4_MASK)
#define TCSR_DZONE_UART5_MASK                     0x100U
#define TCSR_DZONE_UART5_SHIFT                    8U
#define TCSR_DZONE_UART5_WIDTH                    1U
#define TCSR_DZONE_UART5(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_UART5_SHIFT))&TCSR_DZONE_UART5_MASK)
#define TCSR_DZONE_SPI0_MASK                        0x200U
#define TCSR_DZONE_SPI0_SHIFT                       9U
#define TCSR_DZONE_SPI0_WIDTH                       1U
#define TCSR_DZONE_SPI0(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI0_SHIFT))&TCSR_DZONE_SPI0_MASK)
#define TCSR_DZONE_SPI1_MASK                        0x400U
#define TCSR_DZONE_SPI1_SHIFT                       10U
#define TCSR_DZONE_SPI1_WIDTH                       1U
#define TCSR_DZONE_SPI1(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI1_SHIFT))&TCSR_DZONE_SPI1_MASK)
#define TCSR_DZONE_SPI2_MASK                        0x800U
#define TCSR_DZONE_SPI2_SHIFT                       11U
#define TCSR_DZONE_SPI2_WIDTH                       1U
#define TCSR_DZONE_SPI2(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI2_SHIFT))&TCSR_DZONE_SPI2_MASK)
#define TCSR_DZONE_SPI3_MASK                        0x1000U
#define TCSR_DZONE_SPI3_SHIFT                       12U
#define TCSR_DZONE_SPI3_WIDTH                       1U
#define TCSR_DZONE_SPI3(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI3_SHIFT))&TCSR_DZONE_SPI3_MASK)
#define TCSR_DZONE_PWM0_MASK                        0x2000U
#define TCSR_DZONE_PWM0_SHIFT                       13U
#define TCSR_DZONE_PWM0_WIDTH                       1U
#define TCSR_DZONE_PWM0(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI0_SHIFT))&TCSR_DZONE_SPI0_MASK)
#define TCSR_DZONE_PWM1_MASK                        0x4000U
#define TCSR_DZONE_PWM1_SHIFT                       14U
#define TCSR_DZONE_PWM1_WIDTH                       1U
#define TCSR_DZONE_PWM1(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI1_SHIFT))&TCSR_DZONE_SPI1_MASK)
#define TCSR_DZONE_PWM2_MASK                        0x8000U
#define TCSR_DZONE_PWM2_SHIFT                       15U
#define TCSR_DZONE_PWM2_WIDTH                       1U
#define TCSR_DZONE_PWM2(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI2_SHIFT))&TCSR_DZONE_SPI2_MASK)
#define TCSR_DZONE_PWM3_MASK                        0x10000U
#define TCSR_DZONE_PWM3_SHIFT                       16U
#define TCSR_DZONE_PWM3_WIDTH                       1U
#define TCSR_DZONE_PWM3(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI3_SHIFT))&TCSR_DZONE_SPI3_MASK)
#define TCSR_DZONE_PWM4_MASK                        0x20000U
#define TCSR_DZONE_PWM4_SHIFT                       17U
#define TCSR_DZONE_PWM4_WIDTH                       1U
#define TCSR_DZONE_PWM4(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_PWM4_SHIFT))&TCSR_DZONE_PWM4_MASK)
#define TCSR_DZONE_PITMR_MASK                       0x40000U
#define TCSR_DZONE_PITMR_SHIFT                      18U
#define TCSR_DZONE_PITMR_WIDTH                      1U
#define TCSR_DZONE_PITMR(x)                         (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_PITMR_SHIFT))&TCSR_DZONE_PITMR_MASK)
#define TCSR_DZONE_DMA_PITMR_MASK                   0x40000000U
#define TCSR_DZONE_DMA_PITMR_SHIFT                  30U
#define TCSR_DZONE_DMA_PITMR_WIDTH                  1U
#define TCSR_DZONE_DMA_PITMR(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_DMA_PITMR_SHIFT))&TCSR_DZONE_DMA_PITMR_MASK)
#define TCSR_DZONE_WDG_MASK                         0x80000000U
#define TCSR_DZONE_WDG_SHIFT                        31U
#define TCSR_DZONE_WDG_WIDTH                        1U
#define TCSR_DZONE_WDG(x)                           (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_WDG_SHIFT))&TCSR_DZONE_WDG_MASK)

/* DEBUG Bit Fields */
#define TCSR_DEBUG_SUPERIO_MASK                      0x1U
#define TCSR_DEBUG_SUPERIO_SHIFT                     0U
#define TCSR_DEBUG_SUPERIO_WIDTH                     1U
#define TCSR_DEBUG_SUPERIO(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SUPERIO_SHIFT))&TCSR_DZONE_SUPERIO_MASK)
#define TCSR_DEBUG_I2C0_MASK                        0x2U
#define TCSR_DEBUG_I2C0_SHIFT                       1U
#define TCSR_DEBUG_I2C0_WIDTH                       1U
#define TCSR_DEBUG_I2C0(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_I2C0_SHIFT))&TCSR_DZONE_I2C0_MASK)
#define TCSR_DEBUG_I2C1_MASK                        0x4U
#define TCSR_DEBUG_I2C1_SHIFT                       2U
#define TCSR_DEBUG_I2C1_WIDTH                       1U
#define TCSR_DEBUG_I2C1(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_I2C1_SHIFT))&TCSR_DZONE_I2C1_MASK)
#define TCSR_DEBUG_SPI0_MASK                        0x200U
#define TCSR_DEBUG_SPI0_SHIFT                       9U
#define TCSR_DEBUG_SPI0_WIDTH                       1U
#define TCSR_DEBUG_SPI0(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI0_SHIFT))&TCSR_DZONE_SPI0_MASK)
#define TCSR_DEBUG_SPI1_MASK                        0x400U
#define TCSR_DEBUG_SPI1_SHIFT                       10U
#define TCSR_DEBUG_SPI1_WIDTH                       1U
#define TCSR_DEBUG_SPI1(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI1_SHIFT))&TCSR_DZONE_SPI1_MASK)
#define TCSR_DEBUG_SPI2_MASK                        0x800U
#define TCSR_DEBUG_SPI2_SHIFT                       11U
#define TCSR_DEBUG_SPI2_WIDTH                       1U
#define TCSR_DEBUG_SPI2(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI2_SHIFT))&TCSR_DZONE_SPI2_MASK)
#define TCSR_DEBUG_SPI3_MASK                        0x1000U
#define TCSR_DEBUG_SPI3_SHIFT                       12U
#define TCSR_DEBUG_SPI3_WIDTH                       1U
#define TCSR_DEBUG_SPI3(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI3_SHIFT))&TCSR_DZONE_SPI3_MASK)
#define TCSR_DEBUG_PWM0_MASK                        0x2000U
#define TCSR_DEBUG_PWM0_SHIFT                       13U
#define TCSR_DEBUG_PWM0_WIDTH                       1U
#define TCSR_DEBUG_PWM0(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI0_SHIFT))&TCSR_DZONE_SPI0_MASK)
#define TCSR_DEBUG_PWM1_MASK                        0x4000U
#define TCSR_DEBUG_PWM1_SHIFT                       14U
#define TCSR_DEBUG_PWM1_WIDTH                       1U
#define TCSR_DEBUG_PWM1(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI1_SHIFT))&TCSR_DZONE_SPI1_MASK)
#define TCSR_DEBUG_PWM2_MASK                        0x8000U
#define TCSR_DEBUG_PWM2_SHIFT                       15U
#define TCSR_DEBUG_PWM2_WIDTH                       1U
#define TCSR_DEBUG_PWM2(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI2_SHIFT))&TCSR_DZONE_SPI2_MASK)
#define TCSR_DEBUG_PWM3_MASK                        0x10000U
#define TCSR_DEBUG_PWM3_SHIFT                       16U
#define TCSR_DEBUG_PWM3_WIDTH                       1U
#define TCSR_DEBUG_PWM3(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_SPI3_SHIFT))&TCSR_DZONE_SPI3_MASK)
#define TCSR_DEBUG_PWM4_MASK                        0x20000U
#define TCSR_DEBUG_PWM4_SHIFT                       17U
#define TCSR_DEBUG_PWM4_WIDTH                       1U
#define TCSR_DEBUG_PWM4(x)                          (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_PWM4_SHIFT))&TCSR_DZONE_PWM4_MASK)
#define TCSR_DEBUG_PITMR_MASK                       0x40000U
#define TCSR_DEBUG_PITMR_SHIFT                      18U
#define TCSR_DEBUG_PITMR_WIDTH                      1U
#define TCSR_DEBUG_PITMR(x)                         (((uint32_t)(((uint32_t)(x))<<TCSR_DZONE_PITMR_SHIFT))&TCSR_DZONE_PITMR_MASK)
#define TCSR_DEBUG_PCTMR0_MASK                      0x80000U
#define TCSR_DEBUG_PCTMR0_SHIFT                     19U
#define TCSR_DEBUG_PCTMR0_WIDTH                     1U
#define TCSR_DEBUG_PCTMR0(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_PCTMR0_SHIFT))&TCSR_DEBUG_PCTMR0_MASK)
#define TCSR_DEBUG_PCTMR1_MASK                      0x100000U
#define TCSR_DEBUG_PCTMR1_SHIFT                     20U
#define TCSR_DEBUG_PCTMR1_WIDTH                     1U
#define TCSR_DEBUG_PCTMR1(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_PCTMR1_SHIFT))&TCSR_DEBUG_PCTMR1_MASK)
#define TCSR_DEBUG_PCTMR2_MASK                      0x200000U
#define TCSR_DEBUG_PCTMR2_SHIFT                     21U
#define TCSR_DEBUG_PCTMR2_WIDTH                     1U
#define TCSR_DEBUG_PCTMR2(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_PCTMR2_SHIFT))&TCSR_DEBUG_PCTMR2_MASK)
#define TCSR_DEBUG_PCTMR3_MASK                      0x400000U
#define TCSR_DEBUG_PCTMR3_SHIFT                     22U
#define TCSR_DEBUG_PCTMR3_WIDTH                     1U
#define TCSR_DEBUG_PCTMR3(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_PCTMR3_SHIFT))&TCSR_DEBUG_PCTMR3_MASK)
#define TCSR_DEBUG_PCTMR4_MASK                      0x800000U
#define TCSR_DEBUG_PCTMR4_SHIFT                     23U
#define TCSR_DEBUG_PCTMR4_WIDTH                     1U
#define TCSR_DEBUG_PCTMR4(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_PCTMR4_SHIFT))&TCSR_DEBUG_PCTMR4_MASK)
#define TCSR_DEBUG_PCTMR5_MASK                      0x1000000U
#define TCSR_DEBUG_PCTMR5_SHIFT                     24U
#define TCSR_DEBUG_PCTMR5_WIDTH                     1U
#define TCSR_DEBUG_PCTMR5(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_PCTMR5_SHIFT))&TCSR_DEBUG_PCTMR5_MASK)
#define TCSR_DEBUG_PCTMR6_MASK                      0x2000000U
#define TCSR_DEBUG_PCTMR6_SHIFT                     25U
#define TCSR_DEBUG_PCTMR6_WIDTH                     1U
#define TCSR_DEBUG_PCTMR6(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_PCTMR6_SHIFT))&TCSR_DEBUG_PCTMR6_MASK)
#define TCSR_DEBUG_PCTMR7_MASK                      0x4000000U
#define TCSR_DEBUG_PCTMR7_SHIFT                     26U
#define TCSR_DEBUG_PCTMR7_WIDTH                     1U
#define TCSR_DEBUG_PCTMR7(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_PCTMR7_SHIFT))&TCSR_DEBUG_PCTMR7_MASK)
#define TCSR_DEBUG_PCTMR8_MASK                      0x8000000U
#define TCSR_DEBUG_PCTMR8_SHIFT                     27U
#define TCSR_DEBUG_PCTMR8_WIDTH                     1U
#define TCSR_DEBUG_PCTMR8(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_PCTMR8_SHIFT))&TCSR_DEBUG_PCTMR8_MASK)
#define TCSR_DEBUG_DAM_MASK                         0x20000000U
#define TCSR_DEBUG_DAM_SHIFT                        29U
#define TCSR_DEBUG_DAM_WIDTH                        1U
#define TCSR_DEBUG_DAM(x)                           (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_DAM_SHIFT))&TCSR_DEBUG_DAM_MASK)
#define TCSR_DEBUG_DAM_PITMR_MASK                   0x40000000U
#define TCSR_DEBUG_DAM_PITMR_SHIFT                  30U
#define TCSR_DEBUG_DAM_PITMR_WIDTH                  1U
#define TCSR_DEBUG_DAM_PITMR(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_DAM_PITMR_SHIFT))&TCSR_DEBUG_DAM_PITMR_MASK)
#define TCSR_DEBUG_WDG_MASK                         0x80000000U
#define TCSR_DEBUG_WDG_SHIFT                        31U
#define TCSR_DEBUG_WDG_WIDTH                        1U
#define TCSR_DEBUG_WDG(x)                           (((uint32_t)(((uint32_t)(x))<<TCSR_DEBUG_WDG_SHIFT))&TCSR_DEBUG_WDG_MASK)

/* WAITMODE Bit Fields */
#define TCSR_WAITMODE_PWM0_MASK                     0x2000U
#define TCSR_WAITMODE_PWM0_SHIFT                    13U
#define TCSR_WAITMODE_PWM0_WIDTH                    1U
#define TCSR_WAITMODE_PWM0(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_WAITMODE_PWM0_SHIFT))&TCSR_WAITMODE_PWM0_MASK)
#define TCSR_WAITMODE_PWM1_MASK                     0x4000U
#define TCSR_WAITMODE_PWM1_SHIFT                    14U
#define TCSR_WAITMODE_PWM1_WIDTH                    1U
#define TCSR_WAITMODE_PWM1(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_WAITMODE_PWM1_SHIFT))&TCSR_WAITMODE_PWM1_MASK)
#define TCSR_WAITMODE_PWM2_MASK                     0x8000U
#define TCSR_WAITMODE_PWM2_SHIFT                    15U
#define TCSR_WAITMODE_PWM2_WIDTH                    1U
#define TCSR_WAITMODE_PWM2(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_WAITMODE_PWM2_SHIFT))&TCSR_WAITMODE_PWM2_MASK)
#define TCSR_WAITMODE_PWM3_MASK                     0x10000U
#define TCSR_WAITMODE_PWM3_SHIFT                    16U
#define TCSR_WAITMODE_PWM3_WIDTH                    1U
#define TCSR_WAITMODE_PWM3(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_WAITMODE_PWM3_SHIFT))&TCSR_WAITMODE_PWM3_MASK)
#define TCSR_WAITMODE_PWM4_MASK                     0x20000U
#define TCSR_WAITMODE_PWM4_SHIFT                    17U
#define TCSR_WAITMODE_PWM4_WIDTH                    1U
#define TCSR_WAITMODE_PWM4(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_WAITMODE_PWM4_SHIFT))&TCSR_WAITMODE_PWM4_MASK)
#define TCSR_WAITMODE_WDG_MASK                      0x80000000U
#define TCSR_WAITMODE_WDG_SHIFT                     31U
#define TCSR_WAITMODE_WDG_WIDTH                     1U
#define TCSR_WAITMODE_WDG(x)                        (((uint32_t)(((uint32_t)(x))<<TCSR_WAITMODE_WDG_SHIFT))&TCSR_WAITMODE_WDG_MASK)

/* BDM Bit Fields */

/* CANCONFIG Bit Fields */
#define TCSR_CANCONFIG_IPG_DEBUG_MASK               0x3FU
#define TCSR_CANCONFIG_IPG_DEBUG_SHIFT              0U
#define TCSR_CANCONFIG_IPG_DEBUG_WIDTH              6U
#define TCSR_CANCONFIG_IPG_DEBUG(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_CANCONFIG_IPG_DEBUG_SHIFT))&TCSR_CANCONFIG_IPG_DEBUG_MASK)
#define TCSR_CANCONFIG_IPG_STOP_MASK                0xFC0U
#define TCSR_CANCONFIG_IPG_STOP_SHIFT               6U
#define TCSR_CANCONFIG_IPG_STOP_WIDTH               6U
#define TCSR_CANCONFIG_IPG_STOP(x)                  (((uint32_t)(((uint32_t)(x))<<TCSR_CANCONFIG_IPG_STOP_SHIFT))&TCSR_CANCONFIG_IPG_STOP_MASK)
#define TCSR_CANCONFIG_IPG_DOZE_MASK                0x3F000U
#define TCSR_CANCONFIG_IPG_DOZE_SHIFT               12U
#define TCSR_CANCONFIG_IPG_DOZE_WIDTH               6U
#define TCSR_CANCONFIG_IPG_DOZE(x)                  (((uint32_t)(((uint32_t)(x))<<TCSR_CANCONFIG_IPG_DOZE_SHIFT))&TCSR_CANCONFIG_IPG_DOZE_MASK)
#define TCSR_CAN4_IPG_STOPACK_MASK                  0x8000000U
#define TCSR_CAN4_IPG_STOPACK_SHIFT                 27U
#define TCSR_CAN4_IPG_STOPACK_WIDTH                 1U
#define TCSR_CAN4_IPG_STOPACK(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_CAN4_IPG_STOPACK_SHIFT))&TCSR_CAN4_IPG_STOPACK_MASK)
#define TCSR_CAN3_IPG_STOPACK_MASK                  0x10000000U
#define TCSR_CAN3_IPG_STOPACK_SHIFT                 28U
#define TCSR_CAN3_IPG_STOPACK_WIDTH                 1U
#define TCSR_CAN3_IPG_STOPACK(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_CAN3_IPG_STOPACK_SHIFT))&TCSR_CAN3_IPG_STOPACK_MASK)
#define TCSR_CAN2_IPG_STOPACK_MASK                  0x20000000U
#define TCSR_CAN2_IPG_STOPACK_SHIFT                 29U
#define TCSR_CAN2_IPG_STOPACK_WIDTH                 1U
#define TCSR_CAN2_IPG_STOPACK(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_CAN2_IPG_STOPACK_SHIFT))&TCSR_CAN2_IPG_STOPACK_MASK)
#define TCSR_CAN1_IPG_STOPACK_MASK                  0x40000000U
#define TCSR_CAN1_IPG_STOPACK_SHIFT                 30U
#define TCSR_CAN1_IPG_STOPACK_WIDTH                 1U
#define TCSR_CAN1_IPG_STOPACK(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_CAN1_IPG_STOPACK_SHIFT))&TCSR_CAN1_IPG_STOPACK_MASK)
#define TCSR_CAN0_IPG_STOPACK_MASK                  0x80000000U
#define TCSR_CAN0_IPG_STOPACK_SHIFT                 31U
#define TCSR_CAN0_IPG_STOPACK_WIDTH                 1U
#define TCSR_CAN0_IPG_STOPACK(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_CAN0_IPG_STOPACK_SHIFT))&TCSR_CAN0_IPG_STOPACK_MASK)

/* ADCCONFIG Bit Fields */
#define TCSR_ADCCONFIG_PDUBBSEL_MASK                0x1U
#define TCSR_ADCCONFIG_PDUBBSEL_SHIFT               0U
#define TCSR_ADCCONFIG_PDUBBSEL_WIDTH               1U
#define TCSR_ADCCONFIG_PDUBBSEL(x)                  (((uint32_t)(((uint32_t)(x))<<TCSR_ADCCONFIG_PDUBBSEL_SHIFT))&TCSR_ADCCONFIG_PDUBBSEL_MASK)
#define TCSR_ADCCONFIG_PDUBBSEL_1_MASK              0x2U
#define TCSR_ADCCONFIG_PDUBBSEL_1_SHIFT             1U
#define TCSR_ADCCONFIG_PDUBBSEL_1_WIDTH             1U
#define TCSR_ADCCONFIG_PDUBBSEL_1(x)                (((uint32_t)(((uint32_t)(x))<<TCSR_ADCCONFIG_PDUBBSEL_1_SHIFT))&TCSR_ADCCONFIG_PDUBBSEL_1_MASK)
#define TCSR_ADCCONFIG_PDUBBSEL_2_MASK              0x4U
#define TCSR_ADCCONFIG_PDUBBSEL_2_SHIFT             2U
#define TCSR_ADCCONFIG_PDUBBSEL_2_WIDTH             1U
#define TCSR_ADCCONFIG_PDUBBSEL_2(x)                (((uint32_t)(((uint32_t)(x))<<TCSR_ADCCONFIG_PDUBBSEL_2_SHIFT))&TCSR_ADCCONFIG_PDUBBSEL_2_MASK)
#define TCSR_ADCCONFIG_PDU1DEBUG_MASK               0x1U
#define TCSR_ADCCONFIG_PDU1DEBUG_SHIFT              14U
#define TCSR_ADCCONFIG_PDU1DEBUG_WIDTH              1U
#define TCSR_ADCCONFIG_PDU1DEBUG(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_ADCCONFIG_PDU1DEBUG_SHIFT))&TCSR_ADCCONFIG_PDU1DEBUG_MASK)
#define TCSR_ADCCONFIG_PDU0DEBUG_MASK               0x1U
#define TCSR_ADCCONFIG_PDU0DEBUG_SHIFT              14U
#define TCSR_ADCCONFIG_PDU0DEBUG_WIDTH              1U
#define TCSR_ADCCONFIG_PDU0DEBUG(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_ADCCONFIG_PDU0DEBUG_SHIFT))&TCSR_ADCCONFIG_PDU0DEBUG_MASK)
#define TCSR_ADCOPT_ADC0TRGSEL_MASK                 0x10000U
#define TCSR_ADCOPT_ADC0TRGSEL_SHIFT                16U
#define TCSR_ADCOPT_ADC0TRGSEL_WIDTH                1U
#define TCSR_ADCOPT_ADC0TRGSEL(x)                   (((uint32_t)(((uint32_t)(x))<<TCSR_ADCOPT_ADC0TRGSEL_SHIFT))&TCSR_ADCOPT_ADC0TRGSEL_MASK)
#define TCSR_ADCOPT_ADC0SWPRETRG_MASK               0xe0000U
#define TCSR_ADCOPT_ADC0SWPRETRG_SHIFT              17U
#define TCSR_ADCOPT_ADC0SWPRETRG_WIDTH              3U
#define TCSR_ADCOPT_ADC0SWPRETRG(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_ADCOPT_ADC0SWPRETRG_SHIFT))&TCSR_ADCOPT_ADC0SWPRETRG_MASK)
#define TCSR_ADCOPT_ADC0PRETRGSEL_MASK              0x300000U
#define TCSR_ADCOPT_ADC0PRETRGSEL_SHIFT             20U
#define TCSR_ADCOPT_ADC0PRETRGSEL_WIDTH             2U
#define TCSR_ADCOPT_ADC0PRETRGSEL(x)                (((uint32_t)(((uint32_t)(x))<<TCSR_ADCOPT_ADC0PRETRGSEL_SHIFT))&TCSR_ADCOPT_ADC0PRETRGSEL_MASK)
#define TCSR_ADCOPT_ADC1TRGSEL_MASK                 0x1000000U
#define TCSR_ADCOPT_ADC1TRGSEL_SHIFT                24U
#define TCSR_ADCOPT_ADC1TRGSEL_WIDTH                1U
#define TCSR_ADCOPT_ADC1TRGSEL(x)                   (((uint32_t)(((uint32_t)(x))<<TCSR_ADCOPT_ADC1TRGSEL_SHIFT))&TCSR_ADCOPT_ADC1TRGSEL_MASK)
#define TCSR_ADCOPT_ADC1SWPRETRG_MASK               0xe000000U
#define TCSR_ADCOPT_ADC1SWPRETRG_SHIFT              25U
#define TCSR_ADCOPT_ADC1SWPRETRG_WIDTH              3U
#define TCSR_ADCOPT_ADC1SWPRETRG(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_ADCOPT_ADC1SWPRETRG_SHIFT))&TCSR_ADCOPT_ADC1SWPRETRG_MASK)
#define TCSR_ADCOPT_ADC1PRETRGSEL_MASK              0x30000000U
#define TCSR_ADCOPT_ADC1PRETRGSEL_SHIFT             28U
#define TCSR_ADCOPT_ADC1PRETRGSEL_WIDTH             2U
#define TCSR_ADCOPT_ADC1PRETRGSEL(x)                (((uint32_t)(((uint32_t)(x))<<TCSR_ADCOPT_ADC1PRETRGSEL_SHIFT))&TCSR_ADCOPT_ADC1PRETRGSEL_MASK)

/* SUPERTMROPT0 Bit Fields */
#define TCSR_SUPERTMROPT0_OPT_MASK                       0xFFFFFFFFU
#define TCSR_SUPERTMROPT0_OPT_SHIFT                      0U
#define TCSR_SUPERTMROPT0_OPT_WIDTH                      32U
#define TCSR_SUPERTMROPT0_OPT(x)                         (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_OPT_SHIFT))&TCSR_SUPERTMROPT0_OPT_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR3_CLKSEL_MASK          0xC0000000U
#define TCSR_SUPERTMROPT0_SUPERTMR3_CLKSEL_SHIFT         30U
#define TCSR_SUPERTMROPT0_SUPERTMR3_CLKSEL_WIDTH         2U
#define TCSR_SUPERTMROPT0_SUPERTMR3_CLKSEL(x)            (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR3_CLKSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR3_CLKSEL_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR2_CLKSEL_MASK          0x30000000U
#define TCSR_SUPERTMROPT0_SUPERTMR2_CLKSEL_SHIFT         28U
#define TCSR_SUPERTMROPT0_SUPERTMR2_CLKSEL_WIDTH         2U
#define TCSR_SUPERTMROPT0_SUPERTMR2_CLKSEL(x)            (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR2_CLKSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR2_CLKSEL_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR1_CLKSEL_MASK          0x0C000000U
#define TCSR_SUPERTMROPT0_SUPERTMR1_CLKSEL_SHIFT         26U
#define TCSR_SUPERTMROPT0_SUPERTMR1_CLKSEL_WIDTH         2U
#define TCSR_SUPERTMROPT0_SUPERTMR1_CLKSEL(x)            (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR1_CLKSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR1_CLKSEL_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR0_CLKSEL_MASK          0x03000000U
#define TCSR_SUPERTMROPT0_SUPERTMR0_CLKSEL_SHIFT         24U
#define TCSR_SUPERTMROPT0_SUPERTMR0_CLKSEL_WIDTH         2U
#define TCSR_SUPERTMROPT0_SUPERTMR0_CLKSEL(x)            (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR0_CLKSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR0_CLKSEL_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR5_CLKSEL_MASK          0x000C0000U
#define TCSR_SUPERTMROPT0_SUPERTMR5_CLKSEL_SHIFT         18U
#define TCSR_SUPERTMROPT0_SUPERTMR5_CLKSEL_WIDTH         2U
#define TCSR_SUPERTMROPT0_SUPERTMR5_CLKSEL(x)            (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR5_CLKSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR5_CLKSEL_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR4_CLKSEL_MASK          0x00030000U
#define TCSR_SUPERTMROPT0_SUPERTMR4_CLKSEL_SHIFT         16U
#define TCSR_SUPERTMROPT0_SUPERTMR4_CLKSEL_WIDTH         2U
#define TCSR_SUPERTMROPT0_SUPERTMR4_CLKSEL(x)            (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR4_CLKSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR4_CLKSEL_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR3_FLTXSEL_MASK         0x00007000U
#define TCSR_SUPERTMROPT0_SUPERTMR3_FLTXSEL_SHIFT        12U
#define TCSR_SUPERTMROPT0_SUPERTMR3_FLTXSEL_WIDTH        3U
#define TCSR_SUPERTMROPT0_SUPERTMR3_FLTXSEL(x)           (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR3_FLTXSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR3_FLTXSEL_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR2_FLTXSEL_MASK         0x00000700U
#define TCSR_SUPERTMROPT0_SUPERTMR2_FLTXSEL_SHIFT        8U
#define TCSR_SUPERTMROPT0_SUPERTMR2_FLTXSEL_WIDTH        3U
#define TCSR_SUPERTMROPT0_SUPERTMR2_FLTXSEL(x)           (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR2_FLTXSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR2_FLTXSEL_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR1_FLTXSEL_MASK         0x00000070U
#define TCSR_SUPERTMROPT0_SUPERTMR1_FLTXSEL_SHIFT        4U
#define TCSR_SUPERTMROPT0_SUPERTMR1_FLTXSEL_WIDTH        3U
#define TCSR_SUPERTMROPT0_SUPERTMR1_FLTXSEL(x)           (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR1_FLTXSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR1_FLTXSEL_MASK)
#define TCSR_SUPERTMROPT0_SUPERTMR0_FLTXSEL_MASK         0x000000007U
#define TCSR_SUPERTMROPT0_SUPERTMR0_FLTXSEL_SHIFT        0U
#define TCSR_SUPERTMROPT0_SUPERTMR0_FLTXSEL_WIDTH        3U
#define TCSR_SUPERTMROPT0_SUPERTMR0_FLTXSEL(x)           (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT0_SUPERTMR0_FLTXSEL_SHIFT))&TCSR_SUPERTMROPT0_SUPERTMR0_FLTXSEL_MASK)


/* SUPERTMROPT1 Bit Fields */
#define TCSR_SUPERTMROPT1_SUPERTMR0SYNCBIT_MASK               0x1U
#define TCSR_SUPERTMROPT1_SUPERTMR0SYNCBIT_SHIFT              0U
#define TCSR_SUPERTMROPT1_SUPERTMR0SYNCBIT_WIDTH              1U
#define TCSR_SUPERTMROPT1_SUPERTMR0SYNCBIT(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR0SYNCBIT_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR0SYNCBIT_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR1SYNCBIT_MASK               0x2U
#define TCSR_SUPERTMROPT1_SUPERTMR1SYNCBIT_SHIFT              1U
#define TCSR_SUPERTMROPT1_SUPERTMR1SYNCBIT_WIDTH              1U
#define TCSR_SUPERTMROPT1_SUPERTMR1SYNCBIT(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR1SYNCBIT_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR1SYNCBIT_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR2SYNCBIT_MASK               0x4U
#define TCSR_SUPERTMROPT1_SUPERTMR2SYNCBIT_SHIFT              2U
#define TCSR_SUPERTMROPT1_SUPERTMR2SYNCBIT_WIDTH              1U
#define TCSR_SUPERTMROPT1_SUPERTMR2SYNCBIT(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR2SYNCBIT_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR2SYNCBIT_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR3SYNCBIT_MASK               0x8U
#define TCSR_SUPERTMROPT1_SUPERTMR3SYNCBIT_SHIFT              3U
#define TCSR_SUPERTMROPT1_SUPERTMR3SYNCBIT_WIDTH              1U
#define TCSR_SUPERTMROPT1_SUPERTMR3SYNCBIT(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR3SYNCBIT_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR3SYNCBIT_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR1CH0SEL_MASK                0x30U
#define TCSR_SUPERTMROPT1_SUPERTMR1CH0SEL_SHIFT               4U
#define TCSR_SUPERTMROPT1_SUPERTMR1CH0SEL_WIDTH               2U
#define TCSR_SUPERTMROPT1_SUPERTMR1CH0SEL(x)                  (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR1CH0SEL_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR1CH0SEL_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR2CH0SEL_MASK                0xC0U
#define TCSR_SUPERTMROPT1_SUPERTMR2CH0SEL_SHIFT               6U
#define TCSR_SUPERTMROPT1_SUPERTMR2CH0SEL_WIDTH               2U
#define TCSR_SUPERTMROPT1_SUPERTMR2CH0SEL(x)                  (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR2CH0SEL_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR2CH0SEL_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR2CH1SEL_MASK                0x100U
#define TCSR_SUPERTMROPT1_SUPERTMR2CH1SEL_SHIFT               8U
#define TCSR_SUPERTMROPT1_SUPERTMR2CH1SEL_WIDTH               1U
#define TCSR_SUPERTMROPT1_SUPERTMR2CH1SEL(x)                  (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR2CH1SEL_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR2CH1SEL_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR4SYNCBIT_MASK               0x800U
#define TCSR_SUPERTMROPT1_SUPERTMR4SYNCBIT_SHIFT              11U
#define TCSR_SUPERTMROPT1_SUPERTMR4SYNCBIT_WIDTH              1U
#define TCSR_SUPERTMROPT1_SUPERTMR4SYNCBIT(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR4SYNCBIT_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR4SYNCBIT_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR5SYNCBIT_MASK               0x1000U
#define TCSR_SUPERTMROPT1_SUPERTMR5SYNCBIT_SHIFT              12U
#define TCSR_SUPERTMROPT1_SUPERTMR5SYNCBIT_WIDTH              1U
#define TCSR_SUPERTMROPT1_SUPERTMR5SYNCBIT(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR5SYNCBIT_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR5SYNCBIT_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMRGLDOK_MASK                  0x8000U
#define TCSR_SUPERTMROPT1_SUPERTMRGLDOK_SHIFT                 15U
#define TCSR_SUPERTMROPT1_SUPERTMRGLDOK_WIDTH                 1U
#define TCSR_SUPERTMROPT1_SUPERTMRGLDOK(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMRGLDOK_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMRGLDOK_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR0_OUTSEL_MASK               0xFF0000U
#define TCSR_SUPERTMROPT1_SUPERTMR0_OUTSEL_SHIFT              16U
#define TCSR_SUPERTMROPT1_SUPERTMR0_OUTSEL_WIDTH              8U
#define TCSR_SUPERTMROPT1_SUPERTMR0_OUTSEL(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR0_OUTSEL_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR0_OUTSEL_MASK)
#define TCSR_SUPERTMROPT1_SUPERTMR3_OUTSEL_MASK               0xFF000000U
#define TCSR_SUPERTMROPT1_SUPERTMR3_OUTSEL_SHIFT              24U
#define TCSR_SUPERTMROPT1_SUPERTMR3_OUTSEL_WIDTH              8U
#define TCSR_SUPERTMROPT1_SUPERTMR3_OUTSEL(x)                 (((uint32_t)(((uint32_t)(x))<<TCSR_SUPERTMROPT1_SUPERTMR3_OUTSEL_SHIFT))&TCSR_SUPERTMROPT1_SUPERTMR3_OUTSEL_MASK)


/* CAN0RAM Bit Fields */
#define TCSR_CAN0RAM_RAMRA_MASK                     0x3U
#define TCSR_CAN0RAM_RAMRA_SHIFT                    0U
#define TCSR_CAN0RAM_RAMRA_WIDTH                    2U
#define TCSR_CAN0RAM_RAMRA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN0RAM_RAMRA_SHIFT))&TCSR_CAN0RAM_RAMRA_MASK)
#define TCSR_CAN0RAM_RAMWA_MASK                     0x1CU
#define TCSR_CAN0RAM_RAMWA_SHIFT                    2U
#define TCSR_CAN0RAM_RAMWA_WIDTH                    3U
#define TCSR_CAN0RAM_RAMWA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN0RAM_RAMWA_SHIFT))&TCSR_CAN0RAM_RAMWA_MASK)
#define TCSR_CAN0RAM_RAMWPULSE_MASK                 0xE0U
#define TCSR_CAN0RAM_RAMWPULSE_SHIFT                5U
#define TCSR_CAN0RAM_RAMWPULSE_WIDTH                3U
#define TCSR_CAN0RAM_RAMWPULSE(x)                   (((uint32_t)(((uint32_t)(x))<<TCSR_CAN0RAM_RAMWPULSE_SHIFT))&TCSR_CAN0RAM_RAMWPULSE_MASK)
#define TCSR_CAN0RAM_RAMLS_MASK                     0x100U
#define TCSR_CAN0RAM_RAMLS_SHIFT                    8U
#define TCSR_CAN0RAM_RAMLS_WIDTH                    1U
#define TCSR_CAN0RAM_RAMLS(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN0RAM_RAMLS_SHIFT))&TCSR_CAN0RAM_RAMLS_MASK)
#define TCSR_CAN0RAM_RAMRM_MASK                     0x1E00U
#define TCSR_CAN0RAM_RAMRM_SHIFT                    9U
#define TCSR_CAN0RAM_RAMRM_WIDTH                    4U
#define TCSR_CAN0RAM_RAMRM(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN0RAM_RAMRM_SHIFT))&TCSR_CAN0RAM_RAMRM_MASK)
#define TCSR_CAN0RAM_RAMRME_MASK                    0x2000U
#define TCSR_CAN0RAM_RAMRME_SHIFT                   13U
#define TCSR_CAN0RAM_RAMRME_WIDTH                   1U
#define TCSR_CAN0RAM_RAMRME(x)                      (((uint32_t)(((uint32_t)(x))<<TCSR_CAN0RAM_RAMRME_SHIFT))&TCSR_CAN0RAM_RAMRME_MASK)

/* CAN1RAM Bit Fields */
#define TCSR_CAN1RAM_RAMRA_MASK                     0x3U
#define TCSR_CAN1RAM_RAMRA_SHIFT                    0U
#define TCSR_CAN1RAM_RAMRA_WIDTH                    2U
#define TCSR_CAN1RAM_RAMRA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN1RAM_RAMRA_SHIFT))&TCSR_CAN1RAM_RAMRA_MASK)
#define TCSR_CAN1RAM_RAMWA_MASK                     0x1CU
#define TCSR_CAN1RAM_RAMWA_SHIFT                    2U
#define TCSR_CAN1RAM_RAMWA_WIDTH                    3U
#define TCSR_CAN1RAM_RAMWA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN1RAM_RAMWA_SHIFT))&TCSR_CAN1RAM_RAMWA_MASK)
#define TCSR_CAN1RAM_RAMWPULSE_MASK                 0xE0U
#define TCSR_CAN1RAM_RAMWPULSE_SHIFT                5U
#define TCSR_CAN1RAM_RAMWPULSE_WIDTH                3U
#define TCSR_CAN1RAM_RAMWPULSE(x)                   (((uint32_t)(((uint32_t)(x))<<TCSR_CAN1RAM_RAMWPULSE_SHIFT))&TCSR_CAN1RAM_RAMWPULSE_MASK)
#define TCSR_CAN1RAM_RAMLS_MASK                     0x100U
#define TCSR_CAN1RAM_RAMLS_SHIFT                    8U
#define TCSR_CAN1RAM_RAMLS_WIDTH                    1U
#define TCSR_CAN1RAM_RAMLS(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN1RAM_RAMLS_SHIFT))&TCSR_CAN1RAM_RAMLS_MASK)
#define TCSR_CAN1RAM_RAMRM_MASK                     0x1E00U
#define TCSR_CAN1RAM_RAMRM_SHIFT                    9U
#define TCSR_CAN1RAM_RAMRM_WIDTH                    4U
#define TCSR_CAN1RAM_RAMRM(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN1RAM_RAMRM_SHIFT))&TCSR_CAN1RAM_RAMRM_MASK)
#define TCSR_CAN1RAM_RAMRME_MASK                    0x2000U
#define TCSR_CAN1RAM_RAMRME_SHIFT                   13U
#define TCSR_CAN1RAM_RAMRME_WIDTH                   1U
#define TCSR_CAN1RAM_RAMRME(x)                      (((uint32_t)(((uint32_t)(x))<<TCSR_CAN1RAM_RAMRME_SHIFT))&TCSR_CAN1RAM_RAMRME_MASK)

/* CAN2RAM Bit Fields */
#define TCSR_CAN2RAM_RAMRA_MASK                     0x3U
#define TCSR_CAN2RAM_RAMRA_SHIFT                    0U
#define TCSR_CAN2RAM_RAMRA_WIDTH                    2U
#define TCSR_CAN2RAM_RAMRA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN2RAM_RAMRA_SHIFT))&TCSR_CAN2RAM_RAMRA_MASK)
#define TCSR_CAN2RAM_RAMWA_MASK                     0x1CU
#define TCSR_CAN2RAM_RAMWA_SHIFT                    2U
#define TCSR_CAN2RAM_RAMWA_WIDTH                    3U
#define TCSR_CAN2RAM_RAMWA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN2RAM_RAMWA_SHIFT))&TCSR_CAN2RAM_RAMWA_MASK)
#define TCSR_CAN2RAM_RAMWPULSE_MASK                 0xE0U
#define TCSR_CAN2RAM_RAMWPULSE_SHIFT                5U
#define TCSR_CAN2RAM_RAMWPULSE_WIDTH                3U
#define TCSR_CAN2RAM_RAMWPULSE(x)                   (((uint32_t)(((uint32_t)(x))<<TCSR_CAN2RAM_RAMWPULSE_SHIFT))&TCSR_CAN2RAM_RAMWPULSE_MASK)
#define TCSR_CAN2RAM_RAMLS_MASK                     0x100U
#define TCSR_CAN2RAM_RAMLS_SHIFT                    8U
#define TCSR_CAN2RAM_RAMLS_WIDTH                    1U
#define TCSR_CAN2RAM_RAMLS(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN2RAM_RAMLS_SHIFT))&TCSR_CAN2RAM_RAMLS_MASK)
#define TCSR_CAN2RAM_RAMRM_MASK                     0x1E00U
#define TCSR_CAN2RAM_RAMRM_SHIFT                    9U
#define TCSR_CAN2RAM_RAMRM_WIDTH                    4U
#define TCSR_CAN2RAM_RAMRM(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN2RAM_RAMRM_SHIFT))&TCSR_CAN2RAM_RAMRM_MASK)
#define TCSR_CAN2RAM_RAMRME_MASK                    0x2000U
#define TCSR_CAN2RAM_RAMRME_SHIFT                   13U
#define TCSR_CAN2RAM_RAMRME_WIDTH                   1U
#define TCSR_CAN2RAM_RAMRME(x)                      (((uint32_t)(((uint32_t)(x))<<TCSR_CAN2RAM_RAMRME_SHIFT))&TCSR_CAN2RAM_RAMRME_MASK)

/* CAN3RAM Bit Fields */
#define TCSR_CAN3RAM_RAMRA_MASK                     0x3U
#define TCSR_CAN3RAM_RAMRA_SHIFT                    0U
#define TCSR_CAN3RAM_RAMRA_WIDTH                    2U
#define TCSR_CAN3RAM_RAMRA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN3RAM_RAMRA_SHIFT))&TCSR_CAN3RAM_RAMRA_MASK)
#define TCSR_CAN3RAM_RAMWA_MASK                     0x1CU
#define TCSR_CAN3RAM_RAMWA_SHIFT                    2U
#define TCSR_CAN3RAM_RAMWA_WIDTH                    3U
#define TCSR_CAN3RAM_RAMWA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN3RAM_RAMWA_SHIFT))&TCSR_CAN3RAM_RAMWA_MASK)
#define TCSR_CAN3RAM_RAMWPULSE_MASK                 0xE0U
#define TCSR_CAN3RAM_RAMWPULSE_SHIFT                5U
#define TCSR_CAN3RAM_RAMWPULSE_WIDTH                3U
#define TCSR_CAN3RAM_RAMWPULSE(x)                   (((uint32_t)(((uint32_t)(x))<<TCSR_CAN3RAM_RAMWPULSE_SHIFT))&TCSR_CAN3RAM_RAMWPULSE_MASK)
#define TCSR_CAN3RAM_RAMLS_MASK                     0x100U
#define TCSR_CAN3RAM_RAMLS_SHIFT                    8U
#define TCSR_CAN3RAM_RAMLS_WIDTH                    1U
#define TCSR_CAN3RAM_RAMLS(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN3RAM_RAMLS_SHIFT))&TCSR_CAN3RAM_RAMLS_MASK)
#define TCSR_CAN3RAM_RAMRM_MASK                     0x1E00U
#define TCSR_CAN3RAM_RAMRM_SHIFT                    9U
#define TCSR_CAN3RAM_RAMRM_WIDTH                    4U
#define TCSR_CAN3RAM_RAMRM(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN3RAM_RAMRM_SHIFT))&TCSR_CAN3RAM_RAMRM_MASK)
#define TCSR_CAN3RAM_RAMRME_MASK                    0x2000U
#define TCSR_CAN3RAM_RAMRME_SHIFT                   13U
#define TCSR_CAN3RAM_RAMRME_WIDTH                   1U
#define TCSR_CAN3RAM_RAMRME(x)                      (((uint32_t)(((uint32_t)(x))<<TCSR_CAN3RAM_RAMRME_SHIFT))&TCSR_CAN3RAM_RAMRME_MASK)

/* CAN4RAM Bit Fields */
#define TCSR_CAN4RAM_RAMRA_MASK                     0x3U
#define TCSR_CAN4RAM_RAMRA_SHIFT                    0U
#define TCSR_CAN4RAM_RAMRA_WIDTH                    2U
#define TCSR_CAN4RAM_RAMRA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN4RAM_RAMRA_SHIFT))&TCSR_CAN4RAM_RAMRA_MASK)
#define TCSR_CAN4RAM_RAMWA_MASK                     0x1CU
#define TCSR_CAN4RAM_RAMWA_SHIFT                    2U
#define TCSR_CAN4RAM_RAMWA_WIDTH                    3U
#define TCSR_CAN4RAM_RAMWA(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN4RAM_RAMWA_SHIFT))&TCSR_CAN4RAM_RAMWA_MASK)
#define TCSR_CAN4RAM_RAMWPULSE_MASK                 0xE0U
#define TCSR_CAN4RAM_RAMWPULSE_SHIFT                5U
#define TCSR_CAN4RAM_RAMWPULSE_WIDTH                3U
#define TCSR_CAN4RAM_RAMWPULSE(x)                   (((uint32_t)(((uint32_t)(x))<<TCSR_CAN4RAM_RAMWPULSE_SHIFT))&TCSR_CAN4RAM_RAMWPULSE_MASK)
#define TCSR_CAN4RAM_RAMLS_MASK                     0x100U
#define TCSR_CAN4RAM_RAMLS_SHIFT                    8U
#define TCSR_CAN4RAM_RAMLS_WIDTH                    1U
#define TCSR_CAN4RAM_RAMLS(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN4RAM_RAMLS_SHIFT))&TCSR_CAN4RAM_RAMLS_MASK)
#define TCSR_CAN4RAM_RAMRM_MASK                     0x1E00U
#define TCSR_CAN4RAM_RAMRM_SHIFT                    9U
#define TCSR_CAN4RAM_RAMRM_WIDTH                    4U
#define TCSR_CAN4RAM_RAMRM(x)                       (((uint32_t)(((uint32_t)(x))<<TCSR_CAN4RAM_RAMRM_SHIFT))&TCSR_CAN4RAM_RAMRM_MASK)
#define TCSR_CAN4RAM_RAMRME_MASK                    0x2000U
#define TCSR_CAN4RAM_RAMRME_SHIFT                   13U
#define TCSR_CAN4RAM_RAMRME_WIDTH                   1U
#define TCSR_CAN4RAM_RAMRME(x)                      (((uint32_t)(((uint32_t)(x))<<TCSR_CAN4RAM_RAMRME_SHIFT))&TCSR_CAN4RAM_RAMRME_MASK)


/* CJTAGCTRL Bit Fields */
#define TCSR_CJTAG_AUXPINFUNC_MASK                  0x1U
#define TCSR_CJTAG_AUXPINFUNC_SHIFT                 0U
#define TCSR_CJTAG_AUXPINFUNC_WIDTH                 1U
#define TCSR_CJTAG_AUXPINFUNC(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_CJTAG_AUXPINFUNC_SHIFT))&TCSR_CJTAG_AUXPINFUNC_MASK)
#define TCSR_CJTAG_STDPINFUNC_MASK                  0x2U
#define TCSR_CJTAG_STDPINFUNC_SHIFT                 1U
#define TCSR_CJTAG_STDPINFUNC_WIDTH                 1U
#define TCSR_CJTAG_STDPINFUNC(x)                    (((uint32_t)(((uint32_t)(x))<<TCSR_CJTAG_STDPINFUNC_SHIFT))&TCSR_CJTAG_STDPINFUNC_MASK)
#define TCSR_CJTAG_PINWIDE_LOCK_MASK                0x4U
#define TCSR_CJTAG_PINWIDE_LOCK_SHIFT               2U
#define TCSR_CJTAG_PINWIDE_LOCK_WIDTH               1U
#define TCSR_CJTAG_PINWIDE_LOCK(x)                  (((uint32_t)(((uint32_t)(x))<<TCSR_CJTAG_PINWIDE_LOCK_SHIFT))&TCSR_CJTAG_PINWIDE_LOCK_MASK)
#define TCSR_CJTAG_PINWIDE_SEL_MASK                 0x8U
#define TCSR_CJTAG_PINWIDE_SEL_SHIFT                3U
#define TCSR_CJTAG_PINWIDE_SEL_WIDTH                1U
#define TCSR_CJTAG_PINWIDE_SEL(x)                   (((uint32_t)(((uint32_t)(x))<<TCSR_CJTAG_PINWIDE_SEL_SHIFT))&TCSR_CJTAG_PINWIDE_SEL_MASK)

/* MISCTRL1 Bit Fields */
#define TCSR_MISCTRL1_SW_TRG_MASK                   0x1U
#define TCSR_MISCTRL1_SW_TRG_SHIFT                  0U
#define TCSR_MISCTRL1_SW_TRG_WIDTH                  1U
#define TCSR_MISCTRL1_SW_TRG(x)                     (((uint32_t)(((uint32_t)(x))<<TCSR_MISCTRL1_SW_TRG_SHIFT))&TCSR_MISCTRL1_SW_TRG_MASK)

/**
 * @}
 */ /* end of group TCSR_Register_Masks */

/**
 * @}
 */ /* end of group TCSR_Peripheral_Access_Layer */
// clang-format on

#endif /* __EAM2011_TCSR_H__ */
