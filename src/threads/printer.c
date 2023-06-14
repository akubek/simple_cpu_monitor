#include "printer.h"

atomic_bool _printer_run = false;
long _printer_interval = 1000;
struct timespec _printer_last_run = {0, 0};
cpuperc_queue *_printer_input_q = NULL;

bool _printer_guard();

// thread
int printer_thrd(void *arg)
{
    if (!_printer_guard())
    {
        return 1;
    }
    _printer_run = true;
    struct timespec start;
    cpuperc *newest;
    const int core_count = get_nprocs();

    // ncurses setup
    initscr();
    raw();
    noecho();
    nodelay(stdscr, true);
    int input = 0;

    // primary loop
    while (input != 'q' && _printer_run)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        _printer_last_run = start;
        input = getch();
        clear();

        if (_printer_input_q->size == 0)
        {
            printw("No CPU usage data");
        }
        else
        {
            while (_printer_input_q->size > 1)
            {
                newest = cpuperc_dequeue(_printer_input_q);
                free(newest->cores_perc);
                free(newest);
            }
            newest = cpuperc_dequeue(_printer_input_q);
            printw("CPU usage monitor [q to quit]:\n");
            printw("cpu: %.2f %%\n", newest->cores_perc[0]);
            for (int i = 1; i <= core_count; i++)
            {
                printw("core%d %.2f %%\n", i, newest->cores_perc[i]);
            }
            free(newest->cores_perc);
            free(newest);
        }
        refresh();

        sleep_for(start, _printer_interval);
    }
    endwin();
    _printer_run = false;
    return 0;
}

void printer_stop()
{
    _printer_run = false;
}

bool printer_running()
{
    return _printer_run;
}

struct timespec printer_check()
{
    return _printer_last_run;
}

bool printer_set_args(cpuperc_queue *input_q)
{
    if (_printer_run)
    {
        fprintf(stderr, "Printer: cannot change arguments when already running\n");
        return false;
    }
    _printer_input_q = input_q;
    return true;
}

bool _printer_guard()
{
    if (_printer_run)
    {
        fprintf(stderr, "Printer: cannot start another thread when already running");
        return false;
    }
    else if (_printer_input_q == NULL)
    {
        fprintf(stderr, "Printer: args not set");
        return false;
    }
    return true;
}