#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <message_queue.h>

struct message_queue *
mq_init(void)
{
    struct message_queue *mq;

    mq = (struct message_queue *)malloc(sizeof(struct message_queue));
    if (mq == NULL) {
        fputs("Failed to allocate memory for the message_queue\n", stderr);
        return mq;
    }
    
    pthread_mutex_init(&mq->mq_lock, NULL);
    mq->is_empty = 1;
    mq->head = NULL;
    mq->tail = NULL;

    return mq;
}


struct mq_node *
mq_init_node(struct message *msg)
{
    struct mq_node *node;

    node = (struct mq_node *)malloc(sizeof(struct mq_node));
    if (node == NULL) {
        fputs("Failed to allocate memory for the message queue node\n", stderr);
        return node;
    }

    node->msg = msg;
    node->next = NULL;

    return node;
}


struct message *
mq_init_msg(pthread_t tid, char *data)
{
    struct message *msg;

    msg = (struct message *)malloc(sizeof(struct message));
    if (msg == NULL) {
        fputs("Failed to allocate memory for the message\n", stderr);
        return msg;
    }

    msg->tid = tid;
    msg->msg = data;

    return msg;
}


int
mq_enqueue(struct message_queue *mq, struct mq_node *node)
{
    if (mq->is_empty) {
        pthread_mutex_lock(&mq->mq_lock);
        mq->head = node;
        mq->tail = node;
        mq->is_empty = 0;
        pthread_mutex_unlock(&mq->mq_lock);
    }
    else {
        pthread_mutex_lock(&mq->mq_lock);
        mq->tail->next = node;
        mq->tail = node;
        pthread_mutex_unlock(&mq->mq_lock);
    }
    return 0;
}


struct mq_node *
mq_dequeue(struct message_queue *mq)
{
    struct mq_node *ret;

    if (mq->is_empty) {
        return NULL;
    }
    else if (mq->head == mq->tail) {
        pthread_mutex_lock(&mq->mq_lock);
        ret = mq->head;
        mq->head = NULL;
        mq->tail = NULL;
        mq->is_empty = 1;
        pthread_mutex_unlock(&mq->mq_lock);
    }
    else {
        pthread_mutex_lock(&mq->mq_lock);
        ret = mq->head;
        mq->head = mq->head->next;
        pthread_mutex_unlock(&mq->mq_lock);
    }

    return ret;
}
