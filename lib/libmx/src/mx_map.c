#include "../inc/libmx.h"

void mx_print_map(map_t* map) {
	printf("__MAP__\n");
    for (int i = 0; i < MAP_SIZE; i++) {
        map_node_t* node = map->table[i];
       
        while (node != NULL) {
        	printf("Key %d: ", i);
            printf("(%d, %p) ", node->key, node->value);
            node = node->next;
            printf("\n");
        }
    }
}


map_t* mx_create_map() {
    map_t* map = (map_t*)malloc(sizeof(map_t));

    for (int i = 0; i < MAP_SIZE; i++) {
        map->table[i] = NULL;
    }

    return map;
}

void mx_map_put(map_t* map, int key, void* value) {
    int index = key % MAP_SIZE;

    map_node_t* node = map->table[index];
    while (node != NULL) {
        if (node->key == key) {
            node->value = value;
            return;
        }
        node = node->next;
    }

    node = (map_node_t*)malloc(sizeof(map_node_t));
    node->key = key;
    node->value = value;
    node->next = map->table[index];
    map->table[index] = node;
}

void* mx_map_get(map_t* map, int key) {
    int index = key % MAP_SIZE;

    map_node_t* node = map->table[index];
    while (node != NULL) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }

    return NULL;
}

void mx_map_remove(map_t* map, int key) {
    int index = key % MAP_SIZE;
    map_node_t* prev = NULL;
    map_node_t* curr = map->table[index];

    while (curr != NULL) {
        if (curr->key == key) {
            if (prev == NULL) {
                map->table[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void mx_map_clear(map_t* map) {
    for (int i = 0; i < MAP_SIZE; i++) {
        map_node_t* node = map->table[i];
        while (node != NULL) {
            map_node_t* temp = node;
            node = node->next;
            free(temp);
        }
        map->table[i] = NULL;
    }
    free(map);
}
