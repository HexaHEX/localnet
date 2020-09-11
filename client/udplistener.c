#include "udplistener.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int create_udp_listen_socket()
{
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("sock error\n");
        return -1;
    }

    int addr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in server_addr;
    memset((void*)&server_addr, 0, addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(UDP_PORT);

    int ret = bind(udp_sock, (struct sockaddr*)&server_addr, addr_len);
    if (ret < 0) {
        perror("bind error\n");
        return -1;
    }
    return udp_sock;
}

int listen_udp_broadcast(int udp_sock, struct sockaddr_in* client_addr)
{
    int result = 0;
    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(udp_sock, &readfd);

    if ( select(udp_sock+1, &readfd, NULL, NULL, 0) > 0)
    {
        if (FD_ISSET(udp_sock, &readfd))
        {
            char buffer[1024];

            socklen_t addr_len = sizeof(struct sockaddr_in);
            int count = recvfrom(udp_sock, buffer, 1024, 0, ( struct sockaddr*) client_addr, &addr_len);
            if ( strstr( buffer, IP_FOUND) )
            {
                memcpy(buffer, IP_FOUND_ACK, strlen(IP_FOUND_ACK)+1);
                count = sendto(udp_sock, buffer, strlen(buffer), 0, ( struct sockaddr*) client_addr, addr_len);
                result = 1;
            }
        }
    }
    return result;
}
