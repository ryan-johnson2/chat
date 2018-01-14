#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <message_queue.h>
#include <message_handler.h>
#include <server_data.h>


void broadcast(struct client_list *, struct message *);

void *
message_worker(void *targ)
{
    struct server_data *sd; 
    struct client_list *cl; 
    struct message_queue *mq; 
    struct mq_node *node;

    sd = (struct server_data *)targ;
    cl = sd->cl;
    mq = sd->mq;

    while (1) {
        if (!mq->is_empty) {
            puts("Message in the queue");
            node = mq_dequeue(mq); 
            broadcast(cl, node->msg);
            free(node);
            node = NULL;
        }
    }
}


void
broadcast(struct client_list *cl, struct message *msg)
{
    struct cl_node *curr;

    pthread_mutex_lock(&cl->cl_lock);

    curr = cl->head;

    while (curr != NULL) {
        send(curr->data->fd, msg->msg, strlen(msg->msg), 0);
        curr = curr->next;
    }

    pthread_mutex_unlock(&cl->cl_lock);
}
