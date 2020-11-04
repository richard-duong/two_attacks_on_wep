#include "../headers/packet.h"

int main(){
	char src[4] = {120, 100, 10, 1};
	char dest[4] = {9, 1, 19, 29};
	char msg[4] = "123";

	packet pkt;
	populate_packet(&pkt, src, dest, msg);
	raw_packet(&pkt);
}
