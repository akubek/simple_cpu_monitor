#ifndef PRINTER_H
#define PRINTER_H
#define _GNU_SOURCE

#include <stdatomic.h>
#include <ncurses.h>
#include <time.h>
#include <sys/sysinfo.h>

#include "../common/sleepfor.h"
#include "../dataStructures/cpuperc_queue.h"

int printer_thrd(void *);
void printer_stop();
bool printer_running();

typedef struct printer_args{
    cpuperc_queue * input_q;
}printer_args;

#endif