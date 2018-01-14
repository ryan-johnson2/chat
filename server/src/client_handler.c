#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <client_handler.h>
#include <client_list.h>

#define BUFFSIZE 512

void *
client_worker(void *targ) 
{
    struct cl_data *cd;
    char *buff;
    struct message *msg;
    struct mq_node *mqn;
    size_t err;

    cd = (struct cl_data *)targ;
   
    while (1) {
        
        buff = (char *)malloc(BUFFSIZE);

        err = recv(cd->fd, buff, BUFFSIZE - 1, 0);
        if (err < 1) {
            pthread_exit((void *)1);
        }
        else if (err == 0) {
            pthread_exit((void *)1);
        }

        fputs("Got a message\n", stderr);
        msg = mq_init_msg(cd->tid, buff);
        mqn = mq_init_node(msg);

        err = mq_enqueue(cd->mq, mqn);
        if (err != 0) {
            pthread_exit((void *)1);
        }

        /*
        err = send(cd->fd, buff, strlen(buff), 0);
        if (err < 0) {
            pthread_exit((void *)1);
        }
        */
    }
}

