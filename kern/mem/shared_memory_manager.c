#include <inc/memlayout.h>
#include "shared_memory_manager.h"

#include <inc/mmu.h>
#include <inc/error.h>
#include <inc/string.h>
#include <inc/assert.h>
#include <inc/queue.h>
#include <inc/environment_definitions.h>

#include <kern/proc/user_environment.h>
#include <kern/trap/syscall.h>
#include "kheap.h"
#include "memory_manager.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

bool yaya = 0;
struct spinlock sp;

struct Share* get_share(int32 ownerID, char* name);

//===========================
// [1] INITIALIZE SHARES:
//===========================
//Initialize the list and the corresponding lock
void sharing_init()
{
#if USE_KHEAP
	LIST_INIT(&AllShares.shares_list) ;
	init_spinlock(&AllShares.shareslock, "shares lock");
#else
	panic("not handled when KERN HEAP is disabled");
#endif
}

//==============================
// [2] Get Size of Share Object:
//==============================
int getSizeOfSharedObject(int32 ownerID, char* shareName)
{
	//[PROJECT'24.MS2] DONE
	// This function should return the size of the given shared object
	// RETURN:
	//	a) If found, return size of shared object
	//	b) Else, return E_SHARED_MEM_NOT_EXISTS
	//
	struct Share* ptr_share = get_share(ownerID, shareName);
	if (ptr_share == NULL)
		return E_SHARED_MEM_NOT_EXISTS;
	else
		return ptr_share->size;

	return 0;
}

//===========================================================


//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
//===========================
// [1] Create frames_storage:
//===========================
// Create the frames_storage and initialize it by 0
inline struct FrameInfo** create_frames_storage(int numOfFrames)
{
	//TODO: [PROJECT'24.MS2 - #16] [4] SHARED MEMORY - create_frames_storage()
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("create_frames_storage is not implemented yet");
	//Your Code is Here...

	struct FrameInfo** arrframe = (struct FrameInfo**)kmalloc(numOfFrames*sizeof(struct FrameInfo*));

	if(arrframe == NULL)
	{
		return NULL;
	}
	for(int i=0 ; i<numOfFrames ; i++)
	{

			arrframe[i] = NULL;

	}

	return arrframe;
}

//=====================================
// [2] Alloc & Initialize Share Object:
//=====================================
//Allocates a new shared object and initialize its member
//It dynamically creates the "framesStorage"
//Return: allocatedObject (pointer to struct Share) passed by reference
struct Share* create_share(int32 ownerID, char* shareName, uint32 size, uint8 isWritable)
{
	//TODO: [PROJECT'24.MS2 - #16] [4] SHARED MEMORY - create_share()
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("create_share is not implemented yet");
	//Your Code is Here...

	//acquire_spinlock(&AllShares.shareslock);
	struct Share* shareeleyaya = (struct Share*)kmalloc(sizeof(struct Share));
	if(shareeleyaya == NULL)
	{
		//release_spinlock(&AllShares.shareslock);
		return NULL;
	}

	uint32 maskMSB = 0x7FFFFFFF;
	uint32 mva = (uint32)shareeleyaya & maskMSB;
//cprintf("created object %u\n",(uint32)mva);
//cprintf("created object %u\n",(uint32)shareeleyaya);
	int nframe = (int)(size/PAGE_SIZE);
	if(size%PAGE_SIZE != 0)
		nframe++;


	shareeleyaya->ownerID = ownerID;
	shareeleyaya->size = size;
	shareeleyaya->isWritable = isWritable;
	strcpy(shareeleyaya->name, shareName);
	shareeleyaya->references = 1;
	shareeleyaya->ID = (int)mva;

	shareeleyaya->framesStorage = create_frames_storage(nframe);

	if((shareeleyaya->framesStorage)== NULL)
	{
		kfree(shareeleyaya);
		//release_spinlock(&AllShares.shareslock);
		return NULL;
	}
	//release_spinlock(&AllShares.shareslock);
	return shareeleyaya;

}

//=============================
// [3] Search for Share Object:
//=============================
//Search for the given shared object in the "shares_list"
//Return:
//	a) if found: ptr to Share object
//	b) else: NULL
struct Share* get_share(int32 ownerID, char* name)
{
	//TODO: [PROJECT'24.MS2 - #17] [4] SHARED MEMORY - get_share()
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
//	panic("get_share is not implemented yet");
// auired locks cuz i tried to access the lists
	struct Share *ptr_to_myobj;

	if(yaya == 0)
	{

	}

	acquire_spinlock(&AllShares.shareslock);
		LIST_FOREACH(ptr_to_myobj, &AllShares.shares_list) {

		if (strncmp(ptr_to_myobj->name,name, sizeof(ptr_to_myobj->name))==0&&(ptr_to_myobj->ownerID ==ownerID)) {
			release_spinlock(&AllShares.shareslock);
			//cprintf("get_share %u\n",(uint32)ptr_to_myobj);
			    	return ptr_to_myobj;

			    }
			    	}
		release_spinlock(&AllShares.shareslock);
		return NULL;

	//Your Code is Here...

}

//=========================
// [4] Create Share Object:
//=========================
int createSharedObject(int32 ownerID, char* shareName, uint32 size, uint8 isWritable, void* virtual_address)
{
	//TODO: [PROJECT'24.MS2 - #19] [4] SHARED MEMORY [KERNEL SIDE] - createSharedObject()
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
//	panic("createSharedObject is not implemented yet");
	//Your Code is Here...

	struct Env* myenv = get_cpu_proc(); //The calling environment

	struct Share *existing_share;


	acquire_spinlock(&AllShares.shareslock);
	LIST_FOREACH(existing_share, &AllShares.shares_list) {

		if (strncmp(existing_share->name,shareName, sizeof(existing_share->name))==0&&(existing_share->ownerID ==ownerID)) {
	    	release_spinlock(&AllShares.shareslock);
	        return E_SHARED_MEM_EXISTS;
	    }
	}

	//	sopusebly intilize the shared object  i think creat_share intilizeand allocate frame
	//	not so sure of the data type

	struct Share * my_obj=create_share(ownerID,shareName,size,isWritable);
	//   add the my_obj to the list of objects

	//  trying to aquire the lock


	// incase of inserting in the end
	//  LIST_INSERT_TAIL(&AllShares.shares_list,my_obj);
	LIST_INSERT_HEAD(&AllShares.shares_list,my_obj);
	  //trying to relese the lock



	int num_frames = (size/PAGE_SIZE);
		if(size%PAGE_SIZE != 0)
		{
			num_frames++;
		}

	for(int i=0;i<num_frames;i++){
	//	 starts with the first frame[i]
		 // allocate required physical space and and map it
		 struct FrameInfo* my_physical_space;
		   int resultof_the_func=allocate_frame(&my_physical_space);
	//	   get the va for the ith frame
		   uint32 VArua = (uint32)virtual_address + (i * PAGE_SIZE);
		 if(resultof_the_func!=0){
			 release_spinlock(&AllShares.shareslock);
			 return E_NO_SHARE;
		  }

//cprintf("vaura %u \n",VArua);
		 map_frame(myenv->env_page_directory,my_physical_space,(uint32)VArua, PERM_WRITEABLE | PERM_USER);
		 my_obj->framesStorage[i] = my_physical_space;


	}

	release_spinlock(&AllShares.shareslock);

	 return my_obj->ID;
//			 & 0x7FFFFFFF; in case i had to mask out the most significant bits
}


//======================
// [5] Get Share Object:
//======================
int getSharedObject(int32 ownerID, char* shareName, void* virtual_address)
{
	//TODO: [PROJECT'24.MS2 - #21] [4] SHARED MEMORY [KERNEL SIDE] - getSharedObject()
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("getSharedObject is not implemented yet");
	//Your Code is Here...
	struct Env* myenv = get_cpu_proc(); //The calling environment
	struct Share*shared=NULL;
	int shared_size= getSizeOfSharedObject(ownerID,shareName);
    int numOfPages = shared_size/(PAGE_SIZE);
	    if(shared_size%(PAGE_SIZE)!=0)
	    {
	    	numOfPages++;
	    }

	acquire_spinlock(&(AllShares.shareslock));

	shared = AllShares.shares_list.lh_first;

	while (shared != NULL) {
	    if (shared->ownerID == ownerID && strcmp(shared->name, shareName) == 0) {
	        break;
	    }

	    shared = shared->prev_next_info.le_next;
	    }


	    if (shared == NULL) {
	        release_spinlock(&(AllShares.shareslock));
	        return E_SHARED_MEM_NOT_EXISTS;
	    }


	    for (int i = 0; i < numOfPages; i++) {
	           struct FrameInfo* frame = shared->framesStorage[i];
	           if(shared->isWritable)
	           map_frame(myenv->env_page_directory, frame, (uint32)(virtual_address + i * PAGE_SIZE), PERM_WRITEABLE|PERM_USER);
	           else{
		           map_frame(myenv->env_page_directory, frame, (uint32)(virtual_address + i * PAGE_SIZE), ~(PERM_WRITEABLE)&PERM_USER);

	           }

	       }

	    shared->references++;

	    release_spinlock(&(AllShares.shareslock));
//cprintf("getsharedobject %u \n",(uint32)shared->ID & 0x7FFFFFFF);
	    return shared->ID & 0x7FFFFFFF;
	}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//==========================
// [B1] Delete Share Object:
//==========================
//delete the given shared object from the "shares_list"
//it should free its framesStorage and the share object itself
void free_share(struct Share* ptrShare)

{
	//TODO: [PROJECT'24.MS2 - BONUS#4] [4] SHARED MEMORY [KERNEL SIDE] - free_share()
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("free_share is not implemented yet");
	//Your Code is Here...
	//cprintf("free_shae\n");
	struct Share *ptr_to_myobj;

//		acquire_spinlock(&AllShares.shareslock);
			LIST_FOREACH(ptr_to_myobj, &AllShares.shares_list) {

			if (strncmp(ptr_to_myobj->name,ptrShare->name, sizeof(ptr_to_myobj->name))==0&&(ptr_to_myobj->ownerID ==ptrShare->ownerID)) {
			//	release_spinlock(&AllShares.shareslock);
				    	LIST_REMOVE(&AllShares.shares_list,ptr_to_myobj);
	kfree(ptrShare->framesStorage);
	kfree(ptrShare);
				    	break;
				    }
				    	}

	//release_spinlock(&AllShares.shareslock);

}
//========================
// [B2] Free Share Object:
//========================
int freeSharedObject(int32 sharedObjectID, void *startVA)
{

		//TODO: [PROJECT'24.MS2 - BONUS#4] [4] SHARED MEMORY [KERNEL SIDE] - freeSharedObject()

		//COMMENT THE FOLLOWING LINE BEFORE START CODING

		//panic("freeSharedObject is not implemented yet");

		//Your Code is Here...
//cprintf("free shred object id %u \n",sharedObjectID);
//cprintf("va %u \n",(uint32)startVA);
struct Share *ptr_to_myobj;


	struct Env* myenv = get_cpu_proc();
		//	acquire_spinlock(&AllShares.shareslock);

				LIST_FOREACH(ptr_to_myobj, &AllShares.shares_list) {

				if ((ptr_to_myobj->ID ==sharedObjectID)) {

//cprintf("line 1\n");


	//cprintf("line 2\n");

//	uint32* p=NULL;
//
//		struct FrameInfo *nora_ptr =get_frame_info(myenv->env_page_directory, (uint32)startVA, &p);
	uint32 sz=getSizeOfSharedObject(ptr_to_myobj->ownerID,ptr_to_myobj->name);
	//cprintf("start %u end %u \n",(uint32)startVA,(uint32)startVA+sizeof(struct Share));
	//kfree(startVA);
for(uint32 i=(uint32)startVA;i<(uint32)startVA+sz;i+=PAGE_SIZE){
//cprintf("noooooo\n");
				 unmap_frame(myenv->env_page_directory, (uint32)i);
				 uint32* ptr_p=NULL;
				 int ret=get_page_table(myenv->env_page_directory,(uint32) i, &ptr_p);

					// cprintf("table empty noooooooo\n");
					 bool found=0;
					 for(int j=0;j>-1024;j--){
int nora=(ptr_p[-1*j] & PERM_PRESENT);
						 if(nora == PERM_PRESENT){
							 found=1;
							 break;
						 }

					 }

if(!found){
	uint32* p1=NULL;
//
				//struct FrameInfo *nora_ptr1 =get_frame_info(myenv->env_page_directory, (uint32)myenv->env_page_directory[PDX(startVA)], &p1);
pd_set_table_unused(myenv->env_page_directory,i);
	pd_clear_page_dir_entry(myenv->env_page_directory, i);

	kfree( ptr_p);
	// free_frame(nora_ptr1 );
// myenv->env_page_directory[PDX(i)]=0;

}
			 }
ptr_to_myobj->references--;
//	cprintf("line 3\n");

	if(ptr_to_myobj->references==0){

		free_share(ptr_to_myobj);

		//tlb_invalidate((uint32*)myenv->env_page_directory, (uint32*)startVA);
		tlbflush();
	}
return 0;
			//	break;
}



					    }



			//	release_spinlock(&AllShares.shareslock);




	return 0;




}
