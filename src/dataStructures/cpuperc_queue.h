#ifndef CPUPERC_Q_H
#define CPUPERC_Q_H

#include <stdlib.h>
#include <threads.h>

#include "cpuperc.h"

typedef struct cpuperc_node cpuperc_node;

struct cpuperc_node
{
    cpuperc *cpu_perc;
    cpuperc_node *next;
};

typedef struct cpuperc_queue
{
    mtx_t qmtx;
    int size;
    cpuperc_node *front;
    cpuperc_node *back;
} cpuperc_queue;

void cpuperc_init_q(cpuperc_queue *q);
int cpuperc_enqueue(cpuperc_queue *q, cpuperc *cpu_perc);
cpuperc *cpuperc_dequeue(cpuperc_queue *q);
void cpuperc_delete_q(cpuperc_queue *q);

#endif