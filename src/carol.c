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



/* carol.c
 * ===========================================================================
 * Objective:
 * Emulate a server process Carol that receives a message from the Access Point
 * and displays the received message.
 *
 * Actions:
 * 1) Bind and listen for connections
 * 2) Accept connection from AP
 * 3) Read packets from AP
 * 4) Display onto screen
 */

int main(){

  // variable declarations
  char readBuffer[1024];

  // status initializations
  int listen_socket = -1;
  int listen_bind_status = -1;
  int listen_status = -1;
  int in_socket = -1;
  int in_accept_status = -1;
  int in_read_status = -1;
  int in_close_status = -1;
  int count = 0;

  // socket address info for carol (mine) 
  struct sockaddr_in my_address;
  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(CAROL_PORT);
  my_address.sin_addr.s_addr = INADDR_ANY;

  // create LISTEN socket
  listen_socket = socket(AF_INET, SOCK_STREAM, 0);
  exit_on_fail("CAROL", "listen_socket", listen_socket);

  // bind listen_socket to local carol server
  listen_bind_status = bind(listen_socket, (struct sockaddr*) &my_address, sizeof(my_address));
  exit_on_fail("CAROL", "listen_bind_status", listen_bind_status);

  // wait and listen for packets
  listen_status = listen(listen_socket, 20);
  exit_on_fail("CAROL", "listen_status", listen_status);
 
  while(1){

    // clear buffers before using
    memset(readBuffer, 0, sizeof(readBuffer));

    // accept connection as IN socket
    in_socket = accept(listen_socket, NULL, NULL);
    exit_on_fail("CAROL", "in_socket", in_socket);

    // reads buffer coming from AP
    in_read_status = read(in_socket, readBuffer, sizeof(readBuffer));
    exit_on_fail("CAROL", "in_read_status", in_read_status);
    
    // close IN socket
    in_close_status = close(in_socket);
    exit_on_fail("AP", "in_close_status", in_close_status);
    printf("SEQ %d: Received packet: %s\n", count++, readBuffer);

    usleep(250); 
  }

}
