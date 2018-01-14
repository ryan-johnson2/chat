#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include <pthread.h>

#include <message_queue.h>

struct cl_data {
    pthread_t tid;
    int fd;
    char *un;
    struct message_queue *mq;
};

struct cl_node {
    struct cl_data *data;
    struct cl_node *next;
};

struct client_list {
    pthread_mutex_t cl_lock;
    unsigned char is_empty;
    long int len;
    struct cl_node *head;
};

struct client_list *cl_init(void);
struct cl_node *cl_init_node(struct cl_data *);
struct cl_data *cl_init_data(int, struct message_queue *);
int cl_append(struct client_list *, struct cl_node *);
struct cl_node *cl_get_node(struct client_list *, pthread_t);
int cl_remove(struct client_list *, pthread_t); 
void cl_print(struct client_list *);

#endif
