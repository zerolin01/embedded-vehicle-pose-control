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
 * @file superio_common.c
 * @brief SUPERIO common driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "superio_common.h"
#include "superio_register.h"
#include "clock_driver.h"

/* Table of base addresses for SUPERIO instances. */
superio_type_t *const g_superioBase[SUPERIO_INSTANCE_COUNT] = SUPERIO_BASE_PTRS;

/* Pointer to device state structures. This structure contains data common to all drivers on one device */
superio_device_state_t *g_superioDeviceStatePtr[SUPERIO_INSTANCE_COUNT] = {NULL};

/* Table for SUPERIO IRQ numbers */
const IRQn_Type g_superioIrqId[SUPERIO_INSTANCE_COUNT] = SUPERIO_IRQS;

/* PCC clock sources, for getting the input clock frequency */
const clock_names_t g_superioClock[SUPERIO_INSTANCE_COUNT] = {SUPERIO0_CLK};

/* SuperIO PDMA request sources */
const uint8_t g_superioDMASrc[SUPERIO_INSTANCE_COUNT][FEATURE_SUPERIO_MAX_SHIFTER_COUNT] = {
    {(uint8_t)FEATURE_SUPERIO_DMA_REQ_0, (uint8_t)FEATURE_SUPERIO_DMA_REQ_1, (uint8_t)FEATURE_SUPERIO_DMA_REQ_2,
     (uint8_t)FEATURE_SUPERIO_DMA_REQ_3}};

#define OSIF_WAIT_FOREVER (0xFFFFFFFFUL)

/* Declaration of SUPERIO_IRQHandler. Needed just to avoid MISRA violation. */
void SUPERIO_IRQHandler(void *parameters);

/*
 * Function Name : SUPERIO_DRV_InitDevice
 * Description   : Initializes the SuperIO device
 *
 */
status_t SUPERIO_DRV_InitDevice(uint32_t instance, superio_device_state_t *deviceState)
{
    superio_type_t *baseAddr;
    uint8_t count;
    status_t osifStat       = STATUS_SUCCESS;
    status_t clkErr;
    uint32_t inputClock;
    OS_RegisterType_t uType = {0};

    if (NULL == g_superioDeviceStatePtr[instance]) {
        CLOCK_SYS_ClockEnable(g_superioClock[instance], true);
        /* Get the protocol clock frequency */
        clkErr = CLOCK_SYS_GetFreq(g_superioClock[instance], &inputClock);
        OS_ASSERT(clkErr == STATUS_SUCCESS);

        baseAddr = g_superioBase[instance];
        /* Create resource allocation mutex */
        deviceState->resourceLock = OS_MutexNew(NULL);
        // OS_ASSERT(osifStat == STATUS_SUCCESS);
        /* Reset SuperIO module */
        SUPERIO_Init(baseAddr);
        /* Register IRQ */
        /* Install SPI irq handler */
        uType.trig_mode = CLIC_LEVEL_TRIGGER;
        uType.lvl       = 250;
        uType.priority  = 1;
        uType.data_ptr  = NULL;
        OS_RequestIrq(g_superioIrqId[instance], SUPERIO_IRQHandler, &uType);
        /* Enable SUPERIO interrupt in the interrupt manager */
        OS_EnableIrq(g_superioIrqId[instance]);
        /* Enable module */
        SUPERIO_SetEnable(baseAddr, true);
        /* Initialize device structure */
        deviceState->resourceAllocation = 0U;
        /* Clear state structures array */
        for (count = 0U; count < FEATURE_SUPERIO_MAX_SHIFTER_COUNT; count++) {
            deviceState->superioStatePtr[count] = NULL;
        }
        g_superioDeviceStatePtr[instance] = deviceState;
    }

    (void)osifStat;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_DRV_DeinitDevice
 * Description   : De-initializes the SuperIO device
 *
 */
status_t SUPERIO_DRV_DeinitDevice(uint32_t instance)
{
    superio_type_t *baseAddr;

    baseAddr = g_superioBase[instance];
    /* Reset SuperIO module */
    SUPERIO_Init(baseAddr);
    CLOCK_SYS_ClockEnable(g_superioClock[instance], false);
    /* Disable SUPERIO interrupt in the interrupt manager */
    OS_DisableIrq(g_superioIrqId[instance]);
    /* Destroy resource allocation mutex */
    (void)OS_MutexDelete(g_superioDeviceStatePtr[instance]->resourceLock);
    /* Free resources */
    g_superioDeviceStatePtr[instance] = NULL;

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_DRV_Reset
 * Description   : Resets the SuperIO device
 *
 */
status_t SUPERIO_DRV_Reset(uint32_t instance)
{
    superio_type_t *baseAddr;

    baseAddr = g_superioBase[instance];
    /* Reset SuperIO module */
    SUPERIO_Init(baseAddr);

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_Allocate
 * Description   : Allocate timers and shifters for a new driver instance
 *
 */
static status_t SUPERIO_Allocate(uint32_t instance, superio_common_state_t *driver)
{
    uint8_t count;                  /* used to iterate through resources (shifters/timers) */
    uint8_t step;                   /* keeps track of how many resources are needed for this driver */
    uint8_t size;                   /* total number of resources */
    uint8_t mask;                   /* bit-mask corresponding to current resources */
    uint8_t resourceAllocation;     /* current resource allocation map */
    const superio_type_t *baseAddr; /* base address of the SuperIO module */
    status_t status = STATUS_ERROR; /* assume the worst: no resources found for this driver */

    /* Find free resources for a new driver. Drivers may need one or two adjacent shifters and timers */
    baseAddr           = g_superioBase[instance];
    resourceAllocation = g_superioDeviceStatePtr[instance]->resourceAllocation;
    step               = driver->resourceCount;
    size               = SUPERIO_GetShifterNum(baseAddr);
    count              = 0U;
    mask               = (uint8_t)((1U << step) - 1U);
    (void)OS_MutexAcquire(g_superioDeviceStatePtr[instance]->resourceLock, OSIF_WAIT_FOREVER);

    /* find available shifters and timers for the driver */
    while ((status == STATUS_ERROR) && (((uint8_t)(count + step)) <= size)) {
        if (0U == (resourceAllocation & mask)) {
            /* resources found */
            driver->resourceIndex = count;
            /* mark resources as busy */
            g_superioDeviceStatePtr[instance]->resourceAllocation |= mask;
            status = STATUS_SUCCESS;
        }
        /* continue searching */
        count += 1U;
        mask <<= 1;
    }

    (void)OS_MutexRelease(g_superioDeviceStatePtr[instance]->resourceLock);

    return status;
}

/*
 * Function Name : SUPERIO_Deallocate
 * Description   : De-allocate timers and shifters for a driver instance
 *
 */
static void SUPERIO_Deallocate(uint32_t instance, const superio_common_state_t *driver)
{
    uint8_t mask;

    mask = (uint8_t)((1U << driver->resourceCount) - 1U);
    mask <<= driver->resourceIndex;
    (void)OS_MutexAcquire(g_superioDeviceStatePtr[instance]->resourceLock, OSIF_WAIT_FOREVER);
    g_superioDeviceStatePtr[instance]->resourceAllocation &= (uint8_t)~mask;
    (void)OS_MutexRelease(g_superioDeviceStatePtr[instance]->resourceLock);
}

/*
 * Function Name : SUPERIO_ResourcesInit
 * Description   : Initializes the resources for the current driver
 *
 */
static void SUPERIO_ResourcesInit(uint32_t instance, const superio_common_state_t *driver)
{
    uint8_t resource;
    uint8_t resourceCount;
    uint8_t resourceIndex;
    superio_type_t *baseAddr;

    baseAddr      = g_superioBase[instance];
    resourceCount = driver->resourceCount;
    resourceIndex = driver->resourceIndex;
    for (resource = resourceIndex; resource < (resourceIndex + resourceCount); resource++) {
        /* Ensure all shifters/timers are disabled */
        SUPERIO_SetShifterMode(baseAddr, resource, SUPERIO_SHIFTER_MODE_DISABLED);
        SUPERIO_SetTimerMode(baseAddr, resource, SUPERIO_TIMER_MODE_DISABLED);
        /* Ensure all interrupts and PDMA requests are disabled */
        SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << resource), false);
        SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << resource), false);
        SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << resource), false);
        SUPERIO_SetShifterDMARequest(baseAddr, (uint8_t)(1U << resource), false);
        /* Clear any leftover flags */
        SUPERIO_ClearShifterStatus(baseAddr, resource);
        SUPERIO_ClearShifterErrorStatus(baseAddr, resource);
        SUPERIO_ClearTimerStatus(baseAddr, resource);
    }
}

/*
 * Function Name : SUPERIO_DRV_InitDriver
 * Description   : Initializes an instance of SuperIO driver
 *
 */
status_t SUPERIO_DRV_InitDriver(uint32_t instance, superio_common_state_t *driver)
{
    uint16_t count;
    status_t retCode;
    superio_common_state_t **superioStatePtr;

    /* allocate times and shifters for the driver */
    retCode = SUPERIO_Allocate(instance, driver);
    if (STATUS_SUCCESS != retCode) { /* no more resources available */
        return retCode;
    }

    /* get driver list for this device instance */
    superioStatePtr = g_superioDeviceStatePtr[instance]->superioStatePtr;
    /* find an empty state structure slot for the driver */
    for (count = 0U; count < FEATURE_SUPERIO_MAX_SHIFTER_COUNT; count++) {
        if (superioStatePtr[count] == NULL) {
            /* found it, place the new driver struct here */
            superioStatePtr[count] = driver;
            break;
        }
    }
    /* no need to check if there was room since allocation was successful */
    OS_ASSERT(count < FEATURE_SUPERIO_MAX_SHIFTER_COUNT);

    /* initialize the allocated resources */
    SUPERIO_ResourcesInit(instance, driver);

    /* init the rest of the common state structure */
    driver->isr      = NULL;
    driver->instance = instance;

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_DRV_DeinitDriver
 * Description   : De-initializes an instance of SuperIO driver
 *
 */
status_t SUPERIO_DRV_DeinitDriver(const superio_common_state_t *driver)
{
    uint16_t count;
    uint32_t instance;
    superio_common_state_t **superioStatePtr;

    instance = driver->instance;
    /* get driver list for this device instance */
    superioStatePtr = g_superioDeviceStatePtr[instance]->superioStatePtr;
    /* find the driver in the list */
    for (count = 0U; count < FEATURE_SUPERIO_MAX_SHIFTER_COUNT; count++) {
        if (superioStatePtr[count] == driver) {
            /* found it - remove it */
            superioStatePtr[count] = NULL;
            break;
        }
    }
    OS_ASSERT(count < FEATURE_SUPERIO_MAX_SHIFTER_COUNT);

    /* de-allocate timers and shifters for the driver */
    SUPERIO_Deallocate(instance, driver);

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_IRQHandler
 * Description   : Implementation of SUPERIO interrupt handler named in start-up code
 *
 */
void SUPERIO_IRQHandler(void *parameters)
{
    uint16_t count;
    uint32_t instance;
    uint32_t resourceMask;
    uint32_t shifterEvents;
    uint32_t shifterErrorEvents;
    uint32_t timerEvents;
    uint32_t enabledInterrupts;
    const superio_type_t *baseAddr;
    superio_common_state_t *const *driverList;
    superio_common_state_t *driverState;

    instance = 0U;
    baseAddr = g_superioBase[instance];
    /* get masks of SuperIO events */
    /* read enabled interrupts in a separate instruction to avoid MISRA violation */
    enabledInterrupts  = SUPERIO_GetAllShifterInterrupt(baseAddr);
    shifterEvents      = SUPERIO_GetAllShifterStatus(baseAddr) & enabledInterrupts;
    enabledInterrupts  = SUPERIO_GetAllShifterErrorInterrupt(baseAddr);
    shifterErrorEvents = SUPERIO_GetAllShifterErrorStatus(baseAddr) & enabledInterrupts;
    enabledInterrupts  = SUPERIO_GetAllTimerInterrupt(baseAddr);
    timerEvents        = SUPERIO_GetAllTimerStatus(baseAddr) & enabledInterrupts;
    /* get driver list for this device instance */
    driverList = g_superioDeviceStatePtr[instance]->superioStatePtr;
    /* check which driver instances need to be serviced */
    for (count = 0U; count < FEATURE_SUPERIO_MAX_SHIFTER_COUNT; count++) {
        driverState = driverList[count];
        /* check if driver is initialized and uses interrupts */
        if ((driverState != NULL) && (driverState->isr != NULL)) {
            /* compute mask of shifters/timers used by this driver */
            resourceMask = ((1UL << driverState->resourceCount) - 1U) << driverState->resourceIndex;
            /* check if this instance has any pending events */
            if (((shifterEvents & resourceMask) != 0U) || ((shifterErrorEvents & resourceMask) != 0U) ||
                ((timerEvents & resourceMask) != 0U)) {
                /* there is an event for the current instance - call the isr */
                (driverState->isr)(driverState);
            }
        }
    }
}
