#ifndef READER_H
#define READER_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <time.h>
#include <string.h>
#include <sys/sysinfo.h>

#include "../common/cpumon_util.h"
#include "../dataStructures/cpustat_queue.h"
#include "../dataStructures/cpustat.h"
#include "../dataStructures/log_queue.h"

void reader_stop();
void reader_set_interval(long ms);
bool reader_running();

int reader_thrd(void *);
struct timespec reader_check();
bool reader_set_args(cpustat_queue *analyzer_q, log_queue *logger_q);

#endif