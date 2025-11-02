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
 */
/******************************************************************************
 * @file     gcc_e320_ddr.ld
 * @brief    GNU Linker Script for ESWIN e320 based device
 * @version  V0.1.0
 * @date     17. Jan 2021
 ******************************************************************************/
#include <e320.h>

OUTPUT_ARCH( "riscv" )
ENTRY( _start )

_STACK_SIZE = DEFINED(_STACK_SIZE) ? _STACK_SIZE : 4k;
_HEAP_SIZE = DEFINED(_HEAP_SIZE) ? _HEAP_SIZE : 1k;
_APOOL_SIZE = DEFINED(_APOOL_SIZE) ? _APOOL_SIZE : 128;

MEMORY
{
  itim (wxa!ri) : ORIGIN = TIM_1_BASEADDR, LENGTH = TIM_1_SIZE
  ocm (wxa!ri) : ORIGIN = OCM_DCACHE_BASEADDR, LENGTH = OCM_DCACHE_SIZE
  pflash_icache (wxa!ri) : ORIGIN = PFLASH_ICACHE_BASEADDR, LENGTH = PFLASH_ICACHE_SIZE
  dflash_dcache (wxa!ri) : ORIGIN = DFLASH_DCACHE_BASEADDR, LENGTH = DFLASH_DCACHE_SIZE
}

#if defined EMPS_COREMARK_CASE0 /* TXT(TIM),RODATA(TIM), DATA-BSS-STACK(OCM) */
  #define TXT_LMA     itim
  #define TXT_VMA     itim
  #define RODATA_LMA  itim
  #define RODATA_VMA  itim
  #define DATA_LMA    ocm
  #define DATA_VMA    ocm
#elif defined EMPS_COREMARK_CASE1 /* TXT(TIM),RODATA(OCM), DATA-BSS-STACK(OCM) */
  #define TXT_LMA     itim
  #define TXT_VMA     itim
  #define RODATA_LMA  ocm
  #define RODATA_VMA  ocm
  #define DATA_LMA    ocm
  #define DATA_VMA    ocm
#elif defined EMPS_COREMARK_CASE2 /* TXT(FLASH),RODATA(FLASH), DATA-BSS-STACK(TIM) */
  #define TXT_LMA     pflash_icache
  #define TXT_VMA     pflash_icache
  #define RODATA_LMA  pflash_icache
  #define RODATA_VMA  pflash_icache
  #define DATA_LMA    itim
  #define DATA_VMA    itim
#elif defined EMPS_COREMARK_CASE3 /* TXT(FLASH),RODATA(TIM), DATA-BSS-STACK(TIM) */
  #define TXT_LMA     pflash_icache
  #define TXT_VMA     pflash_icache
  #define RODATA_LMA  itim
  #define RODATA_VMA  itim
  #define DATA_LMA    itim
  #define DATA_VMA    itim
#elif defined EMPS_COREMARK_CASE4 /* TXT(FLASH),RODATA(TIM), DATA-BSS-STACK(OCM) */
  #define TXT_LMA     pflash_icache
  #define TXT_VMA     pflash_icache
  #define RODATA_LMA  itim
  #define RODATA_VMA  itim
  #define DATA_LMA    ocm
  #define DATA_VMA    ocm
#elif defined EMPS_COREMARK_CASE5 /* TXT(FLASH),RODATA(OCM), DATA-BSS-STACK(OCM) */
  #define TXT_LMA     pflash_icache
  #define TXT_VMA     pflash_icache
  #define RODATA_LMA  ocm
  #define RODATA_VMA  ocm
  #define DATA_LMA    ocm
  #define DATA_VMA    ocm
#else
  #error coremark configure do not support
#endif

SECTIONS
{
  .init     :
  {
    *(.vtable)
    KEEP (*(SORT_NONE(.init)))
  } >TXT_VMA AT>TXT_LMA

  .ilalign  :
  {
    . = ALIGN(4);
    PROVIDE( _ilm_lma = . );
  } >TXT_VMA AT>TXT_LMA

  .ialign   :
  {
    . = ALIGN(4);
    PROVIDE( _ilm = . );
  } >TXT_VMA AT>TXT_LMA

  .text     :
  {
  	. = ALIGN(4);
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
    . = ALIGN(4);
  } >TXT_VMA AT>TXT_LMA

  .rodata   :
  {
    . = ALIGN(4);
    *(.rdata)
    *(.rodata .rodata.*)
    /* section information for initial. */
    . = ALIGN(4);
    __rt_init_start = .;
    KEEP(*(SORT(.rti_fn*)))
    __rt_init_end = .;
    /* section information for finsh shell */
    . = ALIGN(4);
    __fsymtab_start = .;
    KEEP(*(FSymTab))
    __fsymtab_end = .;
    . = ALIGN(4);
    __vsymtab_start = .;
    KEEP(*(VSymTab))
    __vsymtab_end = .;
    *(.gnu.linkonce.r.*)
  } >RODATA_VMA AT>RODATA_LMA

  .fini   :
  {
    . = ALIGN(4);
    KEEP (*(SORT_NONE(.fini)))
  } >TXT_VMA AT>TXT_LMA

  . = ALIGN(4);
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  PROVIDE( _eilm = . );

  .preinit_array  :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >TXT_VMA AT>TXT_LMA

  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_end = .);
  } >TXT_VMA AT>TXT_LMA

  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >TXT_VMA AT>TXT_LMA

  .ctors      :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >TXT_VMA AT>TXT_LMA

  .dtors      :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >TXT_VMA AT>TXT_LMA
  
  .lalign     : ALIGN(16)
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >DATA_LMA AT>DATA_LMA

  .dalign     : ALIGN(16)
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >DATA_VMA AT>DATA_LMA

  .data       : ALIGN(16)
  {
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.* .sdata*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
  } >DATA_VMA AT>DATA_LMA

  . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );

  .timcode     : ALIGN(16)
  {
    . = ALIGN(4);
    *(.itimcode)
    . = ALIGN(4);
  } >itim AT>TXT_LMA
  PROVIDE( _ramcode_lma = LOADADDR(.timcode) );
  PROVIDE( _ramcode_vma = ADDR(.timcode) );
  PROVIDE( _ramcode_end = ADDR(.timcode) + SIZEOF(.timcode) );

  PROVIDE( _fbss = . );
  PROVIDE( __bss_start = . );
  .bss        :
  {
    . = ALIGN(4);
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
  } >DATA_VMA AT>DATA_LMA

  . = ALIGN(4);
  PROVIDE( end = . );
  PROVIDE( _end = . );

  .alalign    :
  {
    . = ALIGN(4);
    PROVIDE( _amo_lma = . );
  } >DATA_LMA AT>DATA_LMA

  .aalign     :
  {
    . = ALIGN(4);
	PROVIDE( _amo = .);
  } >DATA_VMA AT>DATA_LMA

  .amo        :
  {
    . = ALIGN(4);
    *(.amo* .amo.*)
    . = ALIGN(4);
  } >DATA_VMA AT>DATA_LMA

  . = ALIGN(4);
  PROVIDE( _amo_end = . );

  .heap       :
  {
   . = ALIGN(4);
   _heap = .;
   HeapBase = .;
   _heap_start = .;
   . += _HEAP_SIZE;
   . = ALIGN(4);
   _heap_end = .;
   HeapLimit = .;
  } >DATA_VMA AT>DATA_VMA

  . = ALIGN(16);
  .stack  : ALIGN(16)
  {
    . = ALIGN(16);
    PROVIDE(_stack_end = . );
    . += _STACK_SIZE;
    . = ALIGN(16);
    PROVIDE(_stack_top = . );
  } >DATA_VMA AT>DATA_VMA

  .apool      :
  {
    . = ALIGN(4);
    . += 8;
    PROVIDE( start_cycle = . );
    . += 8;
    PROVIDE( end_cycle = . );
    . += 8;
    PROVIDE( apool_start = . );
    . += _APOOL_SIZE;
    PROVIDE( apool_end = . );
  } >DATA_VMA AT>DATA_VMA
}