// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External Libraries
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

// Custom Libraries
#include "../headers/packet.h"
#include "../headers/err.h"

const unsigned char messages[20][100] = 
{
  "001", "002", "003", "004", "005",
  "006", "007", "008", "009", "010",
  "011", "012", "013", "014", "015",
  "016", "017", "018", "019", "020" 
};



/* alice.c
 * ==========================================================================  
 * Objective:
 * Emulate a client process Alice that generates a packet and sends
 * it on to the Access Point. In this case CarolWEP intercepts the packet
 * so in this emulation we are sending it straight to CarolWEP
 *
 * Actions:
 * 1) Generate packet
 * 2) Connection to CarolWEP 
 * 3) Send to CarolWEP
 */

int main(){

  // packet initialization
  packet pkt;


  // buffer initialization
  char sendBuffer[1024];

  // status initializations
  int out_socket = -1;
  int out_conn_status = -1;
  int out_send_status = -1;
  int out_close_status = -1;

  // construct sockaddr storing carolwep info
  struct sockaddr_in carolwep_address;
  carolwep_address.sin_family = AF_INET;
  carolwep_address.sin_port = htons(CAROLWEP_PORT);
  carolwep_address.sin_addr.s_addr = INADDR_ANY;

  for(int i = 0; i < 20; ++i){
    
    // clear buffers to send new string
    memset(sendBuffer, 0, sizeof(sendBuffer));
    populate_packet(&pkt, ALICE_IP, BOB_IP, messages[i]);  
    strncpy(sendBuffer, pkt.encoding,19);
    printf("SEQ %d: Sending packet: %s\n", i, messages[i]);

    // create the socket
    out_socket = socket(AF_INET, SOCK_STREAM, 0); 
    exit_on_fail("ALICE", "out_socket", out_socket);

    // connect to the server
    out_conn_status = connect(out_socket, (struct sockaddr*) &carolwep_address, sizeof(carolwep_address)); 
    exit_on_fail("ALICE", "out_conn_status", out_conn_status); 

    // send the packet
    out_send_status = send(out_socket, sendBuffer, sizeof(sendBuffer), 0);
    exit_on_fail("ALICE", "out_send_status", out_send_status); 

    // close the socket
    out_close_status = close(out_socket);
    exit_on_fail("ALICE", "out_close_status", out_close_status);

    usleep(500);
  }
}
