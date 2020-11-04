#include <openssl/rc4.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include packet.h

char password[] = "password";

char[] decrypt(char[] *pkt) //19 bytes
{
    //take in char[19], cut off prefix IV, use pass + IV,
    char IV[3];
    for (int i = 0; i < 3; i++)
    {
        IV[i] = pkt[i];
    }
    
    char newpkt[16];
    for (int i = 3; i < 19; i++)
    {
        newpkt[i] = pkt[i];
    }
    
    RC4_KEY buf_rc4_key;
    char key[11];
    strcat(password,key); //password
    strcat(IV, key); //password + IV
    RC4_set_key(&buf_rc4_key, 11, key);
    //decrypt the newpkt excluding the IV
    char decrypt[16];
    RC4(&buf_rc4_key, 16, newpkt, decrypt);  
}


/*
	ENCRYPT
	=========================================================================
	Objective: Encrypt the raw cstring in packet using RC4 and the provided IV
	Input: Packet* pkt, int size
	Output: None
	Results: Adjust pkt->encrypt to hold the 16 byte encryption
*/

void encrypt(packet* pkt, int size)
{
    RC4_KEY buf_rc4_key; 
		int pass_len = strlen(password);
		int iv_len = strlen(pkt->iv.vec);
		char* key = malloc(pass_len + iv_len);

		// concatenates password + iv to key
    strncpy(key, password, pass_len);	
    strcat(key + pass_len, pkt->iv.vec, iv_len); 
    
		// sets key and encrypts pkt->raw to pkt->encrypt
    RC4_set_key(&buf_rc4_key, 11, key);
    RC4(&buf_rc4_key, size, pkt->raw, pkt->encrypt);

		// deallocate memory allocated for key
		free(key);
}

