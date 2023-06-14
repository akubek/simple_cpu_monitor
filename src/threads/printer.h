#ifndef PRINTER_H
#define PRINTER_H
#define _GNU_SOURCE

#include <stdatomic.h>
#include <ncurses.h>
#include <time.h>
#include <sys/sysinfo.h>

#include "../common/cpumon_util.h"
#include "../dataStructures/cpuperc_queue.h"

void printer_stop(void);
bool printer_running(void);

int printer_thrd(void *);
struct timespec printer_check(void);
bool printer_set_args(cpuperc_queue *input_q);

#endif
