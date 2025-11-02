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
 * @file power_manager.c
 * @brief Power manager driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "power_manager.h"
#include <stddef.h>

static status_t POWER_SYS_CallbacksManagement(power_manager_notify_struct_t *notifyStruct,
                                              uint8_t *currentStaticCallback, power_manager_policy_t policy);

/*
 *
 * Function Name : POWER_SYS_Init
 * Description   : Initializes the Power manager for operation.
 * This function initializes the Power manager and its run-time state structure.
 * Reference to an array of Power mode configuration structures has to be passed
 * as parameter along with parameter specifying its size. At least one power mode
 * configuration is required. Optionally, reference to array of predefined
 * call-backs can be passed with its size parameter.
 * For details about call-backs refer to the power_manager_callback_user_config_t.
 * As Power manager stores only references to array of these structures they have
 * to exist while Power manager is used.
 *
 */
status_t POWER_SYS_Init(power_manager_user_config_t *(*powerConfigsPtr)[], uint8_t configsNumber,
                        power_manager_callback_user_config_t *(*callbacksPtr)[], uint8_t callbacksNumber)
{
    OS_ASSERT(powerConfigsPtr != NULL);                // Reference to the power configurations is valid.
    OS_ASSERT(configsNumber != 0U);                    // Power configuration index is valid.
    OS_ASSERT(gPowerManagerState.configs == NULL);     // Driver is not initialized, reference to configuration is not valid.
    OS_ASSERT(gPowerManagerState.configsNumber == 0U); // Driver is not initialized, number of configurations is zero.

    /* Store references to user-defined power mode configurations */
    gPowerManagerState.configs       = (power_manager_user_config_t * (*)[]) powerConfigsPtr;
    gPowerManagerState.configsNumber = configsNumber;
    gPowerManagerState.currentConfig = 0U;

    /* Store references to user-defined callback configurations and increment call-back handle counter */
    if (callbacksPtr != NULL) {
        gPowerManagerState.staticCallbacks       = (power_manager_callback_user_config_t * (*)[]) callbacksPtr;
        gPowerManagerState.staticCallbacksNumber = callbacksNumber;
        /* Default value of handle of last call-back that returned error */
        gPowerManagerState.errorCallbackIndex = callbacksNumber;
    } else {
        gPowerManagerState.staticCallbacks       = NULL;
        gPowerManagerState.staticCallbacksNumber = 0U;
        gPowerManagerState.errorCallbackIndex    = 0U;
    }

    return POWER_SYS_DoInit();
}

/*
 *
 * Function Name : POWER_SYS_Deinit
 * Description   : De-initializes the Power manager.
 *
 */
status_t POWER_SYS_Deinit(void)
{
    gPowerManagerState.configs               = NULL;
    gPowerManagerState.configsNumber         = 0U;
    gPowerManagerState.staticCallbacks       = NULL;
    gPowerManagerState.staticCallbacksNumber = 0U;

    return POWER_SYS_DoDeinit();
}

#if 0
/*
 *
 * Function Name : POWER_SYS_GetLastMode
 * Description   : This function returns power mode set as the last one.
 *
 * This function returns index of power mode which was set using POWER_SYS_SetMode() as the last one.
 * If the power mode was entered although some of the registered call-back denied the mode change
 * or if any of the call-backs invoked after the entering/restoring run mode failed then the return
 * code of this function has STATUS_ERROR value.
 * value.
 *
 */
status_t POWER_SYS_GetLastMode(uint8_t *powerModeIndexPtr)
{
    status_t returnCode; // Function return

    /* Pass index of user-defined configuration structure of currently running power mode */
    *powerModeIndexPtr = gPowerManagerState.currentConfig;

    /* Return whether all call-backs executed without error */
    if (gPowerManagerState.errorCallbackIndex == gPowerManagerState.staticCallbacksNumber) {
        returnCode = STATUS_SUCCESS;
    } else {
        returnCode = STATUS_ERROR;
    }

    return returnCode;
}

/*
 *
 * Function Name : POWER_SYS_GetLastModeConfig
 * Description   : This function returns user configuration structure of power mode set as the last one.
 *
 * This function returns reference to configuration structure which was set using POWER_SYS_SetMode()
 * as the last one. If the current power mode was entered although some of the registered call-back denied
 * the mode change or if any of the call-backs invoked after the entering/restoring run mode failed then
 * the return code of this function has STATUS_ERROR value.
 *
 */
status_t POWER_SYS_GetLastModeConfig(power_manager_user_config_t **powerModePtr)
{
    status_t returnCode; // Function return
    /* Pass reference to user-defined configuration structure of currently running power mode */
    *powerModePtr = (*gPowerManagerState.configs)[gPowerManagerState.currentConfig];

    /* Return whether all call-backs executed without error */
    if (gPowerManagerState.errorCallbackIndex == gPowerManagerState.staticCallbacksNumber) {
        returnCode = STATUS_SUCCESS;
    } else {
        returnCode = STATUS_ERROR;
    }

    return returnCode;
}
#endif

/*
 *
 * Function Name : POWER_SYS_GetErrorCallbackIndex
 * Description   : Returns the last failed notification callback.
 *
 * This function returns index of the last call-back that failed during the power mode switch while
 * the last POWER_SYS_SetMode() was called. If the last POWER_SYS_SetMode() call ended successfully
 * value equal to callbacks number is returned. Returned value represents index in the array of
 * static call-backs.
 *
 */
uint8_t POWER_SYS_GetErrorCallbackIndex(void)
{
    return gPowerManagerState.errorCallbackIndex;
}

/*
 *
 * Function Name : POWER_SYS_GetErrorCallback
 * Description   : Get the callback which returns error in last mode switch.
 *
 */
power_manager_callback_user_config_t *POWER_SYS_GetErrorCallback(void)
{

    /* If all callbacks return success. */
    return (gPowerManagerState.errorCallbackIndex >= gPowerManagerState.staticCallbacksNumber) ?
               NULL :
               (*gPowerManagerState.staticCallbacks)[gPowerManagerState.errorCallbackIndex];
}

/*
 * Function Name : POWER_SYS_CallbacksManagement
 * Description   : Internal function used by POWER_SYS_SetMode function for callback management
 * notifyStruct            callback notification structure
 * currentStaticCallback   index to array of statically registered call-backs
 * policy                  transaction policy
 *
 */
static status_t POWER_SYS_CallbacksManagement(power_manager_notify_struct_t *notifyStruct,
                                              uint8_t *currentStaticCallback, power_manager_policy_t policy)
{
    uint8_t callbacksNumber;                          // The total number of callbacks
    status_t returnCode = STATUS_SUCCESS;             // Function return
    status_t errorCode  = STATUS_SUCCESS;             // Error code to be returned(error case)
    status_t callbackStatusCode;                      // Status code returned by callback function
    power_manager_callback_type_t callbackTypeFilter; // Callback types to be excluded

    switch (notifyStruct->notifyType) {
    /* notify before */
    case POWER_MANAGER_NOTIFY_BEFORE:
        callbacksNumber    = gPowerManagerState.staticCallbacksNumber;
        callbackTypeFilter = POWER_MANAGER_CALLBACK_AFTER;
        errorCode          = STATUS_MCU_NOTIFY_BEFORE_ERROR;
        break;
    /* notify after */
    case POWER_MANAGER_NOTIFY_AFTER:
        callbacksNumber    = gPowerManagerState.staticCallbacksNumber;
        callbackTypeFilter = POWER_MANAGER_CALLBACK_BEFORE;
        errorCode          = STATUS_MCU_NOTIFY_AFTER_ERROR;
        break;
    /* notify recover */
    case POWER_MANAGER_NOTIFY_RECOVER:
        callbacksNumber    = gPowerManagerState.staticCallbacksNumber;
        callbackTypeFilter = POWER_MANAGER_CALLBACK_AFTER;
        errorCode          = STATUS_MCU_NOTIFY_BEFORE_ERROR;
        break;
    default:
        /* invalid value for notifyType */
        callbacksNumber    = 0U;
        callbackTypeFilter = POWER_MANAGER_CALLBACK_BEFORE;
        OS_ASSERT(false);
        break;
    }

    /* From all statically registered call-backs... */
    for ((*currentStaticCallback) = 0U; (*currentStaticCallback) < callbacksNumber; (*currentStaticCallback)++) {
        /* Pointer to callback configuration */
        const power_manager_callback_user_config_t *const callbackConfig =
            ((*gPowerManagerState.staticCallbacks)[*currentStaticCallback]);

        /* Check pointer to static callback configuration */
        if ((callbackConfig != NULL) && (callbackTypeFilter != callbackConfig->callbackType)) {
            /* In case that call-back returned error code mark it, store the call-back handle and eventually cancel the
             * mode switch */

            callbackStatusCode = callbackConfig->callbackFunction(notifyStruct, callbackConfig->callbackData);
            if (STATUS_SUCCESS != callbackStatusCode) {
                returnCode                            = errorCode;
                gPowerManagerState.errorCallbackIndex = *currentStaticCallback;
                /* If not forcing power mode switch, call all already notified call-backs to revert their state as the
                 * mode change is canceled */
                if (policy != POWER_MANAGER_POLICY_FORCIBLE) {
                    break;
                }
            }
        }
    }

    return returnCode;
}

/*
 *
 * Function Name : POWER_SYS_SetMode
 * Description   : Configures the power mode.
 *
 * This function switches to one of the defined power modes. Requested mode number is passed
 * as an input parameter. This function notifies all registered callback functions before
 * the mode change (using  POWER_MANAGER_CALLBACK_BEFORE set as callback type parameter),
 * sets specific power options defined in the power mode configuration and enters the specified
 * mode. In case of success switch, this function also invokes all registered callbacks after
 * the mode change (using POWER_MANAGER_CALLBACK_AFTER).
 * The actual mode switch is performed by POWER_SYS_DoSetMode in the specific implementation.
 * Callbacks are invoked in the following order: All registered callbacks are notified
 * ordered by index in the callbacks array (see callbacksPtr parameter of POWER_SYS_Init()).
 * The same order is used for before and after switch notifications.
 * The notifications before the power mode switch can be used to obtain confirmation about
 * the change from registered callbacks. If any registered callback denies the power
 * mode change, further execution of this function depends on mode change policy: the mode
 * change is either forced(POWER_MANAGER_POLICY_FORCIBLE) or exited(POWER_MANAGER_POLICY_AGREEMENT).
 * When mode change is forced, the result of the before switch notifications are ignored. If
 * agreement is required, if any callback returns an error code then further notifications
 * before switch notifications are cancelled and all already notified callbacks are re-invoked
 * with POWER_MANAGER_CALLBACK_AFTER set as callback type parameter. The index of the callback
 * which returned error code during pre-switch notifications is stored(any error codes during
 * callbacks re-invocation are ignored) and POWER_SYS_GetErrorCallback() can be used to get it.
 * Regardless of the policies, if any callback returned an error code, an error code denoting in which phase
 * the error occurred is returned when POWER_SYS_SetMode() exits.
 * It is possible to enter any mode supported by the processor. Refer to the chip reference manual
 * for list of available power modes. If it is necessary to switch into intermediate power mode prior to
 * entering requested mode, then the intermediate mode is entered without invoking the callback mechanism.
 *
 */
status_t POWER_SYS_SetMode(uint8_t powerModeIndex, power_manager_policy_t policy)
{
    power_manager_user_config_t *configPtr; // Local pointer to the requested user-defined power mode configuration
    status_t returnCode;                    // Function return
    status_t errorCode;
    bool successfulSwitch;                      // Power mode switch is successful or not
    uint8_t currentStaticCallback = 0U;         // Index to array of statically registered call-backs
    power_manager_notify_struct_t notifyStruct; // Callback notification structure

    /* Driver is already initialized. */
    OS_ASSERT(gPowerManagerState.configs != NULL);
    OS_ASSERT(gPowerManagerState.configsNumber != 0U);

    /* Power mode index is valid. */
    OS_ASSERT(powerModeIndex < gPowerManagerState.configsNumber);

    /* Initialization of local pointer to the requested user-defined power mode configuration */
    configPtr = (*gPowerManagerState.configs)[powerModeIndex];

    /* Reference to the requested user-defined power mode configuration is valid. */
    OS_ASSERT(configPtr != NULL);

    /* Default value of handle of last call-back that returned error */
    gPowerManagerState.errorCallbackIndex = gPowerManagerState.staticCallbacksNumber;

    /* Set the transaction policy in the notification structure */
    notifyStruct.policy = policy;

    /* Set the target power mode configuration in the notification structure */
    notifyStruct.targetPowerConfigIndex = powerModeIndex;
    notifyStruct.targetPowerConfigPtr   = configPtr;

    /* Notify those which asked to be called before the power mode change */
    notifyStruct.notifyType = POWER_MANAGER_NOTIFY_BEFORE;
    returnCode              = POWER_SYS_CallbacksManagement(&notifyStruct, &currentStaticCallback, policy);

    /* Power mode switch */
    /* In case that any call-back returned error code and  policy doesn't force the mode switch go to after switch
     * call-backs */
    if ((policy == POWER_MANAGER_POLICY_FORCIBLE) || (returnCode == STATUS_SUCCESS)) {
        returnCode       = POWER_SYS_DoSetMode(configPtr);
        successfulSwitch = (STATUS_SUCCESS == returnCode);
    } else {
        /* Unsuccessful switch */
        successfulSwitch = false;
    }

    if (successfulSwitch) {
        /* End of successful switch */

        /* Update current configuration index */
        gPowerManagerState.currentConfig = powerModeIndex;

        /* Notify those which asked to be called after the power mode change */
        notifyStruct.notifyType = POWER_MANAGER_NOTIFY_AFTER;
        returnCode =
            POWER_SYS_CallbacksManagement(&notifyStruct, &currentStaticCallback, POWER_MANAGER_POLICY_FORCIBLE);
    } else {
        /* End of unsuccessful switch */

        /* Notify those which have been called before the power mode change */
        notifyStruct.notifyType = POWER_MANAGER_NOTIFY_RECOVER;
        errorCode               = POWER_SYS_CallbacksManagement(&notifyStruct, &currentStaticCallback, POWER_MANAGER_POLICY_FORCIBLE);
        (void)(errorCode);
    }

    return returnCode;
}