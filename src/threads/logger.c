#include "logger.h"

bool logger_run = false;

int logger_thrd(void *args) {
    FILE * fp;

    logger_run = true;
    while(logger_run) {

    }
}

void logger_stop() {
    logger_run = false;
}