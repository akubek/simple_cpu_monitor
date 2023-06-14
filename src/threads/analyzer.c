#include "analyzer.h"

atomic_bool _analyzer_run = false;
long _analyzer_interval = 500;
struct timespec _analyzer_last_run = {0, 0};
cpustat_queue *_analyzer_input_q = NULL;
cpuperc_queue *_printer_q = NULL;
log_queue *_analyzer_logger_q = NULL;

void _calc_usage_perc(cpustat_queue *newest_data, int core_count, float *usage);
void _update_data(cpustat_queue *newest_data, cpustat_queue *q);
char *_analyzer_log_msg(struct timespec t, float perc);
cpuperc *_alloc_cpuperc(int core_count);
bool _analyzer_guard();

// thread
int analyzer_thrd(void *arg)
{
    if (!_analyzer_guard())
    {
        return 1;
    }
    _analyzer_run = true;
    char *log_msg;
    int max_len = 0;

    cpustat_queue newest_data; // for storing latest cpu usage data points for about 2s
    cpustat_init_q(&newest_data);

    const int core_count = get_nprocs();
    struct timespec start;

    cpuperc *cpu_perc;

    while (_analyzer_run)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        _analyzer_last_run = start;
        _update_data(&newest_data, _analyzer_input_q);

        if (newest_data.size < 2)
        { // skip iteration and wait if less than 2 datapoints to calculate delta
            sleep_for(start, _analyzer_interval);
            continue;
        }
        // allocate memory for storing percentage data
        cpu_perc = _alloc_cpuperc(core_count);
        cpu_perc->t = newest_data.back->cpu_stat->t;

        _calc_usage_perc(&newest_data, core_count, cpu_perc->cores_perc);

        // queue data to printer
        if (cpuperc_enqueue(_printer_q, cpu_perc))
        {
            fprintf(stderr, "Analyzer: could not add element to q\n");
            exit(1);
        }

        // queue msg to logger
        log_msg = _analyzer_log_msg(start, cpu_perc->cores_perc[0]);
        if (log_enqueue(_analyzer_logger_q, log_msg, start))
        {
            fprintf(stderr, "Analyzer: could not add element to log q\n");
            exit(1);
        }

        sleep_for(start, _analyzer_interval);
    }

    cpustat_delete_q(&newest_data);
    _analyzer_run = false;
    return 0;
}

cpuperc *_alloc_cpuperc(int core_count)
{
    cpuperc *cpu_perc = malloc(sizeof *cpu_perc);
    if (cpu_perc == NULL)
    {
        fprintf(stderr, "Analyzer: failed to allocate memory\n");
        exit(1);
    }
    cpu_perc->cores_perc = calloc(core_count + 1, sizeof(cpuperc));
    if (cpu_perc->cores_perc == NULL)
    {
        fprintf(stderr, "Analyzer: failed to allocate memory\n");
        exit(1);
    }
    return cpu_perc;
}

char *_analyzer_log_msg(struct timespec t, float perc)
{
    char *log_msg = calloc(LOG_MSG_LEN, sizeof *log_msg);
    if (log_msg == NULL)
    {
        fprintf(stderr, "Analyzer: could not allocate memory for log msg\n");
        exit(1);
    }
    snprintf(log_msg, LOG_MSG_LEN, "ANALYZER[%ld:%03ld]:cpu  %.2f %%", t.tv_sec, t.tv_nsec / 1000000, perc);
    return log_msg;
}

int _is_older_than(cpustat *data, int seconds)
{
    // old if is older than 2 sec
    struct timespec comp;
    struct timespec elem = data->t;
    clock_gettime(CLOCK_MONOTONIC, &comp);
    comp.tv_sec -= seconds;
    return (elem.tv_sec < comp.tv_sec || elem.tv_sec == comp.tv_sec && elem.tv_nsec < comp.tv_nsec);
}

void _analyzer_load_datapoints(cpustat_queue *q, cpustat **newest, cpustat **previous)
{
    // tries to load previous from up to 1s ago
    // does not dequeue data
    cpustat_node *node = q->front;
    *newest = q->back->cpu_stat;
    do
    {
        *previous = node->cpu_stat;
        if (!_is_older_than(node->cpu_stat, 1))
        {
            break;
        }
        node = node->next;
    } while (node->next != NULL);
}

unsigned long long _core_total(corestat stat)
{
    return stat.user + stat.nice + stat.system + stat.idle + stat.iowait + stat.irq + stat.softirq + stat.steal;
}

unsigned long long _core_idle(corestat stat)
{
    return stat.idle + stat.iowait;
}

void _calc_usage_perc(cpustat_queue *newest_data, int core_count, float *usage)
{
    unsigned long long total_delta, idle_delta;
    cpustat *stat_curr, *stat_prev;
    // get 2 datapoints
    _analyzer_load_datapoints(newest_data, &stat_curr, &stat_prev);

    // calculate usage percentages
    for (int i = 0; i <= core_count; i++)
    {
        total_delta = _core_total(stat_curr->cores_stat[i]) - _core_total(stat_prev->cores_stat[i]);
        idle_delta = _core_idle(stat_curr->cores_stat[i]) - _core_idle(stat_prev->cores_stat[i]);

        usage[i] = ((total_delta - idle_delta) * 100.0) / total_delta;
    }
}

void _update_data(cpustat_queue *newest_data, cpustat_queue *q)
{
    cpustat *elem;
    while (elem = cpustat_dequeue(q))
    {
        cpustat_enqueue(newest_data, elem);
    }
    // remove old datapoints
    while (newest_data->front != NULL && _is_older_than(newest_data->front->cpu_stat, 2))
    {
        elem = cpustat_dequeue(newest_data);
        free(elem->cores_stat);
        free(elem);
    }
}

bool _analyzer_guard()
{
    if (_analyzer_run)
    {
        fprintf(stderr, "Analyzer: cannot start another thread when already running");
        return false;
    }
    else if (_analyzer_input_q == NULL || _printer_q == NULL || _analyzer_logger_q == NULL)
    {
        fprintf(stderr, "Analyzer: args not set");
        return false;
    }
    return true;
}

void stop_analyzer()
{
    _analyzer_run = false;
}

bool analyzer_running()
{
    return _analyzer_run;
}

void analyzer_set_interval(long ms)
{
    if (!_analyzer_run)
    {
        _analyzer_interval = ms;
    }
}

struct timespec analyzer_check()
{
    return _analyzer_last_run;
}

bool analyzer_set_args(cpustat_queue *analyzer_q, cpuperc_queue *printer_q, log_queue *logger_q)
{
    if (_analyzer_run)
    {
        fprintf(stderr, "Analyzer: cannot change arguments when already running\n");
        return false;
    }
    _analyzer_input_q = analyzer_q;
    _printer_q = printer_q;
    _analyzer_logger_q = logger_q;
    return true;
}