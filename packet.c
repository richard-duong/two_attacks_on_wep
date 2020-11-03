#include <openssl/rc4.h>

//this defines all the parts of the packet to form the 128 bit char
struct packet {
    char IV[24];
    char src[20];
    char dest[20];
    char msg[32];
    char crc[32];
} packet;



int main(){
     
    char IV = rand() % 16777216;
    
    //RC4 ecnryption
    RC4_KEY buf_rc4_key; // how give IP + PACKET as a char data to RC4 ????
    unsigned char buf_rc4[80];
    unsigned char buf_rc4_out[80];
    RC4_set_key(&buf_rc4_key, 40, buf_rc4);
    RC4(&buf_rc4_key, 40, buf_rc4, buf_rc4_out); //encrypt 40 bytes of buf_rc4 and put it in buf_rc4_out     
    
    return 0;
}
