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
  char sendBuffer[1024];
  memset(readBuffer, 0, sizeof(readBuffer));
  memset(sendBuffer, 0, sizeof(sendBuffer));

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
 

  // socket address info for AP (to receive) 
  struct sockaddr_in my_address;
  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(49000);
  my_address.sin_addr.s_addr = INADDR_ANY;

  struct sockaddr_in bob_address;
  bob_address.sin_family = AF_INET;
  bob_address.sin_port = htons(48500);
  bob_address.sin_addr.s_addr = INADDR_ANY;

  struct sockaddr_in carol_address;
  carol_address.sin_family = AF_INET;
  carol_address.sin_port = htons(48000);
  carol_address.sin_addr.s_addr = INADDR_ANY;


  // create sockets for listening and sending
  listen_socket = socket(AF_INET, SOCK_STREAM, 0);
  out_socket = socket(AF_INET, SOCK_STREAM, 0);


  // bind listen_socket to local AP server
  listen_bind_status = bind(listen_socket, (struct sockaddr*) &my_address, sizeof(my_address));
 
  if(listen_bind_status < 0){
    printf("\nError: Failed to bind listening socket to CarolWEP server\n");
  }
  else{
    printf("\nSuccess: Listening socket is bound to local AP server\n");
  }


  // wait and listen for packets
  listen_status = listen(listen_socket, 20);

  if(listen_status == -1){
    printf("\nError: Failed to listen for packets\n");
  }
  else{
    printf("\nSuccess: Successfully listening to packets\n");
  }
 
  packet pkt;
  int crc_status = -1;

  while(1){
   
    // accept the incoming connection ***** Check later recv fails 
    in_socket = accept(listen_socket, NULL, NULL);

    // reads buffer coming from Alice ***** if not reading, add while loop
    in_read_status = read(in_socket, readBuffer, sizeof(readBuffer));


    /* KEEP NOTE FOR DECRYPTION HERE
     *
     *
     *
     *
     */
    
    crc_status = receive_packet(&pkt, readBuffer);
    printf("\nsendBuffer: %s\n", readBuffer);
    if (crc_status != 0){
      printf("\nError: Packet recieved from carolWEP did not pass checksum.\n");
    }
    else
    {
      printf("\nSuccess: Packet recieved from carolWEP passed checksum.\n");
    }

    printf("\nReceived packet: %s\n", readBuffer);

    // Connect to the correct remote server
    if(strncmp(carol_ip,pkt.header.dest, 4) == 0){
      out_conn_status = connect(out_socket, (struct sockaddr*) &carol_address, sizeof(carol_address));
    }

    else if (strncmp(bob_ip,pkt.header.dest,4) == 0)
    {
      out_conn_status = connect(out_socket, (struct sockaddr*) &bob_address, sizeof(bob_address));
    }

    if(out_conn_status < 0){
      printf("\nError: Failed to connect to bob or carol from CarolWEP\n");
    }



    // send the packet
    out_send_status = send(out_socket, sendBuffer, sizeof(sendBuffer), 0);
    
    if(out_send_status < 0){
      printf("\n Error: Failed to send packet to CarolWEP from Alice\n");
    }

    close(out_socket);
    close(in_socket);

    sleep(2); 
  }

}
