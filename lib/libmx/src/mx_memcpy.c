#include "../inc/libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n){
    unsigned char* d = (unsigned char*) dst;
    for(size_t i = 0; i < n; i++)
        d[i] = ((unsigned char *)src)[i];
    return dst;
}

