#ifndef CPUPERC_Q_H
#define CPUPERC_Q_H

#include <stdlib.h>

#include "cpuperc.h"

typedef struct cpuperc_node cpuperc_node;

struct cpuperc_node {
    cpuperc *cpu_perc;
    cpuperc_node *next;
};

typedef struct cpuperc_queue {
    int size;
    cpuperc_node *front;
    cpuperc_node *back;
}cpuperc_queue;

void init_cpupercq(cpuperc_queue *q);
int enqueue_cpupercq(cpuperc_queue *q, cpuperc * cpu_perc) ;
cpuperc * dequeue_cpupercq(cpuperc_queue *q);
void empty_cpuspercq(cpuperc_queue *q);

#endif