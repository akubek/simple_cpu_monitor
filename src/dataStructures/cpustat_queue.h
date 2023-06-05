#ifndef CPUSTAT_Q_H
#define CPUSTAT_Q_H

#include <stdlib.h>


#include "cpustat.h"

typedef struct cpustat_node cpustat_node;

struct cpustat_node {
    cpustat *cpu_stat;
    cpustat_node *next;
};

typedef struct cpustat_queue {
    cpustat_node *front;
    cpustat_node *back;
}cpustat_queue;

void init_cpustatq(cpustat_queue *q);
int enqueue_cpustatq(cpustat_queue *q, cpustat *cpu_stat) ;
cpustat *dequeue_cpustatq(cpustat_queue *q);
void empty_cpustatq(cpustat_queue *q);

#endif