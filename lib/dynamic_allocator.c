/*
 * dynamic_allocator.c
 *
 *  Created on: Sep 21, 2023
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//=====================================================
// 1) GET BLOCK SIZE (including size of its meta data):
//=====================================================
__inline__ uint32 get_block_size(void* va) {
	uint32 *curBlkMetaData = ((uint32 *) va - 1);
	return (*curBlkMetaData) & ~(0x1);
}

//===========================
// 2) GET BLOCK STATUS:
//===========================
__inline__ int8 is_free_block(void* va) {
	uint32 *curBlkMetaData = ((uint32 *) va - 1);
	return (~(*curBlkMetaData) & 0x1);
}

//===========================
// 3) ALLOCATE BLOCK:
//===========================

void *alloc_block(uint32 size, int ALLOC_STRATEGY) {
	void *va = NULL;
	switch (ALLOC_STRATEGY) {
	case DA_FF:
		va = alloc_block_FF(size);
		break;
	case DA_NF:
		va = alloc_block_NF(size);
		break;
	case DA_BF:
		va = alloc_block_BF(size);
		break;
	case DA_WF:
		va = alloc_block_WF(size);
		break;
	default:
		cprintf("Invalid allocation strategy\n");
		break;
	}
	return va;
}

//===========================
// 4) PRINT BLOCKS LIST:
//===========================

void print_blocks_list(struct MemBlock_LIST list) {
	cprintf("=========================================\n");
	struct BlockElement* blk;
	cprintf("\nDynAlloc Blocks List:\n");
	LIST_FOREACH(blk, &list)
	{
		cprintf("(size: %d, isFree: %d)\n", get_block_size(blk),
				is_free_block(blk));
	}
	cprintf("=========================================\n");

}
//
////********************************************************************************//
////********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

bool is_initialized = 0;
//==================================
// [1] INITIALIZE DYNAMIC ALLOCATOR:
//==================================
void initialize_dynamic_allocator(uint32 daStart,
		uint32 initSizeOfAllocatedSpace) {
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		if (initSizeOfAllocatedSpace % 2 != 0)
			initSizeOfAllocatedSpace++; //ensure it's multiple of 2
		if (initSizeOfAllocatedSpace == 0)
			return;
		is_initialized = 1;
	}
	//==================================================================================
	//==================================================================================

	//TODO: [PROJECT'24.MS1 - #04] [3] DYNAMIC ALLOCATOR - initialize_dynamic_allocator
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("initialize_dynamic_allocator is not implemented yet");
	//Your Code is Here...

	LIST_INIT(&freeBlocksList);
	uint32 *beg = (uint32 *) (daStart);
	*beg = 1;
	uint32 *end = (uint32 *) (daStart + initSizeOfAllocatedSpace
			- sizeof(uint32));
	*end = 1;
	struct BlockElement *firstBlock = (struct BlockElement*) (daStart
			+ 2 * sizeof(uint32));
	uint32 *header = (uint32 *) (daStart + sizeof(uint32));
	*header = initSizeOfAllocatedSpace - 2 * sizeof(uint32);
	uint32 *footer = (uint32 *) (daStart + initSizeOfAllocatedSpace
			- 2 * sizeof(uint32));
	*footer = initSizeOfAllocatedSpace - 2 * sizeof(uint32);
	LIST_INSERT_HEAD(&freeBlocksList, firstBlock);
	struct BlockElement* temp1 = LIST_FIRST(&freeBlocksList);



}
//==================================
// [2] SET BLOCK HEADER & FOOTER:
//==================================

void set_block_data(void* va, uint32 totalSize, bool isAllocated) {
	//TODO: [PROJECT'24.MS1 - #05] [3] DYNAMIC ALLOCATOR - set_block_data
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("set_block_data is not implemented yet");
	//Your Code is Here...

	uint32 *header = (uint32*) (va - sizeof(uint32));
	*header  = totalSize + isAllocated;

	uint32 *footer = (uint32*) (va + totalSize - 2 * sizeof(uint32));
	*footer = totalSize + isAllocated;

	return;
}

//=========================================
// [3] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
void *alloc_block_FF(uint32 size)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		if (size % 2 != 0) size++;	//ensure that the size is even (to use LSB as allocation flag)
		if (size < DYN_ALLOC_MIN_BLOCK_SIZE)
			size = DYN_ALLOC_MIN_BLOCK_SIZE ;
		if (!is_initialized)
		{
			uint32 required_size = size + 2*sizeof(int) /*header & footer*/ + 2*sizeof(int) /*da begin & end*/ ;
			uint32 da_start = (uint32)sbrk(ROUNDUP(required_size, PAGE_SIZE)/PAGE_SIZE);
			uint32 da_break = (uint32)sbrk(0);
			initialize_dynamic_allocator(da_start, da_break - da_start);
		}
	}
	//==================================================================================
	//==================================================================================

	//TODO: [PROJECT'24.MS1 - #06] [3] DYNAMIC ALLOCATOR - alloc_block_FF
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("alloc_block_FF is not implemented yet");
	//Your Code is Here...
if(size < 1) {


	return NULL;
}
uint32 counter =0;
uint32 required_size=size+8;
struct BlockElement *element;
bool flag =0;

LIST_FOREACH(element,&freeBlocksList){
uint32 sz=get_block_size(element);
if(sz==required_size){
	set_block_data(element,required_size,1);
	LIST_REMOVE(&freeBlocksList,element);
	return element;
}
else if(sz>required_size){
	uint32 rem=sz-required_size;
	if(rem<16){

				set_block_data(element,sz,1);
				LIST_REMOVE(&freeBlocksList,element);
				return element;
	}
	else{
		struct BlockElement* nex=(struct BlockElement *)((uint32)element +size+8);
		uint32 nSize=get_block_size(nex);
		set_block_data(element,required_size,1);
		set_block_data(nex,rem,0);
		if(LIST_SIZE(&freeBlocksList)==1){
			LIST_REMOVE(&freeBlocksList,element);
			LIST_INSERT_HEAD(&freeBlocksList,nex);
			return element;
		}
		else{
		LIST_INSERT_AFTER(&freeBlocksList,element,nex);
							LIST_REMOVE(&freeBlocksList,element);
							return element ;}
	}
}
}

	void*ret = sbrk(1);

	if(	ret != (void*)-1) {


	if(LIST_SIZE(&freeBlocksList)>=1){



	struct BlockElement* lastBlock = LIST_LAST(&freeBlocksList);
	uint32 lastBlockSize = get_block_size(lastBlock);

	if((uint32)lastBlock + lastBlockSize == (uint32)ret){

		set_block_data(lastBlock, lastBlockSize + 4*1024, 0);
//		uint32 *oldEnd = (uint32 *)((uint32)ret -4);
//		*oldEnd = 0;
		uint32 *newEnd = (uint32 *)((uint32)ret +(4*1024)-4);
		*newEnd = 1;
		return alloc_block_FF(size);

	}
	else {
		set_block_data(ret,4*1024,0);
		LIST_INSERT_TAIL(&freeBlocksList,(struct BlockElement *)ret);
//		uint32 *oldEnd = (uint32 *)((uint32)ret -4);
//		*oldEnd = 0;
		uint32 *newEnd = (uint32 *)((uint32)ret +(4*1024)-4);
		*newEnd = 1;
		return alloc_block_FF(size);

	}
//	uint32 endBlockSize = get_block_size(end);
	struct BlockElement* newBlock;


	}

	else {
		set_block_data(ret,4*1024,0);
		LIST_INSERT_TAIL(&freeBlocksList,(struct BlockElement *)ret);
//		uint32 *oldEnd = (uint32 *)((uint32)ret -4);
//		*oldEnd = 0;
		uint32 *newEnd = (uint32 *)((uint32)ret +(4*1024)-4);
		*newEnd = 1;
		return alloc_block_FF(size);

	}
	}

//    sbrk(required_size);


    return NULL;


}
//=========================================
// [4] ALLOCATE BLOCK BY BEST FIT:
//=========================================
void *alloc_block_BF(uint32 size) {
	if (size % 2 != 0)
		size++;
	if (size < DYN_ALLOC_MIN_BLOCK_SIZE)
		size = DYN_ALLOC_MIN_BLOCK_SIZE;

	if (!is_initialized) {
		uint32 required_size = size + 2 * sizeof(int) /*header & footer*/
		+ 2 * sizeof(int) /*da begin & end*/;
		uint32 da_start = (uint32) sbrk(
		ROUNDUP(required_size, PAGE_SIZE) / PAGE_SIZE);
		uint32 da_break = (uint32) sbrk(0);
		initialize_dynamic_allocator(da_start, da_break - da_start);
	}

	uint32 zerofff = 0xFFFFFF;
	struct BlockElement *minVa = NULL;
	struct BlockElement *element = NULL;

	if (size <= 1) {
		return NULL;
	}

	uint32 req_size = size + 8;

	LIST_FOREACH(element, &freeBlocksList)
	{
		uint32 sz = get_block_size(element);

		if (sz == req_size) {
			set_block_data(element, req_size, 1);
			LIST_REMOVE(&freeBlocksList, element);
			return element;
		}

		else if (sz > req_size && sz <zerofff) {
			zerofff = sz;
			minVa = element;
		}
	}

	if (minVa != NULL) {
		uint32 rem = zerofff - req_size;

		if (rem < 16) {
			set_block_data(minVa, zerofff, 1);
			LIST_REMOVE(&freeBlocksList, minVa);
			return minVa;
		} else {
			struct BlockElement* nex = (struct BlockElement *) ((uint32) minVa
					+ size + 8);
			set_block_data(minVa, req_size, 1);
			set_block_data(nex, rem, 0);
			if (LIST_SIZE(&freeBlocksList) == 1) {
				LIST_REMOVE(&freeBlocksList, minVa);
				LIST_INSERT_HEAD(&freeBlocksList, nex);
				return minVa;
			} else {
				LIST_INSERT_AFTER(&freeBlocksList, minVa, nex);
				LIST_REMOVE(&freeBlocksList, minVa);
				return minVa;
			}
		}
	}

	void*ret = sbrk(1);

		if(	ret != (void*)-1) {


		if(LIST_SIZE(&freeBlocksList)>=1){



		struct BlockElement* lastBlock = LIST_LAST(&freeBlocksList);
		uint32 lastBlockSize = get_block_size(lastBlock);

		if((uint32)lastBlock + lastBlockSize == (uint32)ret){

			set_block_data(lastBlock, lastBlockSize + 4*1024, 0);
	//		uint32 *oldEnd = (uint32 *)((uint32)ret -4);
	//		*oldEnd = 0;
			uint32 *newEnd = (uint32 *)((uint32)ret +(4*1024)-4);
			*newEnd = 1;
			return alloc_block_BF(size);

		}
		else {
			set_block_data(ret,4*1024,0);
			LIST_INSERT_TAIL(&freeBlocksList,(struct BlockElement *)ret);
	//		uint32 *oldEnd = (uint32 *)((uint32)ret -4);
	//		*oldEnd = 0;
			uint32 *newEnd = (uint32 *)((uint32)ret +(4*1024)-4);
			*newEnd = 1;
			return alloc_block_BF(size);

		}
	//	uint32 endBlockSize = get_block_size(end);
		struct BlockElement* newBlock;


		}

		else {
			set_block_data(ret,4*1024,0);
			LIST_INSERT_TAIL(&freeBlocksList,(struct BlockElement *)ret);
	//		uint32 *oldEnd = (uint32 *)((uint32)ret -4);
	//		*oldEnd = 0;
			uint32 *newEnd = (uint32 *)((uint32)ret +(4*1024)-4);
			*newEnd = 1;
			return alloc_block_BF(size);


		          }
		}

	//    sbrk(required_size);


	    return NULL;
}

//===================================================
// [5] FREE BLOCK WITH COALESCING:
//===================================================

void free_block(void *va) {
	if (va == NULL) {
		return;
	}

	uint32 totalSize = get_block_size(va);

	set_block_data(va, totalSize, 0);

	struct BlockElement *block = (struct BlockElement *) va;
	uint32 *prev = (uint32 *) (((uint32) block - 8));
	uint32 prevSize = *(prev);
	if (prevSize % 2 == 1)
		prevSize--;
	struct BlockElement *pre = (struct BlockElement *) (((uint32) block
			- prevSize));
	struct BlockElement *ne = (struct BlockElement *) ((uint32) block
			+ totalSize);
	uint32 neSize = get_block_size(ne);
	pre = (struct BlockElement *) ((uint32) block - prevSize);
	set_block_data(va, totalSize, 0);
	if (is_free_block(ne) && is_free_block(pre) && prevSize >= 16
			&& neSize >= 16) {

		totalSize += neSize;
		totalSize += prevSize;
		LIST_REMOVE(&freeBlocksList, ne);
		set_block_data(pre, totalSize, 0);
	} else if (is_free_block(pre) && prevSize >= 16) {
		totalSize += prevSize;

		set_block_data(pre, totalSize, 0);
	} else if (is_free_block(ne) && neSize >= 16) {

		if (ne == LIST_FIRST(&freeBlocksList)) {

			totalSize += neSize;
			LIST_REMOVE(&freeBlocksList, ne);
			set_block_data(block, totalSize, 0);
			LIST_INSERT_HEAD(&freeBlocksList, block);
		} else {

			totalSize += neSize;
			struct BlockElement *element = LIST_PREV(ne);
			LIST_REMOVE(&freeBlocksList, ne);
			set_block_data(block, totalSize, 0);
			LIST_INSERT_AFTER(&freeBlocksList, element, block);
		}
	} else {

		if ((totalSize + block) == LIST_FIRST(&freeBlocksList)
				|| LIST_SIZE(&freeBlocksList) == 0) {

			LIST_INSERT_HEAD(&freeBlocksList, block);
		} else {

			struct BlockElement *element;
			bool flag = 0;
			LIST_FOREACH(element, &freeBlocksList)
			{

				uint32 eleSize = get_block_size(element);

				if ((uint32) element > (uint32) block) {

					LIST_INSERT_BEFORE(&freeBlocksList, element, block);

					flag = 1;
					return;
				}

			}
			if (!flag){

				LIST_INSERT_TAIL(&freeBlocksList, block);
				}
		}
	}
}

//=========================================
// [6] REALLOCATE BLOCK BY FIRST FIT:
//=========================================
void *realloc_block_FF(void* va, uint32 new_size) {
//	//TODO: [PROJECT'24.MS1 - #08] [3] DYNAMIC ALLOCATOR - realloc_block_FF
//	//COMMENT THE FOLLOWING LINE BEFORE START CODING
//	//panic("realloc_block_FF is not implemented yet");
//	//Your Code is Here...
	if (va == NULL && new_size == 0) {
		return NULL;
	} else if (va == NULL) {
		return alloc_block_FF(new_size);
	} else if (new_size == 0) {
		free_block(va);
		return NULL;
	}
	if (new_size % 2 != 0)
		new_size++;	//ensure that the size is even (to use LSB as allocation flag)
	bool is_ffG = 0;

	struct BlockElement* block = (struct BlockElement*) va;
	uint32 cuSize = get_block_size(block);
	struct BlockElement* nextBlock =
			(struct BlockElement *) ((uint32) block + cuSize);
	uint32 next_size = get_block_size(nextBlock);
	if (new_size + 8 == cuSize) {
		return va;
	}
	if (new_size + 8 > cuSize) {
		// i have next
		if (is_free_block(nextBlock) && next_size >= 16) {
			if (cuSize + next_size == new_size + 8) {

				set_block_data(va, cuSize + next_size, 1);
				LIST_REMOVE(&freeBlocksList, nextBlock);
				return va;
			}
			// i have space in next
			if (cuSize + next_size > new_size + 8) {

				int32 rem = cuSize + next_size - new_size - 8;
				//i will split
				if (rem >= 16) {

					struct BlockElement* new_next =
							(struct BlockElement*) ((uint32) nextBlock
									+ next_size - rem);
					set_block_data(new_next, rem, 0);
					set_block_data(va, cuSize + next_size - rem,
							1);
					if (LIST_LAST(&freeBlocksList) == nextBlock) {
						LIST_REMOVE(&freeBlocksList, nextBlock);
						LIST_INSERT_TAIL(&freeBlocksList, new_next);
					} else {

						struct BlockElement* before = LIST_PREV(
								nextBlock);
						LIST_REMOVE(&freeBlocksList, nextBlock);
						LIST_INSERT_AFTER(&freeBlocksList, before,
								new_next);
					}
					return va;
				}

				//i wont split
				else {

					set_block_data(va, cuSize + next_size, 1);
					LIST_REMOVE(&freeBlocksList, nextBlock);
					return va;
				}
			}
			// idone have spzce in next
			else {
				is_ffG = 1;
			}
		}
		// i dont have next
		else {
			is_ffG = 1;
		}

	}

	if (new_size + 8 < cuSize) {

		if (new_size+8 < 16)
			new_size = 16;
		uint32 sub = cuSize - new_size - 8;
		if (sub >= 16) {

			//i have next
			if (is_free_block(nextBlock) && next_size >= 16) {

				uint32 total = next_size + sub;
				struct BlockElement* new_block =
						(struct BlockElement*) ((uint32) va + new_size);
				set_block_data(new_block, total, 0);
				set_block_data(va, new_size + 8, 1);
				if (LIST_LAST(&freeBlocksList) == nextBlock) {
					LIST_REMOVE(&freeBlocksList, nextBlock);
					LIST_INSERT_TAIL(&freeBlocksList, new_block);
				} else {
					struct BlockElement* before = LIST_PREV(
							nextBlock);
					LIST_REMOVE(&freeBlocksList, nextBlock);
					LIST_INSERT_AFTER(&freeBlocksList, before,
							new_block);
				}
				return va;
			}
			/// i dont have next
			else {

				struct BlockElement* new_block =
						(struct BlockElement*) ((uint32) va + new_size + 8);
				set_block_data(new_block, sub, 0);
				set_block_data(va, new_size + 8, 1);
				struct BlockElement *element;
				bool flag = 0;
				LIST_FOREACH(element, &freeBlocksList)
				{
					uint32 eleSize = get_block_size(element);


					if ((uint32) element > (uint32) new_block) {

						LIST_INSERT_BEFORE(&freeBlocksList, element,
								new_block);
						flag = 1;
						break;
					}
				}
				if (!flag) {

					LIST_INSERT_TAIL(&freeBlocksList, new_block);
				}
				return va;

			}
		}
		//the rem is less than 16
		else {

			if (is_free_block(nextBlock) && next_size >= 16) {

				uint32 total = next_size + sub;
				struct BlockElement* new_block =
						(struct BlockElement*) ((uint32) va + new_size + 8);
				set_block_data(new_block,total, 0);
				set_block_data(va, new_size + 8, 1);
				if (LIST_LAST(&freeBlocksList) == nextBlock) {
					LIST_REMOVE(&freeBlocksList, nextBlock);
					LIST_INSERT_TAIL(&freeBlocksList, new_block);
				} else {
					struct BlockElement* before = LIST_PREV(
							nextBlock);
					LIST_REMOVE(&freeBlocksList, nextBlock);
					LIST_INSERT_AFTER(&freeBlocksList, before,
							new_block);
				}
				return va;
			} else {

				return va;
			}

		}
		if (is_ffG) {
			struct BlockElement* ne=(struct BlockElement *)alloc_block_FF(new_size);
			if(ne==NULL){
				free_block(ne);
				return NULL;
			}
			set_block_data(va, cuSize, 0);
			free_block(va);

			struct BlockElement *element;
			bool flag = 0;
			LIST_FOREACH(element, &freeBlocksList)
			{
				uint32 eleSize = get_block_size(element);
				if ((uint32) element > (uint32) va) {
					LIST_INSERT_BEFORE(&freeBlocksList, element,(struct BlockElement*) va);
					flag = 1;
					break;
				}
			}
			if (!flag) {

				LIST_INSERT_TAIL(&freeBlocksList, (struct BlockElement*)va);
			}
		return ne;
		}

	}

return NULL;
}

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
//=========================================
// [7] ALLOCATE BLOCK BY WORST FIT:
//=========================================
void *alloc_block_WF(uint32 size) {
	panic("alloc_block_WF is not implemented yet");
	return NULL;
}

//=========================================
// [8] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
void *alloc_block_NF(uint32 size) {
	panic("alloc_block_NF is not implemented yet");
	return NULL;

}
