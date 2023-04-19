#include "../inc/libmx.h"

#include <stdio.h>

queue_t* mx_create_queue() {
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    queue->empty = true;
    queue->size = 0;
    queue->elements = NULL;
    return queue;
}

void mx_queue_push(queue_t* queue, void* data) {
    mx_push_back(&(queue->elements), data);
    queue->size++;
    queue->empty = false;
}

void* mx_queue_peek(queue_t* queue) {
    return queue->empty ? NULL : queue->elements->data;
}

void* mx_queue_pop(queue_t* queue){

    if(queue->empty)
        return NULL;

    queue->size--;
    if(queue->size == 0)
        queue->empty = true;
    
    void* ret = queue->elements->data;
    mx_pop_front(&(queue->elements));
    return ret;
}

void mx_destroy_queue(queue_t** queue) {
    t_list* head = (*queue)->elements;
    
    while(head) {
        t_list* to_del = head;
        head = head->next;
        free(to_del);
    }
    free(*queue);
    *queue = NULL;
}
