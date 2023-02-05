#include "../inc/libmx.h"

void *mx_memchr(const void *s, int c, size_t n){
    unsigned char* p = (unsigned char*)s;

    for(size_t i = 0; i < n; i++){
        if(p[i] == (unsigned char)c){
            return (void*)(p + i);
        }
    }

    return NULL;
}

