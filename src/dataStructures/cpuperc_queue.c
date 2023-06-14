#include "cpuperc_queue.h"

void cpuperc_init_q(struct cpuperc_queue *q)
{
    mtx_init(&q->qmtx, mtx_plain);
    q->size = 0;
    q->front = NULL;
    q->back = NULL;
}

int cpuperc_enqueue(cpuperc_queue *q, cpuperc *cpu_perc)
{
    if (mtx_lock(&q->qmtx) == thrd_error)
    {
        exit(1);
    }
    cpuperc_node *new_back = malloc(sizeof *new_back);
    if (new_back == NULL)
    {
        return 1;
    }
    new_back->cpu_perc = cpu_perc;
    new_back->next = NULL;

    if (q->front == NULL)
    {
        q->front = q->back = new_back;
    }
    else
    {
        q->back->next = new_back;
        q->back = new_back;
    }
    q->size++;
    mtx_unlock(&q->qmtx);

    return 0;
}

cpuperc *cpuperc_dequeue(cpuperc_queue *q)
{ 
    if (mtx_lock(&q->qmtx) == thrd_error)
    {
        exit(1);
    }
    if (q->size == 0)
    {
        mtx_unlock(&q->qmtx);
        return NULL;
    }
    cpuperc_node *old_front = q->front;
    q->front = q->front->next;
    q->size--;
    if (q->size == 0)
    {
        q->back = NULL;
    }

    cpuperc *cpu_perc = old_front->cpu_perc;
    free(old_front);
    mtx_unlock(&q->qmtx);
    return cpu_perc;
}

void cpuperc_delete_q(cpuperc_queue *q)
{
    cpuperc *elem;
    while (elem = cpuperc_dequeue(q))
    {
        free(elem->cores_perc);
        free(elem);
    }
    if (mtx_lock(&q->qmtx) == thrd_error)
    {
        exit(1);
    }
    mtx_unlock(&q->qmtx);
    mtx_destroy(&q->qmtx);
}