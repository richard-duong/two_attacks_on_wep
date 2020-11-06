#include "../headers/packet.h"
#include "../headers/rc4.h"

int main(){
  unsigned char src[4] = {1, 2, 3, 4};
  unsigned char dest[4] = {8, 8, 8, 8};  
  unsigned char msg[5] = "nope";
  int valid_packet;

  packet send_pkt;
  populate_packet(&send_pkt, src, dest, msg);

  printf("\n\nOriginal Packet\n=============\n");
  print_packet(&send_pkt);

  packet recv_pkt;
  valid_packet = receive_packet(&recv_pkt, send_pkt.encryption);

  printf("\n\nReceived Packet\n===============\n");
  print_packet(&recv_pkt);
  printf("\n");

  if(valid_packet == 0){
    printf("Packet passed CRC!\n");
  }

  else{
    printf("Packet failed CRC!\n");
  }
  
}
