#ifndef READER_H
#define READER_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/sysinfo.h>

#include "../common/sleepfor.h"
#include "../dataStructures/cpustat_queue.h"
#include "../dataStructures/cpustat.h"

void stop_reader();
void set_reader_interval(long ms);

int reader_thrd(void *);

typedef struct reader_args{
    cpustat_queue * analyzer_q;
    cpustat_queue * logger_q;
}reader_args;

#endif