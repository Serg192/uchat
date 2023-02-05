#include "../inc/libmx.h"

void *mx_memrchr(const void *s, int c, size_t n){
    unsigned char *p = (unsigned char *)s + mx_strlen(s);
    
    while(n--){
        if(*p == (unsigned char)c){
            return p;
        }
        p--;
    }
    return NULL;
}

