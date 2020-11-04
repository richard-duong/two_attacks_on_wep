#include "../headers/packet.h"
#include "../headers/rc5.h"

int main(){
	char src[4] = {1, 2, 3, 4};
	char dest[4] = {8, 8, 8, 8};	
	char msg[5] = "hate";

	packet pkt;
	populate_packet(&pkt, src, dest, msg);
	print_packet(&pkt);


}
