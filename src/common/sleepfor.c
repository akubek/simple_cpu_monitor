#include "sleepfor.h"

void sleepfor(struct timespec start,unsigned long ms) {
    long nsec = start.tv_nsec + ms*1000000;
    time_t sec = start.tv_sec + (nsec/NS_IN_SECOND);
    nsec = nsec%NS_IN_SECOND;

    struct timespec until;
    until.tv_nsec = nsec;
    until.tv_sec = sec;

    clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&until,NULL);
}