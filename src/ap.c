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



/* ap.c
 * =========================================================================
 * Objective:
 * Emulate a client/server process AP that receives encrypted packets, 
 * decrypts the packet, verifies packet integrity with CRC32 checksum,
 * and forwards the decrypted message to the destination ip
 *
 * Actions:
 * 1) Bind and listen for connections
 * 2) Accept connection from CarolWEP
 * 3) Read packets from CarolWEP
 * 4) Decrypt packets
 * 5) Verify CRC checksum, drop packets if CRC fails
 * 6) Connection to dest (either Bob or Carol) on packet
 * 7) Send message to dest
 */

int main(){

  // variable initializations
  packet pkt;
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
  int crc_status = -1; 
  int count = 0;

  // construct sockaddr storing ap info (mine)
  struct sockaddr_in my_address;
  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(AP_PORT);
  my_address.sin_addr.s_addr = INADDR_ANY;

  // construct sockaddr storing bob info
  struct sockaddr_in bob_address;
  bob_address.sin_family = AF_INET;
  bob_address.sin_port = htons(BOB_PORT);
  bob_address.sin_addr.s_addr = INADDR_ANY;

  // construct sockaddr storing carol info
  struct sockaddr_in carol_address;
  carol_address.sin_family = AF_INET;
  carol_address.sin_port = htons(CAROL_PORT);
  carol_address.sin_addr.s_addr = INADDR_ANY;

  // create LISTEN socket
  listen_socket = socket(AF_INET, SOCK_STREAM, 0);
  exit_on_fail("AP", "listen_socket", listen_socket);

  // bind listen_socket to local AP server
  listen_bind_status = bind(listen_socket, (struct sockaddr*) &my_address, sizeof(my_address));
  exit_on_fail("AP", "listen_bind_status", listen_bind_status);

  // wait and listen for packets
  listen_status = listen(listen_socket, 20);
  exit_on_fail("AP", "listen_status", listen_status);


  // continue accepting connections/packets
  while(1){

    // clear buffers before using
    memset(readBuffer, 0, sizeof(readBuffer));
    memset(sendBuffer, 0, sizeof(sendBuffer));
   
    // accept connection as IN socket
    in_socket = accept(listen_socket, NULL, NULL);
    exit_on_fail("AP", "in_socket", in_socket);

    // reads buffer coming from CarolWEP
    in_read_status = read(in_socket, readBuffer, sizeof(readBuffer));
    exit_on_fail("AP", "in_read_status", in_read_status);
    
    // receives packet and drops packet if fails checksum
    crc_status = receive_packet(&pkt, readBuffer);
    if(crc_status != 0){
      printf("SEQ %d: Packet is corrupt!\n", count++);
      continue;
    }

    // copy result into send buffer
    strncpy(sendBuffer, pkt.msg, 4);

    // create OUT socket
    out_socket = socket(AF_INET, SOCK_STREAM, 0);
    exit_on_fail("AP", "out_socket", out_socket);

    // Connect to the server based on dest
    if(strncmp(CAROL_IP, pkt.header.dest, 4) == 0)
    {
      printf("SEQ %d: Forwarding decrypted packet to Carol\n", count++);
      out_conn_status = connect(out_socket, (struct sockaddr*) &carol_address, sizeof(carol_address));
    }
    else if (strncmp(BOB_IP, pkt.header.dest, 4) == 0)
    {
      printf("SEQ %d: Forwarding decrypted packet to Bob\n", count++);
      out_conn_status = connect(out_socket, (struct sockaddr*) &bob_address, sizeof(bob_address));
    }
    exit_on_fail("AP", "out_conn_status", out_conn_status);

    // send the packet
    out_send_status = send(out_socket, sendBuffer, sizeof(sendBuffer), 0);
    exit_on_fail("AP", "out_send_status", out_send_status);

    // close OUT socket
    out_close_status = close(out_socket);
    exit_on_fail("AP", "out_close_status", out_close_status);

    // close IN socket
    in_close_status = close(in_socket);
    exit_on_fail("AP", "in_close_status", in_close_status);

    usleep(500); 
  }
}
