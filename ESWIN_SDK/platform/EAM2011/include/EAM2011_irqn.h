#ifndef __EAM2011_IRQN_H__
#define __EAM2011_IRQN_H__

/* ----------------------------------------------------------------------------
   -- Interrupt Vector Numbers for EAM2011
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup Interrupt_Vector_Numbers_EAM2011 Interrupt Vector Numbers for EAM2011
 * @{
 */

/** Interrupt Number Definitions */
#define NUMBER_OF_INT_VECTORS (512U) /* Number of interrupts in the Vector table */
// TODO: should update according to EAM2011
/**
 * @brief Defines the Interrupt Numbers definitions
 *
 * This enumeration is used to configure the interrupts.
 *
 * Implements : IRQn_Type_Class
 */
#ifndef __ASSEMBLY__
typedef enum IRQn {
    /* Auxiliary constants */
    NotAvail_IRQn = -128, /* Not available device specific interrupt */

    /* Core interrupts */
    Reserved0_IRQn    = 0,  /*  Internal reserved */
    Reserved1_IRQn    = 1,  /*  Internal reserved */
    Reserved2_IRQn    = 2,  /*  Internal reserved */
    Software_IRQn     = 3,  /*  Software interrupt */
    Reserved4_IRQn    = 4,  /*  Internal reserved */
    Reserved5_IRQn    = 5,  /*  Internal reserved */
    Reserved6_IRQn    = 6,  /*  Internal reserved */
    Timer_IRQn        = 7,  /*  Timer Interrupt */
    Reserved8_IRQn    = 8,  /*  Internal reserved */
    Reserved9_IRQn    = 9,  /*  Internal reserved */
    Reserved10_IRQn   = 10, /*  Internal reserved */
    External_IRQn     = 11, /*  External Interrupt */
    ClicSoftware_IRQn = 12, /*  Clic Software Interrupt */
    Reserved13_IRQn   = 13, /*  Internal reserved */
    Reserved14_IRQn   = 14, /*  Internal reserved */
    Reserved15_IRQn   = 15, /*  Internal reserved */

    /* Device specific interrupts */
    WDOG_IRQn                      = 16U,  /* WDOG Interrupt  */
    RTC_IRQn                       = 17U,  /* RTC alarm interrupt */
    POWERUP_IRQn                   = 18U,  /* POWERUP interrupt */
    PMU_VMON_IRQn                  = 19U,  /* PMU VMON interrupt */
    PMU_BOR_IRQn                   = 20U,  /* PMU BOR interrupt */
    BOOT_TIMEOUT_IRQn              = 21U,  /* BOOT TIMEOUT interrupt */
    HWCRC_ERR_IRQn                 = 22U,  /* Hardware CRC Error interrupt */
    CMU0_IRQn                      = 23U,  /* CMU0 interrupt */
    CMU1_IRQn                      = 24U,  /* CMU1 interrupt */
    CMU2_IRQn                      = 25U,  /* CMU2 interrupt */
    CMU3_IRQn                      = 26U,  /* CMU3 interrupt */
    CMU4_IRQn                      = 27U,  /* CMU4 interrupt */
    CMU5_IRQn                      = 28U,  /* CMU5 interrupt */
    CRG_ECC_Error_IRQn             = 29U,  /* CRG ECC Error interrupt */
    WAKEUP_IRQn                    = 30U,  /* WAKEUP interrupt */
    IOPMP_IRQn                     = 31U,  /* IOPMP TIMEOUT interrupt */
    LBIST_TIMEOUT_IRQn             = 32U,  /* LBIST TIMEOUT interrupt */
    FMC_OPT_Error_IRQn             = 33U,  /* FMC OPT Error interrupt */
    FMC_NORMAL_IRQn                = 34U,  /* FMC Normal interrupt */
    FMC_WARNING_IRQn               = 35U,  /* FMC Warning interrupt */
    FMC_NMI_IRQn                   = 36U,  /* FMC NMI interrupt */
    PERIPHBUS_PortParity_IRQn      = 37U,  /* Periph Port Parity interrupt */
    DMA_Error_15_0_IRQn            = 38U,  /* PDMA error interrupt channels 0-15 */
    DMA_Error_31_16_IRQn           = 39U,  /* PDMA error interrupt channels 16-31 */
    PDMA_Error_IRQn                = 40U,  /* PDMA error interrupt channels 0-31 */
    PDMA0_IRQn                     = 41U,  /* PDMA channel 0 transfer complete */
    PDMA1_IRQn                     = 42U,  /* PDMA channel 1 transfer complete */
    PDMA2_IRQn                     = 43U,  /* PDMA channel 2 transfer complete */
    PDMA3_IRQn                     = 44U,  /* PDMA channel 3 transfer complete */
    PDMA4_IRQn                     = 45U,  /* PDMA channel 4 transfer complete */
    PDMA5_IRQn                     = 46U,  /* PDMA channel 5 transfer complete */
    PDMA6_IRQn                     = 47U,  /* PDMA channel 6 transfer complete */
    PDMA7_IRQn                     = 48U,  /* PDMA channel 7 transfer complete */
    PDMA8_IRQn                     = 49U,  /* PDMA channel 8 transfer complete */
    PDMA9_IRQn                     = 50U,  /* PDMA channel 9 transfer complete */
    PDMA10_IRQn                    = 51U,  /* PDMA channel 10 transfer complete */
    PDMA11_IRQn                    = 52U,  /* PDMA channel 11 transfer complete */
    PDMA12_IRQn                    = 53U,  /* PDMA channel 12 transfer complete */
    PDMA13_IRQn                    = 54U,  /* PDMA channel 13 transfer complete */
    PDMA14_IRQn                    = 55U,  /* PDMA channel 14 transfer complete */
    PDMA15_IRQn                    = 56U,  /* PDMA channel 15 transfer complete */
    PDMA16_IRQn                    = 57U,  /* PDMA channel 16 transfer complete */
    PDMA17_IRQn                    = 58U,  /* PDMA channel 17 transfer complete */
    PDMA18_IRQn                    = 59U,  /* PDMA channel 18 transfer complete */
    PDMA19_IRQn                    = 60U,  /* PDMA channel 19 transfer complete */
    PDMA20_IRQn                    = 61U,  /* PDMA channel 20 transfer complete */
    PDMA21_IRQn                    = 62U,  /* PDMA channel 21 transfer complete */
    PDMA22_IRQn                    = 63U,  /* PDMA channel 22 transfer complete */
    PDMA23_IRQn                    = 64U,  /* PDMA channel 23 transfer complete */
    PDMA24_IRQn                    = 65U,  /* PDMA channel 24 transfer complete */
    PDMA25_IRQn                    = 66U,  /* PDMA channel 25 transfer complete */
    PDMA26_IRQn                    = 67U,  /* PDMA channel 26 transfer complete */
    PDMA27_IRQn                    = 68U,  /* PDMA channel 27 transfer complete */
    PDMA28_IRQn                    = 69U,  /* PDMA channel 28 transfer complete */
    PDMA29_IRQn                    = 70U,  /* PDMA channel 29 transfer complete */
    PDMA30_IRQn                    = 71U,  /* PDMA channel 30 transfer complete */
    PDMA31_IRQn                    = 72U,  /* PDMA channel 31 transfer complete */
    DMA_MasterAsync_IRQn           = 73U,  /* PDMA Request Master Async Interrupt */
    DMAPITMR_IRQn                  = 74U,  /* DMAPITMR interrupt */
    PITMR1_Ch0_IRQn                = 75U,  /* DMAPITMR CHNL0 interrupt */
    PITMR1_Ch1_IRQn                = 76U,  /* DMAPITMR CHNL1 interrupt */
    PITMR1_Ch2_IRQn                = 77U,  /* DMAPITMR CHNL2 interrupt */
    PITMR1_Ch3_IRQn                = 78U,  /* DMAPITMR CHNL3 interrupt */
    PITMR1_Ch4_IRQn                = 79U,  /* DMAPITMR CHNL4 interrupt */
    PITMR1_Ch5_IRQn                = 80U,  /* DMAPITMR CHNL5 interrupt */
    PITMR1_Ch6_IRQn                = 81U,  /* DMAPITMR CHNL6 interrupt */
    PITMR1_Ch7_IRQn                = 82U,  /* DMAPITMR CHNL7 interrupt */
    PITMR1_Ch8_IRQn                = 83U,  /* DMAPITMR CHNL8 interrupt */
    PITMR1_Ch9_IRQn                = 84U,  /* DMAPITMR CHNL9 interrupt */
    CAN0_Busoff_IRQn               = 85U,  /* CAN0 Interrupt indicating that Bus Off */
    CAN0_Error_IRQn                = 86U,  /* CAN0 Interrupt indicating that errors were detected on the CAN bus */
    CAN0_ORed_0_127_MB_IRQn        = 87U,  /* CAN0 OR'ed Message buffer (0-63) */
    CAN0_Tx_Warning_IRQn           = 88U,  /* CAN0 Interrupt Transmit Warning  */
    CAN0_Rx_Warning_IRQn           = 89U,  /* CAN0 Interrupt Receive Warning */
    CAN0_Wake_Up_Match_IRQn        = 90U,  /* CAN0 Interrupt asserted when Pretended Networking operation is enabled, and a
                                        valid message matches the selected filter criteria during Low Power mode */
    CAN0_Wake_Up_To_IRQn           = 91U,  /* CAN0 Interrupt asserted when Pretended Networking operation is enabled, and a valid
                                   message matches the selected filter criteria during Low Power mode */
    CAN0_Ce_IRQn                   = 92U,  /* CAN0 Interrupt Correctable Error */
    CAN0_Nce_Ha_IRQn               = 93U,  /* CAN0 Interrupt Host access with Non-Correctable Error */
    CAN0_Nce_Fa_IRQn               = 94U,  /* CAN0 Interrupt Flexcan access with Non-Correctable Error */
    CAN0_Busoff_Done_IRQn          = 95U,  /* CAN0 Interrupt Bus Off Done */
    CAN0_Error_Fd_IRQn             = 96U,  /* CAN0 Interrupt indicating that errors were detected on the CAN bus */
    CAN0_Erfufw_IRQn               = 97U,  /* CAN0 Interrupt indicating that Enhanced Rx FIFO Underflow */
    CAN0_Erfovf_IRQn               = 98U,  /* CAN0 Interrupt indicating that Enhanced Rx FIFO Overflow*/
    CAN0_Erfwm_IRQn                = 99U,  /* CAN0 Interrupt indicating that Enhanced Rx FIFO Watermark*/
    CAN0_Erfda_IRQn                = 100U, /* CAN0 Interrupt indicating that Enhanced Rx FIFO Data Available*/
    CAN1_Busoff_IRQn               = 101U, /* CAN1 Interrupt indicating that Bus Off */
    CAN1_Error_IRQn                = 102U, /* CAN1 Interrupt indicating that errors were detected on the CAN bus */
    CAN1_ORed_0_16_MB_IRQn         = 103U, /* CAN1 OR'ed Message buffer (0-63) */
    CAN1_Tx_Warning_IRQn           = 104U, /* CAN1 Interrupt Transmit Warning  */
    CAN1_Rx_Warning_IRQn           = 105U, /* CAN1 Interrupt Receive Warning */
    CAN1_Wake_Up_Match_IRQn        = 106U, /* CAN1 Interrupt asserted when Pretended Networking operation is enabled, and a
                                       valid message matches the selected filter criteria during Low Power mode */
    CAN1_Wake_Up_To_IRQn           = 107U, /* CAN1 Interrupt asserted when Pretended Networking operation is enabled, and a valid
                                    message matches the selected filter criteria during Low Power mode */
    CAN1_Ce_IRQn                   = 108U, /* CAN1 Interrupt Correctable Error */
    CAN1_Nce_Ha_IRQn               = 109U, /* CAN1 Interrupt Host access with Non-Correctable Error */
    CAN1_Nce_Fa_IRQn               = 110U, /* CAN1 Interrupt Flexcan access with Non-Correctable Error */
    CAN1_Busoff_Done_IRQn          = 111U, /* CAN1 Interrupt Bus Off Done */
    CAN1_Error_Fd_IRQn             = 112U, /* CAN1 Interrupt indicating that errors were detected on the CAN bus */
    CAN1_Erfufw_IRQn               = 113U, /* CAN1 Interrupt indicating that Enhanced Rx FIFO Underflow */
    CAN1_Erfovf_IRQn               = 114U, /* CAN1 Interrupt indicating that Enhanced Rx FIFO Overflow*/
    CAN1_Erfwm_IRQn                = 115U, /* CAN1 Interrupt indicating that Enhanced Rx FIFO Watermark*/
    CAN1_Erfda_IRQn                = 116U, /* CAN1 Interrupt indicating that Enhanced Rx FIFO Data Available*/
    CAN2_Busoff_IRQn               = 117U, /* CAN2 Interrupt indicating that Bus Off */
    CAN2_Error_IRQn                = 118U, /* CAN2 Interrupt indicating that errors were detected on the CAN bus */
    CAN2_ORed_0_128_MB_IRQn        = 119U, /* CAN2 OR'ed Message buffer (0-63) */
    CAN2_Tx_Warning_IRQn           = 120U, /* CAN2 Interrupt Transmit Warning  */
    CAN2_Rx_Warning_IRQn           = 121U, /* CAN2 Interrupt Receive Warning */
    CAN2_Wake_Up_Match_IRQn        = 122U, /* CAN2 Interrupt asserted when Pretended Networking operation is enabled, and a
                                       valid message matches the selected filter criteria during Low Power mode */
    CAN2_Wake_Up_To_IRQn           = 123U, /* CAN2 Interrupt asserted when Pretended Networking operation is enabled, and a valid
                                    message matches the selected filter criteria during Low Power mode */
    CAN2_Ce_IRQn                   = 124U, /* CAN2 Interrupt Correctable Error */
    CAN2_Nce_Ha_IRQn               = 125U, /* CAN2 Interrupt Host access with Non-Correctable Error */
    CAN2_Nce_Fa_IRQn               = 126U, /* CAN2 Interrupt Flexcan access with Non-Correctable Error */
    CAN2_Busoff_Done_IRQn          = 127U, /* CAN2 Interrupt Bus Off Done */
    CAN2_Error_Fd_IRQn             = 128U, /* CAN2 Interrupt indicating that errors were detected on the CAN bus */
    CAN2_Erfufw_IRQn               = 129U, /* CAN2 Interrupt indicating that Enhanced Rx FIFO Underflow */
    CAN2_Erfovf_IRQn               = 130U, /* CAN2 Interrupt indicating that Enhanced Rx FIFO Overflow*/
    CAN2_Erfwm_IRQn                = 131U, /* CAN2 Interrupt indicating that Enhanced Rx FIFO Watermark*/
    CAN2_Erfda_IRQn                = 132U, /* CAN2 Interrupt indicating that Enhanced Rx FIFO Data Available*/
    CAN3_Busoff_IRQn               = 149U, /* CAN3 Interrupt indicating that Bus Off */
    CAN3_Error_IRQn                = 150U, /* CAN3 Interrupt indicating that errors were detected on the CAN bus */
    CAN3_ORed_0_128_MB_IRQn        = 151U, /* CAN3 OR'ed Message buffer (0-63) */
    CAN3_Tx_Warning_IRQn           = 152U, /* CAN3 Interrupt Transmit Warning  */
    CAN3_Rx_Warning_IRQn           = 153U, /* CAN3 Interrupt Receive Warning */
    CAN3_Wake_Up_Match_IRQn        = 154U, /* CAN3 Interrupt asserted when Pretended Networking operation is enabled, and a
                                       valid message matches the selected filter criteria during Low Power mode */
    CAN3_Wake_Up_To_IRQn           = 155U, /* CAN3 Interrupt asserted when Pretended Networking operation is enabled, and a valid
                                    message matches the selected filter criteria during Low Power mode */
    CAN3_Ce_IRQn                   = 156U, /* CAN3 Interrupt Correctable Error */
    CAN3_Nce_Ha_IRQn               = 157U, /* CAN3 Interrupt Host access with Non-Correctable Error */
    CAN3_Nce_Fa_IRQn               = 158U, /* CAN3 Interrupt Flexcan access with Non-Correctable Error */
    CAN3_Busoff_Done_IRQn          = 159U, /* CAN3 Interrupt Bus Off Done */
    CAN3_Error_Fd_IRQn             = 160U, /* CAN3 Interrupt indicating that errors were detected on the CAN bus */
    CAN3_Erfufw_IRQn               = 161U, /* CAN3 Interrupt indicating that Enhanced Rx FIFO Underflow */
    CAN3_Erfovf_IRQn               = 162U, /* CAN3 Interrupt indicating that Enhanced Rx FIFO Overflow*/
    CAN3_Erfwm_IRQn                = 163U, /* CAN3 Interrupt indicating that Enhanced Rx FIFO Watermark*/
    CAN3_Erfda_IRQn                = 164U, /* CAN3 Interrupt indicating that Enhanced Rx FIFO Data Available*/
    SPI0_IRQn                      = 181U, /* SPI0 Interrupt */
    SPI0_ASYNC_IRQn                = 182U, /* SPI0 ASYNC Interrupt */
    SPI1_IRQn                      = 183U, /* SPI1 Interrupt */
    SPI1_ASYNC_IRQn                = 184U, /* SPI1 ASYNC Interrupt */
    SPI2_IRQn                      = 185U, /* SPI2 Interrupt */
    SPI2_ASYNC_IRQn                = 186U, /* SPI2 ASYNC Interrupt */
    SPI3_IRQn                      = 187U, /* SPI3 Interrupt */
    SPI3_ASYNC_IRQn                = 188U, /* SPI3 ASYNC Interrupt */
    UART0_Tx_IRQn                  = 189U, /* UART0 Transmit Interrupt */
    UART0_Tx_ASYNC_IRQn            = 190U, /* UART0 Transmit Async Interrupt */
    UART0_Rx_IRQn                  = 191U, /* UART0 Receive Interrupt */
    UART0_Rx_ASYNC_IRQn            = 192U, /* UART0 Receive Async Interrupt */
    UART1_Tx_IRQn                  = 193U, /* UART1 Transmit Interrupt */
    UART1_Tx_ASYNC_IRQn            = 194U, /* UART1 Transmit Async Interrupt */
    UART1_Rx_IRQn                  = 195U, /* UART1 Receive Interrupt */
    UART1_Rx_ASYNC_IRQn            = 196U, /* UART1 Receive Async Interrupt */
    UART2_Tx_IRQn                  = 197U, /* UART2 Transmit Interrupt */
    UART2_Tx_ASYNC_IRQn            = 198U, /* UART2 Transmit Async Interrupt */
    UART2_Rx_IRQn                  = 199U, /* UART2 Receive Interrupt */
    UART2_Rx_ASYNC_IRQn            = 200U, /* UART2 Receive Async Interrupt */
    UART3_Tx_IRQn                  = 201U, /* UART3 Transmit Interrupt */
    UART3_Tx_ASYNC_IRQn            = 202U, /* UART3 Transmit Async Interrupt */
    UART3_Rx_IRQn                  = 203U, /* UART3 Receive Interrupt */
    UART3_Rx_ASYNC_IRQn            = 204U, /* UART3 Receive Async Interrupt */
    UART4_Tx_IRQn                  = 205U, /* UART4 Transmit Interrupt */
    UART4_Tx_ASYNC_IRQn            = 206U, /* UART4 Transmit Async Interrupt */
    UART4_Rx_IRQn                  = 207U, /* UART4 Receive Interrupt */
    UART4_Rx_ASYNC_IRQn            = 208U, /* UART4 Receive Async Interrupt */
    UART5_Tx_IRQn                  = 209U, /* UART5 Transmit Interrupt */
    UART5_Tx_ASYNC_IRQn            = 210U, /* UART5 Transmit Async Interrupt */
    UART5_Rx_IRQn                  = 211U, /* UART5 Receive Interrupt */
    UART5_Rx_ASYNC_IRQn            = 212U, /* UART5 Receive Async Interrupt */
    I2C0_Master_IRQn               = 213U, /* I2C0 Master Interrupt */
    I2C0_Master_Async_IRQn         = 214U, /* I2C0 Master Async Interrupt */
    I2C0_Slave_IRQn                = 215U, /* I2C0 Slave Interrupt */
    I2C0_Slave_Async_IRQn          = 216U, /* I2C0 Slave Async Interrupt */
    I2C1_Master_IRQn               = 217U, /* I2C1 Master Interrupt */
    I2C1_Master_Async_IRQn         = 218U, /* I2C1 Master Async Interrupt */
    I2C1_Slave_IRQn                = 219U, /* I2C1 Slave Interrupt */
    I2C1_Slave_Async_IRQn          = 220U, /* I2C1 Slave Async Interrupt */
    SUPERIO_IRQn                   = 221U, /* SuperIO Interrupt */
    SUPERIO_Async_IRQn             = 222U, /* SuperIO Async Interrupt */
    SUPERTMR0_Ch0_IRQn             = 223U, /* SUPERTMR0 Channel 0 interrupt */
    SUPERTMR0_Ch1_IRQn             = 224U, /* SUPERTMR0 Channel 1 interrupt */
    SUPERTMR0_Ch2_IRQn             = 225U, /* SUPERTMR0 Channel 2 interrupt */
    SUPERTMR0_Ch3_IRQn             = 226U, /* SUPERTMR0 Channel 3 interrupt */
    SUPERTMR0_Ch4_IRQn             = 227U, /* SUPERTMR0 Channel 4 interrupt */
    SUPERTMR0_Ch5_IRQn             = 228U, /* SUPERTMR0 Channel 5 interrupt */
    SUPERTMR0_Ch6_IRQn             = 229U, /* SUPERTMR0 Channel 6 interrupt */
    SUPERTMR0_Ch7_IRQn             = 230U, /* SUPERTMR0 Channel 7 interrupt */
    SUPERTMR0_Fault_IRQn           = 231U, /* SUPERTMR0 Fault interrupt */
    SUPERTMR0_Ovf_IRQn             = 232U, /* SUPERTMR0 Counter overflow interrupt */
    SUPERTMR0_Reload_IRQn          = 233U, /* SUPERTMR0 Counter Reload interrupt */
    SUPERTMR1_Ch0_IRQn             = 234U, /* SUPERTMR1 Channel 0 interrupt */
    SUPERTMR1_Ch1_IRQn             = 235U, /* SUPERTMR1 Channel 1 interrupt */
    SUPERTMR1_Ch2_IRQn             = 236U, /* SUPERTMR1 Channel 2 interrupt */
    SUPERTMR1_Ch3_IRQn             = 237U, /* SUPERTMR1 Channel 3 interrupt */
    SUPERTMR1_Ch4_IRQn             = 238U, /* SUPERTMR1 Channel 4 interrupt */
    SUPERTMR1_Ch5_IRQn             = 239U, /* SUPERTMR1 Channel 5 interrupt */
    SUPERTMR1_Ch6_IRQn             = 240U, /* SUPERTMR1 Channel 6 interrupt */
    SUPERTMR1_Ch7_IRQn             = 241U, /* SUPERTMR1 Channel 7 interrupt */
    SUPERTMR1_Fault_IRQn           = 242U, /* SUPERTMR1 Fault interrupt */
    SUPERTMR1_Ovf_IRQn             = 243U, /* SUPERTMR1 Counter overflow interrupt */
    SUPERTMR1_Reload_IRQn          = 244U, /* SUPERTMR1 Counter Reload interrupt */
    SUPERTMR2_Ch0_IRQn             = 245U, /* SUPERTMR2 Channel 0 interrupt */
    SUPERTMR2_Ch1_IRQn             = 246U, /* SUPERTMR2 Channel 1 interrupt */
    SUPERTMR2_Ch2_IRQn             = 247U, /* SUPERTMR2 Channel 2 interrupt */
    SUPERTMR2_Ch3_IRQn             = 248U, /* SUPERTMR2 Channel 3 interrupt */
    SUPERTMR2_Ch4_IRQn             = 249U, /* SUPERTMR2 Channel 4 interrupt */
    SUPERTMR2_Ch5_IRQn             = 250U, /* SUPERTMR2 Channel 5 interrupt */
    SUPERTMR2_Ch6_IRQn             = 251U, /* SUPERTMR2 Channel 6 interrupt */
    SUPERTMR2_Ch7_IRQn             = 252U, /* SUPERTMR2 Channel 7 interrupt */
    SUPERTMR2_Fault_IRQn           = 253U, /* SUPERTMR2 Fault interrupt */
    SUPERTMR2_Ovf_IRQn             = 254U, /* SUPERTMR2 Counter overflow interrupt */
    SUPERTMR2_Reload_IRQn          = 255U, /* SUPERTMR2 Counter Reload interrupt */
    SUPERTMR3_Ch0_IRQn             = 256U, /* SUPERTMR3 Channel 0 interrupt */
    SUPERTMR3_Ch1_IRQn             = 257U, /* SUPERTMR3 Channel 1 interrupt */
    SUPERTMR3_Ch2_IRQn             = 258U, /* SUPERTMR3 Channel 2 interrupt */
    SUPERTMR3_Ch3_IRQn             = 259U, /* SUPERTMR3 Channel 3 interrupt */
    SUPERTMR3_Ch4_IRQn             = 260U, /* SUPERTMR3 Channel 4 interrupt */
    SUPERTMR3_Ch5_IRQn             = 261U, /* SUPERTMR3 Channel 5 interrupt */
    SUPERTMR3_Ch6_IRQn             = 262U, /* SUPERTMR3 Channel 6 interrupt */
    SUPERTMR3_Ch7_IRQn             = 263U, /* SUPERTMR3 Channel 7 interrupt */
    SUPERTMR3_Fault_IRQn           = 264U, /* SUPERTMR3 Fault interrupt */
    SUPERTMR3_Ovf_IRQn             = 265U, /* SUPERTMR3 Counter overflow interrupt */
    SUPERTMR3_Reload_IRQn          = 266U, /* SUPERTMR3 Counter Reload interrupt */
    SUPERTMR4_Ch0_IRQn             = 267U, /* SUPERTMR4 Channel 0 interrupt */
    SUPERTMR4_Ch1_IRQn             = 268U, /* SUPERTMR4 Channel 1 interrupt */
    SUPERTMR4_Ch2_IRQn             = 269U, /* SUPERTMR4 Channel 2 interrupt */
    SUPERTMR4_Ch3_IRQn             = 270U, /* SUPERTMR4 Channel 3 interrupt */
    SUPERTMR4_Ch4_IRQn             = 271U, /* SUPERTMR4 Channel 4 interrupt */
    SUPERTMR4_Ch5_IRQn             = 272U, /* SUPERTMR4 Channel 5 interrupt */
    SUPERTMR4_Ch6_IRQn             = 273U, /* SUPERTMR4 Channel 6 interrupt */
    SUPERTMR4_Ch7_IRQn             = 274U, /* SUPERTMR4 Channel 7 interrupt */
    SUPERTMR4_Fault_IRQn           = 275U, /* SUPERTMR4 Fault interrupt */
    SUPERTMR4_Ovf_IRQn             = 276U, /* SUPERTMR4 Counter overflow interrupt */
    SUPERTMR4_Reload_IRQn          = 277U, /* SUPERTMR4 Counter Reload interrupt */
    SUPERTMR5_Ch0_IRQn             = 278U, /* SUPERTMR5 Channel 0 interrupt */
    SUPERTMR5_Ch1_IRQn             = 279U, /* SUPERTMR5 Channel 1 interrupt */
    SUPERTMR5_Ch2_IRQn             = 280U, /* SUPERTMR5 Channel 2 interrupt */
    SUPERTMR5_Ch3_IRQn             = 281U, /* SUPERTMR5 Channel 3 interrupt */
    SUPERTMR5_Ch4_IRQn             = 282U, /* SUPERTMR5 Channel 4 interrupt */
    SUPERTMR5_Ch5_IRQn             = 283U, /* SUPERTMR5 Channel 5 interrupt */
    SUPERTMR5_Ch6_IRQn             = 284U, /* SUPERTMR5 Channel 6 interrupt */
    SUPERTMR5_Ch7_IRQn             = 285U, /* SUPERTMR5 Channel 7 interrupt */
    SUPERTMR5_Fault_IRQn           = 286U, /* SUPERTMR5 Fault interrupt */
    SUPERTMR5_Ovf_IRQn             = 287U, /* SUPERTMR5 Counter overflow interrupt */
    SUPERTMR5_Reload_IRQn          = 288U, /* SUPERTMR5 Counter Reload interrupt */
    PCTMR0_IRQn                    = 289U, /* PCTMR interrupt */
    PCTMR1_IRQn                    = 290U, /* PCTMR interrupt */
    PCTMR2_IRQn                    = 291U, /* PCTMR interrupt */
    PCTMR3_IRQn                    = 292U, /* PCTMR interrupt */
    PCTMR4_IRQn                    = 293U, /* PCTMR interrupt */
    PCTMR5_IRQn                    = 294U, /* PCTMR interrupt */
    PCTMR6_IRQn                    = 295U, /* PCTMR interrupt */
    PCTMR7_IRQn                    = 296U, /* PCTMR interrupt */
    PCTMR8_IRQn                    = 297U, /* PCTMR interrupt */
    PITMR0_GL_IRQn                 = 298U, /* PITMR0 global interrupt */
    PITMR0_Ch0_IRQn                = 299U, /* PITMR0 channel 0 overflow interrupt */
    PITMR0_Ch1_IRQn                = 300U, /* PITMR0 channel 1 overflow interrupt */
    PITMR0_Ch2_IRQn                = 301U, /* PITMR0 channel 2 overflow interrupt */
    PITMR0_Ch3_IRQn                = 302U, /* PITMR0 channel 3 overflow interrupt */
    EWM_IRQ                        = 303U, /* EWM Interrupt  */
    ADC0_IRQn                      = 304U, /* ADC0 interrupt request */
    ADC0_Tx_IRQn                   = 305U, /* ADC0 interrupt request */
    ADC1_IRQn                      = 306U, /* ADC1 interrupt request */
    ADC1_Tx_IRQn                   = 307U, /* ADC0 interrupt request */
    PDU0_IRQn                      = 308U, /* PDU0 Interrupt */
    PDU0_ERR_IRQn                  = 309U, /* PDU0 Error Interrupt */
    PDU1_IRQn                      = 310U, /* PDU1 Interrupt */
    PDU1_ERR_IRQn                  = 311U, /* PDU1 Error Interrupt */
    GTMR_IRQ                       = 312U, /* GTMR interrupt request */
    CMP0_IRQn                      = 313U, /* CMP0 interrupt request */
    CMP0_STOP_IRQn                 = 314U, /* CMP0 STOP interrupt request */
    DAC0_IRQn                      = 315U, /* DAC0 interrupt request */
    PWM0_IRQn                      = 316U, /* PWM0 interrupt */
    PWM1_IRQn                      = 317U, /* PWM1 interrupt */
    PWM2_IRQn                      = 318U, /* PWM2 interrupt */
    PWM3_IRQn                      = 319U, /* PWM3 interrupt */
    PWM4_IRQn                      = 320U, /* PWM4 interrupt */
    GPIO_All_IRQn                  = 321U, /* Gpio all pin detect interrupt */
    GPIOA_IRQn                     = 322U, /* Gpio A 0-31 pin detect interrupt */
    GPIOB_IRQn                     = 323U, /* Gpio B 0-31 pin detect interrupt */
    GPIOC_IRQn                     = 324U, /* Gpio C 0-31 pin detect interrupt */
    GPIOD_IRQn                     = 325U, /* Gpio D 0-31 pin detect interrupt */
    GPIOA_0_IRQn                   = 326U, /* Gpio A 0 pin detect interrupt */
    GPIOA_1_IRQn                   = 327U, /* Gpio A 1 pin detect interrupt */
    GPIOA_2_IRQn                   = 328U, /* Gpio A 2 pin detect interrupt */
    GPIOA_3_IRQn                   = 329U, /* Gpio A 3 pin detect interrupt */
    GPIOA_4_IRQn                   = 330U, /* Gpio A 4 pin detect interrupt */
    GPIOA_5_IRQn                   = 331U, /* Gpio A 5 pin detect interrupt */
    GPIOA_6_IRQn                   = 332U, /* Gpio A 6 pin detect interrupt */
    GPIOA_7_IRQn                   = 333U, /* Gpio A 7 pin detect interrupt */
    GPIOA_8_IRQn                   = 334U, /* Gpio A 8 pin detect interrupt */
    GPIOA_9_IRQn                   = 335U, /* Gpio A 9 pin detect interrupt */
    GPIOA_10_IRQn                  = 336U, /* Gpio A 10 pin detect interrupt */
    GPIOA_11_IRQn                  = 337U, /* Gpio A 11 pin detect interrupt */
    GPIOA_12_IRQn                  = 338U, /* Gpio A 12 pin detect interrupt */
    GPIOA_13_IRQn                  = 339U, /* Gpio A 13 pin detect interrupt */
    GPIOA_14_IRQn                  = 340U, /* Gpio A 14 pin detect interrupt */
    GPIOA_15_IRQn                  = 341U, /* Gpio A 15 pin detect interrupt */
    GPIOA_16_IRQn                  = 342U, /* Gpio A 16 pin detect interrupt */
    GPIOA_17_IRQn                  = 343U, /* Gpio A 17 pin detect interrupt */
    GPIOA_18_IRQn                  = 344U, /* Gpio A 18 pin detect interrupt */
    GPIOA_19_IRQn                  = 345U, /* Gpio A 19 pin detect interrupt */
    GPIOA_20_IRQn                  = 346U, /* Gpio A 20 pin detect interrupt */
    GPIOA_21_IRQn                  = 347U, /* Gpio A 21 pin detect interrupt */
    GPIOA_22_IRQn                  = 348U, /* Gpio A 22 pin detect interrupt */
    GPIOA_23_IRQn                  = 349U, /* Gpio A 23 pin detect interrupt */
    GPIOA_24_IRQn                  = 350U, /* Gpio A 24 pin detect interrupt */
    GPIOA_25_IRQn                  = 351U, /* Gpio A 25 pin detect interrupt */
    GPIOA_26_IRQn                  = 352U, /* Gpio A 26 pin detect interrupt */
    GPIOA_27_IRQn                  = 353U, /* Gpio A 27 pin detect interrupt */
    GPIOA_28_IRQn                  = 354U, /* Gpio A 28 pin detect interrupt */
    GPIOA_29_IRQn                  = 355U, /* Gpio A 29 pin detect interrupt */
    GPIOA_30_IRQn                  = 356U, /* Gpio A 30 pin detect interrupt */
    GPIOA_31_IRQn                  = 357U, /* Gpio A 31 pin detect interrupt */
    GPIOB_0_IRQn                   = 358U, /* Gpio B 0 pin detect interrupt */
    GPIOB_1_IRQn                   = 359U, /* Gpio B 1 pin detect interrupt */
    GPIOB_2_IRQn                   = 360U, /* Gpio B 2 pin detect interrupt */
    GPIOB_3_IRQn                   = 361U, /* Gpio B 3 pin detect interrupt */
    GPIOB_4_IRQn                   = 362U, /* Gpio B 4 pin detect interrupt */
    GPIOB_5_IRQn                   = 363U, /* Gpio B 5 pin detect interrupt */
    GPIOB_6_IRQn                   = 364U, /* Gpio B 6 pin detect interrupt */
    GPIOB_7_IRQn                   = 365U, /* Gpio B 7 pin detect interrupt */
    GPIOB_8_IRQn                   = 366U, /* Gpio B 8 pin detect interrupt */
    GPIOB_9_IRQn                   = 367U, /* Gpio B 9 pin detect interrupt */
    GPIOB_10_IRQn                  = 368U, /* Gpio B 10 pin detect interrupt */
    GPIOB_11_IRQn                  = 369U, /* Gpio B 11 pin detect interrupt */
    GPIOB_12_IRQn                  = 370U, /* Gpio B 12 pin detect interrupt */
    GPIOB_13_IRQn                  = 371U, /* Gpio B 13 pin detect interrupt */
    GPIOB_14_IRQn                  = 372U, /* Gpio B 14 pin detect interrupt */
    GPIOB_15_IRQn                  = 373U, /* Gpio B 15 pin detect interrupt */
    GPIOB_16_IRQn                  = 374U, /* Gpio B 16 pin detect interrupt */
    GPIOB_17_IRQn                  = 375U, /* Gpio B 17 pin detect interrupt */
    GPIOB_18_IRQn                  = 376U, /* Gpio B 18 pin detect interrupt */
    GPIOB_19_IRQn                  = 377U, /* Gpio B 19 pin detect interrupt */
    GPIOB_20_IRQn                  = 378U, /* Gpio B 20 pin detect interrupt */
    GPIOB_21_IRQn                  = 379U, /* Gpio B 21 pin detect interrupt */
    GPIOB_22_IRQn                  = 380U, /* Gpio B 22 pin detect interrupt */
    GPIOB_23_IRQn                  = 381U, /* Gpio B 23 pin detect interrupt */
    GPIOB_24_IRQn                  = 382U, /* Gpio B 24 pin detect interrupt */
    GPIOB_25_IRQn                  = 383U, /* Gpio B 25 pin detect interrupt */
    GPIOB_26_IRQn                  = 384U, /* Gpio B 26 pin detect interrupt */
    GPIOB_27_IRQn                  = 385U, /* Gpio B 27 pin detect interrupt */
    GPIOB_28_IRQn                  = 386U, /* Gpio B 28 pin detect interrupt */
    GPIOB_29_IRQn                  = 387U, /* Gpio B 29 pin detect interrupt */
    GPIOB_30_IRQn                  = 388U, /* Gpio B 30 pin detect interrupt */
    GPIOB_31_IRQn                  = 389U, /* Gpio B 31 pin detect interrupt */
    GPIOC_0_IRQn                   = 390U, /* Gpio C 0 pin detect interrupt */
    GPIOC_1_IRQn                   = 391U, /* Gpio C 1 pin detect interrupt */
    GPIOC_2_IRQn                   = 392U, /* Gpio C 2 pin detect interrupt */
    GPIOC_3_IRQn                   = 393U, /* Gpio C 3 pin detect interrupt */
    GPIOC_4_IRQn                   = 394U, /* Gpio C 4 pin detect interrupt */
    GPIOC_5_IRQn                   = 395U, /* Gpio C 5 pin detect interrupt */
    GPIOC_6_IRQn                   = 396U, /* Gpio C 6 pin detect interrupt */
    GPIOC_7_IRQn                   = 397U, /* Gpio C 7 pin detect interrupt */
    GPIOC_8_IRQn                   = 398U, /* Gpio C 8 pin detect interrupt */
    GPIOC_9_IRQn                   = 399U, /* Gpio C 9 pin detect interrupt */
    GPIOC_10_IRQn                  = 400U, /* Gpio C 10 pin detect interrupt */
    GPIOC_11_IRQn                  = 401U, /* Gpio C 11 pin detect interrupt */
    GPIOC_12_IRQn                  = 402U, /* Gpio C 12 pin detect interrupt */
    GPIOC_13_IRQn                  = 403U, /* Gpio C 13 pin detect interrupt */
    GPIOC_14_IRQn                  = 404U, /* Gpio C 14 pin detect interrupt */
    GPIOC_15_IRQn                  = 405U, /* Gpio C 15 pin detect interrupt */
    GPIOC_16_IRQn                  = 406U, /* Gpio C 16 pin detect interrupt */
    GPIOC_17_IRQn                  = 407U, /* Gpio C 17 pin detect interrupt */
    GPIOC_18_IRQn                  = 408U, /* Gpio C 18 pin detect interrupt */
    GPIOC_19_IRQn                  = 409U, /* Gpio C 19 pin detect interrupt */
    GPIOC_20_IRQn                  = 410U, /* Gpio C 20 pin detect interrupt */
    GPIOC_21_IRQn                  = 411U, /* Gpio C 21 pin detect interrupt */
    GPIOC_22_IRQn                  = 412U, /* Gpio C 22 pin detect interrupt */
    GPIOC_23_IRQn                  = 413U, /* Gpio C 23 pin detect interrupt */
    GPIOC_24_IRQn                  = 414U, /* Gpio C 24 pin detect interrupt */
    GPIOC_25_IRQn                  = 415U, /* Gpio C 25 pin detect interrupt */
    GPIOC_26_IRQn                  = 416U, /* Gpio C 26 pin detect interrupt */
    GPIOC_27_IRQn                  = 417U, /* Gpio C 27 pin detect interrupt */
    GPIOC_28_IRQn                  = 418U, /* Gpio C 28 pin detect interrupt */
    GPIOC_29_IRQn                  = 419U, /* Gpio C 29 pin detect interrupt */
    GPIOC_30_IRQn                  = 420U, /* Gpio C 30 pin detect interrupt */
    GPIOC_31_IRQn                  = 421U, /* Gpio C 31 pin detect interrupt */
    GPIOD_0_IRQn                   = 422U, /* Gpio D 0 pin detect interrupt */
    GPIOD_1_IRQn                   = 423U, /* Gpio D 1 pin detect interrupt */
    GPIOD_2_IRQn                   = 424U, /* Gpio D 2 pin detect interrupt */
    GPIOD_3_IRQn                   = 425U, /* Gpio D 3 pin detect interrupt */
    GPIOD_4_IRQn                   = 426U, /* Gpio D 4 pin detect interrupt */
    GPIOD_5_IRQn                   = 427U, /* Gpio D 5 pin detect interrupt */
    GPIOD_6_IRQn                   = 428U, /* Gpio D 6 pin detect interrupt */
    GPIOD_7_IRQn                   = 429U, /* Gpio D 7 pin detect interrupt */
    GPIOD_8_IRQn                   = 430U, /* Gpio D 8 pin detect interrupt */
    GPIOD_9_IRQn                   = 431U, /* Gpio D 9 pin detect interrupt */
    GPIOD_10_IRQn                  = 432U, /* Gpio D 10 pin detect interrupt */
    GPIOD_11_IRQn                  = 433U, /* Gpio D 11 pin detect interrupt */
    GPIOD_12_IRQn                  = 434U, /* Gpio D 12 pin detect interrupt */
    GPIOD_13_IRQn                  = 435U, /* Gpio D 13 pin detect interrupt */
    GPIOD_14_IRQn                  = 436U, /* Gpio D 14 pin detect interrupt */
    GPIOD_15_IRQn                  = 437U, /* Gpio D 15 pin detect interrupt */
    GPIOD_16_IRQn                  = 438U, /* Gpio D 16 pin detect interrupt */
    GPIOD_17_IRQn                  = 439U, /* Gpio D 17 pin detect interrupt */
    GPIOD_18_IRQn                  = 440U, /* Gpio D 18 pin detect interrupt */
    GPIOD_19_IRQn                  = 441U, /* Gpio D 19 pin detect interrupt */
    GPIOD_20_IRQn                  = 442U, /* Gpio D 20 pin detect interrupt */
    GPIOD_21_IRQn                  = 443U, /* Gpio D 21 pin detect interrupt */
    GPIOD_22_IRQn                  = 444U, /* Gpio D 22 pin detect interrupt */
    GPIOD_23_IRQn                  = 445U, /* Gpio D 23 pin detect interrupt */
    GPIOD_24_IRQn                  = 446U, /* Gpio D 24 pin detect interrupt */
    GPIOD_25_IRQn                  = 447U, /* Gpio D 25 pin detect interrupt */
    GPIOD_26_IRQn                  = 448U, /* Gpio D 26 pin detect interrupt */
    GPIOD_27_IRQn                  = 449U, /* Gpio D 27 pin detect interrupt */
    GPIOD_28_IRQn                  = 450U, /* Gpio D 28 pin detect interrupt */
    GPIOD_29_IRQn                  = 451U, /* Gpio D 29 pin detect interrupt */
    GPIOD_30_IRQn                  = 452U, /* Gpio D 30 pin detect interrupt */
    GPIOD_31_IRQn                  = 453U, /* Gpio D 31 pin detect interrupt */
    PDMA_ECC_Error_IRQn            = 465U, /* Pdma Ecc Err interrupt */
    COM_ECC_Error_IRQn             = 466U, /* Com Ecc Err interrupt */
    SPI_ECC_Error_IRQn             = 467U, /* Spi Ecc Err interrupt */
    CAN_ECC_Error_IRQn             = 468U, /* Can Ecc Err interrupt */
    PERIPH_BUS_MON_IRQn            = 469U, /* Periph Bus Mon interrupt */
    DEBUG_ECC_Error_IRQn           = 480U, /* Debug Ecc Err interrupt */
    HW_CRC_ECC_Error_IRQn          = 481U, /* Hw Crc Ecc Err interrupt */
    PERIPH_PORT_ECC_IRQn           = 482U, /* Periph Port Ecc interrupt */
    ADC_ECC_IRQn                   = 483U, /* Adc Ecc interrupt */
    CPU_ECC_IRQn                   = 484U, /* Cpu Ecc interrupt */
    SAFETY_ECC_IRQn                = 485U, /* Safety Ecc interrupt */
    CRG_ECC_IRQn                   = 486U, /* Crg Ecc interrupt */
    AON_ECC_IRQn                   = 487U, /* Aon Ecc interrupt */
    PINMUX_ECC_IRQn                = 488U, /* Pinmux Ecc interrupt */
    FMC_ECC_IRQn                   = 489U, /* Fmc Ecc interrupt */
    MAIN_BUS_MON_IRQn              = 490U, /* Main Bus Mon interrupt */
    ADC_SlvPortTimeout_Error_IRQn  = 491U, /* Adc Slv Port Time Out interrupt */
    AON_PortTimeout_Error_IRQn     = 492U, /* Aon Port Time Out interrupt */
    CRG_SlvPortTimeout_Error_IRQn  = 493U, /* Crg Slv Port Time Out interrupt */
    DFLASH_PortTimeout_Error_IRQn  = 494U, /* Dflash Port Time Out interrupt */
    FLEXNVM_PortTimeout_Error_IRQn = 495U, /* Flexnvm Port Time Out interrupt */
    FRONT_PortTimeout_Error_IRQn   = 496U, /* Front Port Time Out interrupt */
    PFLASH_PortTimeout_Error_IRQn  = 497U, /* Pflash Port Time Out interrupt */
    SAFETY_PortTimeout_Error_IRQn  = 498U, /* Safety Port Time Out interrupt */
    PINMUX_PortTimeout_Error_IRQn  = 499U, /* Pinmux Port Time Out interrupt */
    CAN0_SlvPortTimeout_Error_IRQn = 500U, /* Can0 Slv Port Time Out interrupt */
    CAN1_SlvPortTimeout_Error_IRQn = 501U, /* Can1 Slv Port Time Out interrupt */
    CAN2_SlvPortTimeout_Error_IRQn = 502U, /* Can2 Slv Port Time Out interrupt */
    COM_PortTimeout_Error_IRQn     = 503U, /* Com Port Time Out interrupt */
    PDMA_SlvPortTimeout_Error_IRQn = 504U, /* Pdma Slv Port Time Out interrupt */
    SPI0_PortTimeout_Error_IRQn    = 505U, /* Spi0 Port Time Out interrupt */
    SPI1_PortTimeout_Error_IRQn    = 506U, /* Spi1 Port Time Out interrupt */
    SPI2_PortTimeout_Error_IRQn    = 507U, /* Spi2 Port Time Out interrupt */
    SPI3_PortTimeout_Error_IRQn    = 508U, /* Spi3 Port Time Out interrupt */
    NMI_B_IRQn                     = 511U, /* Nmi B interrupt */
    SOC_INT_MAX                    = NUMBER_OF_INT_VECTORS
} IRQn_Type;

typedef enum {
    IRQ_MODE_0 = 0x0U, /* 1 interrupt level, 32 interrupt priority */
    IRQ_MODE_1 = 0x1U, /* 2  interrupt level, 16 interrupt priority */
    IRQ_MODE_2 = 0x2U, /* 4  interrupt level, 8  interrupt priority */
    IRQ_MODE_3 = 0x3U, /* 8  interrupt level, 4  interrupt priority */
    IRQ_MODE_4 = 0x4U, /* 16  interrupt level, 2  interrupt priority */
    IRQ_MODE_5 = 0x5U, /* 32  interrupt level, 1  interrupt priority */
} irq_mode_t;

/* interrupt priority */
typedef enum {
    IRQ_PRIORITY_0  = 0U,
    IRQ_PRIORITY_1  = 1U,
    IRQ_PRIORITY_2  = 2U,
    IRQ_PRIORITY_3  = 3U,
    IRQ_PRIORITY_4  = 4U,
    IRQ_PRIORITY_5  = 5U,
    IRQ_PRIORITY_6  = 6U,
    IRQ_PRIORITY_7  = 7U,
    IRQ_PRIORITY_8  = 8U,
    IRQ_PRIORITY_9  = 9U,
    IRQ_PRIORITY_10 = 10U,
    IRQ_PRIORITY_11 = 11U,
    IRQ_PRIORITY_12 = 12U,
    IRQ_PRIORITY_13 = 13U,
    IRQ_PRIORITY_14 = 14U,
    IRQ_PRIORITY_15 = 15U,
    IRQ_PRIORITY_16 = 16U,
    IRQ_PRIORITY_17 = 17U,
    IRQ_PRIORITY_18 = 18U,
    IRQ_PRIORITY_19 = 19U,
    IRQ_PRIORITY_20 = 20U,
    IRQ_PRIORITY_21 = 21U,
    IRQ_PRIORITY_22 = 22U,
    IRQ_PRIORITY_23 = 23U,
    IRQ_PRIORITY_24 = 24U,
    IRQ_PRIORITY_25 = 25U,
    IRQ_PRIORITY_26 = 26U,
    IRQ_PRIORITY_27 = 27U,
    IRQ_PRIORITY_28 = 28U,
    IRQ_PRIORITY_29 = 29U,
    IRQ_PRIORITY_30 = 30U,
    IRQ_PRIORITY_31 = 31U,
} irq_priority_t;

/* interrupt level */
typedef enum {
    IRQ_LEVEL_0  = 0U,
    IRQ_LEVEL_1  = 1U,
    IRQ_LEVEL_2  = 2U,
    IRQ_LEVEL_3  = 3U,
    IRQ_LEVEL_4  = 4U,
    IRQ_LEVEL_5  = 5U,
    IRQ_LEVEL_6  = 6U,
    IRQ_LEVEL_7  = 7U,
    IRQ_LEVEL_8  = 8U,
    IRQ_LEVEL_9  = 9U,
    IRQ_LEVEL_10 = 10U,
    IRQ_LEVEL_11 = 11U,
    IRQ_LEVEL_12 = 12U,
    IRQ_LEVEL_13 = 13U,
    IRQ_LEVEL_14 = 14U,
    IRQ_LEVEL_15 = 15U,
    IRQ_LEVEL_16 = 16U,
    IRQ_LEVEL_17 = 17U,
    IRQ_LEVEL_18 = 18U,
    IRQ_LEVEL_19 = 19U,
    IRQ_LEVEL_20 = 20U,
    IRQ_LEVEL_21 = 21U,
    IRQ_LEVEL_22 = 22U,
    IRQ_LEVEL_23 = 23U,
    IRQ_LEVEL_24 = 24U,
    IRQ_LEVEL_25 = 25U,
    IRQ_LEVEL_26 = 26U,
    IRQ_LEVEL_27 = 27U,
    IRQ_LEVEL_28 = 28U,
    IRQ_LEVEL_29 = 29U,
    IRQ_LEVEL_30 = 30U,
    IRQ_LEVEL_31 = 31U,
} irq_level_t;

#endif

/**
 * @}
 */
/* end of group Interrupt_Vector_Numbers_EAM2011 */

#endif /* __EAM2011_IRQN__ */
