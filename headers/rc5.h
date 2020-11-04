#ifndef __RC4_H__
#define __RC4_H__

#include <openssl/rc4.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iv.h"

char* password = "password";

/*
	RC4_IV
	=========================================================================

	Objective: 
	Run RC4 on char* src using the WEP password + IV, and then translating it
 	into char* dest. This can be used for encryption or decryption

	Output: 
	None

	Results: 
	char* dest now holds the RC4(src) value
*/

void RC4_IV(char* dest, char* src, iv* vecptr, int size)
{
    RC4_KEY buf_rc4_key; 
		int pass_len = strlen(password);
		int iv_len = strlen(vecptr->arr);
		char* key = malloc(pass_len + iv_len);

		// concatenates password + iv to key
    strncpy(key, password, pass_len);	
    strncpy(key + pass_len, vecptr->arr, iv_len); 
    
		// sets key and encrypts pkt->raw to pkt->encrypt
    RC4_set_key(&buf_rc4_key, pass_len + iv_len, key);
    RC4(&buf_rc4_key, size, src, dest);

		// deallocate memory allocated for key
		free(key);
}

#endif
