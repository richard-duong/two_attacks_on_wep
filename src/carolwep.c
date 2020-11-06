#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include packet.h

int main(){
    
    int alice_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    if ((alice_fd = socket(AF_INET, SOCK_STREAM, 0) == 0))
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    //attaching to socket 169.235.16.75 = 49500
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT), &opt, sizeof(opt))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (49500);
    
    if(bind(alice_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    if((new_socket = accept(alice_fd, (struct sockaddr *)&address,(socklen_t&addrlen)) < 0))
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    valread = read( new_socket, buffer, 1024);
    
    packet pkt;
    char src[4] = {0, 0, 0, 0};
    char dest[4] = {0, 0, 0, 0};
    char msg[5] = {0, 0, 0, 0};
    populate_packet(&pkt, src, dest, msg );
    
    for(int i = 0; i < 16; i++)
    {
        buffer[i+3] = pkt.raw[i] ^ buffer[i+3];
    }
    //does setting pkt.raw affect other data fields? i.e) will it change char msg[], src[], dest[]
    
    
    
    
    
    return 0;
}
