/*
 * Copyright (c) 2020 ESWIN Limited. All rights reserved.
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
 */

#ifndef __CORE_FEATURE_CACHE_H__
#define __CORE_FEATURE_CACHE_H__
/**
 * @file     core_feature_cache.h
 * @brief    Cache feature API header file for Eswin Core
 */
/*
 * Cache Feature Configuration Macro:
 * 1. __ICACHE_PRESENT:  Define whether I-Cache Unit is present or not.
 *   * 0: Not present
 *   * 1: Present
 * 1. __DCACHE_PRESENT:  Define whether D-Cache Unit is present or not.
 *   * 0: Not present
 *   * 1: Present
 */

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_RV_CACHE_PRESENT 1
#if defined(CONFIG_RV_CACHE_PRESENT) && (CONFIG_RV_CACHE_PRESENT == 1)
// #if defined(CONFIG_RV_CACHE_PRESENT) && (CONFIG_RV_CACHE_PRESENT == 1)
#define MENVCFG_CBZE                (1<<7)
#define MENVCFG_CBCFE               (1<<6)
#define MENVCFG_CBIE(val)           (val << 4)
#define MENVCFG_INVAL               MENVCFG_CBIE(0x03)
#define MENVCFG_FLUSH               MENVCFG_CBCFE
#define MENVCFG_CLEAN               MENVCFG_CBCFE
/* ##########################  Cache functions  #################################### */
/**
 * \defgroup EMSIS_Core_Cache       Cache Functions
 * \brief    Functions that configure Instruction and Data Cache.
 * @{
 */

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_Cache */

/**
 * \defgroup EMSIS_Core_CMO      CMO Functions
 * \ingroup  EMSIS_Core_Cache
 * \brief    Functions that configure Instruction CMO.
 * @{
 */
#define __CACHE_PRESENT 1
/**
 * \brief   Set CMO register.
 * \details
 * config CMO register.
 * \param [in]	cmoccfg The set config of MENVCFG.
 * \return	null
 */
__STATIC_FORCEINLINE void __set_CMO(rv_csr_t cmocfg)
{
    __RV_CSR_SET(CSR_MENVCFG, cmocfg);
}

/**
 * \brief   Clear CMO register.
 * \details
 * config CMO register.
 * \param [in]  cmoccfg The clear config of MENVCFG.
 * \return  null
 */
__STATIC_FORCEINLINE void __clear_CMO(rv_csr_t cmocfg)
{
    __RV_CSR_CLEAR(CSR_MENVCFG, cmocfg);
}

/**
 * \brief   Set CMO register.
 * \details
 * config CMO register.
 * \param [in]	null.
 * \return	The value of MENVCFG.
 */
__STATIC_FORCEINLINE rv_csr_t __get_CMO(void)
{
    return __RV_CSR_READ(CSR_MENVCFG);
}

/**
 * \brief   Set Cache Inval.
 * \details
 * An invalidate operation deallocates the copy of the cache block.
 * \param [in]	addr The address if cache block.
 * \return	null
 */
#if defined ( __GNUC__ )
#define __cmo_INVAL(addr)                               \
    ({                                                  \
        register rv_csr_t __addr = (rv_csr_t)(addr);    \
        __ASM volatile("cbo.inval %0"                   \
                    :                                   \
                    : "r"(__addr)                       \
                    : "memory");                        \
    })
#else
#define __cmo_INVAL(...)
#endif

/**
 * \brief   Set Cache Clean.
 * \details
 * An clean operation deallocates the copy of the cache block.
 * \param [in]	addr The address if cache block.
 * \return	null
 */
#if defined ( __GNUC__ )
#define __cmo_CLEAN(addr)                               \
    ({                                                  \
        register rv_csr_t __addr = (rv_csr_t)(addr);    \
        __ASM volatile("cbo.clean %0"                   \
                    :                                   \
                    : "r"(__addr)                       \
                    : "memory");                        \
    })
#else
#define __cmo_CLEAN(...)
#endif

/**
 * \brief   Dirty Cache Clean.
 * \details
 * An clean operation clean all dirty cache lines.
 * \param [in]	null
 * \return	null
 */
#if defined ( __GNUC__ )
#define __cmo_CLEANALL()                                \
    ({                                                  \
        __ASM volatile("dc_clean_all"                   \
                    ::: "memory");                      \
    })
#else
#define __cmo_CLEANALL(...)
#endif

/**
 * \brief  Set Cache Flush.
 * \details
 * An flush operation deallocates the copy of the cache block.
 * \param [in]	addr The address if cache block.
 * \return	null
 */
#if defined ( __GNUC__ )
#define __cmo_FLUSH(addr)                               \
    ({                                                  \
        register rv_csr_t __addr = (rv_csr_t)(addr);    \
        __ASM volatile("cbo.flush %0"                   \
                    :                                   \
                    : "r"(__addr)                       \
                    : "memory");                        \
    })
#else
#define __cmo_FLUSH(...)
#endif

/**
 * \brief   Set Cache Zero.
 * \details
 * An zero operation deallocates the copy of the cache block.
 * \param [in]	addr The address if cache block.
 * \return 	null
 */
#if defined ( __GNUC__ )
#define __cmo_ZERO(addr)                                \
    ({                                                  \
        register rv_csr_t __addr = (rv_csr_t)(addr);    \
        __ASM volatile("cbo.zero %0"                    \
                    :                                   \
                    : "r"(__addr)                       \
                    : "memory");                        \
    })
#else
#define __cmo_ZERO(...)
#endif

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_CMO */

/**
 * \defgroup EMSIS_Core_ICache      I-Cache Functions
 * \ingroup  EMSIS_Core_Cache
 * \brief    Functions that configure Instruction Cache.
 * @{
 */
/**
 * \brief  Enable ICache
 * \details
 * This function enable I-Cache
 * \remarks
 * - register control I Cache enable.
 * \sa
 * - \ref DisableICache
 * 
 */
__STATIC_FORCEINLINE void EnableICache(void)
{
}

/**
 * \brief  Disable ICache
 * \details
 * This function Disable I-Cache
 * \remarks
 * - register control I Cache enable.
 * \sa
 * - \ref EnableICache
 */
__STATIC_FORCEINLINE void DisableICache(void)
{
}
/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_ICache */
#endif /* defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1) */

/**
 * \defgroup EMSIS_Core_DCache      D-Cache Functions
 * \ingroup  EMSIS_Core_Cache
 * \brief    Functions that configure Data Cache.
 * @{
 */
/**
 * \brief  Enable DCache
 * \details
 * This function enable D-Cache
 * \remarks
 * - register control D Cache enable.
 * \sa
 * - \ref DisableDCache
 */
__STATIC_FORCEINLINE void EnableDCache(void)
{
}

/**
 * \brief  Disable DCache
 * \details
 * This function Disable D-Cache
 * \remarks
 * - register control D Cache enable.
 * \sa
 * - \ref EnableDCache
 */
__STATIC_FORCEINLINE void DisableDCache(void)
{
}
/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_DCache */

#ifdef __cplusplus
}
#endif
#endif /** __CORE_FEATURE_CACHE_H__ */
