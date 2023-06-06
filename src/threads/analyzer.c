#include "analyzer.h"

bool run_analyzer = false;
long analyzer_interval = 500;

int is_older_than(cpustat * data,int seconds) {
    //old if is older than 2 sec
    struct timespec comp;
    struct timespec elem= data->t;
    clock_gettime(CLOCK_MONOTONIC,&comp);
    comp.tv_sec -= seconds;
    return (elem.tv_sec < comp.tv_sec 
    || elem.tv_sec == comp.tv_sec && elem.tv_nsec < comp.tv_nsec);  
}

void load_datapoints(cpustat_queue * q, cpustat ** newest, cpustat ** previous) {
    //tries to load previous from up to 1s ago
    //does not dequeue data
    cpustat_node * node = q->front;
    *newest = q->back->cpu_stat;
    do {
        *previous = node->cpu_stat;
        if(!is_older_than(node->cpu_stat,1)) {
            break;
        }
        node = node->next;
    } while(node->next != NULL);
}

unsigned long long core_total(corestat stat) {
    return stat.user + stat.nice + stat.system + stat.idle + stat.iowait + stat.irq + stat.softirq + stat.steal;
}

unsigned long long core_idle(corestat stat) {
    return stat.idle + stat.iowait;
}

void calc_usage_perc(cpustat_queue * newest_data, int core_count,float * usage) {
    unsigned long long total_delta, idle_delta;
    cpustat * stat_curr, * stat_prev;
    //get 2 datapoints
    load_datapoints(newest_data,&stat_curr,&stat_prev);

    //calculate usage percentages
    for (int i = 0; i <= core_count; i++) {
        total_delta = core_total(stat_curr->cores_stat[i]) - core_total(stat_prev->cores_stat[i]);
        idle_delta = core_idle(stat_curr->cores_stat[i]) - core_idle(stat_prev->cores_stat[i]);
        
        usage[i] = ((total_delta - idle_delta)*100.0)/total_delta;
    }
}

void update_data(cpustat_queue * newest_data, cpustat_queue * q) {
    cpustat * elem;
    while (q->front != NULL)
    {   
        elem = dequeue_cpustatq(q); 
        enqueue_cpustatq(newest_data,elem);
    }
    //remove old datapoints
    while (newest_data->front != NULL && is_older_than(newest_data->front->cpu_stat,2)) {
        elem = dequeue_cpustatq(newest_data);
        free(elem->cores_stat);
        free(elem);
    }
}

//MAIN FUNCTION
int analyzer_thrd(void *arg) {
    analyzer_args * args = (analyzer_args *)arg;
    cpustat_queue * input_q = args->analyzer_q;
    cpuperc_queue * printer_q = args->printer_q;
    cpuperc_queue * logger_q = args->logger_q;

    cpustat_queue newest_data; //for storing latest cpu usage data points for about 2s
    init_cpustatq(&newest_data);

    const int core_count = get_nprocs();

    struct timespec start;

    float * usage_perc;
    cpuperc * cpu_perc;

    run_analyzer = true;
    while(run_analyzer){
        clock_gettime(CLOCK_MONOTONIC,&start);
        update_data(&newest_data,input_q);

        if(newest_data.size < 2) {  //skip iteration and wait if less than 2 datapoints to calculate delta
            sleepfor(start,analyzer_interval);
            continue;
        }
        //allocate memory for storing percentage data
        cpu_perc = malloc(sizeof *cpu_perc);
        cpu_perc->t = newest_data.back->cpu_stat->t;
        if (cpu_perc == NULL) {
            fprintf(stderr, "Analyzer: failed to allocate memory\n");
            exit(1);
        }
        usage_perc = calloc(core_count+1, sizeof *usage_perc);
        if (usage_perc == NULL) {
            fprintf(stderr, "Analyzer: failed to allocate memory\n");
            exit(1);
        }
        calc_usage_perc(&newest_data,core_count,usage_perc);
        cpu_perc->cores_perc = usage_perc;

        //queue data to printer
        enqueue_cpupercq(printer_q,cpu_perc);

        //TODO logger
            
        sleepfor(start,analyzer_interval);
    }
    return 0;
}

void stop_analyzer() {
    run_analyzer = false;
}

void set_analyzer_interval(long ms) {
    //dont change interval if reader is running
    if(!run_analyzer) {
        analyzer_interval = ms;
    }
}