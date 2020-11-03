#include <openssl/rc4.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//this defines all the parts of the packet to form the 128 bit char
struct packet {
    char IV[3];
    char src[20];
    char dest[20];
    char msg[4];
    char crc[4];
} packet;



int main(){
    
    int x = 0;
    for(int i = 0; i < 3; i++)
    {
        x = rand() % 256;
        packet.IV[i] = x;
    }
    
    //RC4 ecnryption
    RC4_KEY buf_rc4_key; // how give IP + PACKET as a char data to RC4 ????
    
    unsigned char buf_rc4[80];
    strcat(packet.src, buf_rc4); //buf_rc4 =  src
    strcat(packet.dest, buf_rc4); // buf_rc4 = src + dest
    unsigned char buf_rc4_out[80];
    
    //RC4_set_key(RC4_KEY *key, int len, const unsigned char *data)
    RC4_set_key(&buf_rc4_key, 3, packet.IV);
    //RC4(RC4_KEY *key, unsigned long len, const unsigned char *indata,
    //unsigned char *outdata);
    RC4(&buf_rc4_key, 40, buf_rc4, buf_rc4_out); //encrypt 40 bytes of buf_rc4 and put it in buf_rc4_out     
    printf("Encryption of IP header: %s\n", buf_rc4_out);
    
    
    
    
    return 0;
}
