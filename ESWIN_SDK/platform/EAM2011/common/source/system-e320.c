/*
 * Copyright (c) 2020-2021 ESWIN Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 * @file     system_e320.c
 * @brief    EMSIS ESWIN KittyHawk Core Device Peripheral Access Layer Source File for
 *           ESWIN e320 evaluation SoC which support ESWIN KittyHawk core
 * @version  V0.1.0
 * @date     4 Jan. 2021
 ******************************************************************************
 */
#include "eswin_sdk_soc.h"
#include <stdint.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
 *Define clocks
 *----------------------------------------------------------------------------
 */
/* ToDo: add here your necessary defines for device initialization
 *       following is an example for different system frequencies
 */

#ifndef SYSTEM_CLOCK  //TODO FPGA 20M
#define SYSTEM_CLOCK (20 * 1000 * 1000UL)
#endif

/**
 * \defgroup  EMSIS_Core_SystemConfig       System Device Configuration
 * \brief Functions for system and clock setup available in system_<device>.c.
 * \details
 * ESWIN provides a template file **system_Device.c** that must be adapted by
 * the silicon vendor to match their actual device. As a <b>minimum requirement</b>,
 * this file must provide:
 *  -  A device-specific system configuration function, \ref SystemInit.
 *  -  A global variable that contains the system frequency, \ref SystemCoreClock.
 *  -  A global clic configuration initialization, \ref CLIC_Init.
 *  -  Global c library \ref _init and \ref _fini functions called right before calling main function.
 *  -  Vendor customized interrupt, exception and nmi handling code, see \ref EMSIS_Core_IntExcNMI_Handling
 *
 * The file configures the device and, typically, initializes the oscillator (PLL) that is part
 * of the microcontroller device. This file might export other functions or variables that provide
 * a more flexible configuration of the microcontroller system.
 *
 * And this file also provided common interrupt, exception and NMI exception handling framework template,
 * Silicon vendor can customize these template code as they want.
 *
 * \note Please pay special attention to the static variable \c SystemCoreClock. This variable might be
 * used throughout the whole system initialization and runtime to calculate frequency/time related values.
 * Thus one must assure that the variable always reflects the actual system clock speed.
 *
 * \attention
 * Be aware that a value stored to \c SystemCoreClock during low level initialization (i.e. \c SystemInit()) might get
 * overwritten by C libray startup code and/or .bss section initialization.
 * Thus its highly recommended to call \ref SystemCoreClockUpdate at the beginning of the user \c main() routine.
 *
 * @{
 */

/*----------------------------------------------------------------------------
 *System Core Clock Variable
 *----------------------------------------------------------------------------
 */
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
 *       achieved after system intitialization.
 *       This means system core clock frequency after call to SystemInit()
 */
/**
 * \brief      Variable to hold the system core clock value
 * \details
 * Holds the system core clock, which is the system clock frequency supplied to the SysTick
 * timer and the processor core clock. This variable can be used by debuggers to query the
 * frequency of the debug timer or to configure the trace clock speed.
 *
 * \attention
 * Compilers must be configured to avoid removing this variable in case the application
 * program is not using it. Debugging systems require the variable to be physically
 * present in memory so that it can be examined to configure the debugger.
 */
uint32_t SystemCoreClock = SYSTEM_CLOCK; /* System Clock Frequency (Core Clock) */
uint32_t SystemTimerClock = SYSTEM_CLOCK / 4;

/*----------------------------------------------------------------------------
 *Clock functions
 *----------------------------------------------------------------------------
 */

/**
 * \brief      Function to update the variable \ref SystemCoreClock
 * \details
 * Updates the variable \ref SystemCoreClock and must be called whenever the core clock is changed
 * during program execution. The function evaluates the clock register settings and calculates
 * the current core clock.
 */
__WEAK void SystemCoreClockUpdate(void) /* Get Core Clock Frequency */
{
    /* ToDo: add code to calculate the system frequency based upon the current
     *      register settings.
     * Note: This function can be used to retrieve the system core clock frequeny
     *      after user changed register settings.
     */
    SystemCoreClock = SYSTEM_CLOCK;
}

/**
 * \brief      Function to Initialize the system.
 * \details
 * Initializes the microcontroller system. Typically, this function configures the
 * oscillator (PLL) that is part of the microcontroller device. For systems
 * with a variable clock speed, it updates the variable \ref SystemCoreClock.
 * SystemInit is called from the file <b>startup<i>_device</i></b>.
 */
void SystemInit(void)
{

}


/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_SystemAndClock */
