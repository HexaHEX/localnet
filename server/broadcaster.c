#include "broadcaster.h"
#include <stdio.h>
#include <string.h>

int create_udp_broadcast_socket( struct sockaddr_in* broadcast_addr, uint16_t port)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("sock error");
        return -1;
    }

    int yes = 1;
    int ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
    if (ret == -1) {
        perror("setsockopt error");
        return 0;
    }

    int addr_len = sizeof(struct sockaddr_in);

    memset((void*)broadcast_addr, 0, addr_len);
    broadcast_addr->sin_family = AF_INET;
    broadcast_addr->sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcast_addr->sin_port = htons( port );

    return sock;
}

int send_udp_broadcast( int udp_sock, struct sockaddr* broadcast_addr )
{
    int addr_len = sizeof( struct sockaddr_in );
    int ret = sendto(udp_sock, IP_FOUND, strlen(IP_FOUND), 0, broadcast_addr, addr_len);
    return (ret > 0);
}

int recv_reply( int udp_sock, struct sockaddr* broadcast_addr )
{
    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET( udp_sock, &readfd);

    int ret = select( udp_sock + 1, &readfd, NULL, NULL, NULL);

    if (ret > 0)
    {
        if (FD_ISSET( udp_sock, &readfd))
        {
            struct sockaddr_in server_addr;
            char buffer[1024];
            int addr_len = sizeof( struct sockaddr_in );
            int count = recvfrom( udp_sock, buffer, 1024, 0, (struct sockaddr*)&server_addr, &addr_len);
            printf("\trecvmsg is %s\n", buffer);
            if (strstr(buffer, IP_FOUND_ACK))
            {
                printf("\tfound listener IP is %s, Port is %d\n", inet_ntoa(server_addr.sin_addr),htons(server_addr.sin_port));
            }
        }
    }
}

