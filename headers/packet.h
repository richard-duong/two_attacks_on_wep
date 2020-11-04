#ifndef __PACKET_H__
#define __PACKET_H__

#include "iv.h"
#include "ip_header.h"
#include "crc32.h"
#include "rc5.h"

/* Add prototypes here */

typedef struct packets {
  iv vec;
  ip_header header;
  crc32 crc;
  char msg[4];
  char raw[16];
  char encoding[19];
}packet;

// generates & populates crc
void populate_crc(packet* pktptr){
  int src_len = 4;
  int dest_len = 4;
  int msg_len = 4;
  int total_len = src_len + dest_len + msg_len;
  char* M = malloc(total_len);

  // copies ip header & message to M
  strncpy(M, pktptr->header.src, src_len);
  strncpy(M + src_len, pktptr->header.dest, dest_len);
  strncpy(M + src_len + dest_len, pktptr->msg, msg_len);
  generate_crc(&pktptr->crc, M, total_len);  

  // deallocates memory
  free(M);
}

// HELPER - generates the pre-encrypted 16 byte packet into pkt.raw
void construct_packet(packet* pktptr){
  int src_len = 4;
  int dest_len = 4;
  int msg_len = 4;
  int crc_len = 4;

  strncpy(pktptr->raw, pktptr->header.src, src_len);
  strncpy(pktptr->raw + src_len, pktptr->header.dest, dest_len);
  strncpy(pktptr->raw + src_len + dest_len, pktptr->msg, msg_len);
  strncpy(pktptr->raw + src_len + dest_len + msg_len, pktptr->crc.result, crc_len);
}


// HELPER - deconstructs raw value and puts values into packet accordingly
void deconstruct_packet(packet* pktptr){
  int src_len = 4;
  int dest_len = 4;
  int msg_len = 4;
  int crc_len = 4;

  strncpy(pktptr->header.src, pktptr->raw, src_len);
  strncpy(pktptr->header.dest, pktptr->raw + src_len, dest_len);
  strncpy(pktptr->msg, pktptr->raw + src_len + dest_len, msg_len);
  strncpy(pktptr->crc.result, pktptr->raw + src_len + dest_len + msg_len,  crc_len);
}


// runs rc4 and places result into pkt.encrypt
void encode_packet(packet* pktptr){
  int raw_len = 16;
  int iv_len = 3;

  construct_packet(pktptr);
  strncpy(pktptr->encoding, pktptr->vec.arr, iv_len);
  RC4_IV(pktptr->encoding + iv_len, pktptr->raw, &pktptr->vec, raw_len);      
}

void decode_packet(packet* pktptr){
  int raw_len = 16;
  int iv_len = 3;  

  strncpy(pktptr->vec.arr, pktptr->encoding, iv_len);
  RC4_IV(pktptr->raw, pktptr->encoding + iv_len, &pktptr->vec, raw_len);
  deconstruct_packet(pktptr);

  // check CRC for validity

}

// automatically translates values into packet accordingly
void populate_packet(packet* pktptr, char* src, char* dest, char* msg){
  int msg_len = 4;

  strncpy(pktptr->msg, msg, msg_len);
  populate_ip(&pktptr->header, src, dest);    
  populate_iv(&pktptr->vec);
  populate_crc(pktptr);
  encode_packet(pktptr);
}

// decrypts and translates packet down accordingly (Access Point Only)
// returns -1 if crc is invalid, 1 if valid
int receive_packet(packet* pktptr, char* buffer){
  int buffer_len = 19;
  strncpy(pktptr->encoding, buffer, buffer_len); 
  decode_packet(pktptr);
  // verify crc here and return true or false accordingly

  return 1;
}

// prints raw packet values
void print_packet(packet* pktptr){

  printf("\nIV:\n");
  for(int i = 0; i < 3; ++i){
    printf("%d ", pktptr->vec.arr[i]);
  }

  printf("\n\nCRC:\n");
  for(int i = 0; i < 4; ++i){
    printf("%d ", pktptr->crc.result[i]);
  }

  printf("\n\nsrc IP: \n");
  for(int i = 0; i < 4; ++i){
    printf("%d ", pktptr->header.src[i]);
  }

  printf("\n\ndest IP: \n");
  for(int i = 0; i < 4; ++i){
    printf("%d ", pktptr->header.dest[i]);
  }

  printf("\n\nmessage ascii: \n");
  for(int i = 0; i < 4; ++i){
    printf("%d ", pktptr->msg[i]);
  }

  printf("\n\nmessage cstr: \n");
  for(int i = 0; i < 4; ++i){
    printf("%c ", pktptr->msg[i]);
  }

  printf("\n\nencoding: \n");
  for(int i = 0; i < 19; ++i){
    printf("%d ", pktptr->encoding[i]);
  }
  printf("\n");

}

#endif
