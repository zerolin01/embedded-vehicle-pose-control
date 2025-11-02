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
 * @file wdog_register.h
 * @brief WDOG driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __WDOG_REGISTER_H__
#define __WDOG_REGISTER_H__

#include <osal.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- WDOG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup WDOG_Peripheral_Access_Layer WDOG Peripheral Access Layer
 * @{
 */ 

/**
 * @brief Register Layout Typedef 
 */
typedef struct {
  __IO uint32_t WCR;                                /* Watchdog Control Register, offset: 0x0 */
  __IO uint32_t WSR;                                /* Watchdog Service Register, offset: 0x4 */
  __IO uint32_t WRSR;                               /* Watchdog Reset Status Register, offset: 0x8 */
  __IO uint32_t WICR;                               /* Watchdog Interrupt Control Register, offset: 0xC */
} wdog_type_t;

/* ----------------------------------------------------------------------------
   -- WDOG Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup WDOG_Register_Masks WDOG Register Masks
 * @{
 */

/* WCR Bit Fields */
#define WDOG_WCR_WT_MASK                          0xFFFF00U
#define WDOG_WCR_WT_SHIFT                         8U
#define WDOG_WCR_WT_WIDTH                         16U
#define WDOG_WCR_WT(x)                            (((uint32_t)(((uint32_t)(x)) << WDOG_WCR_WT_SHIFT)) & WDOG_WCR_WT_MASK)
#define WDOG_WCR_WDW_MASK                         0x80U
#define WDOG_WCR_WDW_SHIFT                        7U
#define WDOG_WCR_WDW_WIDTH                        1U
#define WDOG_WCR_WDW(x)                           (((uint32_t)(((uint32_t)(x)) << WDOG_WCR_WDW_SHIFT)) & WDOG_WCR_WDW_MASK)
#define WDOG_WCR_SRE_MASK                         0x40U
#define WDOG_WCR_SRE_SHIFT                        6U
#define WDOG_WCR_SRE_WIDTH                        1U
#define WDOG_WCR_SRE(x)                           (((uint32_t)(((uint32_t)(x)) << WDOG_WCR_SRE_SHIFT)) & WDOG_WCR_SRE_MASK)
#define WDOG_WCR_SRS_MASK                         0x10U
#define WDOG_WCR_SRS_SHIFT                        4U
#define WDOG_WCR_SRS_WIDTH                        1U
#define WDOG_WCR_SRS(x)                           (((uint32_t)(((uint32_t)(x)) << WDOG_WCR_SRS_SHIFT)) & WDOG_WCR_SRS_MASK)
#define WDOG_WCR_WDE_MASK                         0x4U
#define WDOG_WCR_WDE_SHIFT                        2U
#define WDOG_WCR_WDE_WIDTH                        1U
#define WDOG_WCR_WDE(x)                           (((uint32_t)(((uint32_t)(x)) << WDOG_WCR_WDE_SHIFT)) & WDOG_WCR_WDE_MASK)
#define WDOG_WCR_WDBG_MASK                        0x2U
#define WDOG_WCR_WDBG_SHIFT                       1U
#define WDOG_WCR_WDBG_WIDTH                       1U
#define WDOG_WCR_WDBG(x)                          (((uint32_t)(((uint32_t)(x)) << WDOG_WCR_WDBG_SHIFT)) & WDOG_WCR_WDBG_MASK)
#define WDOG_WCR_WDZST_MASK                       0x1U
#define WDOG_WCR_WDZST_SHIFT                      0U
#define WDOG_WCR_WDZST_WIDTH                      1U
#define WDOG_WCR_WDZST(x)                         (((uint32_t)(((uint32_t)(x)) << WDOG_WCR_WDZST_SHIFT)) & WDOG_WCR_WDZST_MASK)


/* WSR Bit Fields */
#define WDOG_WSR_WSR_MASK                         0xFFFFU
#define WDOG_WSR_WSR_SHIFT                        0U
#define WDOG_WSR_WSR_WIDTH                        16U
#define WDOG_WSR_WSR(x)                           (((uint32_t)(((uint32_t)(x)) << WDOG_WSR_WSR_SHIFT)) & WDOG_WSR_WSR_MASK)


/* WRSR Bit Fields */
#define WDOG_WRSR_POR_MASK                         0x10U
#define WDOG_WRSR_POR_SHIFT                        4U
#define WDOG_WRSR_POR_WIDTH                        1U
#define WDOG_WRSR_POR(x)                           (((uint32_t)(((uint32_t)(x)) << WDOG_WRSR_POR_SHIFT)) & WDOG_WRSR_POR_MASK)

#define WDOG_WRSR_TOUT_MASK                        0x2U
#define WDOG_WRSR_TOUT_SHIFT                       1U
#define WDOG_WRSR_TOUT_WIDTH                       1U
#define WDOG_WRSR_TOUT(x)                          (((uint32_t)(((uint32_t)(x)) << WDOG_WRSR_TOUT_SHIFT)) & WDOG_WRSR_TOUT_MASK)

#define WDOG_WRSR_SFTW_MASK                        0x1U
#define WDOG_WRSR_SFTW_SHIFT                       1U
#define WDOG_WRSR_SFTW_WIDTH                       1U
#define WDOG_WRSR_SFTW(x)                          (((uint32_t)(((uint32_t)(x)) << WDOG_WRSR_SFTW_SHIFT)) & WDOG_WRSR_SFTW_MASK)

/* WICR Bit Fields */
#define WDOG_WICR_WICT_MASK                        0xFFFF0000U 
#define WDOG_WICR_WICT_SHIFT                       16U
#define WDOG_WICR_WICT_WIDTH                       16U
#define WDOG_WICR_WICT(x)                          (((uint32_t)(((uint32_t)(x)) << WDOG_WICR_WICT_SHIFT)) & WDOG_WICR_WICT_MASK)
#define WDOG_WICR_WIE_MASK                         0x8000U
#define WDOG_WICR_WIE_SHIFT                        15U
#define WDOG_WICR_WIE_WIDTH                        1U
#define WDOG_WICR_WIE(x)                           (((uint32_t)(((uint32_t)(x)) << WDOG_WICR_WIE_SHIFT)) & WDOG_WICR_WIE_MASK)
#define WDOG_WICR_WTIS_MASK                        0x4000U
#define WDOG_WICR_WTIS_SHIFT                       14U
#define WDOG_WICR_WTIS_WIDTH                       1U
#define WDOG_WICR_WTIS(x)                          (((uint32_t)(((uint32_t)(x)) << WDOG_WICR_WTIS_SHIFT)) & WDOG_WICR_WTIS_MASK)

/**
 * @}
 */ /* end of group WDOG_Register_Masks */

 /**
 * @}
 */ /* end of group WDOG_Peripheral_Access_Layer */

// clang-format on

#endif // __WDOG_REGISTER_H__
