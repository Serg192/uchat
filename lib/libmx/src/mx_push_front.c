#include "../inc/libmx.h"

void mx_push_front(t_list **list, void *data){
    t_list *node = mx_create_node(data);

    if(list == NULL || *list == NULL)
        *list = node;
    else {
        t_list *tmp = *list;
        *list = node;
        node->next = tmp;
    }
}

