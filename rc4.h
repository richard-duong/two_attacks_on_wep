#include <openssl/rc4.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include packet.h

char password[] = "password";

char[] decrypt(char[] x)
{
    //take in char[19], cut off prefix IV, use pass + IV,
    
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