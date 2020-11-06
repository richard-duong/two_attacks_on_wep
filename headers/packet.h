#ifndef __PACKET_H__
#define __PACKET_H__

#include "iv.h"
#include "ip_header.h"
#include "crc32.h"
#include "rc4.h"

unsigned char alice_ip[4] =     {169, 235, 16 , 75 };       // port 50000
unsigned char carolwep_ip[4] =  {100, 100, 100, 100};       // port 49500
unsigned char ap_ip[4] =        {255, 255, 255, 255};       // port 49000
unsigned char bob_ip[4] =       {141, 212, 113, 199};       // port 48500
unsigned char carol_ip[4] =     {128, 2,   42,  95 };       // port 48000


typedef struct packets {
  iv vec;
  ip_header header;
  crc32 crc;
  unsigned char msg[4];
  unsigned char raw[16];
  unsigned char encoding[19];
}packet;

/******* PROTOTYPES *******/
/**************************/

void hack_dest_of_packet(packet* pktptr, char* old_dest, char* new_dest);
void populate_packet(packet* pktptr, unsigned char* src, unsigned char* dest, unsigned char* msg);
int receive_packet(packet* pktptr, unsigned char* buffer);
void populate_crc(packet* pktptr);
void construct_packet(packet* pktptr);
void deconstruct_packet(packet* pktptr);
void encode_packet(packet* pktptr);
void decode_packet(packet* pktptr);
void print_packet(packet* pktptr);

/******* FOR USERS *******/
/*************************/

/* populate_packet
 * ===========================================================================
 * Objective:
 * Populate a SENDING packet with the ip_header and message
 * that has a CRC, encrypted, and ready to be sent as a unsigned char buffer
 *
 * Inputs:  
 * packet* pktptr : packet to populate
 * unsigned char* src      : source ip address               [4 bytes]
 * unsigned char* dest     : dest ip address                 [4 bytes]
 * unsigned char* msg      : message to put into packet      [4 bytes]
 *
 * Outputs:
 * None
 *
 * Result: 
 * pktptr is populated with ip_header, message, iv, and crc
 * we store the raw format into pktptr->raw
 * then we also encode it and store into pktptr->encoding
 */

void populate_packet(packet* pktptr, unsigned char* src, unsigned char* dest, unsigned char* msg){
  strncpy(pktptr->msg, msg, 4);
  populate_ip(&pktptr->header, src, dest);    
  populate_iv(&pktptr->vec);
  populate_crc(pktptr);
  encode_packet(pktptr);
}


/* receive_packet
 * ===================================================================
 * Objective:
 * (ACCESS POINT ONLY)
 * Receive an encrypted packet and decrypt it accordingly.
 * interpret ip_header, message, and crc
 * use a crc checksum to see if the packet is still valid
 *
 * Inputs: 
 * packet* pktptr : packet to store decrypted buffer
 * unsigned char* buffer   : buffer received and to be decrypted
 *
 * Outputs:
 * int validity   : returns 0 if valid crc, anything else is invalid
 *
 * Result: 
 * pktptr is populated with ip_header, message, iv, and crc
 * from the buffer. we also calculate the crc checksum to see
 * if the packet was modified during transmit.
 * Returns a 0 if the packet was not modified
 */

int receive_packet(packet* pktptr, unsigned char* buffer){
  crc32 check;

  strncpy(pktptr->encoding, buffer, 19); 
  decode_packet(pktptr); 
  store_crc(&check, pktptr->raw, 12);
  return crc_check(&pktptr->crc, &check);
}

/* hack_dest_of_packet
 * =========================================================================
 * Objective:
 * 
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

void hack_dest_of_packet(packet* pktptr, char* old_dest, char* new_dest){
  packet my_pkt;
  packet cancel_pkt;
  unsigned char empty_buffer[4] = {0, 0, 0, 0};

  populate_packet(&my_pkt, empty_buffer, new_dest, empty_buffer);
  populate_packet(&cancel_pkt, empty_buffer, old_dest, empty_buffer);
  
  for(int i = 0; i < 16; ++i){
    pktptr->encoding[i + 3] ^= my_pkt.raw[i];
    pktptr->encoding[i + 3] ^= cancel_pkt.raw[i];

  }
}





/******* HELPER FUNCTIONS *******/
/********************************/


/* populate_crc
 * ==========================================================================
 * Objective:
 * Generates a CRC based on the ip_header + message, and then
 * inserts that CRC into 
 * 
 * Inputs: 
 * packet* pktptr : packet used in order to populate crc
 *
 * Outputs:
 * None
 *
 * Result:
 * pktptr is used to grab contents of ip_header and message in order
 * to calculate the CRC. we then store the CRC into pktptr->crc
 */

void populate_crc(packet* pktptr){

  unsigned char* M = malloc(12);
  strncpy(M, pktptr->header.src, 4);
  strncpy(M + 4, pktptr->header.dest, 4);
  strncpy(M + 8, pktptr->msg, 4);
  store_crc(&pktptr->crc, M, 12);  
  free(M);
}


/* construct_packet
 * ========================================================================
 * Objective:
 * Stores the values of ip_header + msg + crc into an array that is
 * ready for encryption
 *
 * Inputs:
 * packet* pktptr : packet to retrieve values and store raw into
 *
 * Outputs:
 * None
 *
 * Result:
 * pktptr->raw is populated with ip_header + msg + crc
 * pktptr->raw is 16 bytes
 */

void construct_packet(packet* pktptr){
  strncpy(pktptr->raw, pktptr->header.src, 4);
  strncpy(pktptr->raw + 4, pktptr->header.dest, 4);
  strncpy(pktptr->raw + 8, pktptr->msg, 4);
  strncpy(pktptr->raw + 12, pktptr->crc.result, 4);
}



/* deconstruct_packet
 * ========================================================================
 * Objective:
 * Deconstruct the pktptr->raw into its separate sections such as
 * ip_header, msg, and crc
 *
 * Inputs:
 * packet* pktptr : packet to grab raw and update with sections
 *
 * Outputs:
 * None
 *
 * Result:
 * pktptr->src, pktptr->dest, pktptr->msg, pktptr->crc should be updated
 * to match the pktptr->raw accordingly
 */

void deconstruct_packet(packet* pktptr){
  strncpy(pktptr->header.src, pktptr->raw, 4);
  strncpy(pktptr->header.dest, pktptr->raw + 4, 4);
  strncpy(pktptr->msg, pktptr->raw + 8, 4);
  strncpy(pktptr->crc.result, pktptr->raw + 12,  4);
}



/* encode_packet
 * =========================================================================
 * Objective:
 * Encrypts packet using RC4 over the raw buffer (ip_header + msg + crc)
 * Places that encryption into the encoding buffer
 *
 * Inputs:
 * packet* pktptr : to produce and retrieve raw buffer and place encryption
 *
 * Outputs:
 * None
 *
 * Results:
 * Will generate the encryption and store into pktptr->encoding
 *
 */

void encode_packet(packet* pktptr){
  construct_packet(pktptr);
  strncpy(pktptr->encoding, pktptr->vec.arr, 3);
  RC4_IV(pktptr->encoding + 3, pktptr->raw, &pktptr->vec, 16);      
}

/* decode_packet
 * ==========================================================================
 * Objective:
 * Decrypts the encrypted message and generates a raw message that will be
 * used in order to identify the different parts of the packet
 *
 * Inputs:
 * packet* pktptr : packet to decrypt encryption, and store parts back into
 *
 * Outputs:
 * None
 *
 * Result:
 * pktptr->encoding is decrypted and stored into pktptr->raw, identifies
 * parts from pktptr->raw and places them correctly into pktptr
 */

void decode_packet(packet* pktptr){
  strncpy(pktptr->vec.arr, pktptr->encoding, 3);
  RC4_IV(pktptr->raw, pktptr->encoding + 3, &pktptr->vec, 16);
  deconstruct_packet(pktptr);
}



/* print_packet
 * ========================================================================
 * Objective:
 * Display parts of packet
 *
 * Inputs:
 * packet* pktptr : packet to grab information from
 *
 * Outputs:
 * None
 *
 * Result:
 * Print hexadecimal values of the contents of the packet
 */

void print_packet(packet* pktptr){

  printf("\nIV: ");
  for(int i = 0; i < 3; ++i){
    printf("%X ", pktptr->vec.arr[i]);
  }

  printf("\nSRC: ");
  for(int i = 0; i < 4; ++i){
    printf("%X ", pktptr->header.src[i]);
  }

  printf("\nDEST: ");
  for(int i = 0; i < 4; ++i){
    printf("%X ", pktptr->header.dest[i]);
  }

  printf("\nMSG: ");
  for(int i = 0; i < 4; ++i){
    printf("%X ", pktptr->msg[i]);
  }

  printf("\nMSG (CHAR): ");
  for(int i = 0; i < 4; ++i){
    printf("%c ", pktptr->msg[i]);
  }

  printf("\nCRC: ");
  for(int i = 0; i < 4; ++i){
    printf("%x ", pktptr->crc.result[i]);
  }

  printf("\nRAW: ");
  for(int i = 0; i < 16; ++i){
    printf("%X ", pktptr->raw[i]);
  }

  printf("\nENCODING: ");
  for(int i = 0; i < 19; ++i){
    printf("%X ", pktptr->encoding[i]);
  }
  printf("\n");

}

#endif
