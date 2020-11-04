#include "../headers/packet.h"
#include "../headers/rc4.h"

int main(){
	char src[4] = {1, 2, 3, 4};
	char dest[4] = {8, 8, 8, 8};	
	char msg[5] = "hate";

	packet pkt;
	populate_packet(&pkt, src, dest, msg);
	raw_packet(&pkt);


}
