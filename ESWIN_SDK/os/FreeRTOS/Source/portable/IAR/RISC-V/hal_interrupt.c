/*
 * Copyright 2022 Beijing ESWIN Computing Technology Co., Ltd.
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
 */
#include "platform.h"
#include "hal_interrupt.h"
#include "core_timer.h"

extern uint32_t clic_vector_table[SOC_INT_MAX];

static void interrupt_default_handler(void *data)
{
    while (1);
}

void clic_set_irq_pendings(uint32_t source)
{
    *(volatile uint8_t*)(CLIC_ADDR_BASE + CLIC_INT_IP_OFFSET + source * 1) = 1;
}

void clic_clear_irq_pendings(uint32_t source)
{
    *(volatile uint8_t*)(CLIC_ADDR_BASE + CLIC_INT_IP_OFFSET + source * 1) = 0;
}

void clic_disable_interrupt(uint32_t source)
{
    *(volatile uint8_t*)(CLIC_ADDR_BASE + CLIC_INT_IE_OFFSET + source*1) = 0;
}

void clic_enable_interrupt(uint32_t source)
{
    *(volatile uint8_t*)(CLIC_ADDR_BASE + CLIC_INT_IE_OFFSET + source*1) = 1;
}

uint8_t clic_get_clicintcfg(uint32_t source)
{
    return *(volatile uint8_t*)(CLIC_ADDR_BASE + CLIC_INT_CFG_OFFSET + source*1);
}

void clic_set_clicintcfg(uint32_t source, uint8_t intctrl)
{
    *(volatile uint8_t*)(CLIC_ADDR_BASE + CLIC_INT_CFG_OFFSET + source*1) = intctrl;
}

uint8_t clic_get_cliccfg(void)
{
    return *(volatile uint8_t*)(CLIC_ADDR_BASE + CLIC_CFG_OFFSET);
}

void clic_set_cliccfg(uint8_t cliccfg)
{
    *(volatile uint8_t*)(CLIC_ADDR_BASE + CLIC_CFG_OFFSET) = cliccfg;
}

uint8_t clic_get_clicintctrl(uint32_t source)
{
    uint8_t int_cfg_value = clic_get_clicintcfg(source);
    return (int_cfg_value & CLIC_INT_CFG_CTRL_MASK) >> CLIC_INT_CFG_CTRL_LSB;
}

void clic_set_clicintctrl(uint32_t source, uint8_t intctrl)
{
    uint8_t old_int_cfg_value = clic_get_clicintcfg(source);
    uint8_t new_int_cfg_value = (old_int_cfg_value & (~CLIC_INT_CFG_CTRL_MASK)) \
		      | ((uint8_t)(intctrl << CLIC_INT_CFG_CTRL_LSB) & CLIC_INT_CFG_CTRL_MASK);
    clic_set_clicintcfg(source, new_int_cfg_value);
}

//get nlbits
uint8_t clic_get_nlbits(void) {
  //extract nlbits
  uint8_t nlbits = clic_get_cliccfg();
  nlbits = (nlbits & CLIC_CFG_NLBITS_MASK) >> CLIC_CFG_NLBITS_LSB;
  return nlbits;
}

//sets level File name cliccfg nlbits
void clic_set_nlbits(uint8_t nlbits) {
  //shift nlbits to correct position
  uint8_t nlbits_shifted = nlbits << CLIC_CFG_NLBITS_LSB;

  //read the current cliccfg
  uint8_t old_cliccfg = clic_get_cliccfg();
  uint8_t new_cliccfg = (old_cliccfg & (~CLIC_CFG_NLBITS_MASK)) | (CLIC_CFG_NLBITS_MASK & nlbits_shifted);

  clic_set_cliccfg(new_cliccfg);
}

/* set interrupt select hardware vector mode File name clicintcfg clicintcfgpad */
/*shv: 1: hardware vectoring is enabled 0:no-vectored*/
void clic_set_irq_shv(IRQn_Type irq, uint8_t shv)
{
    //read the current clicintcfg
    uint8_t old_clicintcfg = clic_get_clicintcfg(irq);
    uint8_t new_clicintcfg = (old_clicintcfg & (~CLIC_INT_CFG_SHV_MASK)) | (shv & CLIC_INT_CFG_SHV_MASK);

    clic_set_clicintcfg(irq, new_clicintcfg);
}

void clic_set_iqr_level(IRQn_Type irq, uint8_t lvl_abs)
{
    uint8_t nlbits = clic_get_nlbits();
    uint8_t intctlbits = 7;
    uint8_t ctrval = clic_get_clicintctrl(irq);
    uint8_t maxlvl = 0;

    if (nlbits == 0) {
        return;
    }

    if (nlbits > intctlbits) {
        nlbits = intctlbits;
    }

    maxlvl = ((1 << nlbits) - 1);
    if (lvl_abs > maxlvl) {
        lvl_abs = maxlvl;
    }

    ctrval &= ~(maxlvl << (CLIC_MAX_NLBITS - nlbits));
    ctrval |= (lvl_abs << (CLIC_MAX_NLBITS - nlbits));
    clic_set_clicintctrl(irq, ctrval);
}

void clic_set_irq_priority(IRQn_Type irq, uint8_t pri)
{
    uint8_t nlbits = clic_get_nlbits();
    uint8_t intctlbits = 7;
    uint8_t ctrval = clic_get_clicintctrl(irq);
    uint8_t npbits = 0, maxPrio = 0;

    if (nlbits >= intctlbits) {
        npbits = 0;
    } else {
        npbits = intctlbits - nlbits;
    }

    if (npbits == 0) {
        return;
    }

    maxPrio =((1 << npbits) - 1);
    if (pri > maxPrio) {
        pri = maxPrio;
    }

    ctrval &= ~maxPrio;
    ctrval |= pri;
    clic_set_clicintctrl(irq, ctrval);
}

/****************************************************************************
 * Name: clic_interrupt_enable
 *
 * Description:
 *   Register interrupt handler for interrupt with IRQN.
 *
 * Input Parameters:
 *   irq- Interrupt number.
 *   shv- CLIC mode.
 *   lvl- CLIC level.
 *   priority- CLIC priority
 *
 * Returned Value:
 *   0- Enable  succeeded.
 *   <0- Enable failed, return error code.
 *
 ****************************************************************************/
int clic_interrupt_enable(IRQn_Type irq, uint8_t shv, uint8_t lvl, uint8_t priority)
{

    if ((irq > SOC_INT_MAX) || (shv > CLIC_VECTORED_INTERRUPT))
        return -1;

    /* set interrupt vector mode */
    clic_set_irq_shv(irq, shv);
    /* set interrupt level */
    clic_set_iqr_level(irq, lvl);
    /* set interrupt priority */
    clic_set_irq_priority(irq, priority);
    /* enable interrupt */
    clic_enable_interrupt (irq);

    return 0;
}

/****************************************************************************
 * Name: hal_systick_config
 *
 * Description:
 *   Systerm tick config.
 *
 * Input Parameters:
 *   ticks.
 *
 * Returned Value:
 *   0- Enable config succeeded.
 *   <0- config failed, return error code.
 *
 ****************************************************************************/
uint32_t hal_systick_config(uint64_t ticks)
{
    uint64_t current_timer_value = 0;
    
    clic_disable_interrupt(Timer_IRQn);
    systimer_set_loadvalue(0);
    current_timer_value = systimer_get_loadvalue();
    systimer_set_comparevalue(current_timer_value + ticks);
    
    clic_set_irq_shv(Timer_IRQn, CLIC_VECTORED_INTERRUPT);
    clic_set_iqr_level(Timer_IRQn, 1);
    clic_set_irq_priority(Timer_IRQn, 0);
    clic_enable_interrupt(Timer_IRQn);
    
    return 0UL;   
}

/****************************************************************************
 * Name: hal_intr_enable
 *
 * Description:
 *   Enable disable global interrupt.
 *
 * Input Parameters:
 *   enable- True: enable global interrupt; False: disable global interrupt.
 *
 * Returned Value:
 *   0- Global interrupt enabled successfully.
 *   <0- Failed to enable global interrupt, error code returned.
 *
 ****************************************************************************/
int hal_intr_enable(int enable)
{
    if ((enable < 0) || (enable > 1))
        return -1;

    if (enable == 1)
        __set_bits_csr(_CSR_MSTATUS, MSTATUS_MIE);
    else
        __clear_bits_csr(_CSR_MSTATUS, MSTATUS_MIE);

    return 0;
}

/****************************************************************************
 * Name:  hal_intr_select_mode
 *
 * Description:
 *   Interrupt processing mode is set, and the specific SOC supports different interrupt modes.
 *
 * Input Parameters:
 *   mode- For interrupt mode, see data structure enum intr_mode.
 *
 * Returned Value:
 *   0- Set successfully.
 *   <0- Setting failed, error code returned.
 *
 ****************************************************************************/
int hal_intr_select_mode(inte_mode_t mode)
{
    if ((mode < 0) || (mode > 3))
        return -1;

    mode &= MTVEC_MODE_Msk;
    __clear_bits_csr(_CSR_MTVEC, MTVEC_MODE_Msk);
    __set_bits_csr(_CSR_MTVEC, mode);

    return 0;
}

/****************************************************************************
 * Name:  hal_intr_get_mode
 *
 * Description:
 *   Acquire interrupt processing mode.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Interrupt mode.
 *
 ****************************************************************************/
int hal_intr_get_mode(void)
{
    return __read_csr(_CSR_MTVEC) & MTVEC_MODE_Msk;
}

/****************************************************************************
 * Name: hal_intr_register_handler
 *
 * Description:
 *   Register interrupt handler for interrupt with IRQN.
 *
 * Input Parameters:
 *   irq- Interrupt number.
 *   handler- Processing function.
 *
 * Returned Value:
 *   Returns the original interrupt handler, and returns NULL
 *   to indicate that the interrupt handler has not been registered before.
 *
 ****************************************************************************/
irq_handler hal_intr_register_handler(IRQn_Type irq, irq_handler handler, register_type_t *type)
{
    uint8_t re_value;
    uint8_t intr_shv;

    if ((irq < Reserved0_IRQn) || (irq > SOC_INT_MAX))
        return NULL;

    clic_vector_table[irq] = (uint32_t)handler;

    re_value = hal_intr_get_mode();

    if (INTR_MODE_CLIC_DIRECT == re_value)
        intr_shv = CLIC_DIRECT_INTERRUPT;
    else
        intr_shv = CLIC_VECTORED_INTERRUPT;
    clic_interrupt_enable(irq, intr_shv, type->lvl, type->priority);

    return handler;
}

/****************************************************************************
 * Name: hal_intr_unregister_handler
 *
 * Description:
 *   If the interrupt processing function with IRQ is cancelled, the corresponding
 *   interrupt processing function will be set as the default processing function,
 *
 * Input Parameters:
 *   irq- Interrupt number.
 *
 * Returned Value:
 *   Original interrupt processing function.
 *
 ****************************************************************************/
irq_handler hal_intr_unregister_handler(IRQn_Type irq)
{
    irq_handler re_handle;

    if ((irq < Reserved0_IRQn) || (irq > SOC_INT_MAX))
        return NULL;

    re_handle = (irq_handler)clic_vector_table[irq];
    clic_vector_table[irq] = (uint32_t)interrupt_default_handler;
    hal_intr_enable_irq(irq, 0);

    return re_handle;
}

/****************************************************************************
 * Name: hal_intr_enable_irq
 *
 * Description:
 *   Interrupt number corresponding to enable interrupt number IRQN.
 *
 * Input Parameters:
 *   irq- Interrupt number.
 *   enable- True: enable interrupt. False: disable interrupt.
 *
 * Returned Value:
 *   0- Enable interrupt IRQN succeeded.
 *   <0- Enable interrupt IRQN failed, return error code.
 *
 ****************************************************************************/
int hal_intr_enable_irq(IRQn_Type irq, int enable)
{
    uint8_t re_value;

    if ((irq < Reserved0_IRQn) || (irq > SOC_INT_MAX))
        return -1;

    if ((enable < 0) || (enable > 1))
        return -1;

    re_value = hal_intr_get_mode();
    if ((INTR_MODE_CLINT_DIRECT == re_value) || (INTR_MODE_CLINT_VECTOR == re_value)) {
        if (enable == 0) {
            switch (irq) {
                case Software_IRQn:
                    __clear_bits_csr(_CSR_MIE, MIE_MSIE);
                    break;
                case Timer_IRQn:
                    __clear_bits_csr(_CSR_MIE, MIE_MTIE);
                    break;
                case External_IRQn:
                    __clear_bits_csr(_CSR_MIE, MIE_MEIE);
                    break;
                default:
                    return -1;
            }
        } else {
            switch (irq) {
                case Software_IRQn:
                    __set_bits_csr(_CSR_MIE, MIE_MSIE);
                    break;
                case Timer_IRQn:
                    __set_bits_csr(_CSR_MIE, MIE_MTIE);
                    break;
                case External_IRQn:
                    __set_bits_csr(_CSR_MIE, MIE_MEIE);
                    break;
                default:
                    return -1;
            }
        }
    } else {
        if (enable == 0)
        	clic_disable_interrupt(irq);
        else
        	clic_disable_interrupt(irq);
    }

    return 0;
}

