#include "mymalloc.h"
#include <time.h>

int main(int argc, char * argv[]){
	char * testPointer;
	int i = 0;
	
	printf("A: Malloc and free 1 byte 100 times.\n");
	for(i=0; i < 100; i++){
		testPointer = malloc(1);
		free(testPointer);
	}
	
	char * testB[50];
	printf("B: Malloc 1 byte 50 times.\n");
	for(i=0; i < 50; i++){
		testB[i] = (char*)malloc(sizeof(char));
	}
	printf("B: Free 1 byte 50 times.\n");
	for(i=0; i < 50; i++){
		free(testB[i]);
		printf("Test B Freed: %d\n", i);
	}
	/*
	printf("C: Randomly malloc or free 50 times each.\n");
	for(i=0; i < 100; i++){
		
	}
	
	printf("D: Randomly malloc a random size or free 50 times each.\n");
	for(i=0; i < 100; i++){
		
	}
	*/
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
	
	/*
	printf("F: .\n");
	for(int i=0; i < 100; i++){
		
	}
	*/
	return 0;
}
