#ifndef __IV_H__
#define __IV_H__

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


typedef struct InitVector {
	char arr[3];
}iv;


void populate_iv(iv* vec){
	srand(time(NULL));
	int size = sizeof(vec->arr);	
	for(int i = 0; i < size; ++i){
		vec->arr[i] = rand() % 256;	
	}	
}

#endif
