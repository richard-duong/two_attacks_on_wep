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
  unsigned char a_src[4] = {1, 1, 1, 1};            // alice's ip
  unsigned char a_dest[4] = {2, 2, 2, 2};           // bob's ip
  populate_packet(&a_pkt, a_src, a_dest, a_msg);
  print_packet(&a_pkt);


  // define packet for carol
  // pad everything with zeroes except for destination
  packet c_pkt;
  unsigned char c_msg[4] = {0, 0, 0, 0};          // pad with zeroes
  unsigned char c_src[4] = {0, 0, 0, 0};          // pad with zeroes
  unsigned char c_dest[4] = {3, 3, 3, 3};         // carol's ip
  populate_packet(&c_pkt, c_src, c_dest, c_msg);

  for(int i = 0; i < 16; ++i){
    a_pkt.encoding[i + 3] ^= c_pkt.raw[i];
  }

  int valid_packet = receive_packet(&a_pkt, a_pkt.encoding);
  print_packet(&a_pkt);

  if(valid_packet == 0){
    printf("It's good!\n");
  }
  else{
    printf("It was edited :c\n");
  }



}
