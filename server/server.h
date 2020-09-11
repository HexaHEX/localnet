#ifndef SERVER_C
#define SERVER_C

#define MAX 80
#define TCP_PORT 8080
#define SA struct sockaddr

struct Args
{
    int socket;
    int thread_num;
    int start_value;
    int total_value;
    int cpu;
    double rezult;
};

struct ListenerArgs
{
    int listen_socket;
    struct Args* args;
    int clients_count;
};


void func(int sockfd, int cli_num);
void get_client_params( struct Args* pArgs );
void* cli_thread(void *arg);
int create_srv_socket();
int accept_cli( int srv_sock);
void* clients_listiner(void *arg);

#endif // SERVER_C
