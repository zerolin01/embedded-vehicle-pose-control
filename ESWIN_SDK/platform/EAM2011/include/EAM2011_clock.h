#ifndef __EAM2011_CLOCK_H__
#define __EAM2011_CLOCK_H__

#ifndef __ASSEMBLY__
/** @brief Clock names. */
typedef enum {
    /* Main clocks */
    CPU_CORE_CLK                = 0U,   /** RISC-V Core clock               */
    CPU_RTC_CLK                 = 1U,   /** RISC-V Real timer clock         */
    /* Other internal clocks used by peripherals. */
    SOSC_CLK                    = 2U,   /** OSC clock                       */
    SROSC_CLK                   = 3U,   /** SROSC clock                     */
    FROSC_CLK                   = 4U,   /** FROSC_CLK clock                 */
    LPOSC_CLK                   = 5U,   /** LP OSC 128Khz clock             */ 
    RTCOSC_CLK                  = 6U,   /** RTCCLK clock                    */
    PMU_AON_CLK                 = LPOSC_CLK,
    LPOSC32K_CLK                = 7U,  /** LP OSC 32Khz clock              */
    AON_CLK                     = 8U,   /** Always On 32Khz clock           */
    TCLK0_CLK                   = 9U,
    TCLK1_CLK                   = 10U,
    TCLK2_CLK                   = 11U,
    SPLL_SRC_CLK                = 12U,
    SPLL_CLK                    = 13U,
    WORK_CLK                    = 14U,
    MAIN_BUS_CLK                = CPU_CORE_CLK,
    MAIN_BUS_AUX_CLK            = 15U,
    MAIN_BUS_MOTOR_CTRL_CLK     = 16U,
    MAIN_BUS_PLL_FUNC_CLK       = 17U,
    PERIPH_BUS_CLK              = 18U,
    PERIPH_BUS_PLL_FUNC_CLK     = 19U,
    PERIPH_BUS_PLL_AUX_FUNC_CLK = 20U,
    SUPERTMR_FUNC_CLK           = 21U,
    SUPERIO_CLK                 = 22U,
    FAST_AUX_CLK                = 23U,
    ADC_FUNC_CLK                = 24U,
    GTMR_FUNC_CLK               = 25U,
    TSENSOR_LV_CLK              = 26U,
    PCTMR_FUNC_CLK              = 27U,
    DEB_CLK                     = 28U,
    WDOG_CLK                    = 29U,
    SUPERTMR0_SELCLK            = 30U,
    SUPERTMR1_SELCLK            = 31U,
    SUPERTMR2_SELCLK            = 32U,
    SUPERTMR3_SELCLK            = 33U,
    SUPERTMR4_SELCLK            = 34U,
    SUPERTMR5_SELCLK            = 35U,

    GATE_CLK_START              = 36U,
    DEBUG_CLK                   = GATE_CLK_START,
    TPIU_CLK                    = 37U,
    TIPU_TRACE_CLK              = 38U,
    FMC_CLK                     = 39U,
    ADC0_CLK                    = 40U,
    ADC1_CLK                    = 41U,
    PDU0_CLK                    = 42U,
    PDU1_CLK                    = 43U,
    CMP_CLK                     = 44U,
    DAC_CLK                     = 45U,
    PWM0_CLK                    = 46U,
    PWM1_CLK                    = 47U,
    PWM2_CLK                    = 48U,
    PWM3_CLK                    = 49U,
    PWM4_CLK                    = 50U,
    SUPERTMR0_CLK               = 51U,
    SUPERTMR1_CLK               = 52U,
    SUPERTMR2_CLK               = 53U,
    SUPERTMR3_CLK               = 54U,
    SUPERTMR4_CLK               = 55U,
    SUPERTMR5_CLK               = 56U,
    PITMR0_CLK                  = 57U,
    PCTMR0_CLK                  = 58U,
    PCTMR1_CLK                  = 59U,
    PCTMR2_CLK                  = 60U,
    PCTMR3_CLK                  = 61U,
    PCTMR4_CLK                  = 62U,
    PCTMR5_CLK                  = 63U,
    PCTMR6_CLK                  = 64U,
    PCTMR7_CLK                  = 65U,
    PCTMR8_CLK                  = 66U,
    EWM_CLK                     = 67U,
    CRC_CLK                     = 68U,
    TSENSOR_LV_GATE_CLK         = 69U,
    INTC_CLK                    = 70U,
    TRACE_PCLK                  = 71U,
    IOPMP_CLK                   = 72U,
    FRM_CLK                     = 73U,
    UART0_CLK                   = 74U,
    UART1_CLK                   = 75U,
    UART2_CLK                   = 76U,
    UART3_CLK                   = 77U,
    UART4_CLK                   = 78U,
    UART5_CLK                   = 79U,
    I2C0_CLK                    = 80U,
    I2C1_CLK                    = 81U,
    SUPERIO0_CLK                = 82U,
    SPI0_CLK                    = 83U,
    SPI1_CLK                    = 84U,
    SPI2_CLK                    = 85U,
    SPI3_CLK                    = 86U,
    CAN0_CLK                    = 87U,
    CAN1_CLK                    = 88U,
    CAN2_CLK                    = 89U,
    CAN3_CLK                    = 90U,
    GTMR_CLK                    = 91U,
    PITMR1_CLK                  = 92U,
    PDMA_CLK                    = 93U,
    TRGMUX_CLK                  = 94U,
    CLOCK_NAME_COUNT            = 95U, /** The total number of entries    */
} clock_names_t;
#endif

#endif /* __EAM2011_CLOCK__ */
