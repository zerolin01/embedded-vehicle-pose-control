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
 * @file dac_hw_access.h
 * @brief DAC driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __DAC_HW_ACCESS_H__
#define __DAC_HW_ACCESS_H__

#include "dac_driver.h"
#include "dac_register.h"
#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Verify if the DAC is enabled
 * @param baseAddr - dac base pointer
 * @return - dac state
 *  true - DAC is enabled
 *  false - DAC is disabled
 */
static inline bool DAC_GetEnableState(const dac_type_t *baseAddr)
{
    return (bool)(((baseAddr->C0) >> DAC_C0_EN_SHIFT) & 1U);
}

/**
 * @brief Set the DAC state (enabled/disabled)
 * @param baseAddr - dac base pointer
 * @param toSet - DAC state
 *  true - DAC is enabled
 *  false - DAC is disabled
 */
static inline void DAC_SetEnableState(dac_type_t *baseAddr, bool toSet)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(DAC_C0_EN_MASK))) | ((uint32_t)toSet << DAC_C0_EN_SHIFT);
}

#if 0
/**
 * @brief Set the DAC refresh mode (enabled/disabled)
 * @param baseAddr - dac base pointer
 * @param refresh - DAC refresh state
 *  true - DAC refresh is enabled
 *  false - DAC refresh is disabled
 */
static inline void DAC_SetRefreshMode(dac_type_t *baseAddr, bool refresh)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(DAC_C0_LP_MASK))) | ((uint32_t)refresh << DAC_C0_LP_SHIFT);
}

/**
 * @brief Get the DAC refresh mode state
 * @param baseAddr - dac base pointer
 * @return - dac refresh mode state
 *  true - DAC refresh is enabled
 *  false - DAC refresh is disabled
 */
static inline bool DAC_GetDACRefreshMode(const dac_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp          = (tmp & DAC_C0_LP_MASK) >> DAC_C0_LP_SHIFT;
    return (bool)tmp;
}

/**
 * @brief Gets the current DAC refresh clock number.
 *
 * This function returns the configured clock divider
 * bitfield value for the DAC instance.
 *
 * @param baseAddr adc base pointer
 * @return dac_refresh_clock_t the clock number.
 */
static inline dac_refresh_clock_t DAC_GetDACRefreshClockNumber(const dac_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp          = (tmp & DAC_C0_LPRN_MASK) >> DAC_C0_LPRN_SHIFT;
    return (dac_refresh_clock_t)(tmp);
}

/**
 * @brief Sets the DAC clock divider configuration.
 *
 * This functions configures the DAC instance clock divider.
 *
 * @param baseAddr adc base pointer
 * @param clockNumber clock number
 */
static inline void DAC_SetRefreshClockNumber(dac_type_t *const baseAddr, const dac_refresh_clock_t clockNumber)
{
    uint32_t tmp = baseAddr->C0;
    tmp &= ~(DAC_C0_LPRN_MASK);
    tmp |= DAC_C0_LPRN(clockNumber);
    baseAddr->C0 = tmp;
}
#endif

/**
 * @brief Get the DAC FIFO water line
 * @param baseAddr - dac base pointer
 * @return uint32_t - dac FIFO water line
 */
static inline uint32_t DAC_GetWaterLine(const dac_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp          = (tmp & DAC_C0_WL_MASK) >> DAC_C0_WL_SHIFT;

    return tmp;
}

/**
 * @brief Set the DAC FIFO water line
 * @param baseAddr - dac base pointer
 * @param waterLine - DAC FIFO water line
 */
static inline void DAC_SetWaterLine(dac_type_t *baseAddr, uint32_t waterLine)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(DAC_C0_WL_MASK))) | ((uint32_t)waterLine << DAC_C0_WL_SHIFT);
}

/**
 * @brief Get the DAC interrupt state
 * @param baseAddr - dac base pointer
 * @return - dac interrupt state
 *  true - DAC interrupt enabled
 *  false - DAC interrupt disabled
 */
static inline bool DAC_GetIntEnable(const dac_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp          = (tmp & DAC_C0_IEN_MASK) >> DAC_C0_IEN_SHIFT;

    /* Enum defines all possible values, so casting is safe */
    return (bool)tmp;
}

/**
 * @brief Set the DAC interrupt (enabled/disabled)
 * @param baseAddr - dac base pointer
 * @param enable - DAC interrupt state
 *  true - DAC interrupt is enabled
 *  false - DAC interrupt is disabled
 */
static inline void DAC_SetIntEnable(dac_type_t *baseAddr, bool enable)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(DAC_C0_IEN_MASK))) | ((uint32_t)enable << DAC_C0_IEN_SHIFT);
}

/**
 * @brief Get the DAC data mode state
 * @param baseAddr - dac base pointer
 * @return - dac data mode state
 *  true - DAC data mode enabled
 *  false - DAC data mode is disabled
 */
static inline bool DAC_GetDataMode(const dac_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp          = (tmp & DAC_C0_NEN_MASK) >> DAC_C0_NEN_SHIFT;

    /* Enum defines all possible values, so casting is safe */
    return (bool)tmp;
}

/**
 * @brief Set the DAC data mode (enabled/disabled)
 * @param baseAddr - dac base pointer
 * @param dataMode - DAC data mode state
 *  true - DAC data mode is enabled
 *  false - DAC data mode is disabled
 */
static inline void DAC_SetDataMode(dac_type_t *baseAddr, bool dataMode)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(DAC_C0_NEN_MASK))) | ((uint32_t)dataMode << DAC_C0_NEN_SHIFT);
}

/**
 * @brief Get the DAC output vol buffer state
 * @param baseAddr - dac base pointer
 * @return - dac output vol buffer state
 *  true - DAC output vol buffers enabled
 *  false - DAC output vol buffer is disabled
 */
static inline bool DAC_GetOutputVolBuffer(const dac_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp          = (tmp & DAC_C0_BUFFEN_MASK) >> DAC_C0_BUFFEN_SHIFT;

    /* Enum defines all possible values, so casting is safe */
    return (bool)tmp;
}

/**
 * @brief Set the DAC output vol buffer (enabled/disabled)
 * @param baseAddr - dac base pointer
 * @param buffer - DAC buffer state
 *  true - DAC buffer is enabled
 *  false - DAC buffer is disabled
 */
static inline void DAC_SetOutputVolBuffer(dac_type_t *baseAddr, bool buffer)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(DAC_C0_BUFFEN_MASK))) | ((uint32_t)buffer << DAC_C0_BUFFEN_SHIFT);
}

/**
 * @brief Set the DAC refresh state
 * @param baseAddr - dac base pointer
 * @param buffer - DAC buffer state
 *  true - DAC buffer is enabled
 *  false - DAC buffer is disabled
 */
static inline void DAC_SetRefreshEnable(dac_type_t *baseAddr, bool state)
{
    (baseAddr->C1) = ((baseAddr->C1) & (~(DAC_C1_LPREF_MASK))) | ((uint32_t)state << DAC_C1_LPREF_SHIFT);
}

/**
 * @brief Get the DAC FIFO Full state
 * @param baseAddr - dac base pointer
 * @return - dac FIFO full state
 *  true - DAC FIFO is full
 *  false - DAC FIFO is not full
 */
static inline bool DAC_GetFifoFullState(const dac_type_t *baseAddr)
{
    return (bool)(((baseAddr->C1) >> DAC_C1_FULL_SHIFT) & 1U);
}

/**
 * @brief Get the DAC FIFO Empty state
 * @param baseAddr - dac base pointer
 * @return - dac FIFO empty state
 *  true - DAC FIFO is empty
 *  false - DAC FIFO is not empty
 */
static inline bool DAC_GetFifoEmptyState(const dac_type_t *baseAddr)
{
    return (bool)(((baseAddr->C1) >> DAC_C1_EMPTY_SHIFT) & 1U);
}

/**
 * @brief Set the DAC frequency word
 * @param baseAddr - dac base pointer
 * @param frequencyWord - DAC frequency word
 */
static inline void DAC_SetFrequencyWord(dac_type_t *baseAddr, uint32_t frequencyWord)
{
    (baseAddr->C2) = ((baseAddr->C2) & (~(DAC_C2_FW_MASK))) | ((uint32_t)frequencyWord << DAC_C2_FW_SHIFT);
}

/**
 * @brief Set the DAC input data
 * @param baseAddr - dac base pointer
 * @param data -  DAC input data
 */
static inline void DAC_SetDataInput(dac_type_t *baseAddr, uint32_t data)
{
    (baseAddr->C3) = ((baseAddr->C3) & (~(DAC_C3_DATA_MASK))) | ((uint32_t)data << DAC_C3_DATA_SHIFT);
}

#if defined(__cplusplus)
}
#endif

#endif
