#ifndef ANALYZER_H
#define ANALYZER_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <time.h>
#include <sys/sysinfo.h>

#include "../common/cpumon_util.h"
#include "../dataStructures/cpustat_queue.h"
#include "../dataStructures/cpustat.h"
#include "../dataStructures/cpuperc_queue.h"
#include "../dataStructures/cpuperc.h"
#include "../dataStructures/log_queue.h"

void stop_analyzer();
void analyzer_set_interval(long ms);
bool analyzer_running();

int analyzer_thrd(void *);
struct timespec analyzer_check();
bool analyzer_set_args(cpustat_queue *analyzer_q, cpuperc_queue *printer_q, log_queue *logger_q);

#endif