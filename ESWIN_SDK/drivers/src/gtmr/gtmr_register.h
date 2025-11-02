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
 * @file gtmr_register.h
 * @brief GTMR driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __GTMR_REGISTER_H__
#define __GTMR_REGISTER_H__

#include <osal.h>
#include <stdint.h>

#define GTMR_SYS_CLK (1000000U)

// clang-format off

/* ----------------------------------------------------------------------------
   -- GTMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup GTMR_Peripheral_Access_Layer GTMR Peripheral Access Layer
 * @{
 */

/**
 * @brief  GTMR Register Layout Typedef.
 */
typedef struct {
  __IO uint32_t CTRL;    /**< GTMR Control Register,Register 0, offset: 0x0 */
  __IO uint32_t SR;      /**< GTMR Status  Register,Register 1, offset: 0x4 */
  __IO uint32_t SCNT;    /**< GTMR SCNT    Register,Register 2, offset: 0x8 */
  __IO uint32_t NCNT;    /**< GTMR NCNT    Register,Register 3, offset: 0xC */
  __IO uint32_t TCNT;    /**< GTMR TCNT    Register,Register 4, offset: 0x10 */
} gtmr_type_t, *gtmr_mem_map_ptr_t;


/* ----------------------------------------------------------------------------
   -- GTMR Register Masks
   ---------------------------------------------------------------------------- */
/**
 * @addtogroup GTMR_Register_Masks GTMR Register Masks
 * @{
 */

/** CTRL Bit Fields */
#define GTMR_CTRL_INEN_MASK                      0x1U
#define GTMR_CTRL_INEN_SHIFT                     0U
#define GTMR_CTRL_INEN_WIDTH                     1U
#define GTMR_CTRL_INEN(x)                        (((uint32_t)(((uint32_t)(x))<<GTMR_CTRL_INEN_SHIFT)) & GTMR_CTRL_INEN_MASK)

#define GTMR_CTRL_EN_MASK                        0x2U
#define GTMR_CTRL_EN_SHIFT                       1U
#define GTMR_CTRL_EN_WIDTH                       1U
#define GTMR_CTRL_EN(x)                          (((uint32_t)(((uint32_t)(x))<<GTMR_CTRL_EN_SHIFT)) & GTMR_CTRL_EN_MASK)

#define GTMR_CTRL_TRIGS_MASK                     0x4U
#define GTMR_CTRL_TRIGS_SHIFT                    2U
#define GTMR_CTRL_TRIGS_WIDTH                    1U
#define GTMR_CTRL_TRIGS(x)                       (((uint32_t)(((uint32_t)(x))<<GTMR_CTRL_TRIGS_SHIFT)) & GTMR_CTRL_TRIGS_MASK)

#define GTMR_CTRL_STOP_MASK                      0x100U
#define GTMR_CTRL_STOP_SHIFT                     8U
#define GTMR_CTRL_STOP_WIDTH                     1U
#define GTMR_CTRL_STOP(x)                        (((uint32_t)(((uint32_t)(x))<<GTMR_CTRL_STOP_SHIFT)) & GTMR_CTRL_STOP_MASK)

#define GTMR_CTRL_STRIG_MASK                     0x10000U
#define GTMR_CTRL_STRIG_SHIFT                    16U
#define GTMR_CTRL_STRIG_WIDTH                    1U
#define GTMR_CTRL_STRIG(x)                       (((uint32_t)(((uint32_t)(x))<<GTMR_CTRL_STRIG_SHIFT)) & GTMR_CTRL_STRIG_MASK)

#define GTMR_CTRL_TRIGR_MASK                     0x20000000U
#define GTMR_CTRL_TRIGR_SHIFT                    29U
#define GTMR_CTRL_TRIGR_WIDTH                    1U
#define GTMR_CTRL_TRIGR(x)                       (((uint32_t)(((uint32_t)(x))<<GTMR_CTRL_TRIGR_SHIFT)) & GTMR_CTRL_TRIGR_MASK)

#define GTMR_CTRL_TRIGM_MASK                     0x40000000U
#define GTMR_CTRL_TRIGM_SHIFT                    30U
#define GTMR_CTRL_TRIGM_WIDTH                    1U
#define GTMR_CTRL_TRIGM(x)                       (((uint32_t)(((uint32_t)(x))<<GTMR_CTRL_TRIGM_SHIFT)) & GTMR_CTRL_TRIGM_MASK)

#define GTMR_CTRL_TRIGF_MASK                     0x80000000U
#define GTMR_CTRL_TRIGF_SHIFT                    31U
#define GTMR_CTRL_TRIGF_WIDTH                    1U
#define GTMR_CTRL_TRIGF(x)                       (((uint32_t)(((uint32_t)(x))<<GTMR_CTRL_TRIGF_SHIFT)) & GTMR_CTRL_TRIGF_MASK)

/** GSR Bit Fields */
#define GTMR_GSR_CRST_MASK                       0x1U
#define GTMR_GSR_CRST_SHIFT                    	 0U
#define GTMR_GSR_CRST_WIDTH                    	 1U
#define GTMR_GSR_CRST(x)                       	 (((uint32_t)(((uint32_t)(x))<<GTMR_GSR_CRST_SHIFT)) & GTMR_GSR_CRST_MASK)

#define GTMR_GSR_FULL_MASK                       0x8000000U
#define GTMR_GSR_FULL_SHIFT                    	 31U
#define GTMR_GSR_FULL_WIDTH                    	 1U
#define GTMR_GSR_FULL(x)                       	 (((uint32_t)(((uint32_t)(x))<<GTMR_GSR_FULL_SHIFT)) & GTMR_GSR_FULL_MASK)

/** SCNT Bit Fields */
#define GTMR_SCNT_MASK                         	 0xFFFFFFFFU
#define GTMR_SCNT_SHIFT                    	 	 0U
#define GTMR_SCNT_WIDTH                    		 32U
#define GTMR_SCNT(x)                       	    (((uint32_t)(((uint32_t)(x))<<GTMR_SCNT_SHIFT)) & GTMR_SCNT_MASK)

/** NCNT Bit Fields */
#define GTMR_NCNT_MASK                         	 0xFFFFFFFFU
#define GTMR_NCNT_SHIFT                    	 	 0U
#define GTMR_NCNT_WIDTH                    		 32U
#define GTMR_NCNT(x)                       	    (((uint32_t)(((uint32_t)(x))<<GTMR_NCNT_SHIFT)) & GTMR_NCNT_MASK)

/** TCNT Bit Fields */
#define GTMR_TCNT_MASK                         	 0xFFFFFFFFU
#define GTMR_TCNT_SHIFT                    	 	 0U
#define GTMR_TCNT_WIDTH                    		 32U
#define GTMR_TCNT(x)                       	    (((uint32_t)(((uint32_t)(x))<<GTMR_TCNT_SHIFT)) & GTMR_TCNT_MASK)

/**
 * @}
 */ /* end of group GTMR_Register_Masks */

/**
 * @}
 */ /* end of group GTMR_Peripheral_Access_Layer */

// clang-format on

#endif // __GTMR_REGISTER_H__
