#ifndef MYMALLOC__H__
#define MYMALLOC__H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define malloc(x) mymalloc(x)
#define free(x) myfree(x)

static char blockSim[4096];
static unsigned int magic = 4242424242;

void * mymalloc(int x);
void myfree(void * x);

#endif

