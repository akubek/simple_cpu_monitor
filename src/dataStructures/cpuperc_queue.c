#include "cpuperc_queue.h"

void init_cpupercq(struct cpuperc_queue * q) {
    q->size = 0;
    q->front = NULL;
    q->back = NULL;
}

int enqueue_cpupercq(cpuperc_queue *q,cpuperc * cpu_perc) {
    cpuperc_node *new_back = malloc(sizeof *new_back);
    if(new_back == NULL) {
        return 1;
    }
    new_back->cpu_perc = cpu_perc;
    new_back->next = NULL;

    if(q->front == NULL) {
        q->front = q->back = new_back;
    } else {
        q->back->next = new_back;
        q->back = new_back;
    }
    q->size++;
    
    return 0;
}

cpuperc *dequeue_cpupercq(cpuperc_queue *q) {
    cpuperc_node * old_front = q->front;
    q->front = q->front->next;
    q->size--;
    
    cpuperc *cpu_perc = old_front->cpu_perc;
    free(old_front);
    return cpu_perc;
}

void empty_cpupercq(cpuperc_queue *q) {
    cpuperc * elem;
    while(q->front != NULL) {
        elem = dequeue_cpustatq(q);
        free(elem->cores_perc);
        free(elem);
    }
}