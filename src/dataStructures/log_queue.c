#include "log_queue.h"

void log_init_q(log_queue *q)
{
    mtx_init(&q->qmtx, mtx_plain);
    q->size = 0;
    q->front = NULL;
    q->back = NULL;
}

int log_enqueue(log_queue *q, char *log_msg, struct timespec t)
{
    if (mtx_lock(&q->qmtx) == thrd_error)
    {
        exit(1);
    }
    log_node *search_node, *p = NULL;
    log_node *new_node = malloc(sizeof *new_node);
    if (new_node == NULL)
    {
        return 1;
    }
    new_node->log_msg = log_msg;
    new_node->t = t;
    new_node->next = NULL;
    new_node->prev = NULL;
    if (q->front == NULL)
    {
        q->front = q->back = new_node;
    }
    else
    {
        // search based on time of msg creation
        search_node = q->back;
        while (search_node != NULL && compare_timespec(search_node->t, new_node->t))
        {
            p = search_node;
            search_node = search_node->prev;
        }
        // insert new node
        if (search_node != NULL)
        {
            search_node->next = new_node;
        }
        new_node->prev = search_node;
        new_node->next = p;
        if (p != NULL)
        {
            p->prev = new_node;
        }
        else
        { // p is null if search node was the last node
            q->back = new_node;
        }
    }
    q->size++;
    mtx_unlock(&q->qmtx);
    return 0;
}
char *log_dequeue(log_queue *q)
{
    if (mtx_lock(&q->qmtx) == thrd_error)
    {
        exit(1);
    }
    if (q->front == NULL)
    {
        q->size = 0;
        mtx_unlock(&q->qmtx);
        return NULL;
    }
    log_node *old_front = q->front;
    q->front = q->front->next;
    if (q->front != NULL)
    {
        q->front->prev = NULL;
    }
    q->size--;
    if (q->size == 0)
    {
        q->back = NULL;
    }

    char *log_msg = old_front->log_msg;
    free(old_front);
    mtx_unlock(&q->qmtx);
    return log_msg;
}

void log_delete_q(log_queue *q)
{
    char *elem;
    while ((elem = log_dequeue(q)) && elem != NULL)
    {
        free(elem);
    }
    if (mtx_lock(&q->qmtx) == thrd_error)
    {
        exit(1);
    }
    mtx_unlock(&q->qmtx);
    mtx_destroy(&q->qmtx);
}