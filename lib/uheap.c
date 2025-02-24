#include <inc/lib.h>
#define kilo (1024)
//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
struct user_Page {
	int numOfPages;
	uint32 va;
	bool is_f;
	bool is_m;
uint32 kmasked;
uint32 kunmasked;
} user_Pages[122880];
//=============================================
// [1] CHANGE THE BREAK LIMIT OF THE USER HEAP:
//=============================================
/*2023*/
bool nour=1;
void* sbrk(int increment)
{
	return (void*) sys_sbrk(increment);
}

//=================================
// [2] ALLOCATE SPACE IN USER HEAP:
//=================================
void* malloc(uint32 size)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	if (size == 0) return NULL ;
	//==============================================================
	//TODO: [PROJECT'24.MS2 - #12] [3] USER HEAP [USER SIDE] - malloc()
	// Write your code here, remove the panic and write your code
//	panic("malloc() is not implemented yet...!!");
	uint32 numOfPages=size/(4*kilo);
	if(size%(4*kilo)!=0){
		numOfPages++;
	}
//cprintf("3aaaaaaaaa %d \n",((uint32)USER_HEAP_MAX-(uint32)myEnv->USER_HARD_LIMIT)/(4*kilo));
		if(numOfPages>((uint32)USER_HEAP_MAX-(uint32)myEnv->USER_HARD_LIMIT)/(4*kilo)){
			//cprintf("nooooo uheap \n");

			return NULL;
		}
		if(size<=DYN_ALLOC_MAX_BLOCK_SIZE){

	return alloc_block_FF(size);


		}
//cprintf("nour %d \n",nour);
if(nour==1){
		for(uint32 i= (uint32)myEnv->USER_HARD_LIMIT+(4*kilo);i<(uint32)USER_HEAP_MAX;i+=(4*kilo)){

			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=((uint32)USER_HEAP_MAX-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo);

			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)myEnv->USER_HARD_LIMIT+(4*kilo);
			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
		}
	nour=0;
}


		for(uint32 i=(uint32)myEnv->USER_HARD_LIMIT+(4*kilo);i<(uint32)USER_HEAP_MAX;i+=(4*kilo)){

			if(!(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m)){
if(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages==numOfPages){
	for(uint32 j=(uint32)i;j<(uint32)i+(4*kilo*numOfPages);j+=(4*kilo)){

									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=1;
									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;


																	}
				sys_allocate_user_mem(i,size);
return (void*)i;
}
else if(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages>numOfPages){
	uint32 ne=i+(4*kilo*numOfPages);
					uint32 sz=user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages-numOfPages;
					//cprintf("ldlldld %u \n",sz);
					uint32 last =i+(4*kilo*user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
					for(uint32 j=ne;j<last;j+=(4*kilo)){
						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=sz;

						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=ne;
						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
					}
					for(uint32 j=(uint32)i;j<(uint32)i+(4*kilo*numOfPages);j+=(4*kilo)){

									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=1;
									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;


																	}

					sys_allocate_user_mem(i, size);
					return (void*)i;
}
				}

			}
		//cprintf("nooooo uheap \n");

		return NULL;
	//Use sys_isUHeapPlacementStrategyFIRSTFIT() and	sys_isUHeapPlacementStrategyBESTFIT()
	//to check the current strategy

}

//=================================
// [3] FREE SPACE FROM USER HEAP:
//=================================
void free(void* virtual_address)
{
	//TODO: [PROJECT'24.MS2 - #14] [3] USER HEAP [USER SIDE] - free()
	// Write your code here, remove the panic and write your code
	//panic("free() is not implemented yet...!!");
if((uint32)virtual_address>=(uint32)myEnv->USER_HEAP_DASTART&&(uint32)virtual_address<(uint32)myEnv->USER_BREAK){
	free_block(virtual_address);
	//get_page_table()
}
else if((uint32)virtual_address>=(uint32)myEnv->USER_HARD_LIMIT+(PAGE_SIZE)&&(uint32)virtual_address<USER_HEAP_MAX){
	uint32 s=user_Pages[((uint32)virtual_address-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages;
	for(uint32 i=(uint32)virtual_address;i<(uint32)virtual_address+(s*4*kilo);i+=(4*kilo)){
		user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
	}
	sys_free_user_mem((uint32)virtual_address,(s*PAGE_SIZE));
	if((uint32)virtual_address==(uint32)myEnv->USER_HARD_LIMIT+(4*kilo)){
		uint32 ne= (uint32)virtual_address+(4*kilo*s);
	if(!user_Pages[((uint32)ne-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m){
		s+=(user_Pages[((uint32)ne-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
		for(uint32 i=(uint32)virtual_address;i<(uint32)virtual_address+(4*kilo*s);i+=(PAGE_SIZE)){
			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;
			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)virtual_address;



		}
	}
	}
	else if((uint32)virtual_address==(uint32)USER_HEAP_MAX){
		uint32 pre=user_Pages[((uint32)virtual_address-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)-1].va;
		if(		!user_Pages[((uint32)pre-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m){
			s+=(user_Pages[((uint32)pre-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
			for(uint32 i=(uint32)pre;i<pre+(s*PAGE_SIZE);i+=(PAGE_SIZE)){
				user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
				user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;
				user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=pre;
			}

		}
		}

	else{
		uint32 ne= (uint32)virtual_address+(4*kilo*s);
		uint32 pre=user_Pages[((uint32)virtual_address-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)-1].va;
				if(!user_Pages[((uint32)ne-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m&&!user_Pages[((uint32)pre-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m){
			s+=(user_Pages[((uint32)pre-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
			s+=(user_Pages[((uint32)ne-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
			for(uint32 j=(uint32)pre;j<(uint32)pre+(4*kilo*s);j+=(4*kilo)){
				user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
							user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;
							user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=pre;
								}
		}
		else if(!user_Pages[((uint32)pre-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m){
			uint32 pre=user_Pages[((uint32)virtual_address-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)-1].va;

				s+=(user_Pages[((uint32)pre-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
				for(uint32 j=(uint32)pre;j<(uint32)pre+(PAGE_SIZE*s);j+=(4*kilo)){
					user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
								user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;
								user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=pre;
								}

		}
		else if(!user_Pages[((uint32)ne-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m){
			s+=(user_Pages[((uint32)ne-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
				for(uint32 j=(uint32)virtual_address;j<(uint32)virtual_address+(4*kilo*s);j+=(4*kilo)){
					user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
											user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=s;
											user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)virtual_address;
							}
		}
	}

}
else{
	panic("3aaaaaaa");

}
}


//=================================
// [4] ALLOCATE SHARED VARIABLE:
//=================================
void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	//==============================================================
	    //DON'T CHANGE THIS CODE========================================
	    if (size == 0) return NULL ;
	    //==============================================================
	    //TODO: [PROJECT'24.MS2 - #18] [4] SHARED MEMORY [USER SIDE] - smalloc()
	    // Write your code here, remove the panic and write your code
	    //panic("smalloc() is not implemented yet...!!");

		//struct spinlock ly;
		char* name = "lk";
		//init_spinlock(&(ly) , name);

		 //acquire_spinlock(&ly);
		 //release_spinlock(&ly);
//	    uint32* lal = malloc(size);
//	    if(lal == NULL)
//	    	return NULL;
//
//	    sys_createSharedObject(sharedVarName, size, isWritable,lal);
//	    return (void*) lal;

	    int numOfPages = size/(PAGE_SIZE);
	    if(size%(PAGE_SIZE)!=0)
	    {
	    	numOfPages++;
	    }

	    if(numOfPages>=((uint32)USER_HEAP_MAX-(uint32)myEnv->USER_HARD_LIMIT)/(PAGE_SIZE))
	    {
	        return NULL;
	    }

//	    if(nour==1){
//	    		for(uint32 i= (uint32)myEnv->USER_HARD_LIMIT+(4*kilo);i<(uint32)USER_HEAP_MAX;i+=(4*kilo)){
//
//	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=((uint32)USER_HEAP_MAX-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo);
//
//	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)myEnv->USER_HARD_LIMIT+(4*kilo);
//	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
//	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
//	    		}
//	    	nour=0;
//	    }
//
//	    for(uint32 i=(uint32)myEnv->USER_HARD_LIMIT+(PAGE_SIZE);i<(uint32)USER_HEAP_MAX;i+=(PAGE_SIZE))
//	    {
//	        if(!(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(PAGE_SIZE)))/(PAGE_SIZE)].is_m))
//	        {
//	        	cprintf("yayayayaya1");
//	            if(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(PAGE_SIZE)))/(PAGE_SIZE)].numOfPages==nPage)
//	            {
//	            	cprintf("rururururur1");
//	                sys_createSharedObject(sharedVarName, size, isWritable,(void*)i);
//	                cprintf("heeeeeeeeeeeeeeeeeeeeeeee %u\n" , i);
//	                return (void*)i;
//	            }
//	            else if(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages>nPage){
//	            	uint32 ne=i+(4*kilo*nPage);
//	            					uint32 sz=user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages-nPage;
//	            					//cprintf("ldlldld %u \n",sz);
//	            					uint32 last =i+(4*kilo*user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
//	            					for(uint32 j=ne;j<last;j+=(4*kilo)){
//	            						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=sz;
//
//	            						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=ne;
//	            						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
//	            					}
//
//	           	            sys_createSharedObject(sharedVarName,size,isWritable,(void*)i);
//	           	         cprintf("laaaaaaaaaaaaaaaaaaaaaaaa %u\n" , i);
//	           	            return (void*)i;
//	           	        }
//
//	        }
//
//
//	    }
//	    cprintf("nullllllllllllllll\n");
//	            return NULL;
	    if(nour==1){
	    		for(uint32 i= (uint32)myEnv->USER_HARD_LIMIT+(4*kilo);i<(uint32)USER_HEAP_MAX;i+=(4*kilo)){

	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=((uint32)USER_HEAP_MAX-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo);

	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)myEnv->USER_HARD_LIMIT+(4*kilo);
	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
	    		}
	    	nour=0;
	    }

		// acquire_spinlock(&ly);
	    		for(uint32 i=(uint32)myEnv->USER_HARD_LIMIT+(4*kilo);i<(uint32)USER_HEAP_MAX;i+=(4*kilo)){

	    			if(!(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m)){
	    if(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages==numOfPages){
	    	for(uint32 j=(uint32)i;j<(uint32)i+(4*kilo*numOfPages);j+=(4*kilo)){

	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=1;
	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;


	    																	}
	    	int ret= sys_createSharedObject(sharedVarName, size, isWritable,(void*)i);
	    	if(ret==E_SHARED_MEM_EXISTS||ret==E_NO_SHARE)return NULL;
	    	user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].kmasked=ret;

	   	// release_spinlock(&ly);
	    	 return (void*)i;
	    }
	    else if(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages>numOfPages){
	    	uint32 ne=i+(4*kilo*numOfPages);
	    					uint32 sz=user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages-numOfPages;
	    					//cprintf("ldlldld %u \n",sz);
	    					uint32 last =i+(4*kilo*user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
	    					for(uint32 j=ne;j<last;j+=(4*kilo)){
	    						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=sz;

	    						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=ne;
	    						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
	    					}
	    					for(uint32 j=(uint32)i;j<(uint32)i+(4*kilo*numOfPages);j+=(4*kilo)){

	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=1;
	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;


	    																	}

	    					int ret= sys_createSharedObject(sharedVarName, size, isWritable,(void*)i);
	    			    	if(ret==E_SHARED_MEM_EXISTS||ret==E_NO_SHARE)return NULL;
	    			    	user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].kmasked=ret;

	    			   	// release_spinlock(&ly);
	    					 return (void*)i;
	    }
	    				}

	    			}
	    		//cprintf("nooooo uheap \n");
	    	//	 release_spinlock(&ly);
	    		return NULL;
}
//========================================
// [5] SHARE ON ALLOCATED SHARED VARIABLE:
//========================================
void* sget(int32 ownerEnvID, char *sharedVarName)
{
	//TODO: [PROJECT'24.MS2 - #20] [4] SHARED MEMORY [USER SIDE] - sget()
	// Write your code here, remove the panic and write your code
	//panic("sget() is not implemented yet...!!");
	//return NULL;
	//int heapSize=USER_HEAP_MAX-USER_HEAP_START;
//	char heapAllocation[(USER_HEAP_MAX-USER_HEAP_START) / PAGE_SIZE] = {0};
	int sizeShared=sys_getSizeOfSharedObject(ownerEnvID,sharedVarName);
	//int pagesNeeded=(sizeShared+PAGE_SIZE-1)/PAGE_SIZE;

	if (sizeShared<=0){
		return NULL;
	}
	int numOfPages = sizeShared/(PAGE_SIZE);
		    if(sizeShared%(PAGE_SIZE)!=0)
		    {
		    	numOfPages++;
		    }

		    if(numOfPages>=((uint32)USER_HEAP_MAX-(uint32)myEnv->USER_HARD_LIMIT)/(PAGE_SIZE))
		    {
		        return NULL;
		    }
//	for (int i=0;i<sizeof(heapAllocation);i+=PAGE_SIZE){
//		int isFree=1;
//		for (int j=0;j<pagesNeeded;j++){
//			if(i+j>=sizeof(heapAllocation)||heapAllocation[i+j]!=0){
//				isFree=0;
//				break;
//			}
//		}
//
//
//	if (isFree){
//		for (int j=0;j<pagesNeeded;j++){
//			heapAllocation[i+j]=1;
//		}
//		void *virtualAddress = (void *)(USER_HEAP_START + i * PAGE_SIZE);
//
//		int result = sys_getSharedObject(ownerEnvID, sharedVarName, virtualAddress);
//
//		if (result != E_SHARED_MEM_NOT_EXISTS ) {
//		    return virtualAddress;
//			  } else {
//			for (int j = 0; j < pagesNeeded; j++) {
//				heapAllocation[i + j] = 0;
//			}
//			return NULL;
//						}
//					}
//				}
//		    return NULL;
//		}
		    if(nour==1){
		  	    		for(uint32 i= (uint32)myEnv->USER_HARD_LIMIT+(4*kilo);i<(uint32)USER_HEAP_MAX;i+=(4*kilo)){

		  	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=((uint32)USER_HEAP_MAX-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo);

		  	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=(uint32)myEnv->USER_HARD_LIMIT+(4*kilo);
		  	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_f=1;
		  	    			user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
		  	    		}
		  	    	nour=0;
		  	    }


		  	    		for(uint32 i=(uint32)myEnv->USER_HARD_LIMIT+(4*kilo);i<(uint32)USER_HEAP_MAX;i+=(4*kilo)){

		  	    			if(!(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m)){
		  	    if(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages==numOfPages){
		  	    	for(uint32 j=(uint32)i;j<(uint32)i+(4*kilo*numOfPages);j+=(4*kilo)){

		  	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
		  	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=1;
		  	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;


		  	    																	}
		  	    		int result = sys_getSharedObject(ownerEnvID, sharedVarName,(void*) i);
		  	    				if (result == E_SHARED_MEM_NOT_EXISTS ) {
		  	    				    return NULL;
		  	    					  }
		  	    			//	cprintf("sget i %u\n",i);
		  	    		    	user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].kmasked=result;
		  	    	 return (void*)i;
		  	    }
		  	    else if(user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages>numOfPages){
		  	    	uint32 ne=i+(4*kilo*numOfPages);
		  	    					uint32 sz=user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages-numOfPages;
		  	    					//cprintf("ldlldld %u \n",sz);
		  	    					uint32 last =i+(4*kilo*user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages);
		  	    					for(uint32 j=ne;j<last;j+=(4*kilo)){
		  	    						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=sz;

		  	    						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=ne;
		  	    						user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=0;
		  	    					}
		  	    					for(uint32 j=(uint32)i;j<(uint32)i+(4*kilo*numOfPages);j+=(4*kilo)){

		  	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].numOfPages=numOfPages;
		  	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].is_m=1;
		  	    									user_Pages[((uint32)j-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].va=i;


		  	    																	}
		  	    					int result = sys_getSharedObject(ownerEnvID, sharedVarName,(void*) i);
		  	    					  	    				if (result == E_SHARED_MEM_NOT_EXISTS ) {
		  	    					  	    				    return NULL;
		  	    					  	    					  }
		  	    					  	    			//cprintf("sget i %u\n",i);
		  	    					  	    		user_Pages[((uint32)i-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].kmasked=result;
		  	    					  	    	 return (void*)i;
		  	    }
		  	    				}

		  	    			}
		  	    		//cprintf("nooooo uheap \n");

		  	    		return NULL;
}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//=================================
// FREE SHARED VARIABLE:
//=================================
//	This function frees the shared variable at the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from main memory then switch back to the user again.
//
//	use sys_freeSharedObject(...); which switches to the kernel mode,
//	calls freeSharedObject(...) in "shared_memory_manager.c", then switch back to the user mode here
//	the freeSharedObject() function is empty, make sure to implement it.

void sfree(void* virtual_address)
{
	//TODO: [PROJECT'24.MS2 - BONUS#4] [4] SHARED MEMORY [USER SIDE] - sfree()
	// Write your code here, remove the panic and write your code
//	panic("sfree() is not implemented yet...!!");

	uint32 maskMSB = 0x7FFFFFFF;
	uint32 mva =    	user_Pages[((uint32)virtual_address-((uint32)myEnv->USER_HARD_LIMIT+(4*kilo)))/(4*kilo)].kmasked;
	//cprintf("sfree\n");
//	free(virtual_address);
//	cprintf("sfree object %u\n",mva);
//	cprintf("sfree object va %u\n",(uint32)virtual_address);
sys_freeSharedObject((uint32)mva,virtual_address);




}


//=================================
// REALLOC USER SPACE:
//=================================
//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_move_user_mem(...)
//		which switches to the kernel mode, calls move_user_mem(...)
//		in "kern/mem/chunk_operations.c", then switch back to the user mode here
//	the move_user_mem() function is empty, make sure to implement it.
void *realloc(void *virtual_address, uint32 new_size)
{
	//[PROJECT]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");
	return NULL;

}


//==================================================================================//
//========================== MODIFICATION FUNCTIONS ================================//
//==================================================================================//

void expand(uint32 newSize)
{
	panic("Not Implemented");

}
void shrink(uint32 newSize)
{
	panic("Not Implemented");

}
void freeHeap(void* virtual_address)
{
	panic("Not Implemented");

}
