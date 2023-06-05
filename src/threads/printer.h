#ifndef PRINTER_H
#define PRINTER_H
#define _GNU_SOURCE

#include <threads.h>
#include <ncurses.h>
#include <time.h>

#include "../common/sleepfor.h"

int printer_thrd(void *);

#endif