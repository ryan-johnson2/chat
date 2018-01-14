#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <pthread.h>

struct message {
    pthread_t tid;
    char *msg;
};

struct mq_node {
    struct message *msg;
    struct mq_node *next;
};

struct message_queue {
    pthread_mutex_t mq_lock;
    unsigned char is_empty;
    struct mq_node *head;
    struct mq_node *tail;
};

struct message_queue *mq_init(void);
struct mq_node *mq_init_node(struct message *);
struct message *mq_init_msg(pthread_t, char *);
int mq_enqueue(struct message_queue *, struct mq_node *);
struct mq_node *mq_dequeue(struct message_queue *);

#endif
