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
 * Title:        basic_memory.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"

#ifdef CONFIG_BASIC_MM
#define roundmb(x) (uint32_t)(((uint32_t)(x) + 3) & 0xFFFFFFFC)

/*--------------------------------------------------------------------------*/
/* Memory */
/****************************************************************************
 * Name: basic_pool_init
 *
 * Description:
 *   Initialize memory pool.
 *
 * Input Parameters:
 *   cp- The first address of the space occupied by the memory pool.
 *   size- The amount of space occupied by the memory pool.
 *
 * Returned Value:
 *   0- Initialization of memory pool succeeded.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
int basic_pool_init(char *cp, uint32_t size)
{
	BASIC_ASSERT(cp);
	BASIC_ASSERT(size);

	basic_free_block_t *pMem = (basic_free_block_t *) cp;

	size &= 0xFFFFFFC;
	cp += sizeof(basic_free_block_t);
	size -= sizeof(basic_free_block_t);
	pMem->mnext = (basic_free_block_t *) cp;
	pMem->mlen = 0;
	pMem = (basic_free_block_t *) cp;
	pMem->mnext = NULL;
	pMem->mlen = size;
	return 0;
}

/****************************************************************************
 * Name: basic_malloc
 *
 * Description:
 *   Apply for memory
 *
 * Input Parameters:
 *   pPool:Allocate memory from the memory pool pointed by ppool.
 *         When pPool == null, allocate memory from the default memory pool.
 *   nbytes:Required memory size (bytes).
 *
 * Returned Value:
 *   NULL- Failed to apply for memory.
 *   !NULL- If the memory application is successful,
 *          return the first memory address allocated.
 *
 ****************************************************************************/
void *basic_malloc(void *pPool, uint32_t nbytes)
{
	BASIC_ASSERT(pPool);

	basic_free_block_t *q = (basic_free_block_t *) pPool;
	basic_free_block_t *p = q->mnext;
	basic_free_block_t *leftover;
	basic_allocated_block_t *pRet = NULL;

	if (nbytes == 0)
		return (NULL);

	if (p == (basic_free_block_t *) NULL)
		return (NULL);

	nbytes = (uint32_t) roundmb(nbytes) + sizeof(basic_allocated_block_t);
	for (; p != (basic_free_block_t *) NULL; q = p, p = p->mnext) {
		if (p->mlen == nbytes) {
			q->mnext = p->mnext;
			pRet = (basic_allocated_block_t *) p;
			break;
		} else {
			if (p->mlen > nbytes) {
				if (p->mlen < nbytes + sizeof(basic_free_block_t)) { // too small to be splited into two block.
					q->mnext = p->mnext;
					nbytes = p->mlen;
					pRet = (basic_allocated_block_t *) p;
					break;
				} else {
					leftover = (basic_free_block_t *) ((uint8_t *) p + nbytes);
					q->mnext = leftover;
					leftover->mnext = p->mnext;
					leftover->mlen = p->mlen - nbytes;
					pRet = (basic_allocated_block_t *) p;
					break;
				}
			}
		}
	}

	if (pRet) {
		/*
		 * Record the size of the allocation and note the pool from it was allocated.
		 */
		pRet->size = nbytes;
		pRet->pPool = (basic_free_block_t *) pPool;
		pRet++;
	}

	if (!pRet && nbytes)
		printf("Cannot allocate 0x%lx Bytes\r\n", nbytes);

	return pRet;
}

/****************************************************************************
 * Name: basic_free
 *
 * Description:
 *   Free memory.
 *
 * Input Parameters:
 *   data- Allocated memory blocks to be released.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_free(void *data)
{
	BASIC_ASSERT(data);

	basic_free_block_t *p, *q;
	uint8_t *top;
	basic_allocated_block_t *pMem = (basic_allocated_block_t *) ((uint8_t *) data - sizeof(basic_allocated_block_t));
	basic_free_block_t *block = (basic_free_block_t *) pMem;
	uint32_t size = pMem->size;
	basic_free_block_t *pPool = pMem->pPool;

	for (p = pPool->mnext, q = pPool; p != (basic_free_block_t *) NULL && p < block; q = p, p = p->mnext)
		;
	top = q->mlen + (uint8_t *) q;
	if ((top > (uint8_t *) block && q != pPool) || (p != NULL && (size + (uint8_t *) block) > (uint8_t *) p))
		return;

	if (q != pPool && top == (uint8_t *) block)
		q->mlen += size;
	else {
		block->mlen = size;
		block->mnext = p;
		q->mnext = block;
		q = block;
	}
	if ((uint8_t *) (q->mlen + (uint8_t *) q) == (uint8_t *) p) {
		q->mlen += p->mlen;
		q->mnext = p->mnext;
	}
}

/****************************************************************************
 * Name: memcopy
 *
 * Description:
 *    memory copy.
 *
 * Input Parameters:
 *   dest- destination address.
 *   src- source address
 *   len- size of data 
 * Returned Value:
 *   dest.
 *
 ****************************************************************************/
void * memcopy(void * dest, void * src, uint32_t len)
{
    uint8_t* dt = dest;
    uint8_t* sr = src;
    
    while(len--)
    {

        *dt++ = *sr++ ;
    }
    
    return dest;
}
#endif /* CONFIG_BASIC_MM */
