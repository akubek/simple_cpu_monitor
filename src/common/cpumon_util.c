#include "cpumon_util.h"

bool compare_timespec(struct timespec t1, struct timespec t2) {
    return t1.tv_sec > t2.tv_sec || t1.tv_sec == t2.tv_sec && t1.tv_nsec > t2.tv_nsec;
}