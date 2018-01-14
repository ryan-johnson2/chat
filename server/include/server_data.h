#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <client_list.h>
#include <message_queue.h>

struct server_data {
    int lport;
    struct client_list *cl;
    struct message_queue *mq;
};

struct server_data *sd_init(int, struct client_list *, struct message_queue *);

#endif
