#ifndef __EAM2011_MEMORY_MAP_H__
#define __EAM2011_MEMORY_MAP_H__

/* ----------------------------------------------------------------------------
   -- Memory Map Layer for EAM2011
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup Memory_Map_Layer_EAM2011 Memory Map Layer for EAM2011
 * @{
 */

#define CPU_DEBUG_BASEADDR      0x00000000U /* 00000 | RW              |  */
#define CPU_DEBUG_SIZE          0x00001000U
#define CPU_CSR_BASEADDR        0x00002000U /* 00001 | RW              |  */
#define CPU_CSR_SIZE            0x00001000U
#define CPU_CLIC_BASEADDR       0x02000000U /* 00002 | RW              |  */
#define CPU_CLIC_SIZE           0x01000000U
#define PFLASH_ICACHE_BASEADDR  0x10000000 /* 00003 | RO              |  */
#define PFLASH_ICACHE_SIZE      0x00100000
#define DFLASH_DCACHE_BASEADDR  0x12000000 /* 00004 | RO              |  */
#define DFLASH_DCACHE_SIZE      0x00020000
#define PFLASH_DCACHE_BASEADDR  0x12100000 /* 00004 | RO              |  */
#define PFLASH_DCACHE_SIZE      0x00100000
#define ENVRAM_DCACHE_BASEADDR  0x12020000U /* 00005 | RW              |  */
#define ENVRAM_DCACHE_SIZE      0x00001000U
#define OCM_DCACHE_BASEADDR     0x12030000 /* 00006 | RW              |  */
#define OCM_DCACHE_SIZE         0x00010000
#define BOOT_ROM_BASEADDR       0x1c000000 /* 00007 | RO              |  */
#define BOOT_ROM_SIZE           0x00008000
#define TIM_1_BASEADDR          0x1c010000 /* 00008 | RW              |  */
#define TIM_1_SIZE              0x00010000
#define PFLASH_PERIPH_BASEADDR  0x40000000 /* 00009 | RO              |  */
#define PFLASH_PERIPH_SIZE      0x00100000
#define DFLASH_PERIPH_BASEADDR  0x42000000U /* 00010 | RO              |  */
#define DFLASH_PERIPH_SIZE      0x00020000U
#define ENVRAM_PERIPH_BASEADDR  0x42020000U /* 00011 | RW              |  */
#define ENVRAM_PERIPH_SIZE      0x00001000U
#define OCM_PERIPH_BASEADDR     0x42030000 /* 00012 | RW              |  */
#define OCM_PERIPH_SIZE         0x00010000
#define IOPMP_BASEADDR          0x4404F000U /* IOPMP 0 */
#define IOPMP_SIZE              0x00001000U
#define TPIU_BASEADDR           0x4404d000U /* TPIU */
#define TPIU_SIZE               0x00001000U
#define TRACE_CFG_BASEADDR      0x4404e000U /* TPIU */
#define TRACE_CFG_SIZE          0x00001000U
#define JTAG2AHB_BASEADDR       0x40004000U /* JTAG2AHB */
#define JTAG2AHB_SIZE           0x00001000U
#define JTAG_BASEADDR           0x40005000U /* JTAG */
#define JTAG_SIZE               0x00001000U
#define DMAMUX_BASEADDR         0x44052000U /* PDMA Channel Multiplexer */
#define DMAMUX_SIZE             0x00001000U
#define PDMA_BASEADDR           0x44050000U /* PDMA Controller */
#define DMA_SIZE                0x00001000U
#define DMAPITMR_BASEADDR       0x44054000U /* PDMA PITMR Controller */
#define DMAPITMR_SIZE           0x00001000U
#define DMA_DESCRIPTOR_BASEADDR 0x40062000U /* PDMA controller transfer control descriptors */
#define DMA_DESCRIPTOR_SIZE     0x00001000U
#define HSM_BASEADDR            0x4000b000U /* RT-120 */
#define HSM_SIZE                0x00001000U
#define WDOG_BASEADDR           0x4404A000U /* Software watchdog */
#define WDOG_SIZE               0x00001000U
#define TCSR_BASEADDR           0x44040000U /* TCSR */
#define TCSR_SIZE               0x00001000U
#define TSENSOR_BASEADDR        0x44048000U /* 00055 | Configuration   |  */
#define TSENSOR_SIZE            0x00001000U
#define CRC_BASEADDR            0x44049000U /* CRC */
#define CRC_SIZE                0x00001000U
#define OTP_C_BASEADDR          0x40013000U /* OTP_controller */
#define OTP_C_SIZE              0x00001000U
#define BIST_BASEADDR           0x40016000U /* Bist Controller */
#define BIST_SIZE               0x00001000U
#define ERM_BASEADDR            0x40018000U /* ERM */
#define ERM_SIZE                0x00001000U
#define EIM_BASEADDR            0x40019000U /* EIM */
#define EIM_SIZE                0x00001000U
#define FLEXCAN_0_BASEADDR      0x44080000U /* FlexCAN */
#define FLEXCAN_0_SIZE          0x00004000U
#define FLEXCAN_1_BASEADDR      0x44084000U /* FlexCAN */
#define FLEXCAN_1_SIZE          0x00004000U
#define FLEXCAN_2_BASEADDR      0x44090000U /* FlexCAN */
#define FLEXCAN_2_SIZE          0x00004000U
#define FLEXCAN_3_BASEADDR      0x440A0000U /* FlexCAN */
#define FLEXCAN_3_SIZE          0x00004000U
#define HSM_DATA_PORT_BASEADDR  0x40030000U /* RT-120 */
#define HSM_DATA_PORT_SIZE      0x00004000U
#define CRG_BASEADDR            0x44010000U /* CRG */
#define CRG_SIZE                0x00001000U
#define PMU_BASEADDR            0x44000000U /* Power Management Unit */
#define PMU_SIZE                0x00001000U
#define CMU_0_BASEADDR          0x44041000U /* Clock Monitor Unit 0 */
#define CMU_0_SIZE              0x00001000U
#define CMU_1_BASEADDR          0x44041010U /* Clock Monitor Unit 1 */
#define CMU_1_SIZE              0x00001000U
#define CMU_2_BASEADDR          0x44041020U /* Clock Monitor Unit 2 */
#define CMU_2_SIZE              0x00001000U
#define CMU_3_BASEADDR          0x44041030U /* Clock Monitor Unit 3 */
#define CMU_3_SIZE              0x00001000U
#define CMU_4_BASEADDR          0x44041040U /* Clock Monitor Unit 4 */
#define CMU_4_SIZE              0x00001000U
#define CMU_COMMON_BASEADDR     0x44041070U /* Clock Monitor Unit common */
#define CMU_COMMON_SIZE         0x00001000U
#define RTC_BASEADDR            0x44001000U /* Real-time counter */
#define RTC_SIZE                0x00001000U
#define ADC_0_BASEADDR          0x44030000U /* Analog-to-digital converter */
#define ADC_0_SIZE              0x00001000U
#define ADC_1_BASEADDR          0x44031000U /* Analog-to-digital converter */
#define ADC_1_SIZE              0x00001000U
#define TRGMUX_BASEADDR         0x44055000U /* Trigger Multiplexing Control */
#define TRGMUX_SIZE             0x00001000U
#define TMRMUX_BASEADDR         0x4402A000U /* Tmrmux Multiplexing Control */
#define TMRMUX_SIZE             0x00001000U
#define GTMR_BASEADDR           0x44032000U /* 00038 | Configuration   |  */
#define GTMR_SIZE               0x00001000U
#define PDU_0_BASEADDR          0x44034000U /* Programmable Delay Block */
#define PDU_0_SIZE              0x00001000U
#define PDU_1_BASEADDR          0x44035000U /* Programmable Delay Block */
#define PDU_1_SIZE              0x00001000U
#define SUPERTIMER_0_BASEADDR   0x44020000U /* SuperTimer */
#define SUPERTIMER_0_SIZE       0x00001000U
#define SUPERTIMER_1_BASEADDR   0x44021000U /* SuperTimer */
#define SUPERTIMER_1_SIZE       0x00001000U
#define SUPERTIMER_2_BASEADDR   0x44022000U /* SuperTimer */
#define SUPERTIMER_2_SIZE       0x00001000U
#define SUPERTIMER_3_BASEADDR   0x44023000U /* SuperTimer */
#define SUPERTIMER_3_SIZE       0x00001000U
#define SUPERTIMER_4_BASEADDR   0x44024000U /* SuperTimer */
#define SUPERTIMER_4_SIZE       0x00001000U
#define SUPERTIMER_5_BASEADDR   0x44025000U /* SuperTimer */
#define SUPERTIMER_5_SIZE       0x00001000U
#define LINFLEXD_0_BASEADDR     0x400b0000U /* LinFlexed */
#define LINFLEXD_0_SIZE         0x00001000U
#define LINFLEXD_1_BASEADDR     0x400b1000U /* LinFlexed */
#define LINFLEXD_1_SIZE         0x00001000U
#define LINFLEXD_2_BASEADDR     0x400b2000U /* LinFlexed */
#define LINFLEXD_2_SIZE         0x00001000U
#define LINFLEXD_3_BASEADDR     0x400b3000U /* LinFlexed */
#define LINFLEXD_3_SIZE         0x00001000U
#define UART_0_BASEADDR         0x44060000U /* 00069 | RW              |  */
#define UART_0_SIZE             0x00001000U
#define UART_1_BASEADDR         0x44061000U /* 00070 | RW              |  */
#define UART_1_SIZE             0x00001000U
#define UART_2_BASEADDR         0x44062000U /* 00071 | RW              |  */
#define UART_2_SIZE             0x00001000U
#define UART_3_BASEADDR         0x44063000U /* 00072 | RW              |  */
#define UART_3_SIZE             0x00001000U
#define UART_4_BASEADDR         0x44064000U /* 00073 | RW              |  */
#define UART_4_SIZE             0x00001000U
#define UART_5_BASEADDR         0x44065000U /* 00074 | RW              |  */
#define UART_5_SIZE             0x00001000U
#define SUPERIO_BASEADDR        0x4406e000U /* Flexible IO */
#define SUPERIO_SIZE            0x00001000U
#define I2C_0_BASEADDR          0x44068000U /* Low Power I2C */
#define I2C_0_SIZE              0x00001000U
#define I2C_1_BASEADDR          0x44069000U /* Low Power I2C */
#define I2C_1_SIZE              0x00001000U
#define PINMUX_BASEADDR         0x43000000u /* 00013 | Configuration   |  */
#define PINMUX_SIZE             0x00040000u
#define FMC_BASEADDR            0x44044000U /* Flash Controller */
#define FMC_SIZE                0x00001000U
#define EWM_BASEADDR            0x4404B000U /* External watchdog */
#define EWM_SIZE                0x00001000U
#define PCTMR0_BASEADDR         0x44028000U /* Low-power 0 timer */
#define PCTMR0_SIZE             0x00000200U
#define PCTMR1_BASEADDR         0x44028200U /* Low-power 1 timer */
#define PCTMR1_SIZE             0x00000200U
#define PCTMR2_BASEADDR         0x44028400U /* Low-power 2 timer */
#define PCTMR2_SIZE             0x00000200U
#define PCTMR3_BASEADDR         0x44028600U /* Low-power 3 timer */
#define PCTMR3_SIZE             0x00000200U
#define PCTMR4_BASEADDR         0x44028800U /* Low-power 4 timer */
#define PCTMR4_SIZE             0x00000200U
#define PCTMR5_BASEADDR         0x44028A00U /* Low-power 5 timer */
#define PCTMR5_SIZE             0x00000200U
#define PCTMR6_BASEADDR         0x44028C00U /* Low-power 6 timer */
#define PCTMR6_SIZE             0x00000200U
#define PCTMR7_BASEADDR         0x44028E00U /* Low-power 7 timer */
#define PCTMR7_SIZE             0x00000200U
#define PCTMR8_BASEADDR         0x44029000U /* Low-power 8 timer */
#define PCTMR8_SIZE             0x00001000U
#define PITMR_BASEADDR          0x4402C000U /* Low power periodic interrupt timer */
#define PITMR_SIZE              0x00001000U
#define FLASH_CTRL_BASEADDR     0x400d8000U /* Flash Controller */
#define FLASH_CTRL_SIZE         0x00001000U
#define ADMA_BASEADDR           0x4402e000U /* ADMA */
#define ADMA_SIZE               0x00002000U
#define CMP_0_BASEADDR          0x44036000U /* Analog comparator */
#define CMP_0_SIZE              0x00001000U
#define DAC_0_BASEADDR          0x44037000U /* Digital-to-analog converter*/
#define DAC_0_SIZE              0x00001000U
#define PWM_0_BASEADDR          0x4403a000U /* PWM0 */
#define PWM_0_SIZE              0x00001000U
#define PWM_1_BASEADDR          0x4403b000U /* PWM1 */
#define PWM_1_SIZE              0x00001000U
#define PWM_2_BASEADDR          0x4403c000U /* PWM2 */
#define PWM_2_SIZE              0x00001000U
#define PWM_3_BASEADDR          0x4403d000U /* PWM3 */
#define PWM_3_SIZE              0x00001000U
#define PWM_4_BASEADDR          0x4403e000U /* PWM4 */
#define PWM_4_SIZE              0x00001000U
#define SPI_0_BASEADDR          0x44070000U /* SPI0 */
#define SPI_0_SIZE              0x00001000U
#define SPI_1_BASEADDR          0x44071000U /* SPI1 */
#define SPI_1_SIZE              0x00001000U
#define SPI_2_BASEADDR          0x44072000U /* SPI2 */
#define SPI_2_SIZE              0x00001000U
#define SPI_3_BASEADDR          0x44073000U /* SPI3 */
#define SPI_3_SIZE              0x00001000U
#define WAKEUP_INTC_BASEADDR    0x44047000U
#define WAKEUP_INTC_SIZE        0x00001000U

/**
 * @}
 */
/* end of group Memory_Map_Layer_EAM2011 */

#endif /* __EAM2011_MEMORY_MAP_H__ */
