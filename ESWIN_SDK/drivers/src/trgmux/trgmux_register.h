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
 * @file trgmux_register.h
 * @brief TRGMUX driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __TRGMUX_REGISTER_H__
#define __TRGMUX_REGISTER_H__

// clang-format off
/* ----------------------------------------------------------------------------
   -- TRGMUX and TMRMUX Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup TRGMUX_and_TMRMUX_Peripheral_Access_Layer TRGMUX/TMRMUX Peripheral Access Layer
 * @{
 */

/** TRGMUX - Size of Registers Arrays */
#define TRGMUX_TRGMUX_N_COUNT                     (43U)

/**
 * @brief TRGMUX Register Layout Typedef
 */
typedef struct {
  __IO uint32_t trgmuxN[TRGMUX_TRGMUX_N_COUNT];     /**< TRGMUX DMAMUX0 Register..TRGMUX SUPERTMR5 Register, array offset: 0x0, array step: 0x4 */
} trgmux_type_t, *trgmux_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- TRGMUX and TMRMUX Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup TRGMUX_and_TMRMUX_Register_Masks TRGMUX/TMRMUX Register Masks
 * @{
 */

/** TRGMUX_N Bit Fields */
#define TRGMUX_TRGMUX_N_SEL0_MASK                 0x7FU
#define TRGMUX_TRGMUX_N_SEL0_SHIFT                0U
#define TRGMUX_TRGMUX_N_SEL0_WIDTH                7U
#define TRGMUX_TRGMUX_N_SEL0(x)                   (((uint32_t)(((uint32_t)(x))<<TRGMUX_TRGMUX_N_SEL0_SHIFT))&TRGMUX_TRGMUX_N_SEL0_MASK)
#define TRGMUX_TRGMUX_N_SEL1_MASK                 0x7F00U
#define TRGMUX_TRGMUX_N_SEL1_SHIFT                8U
#define TRGMUX_TRGMUX_N_SEL1_WIDTH                7U
#define TRGMUX_TRGMUX_N_SEL1(x)                   (((uint32_t)(((uint32_t)(x))<<TRGMUX_TRGMUX_N_SEL1_SHIFT))&TRGMUX_TRGMUX_N_SEL1_MASK)
#define TRGMUX_TRGMUX_N_SEL2_MASK                 0x7F0000U
#define TRGMUX_TRGMUX_N_SEL2_SHIFT                16U
#define TRGMUX_TRGMUX_N_SEL2_WIDTH                7U
#define TRGMUX_TRGMUX_N_SEL2(x)                   (((uint32_t)(((uint32_t)(x))<<TRGMUX_TRGMUX_N_SEL2_SHIFT))&TRGMUX_TRGMUX_N_SEL2_MASK)
#define TRGMUX_TRGMUX_N_SEL3_MASK                 0x7F000000U
#define TRGMUX_TRGMUX_N_SEL3_SHIFT                24U
#define TRGMUX_TRGMUX_N_SEL3_WIDTH                7U
#define TRGMUX_TRGMUX_N_SEL3(x)                   (((uint32_t)(((uint32_t)(x))<<TRGMUX_TRGMUX_N_SEL3_SHIFT))&TRGMUX_TRGMUX_N_SEL3_MASK)
#define TRGMUX_TRGMUX_N_LK_MASK                   0x80000000U
#define TRGMUX_TRGMUX_N_LK_SHIFT                  31U
#define TRGMUX_TRGMUX_N_LK_WIDTH                  1U
#define TRGMUX_TRGMUX_N_LK(x)                     (((uint32_t)(((uint32_t)(x))<<TRGMUX_TRGMUX_N_LK_SHIFT))&TRGMUX_TRGMUX_N_LK_MASK)

/**
 * @}
 */ /* end of group TRGMUX_and_TMRMUX_Register_Masks */

/**
 * @}
 */ /* end of group TRGMUX_and_TMRMUX_Peripheral_Access_Layer */
// clang-format on

#endif /* __TRGMUX_REGISTER_H__ */
