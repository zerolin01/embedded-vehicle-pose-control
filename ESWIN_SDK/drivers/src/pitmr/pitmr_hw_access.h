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
 * @file pitmr_hw_access.h
 * @brief PITMR driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PITMR_HW_ACCESS_H__
#define __PITMR_HW_ACCESS_H__

#include "pitmr_driver.h"
#include "pitmr_register.h"
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Enables the PITMR module.
 *
 * This function enables the functional clock of PITMR module (Note: this function
 * does not un-gate the system clock gating control). It should be called before
 * setup any timer channel.
 *
 * @param  base   PITMR peripheral base address
 * @param  delay  delay value
 */
static inline void PITMR_Enable(pitmr_type_t *const base, volatile uint32_t delay)
{
    base->MCR |= PITMR_MCR_M_CEN_MASK;
    /* Run this counter down to zero
        If the delay is 0, the four clock delay between setting and clearing
        the SW_RST bit is ensured by the read-modify-write operation.
    */
    while (delay != 0U) {
        /* Since we need a four cycle delay, we assume the decrement is one cycle
            and insert three NOP instructions. The actual delay will be larger because
            of the loop overhead and the compiler optimization.
        */
        delay--;
        OS_NOP();
        OS_NOP();
        OS_NOP();
    }
}

/**
 * @brief  Disables the PITMR module.
 *
 * This function disables functional clock of PITMR module (Note: it does not
 * affect the system clock gating control).
 *
 * @param  base PITMR peripheral base address
 */
static inline void PITMR_Disable(pitmr_type_t *const base)
{
    base->MCR &= ~PITMR_MCR_M_CEN_MASK;
}

/**
 * @brief  Resets the PITMR module.
 *
 * This function sets all PITMR registers to reset value,
 * except the Module Control Register.
 *
 * @param  base  PITMR peripheral base address
 * @param  delay delay value
 */
static inline void PITMR_Reset(pitmr_type_t *const base, volatile uint32_t delay)
{
    base->MCR |= PITMR_MCR_SW_RST_MASK;
    /* Run this counter down to zero
        If the delay is 0, the four clock delay between setting and clearing
        the SW_RST bit is ensured by the read-modify-write operation.
    */
    while (delay != 0U) {
        /* Since we need a four cycle delay, we assume the decrement is one cycle
            and insert three NOP instructions. The actual delay will be larger because
            of the loop overhead and the compiler optimization.
        */
        delay--;
        OS_NOP();
        OS_NOP();
        OS_NOP();
    }
    base->MCR &= ~PITMR_MCR_SW_RST_MASK;
}

/**
 * @brief  Starts the timer channel counting.
 *
 * This function allows starting timer channels simultaneously .
 * After calling this function, timer channels are going operate depend on mode and
 * control bits which controls timer channel start, reload and restart.
 *
 * @param  base PITMR peripheral base address
 * @param  mask  Timer channels starting mask that decides which channels will be started
 *        - For example:
 *        - with mask = 0x01U then channel 0 will be started
 *        - with mask = 0x02U then channel 1 will be started
 *        - with mask = 0x03U then channel 0 and channel 1 will be started
 */
static inline void PITMR_StartTimerChannels(pitmr_type_t *const base, uint32_t mask)
{
    base->SETTEN |= mask;
}

/**
 * @brief  Stops the timer channel from counting.
 *
 * This function allows stop timer channels simultaneously from counting.
 * Timer channels reload their periods respectively after the next time
 * they call the PITMR_DRV_StartTimerChannels. Note that: In 32-bit Trigger Accumulator
 * mode, the counter will load on the first trigger rising edge.
 *
 * @param  base PITMR peripheral base address
 * @param  mask Timer channels stopping mask that decides which channels will be stopped
 *  - For example:
 *      - with mask = 0x01U then channel 0 will be stopped
 *      - with mask = 0x02U then channel 1 will be stopped
 *      - with mask = 0x03U then channel 0 and channel 1 will be stopped
 *
 */
static inline void PITMR_StopTimerChannels(pitmr_type_t *const base, uint32_t mask)
{
    base->CLRTEN |= mask;
}

/**
 * @brief Sets the timer channel period in count unit.
 *
 * This function sets the timer channel period in count unit.
 * The period range depends on the frequency of the PITMR functional clock and
 * operation mode of timer channel.
 * If the required period is out of range, use the suitable mode if applicable.
 * Timer channel begins counting from the value that is set by this function.
 * The counter period of a running timer channel can be modified by first setting
 * a new load value, the value will be loaded after the timer channel expires.
 * To abort the current cycle and start a timer channel period with the new value,
 * the timer channel must be disabled and enabled again.
 *
 * @param  base    PITMR peripheral base address
 * @param  channel Timer channel number
 * @param  count    Timer channel period in count unit
 */
static inline void PITMR_SetTimerPeriodByCount(pitmr_type_t *const base, uint32_t channel, uint32_t count)
{
    base->TMR[channel].TVAL = count;
}

/**
 * @brief  Gets the timer channel period in count unit.
 *
 * This function returns current period of timer channel given as argument.
 *
 * @param  base     PITMR peripheral base address
 * @param  channel  Timer channel number
 * @return uint32_t Timer channel period in count unit
 */
static inline uint32_t PITMR_GetTimerPeriodByCount(const pitmr_type_t *base, uint32_t channel)
{
    return (base->TMR[channel].TVAL);
}

/**
 * @brief  Gets the current timer channel counting value.
 *
 * This function returns the real-time timer channel counting value, the value in
 * a range from 0 to timer channel period.
 * Need to make sure the running time does not exceed the timer channel period.
 *
 * @param  base     PITMR peripheral base address
 * @param  channel  Timer channel number
 * @return uint32_t Current timer channel counting value
 */
static inline uint32_t PITMR_GetCurrentTimerCount(const pitmr_type_t *base, uint32_t channel)
{
    return (base->TMR[channel].CVAL);
}

/**
 * @brief  Enables the interrupt generation for timer channels.
 *
 * This function allows enabling interrupt generation for timer channels simultaneously.
 *
 * @param  base  PITMR peripheral base address
 * @param  mask  The interrupt enabling mask that decides which channels will
 * be enabled interrupt.
 *  - For example:
 *      - with mask = 0x01U then will enable interrupt for channel 0 only
 *      - with mask = 0x02U then will enable interrupt for channel 1 only
 *      - with mask = 0x03U then will enable interrupt for channel 0 and channel 1
 *
 */
static inline void PITMR_EnableInterruptTimerChannels(pitmr_type_t *const base, uint32_t mask)
{
    base->MIER |= mask;
}

/**
 * @brief  Disables the interrupt generation for timer channels.
 *
 * This function allows disabling interrupt generation for timer channels simultaneously.
 *
 * @param  base PITMR peripheral base address
 * @param  mask The interrupt disabling mask that decides which channels will
 * be disabled interrupt.
 * * - For example:
 *      - with mask = 0x01U then will disable interrupt for channel 0 only
 *      - with mask = 0x02U then will disable interrupt for channel 1 only
 *      - with mask = 0x03U then will disable interrupt for channel 0 and channel 1
 */
static inline void PITMR_DisableInterruptTimerChannels(pitmr_type_t *const base, uint32_t mask)
{
    base->MIER &= ~mask;
}

/**
 * @brief  Gets the interrupt flag of timer channels.
 *
 * This function gets current interrupt flag of timer channels.
 *
 * @param  base  PITMR peripheral base address
 * @param  mask  The interrupt flag getting mask that decides which channels will
 * be got interrupt flag.
 *     * - For example:
 *      - with mask = 0x01U then the interrupt flag of channel 0 only will be got
 *      - with mask = 0x02U then the interrupt flag of channel 1 only will be got
 *      - with mask = 0x03U then the interrupt flags of channel 0 and channel 1 will be got
 *
 * @return uint32_t The interrupt flag of timer channels.
 */
static inline uint32_t PITMR_GetInterruptFlagTimerChannels(const pitmr_type_t *base, uint32_t mask)
{
    return (base->MSR) & mask;
}

/**
 * @brief  Clears the interrupt flag of timer channels.
 *
 * @param  base PITMR peripheral base address
 * @param  mask  The interrupt flag clearing mask that decides which channels will
 * be cleared interrupt flag.
 *      - For example:
 *      - with mask = 0x01U then the interrupt flag of channel 0 only will be cleared
 *      - with mask = 0x02U then the interrupt flag of channel 1 only will be cleared
 *      - with mask = 0x03U then the interrupt flags of channel 0 and channel 1 will be cleared
 *
 */
static inline void PITMR_ClearInterruptFlagTimerChannels(pitmr_type_t *const base, uint32_t mask)
{
    /* Write 1 to clear the interrupt flag. */
    base->MSR = mask;
}

/**
 * @brief  Sets operation mode of timer channel
 *
 * This function sets the timer channel operation mode which control how
 * the timer channel decrements.
 *
 * @param  base PITMR peripheral base address
 * @param  channel Timer channel number
 * @param  mode  Operation mode of timer channel that is member of pitmr_timer_modes_t
 */
static inline void PITMR_SetTimerChannelModeCmd(pitmr_type_t *const base, uint32_t channel, pitmr_timer_modes_t mode)
{
    base->TMR[channel].TCTRL &= ~PITMR_TMR_TCTRL_MODE_MASK;
    base->TMR[channel].TCTRL |= PITMR_TMR_TCTRL_MODE(mode);
}

/**
 * @brief  Gets current operation mode of timer channel.
 *
 * This function gets current operation mode of the timer channel given as argument.
 *
 * @param  base PITMR peripheral base address
 * @param  channel Timer channel number
 * @return pitmr_timer_modes_t
 *         Operation mode of timer channel that is one of pitmr_timer_modes_t
 */
static inline pitmr_timer_modes_t PITMR_GetTimerChannelModeCmd(const pitmr_type_t *base, uint32_t channel)
{
    uint32_t tmp;
    pitmr_timer_modes_t mode;

    tmp = (((base->TMR[channel].TCTRL) & PITMR_TMR_TCTRL_MODE_MASK) >> PITMR_TMR_TCTRL_MODE_SHIFT);
    switch (tmp) {
    case 0x00U:
        mode = PITMR_PERIODIC_COUNTER;
        break;
    case 0x01U:
        mode = PITMR_DUAL_PERIODIC_COUNTER;
        break;
    case 0x02U:
        mode = PITMR_TRIGGER_ACCUMULATOR;
        break;
    case 0x03U:
        mode = PITMR_INPUT_CAPTURE;
        break;
    default:
        mode = PITMR_PERIODIC_COUNTER;
        break;
    }
    return mode;
}

/**
 * @brief  Sets internal trigger source for timer channel
 *
 * This function selects one trigger from the set of internal triggers that is
 * generated by other timer channels.
 * The selected trigger is used for starting and/or reloading the timer channel.
 *
 * @param  base PITMR peripheral base address
 * @param  channel Timer channel number
 * @param  triggerChannelSelect Number of the channel which is selected to be trigger source
 */
static inline void PITMR_SetTriggerSelectCmd(pitmr_type_t *const base, uint32_t channel, uint32_t triggerChannelSelect)
{
    base->TMR[channel].TCTRL &= ~PITMR_TMR_TCTRL_TRG_SEL_MASK;
    base->TMR[channel].TCTRL |= PITMR_TMR_TCTRL_TRG_SEL(triggerChannelSelect);
}

/**
 * @brief  Sets trigger source of timer channel.
 *
 * This function sets trigger source of the timer channel to be internal or external trigger.
 *
 * @param  base   PITMR peripheral base address
 * @param  channel   Timer channel number
 * @param  triggerSource  Trigger source of timer channel(internal or external source)
 */
static inline void PITMR_SetTriggerSourceCmd(pitmr_type_t *const base, uint32_t channel,
                                             pitmr_trigger_source_t triggerSource)
{
    base->TMR[channel].TCTRL &= ~PITMR_TMR_TCTRL_TRG_SRC_MASK;
    base->TMR[channel].TCTRL |= PITMR_TMR_TCTRL_TRG_SRC(triggerSource);
}

/**
 * @brief  Sets timer channel reload on trigger.
 *
 * This function sets the timer channel to reload/don't reload on trigger.
 *
 * @param  base PITMR peripheral base address
 * @param  channel Timer channel number
 * @param  isReloadOnTrigger Timer channel reload on trigger
 */
static inline void PITMR_SetReloadOnTriggerCmd(pitmr_type_t *const base, uint32_t channel, bool isReloadOnTrigger)
{
    base->TMR[channel].TCTRL &= ~PITMR_TMR_TCTRL_TROT_MASK;
    base->TMR[channel].TCTRL |= PITMR_TMR_TCTRL_TROT(isReloadOnTrigger ? 1UL : 0UL);
}

/**
 * @brief  Sets timer channel stop on interrupt.
 *
 * This function sets the timer channel to stop or don't stop after it times out
 *
 * @param  base PITMR peripheral base address
 * @param  channel Timer channel number
 * @param  isStopOnInterrupt Timer channel stop on interrupt
 */
static inline void PITMR_SetStopOnInterruptCmd(pitmr_type_t *const base, uint32_t channel, bool isStopOnInterrupt)
{
    base->TMR[channel].TCTRL &= ~PITMR_TMR_TCTRL_TSOI_MASK;
    base->TMR[channel].TCTRL |= PITMR_TMR_TCTRL_TSOI(isStopOnInterrupt ? 1UL : 0UL);
}

/**
 * @brief   Sets timer channel start on trigger.
 *
 * This function sets the timer channel to starts/don't start on trigger.
 *
 * @param  base   PITMR peripheral base address
 * @param  channel  Timer channel number
 * @param  isStartOnTrigger Timer channel start on trigger
 */
static inline void PITMR_SetStartOnTriggerCmd(pitmr_type_t *const base, uint32_t channel, bool isStartOnTrigger)
{
    base->TMR[channel].TCTRL &= ~PITMR_TMR_TCTRL_TSOT_MASK;
    base->TMR[channel].TCTRL |= PITMR_TMR_TCTRL_TSOT(isStartOnTrigger ? 1UL : 0UL);
}

/**
 * @brief  Sets timer channel chaining.
 *
 * This function sets the timer channel to be chained or not chained.
 *
 * @param  base PITMR peripheral base address
 * @param  channel  Timer channel number(Note: The timer channel 0 cannot be chained)
 * @param  isChannelChained Timer channel chaining
 */
static inline void PITMR_SetTimerChannelChainCmd(pitmr_type_t *const base, uint32_t channel, bool isChannelChained)
{
    base->TMR[channel].TCTRL &= ~PITMR_TMR_TCTRL_CHAIN_MASK;
    base->TMR[channel].TCTRL |= PITMR_TMR_TCTRL_CHAIN(isChannelChained ? 1UL : 0UL);
}

/**
 * @brief  Sets operation of PITMR in debug mode.
 *
 * When the device enters debug mode, the timer channels may or may not be frozen,
 * based on the configuration of this function. This is intended to aid software development,
 * allowing the developer to halt the processor, investigate the current state of
 * the system (for example, the timer channel values), and continue the operation.
 *
 * @param  base  PITMR peripheral base address
 * @param  isRunInDebug PITMR run in debug mode
 */
static inline void PITMR_SetTimerRunInDebugCmd(pitmr_type_t *const base, bool isRunInDebug)
{
    base->MCR &= ~PITMR_MCR_DBG_EN_MASK;
    base->MCR |= PITMR_MCR_DBG_EN(isRunInDebug ? 1UL : 0UL);
}

/**
 * @brief Sets operation of PITMR in DOZE mode.
 *
 * When the device enters debug mode, the timer channels may or may not be frozen,
 * based on the configuration of this function. The PITMR must use an external or
 * internal clock source which remains operating during DOZE modes(low power mode).
 *
 * @param  base PITMR peripheral base address
 * @param  isRunInDoze PITMR run in DOZE mode
 */
static inline void PITMR_SetTimerRunInDozeCmd(pitmr_type_t *const base, bool isRunInDoze)
{
    base->MCR &= ~PITMR_MCR_DOZE_EN_MASK;
    base->MCR |= PITMR_MCR_DOZE_EN(isRunInDoze ? 1UL : 0UL);
}

/**
 * @brief Checkout whether PITMR instance or PITMR mask is correct. 
 * 
 * PITMR instance(0 ~ 1), PITMR0 mask (0x0 ~ 0xF), PITMR1 mask (0x0 ~ 0x3FF)
 * -with mask = 0x01u means channel 0 
 * -with mask = 0x02u means channel 1 
 * -with mask = 0x03u means  channel 0 and channel 1 
 *
 * @param  instance  PITMR instance number.       
 * @param  mask      PITMR instance channel mask.
 * @return ** status_t 
 *          STATUS_SUCCESS if PITMR instance and PITMR mask  correct.
 *          STATUS_ERRORPAR if PITMR instance or PITMR mask number error.
 */
static inline status_t PITMR_CheckMask(uint32_t instance, uint32_t mask)
{

    status_t reStatus = STATUS_SUCCESS;

    switch (instance) {
    case 0:
        mask > PITMR0_MAX_MASK ? reStatus = STATUS_ERRORPAR : reStatus;
        break;
    case 1:
        mask > PITMR1_MAX_MASK ? reStatus = STATUS_ERRORPAR : reStatus;
        break;
    default:
        reStatus = STATUS_ERRORPAR;
        break;
    }
    return reStatus;
}

/**
 * @brief Checkout whether PITMR instance or PITMR channel is correct. 
 * 
 * PITMR instance(0 ~ 1), PITMR0 channel (0 ~3), PITMR1 channel (0 ~ 9)
 * 
 * @param  instance  PITMR instance number.       
 * @param  chnNum    PITMR instance channel number.
 * @return ** status_t 
 *          STATUS_SUCCESS if PITMR instance and PITMR channel number correct.
 *          STATUS_ERRORPAR if PITMR instance or PITMR channel number error.
 */
static inline status_t PITMR_CheckChannel(uint32_t instance, uint32_t chnNum)
{

    status_t reStatus = STATUS_SUCCESS;

    switch (instance) {
    case 0:
        chnNum > 4 ? reStatus = STATUS_ERRORPAR : reStatus;
        break;
    case 1:
        chnNum > 9 ? reStatus = STATUS_ERRORPAR : reStatus;
        break;
    default:
        reStatus = STATUS_ERRORPAR;
        break;
    }
    return reStatus;
}

/**
 * @brief Checkout whether PITMR instance or PITMR channel is correct and userChannelConfig is correct. 
 * 
 * PITMR instance(0 ~ 1), PITMR0 channel (0 ~3), PITMR1 channel (0 ~ 9)
 * 
 * @param  instance  PITMR instance number.       
 * @param  chnNum    PITMR instance channel number.
 * @return ** status_t 
 *          STATUS_SUCCESS if PITMR instance and PITMR channel number correct.
 *          STATUS_ERRORPAR if PITMR instance or PITMR channel number error.
 */

static inline status_t PITMR_CheckPara(uint32_t instance, uint32_t channel, const pitmr_user_channel_config_t *userChannelConfig)
{
    status_t reStatus = STATUS_SUCCESS;

    /* Checkout instance and channel range */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_SUCCESS != reStatus) {
        return reStatus;
    }

    /* checkout userChannelConfig is not NULL */
    if (NULL == userChannelConfig) {
        reStatus = STATUS_ERRORPAR;
        return reStatus;
    }

    /*channel 0 can not use chainChannel */
    if ((0U == channel) && (true == userChannelConfig->chainChannel)) {
        reStatus = STATUS_ERRORPAR;
    }

    return reStatus;
}

#if defined(__cplusplus)
}
#endif

#endif
