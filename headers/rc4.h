#ifndef __RC4_H__
#define __RC4_H__

#include <openssl/rc4.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iv.h"

const unsigned char* WEP_PASSWORD = "password";

/*
 * RC4_IV
 * =========================================================================
 * Objective: 
 * Run RC4 on unsigned char* src using the WEP_PASSWORD + IV, and then translating it
 * into unsigned char* dest. This can be used for encryption or decryption
 *
 * Inputs:
 * unsigned char* dest        : the dest buffer         [16 bytes]
 * const unsigned char* src   : raw text to encrypt     [16 bytes]
 * const iv* vecptr           : predetermined rand()    [3 bytes]
 *
 * Output: 
 * None
 *
 * Results: 
 * dest will now hold the encrypted value of src
*/

void RC4_IV(unsigned char* dest, unsigned char* src, const iv* vecptr, int size)
{
  RC4_KEY buf_rc4_key; 
  int pass_len = strlen(WEP_PASSWORD);
  int iv_len = 3;
  unsigned char* key = malloc(pass_len + iv_len);

  // concatenates WEP_PASSWORD + iv to key
  strncpy(key, WEP_PASSWORD, pass_len);  
  strncpy(key + pass_len, vecptr->arr, iv_len); 
    
  // sets key and encrypts pkt->raw to pkt->encrypt
  RC4_set_key(&buf_rc4_key, pass_len + iv_len, key);
  RC4(&buf_rc4_key, size, src, dest);

  // deallocate memory allocated for key
  free(key);
}

#endif
