//Thomas Fiorilla : trf40
#include "mymalloc.h"
#include <time.h>

int main(int argc, char * argv[]){
	char * testPointer;
	int i = 0;
	int j = 0;
	char * testArray[120];
	double timeTest[50];
	
	for(j = 0; j < 50; j++){
		clock_t start, end;
		start = clock();
		
		//Test A: malloc 1 and immediately free one byte 120 times
		printf("A: Malloc and free 1 byte 120 times.\n");
		for(i=0; i < 120; i++){
			testPointer = malloc(1);
			free(testPointer);
		}
		
		
		
		//Test B: malloc 1 byte 120 times, then free all of the mallocs
		printf("B: Malloc 1 byte 120 times.\n");
		
		for(i=0; i < 120; i++){
			testArray[i] = (char*)malloc(sizeof(char));
		}
		printf("B: Free 1 byte 120 times.\n");
		for(i=0; i < 120; i++){
			free(testArray[i]);
			//printf("Test B Freed: %d\n", i);
		}
		
		
		
		//Test C: randomly malloc or free 1 byte until 120 mallocs called and they have all been freed
		printf("C: Randomly malloc or free 120 times each.\n");
		
		int mallocCount = 0;
		int mallocLeft = 0;
		int freeCount = 0;
		while(mallocCount < 120){
			int coin = rand();
			
			if(coin%2 == 0){
				//printf("Heads; Count: %d; Left: %d; Free: %d\n", mallocCount, mallocLeft, freeCount);
				
				testArray[mallocCount] = malloc(sizeof(char));
				
				mallocCount++;
				mallocLeft++;
				
			} else if(mallocLeft > 0){
				//printf("Tails; Count: %d; Left: %d; Free: %d\n", mallocCount, mallocLeft, freeCount);
				
				free(testArray[freeCount]);
				
				mallocLeft--;
				freeCount++;
				
			} else{
				//printf("Tails.\n");
			}
		}
		while(freeCount < 120){
			free(testArray[freeCount]);
			freeCount++;
		}
		
		
		
		//Custom test D: error testing.
		printf("D: Error testing.\n");
		
		free((int*)j);
		
		char * caseB = malloc(200);
		free(caseB + 10);
		free(caseB);
		
		int * caseB2;
		free(caseB2);
		
		testArray[0] = malloc(2000);
		testArray[1] = malloc(2000);
		testArray[2] = malloc(2000);
		
		free(testArray[0]);
		free(testArray[0]);
		free(testArray[1]);
		free(testArray[2]);
		
		for(i = 0; i < 11; i++){
			testArray[i] = malloc(400);
		}
		for(i = 0; i < 11; i++){
			free(testArray[i]);
		}
		
		testArray[0] = malloc(5000);
		testArray[0] = malloc(4089);
		testArray[1] = malloc(1);
		
		free(testArray[0]);
		free(testArray[1]);
		
		
		
		//Custom test E: calling malloc on a space and splitting one half into further halves each time, then freeing every malloc call before calling malloc for entire size of usable memory
		printf("E: Mallocing and freeing into increasingly smaller spaces.\n");
		
		testArray[0] = (char*)malloc(4089);
		free(testArray[0]);
		
		testArray[0] = (char*)malloc(2042);
		testArray[1] = (char*)malloc(2041);
		free(testArray[0]);
		
		testArray[0] = (char*)malloc(1018);
		testArray[2] = (char*)malloc(1018);
		free(testArray[0]);
		
		testArray[0] = (char*)malloc(506);
		testArray[3] = (char*)malloc(506);
		free(testArray[0]);
		
		testArray[0] = (char*)malloc(250);
		testArray[4] = (char*)malloc(250);
		free(testArray[0]);
		
		testArray[0] = (char*)malloc(122);
		testArray[5] = (char*)malloc(122);
		free(testArray[0]);
		
		testArray[0] = (char*)malloc(58);
		testArray[6] = (char*)malloc(58);
		free(testArray[0]);
		
		testArray[0] = (char*)malloc(26);
		testArray[7] = (char*)malloc(26);
		free(testArray[0]);
		
		testArray[0] = (char*)malloc(10);
		testArray[8] = (char*)malloc(10);
		free(testArray[0]);
		
		testArray[0] = (char*)malloc(2);
		testArray[9] = (char*)malloc(2);
		
		for(i = 0; i < 10; i++){
			free(testArray[i]);
		}
		
		testArray[0] = (char*)malloc(sizeof(char)*4089);
		free(testArray[0]);
		
		end = clock();
		
		timeTest[j] = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
		
		printf("Loop done : %gms\n\n", timeTest[j]);
	}
	
	double runTime = 0;
	for(i = 0; i < 50; i++){
		runTime += timeTest[i];
	}
	
	printf("Memgrind complete. Total runtime: %gms; Average runtime: %gms\n\n", runTime, runTime/50);
	
	//printf("Memgrind complete.\n\n");
	
	return 0;
}
