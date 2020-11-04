#ifndef __PACKET_H__
#define __PACKET_H__

struct packet {
	struct IV vector;
	struct ip_header header;
	char msg[4];
	char crc[4];
};



#endif
