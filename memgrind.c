#include "mymalloc.h"
#include <time.h>

int main(int argc, char * argv[]){
	char * testPointer;
	int i = 0;
	int j = 0;
	
	for(j = 0; j < 50; j++){
		//Test A: malloc 1 and immediately free one byte 120 times
		printf("A: Malloc and free 1 byte 120 times.\n");
		for(i=0; i < 120; i++){
			testPointer = malloc(1);
			free(testPointer);
		}
		
		
		
		//Test B: malloc 1 byte 120 times, then free all of the mallocs
		printf("B: Malloc 1 byte 120 times.\n");
		
		char * testB[120];
		for(i=0; i < 120; i++){
			testB[i] = (char*)malloc(sizeof(char));
		}
		printf("B: Free 1 byte 120 times.\n");
		for(i=0; i < 120; i++){
			free(testB[i]);
			printf("Test B Freed: %d\n", i);
		}
		
		
		
		//Test C: randomly malloc or free 1 byte until 120 mallocs called and they have all been freed
		printf("C: Randomly malloc or free 120 times each.\n");
		
		int mallocCount = 0;
		int mallocLeft = 0;
		int freeCount = 0;
		while(mallocCount < 120){
			coin = rand();
			
			if(coin%2 == 0){
				mallocCount++;
				mallocLeft++;
				
				testB[mallocCount] = malloc(sizeof(char));
				
			} else if(mallocLeft > 0){
				free(testB[freeCount]
				
				mallocLeft--;
				freeCount++;
				
			}
		}
		while(freeCount < 120){
			free(testB[freeCount]
			
		}
		
		printf("D: Randomly malloc a random size or free 120 times each.\n");
		for(i=0; i < 100; i++){
			
		}
		
		
		
		//Custom test D: 
		
		
		
		//Custom test E: calling malloc on a space and splitting one half into further halves each time, then freeing every malloc call before calling malloc for entire size of usable memory
		printf("\nE: Mallocing and freeing into increasingly smaller spaces.\n");
		
		testB[0] = (char*)malloc(sizeof(char)*4089);
		free(testB[0]);
		
		testB[0] = (char*)malloc(sizeof(char)*2042);
		testB[1] = (char*)malloc(sizeof(char)*2041);
		free(testB[0]);
		
		testB[0] = (char*)malloc(sizeof(char)*1018);
		testB[2] = (char*)malloc(sizeof(char)*1018);
		free(testB[0]);
		
		testB[0] = (char*)malloc(sizeof(char)*506);
		testB[3] = (char*)malloc(sizeof(char)*506);
		free(testB[0]);
		
		testB[0] = (char*)malloc(sizeof(char)*250);
		testB[4] = (char*)malloc(sizeof(char)*250);
		free(testB[0]);
		
		testB[0] = (char*)malloc(sizeof(char)*122);
		testB[5] = (char*)malloc(sizeof(char)*122);
		free(testB[0]);
		
		testB[0] = (char*)malloc(sizeof(char)*58);
		testB[6] = (char*)malloc(sizeof(char)*58);
		free(testB[0]);
		
		testB[0] = (char*)malloc(sizeof(char)*26);
		testB[7] = (char*)malloc(sizeof(char)*26);
		free(testB[0]);
		
		testB[0] = (char*)malloc(sizeof(char)*10);
		testB[8] = (char*)malloc(sizeof(char)*10);
		free(testB[0]);
		
		testB[0] = (char*)malloc(sizeof(char)*2);
		testB[9] = (char*)malloc(sizeof(char)*2);
		
		for(i = 0; i < 10; i++){
			free(testB[i]);
			printf("Test B Freed: %d\n", i);
		}
		
		testB[0] = (char*)malloc(sizeof(char)*4089);
		free(testB[0]);
	}
	
	return 0;
}
