#ifndef CPUSTAT_H
#define CPUSTAT_H

typedef struct corestat {
    int core_num;
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
}corestat;

typedef struct cpustat {
    int core_count;
    corestat * cores_stat;
}cpustat;

#endif