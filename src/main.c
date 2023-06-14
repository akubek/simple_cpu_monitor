#include "main.h"

static thrd_t analyzer, printer, reader, logger, watchdog;
static cpustat_queue analyzer_q;
static cpuperc_queue printer_q;
static log_queue log_q;

static void init_threads()
{
    cpustat_init_q(&analyzer_q);
    cpuperc_init_q(&printer_q);
    log_init_q(&log_q);

    reader_set_args(&analyzer_q, &log_q);
    analyzer_set_args(&analyzer_q, &printer_q, &log_q);
    printer_set_args(&printer_q);
    logger_set_args(&log_q);
    watchdog_set_args(&log_q);
}

static void end_threads()
{
    if (printer_running())
    {
        printer_stop();
        thrd_join(printer, NULL);
    }
    watchdog_stop();
    thrd_join(watchdog, NULL);
    reader_stop();
    thrd_join(reader, NULL);
    stop_analyzer();
    thrd_join(analyzer, NULL);
    logger_stop();
    thrd_join(logger, NULL);

    cpustat_delete_q(&analyzer_q);
    cpuperc_delete_q(&printer_q);
    log_delete_q(&log_q);
}

static void run_threads()
{
    thrd_create(&reader, reader_thrd, NULL);
    thrd_create(&analyzer, analyzer_thrd, NULL);
    thrd_create(&printer, printer_thrd, NULL);
    thrd_create(&logger, logger_thrd, NULL);
    sleep(1); // wait some time before starting watchdog
    thrd_create(&watchdog, watchdog_thrd, NULL);

    thrd_join(printer, NULL);
    end_threads();
}

__attribute__((noreturn)) void terminate(int signum);
void terminate(int signum)
{
    fprintf(stderr, "Sigterm %d caught, ending threads...\n", signum);
    end_threads();
    fprintf(stderr, "Threads ended, exiting.\n");
    exit(0);
}

int main()
{
    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_handler = terminate;
    sigaction(SIGTERM, &act, NULL);

    init_threads();
    run_threads();

    return 0;
}
