#include "main.h"

//TODO cleanup / renaming

thrd_t analyzer,printer,reader,logger;
cpustat_queue analyzer_q;
cpuperc_queue printer_q;
log_queue log_q;

void end_threads () {
    if (printer_running()) {
        printer_stop();
        thrd_join(printer,NULL);
    }
    stop_reader();
    thrd_join(reader,NULL);
    stop_analyzer();
    thrd_join(analyzer,NULL);
    logger_stop();
    thrd_join(logger,NULL);

    cpustat_delete_q(&analyzer_q);
    cpuperc_delete_q(&printer_q);
    log_delete_q(&log_q);

}

void terminate (int signum) {
    printf("Signal caught, exiting...\n");
    end_threads();
    printf("Threads ended.\n");
    exit(0);
}

int main() {
    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_handler = terminate;
    sigaction(SIGTERM, &act, NULL);
    
    cpustat_init_q(&analyzer_q);
    cpuperc_init_q(&printer_q);
    log_init_q(&log_q);
    reader_args rargs = {&analyzer_q,&log_q};
    analyzer_args aargs = {&analyzer_q,&printer_q,&log_q};
    printer_args pargs = {&printer_q};
    logger_args largs = {&log_q};

    thrd_create(&reader,reader_thrd,(void *)(&rargs));
    thrd_create(&analyzer,analyzer_thrd,(void *)&aargs);
    thrd_create(&printer,printer_thrd,(void *)&pargs);
    thrd_create(&logger,logger_thrd,(void *)&largs);
    
    thrd_join(printer,NULL);
    end_threads();

    return 0;
}