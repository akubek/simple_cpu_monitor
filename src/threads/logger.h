#ifndef LOGGER_H
#define LOGGER_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "../common/sleepfor.h"
#include "../dataStructures/cpustat_queue.h"
#include "../dataStructures/cpustat.h"
#include "../dataStructures/cpuperc_queue.h"
#include "../dataStructures/cpuperc.h"
#include "../dataStructures/log_queue.h"

void logger_stop();

int logger_thrd(void *);

typedef struct logger_args{
    //input
    log_queue * log_q;
}logger_args;

#endif