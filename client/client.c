#include "client.h"

#define _GNU_SOURCE
#include <sched.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int create_cli_socket( struct sockaddr_in* servaddr, const char* addr, uint16_t port )
{
    // socket create and varification
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero( servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr->sin_family = AF_INET;
    servaddr->sin_addr.s_addr = inet_addr( addr );
    servaddr->sin_port = htons( port );

    return sockfd;
}


void DistributeRelativeCpu(const unsigned cpu)
{
    cpu_set_t cpusetp;
    CPU_ZERO(&cpusetp);
    CPU_SET(cpu, &cpusetp);
    int ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpusetp);
}
///????
void ChangeNextIndexCpu(const unsigned whole, unsigned remainder_np, unsigned current_number_proc,
    int save_current_number_for_proc, int is_remainder_decreased)
{
    if ( whole == 0)
        ++current_number_proc;
    else
    {
        if (save_current_number_for_proc % whole == 0)
        {
            if (remainder_np && is_remainder_decreased == 0)
            {
                --remainder_np;
                is_remainder_decreased = 1;
            }
            else
            {
                ++current_number_proc;
                is_remainder_decreased = 0;
                save_current_number_for_proc = 1;
            }

        }
        else
           ++save_current_number_for_proc;
    }
}

void* CalculatePiMethodMonteCarlo(void *arg)
{
    struct Args* a = (struct Args*)(arg);
    DistributeRelativeCpu( a->cpu );
    int count = a->count;
    double x, y;
    int rezult = 0;
    for(int i = 0; i < count; i++)
    {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        if (x * x + y * y <= 1)
            rezult++;
    }

    a->rezult = ((double)rezult) / count;
    printf( "count = %d result = %lf \n", count, a->rezult );
}

double ParallelCalculatePi(int start_value, int total_value)
{
    printf( " count = %d, threads = %d\n", start_value, total_value );

    unsigned processors = sysconf(_SC_NPROCESSORS_CONF);
    unsigned current_number_proc = 0;
    unsigned remainder_np = processors % processors, whole = processors / processors;
    int is_remainder_decreased = 0, save_current_number_for_proc = 1;

    pthread_t* ids = (pthread_t*) malloc( sizeof(pthread_t) * processors );
    struct Args** rezults = (struct Args** )  malloc( sizeof(struct Args*) * processors);
    int count = (total_value - start_value)/ processors;

    for(int thr = 0; thr < processors; thr++)
    {
        struct Args* arg = (struct Args*) malloc( sizeof(struct Args) );
        arg->count = count;
        arg->cpu = current_number_proc;
        arg->rezult = 0;
        pthread_create(&ids[thr], NULL, CalculatePiMethodMonteCarlo, arg);
        rezults[thr] = arg;

        ChangeNextIndexCpu( whole, remainder_np, current_number_proc,
                    save_current_number_for_proc, is_remainder_decreased);
    }

    double pi = 0;
    for(int i = 0; i < processors; i++)
    {
        pthread_join(ids[i], NULL);
        pi += rezults[i]->rezult;
    }

    return 4 * pi;
}

void func(int sockfd)
{
    char buff[MAX];
    int n;

    bzero(buff, sizeof(buff));
    unsigned processors = sysconf(_SC_NPROCESSORS_CONF);

    int cpu_core_num = processors;
    int start_value = 0;
    int total_value = 0;

    sprintf( buff, "%d\n", cpu_core_num);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));

    read(sockfd, buff, sizeof(buff));
    sscanf( buff, "%d;%d\n", &start_value, &total_value );
    printf("From Server : start_value = %d, totatl_value = %d", start_value, total_value );
    bzero(buff, sizeof(buff));

    double result = ParallelCalculatePi( start_value, total_value);

    sprintf( buff, "%lf\n", result);
    write(sockfd, buff, sizeof(buff));
}
