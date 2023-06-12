#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "../common/sleepfor.h"
#include "../dataStructures/cpustat_queue.h"
#include "../dataStructures/cpustat.h"
#include "../dataStructures/cpuperc_queue.h"
#include "../dataStructures/cpuperc.h"

void logger_stop();

int logger_thrd(void *);

typedef struct logger_args{
    //input
    cpustat_queue * reader_q;
    cpuperc_queue * analyzer_q;
}logger_args;

#endif