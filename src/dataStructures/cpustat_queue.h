#ifndef CPUSTAT_Q_H
#define CPUSTAT_Q_H

#include <stdlib.h>
#include <threads.h>

#include "cpustat.h"

typedef struct cpustat_node cpustat_node;

struct cpustat_node {
    cpustat *cpu_stat;
    cpustat_node *next;
};

typedef struct cpustat_queue {
    mtx_t qmtx;
    int size;
    cpustat_node *front;
    cpustat_node *back;
}cpustat_queue;

void cpustat_init_q(cpustat_queue *q);
int cpustat_enqueue(cpustat_queue *q, cpustat *cpu_stat) ;
cpustat *cpustat_dequeue(cpustat_queue *q);
void cpustat_delete_q(cpustat_queue *q);

#endif