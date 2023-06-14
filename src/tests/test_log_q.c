#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "../dataStructures/log_queue.h"

char text1[] = "text1";
char text2[] = "text2";
char text3[] = "text3";
struct timespec t1 = {100, 100};
struct timespec t2 = {100, 200};
struct timespec t3 = {200, 100};
log_queue q;

int main() {
    char *result;

    log_init_q(&q);
    assert(q.back == NULL);
    assert(q.front == NULL);
    assert(q.size == 0);

    log_enqueue(&q,text1,t1);
    assert(q.size == 1);
    log_enqueue(&q,text3,t3);
    assert(q.size == 2);
    log_enqueue(&q,text2,t2);
    assert(q.size == 3);

    result = log_dequeue(&q);
    assert(strcmp(result,"text1") == 0);
    assert(q.size == 2);
    result = log_dequeue(&q);
    assert(strcmp(result,"text2") == 0);
    assert(q.size == 1);
    result = log_dequeue(&q);
    assert(strcmp(result,"text3") == 0);
    assert(q.size == 0);

    assert(q.back == NULL);
    assert(q.front == NULL);

    log_delete_q(&q);

    return 0;
}