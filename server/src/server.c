#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <server_data.h>
#include <client_list.h>
#include <client_handler.h>
#include <message_queue.h>
#include <message_handler.h>
#include <listener.h>


int
main(int argc, char *argv[])
{
    int lport;
    struct client_list *cl;
    struct message_queue *mq;
    struct server_data *sd;
    pthread_t lstnr_t;
    pthread_t msg_t;

    // Check for an argument
    if (argc != 2) {
        printf("Usage: %s <PORT>\n", argv[0]);
        exit(1);
    }

    // Convert the port argument to an int and validate it
    char *tmp = NULL;
    lport = strtol(argv[1], &tmp, 10);
    if (*tmp != 0) {
        fprintf(stderr, "Failed to convert port: %s\n", argv[1]);
        exit(1);
    }
    else if (lport < 1 || lport > 65535) {
        fputs("Port must be between 0 and 65536\n", stderr);
        exit(1);
    }

    // Initialize shared data structures
    cl = cl_init();
    cl_print(cl);
    mq = mq_init();

    // create server data to use as thread arg
    sd = sd_init(lport, cl, mq);

    // Spawn threads 
    pthread_create(&lstnr_t, NULL, listener_worker, (void *)sd);
    pthread_create(&msg_t, NULL, message_worker, (void *)sd);

    pthread_exit(NULL);
    exit(0);
}

