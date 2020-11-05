#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include packet.h

int main(int argc, char const *argv[]) 
{ 
    int alice = 0, valread; 
    struct sockaddr_in serv_addr; 
    
    char buffer[1024] = {0}; 
    if ((alice = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(50000); 

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 

    if (connect(alice, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    
    packet pkt;
    char src[4] = {1, 1, 1, 1};
    char dest[4] = {4, 4, 4, 4}; // always goes to Bob
    char msg[5] = {0, 0, 0, 0}; 
    populate_packet(*pkt, src,dest,msg);
    
    strncpy(buffer,pkt.encoding,19);
    send(sock , buffer , 1024 , 0 ); 
    
    //valread = read( sock , buffer, 1024); 
    //printf("%s\n", buffer); 

    return 0; 
}