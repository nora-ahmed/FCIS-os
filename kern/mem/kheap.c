#include "kheap.h"

#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"
#define kilo (1024)
//Initialize the dynamic allocator of kernel heap with the given start address, size & limit
//All pages in the given range should be allocated
//Remember: call the initialize_dynamic_allocator(..) to complete the initialization
//Return:
//	On success: 0
//	Otherwise (if no memory OR initial size exceed the given limit): PANIC
	//struct spinlock ly;
int initialize_kheap_dynamic_allocator(uint32 daStart, uint32 initSizeToAllocate, uint32 daLimit)
{
	//TODO: [PROJECT'24.MS2 - #01] [1] KERNEL HEAP - initialize_kheap_dynamic_allocator
	// Write your code here, remove the panic and write your code
	//panic("initialize_kheap_dynamic_allocator() is not implemented yet...!!");
	START_KHEAP=daStart;
	BREAK_KHEAP=daStart+initSizeToAllocate;
	HARD_LIMIT=daLimit;
	//	struct spinlock ly;
//		char* name = "lk";
//		init_spinlock(&(ly) , name);
	for(int i=daStart;i<BREAK_KHEAP;i+=(4*kilo)){
struct FrameInfo* nora_ptr;
int ret=allocate_frame(&nora_ptr);
if(ret==0){
	map_frame(ptr_page_directory,nora_ptr,i,PERM_WRITEABLE);
}
uint32*pge_table=NULL;
int r=get_page_table(ptr_page_directory,i,&pge_table);
uint32 pa=to_physical_address(nora_ptr);
																frames_pages[pa/(4*kilo)]=i;
//uint32 *ptr_page_table=(uint32 *)ptr_page_directory[PDX(i)];

	}
for(uint32 i= HARD_LIMIT+(4*kilo);i<(uint32)KERNEL_HEAP_MAX;i+=(4*kilo)){
	Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=((uint32)KERNEL_HEAP_MAX-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo);

Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)HARD_LIMIT+(4*kilo);
Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].lastva=(uint32)KERNEL_HEAP_MAX;
}

LIST_INIT(&freePagesList);
//struct PageEle* el=(struct PageEle*)KERNEL_HEAP_START;
//LIST_INSERT_HEAD(&freePagesList,el);
initialize_dynamic_allocator(daStart,initSizeToAllocate);
	return 0;
}

void* sbrk(int numOfPages)
{
	/* numOfPages > 0: move the segment break of the kernel to increase the size of its heap by the given numOfPages,
	 * 				you should allocate pages and map them into the kernel virtual address space,
	 * 				and returns the address of the previous break (i.e. the beginning of newly mapped memory).
	 * numOfPages = 0: just return the current position of the segment break
	 *
	 * NOTES:
	 * 	1) Allocating additional pages for a kernel dynamic allocator will fail if the free frames are exhausted
	 * 		or the break exceed the limit of the dynamic allocator. If sbrk fails, kernel should panic(...)
	 */

	//MS2: COMMENT THIS LINE BEFORE START CODING====
//	return (void*)-1 ;
	//====================================================

	//[PROJECT'24.MS2] Implement this function
	// Write your code here, remove the panic and write your code


	 uint32 pageSize = sizeof(int) * 1024;
	 uint32 sizeOfPages = numOfPages * pageSize;

	 if(sizeOfPages == 0){
		 return (void*)BREAK_KHEAP;
	 }

	 uint32 reqPageSize = BREAK_KHEAP + sizeOfPages;

if(reqPageSize>HARD_LIMIT){
	return(void*)-1;
}

		 struct BlockElement *prevBreakPosition = (struct BlockElement *)BREAK_KHEAP;
		 BREAK_KHEAP = BREAK_KHEAP + sizeOfPages;

		 uint32 va = (uint32)prevBreakPosition;
		 uint32 nextPageAdrs = va;

		 for(int i=0;i<numOfPages;i++){
			 struct FrameInfo *ptr_frame;
			int ret = allocate_frame(&ptr_frame);

			if (ret == 0) {
				map_frame(ptr_page_directory, ptr_frame, nextPageAdrs, PERM_WRITEABLE);

				uint32 pa=to_physical_address(ptr_frame);
																				frames_pages[pa/(4*kilo)]=nextPageAdrs;

			}

			nextPageAdrs += pageSize;

		 }


		 return prevBreakPosition;






}

//TODO: [PROJECT'24.MS2 - BONUS#2] [1] KERNEL HEAP - Fast Page Allocator

void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT'24.MS2 - #03] [1] KERNEL HEAP - kmalloc
	// Write your code here, remove the panic and write your code
//	kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	// use "isKHeapPlacementStrategyFIRSTFIT() ..." functions to check the current strategy
//	struct spinlock ly;
//	char* name = "lk";
//	init_spinlock(&(ly) , name);

	 //acquire_spinlock(&ly);
	 //release_spinlock(&ly);

uint32 numOfPages=size/(4*kilo);
if(size%(4*kilo)!=0){
	numOfPages++;
}
	if(numOfPages>((uint32)KERNEL_HEAP_MAX-(uint32)HARD_LIMIT)/(4*kilo)){
//cprintf("nooooo kheap \n");
		return NULL;
	}
	if(size<=DYN_ALLOC_MAX_BLOCK_SIZE){

return alloc_block_FF(size);


	}
	//acquire_spinlock(&ly);
	for(uint32 i=(uint32)HARD_LIMIT+(4*kilo);i<(uint32)KERNEL_HEAP_MAX;i+=(4*kilo)){
		if(Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f&&Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages==numOfPages){
			for(uint32 j=(uint32)i;j<(uint32)i+(4*kilo*numOfPages);j+=(4*kilo)){
														struct FrameInfo* nora_ptr;
																	int ret=allocate_frame(&nora_ptr);
																	if(ret==0){
																		map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE);
																uint32 pa=to_physical_address(nora_ptr);
																frames_pages[pa/(4*kilo)]=j;
			}
			Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;

		Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;
		Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;


													}
		//release_spinlock(&ly);
			return (void*)i;
		}
		else if(Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f&&Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages>numOfPages){
			uint32 ne=i+(4*kilo*numOfPages);
			uint32 size=Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages-numOfPages;
			uint32 last =i+(4*kilo*Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
			for(uint32 j=ne;j<last;j+=(4*kilo)){
				Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=size;

						Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=ne;
						Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
			}
			for(uint32 j=(uint32)i;j<(uint32)i+(4*kilo*numOfPages);j+=(4*kilo)){
														struct FrameInfo* nora_ptr;
																	int ret=allocate_frame(&nora_ptr);
																	if(ret==0){
																		map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE);
																		uint32 pa=to_physical_address(nora_ptr);
																																		frames_pages[pa/(4*kilo)]=j;
																	}
			Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;

		Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;
		Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;


													}
		//release_spinlock(&ly);
			return (void*)i;
		}
	}
//	release_spinlock(&ly);
	return NULL;
//	if((uint32)LIST_SIZE(&freePagesList)==0){
//
//		cprintf("line 3\n");
//		if(Pages[0].numOfPages==numOfPages){
//			cprintf("line 1\n");
//			struct PageEle* fir=(struct PageEle*)((uint32)HARD_LIMIT+(4*kilo));
//		Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;
//		Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
//		Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)HARD_LIMIT+(4*kilo);
//			for(uint32 j=(uint32)fir;j<=(uint32)fir+(4*kilo*numOfPages);j+=(4*kilo)){
//											struct FrameInfo* nora_ptr;
//														int ret=allocate_frame(&nora_ptr);
//														if(ret==0){
//															map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE);
//														}
//
//										}
//			//LIST_REMOVE(&freePagesList,LIST_FIRST(&freePagesList));
//			return (void*)((uint32)HARD_LIMIT+(4*kilo));
//		}
//		else if(Pages[0].numOfPages>numOfPages){
//			cprintf("line 2\n");
//			struct PageEle* fir=(struct PageEle*)((uint32)HARD_LIMIT+(4*kilo));
//			cprintf("line 3\n");
//			struct PageEle* ne=(struct PageEle*)((uint32)fir+(4*kilo*numOfPages));
//			cprintf("line 4\n");
//			for(uint32 j=(uint32)fir;j<=(uint32)ne;j+=(4*kilo)){
//
//				struct FrameInfo* nora_ptr;
//																	int ret=allocate_frame(&nora_ptr);
//																	if(ret==0){
//																		map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE);
//																	}
//
//													}
//			cprintf("line 5\n");
//			Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;
//			cprintf("line 5\n");
//			Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages-numOfPages;
//					Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
//					cprintf("line 5\n");
//					Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)HARD_LIMIT+(4*kilo);
//					cprintf("line 5\n");
//					Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
//					cprintf("line 5\n");
//
//							cprintf("line 5\n");
//							Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)ne;
//							cprintf("line 5\n");
////struct PageEle* f=LIST_FIRST(&freePagesList);
//cprintf("line 5\n");
//		//	LIST_REMOVE(&freePagesList,f);
//			cprintf("line 5\n");
//			LIST_INSERT_HEAD(&freePagesList,ne);
//			cprintf("LIST_HEAD %u \n",(uint32)LIST_FIRST(&freePagesList));
//			cprintf("LIST_HEAD %u \n",Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
//			cprintf("LIST_HEAD %u \n",	Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va);
//			cprintf("line 5\n");
//			return (void*)((uint32)HARD_LIMIT+(4*kilo));
//
//		}
//	}
//	struct PageEle* el=NULL;
//
//	LIST_FOREACH(el,&freePagesList){
//		uint32 numOfSizes=Pages[((uint32)el-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages;
//		cprintf("size %u \n",numOfPages);
//		cprintf("size %u \n",numOfSizes);
//
//		if(numOfSizes==numOfPages)
//		{
//			cprintf("line 1\n");
//			struct PageEle* fir=(struct PageEle*)((uint32)el);
//			Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;
//			Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
//			Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)fir;
//				for(uint32 j=(uint32)fir;j<=(uint32)fir+(4*kilo*numOfPages);j+=(4*kilo)){
//												struct FrameInfo* nora_ptr;
//															int ret=allocate_frame(&nora_ptr);
//															if(ret==0){
//																map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE|PERM_PRESENT);
//															}
//
//											}
//				LIST_REMOVE(&freePagesList,el);
//				return (void*)el;
//		}
//		else if(numOfSizes>numOfPages){
//			cprintf("line 2\n");
//			struct PageEle* fir=(struct PageEle*)((uint32)el);
//					struct PageEle* ne=(struct PageEle*)((uint32)fir+(4*kilo*numOfPages));
//					Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;
//					Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages-numOfPages;
//
//							Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
//							Pages[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)fir;
//      			Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
//				Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)ne;
//					for(uint32 j=(uint32)fir;j<=(uint32)ne;j+=(4*kilo)){
//																struct FrameInfo* nora_ptr;
//																			int ret=allocate_frame(&nora_ptr);
//																			if(ret==0){
//																				map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE|PERM_PRESENT);
//																			}
//
//															}
//					LIST_INSERT_AFTER(&freePagesList,el,ne);
//					LIST_REMOVE(&freePagesList,el);
//
//					return (void*)el;
//		}
//	}
//	for(uint32 i=(uint32 )HARD_LIMIT+(4*kilo);i<(uint32)KERNEL_HEAP_MAX;i+=(4*kilo)){
//		if(Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f){
//			if(Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages==numOfPages){
//				Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;
//
//				for(uint32 j=i;j<=i+(4*kilo*numOfPages);j+=(4*kilo)){
//								struct FrameInfo* nora_ptr;
//											int ret=allocate_frame(&nora_ptr);
//											if(ret==0){
//												map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE|PERM_PRESENT);
//											}
//											Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;
//											Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
//												  Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;
//							}
//				return (void* )i;
//			}
//			else if(Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages>numOfPages){
//				Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;
//				Pages[((uint32)i+(numOfPages*4*kilo)-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages-numOfPages;
//				Pages[((uint32)i+(numOfPages*4*kilo)-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
//				Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
//				for(uint32 j=i;j<=i+(4*kilo*numOfPages);j+=(4*kilo)){
//											struct FrameInfo* nora_ptr;
//														int ret=allocate_frame(&nora_ptr);
//														if(ret==0){
//															map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE|PERM_PRESENT);
//														}
//														Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=0;
//																								Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
//																									  Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;
//										}
//				return (void* )i;
//			}
//
//		}
		//i+=(Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages*4*kilo);

//	if((uint32)LIST_SIZE(&freePagesList)==1){
//		cprintf("line1 \n");
//
//		cprintf("line2 \n");
//
//
//		for(uint32 i=(uint32 )0;i<=numOfPages;i++){
//			cprintf("line5 \n");
//			struct FrameInfo* nora_ptr;
//											int ret=allocate_frame(&nora_ptr);
//											if(ret==0){
//												map_frame(ptr_page_directory,nora_ptr,((uint32)HARD_LIMIT+(4*kilo)+(i*4*kilo)),PERM_WRITEABLE|PERM_PRESENT);
//											}
//		}struct PageEle *point=(struct PageEle *)(uint32)HARD_LIMIT+(4*kilo);
//		cprintf("line3 \n");
//		point->is_f=1;
//		point->va=(uint32)HARD_LIMIT+(4*kilo);
//		point->lastva=(uint32)KERNEL_HEAP_MAX;
//		point->numOfPages=((uint32)KERNEL_HEAP_MAX-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo);
//		cprintf("jfjdjf %u",point->numOfPages);
//		cprintf("djjdjdj %u",point->va);
//		cprintf("djjdjdj %u",(uint32)HARD_LIMIT+(4*kilo));
//		struct PageEle*ne=(struct PageEle*)((uint32)point->va+(4*(numOfPages)*kilo));
//		cprintf("line4 \n");
//							ne->is_f=1;
//							ne->va=((uint32)point->va+(4*(numOfPages)*kilo));
//							ne->numOfPages=point->numOfPages-numOfPages;
//							ne->lastva=point->lastva;
//							point->is_f=0;
//							point->lastva=(uint32)point->va+(4*kilo*numOfPages);
//							point->numOfPages=numOfPages;
//		LIST_REMOVE(&freePagesList,LIST_FIRST(&freePagesList));
//		LIST_INSERT_HEAD(&freePagesList,point);
//		cprintf("line6 \n");
//		LIST_INSERT_AFTER(&freePagesList,point,ne);
//		cprintf("line7 \n");
//								return (void*)point->va;
//
//	}
//	struct PageEle* el;
//	LIST_FOREACH(el,&freePagesList){
//		cprintf("el %u \n",el);
//		cprintf("accc %u \n",(KERNEL_HEAP_START));
//		cprintf("va %u \n",el->va);
//		cprintf("is  %u \n",el->is_f);
//		cprintf("num %u \n",el->numOfPages);
//		if(el->is_f==1&&el->numOfPages==numOfPages){
//			cprintf("line1\n");
//			el->is_f=0;
//
//			el->lastva=(uint32)el->va+(4*kilo*el->numOfPages);
//
//			return (void*)el->va;
//		}
//		else if(el->is_f==1 && el->numOfPages>numOfPages){
//			cprintf("line2\n");
//						struct PageEle*ne=(struct PageEle*)((uint32)el->va+(4*(numOfPages-1)*kilo));
//						ne->is_f=1;
//						ne->va=((uint32)el->va+(4*(numOfPages)*kilo));
//						ne->numOfPages=el->numOfPages-numOfPages;
//						ne->lastva=el->lastva;
//						el->is_f=0;
//						el->lastva=(uint32)el->va+(4*kilo*numOfPages);
//cprintf("el %u \n",el->va);
//						LIST_INSERT_AFTER(&freePagesList,el,ne);
//						return (void*)el->va;
//
//		}
//	}
//	for(uint32 i=0;i<50000;i++){
//		if(Pages[i].numOfPages==numOfPages&&Pages[i].is_f==1){
//			struct PageEle*ne=(struct PageEle*)((uint32)Pages[i].va+(4*numOfPages*kilo));
//			for(uint32 j=Pages[i].va;j<Pages[i].lastva;j+=(4*kilo)){
//				struct FrameInfo* nora_ptr;
//							int ret=allocate_frame(&nora_ptr);
//							if(ret==0){
//								map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE|PERM_PRESENT);
//							}
//			}
//			Pages[i].is_f=0;
//
//			Pages[i].lastva=(uint32)ne;
//			return (void* )Pages[i].va;
//		}
//		else if(Pages[i].numOfPages>numOfPages&&Pages[i].is_f==1){
//			struct PageEle*ne=(struct PageEle*)((uint32)Pages[i].va+(4*numOfPages*kilo));
//			for(uint32 j=Pages[i].va;j<Pages[i].va+(4*numOfPages*kilo);j+=(4*kilo)){
//						struct FrameInfo* nora_ptr;
//									int ret=allocate_frame(&nora_ptr);
//									if(ret==0){
//										map_frame(ptr_page_directory,nora_ptr,j,PERM_WRITEABLE|PERM_PRESENT);
//									}
//					}
//			Pages[i].is_f=0;
//
//		Pages[i+1].va=(uint32)ne;
//		Pages[i+1].lastva=Pages[i].lastva;
//		Pages[i].lastva=(uint32)ne;
//			Pages[i+1].numOfPages+=(Pages[i].numOfPages-numOfPages);
//			Pages[i+1].is_f=1;
//			return (void* )Pages[i].va;
//		}
//	}
//	if((uint32)LIST_FIRST(&freePagesList)==(uint32)KERNEL_HEAP_START){
//		struct PageEle* fir=(struct PageEle*)((uint32)HARD_LIMIT+(4*kilo));
//		struct PageEle*ne=(struct PageEle*)((uint32)fir+(4*numOfPages*kilo));
//		cprintf("fir %u \n",fir);
//		cprintf("ne %u \n",ne);
//		cprintf("fir %u \n",(4*numOfPages*kilo));
//		cprintf("ne %u \n",((uint32)ne-(uint32)fir));
//		cprintf("line1\n");
//		for(uint32 i=(uint32)fir;i<=(uint32)ne;i+=(4*kilo)){
//
//			struct FrameInfo* nora_ptr;
//			int ret=allocate_frame(&nora_ptr);
//			if(ret==0){
//				map_frame(ptr_page_directory,nora_ptr,i,PERM_WRITEABLE);
//			}
//		}
//
//		pages_sizes[((uint32)fir-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=numOfPages;
//		pages_sizes[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=((uint32)KERNEL_HEAP_MAX-((uint32)HARD_LIMIT))/(4*kilo)-numOfPages;
//		LIST_REMOVE(&freePagesList,fir);
//		LIST_INSERT_HEAD(&freePagesList,(struct PageEle*)((uint32)ne));
//		return(void *)fir;
//	}
//	else {
//		cprintf("line2\n");
//	struct PageEle* el=NULL;
//	LIST_FOREACH(el,&freePagesList){
//		cprintf("line3\n");
//		uint32 numOfSizes=pages_sizes[((uint32)el-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)];
//		cprintf("line4\n");
//		if(numOfSizes==numOfPages){
//			cprintf("line5\n");
//			struct PageEle*ne=(struct PageEle*)((uint32)el+(4*numOfPages*kilo));
//			for(uint32 i=(uint32)el;i<=(uint32)ne;i+=(4*kilo)){
//
//						struct FrameInfo* nora_ptr;
//						int ret=allocate_frame(&nora_ptr);
//						if(ret==0){
//							map_frame(ptr_page_directory,nora_ptr,i,PERM_WRITEABLE);
//						}
//					}
//			LIST_REMOVE(&freePagesList,(struct PageEle*)((uint32)el));
//			return (void*)((uint32)el);
//		}
//		else if(numOfSizes>numOfPages){
//			cprintf("line6\n");
//			uint32 rem=numOfSizes-numOfPages;
//			//i dont have next
//
//
//				struct PageEle*ne=(struct PageEle*)((uint32)el+(4*numOfPages*kilo));
//				cprintf("line7\n");
//				for(uint32 i=(uint32)el;i<=(uint32)ne;i+=(4*kilo)){
//
//									struct FrameInfo* nora_ptr;
//									int ret=allocate_frame(&nora_ptr);
//									if(ret==0){
//										map_frame(ptr_page_directory,nora_ptr,i,PERM_WRITEABLE);
//									}
//								}
//				cprintf("line8\n");
//				pages_sizes[((uint32)el-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=numOfPages;
//				pages_sizes[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=rem;
//				cprintf("line9\n");
//				LIST_INSERT_AFTER(&freePagesList,(struct PageEle*)((uint32)el),(struct PageEle*)((uint32)ne));
//				cprintf("line10\n");
//				LIST_REMOVE(&freePagesList,(struct PageEle*)((uint32)el));
//				return (void*)((uint32)el);
//
//
//		    // i have free next in front of me
//
//
//
//			//i have next but its not the next to me
//		}
//	}
//	}



	//cprintf("nooooo kheap1 \n");



}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT'24.MS2 - #04] [1] KERNEL HEAP - kfree
	// Write your code here, remove the panic and write your code
	//panic("kfree() is not implemented yet...!!");

	//you need to get the size of the given allocation using its address
	//refer to the project presentation and documentation for details

	//first find the size then /4
	//for the size loop unmap and free
	//if the page next to me free then
	//   set my size to it and remove it from free pages
	//else
	//   set me to size
	//go to frames pages and set it to -1
	//insert to free pages list

//	struct spinlock ly;
//		char* name = "kys";
//		init_spinlock(&(ly) , name);

		//acquire_spinlock(&ly);
		//release_spinlock(&ly);

if((uint32)virtual_address>=(uint32)START_KHEAP&&(uint32)virtual_address<=(uint32)HARD_LIMIT){
//cprintf("dyn\n");
	return free_block(virtual_address);
}

uint32 s=Pages[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages;


for(uint32 i=(uint32)virtual_address;i<(uint32)virtual_address+(s*4*kilo);i+=(4*kilo)){
	uint32* p=NULL;
	struct FrameInfo *nora_ptr =get_frame_info(ptr_page_directory, i, &p);

	uint32 pa=to_physical_address(nora_ptr);
																	frames_pages[pa/(4*kilo)]=-1;
			 unmap_frame(ptr_page_directory, i);
				Pages[((uint32)i-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;

}
//acquire_spinlock(&ly);
if((uint32)virtual_address==(uint32)HARD_LIMIT+(4*kilo)){
	uint32 ne= (uint32)virtual_address+(4*kilo*s);
	if(Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f){
		s+=(Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
		for(uint32 j=(uint32)virtual_address;j<(uint32)virtual_address+(4*kilo*s);j+=(4*kilo)){
						Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;

								Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)virtual_address;
								Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
					}
	}
}
else if((uint32)virtual_address==(uint32)KERNEL_HEAP_MAX){
	uint32 pre=Pages[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)-1].va;
	if(Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f){
		uint32 last =(uint32)virtual_address+(4*kilo*Pages[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
		s+=(Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
		for(uint32 j=(uint32)pre;j<(uint32)last;j+=(4*kilo)){
							Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;

									Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=pre;
									Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
						}
	}
}
else{
	uint32 ne= (uint32)virtual_address+(4*kilo*s);
	uint32 pre=Pages[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)-1].va;
	if(Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f&&Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f){
		s+=(Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
		s+=(Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
		for(uint32 j=(uint32)pre;j<(uint32)ne+(4*kilo*Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);j+=(4*kilo)){
								Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;

										Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=pre;
										Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
							}
	}
	else if(Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f){
		uint32 pre=Pages[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)-1].va;
		if(Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f){
			uint32 last =(uint32)virtual_address+(4*kilo*Pages[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
			s+=(Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
			for(uint32 j=(uint32)pre;j<(uint32)last;j+=(4*kilo)){
								Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;

										Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=pre;
										Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
							}
		}
	}
	else if(Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f){
		s+=(Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
			for(uint32 j=(uint32)virtual_address;j<(uint32)virtual_address+(4*kilo*s);j+=(4*kilo)){
							Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;

									Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)virtual_address;
									Pages[((uint32)j-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
						}
	}
}
//release_spinlock(&ly);
return;
//struct PageEle* b=(struct PageEle*)virtual_address;
//uint32 n=Pages[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages;
//struct PageEle* el=NULL;
//bool flag=0;
//cprintf("line1 \n");
//LIST_FOREACH(el,&freePagesList){
//	if((uint32)el>(uint32)virtual_address){
//		LIST_INSERT_BEFORE(&freePagesList,el,(struct PageEle*)virtual_address);
//		flag=1;
//		break;
//	}
//}
//if(!flag){
//	LIST_INSERT_TAIL(&freePagesList,b);
//}
//if(LIST_NEXT(b)!=NULL&&LIST_PREV(b)!=NULL){
//	cprintf("line2 \n");
//	struct PageEle* ne=LIST_NEXT(b);
//	struct PageEle* pre=LIST_PREV(b);
//	uint32 preS=Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages;
//	uint32 neS=Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages;
//	if((uint32)virtual_address+(n*4*kilo)==(uint32)ne&&(uint32)pre+(preS*4*kilo)==(uint32)virtual_address){
//		cprintf("line3 \n");
//		Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages+=(n+neS);
//		LIST_REMOVE(&freePagesList,ne);
//		LIST_REMOVE(&freePagesList,b);
//	}
//	else if((uint32)virtual_address+(n*4*kilo)==(uint32)ne){
//		cprintf("line4 \n");
//		Pages[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages+=(neS);
//			LIST_REMOVE(&freePagesList,ne);
//	}
//	else if((uint32)pre+(preS*4*kilo)==(uint32)virtual_address){
//		cprintf("line5 \n");
//		Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages+=(n);
//		LIST_REMOVE(&freePagesList,b);
//	}
//
//}
//else if(LIST_NEXT(b)!=NULL){
//	cprintf("line6 \n");
//	struct PageEle* ne=LIST_NEXT(b);
//	cprintf("ne %u \n",ne);
//	cprintf("va %u \n",virtual_address);
//	uint32 neS=Pages[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages;
//	cprintf("size %u \n",n);
//	cprintf("tes %u \n",(uint32)virtual_address+(n*4*kilo));
//	if((uint32)virtual_address+(n*4*kilo)==(uint32)ne){
//		cprintf("line7 \n");
//			Pages[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages+=(neS);
//				LIST_REMOVE(&freePagesList,ne);
//		}
//}
//else if(LIST_PREV(b)!=NULL){
//	cprintf("line8 \n");
//	struct PageEle* pre=LIST_PREV(b);
//	uint32 preS=Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages;
//	if((uint32)pre+(preS*4*kilo)==(uint32)virtual_address){
//		cprintf("line9 \n");
//			Pages[((uint32)pre-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages+=(n);
//			LIST_REMOVE(&freePagesList,b);
//		}
//
//}
//for(uint32 i=(uint32)virtual_address;i<(uint32)virtual_address+(n*4*kilo);i+=(4*kilo)){
//	cprintf("line10 \n");
//			 unmap_frame(ptr_page_directory, i);
//}
//uint32 neInd=((uint32)virtual_address+Pages[virtual_address].numOfPages*(4*kilo)-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo);
//for(uint32 i=0;i<50000;i++){
//	if(Pages[i].va==(uint32)virtual_address){
//		cprintf("yaaaaaaaaaat\n");
//		struct PageEle*ne=(struct PageEle*)((uint32)virtual_address+(4*Pages[i].numOfPages*kilo));
//			cprintf("djjdjd %u\n",((uint32) (ne-Pages[i].va)/(4*kilo) ));
//		for(uint32 j=Pages[i].va;j<Pages[i].lastva;j+=(4*kilo)){
//
//			 unmap_frame(ptr_page_directory, j);
//
//
//			}
//Pages[i].is_f=1;
//		if(i!=0&&Pages[i-1].is_f==1&&Pages[i+1].is_f==1){
//			Pages[i-1].numOfPages+=(Pages[i].numOfPages+Pages[i+1].numOfPages);
//			Pages[i-1].lastva=Pages[i+1].lastva;
//
//			Pages[i].numOfPages=0;
//			Pages[i].is_f=0;
//			Pages[i+1].numOfPages=0;
//			Pages[i+1].is_f=0;
//		}
//		else if(i!=0&&Pages[i-1].is_f==1){
//			Pages[i-1].numOfPages+=(Pages[i].numOfPages);
//			Pages[i-1].lastva=Pages[i].lastva;
//			Pages[i].numOfPages=0;
//						Pages[i].is_f=0;
//		}
//		else if(Pages[i+1].is_f==1){
//			Pages[i].numOfPages+=(Pages[i+1].numOfPages);
//			Pages[i].lastva=Pages[i+1].lastva;
//
//						Pages[i+1].numOfPages=0;
//						Pages[i+1].is_f=0;
//
//		}
//break;
//	}
//}
//cprintf("djjdjdjjd %u \n",((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo));
//struct PageEle* block=(struct PageEle*)((uint32)virtual_address);
//
//uint32 numOfPages=pages_sizes[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)];
//
//struct PageEle* el=	NULL;
//bool flag=0;
//if(LIST_SIZE(&freePagesList)==0){
//	LIST_INSERT_HEAD(&freePagesList,(struct PageEle*)((uint32)block));
//}
//else{
//LIST_FOREACH(el,&freePagesList){
//	cprintf("va %u \n",HARD_LIMIT+(4*kilo));
//	if((uint32)el>(uint32)virtual_address){
//		cprintf("line1\n");
//		cprintf("el %u\n",el);
//		cprintf("el %u\n",(struct PageEle*)((uint32)block));
//		if(el==LIST_FIRST(&freePagesList)){
//			LIST_INSERT_HEAD(&freePagesList,(struct PageEle*)((uint32)block));
//		}
//		else
//		LIST_INSERT_BEFORE(&freePagesList,el,(struct PageEle*)((uint32)block));
//		flag=1;
//		break;
//	}
//}
//if(!flag){
//	LIST_INSERT_TAIL(&freePagesList,(struct PageEle *)((uint32)virtual_address));
//}}
//if(LIST_NEXT((struct PageEle*)((uint32)block))!=NULL&&LIST_PREV((struct PageEle*)((uint32)block))!=NULL){
//	cprintf("line2\n");
//	uint32 neSize=pages_sizes[((uint32)LIST_NEXT((struct PageEle*)((uint32)block))-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)];
//	uint32 preSize=pages_sizes[((uint32)LIST_PREV((struct PageEle*)((uint32)block))-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)];
//	struct PageEle*ne=(struct PageEle*)((uint32)block+(4*numOfPages*kilo));
//	struct PageEle*pre=(struct PageEle*)((uint32)block-(4*preSize*kilo));
//if((uint32)LIST_NEXT((struct PageEle*)((uint32)block))==(uint32)ne&&(uint32)LIST_PREV((struct PageEle*)((uint32)block))==(uint32)pre){
//	cprintf("line3\n");
//	pages_sizes[((uint32)LIST_PREV((struct PageEle*)((uint32)block))-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]+=(neSize+numOfPages);
//	pages_sizes[((uint32)LIST_NEXT((struct PageEle*)((uint32)block))-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=0;
//	pages_sizes[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=0;
//	LIST_REMOVE(&freePagesList,(struct PageEle*)((uint32)block));
//	LIST_REMOVE(&freePagesList,(struct PageEle*)((uint32)ne));
//}
//else if((uint32)LIST_NEXT((struct PageEle*)((uint32)block))==(uint32)ne){
//	cprintf("line4\n");
//	pages_sizes[((uint32)LIST_NEXT((struct PageEle*)((uint32)block))-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=0;
//	pages_sizes[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]+=(neSize);
//	LIST_REMOVE(&freePagesList,(struct PageEle*)((uint32)ne));
//}
//else if((uint32)LIST_PREV((struct PageEle*)((uint32)block))==(uint32)pre){
//	cprintf("line5\n");
//	pages_sizes[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=0;
//	pages_sizes[((uint32)LIST_PREV((struct PageEle*)((uint32)block))-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]+=(numOfPages);
//	LIST_REMOVE(&freePagesList,(struct PageEle*)((uint32)block));
//}
//}
//else if(LIST_NEXT((struct PageEle*)((uint32)block))!=NULL){
//	cprintf("line6\n");
//
//	struct PageEle*ne=(struct PageEle*)((uint32)block+(4*numOfPages*kilo));
//	cprintf("list next %u \n",((uint32)LIST_NEXT((struct PageEle*)((uint32)block))-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo));
//	cprintf("list next %u \n",ne);
//	uint32 neSize=pages_sizes[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)];
//		 cprintf("line7\n");
//		pages_sizes[((uint32)ne-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=0;
//		pages_sizes[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]+=(neSize);
//		LIST_REMOVE(&freePagesList,(struct PageEle*)((uint32)ne));
//
//}
//else if(LIST_PREV((struct PageEle*)((uint32)block))!=NULL){
//	cprintf("line8\n");
//	uint32 preSize=pages_sizes[((uint32)LIST_PREV((struct PageEle*)((uint32)block))-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)];
//	struct PageEle*pre=(struct PageEle*)((uint32)block-(4*preSize*kilo));
//	if((uint32)LIST_PREV((struct PageEle*)((uint32)block))==(uint32)pre){
//		cprintf("line9\n");
//		pages_sizes[((uint32)virtual_address-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]=0;
//		pages_sizes[((uint32)LIST_PREV((struct PageEle*)((uint32)block))-((uint32)HARD_LIMIT+(4*kilo)))/(4*kilo)]+=(numOfPages);
//		LIST_REMOVE(&freePagesList,(struct PageEle*)((uint32)block));
//	}
//}
//struct PageEle*ne=(struct PageEle*)((uint32)block+(4*numOfPages*kilo));
//for(uint32 i=(uint32)block;i<=(uint32)ne;i+=(4*kilo)){
//
//	 unmap_frame(ptr_page_directory, i);
//	}

}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT'24.MS2 - #05] [1] KERNEL HEAP - kheap_physical_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");
	// uint32 direc=PDX(virtual_address);
	uint32 direc=PDX(virtual_address);
		uint32 page_entry=PTX(virtual_address);

	//	intially puting the ptr-PAGE_directory by null so i can return a an adds in it
		uint32 *ptr_page_table=NULL;
		get_page_table(ptr_page_directory,(int)virtual_address, &ptr_page_table);

		 if(ptr_page_table != NULL){
			 uint32 framenum=ptr_page_table [page_entry];
			 if (framenum & 0x1) {  // Check present flag
			             // Get the physical address by extracting the frame and adding the offset
			             uint32 my_physical = (framenum & 0xFFFFF000);  // Extract the frame
			             uint32 offset = virtual_address & 0xFFF;  // Get the offset within the page
			             return my_physical + offset;}

		 }
			 return 0;
	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details

	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT'24.MS2 - #06] [1] KERNEL HEAP - kheap_virtual_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_virtual_address() is not implemented yet...!!");

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details
	if(frames_pages[physical_address/(4*kilo)]==-1){
		return 0;
	}
return frames_pages[physical_address/(4*kilo)]+(physical_address & 0xFFF);
//	struct FrameInfo* f=to_frame_info(physical_address);

	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
}
//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, if moved to another loc: the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT'24.MS2 - BONUS#1] [1] KERNEL HEAP - krealloc
	// Write your code here, remove the panic and write your code
	return NULL;
	panic("krealloc() is not implemented yet...!!");
}
