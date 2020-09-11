#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "server.h"
#include "broadcaster.h"

#define TOTAL_VALUE 10000000

// Driver function 
int main() 
{ 
    int server_listener_sockfd = create_srv_socket(); // TCP socket for listening

    struct sockaddr_in broadcast_addr;
    int udp_sock = create_udp_broadcast_socket( &broadcast_addr, UDP_PORT ); // UDP socke for broadcast

    if ( udp_sock > 0 )
    {
        send_udp_broadcast( udp_sock, &broadcast_addr );
    }

    pthread_t ids[16];  // client's threads id
    struct Args args[16];  // args for client's threads
    int threads_count = 0;

    // collect clients by TCP socket listening
    pthread_t listener_thread;
    struct ListenerArgs listenerArgs;
    listenerArgs.clients_count = 0;
    listenerArgs.args = &args[0];
    listenerArgs.listen_socket = server_listener_sockfd;

    pthread_create( &listener_thread, NULL, clients_listiner, &listenerArgs );
    sleep(3);
    pthread_cancel(listener_thread);
    // end of collect

    // calculate total CPU cores
    threads_count = listenerArgs.clients_count;
    int total_cpu = 0;
    for ( int i = 0; i < threads_count ; ++i )
    {
        total_cpu += args[i].cpu;
    }

    printf( "Total cpu core count = %d\n", total_cpu );
    int start_value = 0;
    int value_for_cpu = TOTAL_VALUE/total_cpu;
    // start threads for dialog with cliens "task-result"
    for ( int i = 0; i < threads_count ; ++i )
    {
        args[i].start_value = start_value;
        args[i].total_value = value_for_cpu * args[i].cpu;
        start_value += args[i].total_value;
        pthread_create(&ids[i], NULL, cli_thread, &args[i] );
        printf( "Start thread = %d\n", i );
    }

    double pi = 0;
    for(int i = 0; i < threads_count; i++)
    {
        pthread_join(ids[i], NULL);
        pi += args[i].rezult;
    }
    // end of threads

    printf( "Result = %lf \n", pi/total_cpu );

    close(server_listener_sockfd);
} 
