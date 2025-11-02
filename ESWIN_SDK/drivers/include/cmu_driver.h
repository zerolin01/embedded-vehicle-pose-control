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
 * @file cmu_driver.h
 * @brief 
 * @author abu (abu@eswincomputing.com)
 * @date 2023-05-23
 * 
 * Modification History :
 * Date:               Version:                    Author:     
 * Changes: 
 * 
 */

#ifndef __CMU_DRIVER_H__
#define __CMU_DRIVER_H__

#include "callbacks.h"
#include "osal.h"
#include "status.h"
#include <stdint.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup CMU_Driver CMU Driver
 * @{
 */

#define CMU_IT_STATUS_NORMAL   (1U)
#define CMU_IT_STATUS_ABNORMAL (2U)

/**
 * @brief Data structure containing information about CMU.
 *
 * The user must populate these members to set up the CMU.
 */
typedef struct {
    uint32_t th_hval;        /**< threshold high value of the CMU. \R{0x1,0xffffffff} \D{0x2000} */
    uint32_t th_lval;        /**< threshold low value of the CMU. \R{0x1,0xffffffff} \D{0x100} */
    bool int_normal_en;      /**< normal Interrupt whether is enable. \D{true} */
    bool int_abnormal_en;    /**< abnormal Interrupt whether is enable. \D{true} */
    cmu_callback_t callback; /**< The callback of interrupt. \D{NULL} */
    void *callbackParam;     /**< Callback parameters. \D{NULL} */
} cmu_config_t;

/**
 * @brief CMU callback state type.
 */
typedef struct {
    void *callbackParam;     /**< Callback parameters. */
    cmu_callback_t callback; /**< The callback of interrupt. */
} cmu_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initializes the CMU instance.
 * 
 * @param  instance A CMU instance number.
 * @param  cmuState Pointer to the CMU driver state structure.
 * @param  cmuConfig The CMU platform config data.
 * @return status_t 
 * STATUS_SUCCESS: Successful.
 */
status_t CMU_DRV_Init(uint32_t instance, cmu_state_t *cmuState, const cmu_config_t *cmuConfig);

/**
 * @brief De-initializes the CMU instance.
 * 
 * @param  instance A CMU instance number.
 * @return status_t 
 * STATUS_SUCCESS: Successful.
 */
status_t CMU_DRV_Deinit(uint32_t instance);

/**
 * @brief Check whether the measure activity has done.
 * 
 * @param  instance A CMU instance number.
 * @return true 
 * @return false 
 */
bool CMU_DRV_Check_MeasDone(uint32_t instance);

/**
 * @brief Get the measure status after its done
 * 
 * @param  instance A CMU instance number.
 * @return uint8_t CMU_IT_STATUS_NORMAL or CMU_IT_STATUS_ABNORMAL
 */
uint8_t CMU_DRV_Get_MeaStatus(uint32_t instance);

/**
 * @brief Get the specified CMU's monitor if it is done.
 * 
 * @param  instance A CMU instance number.
 * @return true 
 * @return false 
 */
bool CMU_DRV_Get_MeasDone(uint32_t instance);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group CMU_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __CMU_DRIVER_H__ */