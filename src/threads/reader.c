#include "reader.h"

bool run_reader = false;
long reader_interval = 200;

int reader_thrd(void *arg) {
    FILE * fp;
    char * line;
    size_t len = 0;
    ssize_t read;
    const long core_count = get_nprocs();
    struct timespec start;

    corestat * core_stat;
    cpustat * cpu_stat;
    reader_args * args = (reader_args *)arg;
    cpustat_queue * analyzer_q = args->analyzer_q;
    cpustat_queue * logger_q = args->logger_q;

    run_reader = true;
    while(run_reader) {
        clock_gettime(CLOCK_MONOTONIC,&start);
        fp = fopen("/proc/stat","r");
        if(!fp){
            fprintf(stderr, "Reader: could not open file /proc/stat, stopping\n");
            exit(1);
        }

        cpu_stat = malloc(sizeof *cpu_stat);
        cpu_stat->t = start;
        if(cpu_stat == NULL) {
            fprintf(stderr, "Reader: could not allocate memory for cpu stat\n");
            exit(1);
        }
        cpu_stat->cores_stat = calloc(core_count+1,sizeof *core_stat);
        if(cpu_stat->cores_stat == NULL) {
            fprintf(stderr, "Reader: could not allocate memory for cores stat\n");
            exit(1);
        }
        
        //read lines describing cpu usage from file to
        for (int i = 0; i <= core_count; i++) {
            core_stat = &(cpu_stat->cores_stat[i]);
            core_stat->core_num = i-1;
            if (read = getline(&line,&len,fp) == -1) {
                fprintf(stderr,"Reader: error reading line from file or unexpected EOF");
                exit(1);
            }
            sscanf(line,"cpu %llu %llu %llu %llu %llu %llu %llu %llu",
            &(core_stat->user), &(core_stat->nice), &(core_stat->system), &(core_stat->idle), &(core_stat->iowait), &(core_stat->irq), &(core_stat->softirq), &(core_stat->steal));
        }
        //queue data to anylyzer
        if(enqueue_cpustatq(analyzer_q,cpu_stat)) {
            fprintf(stderr, "Reader: could not add element to q\n");
            exit(1);
        }
        //TODO logger

        fclose(fp);
        sleepfor(start,reader_interval);
    }
    
    return 0;
}

void stop_reader() {
    run_reader = false;
}

void set_reader_interval(long ms) {
    //dont change interval if reader is running
    if(!run_reader) {
        reader_interval = ms;
    }
}