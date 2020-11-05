#ifndef __IP_HEADER_H__
#define __IP_HEADER_H__

#include "string.h"

typedef struct ip_headers {
  unsigned char src[4];
  unsigned char dest[4];
}ip_header;

void populate_ip(ip_header* header, unsigned char *src, unsigned char *dest){
  memset(header->src, '\0', 4);  
  memset(header->dest, '\0', 4);
  strncpy(header->src, src, 4);
  strncpy(header->dest, dest, 4);
}



#endif
