#ifndef __IP_HEADER_H__
#define __IP_HEADER_H__

#include "string.h"

typedef struct ip_headers {
	char src[4];
	char dest[4];
}ip_header;

void populate_ip(ip_header* header, char *src, char *dest){
	memset(header->src, '\0', 4*sizeof(char));	
	memset(header->dest, '\0', 4*sizeof(char));
	strncpy(header->src, src, 4);
	strncpy(header->dest, dest, 4);
}


#endif
