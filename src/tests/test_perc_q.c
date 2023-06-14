#include <assert.h>
#include "../dataStructures/cpuperc_queue.h"

cpuperc p1;
cpuperc p2;
float cores1[] = {1.0,2.0,3.0};
float cores2[] = {10.0,20.0,30.0};
cpuperc_queue q;

int main() {
    p1.cores_perc = cores1;
    p2.cores_perc = cores2;

    cpuperc *result;

    cpuperc_init_q(&q);
    assert(q.back == NULL);
    assert(q.front == NULL);
    assert(q.size == 0);

    cpuperc_enqueue(&q,&p1);
    assert(q.size == 1);
    cpuperc_enqueue(&q,&p2);
    assert(q.size == 2);

    result = cpuperc_dequeue(&q);
    assert(result == &p1);
    assert(q.size == 1);
    result = cpuperc_dequeue(&q);
    assert(result == &p2);
    assert(q.size == 0);

    assert(q.back == NULL);
    assert(q.front == NULL);

    cpuperc_delete_q(&q);
    
    return 0;
}