#include "log_queue.h"

bool _log_compare_time(struct timespec t1, struct timespec t2) {
    return t1.tv_sec > t2.tv_sec || t1.tv_sec == t2.tv_sec && t1.tv_nsec > t2.tv_nsec;
}

void log_init_q(log_queue *q) {
    mtx_init(&q->qmtx,mtx_plain);
    q->size = 0;
    q->front = NULL;
    q->back = NULL;
}

int log_enqueue(log_queue *q, char * log_msg, struct timespec t) {
    if(mtx_lock(&q->qmtx)==thrd_error) {
        exit(1);
    }
    log_node *search_node, *p;
    log_node *new_node = malloc(sizeof *new_node);
    if(new_node == NULL) {
        return 1;
    }
    new_node->log_msg = log_msg;
    new_node->t = t;
    new_node->next = NULL;
    new_node->prev = NULL;
    if(q->front == NULL) {
        q->front = q->back = new_node;
    } else {
        //search based on time of msg creation
        search_node = q->back;
        while (_log_compare_time(search_node->t,new_node->t)) {
            search_node = search_node->prev;
        }
        //insert new node
        p = search_node->next;
        search_node->next = new_node;
        new_node->prev = search_node;
        new_node->next = p;
        if (p != NULL) {
            p->prev = new_node;
        } else {//p is null if search node was the last node
            q->back = new_node;
        }
        
    }
    q->size++;
    mtx_unlock(&q->qmtx);
    return 0;
}
char *log_dequeue(log_queue *q) {
    if(mtx_lock(&q->qmtx)==thrd_error) {
        exit(1);
    }
    log_node * old_front = q->front;
    q->front = q->front->next;
    if (q->front != NULL) {
        q->front->prev = NULL;
    }
    q->size--;
    
    char *log_msg = old_front->log_msg;
    free(old_front);
    mtx_unlock(&q->qmtx);
    return log_msg;
}

void log_delete_q(log_queue *q) {
    char * elem;
    while(q->front != NULL) {
        elem = log_dequeue(q);
        free(elem);
    }
    if(mtx_lock(&q->qmtx)==thrd_error) {
        exit(1);
    }
    mtx_unlock(&q->qmtx);
    mtx_destroy(&q->qmtx);
}