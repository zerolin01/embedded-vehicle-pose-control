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
 * @file supertmr_ic_driver.c
 * @brief SUPERTMR input capture driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "supertmr_ic_driver.h"
#include "supertmr_hw_access.h"

static void SUPERTMR_DRV_InputCaptureHandler(uint32_t instance, uint8_t channelPair);

static void SUPERTMR_DRV_IrqHandler(uint32_t instance, uint8_t channelPair);

static status_t SUPERTMR_DRV_InitMeasurement(uint32_t instance, uint8_t channel, bool continuousModeEn,
                                             supertmr_signal_measurement_mode_t measurementType);

/**
 * @brief SUPERTMR_IRQHandler handler.
 */
void SUPERTMR_IRQHandler(void *data)
{
    supertmr_interrupt_t *int_param = (supertmr_interrupt_t *)data;
    SUPERTMR_DRV_IrqHandler(int_param->instance, int_param->channel / 2);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_InitMeasurement
 * Description   : This function is used to initialize the measurement mode in the
 * input capture mode. It is called in the SUPERTMR_DRV_InitInputCapture function.
 *
 *END**************************************************************************/
static status_t SUPERTMR_DRV_InitMeasurement(uint32_t instance, uint8_t channel, bool continuousModeEn,
                                             supertmr_signal_measurement_mode_t measurementType)
{
    OS_ASSERT(0U == (channel % 2U));
    supertmr_type_t *supertmrBase   = g_pstSupertmrBase[instance];
    supertmr_state_t *state         = supertmrStatePtr[instance];
    uint8_t chnlPairNum             = (uint8_t)(channel >> 1U);
    status_t retStatus              = STATUS_SUCCESS;
    supertmr_interrupt_t *int_param = &state->int_param[channel];
    OS_RegisterType_t uType         = {0};

    /* Enable the dual edge mode */
    SUPERTMR_DRV_SetDualEdgeCaptureCmd(supertmrBase, chnlPairNum, true);
    /* Enable dual edge input capture */
    SUPERTMR_DRV_SetDualChnDecapCmd(supertmrBase, chnlPairNum, true);

    /* If continuous mode is set */
    if (true == continuousModeEn) {
        /* Set MSnA and MSnB bit */
        SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, channel, 3U);
    } else {
        /* Clear MSnA and Set MSnB bit */
        SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, channel, 2U);
    }

    /* Check If want to measure a pulse width or period of the signal */
    if ((SUPERTMR_PERIOD_ON_MEASUREMENT == measurementType) ||
        (SUPERTMR_RISING_EDGE_PERIOD_MEASUREMENT == measurementType)) {
        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channel, 1U);
        if (SUPERTMR_PERIOD_ON_MEASUREMENT == measurementType) {
            /* Measure time between rising and falling edge - positive duty */
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, (uint8_t)(channel + 1U), 2U);
        } else {
            /* If channel (n) is configured to capture falling edges (ELS(n)B:ELS(n)A = 0:1)
             * then channel (n+1) also captures falling edges (ELS(n+1)B:ELS(n+1)A = 0:1) */
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, (uint8_t)(channel + 1U), 1U);
        }
    } else if ((SUPERTMR_PERIOD_OFF_MEASUREMENT == measurementType) ||
               (SUPERTMR_FALLING_EDGE_PERIOD_MEASUREMENT == measurementType)) {
        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channel, 2U);
        if (SUPERTMR_PERIOD_OFF_MEASUREMENT == measurementType) {
            /* Measure time between falling and rising edge - negative duty */
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, (uint8_t)(channel + 1U), 1U);
        } else {
            /* If channel (n) is configured to capture rising edges (ELS(n)B:ELS(n)A = 1:0) than
             * channel (n+1) is setup to capture also raising edges (ELS(n+1)B:ELS(n+1)A = 1:0) */
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, (uint8_t)(channel + 1U), 2U);
        }
    } else {
        retStatus = STATUS_ERROR;
    }

    /* Enable the interrupt request for the channel which will indicate that the measurement is done. */
    SUPERTMR_DRV_EnableChnInt(supertmrBase, (uint8_t)(channel + 1U));
    /* Enable IRQ for SUPERTMR */
    int_param->instance = instance;
    int_param->channel  = channel + 1;
    uType.trig_mode     = CLIC_LEVEL_TRIGGER;
    uType.lvl           = 2;
    uType.priority      = 1;
    uType.data_ptr      = (void *)int_param;
    OS_RequestIrq(g_euSupertmrIrqId[instance][channel + 1], SUPERTMR_IRQHandler, &uType);
    OS_EnableIrq(g_euSupertmrIrqId[instance][channel + 1]);

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_InitInputCapture
 * Description   : Configures Channel Input Capture for either getting time-stamps on edge detection
 * or on signal measurement . When the edge specified in the captureMode
 * argument occurs on the channel the SUPERTMR counter is captured into the CnV register.
 * The user will have to read the CnV register separately to get this value. The filter
 * function is disabled if the filterVal argument passed in is 0. The filter function
 * is available only on channels 0,1,2,3.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_InitInputCapture(uint32_t instance, const supertmr_input_param_t *param)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(NULL != param);
    OS_ASSERT(param->nMaxCountValue > 0U);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t chnlPairNum           = 0U;
    uint8_t index                 = 0U;
    uint8_t hwChannel             = 0U;
    bool contModeEn;
    supertmr_signal_measurement_mode_t measurementType;
    supertmr_state_t *state = supertmrStatePtr[instance];
    status_t retStatus      = STATUS_SUCCESS;
    OS_RegisterType_t uType = {0};

    if ((NULL != state) && ((SUPERTMR_MODE_NOT_INITIALIZED == state->supertmrMode) ||
                            (SUPERTMR_MODE_EDGE_ALIGNED_PWM == state->supertmrMode) ||
                            (SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE == state->supertmrMode))) {
        if (state->supertmrMode == SUPERTMR_MODE_NOT_INITIALIZED) {
            SUPERTMR_DRV_SetClockSource(supertmrBase, SUPERTMR_CLOCK_SOURCE_NONE);
            SUPERTMR_DRV_SetCounterInitVal(supertmrBase, 0U);
            SUPERTMR_DRV_SetMod(supertmrBase, param->nMaxCountValue);
            SUPERTMR_DRV_SetCpwms(supertmrBase, false);
        } else {
            /* In this case clock source is configured from PWM init. */
            state->supertmrMode = SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE;
        }

        /* Get curent MOD value */
        state->supertmrModValue = SUPERTMR_DRV_GetMod(supertmrBase);

        for (index = 0U; index < param->nNumChannels; index++) {
            hwChannel   = param->inputChConfig[index].hwChannelId;
            chnlPairNum = (uint8_t)(hwChannel >> 1U);
            /* Save in state structure user define handlers */
            state->channelsCallbacksParams[hwChannel] = param->inputChConfig[index].channelsCallbacksParams;
            state->channelsCallbacks[hwChannel]       = param->inputChConfig[index].channelsCallbacks;
            state->enableNotification[hwChannel]      = true;

            /* Enable filtering for input channels */
            if (hwChannel < CHAN4_IDX) {
                if (true == param->inputChConfig[index].filterEn) {
                    SUPERTMR_DRV_SetChnInputCaptureFilter(supertmrBase, hwChannel,
                                                          (uint8_t)param->inputChConfig[index].filterValue);
                } else {
                    SUPERTMR_DRV_SetChnInputCaptureFilter(supertmrBase, hwChannel, 0U);
                }
            }

            if (SUPERTMR_EDGE_DETECT == param->inputChConfig[index].inputMode) {
                /* Disable the dual edge mode */
                SUPERTMR_DRV_SetDualEdgeCaptureCmd(supertmrBase, chnlPairNum, false);
                /* Set input capture mode */
                SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, hwChannel, 0U);
                /* Set the event which will generate the interrupt */
                SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, hwChannel,
                                             (uint8_t)param->inputChConfig[index].edgeAlignement);
                /* Enable interrupt request for the current channel */
                SUPERTMR_DRV_EnableChnInt(supertmrBase, hwChannel);
                /* TODO:zhoulei Enable the SUPERTMR counter reset? */
                SUPERTMR_DRV_SetChnIcrstCmd(supertmrBase, hwChannel, true);
                /* Enable IRQ for SUPERTMR */
                supertmr_interrupt_t *int_param = &state->int_param[hwChannel];
                int_param->instance             = instance;
                int_param->channel              = hwChannel;
                uType.trig_mode                 = CLIC_LEVEL_TRIGGER;
                uType.lvl                       = 2;
                uType.priority                  = 1;
                uType.data_ptr                  = (void *)int_param;
                OS_RequestIrq(g_euSupertmrIrqId[instance][hwChannel], SUPERTMR_IRQHandler, &uType);
                OS_EnableIrq(g_euSupertmrIrqId[instance][hwChannel]);
            } else if (SUPERTMR_SIGNAL_MEASUREMENT == param->inputChConfig[index].inputMode) {
                measurementType = param->inputChConfig[index].measurementType;
                contModeEn      = param->inputChConfig[index].continuousModeEn;
                /* Configure the measurement type */
                retStatus = SUPERTMR_DRV_InitMeasurement(instance, hwChannel, contModeEn, measurementType);
            } else {
                /* Do nothing */
            }

            if (STATUS_SUCCESS != retStatus) {
                break;
            }
        }

        if (STATUS_SUCCESS == retStatus) {
            /* Update mode. */
            if (SUPERTMR_MODE_NOT_INITIALIZED == state->supertmrMode) {
                state->supertmrMode = SUPERTMR_MODE_INPUT_CAPTURE;
                /* Set clock source to start the counter. */
                SUPERTMR_DRV_SetClockSource(supertmrBase, state->supertmrClockSource);
            }
        } else {
            if (SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE == state->supertmrMode) {
                state->supertmrMode = SUPERTMR_MODE_EDGE_ALIGNED_PWM;
            }
        }
    } else {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_DeinitInputCapture
 * Description   : Disables Channel Input Capture
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_DeinitInputCapture(uint32_t instance, const supertmr_input_param_t *param)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(NULL != param);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t chnlPairNum           = 0U;
    uint8_t index                 = 0U;
    uint8_t hwChannel             = 0U;
    supertmr_state_t *state       = supertmrStatePtr[instance];
    OS_ASSERT(NULL != state);

    /* Remain configuration for PWM */
    if (SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE != state->supertmrMode) {
        /* SUPERTMR counter is disabled */
        SUPERTMR_DRV_SetClockSource(supertmrBase, SUPERTMR_CLOCK_SOURCE_NONE);
        SUPERTMR_DRV_SetCounterInitVal(supertmrBase, 0U);
        SUPERTMR_DRV_SetMod(supertmrBase, 0xFFFFU);
        SUPERTMR_DRV_SetCpwms(supertmrBase, false);
    }

    for (index = 0U; index < param->nNumChannels; index++) {
        hwChannel   = param->inputChConfig[index].hwChannelId;
        chnlPairNum = (uint8_t)(hwChannel >> 1U);
        /* Disable filtering for input channels */
        if (hwChannel < CHAN4_IDX) {
            SUPERTMR_DRV_SetChnInputCaptureFilter(supertmrBase, hwChannel, 0U);
        }

        SUPERTMR_DRV_SetDualChnCombineCmd(supertmrBase, chnlPairNum, false);
        SUPERTMR_DRV_SetDualEdgeCaptureCmd(supertmrBase, chnlPairNum, false);
        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, hwChannel, (uint8_t)0U);
        SUPERTMR_DRV_DisableChnInt(supertmrBase, hwChannel);
        SUPERTMR_DRV_ClearChnEventFlag(supertmrBase, hwChannel);

        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, (hwChannel | 1U), (uint8_t)0U);
        SUPERTMR_DRV_DisableChnInt(supertmrBase, (hwChannel | 1U));
        SUPERTMR_DRV_ClearChnEventFlag(supertmrBase, (hwChannel | 1U));
    }

    if (SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE == state->supertmrMode) {
        state->supertmrMode = SUPERTMR_MODE_EDGE_ALIGNED_PWM;
    } else {
        state->supertmrMode = SUPERTMR_MODE_NOT_INITIALIZED;
    }

    /* Clear Callbacks function from the state structure */
    for (index = 0U; index < SUPERTMR_CHANNEL_COUNT; index++) {
        state->channelsCallbacksParams[index] = NULL;
        state->channelsCallbacks[index]       = NULL;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_GetInputCaptureMeasurement
 * Description   : This function is used to calculate the measurement and/or time stamps values
 * which are read from the C(n, n+1)V registers and stored to the static buffers.
 *
 *END**************************************************************************/
uint16_t SUPERTMR_DRV_GetInputCaptureMeasurement(uint32_t instance, uint8_t channel)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    supertmr_state_t *state = supertmrStatePtr[instance];
    uint16_t value          = 0U;

    if (NULL != state) {
        value                              = state->measurementResults[channel];
        state->measurementResults[channel] = 0;
    }

    return value;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_StartNewSignalMeasurement
 * Description   : This function starts new Signal Measurements on a dual input compare channel
 * that is configured as single-shot measurement.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_StartNewSignalMeasurement(uint32_t instance, uint8_t channel)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    /* Clear CH(n)F and CH(n+1)F  flags and Set DECAP bit */
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t chnlPairNum           = (uint8_t)(channel >> 1U);
    status_t retStatus            = STATUS_SUCCESS;

    /* Get channel mode */
    if (SUPERTMR_INPUT_CAPTURE_SINGLE_SHOT == SUPERTMR_DRV_GetChnMode(supertmrBase, channel)) {
        if (SUPERTMR_DRV_GetDualEdgeCaptureBit(supertmrBase, chnlPairNum)) {
            /* Clear event flags for channel n and n + 1 */
            SUPERTMR_DRV_ClearChnEventFlag(supertmrBase, (uint8_t)(channel + 1U));
            SUPERTMR_DRV_ClearChnEventFlag(supertmrBase, channel);
            /* Set DECAP bit to start measurement */
            SUPERTMR_DRV_SetDualChnDecapCmd(supertmrBase, chnlPairNum, true);
        }
    } else {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_IC_DRV_SetChannelMode
 * Description   : This function is used to change the channel mode at run time or
 * when stopping channel. The channel mode is selected in the supertmr_ic_op_mode_t
 * enumeration type.
 *
 *END**************************************************************************/
status_t SUPERTMR_IC_DRV_SetChannelMode(uint32_t instance, uint8_t channel, supertmr_ic_op_mode_t inputMode,
                                        bool enableContinuousCapture)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t level                 = 1U;
    uint8_t nextChnLevel          = 1U;
    uint8_t channelPair           = (uint8_t)(channel >> 1U);

    if (SUPERTMR_DISABLE_OPERATION == inputMode) {
        /* Set the edge level to disable operation on the channel input */
        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channel, 0x0U);
    } else if ((SUPERTMR_TIMESTAMP_RISING_EDGE == inputMode) || (SUPERTMR_TIMESTAMP_FALLING_EDGE == inputMode) ||
               (SUPERTMR_TIMESTAMP_BOTH_EDGES == inputMode)) {
        /* Disable the dual edge mode */
        SUPERTMR_DRV_SetDualEdgeCaptureCmd(supertmrBase, channelPair, false);
        /* Set input capture mode */
        SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, channel, 0U);
        /* Set the event which will generate the interrupt */
        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channel, (uint8_t)inputMode);
        /* Enable interrupt request for the current channel */
        SUPERTMR_DRV_EnableChnInt(supertmrBase, channel);
    } else {
        /* Check the channel is even number */
        OS_ASSERT(0U == (channel % 2U));
        /* Enable the dual edge mode */
        SUPERTMR_DRV_SetDualEdgeCaptureCmd(supertmrBase, channelPair, true);
        /* Enable dual edge input capture */
        SUPERTMR_DRV_SetDualChnDecapCmd(supertmrBase, channelPair, true);
        /* If continuous mode is set*/
        if (true == enableContinuousCapture) {
            /* Set MSnA and MSnB bit*/
            SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, channel, 3U);
        } else {
            /* Clear MSnA and Set MSnB bit*/
            SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, channel, 2U);
        }

        /* Check If want to measure a pulse width or period of the signal */
        if ((SUPERTMR_MEASURE_PULSE_HIGH == inputMode) || (SUPERTMR_MEASURE_RISING_EDGE_PERIOD == inputMode)) {
            if (SUPERTMR_MEASURE_PULSE_HIGH == inputMode) {
                /* Measure time between rising and falling edge - positive duty */
                nextChnLevel = 2U;
            }
        } else {
            level = 2U;
            if (SUPERTMR_MEASURE_FALLING_EDGE_PERIOD == inputMode) {
                /* If channel (n) is configured to capture rising edges (ELS(n)B:ELS(n)A = 1:0) than
                 * channel (n+1) is setup to capture also raising edges (ELS(n+1)B:ELS(n+1)A = 1:0) */
                nextChnLevel = 2U;
            }
        }

        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channel, level);
        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, (uint8_t)(channel + 1U), nextChnLevel);

        /* Disable interrupt request for the current channel */
        SUPERTMR_DRV_DisableChnInt(supertmrBase, channel);
        SUPERTMR_DRV_EnableChnInt(supertmrBase, (uint8_t)(channel + 1U));
    }

    return STATUS_SUCCESS;
}

/** Implementation of SUPERTMR0_Ch0_Ch7_IRQHandler master handler named in startup code. */
void SUPERTMR_Ch0_7_IRQHandler(void *data)
{
    // log_debug("%s %d\n", __func__, __LINE__);
    uint32_t instance   = (size_t)data;
    uint32_t statusFlag = SUPERTMR_DRV_GetStatusFlags(instance);
    uint32_t enabledInt = SUPERTMR_DRV_GetEnabledInterrupts(instance);
    uint32_t index      = 0U;
    uint32_t shift      = 0U;
    bool isIntOnChn     = false;
    bool isIntOnNextChn = false;

    for (index = 0U; index < (uint32_t)(SUPERTMR_CHANNEL_COUNT / 2U); index++) {
        shift      = index << 1U;
        isIntOnChn = (((statusFlag & enabledInt) & (0x1UL << shift)) != 0U) ? true : false;
        shift++;
        isIntOnNextChn = (((statusFlag & enabledInt) & (0x1UL << shift)) != 0U) ? true : false;

        if ((true == isIntOnChn) || (true == isIntOnNextChn)) {
            SUPERTMR_DRV_IrqHandler(instance, (uint8_t)index);
        }
    }
}

static void SUPERTMR_DRV_IrqHandler(uint32_t instance, uint8_t channelPair)
{
    const supertmr_state_t *state = supertmrStatePtr[instance];
    switch (state->supertmrMode) {
    case SUPERTMR_MODE_INPUT_CAPTURE:
    case SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE:
        SUPERTMR_DRV_InputCaptureHandler(instance, channelPair);
        break;
    default:
        /* Nothing to do */
        break;
    }
}

static void SUPERTMR_DRV_InputCaptureHandler(uint32_t instance, uint8_t channelPair)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(channelPair < (SUPERTMR_CHANNEL_COUNT >> 1U));

    supertmr_state_t *state       = supertmrStatePtr[instance];
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t channel               = (uint8_t)(channelPair << 1U);

    /* Verify the mode for current pair of channels */
    if (SUPERTMR_DRV_GetDualEdgeCaptureBit(supertmrBase, channelPair)) {
        /* Dual edge input capture case */
        uint16_t first_event_time  = SUPERTMR_DRV_GetChnCountVal(supertmrBase, channel);
        uint16_t second_event_time = SUPERTMR_DRV_GetChnCountVal(supertmrBase, (uint8_t)(channel + 1U));
        if (second_event_time <= first_event_time) {
            /* Measurement when overflow occurred */
            state->measurementResults[channel] =
                (uint16_t)(second_event_time + (state->supertmrModValue - first_event_time));
        } else {
            /* Measurement when overflow doesn't occurred */
            state->measurementResults[channel] = (uint16_t)(second_event_time - first_event_time);
        }

        /* Clear flags for channels n and n+1 */
        SUPERTMR_DRV_ClearChnEventFlag(supertmrBase, channel);
        SUPERTMR_DRV_ClearChnEventFlag(supertmrBase, (uint8_t)(channel + 1U));
    } else {
        /* To get the channel interrupt source the both channels flag must be checked */
        if (false == SUPERTMR_DRV_HasChnEventOccurred(supertmrBase, channel)) {
            channel++;
        }

        /* Get the time stamp of the event */
        state->measurementResults[channel] = SUPERTMR_DRV_GetChnCountVal(supertmrBase, channel);
        /* Clear the flag for C(n+1) channel */
        SUPERTMR_DRV_ClearChnEventFlag(supertmrBase, channel);
    }

    /* Re-update MOD value */
    state->supertmrModValue = SUPERTMR_DRV_GetMod(supertmrBase);

    /* If the callback is defined to use it */
    if (NULL != ((state->channelsCallbacks[channel])) && true == (state->enableNotification[channel])) {
        state->channelsCallbacks[channel](IC_EVENT_MEASUREMENT_COMPLETE, state->channelsCallbacksParams[channel]);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_IC_DRV_SetHallMode
 * Description   : This function is used to set supertmr hall mode
 * Set SUPERTMR2CH1SEL bit to XOR SUPERTMR2_CH0, SUPERTMR2_CH1 and SUPERTMR1_CH1 to one single SUPERTMR2_CH1 input
 *
 *END**************************************************************************/
status_t SUPERTMR_IC_DRV_SetHallMode(bool enable)
{
    TCSR->SUPERTMROPT1 |= TCSR_SUPERTMROPT1_SUPERTMR2CH1SEL(1);
    if (TCSR->SUPERTMROPT1 & TCSR_SUPERTMROPT1_SUPERTMR2CH1SEL(1)) {
        return STATUS_SUCCESS;
    }

    return STATUS_ERROR;
}