#ifndef __IP_HEADER_H__
#define __IP_HEADER_H__

#include "string.h"

typedef struct ip_headers {
  unsigned char src[4];
  unsigned char dest[4];
}ip_header;

/* populate_ip
 * ===========================================================================
 * Objective:
 * Populate ip_header struct with src ip and dest ip
 *
 * Inputs:
 * ip_header* header          : ip_header to populate   [8 bytes]
 * const unsigned char* src   : source ip               [4 bytes]
 * const unsigned char* dest  : destination ip          [4 bytes]
 *
 * Outputs:
 * None
 *
 * Result:
 * header will be populated with source ip and destination ip
 */

void populate_ip(ip_header* header, const unsigned char *src, const unsigned char *dest){
  memset(header->src, '\0', 4);  
  memset(header->dest, '\0', 4);
  strncpy(header->src, src, 4);
  strncpy(header->dest, dest, 4);
}



#endif
