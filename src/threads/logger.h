#ifndef LOGGER_H
#define LOGGER_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <time.h>

#include "../common/cpumon_util.h"
#include "../dataStructures/cpustat_queue.h"
#include "../dataStructures/cpustat.h"
#include "../dataStructures/cpuperc_queue.h"
#include "../dataStructures/cpuperc.h"
#include "../dataStructures/log_queue.h"

void logger_stop();
bool logger_running();

int logger_thrd(void *);
struct timespec logger_check();
bool logger_set_args(log_queue *log_q);

#endif