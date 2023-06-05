#include "cpustat_queue.h"

void init_cpustatq(struct cpustat_queue * q) {
    q->front = NULL;
    q->back = NULL;
}

int enqueue_cpustatq(cpustat_queue *q,cpustat *cpu_stat) {
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
    
    return 0;
}

cpustat *dequeue_cpustatq(cpustat_queue *q) {
    cpustat_node * old_front = q->front;
    q->front = q->front->next;
    
    cpustat *cpu_stat = old_front->cpu_stat;
    free(old_front);
    return cpu_stat;
}

void empty_cpustatq(cpustat_queue *q) {
    cpustat * elem;
    while(q->front != NULL) {
        elem = dequeue_cpustatq(q);
        free(elem);
    }
}