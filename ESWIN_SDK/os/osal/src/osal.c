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
 * @file osal.c
 * @brief emps osal
 * @author abu (abu@eswincomputing.com)
 * @date 2023-03-24
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "osal.h"

#include "../../../log/include/log.h"
#include "platform.h"

/*  ==== Kernel Management Functions ==== */

/**
 * @brief OS_Msec2Tick
 * @details
 * 		The millisecond is converted to tick.
 * @param
 * 		millisecond - how many millisecond need to be converted.
 * @retval
 * 		return ticks.
 * */
uint64_t OS_Msec2Tick(uint32_t milliseconds)
{
    return BASIC_Msec2Tick(milliseconds);
}

/**
 * @brief OS_TimerCountReset
 * @details
 * 		reset the counter to zero.
 * @retval
 * 		return void.
 * */
void OS_TimerCountReset(void)
{
    BASIC_TimerCountReset();
}

/**
 * @brief OS_DelayS
 * @details
 * 		Wait for a specified time.The accuracy is seconds.
 * @param
 * 		seconds - how many seconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */

OS_Status_t OS_DelayS(uint32_t seconds)
{
    (void)BASIC_DelayS(seconds);
    return OS_OK;
}

/**
 * @brief OS_DelayMs
 * @details
 * 		Wait for a specified time.The accuracy is milliseconds.
 * @param
 * 		milliseconds - how many milliseconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */
OS_Status_t OS_DelayMs(uint32_t milliseconds)
{
    (void)BASIC_DelayMs(milliseconds);
    return OS_OK;
}

/**
 * @brief OS_DelayUs
 * @details
 * 		Wait for a specified time.The accuracy is microseconds.
 * @param
 * 		microseconds - how many microseconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */
OS_Status_t OS_DelayUs(uint32_t microseconds)
{
    (void)BASIC_DelayUs(microseconds);
    return OS_OK;
}

/*  ==== IRQ Management Functions ==== */

/**
 * @brief OS_RequestIrq
 * @details Interrupt processing mode is set, and the specific SOC supports different interrupt modes.
 * @param
 * 		mode- For interrupt mode, see data structure enum intr_mode.
 * @retval
 * 		0- Set successfully.
 *   	<0- Setting failed, error code returned.
 **/
int OS_IntrMode(inte_mode_t mode)
{
    return basic_intr_select_mode(mode);
}

/**
 * @brief OS_RequestIrq
 * @details Register interrupt handler for interrupt with IRQN.
 * @param irq- Interrupt number.
 * @param handler- Processing function.
 * @param type- sturct OS_RegisterType_t config irq level/model/priority
 * @retval
 * 		Returns the original interrupt handler, and returns NULL
 * 		to indicate that the interrupt handler has not been registered before.
 **/
irq_handler OS_RequestIrq(IRQn_Type irq, irq_handler handler, OS_RegisterType_t *type)
{
    return basic_intr_register_handler(irq, handler, type);
}
/**
 * @brief OS_UnRequestIrq
 * @details
 * 		If the interrupt processing function with IRQ is cancelled, the corresponding
 *   	interrupt processing function will be set as the default processing function,
 * @param irq- Interrupt number.
 * @retval
 * 		Original interrupt processing function.
 **/
irq_handler OS_UnRequestIrq(IRQn_Type irq)
{
    return basic_intr_unregister_handler(irq);
}

/**
 * @brief  Used to set the interrupt priority and interrupt level.
 *
 * All interrupts are at the same interrupt level with a total of 32 interrupt priorities by default.
 * Higher-numbered interrupt levels can preempt lower-numbered interrupt levels.
 * The Highest-priority interrupt at a given interrupt level is taken first.
 *
 *
 * @param  irq       IRQ number.
 * @param  trig_mode CLIC Trigger Enum for different Trigger Type
 * @param  lvl       interrupt level, default interrupt level 1, changes can be made through function OS_SetLevelProperty.
 * @param  priority  interrupt priority, default(0 ~ 31), changes can be made through function OS_SetLevelProperty.
 *
 * @return ** void
 */
void OS_SetIntProperty(IRQn_Type irq, CLIC_TRIGGER_Type trig_mode, irq_level_t lvl, irq_priority_t priority)
{
    uint32_t LevelMode = 0;
    uint32_t irqType   = 5;

    /* Get interrupt level and priority mode */
    LevelMode = CLIC_GetCfgNlbits();
    if (LevelMode > 5) {
        LevelMode = 5;
    }

    /* checkout lvl and priority range */
    if ((1 == LevelMode) || (2 == LevelMode) || (3 == LevelMode) || (4 == LevelMode)) {

        //checkout interrupt level
        if (lvl > (1 << LevelMode) - 1) {
            lvl = (irq_level_t)((1 << LevelMode) - 1);
        }

        //checkout interrupt priority
        if (priority > (1 << (irqType - LevelMode)) - 1) {
            priority = (irq_priority_t)((1 << (irqType - LevelMode)) - 1);
        }
    }

    /* set interrupt trigger mode and polarity */
    CLIC_SetTrigIRQ(irq, trig_mode);

    if (0 != LevelMode) {
        /* set interrupt level */
        CLIC_SetLevelIRQ(irq, lvl);
    }

    if (5 != LevelMode) {
        /* set interrupt priority */
        CLIC_SetPriorityIRQ(irq, priority);
    }
}

/**
 * @brief Used to set the interrupt level and interrupt priority.
 *
 *  There are five ways to classify interrupt levels and interrupt priorities.
 *  the default type is 0(1 interrupt level and 32 interrupt priority)
 *
 * Higher-numbered interrupt levels can preempt lower-numbered interrupt levels.
 * The Highest-priority interrupt at a given interrupt level is taken first.
 *
 * @param  irqMode
 *
 * 0: 1 interrupt level(Ignore, No configuration required),  32 interrupt priority(0 ~ 31). default
 * 1: 2 interrupt level(0 ~ 1),  16 interrupt priority(0 ~ 15).
 * 2: 4 interrupt level(0 ~ 3),  8  interrupt priority(0 ~ 7).
 * 3: 8 interrupt level(0 ~ 7),  4  interrupt priority(0 ~ 3).
 * 4: 16 interrupt level(0 ~ 15), 2 interrupt priority(0 ~ 1)
 * 5: 32 interrupt level(0 ~ 31), 1 interrupt priority(Ignore, No configuration required)
 *
 * @return ** void
 */
void OS_SetLevelProperty(irq_mode_t irqMode)
{

    uint32_t mode = (uint32_t)(irqMode);

    switch (irqMode) {
    case IRQ_MODE_0:
    case IRQ_MODE_1:
    case IRQ_MODE_2:
    case IRQ_MODE_3:
    case IRQ_MODE_4:
    case IRQ_MODE_5:
        CLIC_SetCfgNlbits(mode);
        break;
    default:
        break;
    }
}

/**
 * @brief OS_EnableInterrupt
 * @details
 * 		Enable global interrupt.
 * @retval
 * 		0- Global interrupt enabled successfully.
 *      <0- Failed to enable global interrupt, error code returned.
 **/
int OS_EnableInterrupt(void)
{
    return basic_intr_enable(1);
}

/**
 * @brief OS_DisableInterrupt
 * @details
 * 		Disable global interrupt.
 * @retval
 * 		0- Global interrupt disable successfully.
 *      <0- Failed to disable global interrupt, error code returned.
 **/
int OS_DisableInterrupt(void)
{
    return basic_intr_enable(0);
}

/**
 * @brief OS_EnableIrq
 * @details
 * 		Interrupt number corresponding to enable interrupt number IRQN.
 * @param
 * 		irq- Interrupt number.
 * @retval
 * 		0- Enable interrupt IRQN succeeded.
 *   	<0- Enable interrupt IRQN failed, return error code.
 **/
int OS_EnableIrq(IRQn_Type irq)
{
    return basic_intr_enable_irq(irq, 1);
}

/**
 * @brief OS_DisableIrq
 * @details
 * 		Interrupt number corresponding to disable interrupt number IRQN.
 * @param
 * 		irq- Interrupt number.
 * @retval
 * 		0- Disable interrupt IRQN succeeded.
 *   	<0- Disable interrupt IRQN failed, return error code.
 **/
int OS_DisableIrq(IRQn_Type irq)
{
    return basic_intr_enable_irq(irq, 0);
}

/*  ==== PMP Management Functions ==== */

/**
 * @brief OS_PMU_Add
 * @details
 * 		 Add access control
 * @param
 * 		addr- First address of address space to be restricted.
 *   	num- The number of the PMPxCFg hand PMPADDRx.
 *  	size- The size of the address space that needs to be limited. The size is 2^(size+3) byte.
 *         this parameters is only available when the mode is PMP_NAPOT is valid.
 *   	perm- Permission setting.
 *   	addr- PMP type
 * @retval
 * 		0-16- Returns the access control restriction handle.
 *   	ERROR- Error code.
 **/
int OS_PMU_Add(void *addr, uint32_t num, uint32_t size, basic_pmp_perm_t perm, basic_pmp_type_t type)
{
    return basic_pmp_add(addr, num, size, perm, type);
}

/**
 * @brief OS_PMU_Remove
 * @details
 * 		 Remove access control
 * @param
 * 		id- The ID returned when adding access control.
 * @retval
 * 		0- Access control removed successfully.
 *   	ERRNO:Error code.
 * */
int OS_PMU_Remove(int id)
{
    return basic_pmp_remove(id);
}

/*  ==== IO Functions ==== */

/**
 * @brief OS_IoRead8
 * @details
 * 		IO Read 8 bits of data.
 * @param
 * 		addr- addr to be read
 * @retval
 * 		Returns the read data
 * */
uint8_t OS_IoRead8(volatile uint8_t *addr)
{
    return readb(addr);
}

/**
 * @brief OS_IoRead16
 * @details
 * 		IO Read 16 bits of data.
 * @param
 * 		addr- addr to be read
 * @retval
 * 		Returns the read data
 * */
uint16_t OS_IoRead16(volatile uint16_t *addr)
{
    return readw(addr);
}

/**
 * @brief OS_IoRead32
 * @details
 * 		IO Read 32 bits of data.
 * @param
 * 		addr- addr to be read
 * @retval
 * 		Returns the read data
 * */
uint32_t OS_IoRead32(volatile uint32_t *addr)
{
    return readl(addr);
}

/**
 * @brief OS_IoRead64
 * @details
 * 		IO Read 64 bits of data.
 * @param
 * 		addr- addr to be read
 * @retval
 * 		Returns the read data
 * */
uint64_t OS_IoRead64(volatile uint64_t *addr)
{
    return readq(addr);
}

/**
 * @brief OS_IoWrite8
 * @details
 * 		IO Write 8 bits of data.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoWrite8(volatile uint8_t *addr, uint8_t data)
{
    writeb(data, addr);
}

/**
 * @brief OS_IoWrite16
 * @details
 * 		IO Write 16 bits of data.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoWrite16(volatile uint16_t *addr, uint16_t data)
{
    writew(data, addr);
}

/**
 * @brief OS_IoWrite32
 * @details
 * 		IO Write 32 bits of data.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoWrite32(volatile uint32_t *addr, uint32_t data)
{
    writel(data, addr);
}

/**
 * @brief OS_IoWrite64
 * @details
 * 		IO Write 64 bits of data.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoWrite64(volatile uint64_t *addr, uint64_t data)
{
    writeq(data, addr);
}

/**
 * @brief OS_IoSetBit8
 * @details
 * 		io set bit of 8 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoSetBit8(volatile uint8_t *addr, uint8_t mask)
{
    OS_IoWrite8(addr, (OS_IoRead8(addr) | mask));
}

/**
 * @brief OS_IoSetBit16
 * @details
 * 		io set bit of 16 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoSetBit16(volatile uint16_t *addr, uint16_t mask)
{
    OS_IoWrite16(addr, (OS_IoRead16(addr) | mask));
}

/**
 * @brief OS_IoSetBit32
 * @details
 * 		io set bit of 32 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoSetBit32(volatile uint32_t *addr, uint32_t mask)
{
    OS_IoWrite32(addr, (OS_IoRead32(addr) | mask));
}

/**
 * @brief OS_IoSetBit64
 * @details
 * 		io set bit of 64 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoSetBit64(volatile uint64_t *addr, uint64_t mask)
{
    OS_IoWrite64(addr, (OS_IoRead64(addr) | mask));
}

/**
 * @brief OS_IoClearBit8
 * @details
 * 		io clear bit of 8 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoClearBit8(volatile uint8_t *addr, uint8_t mask)
{
    OS_IoWrite8(addr, (OS_IoRead8(addr) & (~mask)));
}

/**
 * @brief OS_IoClearBit16
 * @details
 * 		io clear bit of 16 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoClearBit16(volatile uint16_t *addr, uint16_t mask)
{
    OS_IoWrite16(addr, (OS_IoRead16(addr) & (~mask)));
}

/**
 * @brief OS_IoClearBit32
 * @details
 * 		io clear bit of 32 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoClearBit32(volatile uint32_t *addr, uint32_t mask)
{
    OS_IoWrite32(addr, (OS_IoRead32(addr) & (~mask)));
}

/**
 * @brief OS_IoClearBit64
 * @details
 * 		io clear bit of 64 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoClearBit64(volatile uint64_t *addr, uint64_t mask)
{
    OS_IoWrite64(addr, (OS_IoRead64(addr) & (~mask)));
}

/**
 * @brief OS_IoRMW8
 * @details
 * 		8 bit clear bits and set with new value
 * @param
 * 		addr- addr to be write
 * @note It is user's responsability to make sure that value has only "mask" bits set - (value&~mask)==0
 *
 * */
void OS_IoRMW8(volatile uint8_t *addr, uint8_t mask, uint8_t value)
{
    OS_IoWrite8(addr, ((OS_IoRead8(addr) & (~mask)) | value));
}

/**
 * @brief OS_IoRMW16
 * @details
 * 		16 bit clear bits and set with new value
 * @param
 * 		addr- addr to be write
 * @note It is user's responsability to make sure that value has only "mask" bits set - (value&~mask)==0
 *
 * */
void OS_IoRMW16(volatile uint16_t *addr, uint16_t mask, uint16_t value)
{
    OS_IoWrite16(addr, ((OS_IoRead16(addr) & (~mask)) | value));
}

/**
 * @brief OS_IoRMW32
 * @details
 * 		32 bit clear bits and set with new value
 * @param
 * 		addr- addr to be write
 * @note It is user's responsability to make sure that value has only "mask" bits set - (value&~mask)==0
 *
 * */
void OS_IoRMW32(volatile uint32_t *addr, uint32_t mask, uint32_t value)
{
    OS_IoWrite32(addr, ((OS_IoRead32(addr) & (~mask)) | value));
}

/**
 * @brief OS_IoRMW64
 * @details
 * 		64 bit clear bits and set with new value
 * @param
 * 		addr- addr to be write
 * @note It is user's responsability to make sure that value has only "mask" bits set - (value&~mask)==0
 *
 * */
void OS_IoRMW64(volatile uint64_t *addr, uint64_t mask, uint64_t value)
{
    OS_IoWrite64(addr, ((OS_IoRead64(addr) & (~mask)) | value));
}

/*  ==== Cache Management Functions ==== */

/**
 * @brief OS_InvalidCache
 * @details
 * 		An invalidate operation deallocates the copy of the cache block.
 * @param
 * 		addr The address if cache block.
 * @retval
 * 		null
 * */
void OS_InvalidCache(void *addr)
{
    BASIC_InvalidCache(addr);
}

/**
 * @brief OS_FlushCache
 * @details
 * 		An flush operation deallocates the copy of the cache block.
 * @param
 * 		addr The address if cache block.
 * @retval
 * 		null
 * */

void OS_FlushCache(void *addr)
{
    BASIC_FlushCache(addr);
}
