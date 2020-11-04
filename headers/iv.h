#ifndef __IV_H__
#define __IV_H__

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


typedef struct InitVector {
	char vec[3];
}iv;


void populate_iv(iv* vector){
	srand(time(NULL));
	int size = sizeof(vector->vec);	
	for(int i = 0; i < size; ++i){
		vector->vec[i] = rand() % 256;	
	}	
}

#endif
