#include "printer.h"

long printer_interval = 1000;

int printer_thrd(void *arg) {
    struct timespec start;
    printer_args * args = (printer_args *)arg;
    cpuperc_queue * input_q = args->input_q;
    cpuperc * newest;
    const int core_count = get_nprocs();
    
    //ncurses setup
    initscr();
    raw();
    noecho();
    nodelay(stdscr,true);
    int input = 0;

    //primary loop
    while(input != 'q') {
        clock_gettime(CLOCK_MONOTONIC,&start);
        input=getch();
        clear();

        if(input_q->size == 0) {
            printw("No CPU usage data");
        } else {
            while(input_q->size > 1) {
                newest = cpuperc_dequeue(input_q);
                free(newest->cores_perc);
                free(newest);
            }
            newest = cpuperc_dequeue(input_q);
            printw("CPU usage:\n");
            printw("cpu: %.2f %%\n", newest->cores_perc[0]);
            for(int i = 1; i <= core_count; i++) {
                printw("core%d %.2f %%\n", i, newest->cores_perc[i]);
            }
            free(newest->cores_perc);
            free(newest);
        }
        refresh();

        sleepfor(start,1000);
    }
    endwin();
    return 0;
}