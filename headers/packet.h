#ifndef __PACKET_H__
#define __PACKET_H__

#include "iv.h"
#include "ip_header.h"
#include "crc32.h"

typedef struct packets {
	struct IV vector;
	struct ip_header header;
	char msg[4];
	char crc[4];
}packet;

// automatically 
void populate_packet(packet* pkt, char* src, char* dest, char* msg){
	strncpy(pkt->msg, msg, 4);
	populate_ip(pkt->header, src, dest);		
	populate_iv(pkt->vector);
}




#endif
