#ifndef __PACKET_H__
#define __PACKET_H__

#include "iv.h"
#include "ip_header.h"
#include "crc32.h"
#include "rc4.h"

typedef struct packets {
	iv vector;
	ip_header header;
	crc32 crc;
	char msg[4];
	char raw[16];
	char encrypt[19];
}packet;

// generates & populates crc
void populate_crc(packet* ptrpkt){
	char M[12];			
	strncpy(M, ptrpkt->header.src, 4);
	strncpy(M + 4, ptrpkt->header.dest, 4);
	strncpy(M + 8, ptrpkt->msg, 4);
	generate_crc(&ptrpkt->crc, M, 12);	
}

// HELPER - generates the pre-encrypted 16 byte packet into pkt.raw
void form_packet(packet* ptrpkt){
	strncat(ptrpkt->raw, ptrpkt->header.src, 4);
	strncat(ptrpkt->raw, ptrpkt->header.dest, 4);
	strncat(ptrpkt->raw, ptrpkt->msg, 4);
	strncat(ptrpkt->raw, ptrpkt->crc.result, 4);
}

// runs rc4 and places result into pkt.encrypt
void encrypt_packet(packet* ptrpkt){
	form_packet(ptrpkt);
	encrypt(ptrpkt, 16);		
}

// automatically sets values into packet accordingly
void populate_packet(packet* ptrpkt, char* src, char* dest, char* msg){
	strncpy(ptrpkt->msg, msg, 4);
	populate_ip(&ptrpkt->header, src, dest);		
	populate_iv(&ptrpkt->vector);
	populate_crc(ptrpkt);
	encrypt_packet(ptrpkt);
}

// prints raw packet values
void print_raw_packet(packet* ptrpkt){

	printf("\nIV:\n");
	for(int i = 0; i < 3; ++i){
		printf("IV index %d: %d\n", i, ptrpkt->vector.vec[i]);
	}

	printf("\nCRC:\n");
	for(int i = 0; i < 4; ++i){
		printf("CRC index %d: %d\n", i, ptrpkt->crc.result[i]);
	}


	printf("\nsrc IP: ");
	for(int i = 0; i < 4; ++i){
		printf("%d ", ptrpkt->header.src[i]);
	}

	printf("\ndest IP: ");
	for(int i = 0; i < 4; ++i){
		printf("%d ", ptrpkt->header.dest[i]);
	}

	printf("\nmessage ascii: ");
	for(int i = 0; i < 4; ++i){
		printf("%d", ptrpkt->msg[i]);
	}

	printf("\nmessage cstr: %s\n", ptrpkt->msg);

}

#endif
