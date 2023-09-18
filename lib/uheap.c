	#include <inc/lib.h>

	// malloc()
	//	This function use FIRST FIT strategy to allocate space in heap
	//  with the given size and return void pointer to the start of the allocated space

	//	To do this, we need to switch to the kernel, allocate the required space
	//	in Page File then switch back to the user again.
	//
	//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
	//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
	//		"memory_manager.c", then switch back to the user mode here
	//	the allocateMem function is empty, make sure to implement it.

	//==================================================================================//
	//============================ REQUIRED FUNCTIONS ==================================//
	//==================================================================================//
	struct allocate{
	uint32 allocateFrames;
	uint32 start;
};
	int counter3 =0;
	struct free{
	uint32 free_Va;
	uint32 arrOfFreeFrames;

};
	int counter2 = 0;
	uint32 address = USER_HEAP_START;
	struct best{
		uint32 bestOnesadds;
		uint32 bestOnessize;
	};
	struct allocate arrOfAllocate[20000];
	struct free free_info[20000];
	struct best best_info[20000];
	void* malloc(uint32 size)
	{
		//TODO: [PROJECT 2021 - [2] User Heap] malloc() [User Side]
		// Write your code here, remove the panic and write your code
		//panic("malloc() is not implemented yet...!!");
		//This function should find the space of the required range
		//using the BEST FIT strategy
		//refer to the project presentation and documentation for details
		int bests=0;
		int newSize = ROUNDUP(size, PAGE_SIZE);
		int numOfFrames = newSize/PAGE_SIZE;
		uint32 avalibleSize = (USER_HEAP_MAX - address);
		int flag=0;
		if(sizeof(free_info[counter2].arrOfFreeFrames) == 0)
		{
			//start = address;
			sys_allocateMem(address,newSize);
			arrOfAllocate[counter3].allocateFrames= numOfFrames;//put number of frames in array
			arrOfAllocate[counter3].start=address;//put start address in array
			counter3++;
			address+=newSize;
			return (void *)(address-newSize);
		}
		else
		{
			for(int i =0; i<counter2;i++)
			{
				if(free_info[i].arrOfFreeFrames>= numOfFrames)
				{
					flag++;
					best_info[bests].bestOnessize=free_info[i].arrOfFreeFrames;
					best_info[bests].bestOnesadds=free_info[i].free_Va;
					bests++;
				}
			}

		}
		if(flag ==0)
		{
			if(avalibleSize> newSize)
			{
				//start =address;
				sys_allocateMem(address,newSize);

				arrOfAllocate[counter3].allocateFrames= numOfFrames;//put number of frames in array
				arrOfAllocate[counter3].start=address;//put start address in array
				counter3++;
				address+=newSize;
				return (void *)(address-newSize);
			}
			else
			{
				return NULL;
			}
		}

//			//end loop ---
		int min=100000000;
		int index =0;
		int check=0;
		for(int i=0;i<bests;i++)
		{
			check = best_info[i].bestOnessize - numOfFrames;
			if(min > check&&check>=0)
			{
				min = check;
				index=i;

			}
		}
		int var=0;
		for(int i=0;i<counter2;i++)
		{
			if(free_info[i].arrOfFreeFrames==best_info[index].bestOnessize)
			{
				var=i;
			}
		}
		if(free_info[var].arrOfFreeFrames>numOfFrames)
		{
			free_info[var].free_Va= free_info[var].free_Va+(numOfFrames*PAGE_SIZE);
			free_info[var].arrOfFreeFrames=free_info[var].arrOfFreeFrames-numOfFrames;
		}

		//found the best place fitrunrunting with the size
		sys_allocateMem(best_info[index].bestOnesadds,newSize);
		arrOfAllocate[counter3].allocateFrames= numOfFrames;//put number of frames in array
		arrOfAllocate[counter3].start=best_info[index].bestOnesadds; //put start address in array
		counter3++;
		return (void *)best_info[index].bestOnesadds;
	//	}
	}
	// free():
	//	This function frees the allocation of the given virtual_address
	//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
	//	from page file and main memory then switch back to the user again.
	//
	//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
	//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
	//		"memory_manager.c", then switch back to the user mode here
	//	the freeMem function is empty, make sure to implement it.

	void free(void* virtual_address)
	{
		//TODO: [PROJECT 2021 - [2] User Heap] free() [User Side]
		// Write your code here, remove the panic and write your code
		//panic("free() is not implemented yet...!!");
		//you should get the size of the given allocation using its address
		//refer to the project presentation and documentation for details
		int flag=0;
		int index=0;

		for(int i=0;i<counter3;i++)
		{
			//search for the virtual address in array
			if(arrOfAllocate[i].start == (uint32)virtual_address)
			{
				index=i;
				flag =1;
				break;
			}
		}


		if(flag==1)
		{
			if((arrOfAllocate[index].start+(arrOfAllocate[index].allocateFrames*PAGE_SIZE))== address )
			{
				address -= arrOfAllocate[index].allocateFrames*PAGE_SIZE;
			}
			free_info[counter2].free_Va=arrOfAllocate[index].start;
			free_info[counter2].arrOfFreeFrames = arrOfAllocate[index].allocateFrames;
			counter2++;
			sys_freeMem((uint32)virtual_address,arrOfAllocate[index].allocateFrames*PAGE_SIZE);
			arrOfAllocate[index].start = (uint32)NULL;
			arrOfAllocate[index].allocateFrames= (uint32)NULL;
			for(int i=0;i<counter2 ; i++)
			{
				for(int k=0;k<counter2 ; k++)
				{
					if(i!=k)
					{
					   if(free_info[k].free_Va == (free_info[i].free_Va+free_info[i].arrOfFreeFrames*PAGE_SIZE))
						{
						   free_info[k].free_Va =(uint32)NULL;
						   free_info[i].arrOfFreeFrames = free_info[i].arrOfFreeFrames + free_info[k].arrOfFreeFrames;
						   free_info[k].arrOfFreeFrames=(uint32)NULL;
						}
					}
				}
			}
		}
	}

	//==================================================================================//
	//================================ OTHER FUNCTIONS =================================//
	//==================================================================================//

	void halfLast()
	{
		uint32 x;
		uint32 oldSize;
		for(int i =0; i<counter3;i++)
		{
			if(arrOfAllocate[i].start+(arrOfAllocate[i].allocateFrames*PAGE_SIZE)==address)
			{
				oldSize=arrOfAllocate[i].allocateFrames;
				arrOfAllocate[i].allocateFrames=(arrOfAllocate[i].allocateFrames)/2;
				x=i;
				break;
			}
		}
		oldSize/=2;
		sys_freeMem(arrOfAllocate[x].start+(oldSize*PAGE_SIZE),oldSize*PAGE_SIZE);

	}
	void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
	{
		panic("this function is not required...!!");
		return 0;
	}

	void* sget(int32 ownerEnvID, char *sharedVarName)
	{
		panic("this function is not required...!!");
		return 0;
	}

	void sfree(void* virtual_address)
	{
		panic("this function is not required...!!");
	}

	void *realloc(void *virtual_address, uint32 new_size)
	{
		panic("this function is not required...!!");
		return 0;
	}

	void expand(uint32 newSize)
	{
		panic("this function is not required...!!");
	}
	void shrink(uint32 newSize)
	{
		panic("this function is not required...!!");
	}

	void freeHeap(void* virtual_address)
	{
		panic("this function is not required...!!");
	}
