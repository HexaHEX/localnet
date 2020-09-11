#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_FOUND "IP_FOUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"
#define UDP_PORT 9999

int create_udp_broadcast_socket( struct sockaddr_in* broadcast_addr, uint16_t port);
int send_udp_broadcast( int udp_sock, struct sockaddr* broadcast_addr );
int recv_reply( int udp_sock, struct sockaddr* broadcast_addr );

#endif // BROADCASTER_H
