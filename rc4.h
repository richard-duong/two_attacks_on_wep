#include <openssl/rc4.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include packet.h

char password[] = "password";

char[] decrypt(char[] pkt) //19 bytes
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

char[] encrypt(packet &pkt)
{
    RC4_KEY buf_rc4_key; 
    unsigned char buf_rc4[80];
    strcat(pkt.src, buf_rc4); //src
    strcat(pkt.dest, buf_rc4); //src + dest
    unsigned char buf_rc4_out[80];
    char key[11];
    strcat(password,key); //password
    strcat(pkt.IV, key); //password + IV
    RC4_set_key(&buf_rc4_key, 11, key);
    RC4(&buf_rc4_key, 8, buf_rc4, buf_rc4_out);
    //printf("Encryption of IP header: %s\n", buf_rc4_out);
    return buf_rc4_out;
    
}