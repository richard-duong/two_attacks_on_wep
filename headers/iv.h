#ifndef __IV_H__
#define __IV_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct InitVector {
	char iv[3];
}IV;


void populate_iv(IV* vector){
	int size = sizeof(vector->iv);	
	for(int i = 0; i < size; ++i){
		vector->iv[i] = rand() % 256;	
	}	
}

#endif
