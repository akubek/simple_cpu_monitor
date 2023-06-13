#include "main.h"

//TODO cleanup / renaming / tests

thrd_t analyzer,printer,reader,logger,watchdog;
cpustat_queue analyzer_q;
cpuperc_queue printer_q;
log_queue log_q;

void end_threads () {
    if (printer_running()) {
        printer_stop();
        thrd_join(printer,NULL);
    }
    watchdog_stop();
    thrd_join(watchdog,NULL);
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
    fprintf(stderr,"Sigterm caught, ending threads...\n");
    end_threads();
    fprintf(stderr,"Threads ended, exiting.\n");
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

    watchdog_set_args(&log_q);

    thrd_create(&reader,reader_thrd,(void *)(&rargs));
    thrd_create(&analyzer,analyzer_thrd,(void *)&aargs);
    thrd_create(&printer,printer_thrd,(void *)&pargs);
    thrd_create(&logger,logger_thrd,(void *)&largs);
    sleep(1); //wait some time before starting watchdog
    thrd_create(&watchdog,watchdog_thrd,NULL);
    
    thrd_join(printer,NULL);
    end_threads();

    return 0;
}