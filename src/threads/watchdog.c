#include "watchdog.h"

atomic_bool _watchdog_run = false;
unsigned long _watchdog_interval = 500; // ms
log_queue *_watchdog_logger_q = NULL;
bool _analyzer_ok, _reader_ok, _printer_ok, _logger_ok;

bool watchdog_set_args(log_queue *log_q)
{
    if (_watchdog_run)
    {
        fprintf(stderr, "Watchdog: cannot change log_q when already running\n");
        return false;
    }
    _watchdog_logger_q = log_q;
    return true;
}

bool _watchdog_threads_check(struct timespec current_t);
bool _watchdog_guard();

// thread
int watchdog_thrd(void *arg)
{
    if (!_watchdog_guard())
    {
        return 1;
    }
    _watchdog_run = true;
    struct timespec start;
    char *log_msg = calloc(LOG_MSG_LEN, sizeof *log_msg);

    while (_watchdog_run)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        if (!_watchdog_threads_check(start))
        {
            if (_logger_ok)
            {
                snprintf(log_msg, LOG_MSG_LEN,
                         "WATCHDOG[%ld:%03ld]: unresponsive thread detected, state: analyzer:%s, reader:%s, printer:%s, logger:responsive",
                         start.tv_sec, start.tv_nsec / 1000000, W_RESP(_analyzer_ok), W_RESP(_reader_ok), W_RESP(_printer_ok));
                log_enqueue(_watchdog_logger_q, log_msg, start);
                sleep_for(start, 1500); // wait for logger to save msg
            }
            endwin();
            fprintf(stderr, "Watchdog: ERROR unresponsive thread detected, state: analyzer:%s, reader:%s, printer:%s, logger:%s\n",
                    W_RESP(_analyzer_ok), W_RESP(_reader_ok), W_RESP(_printer_ok), W_RESP(_logger_ok));
            exit(1);
        }
        sleep_for(start, _watchdog_interval);
    }

    free(log_msg);
    _watchdog_run = false;
    return 0;
}

bool _watchdog_guard()
{
    if (_watchdog_run)
    {
        fprintf(stderr, "Watchdog: cannot start another thread when already running");
        return false;
    }
    else if (_watchdog_logger_q == NULL)
    {
        fprintf(stderr, "Watchdog: log_q not set");
        return false;
    }
    return true;
}

bool _watchdog_threads_check(struct timespec current_t)
{
    if (!(analyzer_running() && reader_running() && printer_running() && logger_running()))
    {
        log_enqueue(_watchdog_logger_q, "waiting for threads to start", current_t);
        return true; // dont check threads when they are not all running
    }
    struct timespec sec2 = current_t;
    sec2.tv_sec = sec2.tv_sec - 2;
    _analyzer_ok = compare_timespec(analyzer_check(), sec2);
    _reader_ok = compare_timespec(reader_check(), sec2);
    _printer_ok = compare_timespec(printer_check(), sec2);
    _logger_ok = compare_timespec(logger_check(), sec2);

    return _analyzer_ok && _reader_ok && _printer_ok && _logger_ok;
}

void watchdog_stop()
{
    _watchdog_run = false;
}

bool watchdog_set_interval(unsigned long ms)
{
    if (!_watchdog_run)
    {
        fprintf(stderr, "Watchdog: cannot change interval when already running");
        return false;
    }
    else if (WATCHDOG_INTERVAL_MIN > ms || ms > WATCHDOG_INTERVAL_MAX)
    {
        fprintf(stderr, "Watchdog: invalid interval value");
        return false;
    }
    _watchdog_interval = ms;
    return true;
}