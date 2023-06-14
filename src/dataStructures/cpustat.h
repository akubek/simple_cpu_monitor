#ifndef CPUSTAT_H
#define CPUSTAT_H
#include <time.h>

typedef struct corestat
{
    int core_num;
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} corestat;

typedef struct cpustat
{
    struct timespec t;
    corestat *cores_stat;
} cpustat;

#endif
