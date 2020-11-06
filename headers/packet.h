#ifndef __PACKET_H__
#define __PACKET_H__

#include "iv.h"
#include "ip_header.h"
#include "crc32.h"
#include "rc4.h"

/******* DEFINITIONS *******/
/***************************/

const unsigned char ALICE_IP[4]     = {169, 235, 16 , 75 };
const unsigned char CAROLWEP_IP[4]  = {100, 100, 100, 100};
const unsigned char AP_IP[4]        = {255, 255, 255, 255};
const unsigned char BOB_IP[4]       = {141, 212, 113, 199};
const unsigned char CAROL_IP[4]     = {128, 2,   42,  95 };

const unsigned int ALICE_PORT       = 50000;
const unsigned int CAROLWEP_PORT    = 49500;
const unsigned int AP_PORT          = 49000;
const unsigned int BOB_PORT         = 48500;
const unsigned int CAROL_PORT       = 48000;

typedef struct packets {
  iv vec;
  ip_header header;
  crc32 crc;
  unsigned char msg[4];
  unsigned char raw[16];
  unsigned char encryption[19];
}packet;



/******* PROTOTYPES *******/
/**************************/

void hack_dest_of_packet(packet* pktptr, const unsigned char* old_dest, const unsigned char* new_dest);
void populate_packet(packet* pktptr, const unsigned char* src, const unsigned char* dest, const unsigned char* msg);
int receive_packet(packet* pktptr, const unsigned char* buffer);
void populate_crc(packet* pktptr);
void construct_packet(packet* pktptr);
void deconstruct_packet(packet* pktptr);
void encode_packet(packet* pktptr);
void decode_packet(packet* pktptr);
void print_packet(packet* pktptr);



/******* USER INTERFACE FUNCTIONS *******/
/****************************************/

/* populate_packet
 * ===========================================================================
 * Objective:
 * Populate a SENDING packet with the ip_header and message
 * that has a CRC, encrypted, and ready to be sent as a unsigned char buffer
 *
 * Inputs:  
 * packet* pktptr                : packet to populate
 * const unsigned char* src      : source ip address               [4 bytes]
 * const unsigned char* dest     : dest ip address                 [4 bytes]
 * const unsigned char* msg      : message to put into packet      [4 bytes]
 *
 * Outputs:
 * None
 *
 * Result: 
 * pktptr is populated with ip_header, message, iv, and crc
 * we store the raw format into pktptr->raw
 * then we also encode it and store into pktptr->encryption
 */

void populate_packet(packet* pktptr, const unsigned char* src, const unsigned char* dest, const unsigned char* msg){
  strncpy(pktptr->msg, msg, 4);
  populate_ip(&pktptr->header, src, dest);    
  populate_iv(&pktptr->vec);
  populate_crc(pktptr);
  encode_packet(pktptr);
}



/* receive_packet
 * =========================================================================
 * Objective:
 * Receive an encrypted packet and decrypt it accordingly.
 * interpret ip_header, message, and crc
 * use a crc checksum to see if the packet is still valid
 *
 * Inputs: 
 * packet* pktptr                 : packet to store decrypted buffer
 * const unsigned char* buffer    : buffer to decrypt and fill packet    [4 bytes]
 *
 * Outputs:
 * int validity             : returns 0 if valid crc, else invalid
 *
 * Result: 
 * pktptr is populated with ip_header, message, iv, and crc
 * from the buffer. we also calculate the crc checksum to see
 * if the packet was modified during transmit.
 * Returns a 0 if the packet was not modified
 *
 * Note: Only Access Point should be allowed to use this function
 */

int receive_packet(packet* pktptr, const unsigned char* buffer){
  crc32 check;
  strncpy(pktptr->encryption, buffer, 19); 
  decode_packet(pktptr); 
  store_crc(&check, pktptr->raw, 12);
  return crc_check(&pktptr->crc, &check);
}



/* hack_dest_of_packet
 * =========================================================================
 * Objective:
 * Modify the destination of an encrypted packet that has a CRC attached
 * and allow the packet to still pass the CRC checksum. In order to use
 * this function, you are expected to know the decrypted bytes in the
 * destination
 *
 * Inputs:
 * unsigned char* buffer              : buffer holding the encryption  [19 bytes]
 * const unsigned char* old_dest      : unencrypted dest on packet     [4 bytes]
 * const unsigned char* new_dest      : new dest to assign to packet   [4 bytes]
 *
 * Outputs:
 * None
 *
 * Result:
 * The buffer's new dest will replace the old dest.
 *
 * Note: Only CarolWEP should be allowed to use this function
 */

void hack_dest(unsigned char* buffer, const unsigned char* old_dest, const unsigned char* new_dest){
  packet my_pkt;
  packet cancel_pkt;
  unsigned char empty_buffer[4] = {0, 0, 0, 0};

  populate_packet(&my_pkt, empty_buffer, new_dest, empty_buffer);
  populate_packet(&cancel_pkt, empty_buffer, old_dest, empty_buffer);
  
  for(int i = 0; i < 16; ++i){
    buffer[i + 3] ^= my_pkt.raw[i];
    buffer[i + 3] ^= cancel_pkt.raw[i];
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
 * Places that encryption into the encryption buffer
 *
 * Inputs:
 * packet* pktptr : to produce and retrieve raw buffer and place encryption
 *
 * Outputs:
 * None
 *
 * Results:
 * Will generate the encryption and store into pktptr->encryption
 *
 */

void encode_packet(packet* pktptr){
  construct_packet(pktptr);
  strncpy(pktptr->encryption, pktptr->vec.arr, 3);
  RC4_IV(pktptr->encryption + 3, pktptr->raw, &pktptr->vec, 16);      
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
 * pktptr->encryption is decrypted and stored into pktptr->raw, identifies
 * parts from pktptr->raw and places them correctly into pktptr
 */

void decode_packet(packet* pktptr){
  strncpy(pktptr->vec.arr, pktptr->encryption, 3);
  RC4_IV(pktptr->raw, pktptr->encryption + 3, &pktptr->vec, 16);
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

  printf("\t\tCRC: ");
  for(int i = 0; i < 4; ++i){
    printf("%x ", pktptr->crc.result[i]);
  }

  printf("\nSRC: ");
  for(int i = 0; i < 4; ++i){
    printf("%X ", pktptr->header.src[i]);
  }

  printf("\t\tDEST: ");
  for(int i = 0; i < 4; ++i){
    printf("%X ", pktptr->header.dest[i]);
  }

  printf("\nMSG: ");
  for(int i = 0; i < 4; ++i){
    printf("%X ", pktptr->msg[i]);
  }

  printf("\t\tMSG (CHAR): ");
  for(int i = 0; i < 4; ++i){
    printf("%c ", pktptr->msg[i]);
  }

  printf("\nRAW: ");
  for(int i = 0; i < 16; ++i){
    printf("%X ", pktptr->raw[i]);
  }

  printf("\nencryption: ");
  for(int i = 0; i < 19; ++i){
    printf("%X ", pktptr->encryption[i]);
  }
  printf("\n");

}

#endif
