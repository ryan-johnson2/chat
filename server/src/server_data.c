#include <server_data.h>
#include <stdlib.h>

struct server_data *
sd_init(int lport, struct client_list *cl, struct message_queue *mq)
{
    struct server_data *sd;

    sd = (struct server_data *)malloc(sizeof(struct server_data));
    if (sd == NULL) {
        return NULL;
    }

    sd->lport = lport;
    sd->cl = cl;
    sd->mq = mq;

    return sd;
}
