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

MEMORY
{
  itim (wxa!ri) : ORIGIN = TIM_1_BASEADDR, LENGTH = TIM_1_SIZE
  ocm (wxa!ri) : ORIGIN = OCM_DCACHE_BASEADDR, LENGTH = OCM_DCACHE_SIZE
}

SECTIONS
{
  .init     :
  {
    *(.vtable)
    KEEP (*(SORT_NONE(.init)))
  } >itim AT>itim

  .ilalign  :
  {
    . = ALIGN(4);
    PROVIDE( _ilm_lma = . );
  } >itim AT>itim

  .ialign   :
  {
    . = ALIGN(4);
    PROVIDE( _ilm = . );
  } >itim AT>itim

  .text     :
  {
  	. = ALIGN(4);
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
    . = ALIGN(4);
  } >itim AT>itim

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
  } >ocm AT>ocm

  .fini   :
  {
    . = ALIGN(4);
    KEEP (*(SORT_NONE(.fini)))
  } >itim AT>itim

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
  } >itim AT>itim

  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_end = .);
  } >itim AT>itim

  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >itim AT>itim

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
    /* We don't want to include the .ctor section fpflash_icache
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >itim AT>itim

  .dtors      :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >itim AT>itim

  .timcode     :
  {
    . = ALIGN(4);
    *(.itimcode)
    . = ALIGN(4);
  } >itim AT>itim
  PROVIDE( _ramcode_lma = LOADADDR(.timcode) );
  PROVIDE( _ramcode_vma = ADDR(.timcode) );
  PROVIDE( _ramcode_end = ADDR(.timcode) + SIZEOF(.timcode) );

  .lalign     :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >ocm AT>ocm

  .dalign     :
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >ocm AT>ocm

  .data       :
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
  } >ocm AT>ocm

  . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );

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
  } >ocm AT>ocm

  . = ALIGN(4);
  PROVIDE( end = . );
  PROVIDE( _end = . );

  .alalign    :
  {
    . = ALIGN(4);
    PROVIDE( _amo_lma = . );
  } >itim AT>itim

  .aalign     :
  {
    . = ALIGN(4);
	PROVIDE( _amo = .);
  } >itim AT>itim

  .amo        :
  {
    . = ALIGN(4);
    *(.amo* .amo.*)
    . = ALIGN(4);
  } >itim AT>itim

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
  } >ocm AT>ocm

  .stack ALIGN(16):
  {
    . = ALIGN(16);
    PROVIDE(_stack_end = . );
    . += _STACK_SIZE;
    . = ALIGN(16);
    PROVIDE(_stack_top = . );
  } >ocm AT>ocm

  .apool      :
  {
    . = ALIGN(4);
    . += 8;
    PROVIDE( start_cycle = . );
    . += 8;
    PROVIDE( end_cycle = . );
    . += 8;
    PROVIDE( apool_start = . );
  } >ocm AT>ocm

  .apool_end ORIGIN(ocm) + LENGTH(ocm) - 4:
  {
    . = ALIGN(4);
    PROVIDE( apool_end = . );
  } >ocm AT>ocm
}
