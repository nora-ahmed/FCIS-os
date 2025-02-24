/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include <kern/proc/user_environment.h>
#include "kheap.h"
#include "memory_manager.h"
#include <inc/queue.h>

//extern void inctst();

/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
//This function should cut-paste the given number of pages from source_va to dest_va on the given page_directory
//	If the page table at any destination page in the range is not exist, it should create it
//	If ANY of the destination pages exists, deny the entire process and return -1. Otherwise, cut-paste the number of pages and return 0
//	ALL 12 permission bits of the destination should be TYPICAL to those of the source
//	The given addresses may be not aligned on 4 KB
int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	//[PROJECT] [CHUNK OPERATIONS] cut_paste_pages
	// Write your code here, remove the panic and write your code
	panic("cut_paste_pages() is not implemented yet...!!");
}

//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va on the given page_directory
//	Ranges DO NOT overlapped.
//	If ANY of the destination pages exists with READ ONLY permission, deny the entire process and return -1.
//	If the page table at any destination page in the range is not exist, it should create it
//	If ANY of the destination pages doesn't exist, create it with the following permissions then copy.
//	Otherwise, just copy!
//		1. WRITABLE permission
//		2. USER/SUPERVISOR permission must be SAME as the one of the source
//	The given range(s) may be not aligned on 4 KB
int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	//[PROJECT] [CHUNK OPERATIONS] copy_paste_chunk
	// Write your code here, remove the //panic and write your code
	panic("copy_paste_chunk() is not implemented yet...!!");
}

//===============================
// 3) SHARE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va on the given page_directory
//	Ranges DO NOT overlapped.
//	It should set the permissions of the second range by the given perms
//	If ANY of the destination pages exists, deny the entire process and return -1. Otherwise, share the required range and return 0
//	If the page table at any destination page in the range is not exist, it should create it
//	The given range(s) may be not aligned on 4 KB
int share_chunk(uint32* page_directory, uint32 source_va,uint32 dest_va, uint32 size, uint32 perms)
{
	//[PROJECT] [CHUNK OPERATIONS] share_chunk
	// Write your code here, remove the //panic and write your code
	panic("share_chunk() is not implemented yet...!!");
}

//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
//This function should allocate the given virtual range [<va>, <va> + <size>) in the given address space  <page_directory> with the given permissions <perms>.
//	If ANY of the destination pages exists, deny the entire process and return -1. Otherwise, allocate the required range and return 0
//	If the page table at any destination page in the range is not exist, it should create it
//	Allocation should be aligned on page boundary. However, the given range may be not aligned.
int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms)
{
	//[PROJECT] [CHUNK OPERATIONS] allocate_chunk
	// Write your code here, remove the //panic and write your code
	panic("allocate_chunk() is not implemented yet...!!");
}

//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	//[PROJECT] [CHUNK OPERATIONS] calculate_allocated_space
	// Write your code here, remove the panic and write your code
	panic("calculate_allocated_space() is not implemented yet...!!");
}

//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
//This function should calculate the required number of pages for allocating and mapping the given range [start va, start va + size) (either for the pages themselves or for the page tables required for mapping)
//	Pages and/or page tables that are already exist in the range SHOULD NOT be counted.
//	The given range(s) may be not aligned on 4 KB
uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	//[PROJECT] [CHUNK OPERATIONS] calculate_required_frames
	// Write your code here, remove the panic and write your code
	panic("calculate_required_frames() is not implemented yet...!!");
}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
/* DYNAMIC ALLOCATOR SYSTEM CALLS */
//=====================================
void* sys_sbrk(int numOfPages)
{
	/* numOfPages > 0: move the segment break of the current user program to increase the size of its heap
	 * 				by the given number of pages. You should allocate NOTHING,
	 * 				and returns the address of the previous break (i.e. the beginning of newly mapped memory).
	 * numOfPages = 0: just return the current position of the segment break
	 *
	 * NOTES:
	 * 	1) As in real OS, allocate pages lazily. While sbrk moves the segment break, pages are not allocated
	 * 		until the user program actually tries to access data in its heap (i.e. will be allocated via the fault handler).
	 * 	2) Allocating additional pages for a process’ heap will fail if, for example, the free frames are exhausted
	 * 		or the break exceed the limit of the dynamic allocator. If sys_sbrk fails, the net effect should
	 * 		be that sys_sbrk returns (void*) -1 and that the segment break and the process heap are unaffected.
	 * 		You might have to undo any operations you have done so far in this case.
	 */

	//TODO: [PROJECT'24.MS2 - #11] [3] USER HEAP - sys_sbrk
	/*====================================*/
	/*Remove this line before start coding*/

	/*====================================*/
	struct Env* env = get_cpu_proc(); //the current running Environment to adjust its break limit
	   uint32 sizeOfPages = numOfPages * PAGE_SIZE;

		        if (numOfPages == 0) {

		            return (void*)env->USER_BREAK;
		        }

		        uint32 reqPageSize = (uint32)env->USER_BREAK + sizeOfPages;
		        if (reqPageSize > (uint32)env->USER_HARD_LIMIT) {

		            return (void*)-1;
		        }
		        uint32 prevBreakPosition = (uint32)env->USER_BREAK;
		        uint32 va = (uint32)prevBreakPosition;
//if(env->USER_HEAP_DASTART==env->USER_BREAK){
//
//		        	uint32 *newbeg = (uint32 *)((uint32)va );
//		        	*newbeg=1;
//
//		        }

		        env->USER_BREAK += sizeOfPages;


//
//		    	uint32 *newEnd = (uint32 *)((uint32)va +(sizeOfPages)-4);
//		    		*newEnd = 1;

		        for ( uint32 i=(uint32)prevBreakPosition; i <(uint32) env->USER_BREAK; i+=PAGE_SIZE) {

					uint32* p=NULL;
					int ret=get_page_table(env->env_page_directory, i, &p);
						if(p==NULL||ret==TABLE_NOT_EXIST){

							create_page_table(env->env_page_directory, i);
						}


					pt_set_page_permissions(env->env_page_directory,(uint32)i,PERM_AVAILABLE,0);
		    		//	pt_set_page_permissions(env->env_page_directory,(uint32)i,PERM_AVAILABLE,0);
//					user_Pages[((uint32)i-((uint32)env->USER_HARD_LIMIT+(PAGE_SIZE)))/(PAGE_SIZE)].numOfPages=numOfPages;
//
//					user_Pages[((uint32)i-((uint32)env->USER_HARD_LIMIT+(PAGE_SIZE)))/(PAGE_SIZE)].va=(uint32)prevBreakPosition;
//					user_Pages[((uint32)i-((uint32)env->USER_HARD_LIMIT+(PAGE_SIZE)))/(PAGE_SIZE)].is_m=1;
		        }
		        return (void *)prevBreakPosition;

}

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	/*====================================*/
	/*Remove this line before start coding*/
//	inctst();
//	return;
	/*====================================*/

	//TODO: [PROJECT'24.MS2 - #13] [3] USER HEAP [KERNEL SIDE] - allocate_user_mem()
	// Write your code here, remove the panic and write your code
	//panic("allocate_user_mem() is not implemented yet...!!");
	uint32 numOfPages=size/(PAGE_SIZE);
		if(size%(PAGE_SIZE)!=0){
			numOfPages++;
		}
//		uint32* p=NULL;
//
//		int ret=get_page_table(e->env_page_directory, virtual_address, &p);
//		if(p==NULL){
//
//			create_page_table(e->env_page_directory, virtual_address);
//		}

		for(uint32 i=(uint32)virtual_address;i<(uint32)virtual_address+(numOfPages*PAGE_SIZE);i+=(PAGE_SIZE)){
			//cprintf("kdkdkd %u \n",e->env_page_directory);
			uint32* p=NULL;
			int ret=get_page_table(e->env_page_directory, i, &p);
				if(ret==TABLE_NOT_EXIST){

					create_page_table(e->env_page_directory, i);
				}


			pt_set_page_permissions(e->env_page_directory,(uint32)i,PERM_AVAILABLE|PERM_WRITEABLE|PERM_USER,0);
//			pt_set_page_permissions(e->env_page_directory,(uint32)i,PERM_PRESENT,0);
//
//			user_Pages[((uint32)i-((uint32)e->USER_HARD_LIMIT+(PAGE_SIZE)))/(PAGE_SIZE)].numOfPages=numOfPages;
//			user_Pages[((uint32)i-((uint32)e->USER_HARD_LIMIT+(PAGE_SIZE)))/(PAGE_SIZE)].is_m=1;
//			user_Pages[((uint32)i-((uint32)e->USER_HARD_LIMIT+(PAGE_SIZE)))/(PAGE_SIZE)].va=virtual_address;

		}


}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	/*====================================*/
	/*Remove this line before start coding*/
//	inctst();
//	return;
	/*====================================*/

	//TODO: [PROJECT'24.MS2 - #15] [3] USER HEAP [KERNEL SIDE] - free_user_mem
	// Write your code here, remove the panic and write your code
	//panic("free_user_mem() is not implemented yet...!!");

for(uint32 i=virtual_address;i<(uint32)virtual_address+size;i+=(PAGE_SIZE)){
	uint32* p=NULL;
		int ret=get_page_table(e->env_page_directory, i, &p);
			if(ret==TABLE_IN_MEMORY){
pt_set_page_permissions(e->env_page_directory,(uint32)i,0,PERM_AVAILABLE);
				//create_page_table(e->env_page_directory, i);
//struct FrameInfo * ss=get_frame_info(e->env_page_directory,i, &p);
//if(ss!=NULL){
//	 unmap_frame(e->env_page_directory, i);
//}
			}

pf_remove_env_page(e,i);


		env_page_ws_invalidate(e,i);
		uint32 wsSize = LIST_SIZE(&(e->page_WS_list));

				if(wsSize < (e->page_WS_max_size)){

					struct WorkingSetElement* tia = LIST_FIRST((&e->page_WS_list));

					while (tia != e->page_last_WS_element) {

						struct WorkingSetElement* new = tia;

						LIST_REMOVE(&e->page_WS_list , new);

						LIST_INSERT_TAIL(&e->page_WS_list , new);

						tia = LIST_NEXT(tia);

					}
				}


}
if(LIST_SIZE(&e->page_WS_list)!=e->page_WS_max_size)e->page_last_WS_element=NULL;
	//TODO: [PROJECT'24.MS2 - BONUS#3] [3] USER HEAP [KERNEL SIDE] - O(1) free_user_mem
}

//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env* e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");
}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	//[PROJECT] [USER HEAP - KERNEL SIDE] move_user_mem
	//your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//

