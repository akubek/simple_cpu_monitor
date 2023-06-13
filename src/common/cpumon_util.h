#ifndef CPUMON_UTIL_H
#define CPUMON_UTIL_H
#define _GNU_SOURCE
#include <time.h>
#include <stdbool.h>

#define NS_IN_SECOND 1000000000

bool compare_timespec(struct timespec t1, struct timespec t2);

void sleep_for(struct timespec, unsigned long);

#endif