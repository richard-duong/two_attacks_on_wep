#ifndef __PACKET_H__
#define __PACKET_H__

#include "iv.h"
#include "ip_header.h"
#include "crc32.h"
<<<<<<< HEAD
#include "rc4.h"
=======
#include "rc5.h"

char alice[4] = {1,1,1,1}; //port 50000
char carolwep[4] = {2,2,2,2}; //port 49500
char ap[4] = {3,3,3,3}; //
char bob[4] = {4,4,4,4}; //
char carol[4] = {5,5,5,5}; //

/* Add prototypes here */
>>>>>>> f90d545be3534c4efa0e076b87185cf1b858852e

typedef struct packets {
  iv vec;
  ip_header header;
  crc32 crc;
  char msg[4];
  char raw[16];
  char encoding[19];
}packet;

/******* PROTOTYPES *******/
/**************************/

void populate_packet(packet* pktptr, char* src, char* dest, char* msg);
int receive_packet(packet* pktptr, char* buffer);
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
 * that has a CRC, encrypted, and ready to be sent as a char buffer
 *
 * Inputs:  
 * packet* pktptr : packet to populate
 * char* src      : source ip address               [4 bytes]
 * char* dest     : dest ip address                 [4 bytes]
 * char* msg      : message to put into packet      [4 bytes]
 *
 * Outputs:
 * None
 *
 * Result: 
 * pktptr is populated with ip_header, message, iv, and crc
 * we store the raw format into pktptr->raw
 * then we also encode it and store into pktptr->encoding
 */

void populate_packet(packet* pktptr, char* src, char* dest, char* msg){
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
 * char* buffer   : buffer received and to be decrypted
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



int receive_packet(packet* pktptr, char* buffer){
  crc32 check;

  strncpy(pktptr->encoding, buffer, 19); 
  decode_packet(pktptr); 
  store_crc(&check, pktptr->raw, 12);
  return crc_check(&pktptr->crc, &check);
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

  char* M = malloc(12);
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
 * Encrypts packet
 *
 */

void encode_packet(packet* pktptr){
  construct_packet(pktptr);
  strncpy(pktptr->encoding, pktptr->vec.arr, 3);
  RC4_IV(pktptr->encoding + 3, pktptr->raw, &pktptr->vec, 16);      
}

// 
void decode_packet(packet* pktptr){
  strncpy(pktptr->vec.arr, pktptr->encoding, 3);
  RC4_IV(pktptr->raw, pktptr->encoding + 3, &pktptr->vec, 16);
  deconstruct_packet(pktptr);
}


// prints raw packet values
void print_packet(packet* pktptr){

  printf("\nIV: ");
  for(int i = 0; i < 3; ++i){
    printf("%d ", pktptr->vec.arr[i]);
  }

  printf("\nSRC: ");
  for(int i = 0; i < 4; ++i){
    printf("%d ", pktptr->header.src[i]);
  }

  printf("\nDEST: ");
  for(int i = 0; i < 4; ++i){
    printf("%d ", pktptr->header.dest[i]);
  }

  printf("\nMSG (uINT): ");
  for(int i = 0; i < 4; ++i){
    printf("%d ", pktptr->msg[i]);
  }

  printf("\nMSG (CHAR): ");
  for(int i = 0; i < 4; ++i){
    printf("%c ", pktptr->msg[i]);
  }

  printf("\nCRC: ");
  for(int i = 0; i < 4; ++i){
    printf("%d ", pktptr->crc.result[i]);
  }

  printf("\nRAW: ");
  for(int i = 0; i < 16; ++i){
    printf("%d ", pktptr->raw[i]);
  }

  printf("\nENCODE: ");
  for(int i = 0; i < 19; ++i){
    printf("%d ", pktptr->encoding[i]);
  }
  printf("\n\n");

}

#endif
