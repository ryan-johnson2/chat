// System includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// My includes
#include <listener.h>
#include <server_data.h>
#include <client_list.h> 
#include <client_handler.h>


void *
listener_worker(void *targ) 
{
    int serv_fd, client_fd, err;
    struct sockaddr_in serv_addr;
    struct sockaddr_in *client_addr;
    socklen_t client_addr_size;
    struct server_data *sd;
    struct cl_data *cld;
    struct cl_node *cln;

    sd = (struct server_data *)targ;

    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_fd < 0) {
        fputs("Failed to open server socket\n", stderr);
        pthread_exit((void *)1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(sd->lport);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    err = bind(serv_fd, (struct sockaddr *)&serv_addr, (socklen_t)sizeof(serv_addr));
    if (err < 0) {
        fputs("Failed to bind server socket\n", stderr);
        close(serv_fd);
        pthread_exit((void *)1);
    }

    err = listen(serv_fd, 16);
    if (err < 0) {
        fputs("Failed to listen on server socket\n", stderr);
        close(serv_fd);
        pthread_exit((void *)1);
    }

    while (1) {

        client_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
        if (client_addr == NULL) {
            fputs("Failed to allocate memory for the client address\n", stderr);
            close(serv_fd);
            pthread_exit((void *)1);
        }

        client_addr_size = sizeof(struct sockaddr_in);

        client_fd = accept(serv_fd, (struct sockaddr *)client_addr, &client_addr_size);
        
        cld = cl_init_data(client_fd, sd->mq);
        cln = cl_init_node(cld);
        cl_append(sd->cl, cln);
        cl_print(sd->cl);
        fprintf(stderr, "New connection spawning a new client thread\n");
        pthread_create(&cld->tid, NULL, client_worker, (void *)cld);
    }
}
        





