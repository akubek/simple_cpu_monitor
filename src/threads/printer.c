#include "printer.h"

long printer_interval = 1000;

int printer_thrd(void *arg) {
    struct timespec start;
    
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
        

        int core_num = 2;
        clear();
        printw("CPU usage:\n");
        printw("cpu: %.2f %%\n", 10.0);
        for(int i = 0; i < core_num; i++) {
            printw("core%d %.2f %%", i, 1.0);
        }
        refresh();

        sleepfor(start,1000);
    }
    endwin();
    return 0;
}