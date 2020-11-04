#include "../headers/packet.h"

int main(){
	char src[4] = {120, 100, 10, 1};
	char dest[4] = {9, 1, 19, 29};
	char msg[4] = "123";

	packet pkt;
	populate_packet(&pkt, src, dest, msg);
	print_packet(&pkt);
	construct_packet(&pkt);
	
	printf("\nPrint raw packet: \n");

	// print iv
	for(int i = 0; i < 3; ++i){
		printf("%d ", pkt.vec.arr[i]);
	}

	// print raw
	for(int i = 0; i < 16; ++i){
		printf("%d ", pkt.raw[i]);
	}
}
