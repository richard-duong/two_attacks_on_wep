#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../headers/packet.h"

// non persistent
int main(){

  // buffer initializations
  char readBuffer[1024];
  memset(readBuffer, 0, sizeof(readBuffer));

  // status initializations
  int listen_socket = -1;
  int listen_bind_status = -1;
  int listen_status = -1;

  int in_socket = -1;
  int in_accept_status = -1;
  int in_read_status = -1;
  int in_close_status = -1;


  // socket address info for carol (to receive) 
  struct sockaddr_in my_address;
  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(48500);
  my_address.sin_addr.s_addr = INADDR_ANY;

  // create sockets for listening and sending
  listen_socket = socket(AF_INET, SOCK_STREAM, 0);


  // bind listen_socket to carolWEP server
  listen_bind_status = bind(listen_socket, (struct sockaddr*) &my_address, sizeof(my_address));
  
  if(listen_bind_status < 0){
    printf("\n Error: Failed to bind listening socket to CarolWEP server\n");
  }


  // wait and listen for packets
  listen_status = listen(listen_socket, 20);

  if(listen_status == -1){
    printf("\nError: Failed to listen for packets\n");
  }
  
  packet pkt;
  int crc_status = -1;
 
  while(1){
   
    printf("Waiting on connection...\n");

    // accept the incoming connection ***** Check later recv fails 
    in_socket = accept(listen_socket, NULL, NULL);

    printf("Accepted connection...\n");


    // reads buffer coming from Alice ***** if not reading, add while loop
    in_read_status = read(in_socket, readBuffer, sizeof(readBuffer));

    printf("\nReceived packet: %s", readBuffer);
    
    crc_status = receive_packet(&pkt, readBuffer);
    if (crc_status != 0){
      printf("Error: Packet recieved from AP did not pass checksum.");
    }
    else
    {
      printf("Success: Packet recieved from AP passed checksum.");
    }
    
    printf("Message: ");
    
    for(int i = 0; i < 4; i++)
    {
      printf("%c,", pkt.msg[i]);
    }
    
    close(in_socket);

    sleep(2); 
  }

}
