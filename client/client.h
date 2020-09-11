#ifndef CLIENT_H
#define CLIENT_H

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX 80
#define TCP_PORT 8080
#define SA struct sockaddr

struct Args
{
    int count;
    int cpu;
    double rezult;
};

void func(int sockfd);
int create_cli_socket( struct sockaddr_in* servaddr, const char* addr, uint16_t port );


#endif // CLIENT_H
