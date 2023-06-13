#include "reader.h"

atomic_bool run_reader = false;
long reader_interval = 200;
struct timespec reader_last_run = { 0, 0 };

//MAIN FUNCTION
int reader_thrd(void *arg) {
    run_reader = true;
    FILE * fp = NULL;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    const int core_count = get_nprocs();
    struct timespec start;

    corestat * core_stat;
    cpustat * cpu_stat;
    reader_args * args = (reader_args *)arg;
    cpustat_queue * analyzer_q = args->analyzer_q;
    log_queue * logger_q = args->logger_q;
    char * log_msg;
    int max_len = 0;

    while(run_reader) {
        clock_gettime(CLOCK_MONOTONIC,&start);
        reader_last_run = start;
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
        //logger msg beginning
        log_msg = calloc(LOG_MSG_LEN, sizeof * log_msg);
        if(log_msg == NULL) {
            fprintf(stderr, "Reader: could not allocate memory for log msg\n");
            exit(1);
        }
        snprintf(log_msg,LOG_MSG_LEN,"READER  [%ld:%03ld]:", start.tv_sec, start.tv_nsec/1000000);

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

            //logger message append contents
            if ( i == 0) { // for less clutter in log file only cpu and no core stats
                max_len = LOG_MSG_LEN - strlen(log_msg);
                strncat(log_msg,line,max_len);
                strtok(log_msg, "\n");
            }
        }
        //queue msg to logger
        if(log_enqueue(logger_q,log_msg,start)) { 
            fprintf(stderr, "Reader: could not add element to log q\n");
            exit(1);
        }

        //queue data to anylyzer
        if(cpustat_enqueue(analyzer_q,cpu_stat)) {
            fprintf(stderr, "Reader: could not add element to q\n");
            exit(1);
        }
        
        fclose(fp);
        sleepfor(start,reader_interval);
    }
    free(line);
    return 0;
}

void stop_reader() {
    run_reader = false;
}

bool reader_running() {
    return run_reader;
}

void set_reader_interval(long ms) {
    //dont change interval if reader is running
    if(!run_reader) {
        reader_interval = ms;
    }
}

struct timespec reader_check() {
    return reader_last_run;
}