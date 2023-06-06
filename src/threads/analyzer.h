#ifndef ANALYZER_H
#define ANALYZER_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/sysinfo.h>

#include "../common/sleepfor.h"
#include "../dataStructures/cpustat_queue.h"
#include "../dataStructures/cpustat.h"
#include "../dataStructures/cpuperc_queue.h"
#include "../dataStructures/cpuperc.h"

void stop_analyzer();
void set_analyzer_interval(long ms);

int analyzer_thrd(void *);

typedef struct analyzer_args{
    //input
    cpustat_queue * analyzer_q;
    //output
    cpuperc_queue * printer_q;
    cpuperc_queue * logger_q;
}analyzer_args;

#endif