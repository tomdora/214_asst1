//Thomas Fiorilla : trf40
#include "mymalloc.h"



//Function to initialise the memory
static void * initializeMem(size_t x, char * file, int line){
	unsigned int magic = 4242424242;
	int sizeofMagic = sizeof(*(int*)(memory));
	int sizeofMeta = sizeof(*(short*)(memory));
	int currentLoc = 0;
	int startLoc = 0;
	
	//if the user asks for enough size for magic # and one single metadata block, make the block and don't create metadata after
	//OR user asks for enough size for magic # and one single metadata block with the size of one metadata block or less after, make the block and don't create metadata after
	if(x <= sizeof(memory) - sizeofMagic - sizeofMeta - 1 && x >= sizeof(memory) - sizeofMagic - (2*sizeofMeta) - 2){
		//printf("	User is asking for exactly enough space to only have one metadata, or not have any reasonable metadata.\n");
		
		*(int*)(memory) = magic;
		currentLoc += sizeofMagic;
		
		//printf("	Magic number set. First 4 bytes = %u\n", *(int*)(memory));		//sets the first 4 bytes to the magic number and increments the currentLoc
		//printf("\n	Current currentLoc (in-use): %d\n", currentLoc);
		
		memory[currentLoc] = 1;
		currentLoc += 1;														//sets the byte at the currentLoc to "used" and increments the currentLoc
		
		//printf("\n	Current currentLoc (start of size-block): %d\n", currentLoc);
		
		*(short*)(memory+currentLoc) = x;
		//printf("	Size of user data = %u\n", *(short*)(memory+currentLoc));
		currentLoc += sizeofMeta;												//casts the user size as a short to the two bytes after the used byte and increments the currentLoc
		
		//return currentLoc;
		return &(memory[currentLoc]);
	}
	
	//otherwise assumes there is space for reasonable metadata afterwards
	else{
		*(int*)(memory) = magic;
		currentLoc += sizeofMagic;
		
		//printf("	Magic number set. First 4 bytes = %u\n", *(int*)(memory));		//sets the first 4 bytes to the magic number and increments the currentLoc
		//printf("\n	Current currentLoc (in-use): %d\n", currentLoc);
		
		memory[currentLoc] = 1;
		currentLoc += 1;														//sets the byte at the currentLoc to "used" and increments the currentLoc
		
		//printf("\n	Current currentLoc (start of size-block): %d\n", currentLoc);
		
		*(short*)(memory+currentLoc) = x;
		//printf("	Size of user data = %u\n", *(short*)(memory+currentLoc));
		currentLoc += sizeofMeta;												//casts the user size as a short to the two bytes after the used byte and increments the currentLoc
		
		startLoc = currentLoc;
		currentLoc += x;														//gives the current address to startLoc, adds size of user data to address
		
		//printf("\n	Current currentLoc (first piece of second metadata): %d\n", currentLoc);
		
		memory[currentLoc] = 0;
		currentLoc += 1;														//sets the in-use byte after the user data to "not in use" and increments the currentLoc
		
		//printf("\n	Current currentLoc (start of second size-block): %d\n", currentLoc);
		
		*(short*)(memory+currentLoc) = sizeof(memory) - sizeofMagic - 2 - 2*(sizeofMeta) - x;
		//printf("	Size of remaining data: = %u\n", *(short*)(memory + currentLoc));
		
		//return startLoc;
		return &(memory[startLoc]);
	}
}




static void * allocateMem(size_t x, char * file, int line){
	int sizeofMagic = sizeof(*(int*)(memory));
	int sizeofMeta = sizeof(*(short*)(memory));
	int currentLoc = 0;
	int startLoc = 0;
	
	int flag = 0;
	
	currentLoc += sizeofMagic;												//skipping magic number, since it has to exist to get here
	
	//loops through to iterate through all data and possibilities
	while(flag == 0){
		
		//if the in-use byte is 1, code will find size of the data and iterate past it
		if(memory[currentLoc] == 1){		
			currentLoc += 1;
			//printf("	Block is already in use. Size of in-use block: %d\n", *(short*)(memory + currentLoc));
			
			currentLoc += *(short*)(memory + currentLoc) + sizeofMeta;					//reads the actual size value of the block, skips past it
			
			//if the currentLoc goes out of bounds
			if(currentLoc >= sizeof(memory) - 1 - sizeofMeta){
				printf("\n	You have no space left!\n");
				return NULL;
			}
			
			//printf("	Location after used block: %d\n\n", currentLoc);
		}
		
		//if the in-use byte is 0, code will check to see if it is the right size, and if so, assign sizes and return the address to the user
		else if(memory[currentLoc] == 0 && *(short*)(memory+(currentLoc+1)) >= x){
			//printf("	Free space!\n");
			
			//if the first space is the same size or just barely larger than what user asked for (cannot have reasonable metadata afterwards)
			//it will assign the whole space and return the address to the user, then it won't make a metadata block after
			if(x <= *(short*)(memory+(currentLoc+1)) && x + sizeofMeta + 1 >= *(short*)(memory+(currentLoc+1))){
				//printf("\n  First open space does not have enough space for reasonable metadata after or is the exact size.\n");
				
				memory[currentLoc] = 1;
				currentLoc += (sizeofMeta + 1);
				
				//return currentLoc;
				return &(memory[currentLoc]);
			}
			
			//if the user requests space and the first block has more than enough; gives user space and splits the remaining space into metadata
			else if(*(short*)(memory+(currentLoc+1)) > (x + 1 + sizeofMeta)){
				//printf("	Block has enough space and will need to be split.\n");
				
				int size = *(short*)(memory+(currentLoc+1));								//keeps size of the current block as-is
				
				memory[currentLoc] = 1;
				currentLoc += 1;																//sets in-use byte to 1 and iterates
				
				*(short*)(memory+currentLoc) = x;
				startLoc = currentLoc + sizeofMeta;											//sets the size block to the size of requested data, sets startLoc to address to return to user
				
				currentLoc += x + sizeofMeta;
				
				memory[currentLoc] = 0;
				currentLoc += 1;																//sets in-use byte of the immediate next metadata block to 0
				
				*(short*)(memory+currentLoc) = size - x - sizeofMeta - 1;
				
				//return startLoc;
				return &(memory[startLoc]);
			}	
		}
		
		//if the user requests space, the currentLoc is free, but does NOT have the available space, it will iterate the currentLoc
		else if(x > *(short*)(memory+(currentLoc+1) + 1 + sizeofMeta)){
			//printf("\n	Free currentLoc, but not enough space.\n");
			
			currentLoc += *(short*)(memory + (currentLoc+1)) + 1 + sizeofMeta;					//reads the actual size value of the block, skips past it
			
			if(currentLoc >= 4096){
				printf("	No space left, file %s line %d.\n", file, line);
				
				return NULL;
			}
		}
		
		//if none of the above cases happened, something went wrong
		else{
			printf("*!*!*!*!*Something went horribly wrong error file %s line %d.\n", file, line);
			
			return NULL;
		}
	}
}






//THIS IS THE CODE FOR MYMALLOC **************************************************************************************************************************************************************
void * mymalloc(size_t x, char * file, int line){
	unsigned int magic = 4242424242;
	int sizeofMagic = sizeof(*(int*)(memory));
	int sizeofMeta = sizeof(*(short*)(memory));
	
	//If user inputs a number that cannot possibly fit in the space, or is less than 1
    //It is considered too large if it is larger than the size of all the memory, minus the size of the initial magic block, minus the size of the metadata
	if(x > sizeof(memory) - sizeofMagic - sizeofMeta - 1){
		printf("That is larger than the possible space error file %s line %d.\n", file, line);
		return NULL;
	}
	
	//Check if 0; return NULL if so.
	else if(x == 0){
		return NULL;
	}
	
	//detects the magic block, intiialises if not found
	else if(*(int*)(memory) != magic){
		void * memAddress = initializeMem(x, file, line);
		
		return memAddress;
	}
	
	//runs if it is not the first time running malloc
	else{			
		//printf("\nNot the first time malloc has been run; operating as usual.\n");
		
		void * memAddress = allocateMem(x, file, line);
		
		return memAddress;
	}
	//printf("\n	End of the function.\n");
}





//THIS IS THE CODE FOR MYFREE **************************************************************************************************************************************************************
void myfree(void * x, char * file, int line){
	unsigned int magic = 4242424242;
	int currentLoc = 0;
	int sizeofMagic = sizeof(*(int*)(memory));
	int sizeofMeta = sizeof(*(short*)(memory));
	
	//printf("Running free. Address of the value: %d\n", x);
	
	//check to see if the address is before or after the limits
	if(x < &(memory) || x >= &(memory[sizeof(memory)])){
		printf("	Out of bounds error file %s line %d.\n", file, line);
		
		return;
	}
	
	//check to see if malloc has even been run
	else if(*(int*)(memory) != magic){
		printf("	Malloc has not been run error file %s line %d.\n", file, line);
		
		return;
	}
	
	//iterate down the array and check if the address given is the start of a block
	else{
		int flag = 0;
		
		currentLoc += sizeofMagic;		//skips over the size of the magic number
		int prevLoc = currentLoc;		//holds the previous location
		
		//create loop for iterating down the array
		while(flag == 0){
			//if the user tries to free a currentLoc that is not in use, returns
			if(memory[currentLoc] == 0 && x == &(memory[currentLoc + 1 + sizeofMeta])){
				printf("	Memory address given is not in use error file %s line %d.\n", file, line);
				
				return;
			}
			
			//if the currentLoc's address is lower than the user-given address, iterate to the next currentLoc and save the previous currentLoc
			else if(x > &(memory[currentLoc + 1 + sizeofMeta])){
				//printf("	Iterating the array.\n");
				
				prevLoc = currentLoc;
				
				currentLoc += 1;
				currentLoc += *(short*)(memory + currentLoc) + sizeofMeta;
				
				//if the currentLoc goes out of bounds
				if(currentLoc >= sizeof(memory) - 1 - sizeofMeta){
					printf("\n	!\n");
					return;
				}
			}
			
			//if the currentLoc's address is in-use and exactly what the user asked for, frees and starts looking through for adjacent free blocks
			else if(memory[currentLoc] == 1 && x == &(memory[currentLoc + 1 + sizeofMeta])){
				//printf("	Address confirmed. Freeing the block and checking for adjacent free blocks to merge with.\n");
				
				memory[currentLoc] = 0;
				
				//if the previous currentLoc is also free and not the same currentLoc, combine
				if(memory[prevLoc] == 0 && currentLoc != prevLoc){
					//printf("		Previous currentLoc is free. Combining.\n");
					
					prevLoc += 1;
					
					*(short*)(memory+prevLoc) += *(short*)(memory+(currentLoc+1)) + sizeofMeta + 1;
					
					prevLoc -= 1;
					currentLoc = prevLoc;
				}
				
				//if the previous currentLoc is also free but the same currentLoc
				else if(memory[prevLoc] == 0 && currentLoc == prevLoc){
					//printf("		Previous currentLoc is the same as the current currentLoc.\n");
					
				}
				
				//if the previous currentLoc isn't free, advance
				else if(memory[prevLoc] == 1){
					//printf("	Previous currentLoc not free.\n");	
					
					prevLoc = currentLoc;
				}
				
				//if the currentLoc is neither 0 or 1
				else{
					printf("*!*!*!*!*Something went horribly wrong error file %s line %d.\n", file, line);
					
					return;
				}
				
				//now advance to the next block and check if it's free
				currentLoc += 1;
				currentLoc += *(short*)(memory+currentLoc) + sizeofMeta;
				
				//if the next block is not free 
				//OR is out of bounds
				if(memory[currentLoc] == 1 || currentLoc >= sizeof(memory) - 1 - sizeofMeta){
					//printf("	Next currentLoc is not free, block has been merged as much as possible.\n");
					
					return;
				}
				
				//if the next block is free
				else if(memory[currentLoc] == 0){
					//printf("		Next currentLoc is free. Combining.\n");
					
					prevLoc += 1;
					
					*(short*)(memory+prevLoc) += *(short*)(memory+(currentLoc+1)) + sizeofMeta + 1;
					
					//printf("	Adjacent blocks have all been combined. Returning.\n");
					
					return;
				}
				
				return;
			}
			
			//if the currentLoc's address is larger than what the user asked for, user is not pointing to the beginning of a currentLoc
			else if(x < &(memory[currentLoc + 1 + sizeofMeta])){
				printf("	Address given to free is not the start of the block error file %s line %d.\n", file, line);
				
				return;
			}
			
			//if none of the above cases work, something went wrong
			else{
				printf("*!*!*!*!*Something went horribly wrong error file %s line %d.\n", file, line);
				
				return;
			}
		}
		
		return;
	}
	
	return;
}
