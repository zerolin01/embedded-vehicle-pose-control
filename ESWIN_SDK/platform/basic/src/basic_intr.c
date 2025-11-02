/*
 * Copyright (c) 2013-2020 Arm Limited. All rights reserved.
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
 * ----------------------------------------------------------------------
 *
 * $Date:        19. July 2021
 * $Revision:    V0.0.0
 *
 * Project:      EMSIS-BASIC API
 * Title:        basic_intr.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"
#include <stdlib.h>

extern uint32_t clic_vector_table[SOC_INT_MAX];

/*--------------------------------------------------------------------------*/
/* Interrupt */
#if defined(CONFIG_RV_CORE_NUM) && (CONFIG_RV_CORE_NUM > 1)
kitty_interrupt_handler_table_t *_kitty_interrupt_handler_table;
kitty_interrupt_handler_data_ptr_table_t *_kitty_interrupt_handler_data_ptr_table;
#else
kitty_interrupt_handler_table_t _kitty_interrupt_handler_table;
kitty_interrupt_handler_data_ptr_table_t _kitty_interrupt_handler_data_ptr_table;
#endif

static void _kitty_interrupt_default_handler(void *data)
{
    while (1)
        ;
}

__WEAK void _clic_msip_handler(void *data)
{
}
__WEAK void _clic_mtip_handler(void *data)
{
}
__WEAK void _clic_meip_handler(void *data)
{
}
__WEAK void _clic_csip_handler(void *data)
{
}

static int clic_interrupt_enable(IRQn_Type IRQn, uint8_t shv, CLIC_TRIGGER_Type trig_mode, uint8_t lvl,
                                 uint8_t priority)
{
    if (trig_mode != CLIC_LEVEL_TRIGGER)
        return -BASICEINVAL;

    if ((IRQn > SOC_INT_MAX) || (shv > CLIC_VECTORED_INTERRUPT))
        return -BASICEINVAL;

    /* set interrupt vector mode */
    CLIC_SetShvIRQ(IRQn, shv);
    /* set interrupt trigger mode and polarity */
    CLIC_SetTrigIRQ(IRQn, trig_mode);
    /* set interrupt level */
    CLIC_SetLevelIRQ(IRQn, lvl);
    /* set interrupt priority */
    CLIC_SetPriorityIRQ(IRQn, priority);
    /* enable interrupt */
    CLIC_EnableIRQ(IRQn);

    return 0;
}

/****************************************************************************
 * Name: basic_intr_enable
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
int basic_intr_enable(int enable)
{
    if ((enable < 0) || (enable > 1))
        return -BASICEINVAL;

    if (enable == 1)
        __RV_CSR_SET(CSR_MSTATUS, MSTATUS_MIE);
    else
        __RV_CSR_CLEAR(CSR_MSTATUS, MSTATUS_MIE);

    return 0;
}

/****************************************************************************
 * Name:  basic_intr_select_mode
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
int basic_intr_select_mode(inte_mode_t mode)
{
    if ((mode < 0) || (mode > 3))
        return -BASICEINVAL;

    mode &= MTVEC_MODE_Msk;
    __RV_CSR_CLEAR(CSR_MTVEC, MTVEC_MODE_Msk);
    __RV_CSR_SET(CSR_MTVEC, mode);

    return 0;
}

/****************************************************************************
 * Name:  basic_intr_get_mode
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
int basic_intr_get_mode(void)
{
    return __RV_CSR_READ(CSR_MTVEC) & MTVEC_MODE_Msk;
}

/****************************************************************************
 * Name: basic_intr_register_handler
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
irq_handler basic_intr_register_handler(IRQn_Type irq, irq_handler handler, register_type_t *type)
{
    BASIC_ASSERT(irq);
    BASIC_ASSERT(handler);
    // BASIC_ASSERT(type);

    uint8_t re_value;
    uint8_t intr_shv;

    if ((irq < Reserved0_IRQn) || (irq > SOC_INT_MAX))
        return NULL;

#if defined(CONFIG_RV_CORE_NUM) && (CONFIG_RV_CORE_NUM > 1)
    _kitty_interrupt_handler_table[basic_get_hartid()][irq]          = handler;
    _kitty_interrupt_handler_data_ptr_table[basic_get_hartid()][irq] = type->data_ptr;
#else
    _kitty_interrupt_handler_table[irq]          = handler;
    _kitty_interrupt_handler_data_ptr_table[irq] = type->data_ptr;
#endif

#if defined(CONFIG_RV_CLIC_PRESENT) && (CONFIG_RV_CLIC_PRESENT == 1)
    re_value = basic_intr_get_mode();

    if (INTR_MODE_CLIC_DIRECT == re_value)
        intr_shv = CLIC_DIRECT_INTERRUPT;
    else
        intr_shv = CLIC_VECTORED_INTERRUPT;
    clic_interrupt_enable(irq, intr_shv, type->trig_mode, type->lvl, type->priority);
#endif
    return handler;
}

/****************************************************************************
 * Name: basic_intr_unregister_handler
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
irq_handler basic_intr_unregister_handler(IRQn_Type irq)
{
    irq_handler re_handle;
    void *data_ptr = NULL;

    BASIC_ASSERT(irq);

    if ((irq < Reserved0_IRQn) || (irq > SOC_INT_MAX))
        return NULL;

#if defined(CONFIG_RV_CORE_NUM) && (CONFIG_RV_CORE_NUM > 1)
    re_handle                                               = _kitty_interrupt_handler_table[basic_get_hartid()][irq];
    _kitty_interrupt_handler_table[basic_get_hartid()][irq] = _kitty_interrupt_default_handler;
    data_ptr = _kitty_interrupt_handler_data_ptr_table[basic_get_hartid()][irq];
    basic_intr_enable_irq(irq, 0);
#else
    re_handle                                    = _kitty_interrupt_handler_table[irq];
    _kitty_interrupt_handler_table[irq]          = _kitty_interrupt_default_handler;
    data_ptr                                     = _kitty_interrupt_handler_data_ptr_table[irq];
    basic_intr_enable_irq(irq, 0);
#endif
    if (data_ptr != NULL) {
        // free(data_ptr);
        // data_ptr = NULL;
        _kitty_interrupt_handler_data_ptr_table[irq] = NULL;
    }

    return re_handle;
}

/****************************************************************************
 * Name: basic_intr_enable_irq
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
int basic_intr_enable_irq(IRQn_Type irq, int enable)
{
    uint8_t re_value;

    BASIC_ASSERT(irq);

    if ((irq < Reserved0_IRQn) || (irq > SOC_INT_MAX))
        return -BASICEINVAL;

    if ((enable < 0) || (enable > 1))
        return -BASICEINVAL;

    re_value = basic_intr_get_mode();
    if ((INTR_MODE_CLINT_DIRECT == re_value) || (INTR_MODE_CLINT_VECTOR == re_value)) {
        if (enable == 0) {
            switch (irq) {
            case Software_IRQn:
                __RV_CSR_CLEAR(CSR_MIE, MIE_MSIE);
                break;
            case Timer_IRQn:
                __RV_CSR_CLEAR(CSR_MIE, MIE_MTIE);
                break;
            case External_IRQn:
                __RV_CSR_CLEAR(CSR_MIE, MIE_MEIE);
                break;
            default:
                return -BASICEINVAL;
            }
        } else {
            switch (irq) {
            case Software_IRQn:
                __RV_CSR_SET(CSR_MIE, MIE_MSIE);
                break;
            case Timer_IRQn:
                __RV_CSR_SET(CSR_MIE, MIE_MTIE);
                break;
            case External_IRQn:
                __RV_CSR_SET(CSR_MIE, MIE_MEIE);
                break;
            default:
                return -BASICEINVAL;
            }
        }
    } else {
        if (enable == 0)
            CLIC_SET_OP(__CLIC_DisableIRQ, irq);
        else
            CLIC_SET_OP(__CLIC_EnableIRQ, irq);
    }

    return 0;
}

#if defined(CONFIG_RV_CORE_NUM) && (CONFIG_RV_CORE_NUM > 1)
#define CALL_HANDLER(IRQn)                                                                                             \
    do {                                                                                                               \
        irq_handler handler = _kitty_interrupt_handler_table[basic_get_hartid()][IRQn];                                \
        void *data_ptr      = _kitty_interrupt_handler_data_ptr_table[basic_get_hartid()][IRQn];                       \
        handler(data_ptr);                                                                                             \
    } while (0)
#else /** defined(CONFIG_RV_CORE_NUM) && (CONFIG_RV_CORE_NUM > 1) */
#define CALL_HANDLER(IRQn)                                                                                             \
    do {                                                                                                               \
        irq_handler handler = _kitty_interrupt_handler_table[IRQn];                                                    \
        void *data_ptr      = _kitty_interrupt_handler_data_ptr_table[IRQn];                                           \
        handler(data_ptr);                                                                                             \
    } while (0)
#endif

/****************************************************************************
 * Name: clic_msip_handler
 *
 * Description:
 *   Software interrupt processing function in Clic mode.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
__WEAK __INTERRUPT void clic_msip_handler(void)
{
    __attribute__((unused)) struct _reent *local;

    SAVE_REENT_STRUCT(local);
    SAVE_IRQ_CSR_CONTEXT();

    CALL_HANDLER(Software_IRQn);

    RESTORE_IRQ_CSR_CONTEXT();
    RESTORE_REENT_STRUCT(local);
}

/****************************************************************************
 * Name: clic_mtip_handler
 *
 * Description:
 *   Timer interrupt processing function in Clic mode.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
__WEAK __INTERRUPT void clic_mtip_handler(void)
{
    __attribute__((unused)) struct _reent *local;

    SAVE_REENT_STRUCT(local);
    SAVE_IRQ_CSR_CONTEXT();

    CALL_HANDLER(Timer_IRQn);

    RESTORE_IRQ_CSR_CONTEXT();
    RESTORE_REENT_STRUCT(local);
}

/****************************************************************************
 * Name:  clic_meip_handler
 *
 * Description:
 *   External interrupt processing function in Clic mode.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
__WEAK __INTERRUPT void clic_meip_handler(void)
{
    __attribute__((unused)) struct _reent *local;

    SAVE_REENT_STRUCT(local);
    SAVE_IRQ_CSR_CONTEXT();

    CALL_HANDLER(External_IRQn);

    RESTORE_IRQ_CSR_CONTEXT();
    RESTORE_REENT_STRUCT(local);
}

/****************************************************************************
 * Name: clic_csip_handler
 *
 * Description:
 *   Clic timer interrupt processing function in Clic mode.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
__WEAK __INTERRUPT void clic_csip_handler(void)
{
    __attribute__((unused)) struct _reent *local;

    SAVE_REENT_STRUCT(local);
    SAVE_IRQ_CSR_CONTEXT();

    CALL_HANDLER(ClicSoftware_IRQn);

    RESTORE_IRQ_CSR_CONTEXT();
    RESTORE_REENT_STRUCT(local);
}

/****************************************************************************
 * Name: clic_default_intexc_handler
 *
 * Description:
 *   Clic timer interrupt processing function in Clic mode.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
__INTERRUPT void clic_default_intexc_handler(void)
{
    __attribute__((unused)) struct _reent *local;

    SAVE_REENT_STRUCT(local);
    SAVE_IRQ_CSR_CONTEXT();

    CALL_HANDLER(__mcause & 0x00000FFF);

    RESTORE_IRQ_CSR_CONTEXT();
    RESTORE_REENT_STRUCT(local);
}

/****************************************************************************
 * Name: clint_msip_handler
 *
 * Description:
 *   Software interrupt processing function in Clint mode.
 *
 * Input Parameters:
 *   NOne.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
__WEAK __INTERRUPT void clint_msip_handler(void)
{
    __attribute__((unused)) struct _reent *local;

    SAVE_REENT_STRUCT(local);
    SAVE_IRQ_CSR_CONTEXT();

    CALL_HANDLER(Software_IRQn);

    RESTORE_IRQ_CSR_CONTEXT();
    RESTORE_REENT_STRUCT(local);
}

/****************************************************************************
 * Name: clint_mtip_handler
 *
 * Description:
 *   Timer interrupt processing function in Clint mode
 *
 * Input Parameters:
 *   NOne.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
__WEAK __INTERRUPT void clint_mtip_handler(void)
{
    __attribute__((unused)) struct _reent *local;

    SAVE_REENT_STRUCT(local);
    SAVE_IRQ_CSR_CONTEXT();

    CALL_HANDLER(Timer_IRQn);

    RESTORE_IRQ_CSR_CONTEXT();
    RESTORE_REENT_STRUCT(local);
}

/****************************************************************************
 * Name: clint_meip_handler
 *
 * Description:
 *   External interrupt processing function in Clint mode.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
__WEAK __INTERRUPT void clint_meip_handler(void)
{
    __attribute__((unused)) struct _reent *local;

    SAVE_REENT_STRUCT(local);
    SAVE_IRQ_CSR_CONTEXT();

    CALL_HANDLER(External_IRQn);

    RESTORE_IRQ_CSR_CONTEXT();
    RESTORE_REENT_STRUCT(local);
}

/****************************************************************************
 * Name: nmi_handler
 *
 * Description:
 *   NMI interrupt handling function.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
__WEAK __INTERRUPT void nmi_handler(void)
{
}

/****************************************************************************
 * Name: basic_intr_init
 *
 * Description:
 *   Initialization interrupt subsystem, including interrupt controller and software
 *   interrupt processing subsystem.
 *   Direct interrupt processing mode is used by default, and each interrupt default
 *   processing function is the default processing function.
 *   If you need to use other interrupt processing modes, please use basic_intr_select_mode.
 *   If you need to modify the interrupt handling function, please use basic_intr_register_handler
 *   function.
 *
 *   Input Parameters:
 *   None.
 *
 * Returned Value:
 *   0- Normal initialization return;
 *   <0- Initialization failed, error code returned.
 *
 ****************************************************************************/
int basic_intr_init(void)
{
    uint32_t num;

#if defined(CONFIG_RV_CORE_NUM) && (CONFIG_RV_CORE_NUM > 1)
    uint32_t coreNum;

    for (coreNum = 0; coreNum < CONFIG_RV_CORE_NUM; coreNum++) {
        for (num = 0; num < SOC_INT_MAX; num++) {
            _kitty_interrupt_handler_table[coreNum][num]          = _kitty_interrupt_default_handler;
            _kitty_interrupt_handler_data_ptr_table[coreNum][num] = NULL;
        }
    }

    for (coreNum = 0; coreNum < CONFIG_RV_CORE_NUM; coreNum++) {
        /* set default handler for msip, meip, mtip and csip */
        _kitty_interrupt_handler_table[coreNum][Software_IRQn]     = _clic_msip_handler;
        _kitty_interrupt_handler_table[coreNum][Timer_IRQn]        = _clic_mtip_handler;
        _kitty_interrupt_handler_table[coreNum][External_IRQn]     = _clic_meip_handler;
        _kitty_interrupt_handler_table[coreNum][ClicSoftware_IRQn] = _clic_csip_handler;
    }

#else
    for (int i = 17; i < SOC_INT_MAX; i++) {
        clic_vector_table[i] = (uint32_t)clic_default_intexc_handler;
    }

    for (num = 0; num < SOC_INT_MAX; num++) {
        _kitty_interrupt_handler_table[num]          = _kitty_interrupt_default_handler;
        _kitty_interrupt_handler_data_ptr_table[num] = NULL;
        CLIC_ClearPendingIRQ((IRQn_Type)num);
        CLIC_DisableIRQ((IRQn_Type)num);
    }

    /* set default handler for msip, meip, mtip and csip */
    _kitty_interrupt_handler_table[Software_IRQn]     = _clic_msip_handler;
    _kitty_interrupt_handler_table[Timer_IRQn]        = _clic_mtip_handler;
    _kitty_interrupt_handler_table[External_IRQn]     = _clic_meip_handler;
    _kitty_interrupt_handler_table[ClicSoftware_IRQn] = _clic_csip_handler;
#endif

#if defined(CONFIG_RV_CLIC_PRESENT) && (CONFIG_RV_CLIC_PRESENT == 1)
    basic_intr_select_mode(INTR_MODE_CLIC_VECTOR);
#else
    basic_intr_select_mode(INTR_MODE_CLINT_VECTOR);
#endif

    return 0;
}
