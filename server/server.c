#include "server.h"

#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>


// Function designed for chat between client and server.
void func(int sockfd, int cli_num)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client %d: %s\t To client : ", cli_num, buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

void get_client_params( struct Args* pArgs )
{
    char buff[MAX];
    bzero(buff, MAX);

    printf("Read from client\n");
    // read CPU cores count from client and copy it in buffer
    read( pArgs->socket, buff, sizeof(buff));
    // print buffer which contains the client contents
    printf("From client %d: %s\n", pArgs->thread_num, buff);
    sscanf( buff, "%d\n", &pArgs->cpu );
}

void* cli_thread(void *arg)
{
    struct Args* pArgs = (struct Args*) arg;
    char buff[MAX];
    int n = 0;

    // and send that buffer to client
    sprintf( buff, "%d;%d\n", pArgs->start_value, pArgs->total_value);
    write( pArgs->socket, buff, sizeof(buff));

    // read calculating result from client and copy it in buffer
    read( pArgs->socket, buff, sizeof(buff));
    sscanf( buff, "%lf\n", &pArgs->rezult);
    printf("From client %d: result = %lf\n", pArgs->thread_num, pArgs->rezult );
}

int create_srv_socket()
{
    // socket create and verification
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("TCP socket creation failed...\n");
        exit(0);
    }
    else
        printf("TCP socket successfully created..\n");

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(TCP_PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("TCP socket bind failed...\n");
        exit(0);
    }
    else
        printf("TCP socket successfully binded..\n");

    return sockfd;
}

int accept_cli( int srv_sock)
{
    struct sockaddr_in cli;
    int len = sizeof(cli);

    // Accept the data packet from client and verification
    int connfd = accept( srv_sock, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    return connfd;
}

void* clients_listiner(void *arg)
{
    struct ListenerArgs* listenerArgs = (struct ListenerArgs*) arg;
    while( 1 )
    {
        if ((listen(listenerArgs->listen_socket, 5)) != 0)
        {
            printf("Listen failed...\n");
            exit(0);
        }
        else
        {
            printf("Server listening..\n");
            int connfd = accept_cli( listenerArgs->listen_socket );

            listenerArgs->args[ listenerArgs->clients_count].socket = connfd;
            listenerArgs->args[ listenerArgs->clients_count].thread_num = listenerArgs->clients_count;
            get_client_params( &listenerArgs->args[ listenerArgs->clients_count] );
            listenerArgs->clients_count++;
        }
    }
}
