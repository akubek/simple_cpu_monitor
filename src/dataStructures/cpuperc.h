#ifndef CPUPERC_H
#define CPUPERC_H
#include <time.h>

typedef struct cpuperc
{
    struct timespec t;
    float *cores_perc;
} cpuperc;

#endif
