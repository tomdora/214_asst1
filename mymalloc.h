#ifndef MYMALLOC__H__
#define MYMALLOC__H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

static char memory[4096];
static unsigned int magic = 4242424242;

void * mymalloc(int x, char * file, int line);
void myfree(void * x, char * file, int line);

#endif

