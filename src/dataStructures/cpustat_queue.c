#include "cpustat_queue.h"

void cpustat_init_q(struct cpustat_queue * q) {
    mtx_init(&q->qmtx,mtx_plain);
    q->size = 0;
    q->front = NULL;
    q->back = NULL;
}

int cpustat_enqueue(cpustat_queue *q,cpustat *cpu_stat) {
    if(mtx_lock(&q->qmtx)==thrd_error) {
        exit(1);
    }
    cpustat_node *new_back = malloc(sizeof *new_back);
    if(new_back == NULL) {
        return 1;
    }
    new_back->cpu_stat = cpu_stat;
    new_back->next = NULL;

    if(q->front == NULL) {
        q->front = q->back = new_back;
    } else {
        q->back->next = new_back;
        q->back = new_back;
    }
    q->size++;
    mtx_unlock(&q->qmtx);
    return 0;
}

cpustat *cpustat_dequeue(cpustat_queue *q) {
    if(mtx_lock(&q->qmtx)==thrd_error) {
        exit(1);
    }
    cpustat_node * old_front = q->front;
    q->front = q->front->next;
    q->size--;
    
    cpustat *cpu_stat = old_front->cpu_stat;
    free(old_front);
    mtx_unlock(&q->qmtx);
    return cpu_stat;
}

void cpustat_delete_q(cpustat_queue *q) {
    cpustat * elem;
    while(q->front != NULL) {
        elem = cpustat_dequeue(q);
        free(elem->cores_stat);
        free(elem);
    }
    if(mtx_lock(&q->qmtx)==thrd_error) {
        exit(1);
    }
    mtx_unlock(&q->qmtx);
    mtx_destroy(&q->qmtx);
}