// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External Libraries
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Custom Libraries
#include "../headers/packet.h"
#include "../headers/err.h"



/* carolwep.c
 * ===========================================================================
 * Objective:
 * Emulate a server/client process carolWEP that receives a packet from alice,
 * and then proceeds to modify every other packet over WEP encryption making
 * sure that the dest leads back to carol, and the crc checksum is still valid
 * Then sends all packets to the AP.
 *
 * Actions:
 * 1) Bind and listen for connections 
 * 2) Accept connection from Alice
 * 3) Read packets from Alice
 * 4) Modify packets from Alice
 * 5) Connection to AP
 * 6) Send modified packets to AP
 */

int main(){

  // variable initializations
  char readBuffer[1024];
  char sendBuffer[1024];

  // status initializations
  int listen_socket = -1;         
  int listen_bind_status = -1;
  int listen_status = -1;
  int in_socket = -1;
  int in_accept_status = -1;
  int in_read_status = -1;
  int in_close_status = -1;
  int out_socket = -1;
  int out_conn_status = -1;
  int out_send_status = -1;
  int out_close_status = -1; 
  int modify = 0;
  int count = 0;

  // construct sockaddr storing carolwep info (mine)
  struct sockaddr_in my_address;
  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(CAROLWEP_PORT);
  my_address.sin_addr.s_addr = INADDR_ANY;

  // construct sockaddr storing ap info
  struct sockaddr_in ap_address;
  ap_address.sin_family = AF_INET;
  ap_address.sin_port = htons(AP_PORT);
  ap_address.sin_addr.s_addr = INADDR_ANY;

  // create listening socket
  listen_socket = socket(AF_INET, SOCK_STREAM, 0);
  exit_on_fail("CAROLWEP", "listen_socket", listen_socket);

  // bind listen_socket to local carolWEP server
  listen_bind_status = bind(listen_socket, (struct sockaddr*) &my_address, sizeof(my_address));
  exit_on_fail("CAROLWEP", "listen_bind_status", listen_bind_status);

  // listen for connections
  listen_status = listen(listen_socket, 20);
  exit_on_fail("CAROLWEP", "listen_status", listen_status);
 

  // continue accepting connections/packets
  while(1){

    // clear buffers before using
    memset(readBuffer, 0, sizeof(readBuffer));
    memset(sendBuffer, 0, sizeof(sendBuffer));
   
    // accept connection as IN socket
    in_socket = accept(listen_socket, NULL, NULL);
    exit_on_fail("CAROLWEP", "in_socket", in_socket);

    // reads buffer coming from Alice
    in_read_status = read(in_socket, readBuffer, sizeof(readBuffer));
    exit_on_fail("CAROLWEP", "in_read_status", in_read_status); 

    // modify every 2nd message to change destination to carol
    if(modify == 1)
    {
      hack_dest(readBuffer, BOB_IP, CAROL_IP);
    } 

    // update modify to alternate between [0 - 1]
    modify = 1 - modify;

    // copy result into the sendBuffer
    strncpy(sendBuffer, readBuffer, 19);

    // create OUT socket
    out_socket = socket(AF_INET, SOCK_STREAM, 0);
    exit_on_fail("CAROLWEP", "out_socket", out_socket);

    // connect to AP
    out_conn_status = connect(out_socket, (struct sockaddr*) &ap_address, sizeof(ap_address));
    exit_on_fail("CAROLWEP", "out_conn_status", out_conn_status);

    // send the packet
    
    printf("SEQ %d: Forwarding modified message to AP\n", count++);
    out_send_status = send(out_socket, sendBuffer, sizeof(sendBuffer), 0);
    exit_on_fail("CAROLWEP", "out_send_status", out_send_status);

    // close OUT socket
    out_close_status = close(out_socket); 
    exit_on_fail("CAROLWEP", "out_close_status", out_close_status);

    // close IN socket
    in_close_status = close(in_socket);
    exit_on_fail("CAROLWEP", "in_close_status", in_close_status);

    usleep(500); 
  }
}
