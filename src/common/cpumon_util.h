#ifndef CPUMON_UTIL_H
#define CPUMON_UTIL_H
#define _GNU_SOURCE
#include <time.h>
#include <stdbool.h>

bool compare_timespec(struct timespec t1, struct timespec t2);

#endif