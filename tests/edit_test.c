#include "../headers/packet.h"
#include "../headers/rc4.h"

/* edit_test.c
 * =================================================================================
 * Objective:
 * Test the function of modifying the message Alice's message while under
 * encryption and still retaining CRC by Carol. Carol produces a packet with
 * her dest ip, and the rest is padded with zeroes. We calculate the CRC for
 * Carol's packet, and then xor the entire thing on top of Alice's encrypted
 * packet appropriately. Then when we decrypt it, we should have Alice's
 * message with Carol's dest IP written in, all the while passing the CRC
 *
 * Result:
 * Packet passes after modifying destination from Bob's ip address to Carol's address
 */

int main(){

  packet a_pkt;
  unsigned char a_msg[5] = "nope";    
  populate_packet(&a_pkt, alice_ip, bob_ip, a_msg);

  printf("\n\nPacket before being modified by Carol\n");
  print_packet(&a_pkt);

  hack_dest_of_packet(&a_pkt, bob_ip, carol_ip);
  int valid_packet = receive_packet(&a_pkt, a_pkt.encoding);

  printf("\n\nPacket after being modified by Carol \n");
  print_packet(&a_pkt);

  if(valid_packet == 0){
    printf("\n\nCRC Checksum: PASSED\n");
  }
  else{
    printf("\n\nCRC Checksum: FAILED\n");
  }

}
