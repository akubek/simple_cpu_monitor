#include "logger.h"

atomic_bool logger_run = false;
long logger_interval = 1000;
struct timespec logger_last_run = { 0, 0 };

void log_current_data(FILE * fp, log_queue * log_q);

int logger_thrd(void *args) {
    logger_run = true;
    logger_args * largs = (logger_args *)args;
    log_queue * log_q = largs->log_q;
    FILE * fp = NULL;

    struct timespec start;

    fp = fopen("cpumon.log","w");
    if(!fp){
        fprintf(stderr, "Logger: could not create file cpumon.log, stopping\n");
        exit(1);
    }

    while(logger_run) {
        clock_gettime(CLOCK_MONOTONIC,&start);
        logger_last_run = start;

        log_current_data(fp, log_q);

        sleepfor(start,logger_interval);
    }
    fclose(fp);

}

void logger_stop() {
    logger_run = false;
}

bool logger_running() {
    return logger_run;
}

void log_current_data(FILE * fp, log_queue * log_q) {
    char * msg;

    while (log_q->size > 0) {
        msg = log_dequeue(log_q);
        fprintf(fp,"%s\n",msg);
        free(msg);
    }
}

struct timespec logger_check() {
    return logger_last_run;
}