//Thomas Fiorilla : trf40
#include "mymalloc.h"
#include <sys/time.h>

int main(int argc, char * argv[]){
	char * testPointer;
	int i = 0;
	int j = 0;
	char * testArray[120];
	
	//Initialise timeval structs for total runtime, variables for keeping track of average workload times
	struct timeval start;
	struct timeval end;
	double totalTime, loopTime, timeA, timeB, timeC, timeD, timeE;
	
	gettimeofday(&start, NULL);
	
	//Start looping all five workloads to 50 total times run, keeping track of each workload time and loop time
	for(j = 0; j < 50; j++){
		struct timeval loopStart, startA, startB, startC, startD, startE;
		struct timeval loopEnd, endA, endB, endC, endD, endE;
		gettimeofday(&loopStart, NULL);
		
		//Test A: malloc 1 and immediately free one byte 120 times
		gettimeofday(&startA, NULL);
		printf("A: Malloc and free 1 byte 120 times.\n");
		for(i=0; i < 120; i++){
			testPointer = malloc(1);
			free(testPointer);
		}
		gettimeofday(&endA, NULL);
		
		
		
		//Test B: malloc 1 byte 120 times, then free all of the mallocs
		gettimeofday(&startB, NULL);
		printf("B: Malloc, then free, 1 byte 120 times.\n");
		
		for(i=0; i < 120; i++){
			testArray[i] = (char*)malloc(sizeof(char));
		}
		for(i=0; i < 120; i++){
			free(testArray[i]);
			//printf("Test B Freed: %d\n", i);
		}
		gettimeofday(&endB, NULL);
		
		
		
		//Test C: randomly malloc or free 1 byte until 120 mallocs called and they have all been freed
		gettimeofday(&startC, NULL);
		printf("C: Randomly malloc or free 120 times each.\n");
		
		int mallocCount = 0;
		int mallocLeft = 0;
		int freeCount = 0;
		while(mallocCount < 120){
			int coin = rand();
			
			//Flips a "coin"; if the number is divided by 2 and has no remainder, it's heads and mallocs
			if(coin%2 == 0){
				//printf("Heads; Count: %d; Left: %d; Free: %d\n", mallocCount, mallocLeft, freeCount);
				
				testArray[mallocCount] = malloc(sizeof(char));
				
				mallocCount++;
				mallocLeft++;
				
			} else if(mallocLeft > 0){		//Else if there are existing mallocs to be freed, the mallocs are freed
				//printf("Tails; Count: %d; Left: %d; Free: %d\n", mallocCount, mallocLeft, freeCount);
				
				free(testArray[freeCount]);
				
				mallocLeft--;
				freeCount++;
				
			} else{						//Else there aren't currently any mallocs to be freed.
				//printf("Tails.\n");
			}
		}	//free the remaining mallocs
		while(freeCount < 120){
			free(testArray[freeCount]);
			freeCount++;
		}
		gettimeofday(&endC, NULL);
		
		
		
		//Custom test D: error testing. Most of these should return with errors to a file and line.
		gettimeofday(&startD, NULL);
		printf("D: Error testing.\n");
		
		//Checks if something that wasn't malloc'd can be freed
		free((int*)j);
		
		//Check for freeing past the beginning of a pointer.
		char * caseB = malloc(200);
		free(caseB + 10);
		free(caseB);
		
		//Check for freeing something that wasn't malloc'd at all
		int * caseB2;
		free(caseB2);
		
		//Check for malloc-ing too many bytes for the array
		testArray[0] = malloc(2000);
		testArray[1] = malloc(2000);
		testArray[2] = malloc(2000);
		
		//Check for freeing redundant and null pointers
		free(testArray[0]);
		free(testArray[0]);
		free(testArray[1]);
		free(testArray[2]);
		
		//Check repeatedly mallocing 400B chunks, with the last one beeing too large for the space left, and then free them all
		for(i = 0; i < 11; i++){
			testArray[i] = malloc(400);
		}
		for(i = 0; i < 11; i++){
			free(testArray[i]);
		}
		
		//Test for malloc-ing too many bytes, and then the largest possible + malloc one more byte on top of that, then free
		testArray[0] = malloc(5000);
		testArray[0] = malloc(4089);
		testArray[1] = malloc(1);
		
		free(testArray[0]);
		free(testArray[1]);
		gettimeofday(&endD, NULL);
		
		
		
		//Custom test E: calling malloc on a space and splitting one half into further halves each time, then freeing every malloc call before calling malloc for entire size of usable memory
		gettimeofday(&startE, NULL);
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
		gettimeofday(&endE, NULL);
		
		gettimeofday(&loopEnd, NULL);
		
		//If this is the first loop, times are simply set and not averaged, since there's not enough data
		if(j == 0){
			timeA = ((endA.tv_sec - startA.tv_sec) * 1000.0) + ((endA.tv_usec - startA.tv_usec) / 1000.0);
			timeB = ((endB.tv_sec - startB.tv_sec) * 1000.0) + ((endB.tv_usec - startB.tv_usec) / 1000.0);
			timeC = ((endC.tv_sec - startC.tv_sec) * 1000.0) + ((endC.tv_usec - startC.tv_usec) / 1000.0);
			timeD = ((endD.tv_sec - startD.tv_sec) * 1000.0) + ((endD.tv_usec - startD.tv_usec) / 1000.0);
			timeE = ((endE.tv_sec - startE.tv_sec) * 1000.0) + ((endE.tv_usec - startE.tv_usec) / 1000.0);
			loopTime = ((loopEnd.tv_sec - loopStart.tv_sec) * 1000.0) + ((loopEnd.tv_usec - loopStart.tv_usec) / 1000.0);
			
		} else{	//else this isn't the first loop and we have enough data to create an average
			timeA += (((endA.tv_sec - startA.tv_sec) * 1000.0) + ((endA.tv_usec - startA.tv_usec) / 1000.0)) / 2;
			timeB += (((endB.tv_sec - startB.tv_sec) * 1000.0) + ((endB.tv_usec - startB.tv_usec) / 1000.0)) / 2;
			timeC += (((endC.tv_sec - startC.tv_sec) * 1000.0) + ((endC.tv_usec - startC.tv_usec) / 1000.0)) / 2;
			timeD += (((endD.tv_sec - startD.tv_sec) * 1000.0) + ((endD.tv_usec - startD.tv_usec) / 1000.0)) / 2;
			timeE += (((endE.tv_sec - startE.tv_sec) * 1000.0) + ((endE.tv_usec - startE.tv_usec) / 1000.0)) / 2;
			loopTime += (((loopEnd.tv_sec - loopStart.tv_sec) * 1000.0) + ((loopEnd.tv_usec - loopStart.tv_usec) / 1000.0)) / 2;
		}
		
		printf("Loop done : %gms\n\n", loopTime);
	}
	
	gettimeofday(&end, NULL);
	
	//Get total runtime and print all the runtimes to the console
	totalTime = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_usec - start.tv_usec) / 1000.0);
	
	printf("Memgrind complete. Total runtime: %gms; Average loop runtime: %gms\n", totalTime, loopTime);
	printf("Average runtime per workload:\nA: %gms\nB: %gms\nC: %gms\nD: %gms\nE: %gms\n\n", timeA, timeB, timeC, timeD, timeE);
	
	//printf("Memgrind complete.\n\n");
	
	return 0;
}
