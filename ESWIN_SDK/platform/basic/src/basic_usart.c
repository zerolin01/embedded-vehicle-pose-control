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
 * Title:        basic_usart.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"

extern struct basic_platform_operations_s platform_ops;

/*--------------------------------------------------------------------------*/
/* USART */
/****************************************************************************
 * Name: basic_usart_read(uint8_t *readbuf, uint8_t num)
 *
 * Description:
 *   Get hardware hard ID.

 * Input Parameters:
 *   readbuf-The usart read buf.
 *   num-The size of buf.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_usart_read(uint8_t *readbuf, uint8_t num)
{
	platform_ops.usart_read(readbuf, num);
}

/****************************************************************************
 * Name: basic_usart_write(uint8_t *readbuf, uint8_t num)
 *
 * Description:
 *   Get hardware hard ID.

 * Input Parameters:
 *   writebuf-The usart write buf.
 *   num-The size of buf.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_usart_write(uint8_t *writebuf, uint8_t num)
{
	platform_ops.usart_write(writebuf, num);
}
