#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// port
int main(int args, char ** argv) {
    struct sockaddr_in servaddr, client;
    int socket_desc , client_sock , read_size;

    int port = atoi(argv[1]);
    printf("%d", port);

    socket_desc = socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    bzero( (void *)&servaddr, sizeof(servaddr) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
    servaddr.sin_port = htons(port);

    if( bind( socket_desc, (struct sockaddr *)&servaddr, sizeof(servaddr) ) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    listen( socket_desc, 5 );

    puts("Waiting for incoming connections...");
    int c;
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    //Receive a message from client
    char client_message[2000];
    while( (read_size = read(client_sock , client_message , sizeof(client_message))) > 0 )
    {
        puts(client_message);
        //Send the message back to client
        write(client_sock , client_message , strlen(client_message));
        memset( &client_message, 0, sizeof(client_message));

    }

    close(client_sock);

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}