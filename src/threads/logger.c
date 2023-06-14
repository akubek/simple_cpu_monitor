#include "logger.h"

atomic_bool _logger_run = false;
long _logger_interval = 1000;
struct timespec _logger_last_run = {0, 0};
log_queue *_log_q = NULL;

void _log_current_data(FILE *fp, log_queue *log_q);
bool _logger_guard();

// thread
int logger_thrd(void *)
{
    if (!_logger_guard())
    {
        return 1;
    }
    _logger_run = true;
    FILE *fp = NULL;

    struct timespec start;

    fp = fopen("cpumon.log", "w");
    if (!fp)
    {
        fprintf(stderr, "Logger: could not create file cpumon.log, stopping\n");
        exit(1);
    }

    while (_logger_run)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        _logger_last_run = start;

        _log_current_data(fp, _log_q);

        sleep_for(start, _logger_interval);
    }
    fclose(fp);
    _logger_run = false;
    return 0;
}

void _log_current_data(FILE *fp, log_queue *log_q)
{
    char *msg;

    while (msg = log_dequeue(log_q))
    {
        fprintf(fp, "%s\n", msg);
        free(msg);
    }
}

bool _logger_guard()
{
    if (_logger_run)
    {
        fprintf(stderr, "Logger: cannot start another thread when already running");
        return false;
    }
    else if (_log_q == NULL)
    {
        fprintf(stderr, "Logger: args not set");
        return false;
    }
    return true;
}

void logger_stop()
{
    _logger_run = false;
}

bool logger_running()
{
    return _logger_run;
}

struct timespec logger_check()
{
    return _logger_last_run;
}

bool logger_set_args(log_queue *log_q)
{
    if (_logger_run)
    {
        fprintf(stderr, "Logger: cannot change arguments when already running\n");
        return false;
    }
    _log_q = log_q;
    return true;
}