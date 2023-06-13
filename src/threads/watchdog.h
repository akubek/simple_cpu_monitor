#ifndef WATCHDOG_H
#define WATCHDOG_H
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdatomic.h>
#include <time.h>
#include <threads.h>
#include <ncurses.h>

#include "../dataStructures/log_queue.h"
#include "../common/cpumon_util.h"
#include "reader.h"
#include "analyzer.h"
#include "printer.h"
#include "logger.h"

#define WATCHDOG_INTERVAL_MIN 200
#define WATCHDOG_INTERVAL_MAX 1000
#define _RESP(x) (x ? "responsive" : "unresponsive")

void watchdog_stop();
bool watchdog_set_interval(unsigned long ms);

int watchdog_thrd(void *);
bool watchdog_set_args(log_queue * log_q);

#endif