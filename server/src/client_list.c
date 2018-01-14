#include <stdlib.h>
#include <stdio.h>

#include <client_list.h>


struct client_list *
cl_init(void)
{
    struct client_list *cl;
    int err;

    cl = (struct client_list *)malloc(sizeof(struct client_list));
    if (cl == NULL) {
        fputs("Failed to allocate memory for the client list\n", stderr);
        return NULL;
    }

    err = pthread_mutex_init(&cl->cl_lock, NULL);
    if (err != 0) {
        fputs("Failed to initialize client list lock\n", stderr);
        free(cl);
        return NULL;
    }

    cl->is_empty = 1;
    cl->len = 0;
    cl->head = NULL;

    return cl;
}


struct cl_node *
cl_init_node(struct cl_data *cd) {
    struct cl_node *cln;
    
    cln = (struct cl_node *)malloc(sizeof(struct cl_node));
    if (cln == NULL) {
        fputs("Failed to allocate memory for the client list node\n", stderr);
        return NULL;
    }

    cln->data = cd;
    cln->next = NULL;

    return cln;
}


struct cl_data *cl_init_data(int fd, struct message_queue *mq) {
    struct cl_data *cld;

    cld = (struct cl_data *)malloc(sizeof(struct cl_data));
    if (cld == NULL) {
        fputs("Failed to allocate memory for the client data\n", stderr);
        return NULL;
    }
    
    cld->fd = fd;
    cld->un = NULL;
    cld->mq = mq;

    return cld;
}


int
cl_append(struct client_list *cl, struct cl_node *cln)
{
    struct cl_node *curr;
    int err;

    if (cl->is_empty) {
        err = pthread_mutex_lock(&cl->cl_lock);
        if (err != 0) {
            fputs("Failed to get the client list lock\n", stderr);
            return 1;
        }

        cl->head = cln;
        cl->is_empty = 0;
        cl->len++;
        
        err = pthread_mutex_unlock(&cl->cl_lock);
        if (err != 0) {
            fputs("Failed to unlock the client list lock\n", stderr);
            return 1;
        }
    }

    else {
        err = pthread_mutex_lock(&cl->cl_lock);
        if (err != 0) {
            fputs("Failed to get the client list lock\n", stderr);
            return 1;
        }

        curr = cl->head;

        while (curr->next != NULL) {
            curr = curr->next;
        }
    
        curr->next = cln;
        cl->len++;

        err = pthread_mutex_unlock(&cl->cl_lock);
        if (err != 0) {
            fputs("Failed to unlock the client list lock\n", stderr);
            return 1;
        }
    }
    return 0;
}


struct cl_node *
cl_get_node(struct client_list *cl, pthread_t tid) 
{
    struct cl_node *curr;
    int err;

    if (cl->is_empty) {
        return NULL;
    }

    err = pthread_mutex_lock(&cl->cl_lock);
    if (err != 0) {
        fputs("Failed to get the client list lock\n", stderr);
        return NULL;
    }

    curr = cl->head;

    while (curr != NULL && curr->data->tid != tid) {
        curr = curr->next;
    }

    err = pthread_mutex_unlock(&cl->cl_lock);
    if (err != 0) {
        fputs("Failed to unlock the client list lock\n", stderr);
        return NULL;
    }

    return curr;
}


int
cl_remove(struct client_list *cl, pthread_t tid) 
{
    struct cl_node *curr, *prev;
    int err;

    if (cl->is_empty) {
        return 0;
    }

    err = pthread_mutex_lock(&cl->cl_lock);
    if (err != 0) {
        fputs("Failed to get the client list lock\n", stderr);
        return 1;
    }

    curr = cl->head;
    prev = NULL;

    while (curr != NULL && curr->data->tid != tid) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        return 0;
    }
    else if (prev == NULL) {
        cl->head = curr->next;
        free(curr);

        if (cl->head == NULL) {
            cl->is_empty = 1;
        }
        cl->len--;
    }
    else {
        prev->next = curr->next;
        free(curr);
        cl->len--;
    }

    err = pthread_mutex_unlock(&cl->cl_lock);
    if (err != 0) {
        fputs("Failed to unlock the client list lock\n", stderr);
        return 1;
    }

    return 0;
}


void 
cl_print(struct client_list *cl)
{
    struct cl_node *curr;

    if (cl->is_empty) {
        puts("List()");
    }
    else {
        curr = cl->head;
        printf("List(");
        while (curr->next != NULL) {
            printf("%d, ", curr->data->fd);
            curr = curr->next;
        }
        printf("%d)\n", curr->data->fd);
    }
}
