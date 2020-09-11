#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_FOUND "IP_FOUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"
#define UDP_PORT 9999

int create_udp_listen_socket();
int listen_udp_broadcast(int udp_sock, struct sockaddr_in* client_addr);


#endif // UDPLISTENER_H
