#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../headers/packet.h"


int main(){

  // buffer initializations
  packet read_pkt;
  packet send_pkt;
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

  int crc_status = -1;
 

  // socket address info for carolwep (to receive) 
  struct sockaddr_in my_address;
  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(49500);
  my_address.sin_addr.s_addr = INADDR_ANY;

  struct sockaddr_in ap_address;
  ap_address.sin_family = AF_INET;
  ap_address.sin_port = htons(49000);
  ap_address.sin_addr.s_addr = INADDR_ANY;

  // create sockets for listening and sending
  listen_socket = socket(AF_INET, SOCK_STREAM, 0);


  // bind listen_socket to carolWEP server
  listen_bind_status = bind(listen_socket, (struct sockaddr*) &my_address, sizeof(my_address));
  
  if(listen_bind_status < 0){
    printf("\nError: Failed to bind listening socket to CarolWEP server\n");
  }


  // wait and listen for packets
  listen_status = listen(listen_socket, 20);

  if(listen_status == -1){
    printf("\nError: Failed to listen for packets\n");
  }
 
  int modify = 0;
  while(1){
   
    // accept the incoming connection 
    in_socket = accept(listen_socket, NULL, NULL);

    // reads buffer coming from Alice
    in_read_status = read(in_socket, readBuffer, sizeof(readBuffer));
   

    // Decrypt the packet
    crc_status = receive_packet(&read_pkt, readBuffer);  
    if(crc_status != 0){
      printf("\nError: Packet sent to CarolWEP and received from Alice was modified!\n"); 
    }
    else{
      printf("\nSuccess: Packet sent to CarolWEP was received successfully!\n");
      print_packet(&read_pkt);
    }
    
    // PERFORM EVERY OTHER PACKET. modify variable will oscillate
    // Make sure to edit this to modify the destination
    if(modify == 1)
    {
      // Prepare the packet with adjusted destination
      printf("Enters Carol\n");
      populate_packet(&send_pkt, read_pkt.header.src, carol_ip, read_pkt.msg);
      strncpy(sendBuffer, send_pkt.encoding, 19);
    }
    else
    {
      printf("Enters Bob\n");
      populate_packet(&send_pkt, read_pkt.header.src, read_pkt.header.dest, read_pkt.msg);
      strncpy(sendBuffer, send_pkt.encoding, 19);
    }



    printf("\nThis is the package once it is sent: \n");
    print_packet(&send_pkt);


    // Create OUT socket
    out_socket = socket(AF_INET, SOCK_STREAM, 0);

    // CONNECT CarolWEP to AP
    out_conn_status = connect(out_socket, (struct sockaddr*) &ap_address, sizeof(ap_address));

    if(out_conn_status < 0){
      printf("\nError: Failed to connect to AP from CarolWEP\n");
    }


    // send the packet
    out_send_status = send(out_socket, sendBuffer, sizeof(sendBuffer), 0);
    
    if(out_send_status < 0){
      printf("\nError: Failed to send packet to CarolWEP from Alice\n");
    }

    close(in_socket); 
    close(out_socket);
    modify = 1 - modify; 
    sleep(2); 
  }
}
