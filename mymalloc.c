#include "mymalloc.h"




static void * allocateMem(int x){
	
	
	return NULL;
}


//THIS IS THE CODE FOR MYMALLOC **************************************************************************************************************************************************************
void * mymalloc(int x, char * file, int line){
	int location = 0;
	int location2 = 0;
	int sizeofMagic = sizeof(*(int*)(memory));
	int sizeofData = sizeof(*(short*)(memory));
	
	//If user inputs a number that cannot possibly fit in the space, or is less than 1
    //It is considered too large if it is larger than the size of all the memory, minus the size of the initial magic block, minus the size of the metadata
	if(x > sizeof(memory) - sizeofMagic - sizeofData - 1 || x < 1){
		printf("That is larger than the possible space or smaller than 1. Returning with no value.\n");
		return NULL;
		
	} 			
	
	//detects the magic block, intiialises if not found
	else if(*(int*)(memory) != magic){
		//printf("\nNo magic number. First 4 bytes = %u\n", *(int*)(memory));
		//printf("Address of the first location: %d\n", &(memory[location]));
		
		//if the user asks for enough size for magic # and one single metadata block, make the block and don't create metadata after
		//OR user asks for enough size for magic # and one single metadata block with the size of one metadata block or less after, make the block and don't create metadata after
		if(x <= sizeof(memory) - sizeofMagic - sizeofData - 1 && x >= sizeof(memory) - sizeofMagic - (2*sizeofData) - 2){
			//printf("	User is asking for exactly enough space to only have one metadata, or not have any reasonable metadata.\n");
			
			*(int*)(memory) = magic;
			location += sizeofMagic;
			
			//printf("	Magic number set. First 4 bytes = %u\n", *(int*)(memory));		//sets the first 4 bytes to the magic number and increments the location
			//printf("\n	Current location (in-use): %d\n", location);
			
			memory[location] = 1;
			location += 1;														//sets the byte at the location to "used" and increments the location
			
			//printf("\n	Current location (start of size-block): %d\n", location);
			
			*(short*)(memory+location) = x;
			//printf("	Size of user data = %u\n", *(short*)(memory+location));
			location += sizeofData;												//casts the user size as a short to the two bytes after the used byte and increments the location
			
			//return location;
			return &(memory[location]);
		}
		
		//otherwise assumes there is space for reasonable metadata afterwards
		else{
			*(int*)(memory) = magic;
			location += sizeofMagic;
			
			//printf("	Magic number set. First 4 bytes = %u\n", *(int*)(memory));		//sets the first 4 bytes to the magic number and increments the location
			//printf("\n	Current location (in-use): %d\n", location);
			
			memory[location] = 1;
			location += 1;														//sets the byte at the location to "used" and increments the location
			
			//printf("\n	Current location (start of size-block): %d\n", location);
			
			*(short*)(memory+location) = x;
			//printf("	Size of user data = %u\n", *(short*)(memory+location));
			location += sizeofData;												//casts the user size as a short to the two bytes after the used byte and increments the location
			
			location2 = location;
			location += x;														//gives the current address to location2, adds size of user data to address
			
			//printf("\n	Current location (first piece of second metadata): %d\n", location);
			
			memory[location] = 0;
			location += 1;														//sets the in-use byte after the user data to "not in use" and increments the location
			
			//printf("\n	Current location (start of second size-block): %d\n", location);
			
			*(short*)(memory+location) = sizeof(memory) - sizeofMagic - 2 - 2*(sizeofData) - x;
			//printf("	Size of remaining data: = %u\n", *(short*)(memory + location));
			
			//return location2;
			return &(memory[location2]);
		}
	}
	
	//runs if it is not the first time running malloc
	else{			
		//printf("\nNot the first time malloc has been run; operating as usual.\n");
		
		int flag = 0;
		
		location += sizeofMagic;												//skipping magic number, since it has to exist to get here
		
		//loops through to iterate through all data and possibilities
		while(flag == 0){
			
			//if the in-use byte is 1, code will find size of the data and iterate past it
			if(memory[location] == 1){		
				location += 1;
				//printf("	Block is already in use. Size of in-use block: %d\n", *(short*)(memory + location));
				
				location += *(short*)(memory + location) + sizeofData;					//reads the actual size value of the block, skips past it
				
				//if the location goes out of bounds
				if(location >= sizeof(memory) - 1 - sizeofData){
					printf("\n	You have no space left!\n");
				return NULL;
				}
				
				//printf("	Location after used block: %d\n\n", location);
			}
			
			//if the in-use byte is 0, code will check to see if it is the right size, and if so, assign sizes and return the address to the user
			else if(memory[location] == 0 && *(short*)(memory+(location+1)) >= x){
				//printf("	Free space!\n");
				
				//if the first space is the same size or just barely larger than what user asked for (cannot have reasonable metadata afterwards)
				//it will assign the whole space and return the address to the user, then it won't make a metadata block after
				if(x <= *(short*)(memory+(location+1)) && x + sizeofData + 1 >= *(short*)(memory+(location+1))){
					//printf("\n  First open space does not have enough space for reasonable metadata after or is the exact size.\n");
					
					memory[location] = 1;
					location += (sizeofData + 1);
					
					//return location;
					return &(memory[location]);
				}
				
				//if the user requests space and the first block has more than enough; gives user space and splits the remaining space into metadata
				else if(*(short*)(memory+(location+1)) > (x + 1 + sizeofData)){
					//printf("	Block has enough space and will need to be split.\n");
				    
					int size = *(short*)(memory+(location+1));								//keeps size of the current block as-is
					
					memory[location] = 1;
					location += 1;																//sets in-use byte to 1 and iterates
					
					*(short*)(memory+location) = x;
					location2 = location + sizeofData;											//sets the size block to the size of requested data, sets location2 to address to return to user
					
					location += x + sizeofData;
					
					memory[location] = 0;
					location += 1;																//sets in-use byte of the immediate next metadata block to 0
					
					*(short*)(memory+location) = size - x - sizeofData - 1;
					
					//return location2;
					return &(memory[location2]);
				}	
			}
			
			//if the user requests space, the location is free, but does NOT have the available space, it will iterate the location
			else if(x > *(short*)(memory+(location+1) + 1 + sizeofData)){
				//printf("\n	Free location, but not enough space.\n");
				
				location += *(short*)(memory + (location+1)) + 1 + sizeofData;					//reads the actual size value of the block, skips past it
				
			   	if(location >= 4096){
					printf("\n	You have no space left!\n\n");
					
					return NULL;
			    	}
			}
			
			//if none of the above cases happened, something went wrong
			else{
				printf("*!*!*!*!*Something went horribly wrong. Returning.\n");
				
				return NULL;
			}
		}
		
		return NULL;
	}
	//printf("\n	End of the function.\n");
}





//THIS IS THE CODE FOR MYFREE **************************************************************************************************************************************************************
void myfree(void * x, char * file, int line){
	//free function goes here
	int location = 0;
	int sizeofMagic = sizeof(*(int*)(memory));
	int sizeofData = sizeof(*(short*)(memory));
	
	//printf("Running free. Address of the value: %d\n", x);
	
	//check to see if the address is before or after the limits
	if(x < &(memory[location + sizeofMagic + 1 + sizeofData]) || x >= &(memory[sizeof(memory)-1-sizeofData])){
		printf("	Trying to free memory not within the bounds of memory. Returning.\n");
		
		return;
	}
	
	//iterate down the array and check if the address given is the start of a block
	else if(x >= &(memory[0 + sizeofMagic + 1 + sizeofData]) || x < &(memory[sizeof(memory)-1-sizeofData])){
		int flag = 0;
		
		location += sizeofMagic;		//skips over the size of the magic number
		int prevLoc = location;		//holds the previous location
		
		//create loop for iterating down the array
		while(flag == 0){
			//if the user tries to free a location that is not in use, returns
			if(memory[location] == 0 && x == &(memory[location + 1 + sizeofData])){
				printf("	Memory address given is not in use! Returning.\n");
				
				return;
			}
			
			//if the location's address is lower than the user-given address, iterate to the next location and save the previous location
			else if(x > &(memory[location + 1 + sizeofData])){
				//printf("	Iterating the array.\n");
				
				prevLoc = location;
				
				location += 1;
				location += *(short*)(memory + location) + sizeofData;
				
				//if the location goes out of bounds
				if(location >= sizeof(memory) - 1 - sizeofData){
					printf("\n	!\n");
					return;
				}
			}
			
			//if the location's address is in-use and exactly what the user asked for, frees and starts looking through for adjacent free blocks
			else if(memory[location] == 1 && x == &(memory[location + 1 + sizeofData])){
				//printf("	Address confirmed. Freeing the block and checking for adjacent free blocks to merge with.\n");
				
				memory[location] = 0;
				
				//if the previous location is also free and not the same location, combine
				if(memory[prevLoc] == 0 && location != prevLoc){
					//printf("		Previous location is free. Combining.\n");
					
					prevLoc += 1;
					
					*(short*)(memory+prevLoc) += *(short*)(memory+(location+1)) + sizeofData + 1;
					
					prevLoc -= 1;
					location = prevLoc;
				}
				
				//if the previous location is also free but the same location
				else if(memory[prevLoc] == 0 && location == prevLoc){
					//printf("		Previous location is the same as the current location.\n");
					
				}
				
				//if the previous location isn't free, advance
				else if(memory[prevLoc] == 1){
					//printf("	Previous location not free.\n");	
					
					prevLoc = location;
				}
				
				//if the location is neither 0 or 1
				else{
					printf("*!*!*!*!*Something went horribly wrong. Returning.\n");
					
					return;
				}
				
				//now advance to the next block and check if it's free
				location += 1;
				location += *(short*)(memory+location) + sizeofData;
				
				//if the next block is not free 
				//OR is out of bounds
				if(memory[location] == 1 || location >= sizeof(memory) - 1 - sizeofData){
					//printf("	Next location is not free, block has been merged as much as possible.\n");
					
					return;
				}
				
				//if the next block is free
				else if(memory[location] == 0){
					//printf("		Next location is free. Combining.\n");
					
					prevLoc += 1;
					
					*(short*)(memory+prevLoc) += *(short*)(memory+(location+1)) + sizeofData + 1;
					
					//printf("	Adjacent blocks have all been combined. Returning.\n");
					
					return;
				}
				
				return;
			}
			
			//if the location's address is larger than what the user asked for, user is not pointing to the beginning of a location
			else if(x < &(memory[location + 1 + sizeofData])){
				printf("	Address given to free is not the start of the block. Returning.\n");
				
				return;
			}
			
			//if none of the above cases work, something went wrong
			else{
				printf("*!*!*!*!*Something went horribly wrong. Returning.\n");
				
				return;
			}
		}
		
		return;
	}
	
	return;
}
