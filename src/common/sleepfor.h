#ifndef SLEEPFOR_H
#define SLEEPFOR_H
#define _GNU_SOURCE

#include <time.h>

#define NS_IN_SECOND 1000000000

void sleepfor(struct timespec, unsigned long);

#endif