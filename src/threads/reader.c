#include "reader.h"

atomic_bool _reader_run = false;
long _reader_interval = 200;
struct timespec _reader_last_run = {0, 0};

cpustat_queue *_analyzer_q = NULL;
log_queue *_reader_logger_q = NULL;

FILE *_reader_open_procstat();
cpustat *_reader_alloc_cpustat(int cores);
char *_reader_msg_start(struct timespec t);
bool _reader_guard();

// thread
int reader_thrd(void *arg)
{
    if (!_reader_guard())
    {
        return 1;
    }
    _reader_run = true;
    const int core_count = get_nprocs();
    struct timespec start;
    // file
    FILE *fp = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    // data read
    corestat *core_stat;
    cpustat *cpu_stat;
    // msg to logger
    char *log_msg;
    int max_len = 0;

    while (_reader_run)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        _reader_last_run = start;
        fp = _reader_open_procstat();

        cpu_stat = _reader_alloc_cpustat(core_count);
        cpu_stat->t = start;

        // logger msg beginning
        log_msg = _reader_msg_start(start);

        // read lines describing cpu usage from file to
        for (int i = 0; i <= core_count; i++)
        {
            core_stat = &(cpu_stat->cores_stat[i]);
            core_stat->core_num = i - 1;
            if ((read = getline(&line, &len, fp)) == -1)
            {
                fprintf(stderr, "Reader: error reading line from file or unexpected EOF");
                exit(1);
            }
            sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
                   &(core_stat->user), &(core_stat->nice), &(core_stat->system), &(core_stat->idle), &(core_stat->iowait), &(core_stat->irq), &(core_stat->softirq), &(core_stat->steal));

            // logger message append contents
            if (i == 0)
            { // for less clutter in log file only cpu and no core stats
                max_len = LOG_MSG_LEN - strlen(log_msg);
                strncat(log_msg, line, max_len);
                strtok(log_msg, "\n");
            }
        }
        // queue msg to logger
        if (log_enqueue(_reader_logger_q, log_msg, start))
        {
            fprintf(stderr, "Reader: could not add element to log q\n");
            exit(1);
        }

        // queue data to anylyzer
        if (cpustat_enqueue(_analyzer_q, cpu_stat))
        {
            fprintf(stderr, "Reader: could not add element to q\n");
            exit(1);
        }

        fclose(fp);
        sleep_for(start, _reader_interval);
    }
    free(line);
    _reader_run = false;
    return 0;
}

void reader_stop()
{
    _reader_run = false;
}

bool reader_running()
{
    return _reader_run;
}

void reader_set_interval(long ms)
{
    // dont change interval if reader is running
    if (!_reader_run)
    {
        _reader_interval = ms;
    }
}

bool reader_set_args(cpustat_queue *analyzer_q, log_queue *logger_q)
{
    if (_reader_run)
    {
        fprintf(stderr, "Reader: cannot change arguments when already running\n");
        return false;
    }
    _analyzer_q = analyzer_q;
    _reader_logger_q = logger_q;
    return true;
}

struct timespec reader_check()
{
    return _reader_last_run;
}

FILE *_reader_open_procstat()
{
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp)
    {
        fprintf(stderr, "Reader: could not open file /proc/stat, stopping\n");
        exit(1);
    }
    return fp;
}

cpustat *_reader_alloc_cpustat(int cores)
{
    cpustat *cpu_stat = malloc(sizeof *cpu_stat);
    if (cpu_stat == NULL)
    {
        fprintf(stderr, "Reader: could not allocate memory for cpu stat\n");
        exit(1);
    }
    cpu_stat->cores_stat = calloc(cores + 1, sizeof(corestat));
    if (cpu_stat->cores_stat == NULL)
    {
        fprintf(stderr, "Reader: could not allocate memory for cores stat\n");
        exit(1);
    }
    return cpu_stat;
}

char *_reader_msg_start(struct timespec t)
{
    char *log_msg = calloc(LOG_MSG_LEN, sizeof *log_msg);
    if (log_msg == NULL)
    {
        fprintf(stderr, "Reader: could not allocate memory for log msg\n");
        exit(1);
    }
    snprintf(log_msg, LOG_MSG_LEN, "READER  [%ld:%03ld]:", t.tv_sec, t.tv_nsec / 1000000);
    return log_msg;
}

bool _reader_guard()
{
    if (_reader_run)
    {
        fprintf(stderr, "Reader: cannot start another thread when already running");
        return false;
    }
    else if (_analyzer_q == NULL || _reader_logger_q == NULL)
    {
        fprintf(stderr, "Reader: args not set");
        return false;
    }
    return true;
}