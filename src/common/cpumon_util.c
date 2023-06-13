#include "cpumon_util.h"

bool compare_timespec(struct timespec t1, struct timespec t2)
{
    return t1.tv_sec > t2.tv_sec || t1.tv_sec == t2.tv_sec && t1.tv_nsec > t2.tv_nsec;
}

void sleep_for(struct timespec start, unsigned long ms)
{
    long nsec = start.tv_nsec + ms * 1000000;
    time_t sec = start.tv_sec + (nsec / NS_IN_SECOND);
    nsec = nsec % NS_IN_SECOND;

    struct timespec until;
    until.tv_nsec = nsec;
    until.tv_sec = sec;

    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &until, NULL);
}