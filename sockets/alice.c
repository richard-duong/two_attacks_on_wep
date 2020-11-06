#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "../headers/packet.h"

int main(){

  // buffer initialization
  char sendBuffer[1024];
  memset(sendBuffer, 0,strlen(sendBuffer));

  // status initializations
  int out_socket = -1;
  int out_conn_status = -1;
  int out_send_status = -1;
  int out_close_status = -1;

  // socket address info for carolwep (to send)
  struct sockaddr_in carolwep_address;
  carolwep_address.sin_family = AF_INET;
  carolwep_address.sin_port = htons(49500);
  carolwep_address.sin_addr.s_addr = INADDR_ANY;


  // create the socket
  out_socket = socket(AF_INET, SOCK_STREAM, 0); 

  if(out_socket < 0){
     printf("\nError: Failed to create socket for Alice\n");
     exit(1);
  }


  // connect to the server
  out_conn_status = connect(out_socket, (struct sockaddr*) &carolwep_address, sizeof(carolwep_address)); 

  if(out_conn_status < 0){
    printf("\nError: Failed to connect to CarolWEP from Alice\n");
    exit(1);
  }
  
  //declare the packet
  packet pkt;
  char msg[5] = "Sack"; 
  populate_packet(&pkt,alice_ip,bob_ip,msg);  
  strncpy(sendBuffer,pkt.encoding,19);

  // send the packet
  out_send_status = send(out_socket, sendBuffer, sizeof(sendBuffer), 0);
 
  if(out_send_status < 0){
    printf("\nError: Failed to send packet to CarolWEP from Alice\n");
  }
  else{
    printf("\nSuccess: Alice sends packet successfully\n");
    print_packet(&pkt);
  }
 

  // close the socket (1 means to stop transmitting)
  out_close_status = close(out_socket);

  if(out_close_status < 0){
    printf("\nError: Failed to close sending socket from CarolWEP\n");
  }
}
