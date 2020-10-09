Thomas Fiorilla, trf40

How my metadata is implemented:
	Rather than a struct, that would be padded and a larger size, I went with implementing it as a series of two types:
		Char  = the in-use byte; written and read directly from the array
		Short = stores the size of the following data block
	This method turned out as 3 bytes long.

How the mymalloc function works:
	Every time I run my mymalloc function, it creates 4 local variables:
		location		= variable to store the location as the memory is iterated through
		location2		= variable to save the location to return
		sizeofMagic	= stores the size of what I cast as my magic number; this allows some give-and-take with different systems and architectures
		sizeofMeta	= stores the size of what I cast as my short for my metadata; this allows some give-and-take with different systems and architectures
	
	The mymalloc function checks to see if the user inputs a number that cannot possibly ever be a number, then if it passes, checks for the magic number.
		If the magic number does not exist, it will initialise the array under two cases:
			1) The user requested just enough data that either a metadata cannot fit after, or will fit with exactly 0 space after
				If this happens, it will fragment and hold the entire size of the array, whether the user requested that much space or not
				
			2) The user requests less than that, and it sets the first block, then moves forward and creates another metadata block for the remaining space
		
		If the magic number does exist, mymalloc presumes it has been run before and moves on to allocate space.
			It iterates down the array, storing the current location in the "location" variable.
			The function looks for the first free block that is at least the same size and returns the memory address, splitting the memory afterwards if necesssary.
		
		Iteration through the array works as:
			The location reads the first byte that can be the start of a metadata block (in this case, the very first location possible would be "location + sizeofMagic")
				If it returns 1, it skips ahead one, casts the array at that location as a dereferenced short pointer, and reads the size.
					The size of the space and short are added to the location variable, where it then checks again at the next location for a 0 or a 1.
					
				If it returns 0, and the size read at "location + 1" is at least what the user requested for, it will use that block for the user data.

How the myfree function works:
	Every time myfree runs, it creates 3 local variables, similarly to the mymalloc function:
		location		= variable to store the location as the memory is iterated through
		sizeofMagic	= stores the size of what I cast as my magic number; this allows some give-and-take with different systems and architectures
		sizeofMeta	= stores the size of what I cast as my short for my metadata; this allows some give-and-take with different systems and architectures
	
	Instead of using the index as a reference point, because the user hands over a memory location, the address at a particular index is used for reference.
	The function starts off comparing the address the user handed over to the address at index 7 (if it is smaller, it cannot be an address issued by mymalloc)
		AND checks if it is equal to or larger than the address of 4093 (in this case, the size of the array - size of my whole metadata block), in which case it can also not have been allocated
			This check will catch non-dynamically allocated pointers and non-pointer types.
	
	The iteration down the array works the same as mymalloc, but now the myfree catches specific errors unique to its function;
		If the memory address the user hands over is at any point not equal to a memory address as the array is iterated through,
			AND falls behind the memory address without ever equalling any of the indices' addresses
			
			It can be safely assumed that the user tried to hand over a memory address that is not the one returned to them and in the middle of a block instead.
	
	The function keeps track of the index of the start of the previous metadata block with another local variable "prevLoc" and, if the address the user handed over can be freed,
		1) The previous block is checked; if it is free, the size of the "current block" (where the variable "location" is) + size of metadata block is added to the previous location,
		   and then the location advances to check if the next block is free.
		   	If it is not free, instead "prevLoc" moves forward to the current location and the "location" checks with the block after that.

memgrind.c
	I unfortunately ran out of time to properly implement memgrind.c, however I was able to implement a few of the test-cases, which ran without any errors.
	
	Test A:
		Mallocing 1 byte and then immediately clearing it; 100 times. No errors.
	
	Test B:
		Mallocing 1 byte into an array 50 times and then freeing all 50. No errors.
	
	Test E:
		For test E I chose to malloc the entire space to one pointer, then free it and malloc two pointers half the size (accounting for the size of metadata)
			I repeated this process until the entire array was full, then freed every single space and malloced the maximum possible size (4089) and there were no errors.
			
			I chose this test because I felt it was a good indicator of whether or not my functions would lead to memory leaks; if any of the mallocs failed, especially the last one,
			it would mean that somewhere in my code I was losing space.
