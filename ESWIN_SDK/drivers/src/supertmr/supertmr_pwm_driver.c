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
 * @file supertmr_pwm_driver.c
 * @brief SUPERTMR pwm driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "supertmr_pwm_driver.h"
#include "supertmr_hw_access.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_InitPwmIndependentChannel
 * Description   : Configures the PWM signal for the independent channel.
 *
 *END**************************************************************************/
static void SUPERTMR_DRV_InitPwmIndependentChannel(uint32_t instance, const supertmr_pwm_param_t *param)
{
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t index                 = 0U;
    uint8_t channelId             = 0U;
    uint8_t chnlPairNum           = 0U;

    /* Configure independent PWM channels */
    for (index = 0U; index < param->nNumIndependentPwmChannels; index++) {
        channelId   = param->pwmIndependentChannelConfig[index].hwChannelId;
        chnlPairNum = (uint8_t)(channelId >> 1U);
        /* Configure POL bits for fail safe state */
        SUPERTMR_DRV_SetChnOutputPolarityCmd(supertmrBase, channelId,
                                             (bool)param->pwmIndependentChannelConfig[index].safeState);

        /* Configure polarity of the PWM signal taking into consideration POL and ELSA/ELSB */
        if ((uint32_t)(param->pwmIndependentChannelConfig[index].safeState) ==
            (uint32_t)(param->pwmIndependentChannelConfig[index].polarity)) {
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channelId, (uint8_t)1U);
        } else {
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channelId, (uint8_t)2U);
        }

        if (param->pwmIndependentChannelConfig[index].enableSecondChannelOutput) {
            /* Configure dead time, and enable complementary channel. */
            SUPERTMR_DRV_SetDualChnDeadtimeCmd(supertmrBase, chnlPairNum,
                                               param->pwmIndependentChannelConfig[index].deadTime);
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channelId + 1U, (uint8_t)2U);
            SUPERTMR_DRV_SetDualChnCompCmd(supertmrBase, chnlPairNum, true);
            if (SUPERTMR_MAIN_INVERTED == param->pwmIndependentChannelConfig[index].secondChannelPolarity) {
                SUPERTMR_DRV_SetChnOutputPolarityCmd(supertmrBase, channelId + 1U,
                                                     (bool)param->pwmIndependentChannelConfig[index].safeState);
            } else {
                SUPERTMR_DRV_SetChnOutputPolarityCmd(supertmrBase, channelId + 1U,
                                                     !((bool)param->pwmIndependentChannelConfig[index].safeState));
            }
        } else {
            SUPERTMR_DRV_SetDualChnCompCmd(supertmrBase, chnlPairNum, false);
        }

        /* Disable combined mode. */
        SUPERTMR_DRV_SetDualEdgeCaptureCmd(supertmrBase, chnlPairNum, false);
        SUPERTMR_DRV_SetDualChnCombineCmd(supertmrBase, chnlPairNum, false);
        SUPERTMR_DRV_SetDualChnMofCombineCmd(supertmrBase, chnlPairNum, false);
        /* Set MSB and MSA bits*/
        SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, channelId, 3U);
        /* Configure fault mode */
        SUPERTMR_DRV_SetDualChnFaultCmd(supertmrBase, chnlPairNum,
                                        ((param->faultConfig)->faultMode != SUPERTMR_FAULT_CONTROL_DISABLED) ? true :
                                                                                                               false);
        /* Enable sync control for channels*/
        SUPERTMR_DRV_SetDualChnPwmSyncCmd(supertmrBase, chnlPairNum, true);
        /* Enable the generation a trigger on chip module */
        SUPERTMR_DRV_SetChnTriggerCmd(supertmrBase, channelId,
                                      param->pwmIndependentChannelConfig[index].enableExternalTrigger);

        /* Write SUPERTMRn_PWMLOAD register to enable synchronized loading points for the given channel */
        SUPERTMR_DRV_EnablePwmChannelOutputs(supertmrBase, channelId);
        if (param->pwmIndependentChannelConfig[index].enableSecondChannelOutput) {
            SUPERTMR_DRV_EnablePwmChannelOutputs(supertmrBase, channelId + 1U);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_InitPwmCombinedChannel
 * Description   : Configures the PWM signal for the combined channel.
 *
 *END**************************************************************************/
static void SUPERTMR_DRV_InitPwmCombinedChannel(uint32_t instance, const supertmr_pwm_param_t *param)
{
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t index                 = 0U;
    uint8_t channelId             = 0U;
    uint8_t chnlPairNum           = 0U;

    /* Configure combined PWM channels */
    for (index = 0U; index < param->nNumCombinedPwmChannels; index++) {
        channelId   = param->pwmCombinedChannelConfig[index].hwChannelId;
        chnlPairNum = (uint8_t)(channelId >> 1U);
        /* Check if the channel id is even number */
        OS_ASSERT(0U == (channelId % 2U));

        /* Configure POL bits for fail safe state */
        SUPERTMR_DRV_SetChnOutputPolarityCmd(supertmrBase, channelId,
                                             (bool)param->pwmCombinedChannelConfig[index].mainChannelSafeState);

        /* Configure polarity of the PWM signal taking into consideration POL and ELSA/ELSB */
        if ((uint32_t)(param->pwmCombinedChannelConfig[index].mainChannelSafeState) ==
            (uint32_t)(param->pwmCombinedChannelConfig[index].mainChannelPolarity)) {
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channelId, (uint8_t)1U);
        } else {
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channelId, (uint8_t)2U);
        }

        SUPERTMR_DRV_SetDualEdgeCaptureCmd(supertmrBase, chnlPairNum, false);
        /* Set MSB and MSA bits */
        SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, channelId, 3U);

        /* Enable channel (n) output */
        SUPERTMR_DRV_EnablePwmChannelOutputs(supertmrBase, channelId);
        /* Configure channel n+1 if it necessary. */
        if (param->pwmCombinedChannelConfig[index].enableSecondChannelOutput) {
            channelId = channelId + 1U;
            /* Configure POL bits for fail safe state */
            SUPERTMR_DRV_SetChnOutputPolarityCmd(supertmrBase, channelId,
                                                 (bool)param->pwmCombinedChannelConfig[index].secondChannelSafeState);
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channelId, (uint8_t)2U);
            /* Configure polarity of the second channel relative to main channel polarity. */
            if (param->pwmCombinedChannelConfig[index].secondChannelSafeState ==
                param->pwmCombinedChannelConfig[index].mainChannelSafeState) {
                if (param->pwmCombinedChannelConfig[index].secondChannelPolarity == SUPERTMR_MAIN_DUPLICATED) {
                    /* If dead time is enabled and COMPx = 0 the channel n+1 is automatically disabled. */
                    OS_ASSERT(!(param->pwmCombinedChannelConfig[index].deadTime));
                    SUPERTMR_DRV_SetDualChnCompCmd(supertmrBase, chnlPairNum, false);
                } else {
                    SUPERTMR_DRV_SetDualChnCompCmd(supertmrBase, chnlPairNum, true);
                }
            } else {
                if (param->pwmCombinedChannelConfig[index].secondChannelPolarity == SUPERTMR_MAIN_DUPLICATED) {
                    SUPERTMR_DRV_SetDualChnCompCmd(supertmrBase, chnlPairNum, true);
                } else {
                    /* If dead time is enabled and COMPx = 0 the channel n+1 is automatically disabled. */
                    OS_ASSERT(!(param->pwmCombinedChannelConfig[index].deadTime));
                    SUPERTMR_DRV_SetDualChnCompCmd(supertmrBase, chnlPairNum, false);
                }
            }
            /* Enable channel (n+1) output */
            SUPERTMR_DRV_EnablePwmChannelOutputs(supertmrBase, (uint8_t)(channelId));
        }

        /* Set fault control for the channel */
        SUPERTMR_DRV_SetDualChnFaultCmd(supertmrBase, chnlPairNum,
                                        ((param->faultConfig)->faultMode != SUPERTMR_FAULT_CONTROL_DISABLED) ? true :
                                                                                                               false);
        /* Enable sync control for channels */
        SUPERTMR_DRV_SetDualChnPwmSyncCmd(supertmrBase, chnlPairNum, true);
        /* Enable the combine mode */
        SUPERTMR_DRV_SetDualChnCombineCmd(supertmrBase, chnlPairNum, true);
        /* Configure the modified combine mode */
        SUPERTMR_DRV_SetDualChnMofCombineCmd(supertmrBase, chnlPairNum,
                                             param->pwmCombinedChannelConfig[index].enableModifiedCombine);
        /* Configure dead time */
        SUPERTMR_DRV_SetDualChnDeadtimeCmd(supertmrBase, chnlPairNum, param->pwmCombinedChannelConfig[index].deadTime);
        /* Enable the generation a trigger on the channel (n) */
        channelId = (uint8_t)(chnlPairNum << 1U);
        SUPERTMR_DRV_SetChnTriggerCmd(supertmrBase, channelId,
                                      param->pwmCombinedChannelConfig[index].enableExternalTrigger);
        /* Enable the generation a trigger on the channel (n+1) */
        channelId = channelId + 1U;
        SUPERTMR_DRV_SetChnTriggerCmd(supertmrBase, channelId,
                                      param->pwmCombinedChannelConfig[index].enableExternalTriggerOnNextChn);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_InitPwmDutyCycleChannel
 * Description   : This function will update the duty cycle for the PWM signal
 * at the initialization.
 *
 *END**************************************************************************/
static status_t SUPERTMR_DRV_InitPwmDutyCycleChannel(uint32_t instance, const supertmr_pwm_param_t *param)
{
    uint8_t index     = 0U;
    uint8_t hwChannel = 0U;
    status_t retVal   = STATUS_SUCCESS;

    for (index = 0U; index < param->nNumIndependentPwmChannels; index++) {
        hwChannel = param->pwmIndependentChannelConfig[index].hwChannelId;
        /* Write CnV registers and setup duty cycle and phase values */
        retVal = SUPERTMR_DRV_UpdatePwmChannel(instance, hwChannel, SUPERTMR_PWM_UPDATE_IN_DUTY_CYCLE,
                                               param->pwmIndependentChannelConfig[index].uDutyCyclePercent, 0U, false);
    }

    for (index = 0U; index < param->nNumCombinedPwmChannels; index++) {
        hwChannel = param->pwmCombinedChannelConfig[index].hwChannelId;
        /* Write CnV registers and setup duty cycle and phase values */
        retVal = SUPERTMR_DRV_UpdatePwmChannel(instance, hwChannel, SUPERTMR_PWM_UPDATE_IN_DUTY_CYCLE,
                                               param->pwmCombinedChannelConfig[index].firstEdge,
                                               param->pwmCombinedChannelConfig[index].secondEdge, false);
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_InitPwm
 * Description   : Configures duty cycle and frequency and starts outputting
 * PWM on specified channels.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_InitPwm(uint32_t instance, const supertmr_pwm_param_t *param)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(NULL != param);
    status_t retVal               = STATUS_SUCCESS;
    uint8_t fltChannel            = 0U;
    uint8_t faultChnNum           = 0U;
    uint32_t tempInst             = instance;
    supertmr_state_t *state       = supertmrStatePtr[instance];
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    if ((NULL != state) && (SUPERTMR_MODE_NOT_INITIALIZED == state->supertmrMode)) {
        /* Disable counter clock */
        SUPERTMR_DRV_SetClockSource(supertmrBase, SUPERTMR_CLOCK_SOURCE_NONE);
        /* Clear the overflow flag */
        SUPERTMR_DRV_ClearTimerOverflow(supertmrBase);
        /* Disable write protection */
        SUPERTMR_DRV_SetWriteProtectionCmd(supertmrBase, false);
        /* Configure SUPERTMR mode */
        state->supertmrMode = param->mode;
        /* Configure independent PWM channels */
        SUPERTMR_DRV_InitPwmIndependentChannel(instance, param);
        /* Configure combined PWM channels */
        SUPERTMR_DRV_InitPwmCombinedChannel(instance, param);
        /* Set enable outputs to be set to Initial/default value */
        SUPERTMR_DRV_SetInitChnOutputCmd(supertmrBase, true);
        /* Enable faults (if faults were configured) */
        if (SUPERTMR_FAULT_CONTROL_DISABLED != (param->faultConfig)->faultMode) {
            /* Configure PWM Output behavior */
            SUPERTMR_DRV_SetPwmFaultBehavior(supertmrBase,
                                             ((param->faultConfig)->pwmOutputStateOnFault) ? true : false);
            /* Configure fault filter value */
            SUPERTMR_DRV_SetFaultInputFilterVal(supertmrBase, ((param->faultConfig)->faultFilterValue));
            /* Check the SUPERTMR instances */
            if (tempInst <= 3U) {
                faultChnNum = (uint8_t)SUPERTMR_FAULT_CHANNELS;
            } else {
                faultChnNum = (uint8_t)(SUPERTMR_FAULT_CHANNELS >> 1U);
            }

            for (fltChannel = 0U; fltChannel < faultChnNum; fltChannel++) {
                if (true == (param->faultConfig)->supertmrFaultChannelParam[fltChannel].faultChannelEnabled) {
                    /* Enable fault channel */
                    SUPERTMR_DRV_SetFaultInputCmd(supertmrBase, fltChannel, true);
                    /* Configure fault filter */
                    SUPERTMR_DRV_SetFaultInputFilterCmd(
                        supertmrBase, fltChannel,
                        ((param->faultConfig)->supertmrFaultChannelParam[fltChannel].faultFilterEnabled) ? true :
                                                                                                           false);
                    /* Configure fault outputs */
                    SUPERTMR_DRV_SetChnFaultInputPolarityCmd(
                        supertmrBase, fltChannel,
                        (((param->faultConfig)->supertmrFaultChannelParam[fltChannel].supertmrFaultPinPolarity ==
                          SUPERTMR_POLARITY_HIGH) ?
                             true :
                             false));
                }
            }

            /* Set fault interrupt */
            if (true == ((param->faultConfig)->pwmFaultInterrupt)) {
                SUPERTMR_DRV_SetFaultInt(supertmrBase, true);
            }

            /* Enable fault control */
            SUPERTMR_DRV_SetFaultControlMode(supertmrBase, (uint32_t)(param->faultConfig)->faultMode);
        }

        /* Configure PWM mode: edge or center aligned */
        SUPERTMR_DRV_SetCpwms(supertmrBase, (SUPERTMR_MODE_CEN_ALIGNED_PWM == param->mode) ? true : false);
        /* Calculate frequency of the give SUPERTMR hardware module - all channels will run at the same frequency */
        state->supertmrPeriod = SUPERTMR_DRV_ConvertFreqToPeriodTicks(instance, param->uFrequencyHZ);
        /* Based on reference manual, in PWM mode CNTIN is to be set 0*/
        SUPERTMR_DRV_SetCounterInitVal(supertmrBase, 0U);
        /* Write MOD register with the value of the period */
        /* For center aligned mode MOD register should be divided by 2 */
        /* For edge aligned mode period is determined by: MOD-CNTIN+1 */
        if (SUPERTMR_MODE_CEN_ALIGNED_PWM == param->mode) {
            SUPERTMR_DRV_SetMod(supertmrBase, (uint16_t)(state->supertmrPeriod >> 1U));
        } else {
            SUPERTMR_DRV_SetMod(supertmrBase, (uint16_t)(state->supertmrPeriod - 1U));
        }

        /* Update the duty cycle */
        retVal = SUPERTMR_DRV_InitPwmDutyCycleChannel(instance, param);

        if (STATUS_SUCCESS == retVal) {
            /* Configure dead time for combine mode */
            SUPERTMR_DRV_SetDeadtimeCount(supertmrBase, param->deadTimeValue);
            SUPERTMR_DRV_SetDeadtimePrescale(supertmrBase, param->deadTimePrescaler);
            SUPERTMR_DRV_Enable(supertmrBase, true);
            SUPERTMR_DRV_SetPwmSyncMode(supertmrBase, true);
            /* Set clock source to start counter */
            SUPERTMR_DRV_SetClockSource(supertmrBase, state->supertmrClockSource);
        } else {
            /* Restore SUPERTMR mode if initialization fails */
            state->supertmrMode = SUPERTMR_MODE_NOT_INITIALIZED;
        }
    } else {
        retVal = STATUS_ERROR;
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_DeinitPwm
 * Description   : Stops all PWM channels.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_DeinitPwm(uint32_t instance)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t channel;
    uint8_t chnlPairNum;
    supertmr_state_t *state = supertmrStatePtr[instance];
    OS_ASSERT(NULL != state);

    /* Stop the SUPERTMR counter */
    SUPERTMR_DRV_SetClockSource(supertmrBase, SUPERTMR_CLOCK_SOURCE_NONE);
    for (channel = 0U; channel < SUPERTMR_CHANNEL_COUNT; channel++) {
        chnlPairNum = (uint8_t)(channel >> 1U);
        /* Disable PWM mode in hardware */
        SUPERTMR_DRV_SetChnCountVal(supertmrBase, channel, 0U);
        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channel, 0U);
        SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, channel, 0U);
        SUPERTMR_DRV_SetCpwms(supertmrBase, false);
        /* Configure polarity bit */
        SUPERTMR_DRV_SetChnOutputPolarityCmd(supertmrBase, channel, false);
        SUPERTMR_DRV_DisablePwmChannelOutputs(supertmrBase, channel);
        /* Clear the PWM synchronization */
        SUPERTMR_DRV_SetDualChnPwmSyncCmd(supertmrBase, chnlPairNum, false);
        /* Clear combination for each pair of channels */
        SUPERTMR_DRV_SetDualChnMofCombineCmd(supertmrBase, chnlPairNum, false);
        SUPERTMR_DRV_SetDualChnCombineCmd(supertmrBase, chnlPairNum, false);
        SUPERTMR_DRV_SetDualChnDeadtimeCmd(supertmrBase, chnlPairNum, false);
        SUPERTMR_DRV_SetDualChnFaultCmd(supertmrBase, chnlPairNum, false);
    }

    /* Clear the dead-time pre-scaler and value */
    SUPERTMR_DRV_SetExtDeadtimeValue(supertmrBase, 0U);
    SUPERTMR_DRV_SetDeadtimePrescale(supertmrBase, SUPERTMR_DEADTIME_DIVID_BY_1);
    SUPERTMR_DRV_SetDeadtimeCount(supertmrBase, 0U);
    /* Clear fault control register */
    SUPERTMR_DRV_ClearFaultControl(supertmrBase);
    /* Disable fault interrupt */
    SUPERTMR_DRV_SetFaultInt(supertmrBase, false);
    /* Disable fault control */
    SUPERTMR_DRV_SetFaultControlMode(supertmrBase, (uint32_t)SUPERTMR_FAULT_CONTROL_DISABLED);
    /* Clear the module value of the registers */
    SUPERTMR_DRV_SetMod(supertmrBase, 0U);
    SUPERTMR_DRV_SetCounter(supertmrBase, 0U);
    state->supertmrMode = SUPERTMR_MODE_NOT_INITIALIZED;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_UpdatePwmChannel
 * Description   : This function will update the duty cycle of PWM output.
 * - If the type of update in the duty cycle, this function will convert the input parameters representing
 * frequency in Hz to a period value in ticks needed by the hardware timer. Period is calculated depending
 * on the operating mode of the SUPERTMR module and is stored in internal state structure.
 * firstEdge and secondEdge can have value between 0 - SUPERTMR_MAX_DUTY_CYCLE(0 = 0% duty
 * and SUPERTMR_MAX_DUTY_CYCLE = 100% duty). secondEdge value is used only whenSUPERTMR module is used in PWM combine
 *mode.
 * - If the type of update in ticks, this function will get value in ticks to the hardware timer.
 * firstEdge and secondEdge variables can have value between 0 and supertmrPeriod is stored in the state structure.
 * - in the modified combine mode, the firstEdge parameter is fixed value and only can modify the secondEdge variables
 * which is the initial value in the channel (n+1) match edge when the SUPERTMR counter has been ran.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_UpdatePwmChannel(uint32_t instance, uint8_t channel, supertmr_pwm_update_option_t typeOfUpdate,
                                       uint16_t firstEdge, uint16_t secondEdge, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    uint16_t hwFirstEdge          = 0U;
    uint16_t hwSecondEdge         = 0U;
    uint16_t supertmrPeriod       = 0U;
    uint8_t chnlPairNum           = (uint8_t)(channel >> 1U);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    supertmr_state_t *state       = supertmrStatePtr[instance];
    status_t retStatus            = STATUS_SUCCESS;

    /* Get the newest period in the MOD register */
    supertmrPeriod = SUPERTMR_DRV_GetMod(supertmrBase);
    /* Check the mode operation in SUPERTMR module */
    if (SUPERTMR_MODE_CEN_ALIGNED_PWM == state->supertmrMode) {
        supertmrPeriod = (uint16_t)(supertmrPeriod << 1U);
    } else if ((SUPERTMR_MODE_EDGE_ALIGNED_PWM == state->supertmrMode) ||
               (SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE == state->supertmrMode)) {
        supertmrPeriod = (uint16_t)(supertmrPeriod + 1U);
    } else {
        retStatus = STATUS_ERROR;
    }

    /* Check the type of update for PWM */
    if (SUPERTMR_PWM_UPDATE_IN_DUTY_CYCLE == typeOfUpdate) {
        if ((firstEdge <= SUPERTMR_MAX_DUTY_CYCLE) && (secondEdge <= SUPERTMR_MAX_DUTY_CYCLE)) {
            /* Calculate DutyCycle based of the previously calculated frequency*/
            /* For greater resolution the DutyCycle values are in the range [0. SUPERTMR_MAX_DUTY_CYCLE]
             *  where 0 = 0% or PWM always at Low and SUPERTMR_MAX_DUTY_CYCLE = 100% or PWM always HIGH;
             *  a value of 0x4000 is equivalent of 50% DutyCycle. */
            hwFirstEdge  = (uint16_t)((supertmrPeriod * firstEdge) >> SUPERTMR_DUTY_TO_TICKS_SHIFT);
            hwSecondEdge = (uint16_t)((supertmrPeriod * secondEdge) >> SUPERTMR_DUTY_TO_TICKS_SHIFT);
            /* adjust DutyCycle if 100% value is to be achieved. */
            if (SUPERTMR_MAX_DUTY_CYCLE == firstEdge) {
                /* If expected duty is 100% then increase by 1 the value that is to be written
                 *  to Hardware so it will exceed value of period */
                hwFirstEdge = (uint16_t)(hwFirstEdge + 1U);
            }
        } else {
            retStatus = STATUS_ERROR;
        }
    } else {
        if ((firstEdge <= supertmrPeriod) && (secondEdge <= supertmrPeriod)) {
            hwFirstEdge  = firstEdge;
            hwSecondEdge = secondEdge;
        } else {
            retStatus = STATUS_ERROR;
        }
    }

    if (STATUS_SUCCESS == retStatus) {
        if (true == SUPERTMR_DRV_GetDualChnCombineCmd(supertmrBase, chnlPairNum)) {
            if (true == SUPERTMR_DRV_GetDualChnMofCombineCmd(supertmrBase, chnlPairNum)) {
                /* Check the clock source for SUPERTMR counter is disabled or not */
                if (SUPERTMR_DRV_GetClockSource(supertmrBase) == 0U) {
                    SUPERTMR_DRV_SetChnCountVal(supertmrBase, (uint8_t)(chnlPairNum * 2U), hwFirstEdge);
                }
            } else {
                SUPERTMR_DRV_SetChnCountVal(supertmrBase, (uint8_t)(chnlPairNum * 2U), hwFirstEdge);
            }

            /* Modify the initial value in the channel (n+1) match edge */
            SUPERTMR_DRV_SetChnCountVal(supertmrBase, (uint8_t)((chnlPairNum * 2U) + 1U), hwSecondEdge);
        } else {
            /* Channel value is divided by 2 for up down counter mode to keep same duty */
            if (true == SUPERTMR_DRV_GetCpwms(supertmrBase)) {
                SUPERTMR_DRV_SetChnCountVal(supertmrBase, channel, (uint16_t)(hwFirstEdge >> 1U));
            } else {
                SUPERTMR_DRV_SetChnCountVal(supertmrBase, channel, hwFirstEdge);
            }
        }

        /* Software trigger is generated to change CnV registers */
        /* Before this please configure sync mechanism to use software trigger */
        SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

        /* Store the PWM period in the state structure */
        state->supertmrPeriod = supertmrPeriod;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_UpdatePwmPeriod
 * Description   : This function will update the new period in the frequency or
 * in the counter value into mode register which modify the period of PWM signal
 * on the channel output.
 * - If the type of update in the duty cycle which is reused in SUPERTMR_DRV_UpdatePwmChannel
 * function to convert the newValue parameters representing frequency in Hz to
 * a period value to update the MOD register. The newValue parameter must be value
 * between 1U and maximum is the frequency of the SUPERTMR counter.
 * - If the type of update in ticks, this function will get value in counting to
 * the MOD register. The newValue parameter must be value between 1U and 0xFFFFU
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_UpdatePwmPeriod(uint32_t instance, supertmr_pwm_update_option_t typeOfUpdate, uint32_t newValue,
                                      bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(newValue != 0U);
    uint32_t supertmrPeriod       = 0U;
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    supertmr_state_t *state       = supertmrStatePtr[instance];
    status_t retStatus            = STATUS_SUCCESS;

    /* Check the type of update for period in PWM mode */
    if (SUPERTMR_PWM_UPDATE_IN_TICKS == typeOfUpdate) {
        supertmrPeriod = newValue;
    } else {
        if (newValue <= state->supertmrSourceClockFrequency) {
            supertmrPeriod = (uint32_t)SUPERTMR_DRV_ConvertFreqToPeriodTicks(instance, newValue);
        } else {
            retStatus = STATUS_ERROR;
        }
    }

    if (STATUS_SUCCESS == retStatus) {
        /* Check the supertmrPeriod is invalid */
        OS_ASSERT(supertmrPeriod <= 0xFFFFU);
        /* Check the signal operation in which PWM mode */
        OS_ASSERT((SUPERTMR_MODE_CEN_ALIGNED_PWM == state->supertmrMode) ||
                  (SUPERTMR_MODE_EDGE_ALIGNED_PWM == state->supertmrMode) ||
                  (SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE == state->supertmrMode));
        if (SUPERTMR_MODE_CEN_ALIGNED_PWM == state->supertmrMode) {
            supertmrPeriod = (supertmrPeriod >> 1U);
        } else {
            OS_ASSERT(supertmrPeriod > 0U);
            supertmrPeriod = (supertmrPeriod - 1U);
        }
        /* Set the new modulo value into MOD register */
        SUPERTMR_DRV_SetMod(supertmrBase, (uint16_t)(supertmrPeriod));
        /* Software trigger is generated to change MOD registers */
        SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

        /* Store the PWM period in the state structure */
        state->supertmrPeriod = (uint16_t)supertmrPeriod;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_FastUpdatePwmChannels
 * Description   : This function will update the duty cycle of PWM output for multiple channels.
 *
 * The main differences between this function and SUPERTMR_DRV_UpdatePwmChannel is the execution speed. This
 * feature makes this function ideal for applications like motor controlling.
 * The downside is the low flexibility of the parameters (this function accept only updates in ticks).
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_FastUpdatePwmChannels(uint32_t instance, uint8_t numberOfChannels, const uint8_t *channels,
                                            const uint16_t *duty, bool softwareTrigger)
{
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(numberOfChannels <= SUPERTMR_CHANNEL_COUNT);
    uint8_t i;

    for (i = 0U; i < numberOfChannels; i++) {
        ((supertmrBase)->CONTROLS[channels[i]].CnV) = duty[i];
    }

    if (softwareTrigger) {
        supertmrBase->SYNC |= SUPERTMR_SYNC_SWSYNC_MASK;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_ControlChannelOutput
 * Description   : This function is used to control the final logic of the
 * channel output.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_ControlChannelOutput(uint32_t instance, uint8_t channel, bool enableChannelOutput)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    if (true == enableChannelOutput) {
        /* Enable the channel output */
        SUPERTMR_DRV_EnablePwmChannelOutputs(supertmrBase, channel);
    } else {
        /* Disable the channel output */
        SUPERTMR_DRV_DisablePwmChannelOutputs(supertmrBase, channel);
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_UpdatePwmPeriodDither
 * Description   : This function will use in the PWM period dithering. This value
 * is added to an internal accumulator at the end of each PWM period. The value is
 * updated with its write buffer value according to the register synchronization.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_UpdatePwmPeriodDither(uint32_t instance, uint8_t newModFracVal, bool softwareTrigger)
{
    OS_ASSERT((instance <= 5U));
    OS_ASSERT(newModFracVal <= 32U);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    SUPERTMR_DRV_SetModFracVal(supertmrBase, newModFracVal);
    /* Software trigger is generated to change MOD_MIRROR registers */
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_UpdatePwmEdgeChannelDither
 * Description   : This function will use in the PWM edge dithering. This value
 * is added to the channel (n) internal accumulator at the end of each PWM period.
 * The FRACVAL is updated with its write buffer value according to the register
 * synchronization. The PWM edge dithering is not available when the channel in the
 * input capture modes, and the channel in output compare mode.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_UpdatePwmEdgeChannelDither(uint32_t instance, uint8_t channel, uint8_t newMatchFracVal,
                                                 bool softwareTrigger)
{
    OS_ASSERT((instance <= 5U));
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    OS_ASSERT(newMatchFracVal <= 32U);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    SUPERTMR_DRV_SetChnMatchFracVal(supertmrBase, channel, newMatchFracVal);
    /* Software trigger is generated to change CnV_MIRROR registers */
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetPairDeadtimeValue
 * Description   : Set pair deadtime value
 *
 *END**************************************************************************/
void SUPERTMR_DRV_SetPairDeadtimeValue(uint32_t instance, uint8_t channelPair, uint8_t count)
{
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    SUPERTMR_DRV_SetPairDeadtimeCount(supertmrBase, channelPair, count);
    return;
}
