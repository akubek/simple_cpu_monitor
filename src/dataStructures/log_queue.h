#ifndef LOG_Q_H
#define LOG_Q_H
#include <time.h>
#include <stdlib.h>
#include <threads.h>
#include <stdbool.h>
#include <stdio.h>

#include "../common/cpumon_util.h"

#define LOG_MSG_LEN 1024

typedef struct log_node log_node;

struct log_node
{
    char *log_msg;
    struct timespec t;
    log_node *next;
    log_node *prev;
};

typedef struct log_queue
{
    mtx_t qmtx;
    int size;
    log_node *front;
    log_node *back;
} log_queue;

void log_init_q(log_queue *q);
int log_enqueue(log_queue *q, char *log_msg, struct timespec t);
char *log_dequeue(log_queue *q);
void log_delete_q(log_queue *q);

#endif
