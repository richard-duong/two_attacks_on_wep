#include "../headers/packet.h"
#include "../headers/rc4.h"

/* Objective:
 * Test the function of modifying the message Alice's message while under
 * encryption and still retaining CRC by Carol. Carol produces a packet with
 * her dest ip, and the rest is padded with zeroes. We calculate the CRC for
 * Carol's packet, and then xor the entire thing on top of Alice's encrypted
 * packet appropriately. Then when we decrypt it, we should have Alice's
 * message with Carol's dest IP written in, all the while passing the CRC
 *
 * Result:
 * CRC modification failed to pass the CRC checksum AND destination IP is not
 * Carol's after decrypting the final modified packet
 *
 * CRC modification method fails:
 * - I've talked to the professor about this, he says we can skip over this
 *   section for now.
 */

int main(){

  // define packet for alice
  packet a_pkt;
  unsigned char a_msg[5] = "nope";    
  populate_packet(&a_pkt, alice_ip, bob_ip, a_msg);
  printf("\n\nThis is Alice's Packet\n");
  print_packet(&a_pkt);

  hack_dest_of_packet(&a_pkt, bob_ip, carol_ip);
  int valid_packet = receive_packet(&a_pkt, a_pkt.encoding);

  printf("\n\nThis is Alice's packet after being modified by Carol (Expect 3333 for DEST)\n");
  print_packet(&a_pkt);

  if(valid_packet == 0){
    printf("\n\nCRC Checksum: PASSED\n");
  }
  else{
    printf("\n\nCRC Checksum: FAILED\n");
  }

}
