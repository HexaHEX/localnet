#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include "udplistener.h"
#include "client.h"


int main() 
{ 
    int udp_sock = create_udp_listen_socket();

    struct sockaddr_in client_addr;
    if ( udp_sock > 0)
    {
        if ( listen_udp_broadcast( udp_sock, &client_addr) )
        {
            printf("\nClient connection information:\n\t IP: %s, Port: %d\n",
                   inet_ntoa( client_addr.sin_addr ), ntohs( client_addr.sin_port ));
        }
        else
        {
            printf("\nCan not listen UDP broadcast message.\n");
        }
    }

    struct sockaddr_in servaddr;
    int sockfd = create_cli_socket( &servaddr, inet_ntoa( client_addr.sin_addr ), TCP_PORT );

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	func(sockfd); 


    close(sockfd);
} 
