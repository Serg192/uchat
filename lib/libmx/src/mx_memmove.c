#include "../inc/libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len){
    unsigned char t[len];
    mx_memcpy(t, src, len);
    mx_memcpy(dst, t, len);
    return dst;    
}

