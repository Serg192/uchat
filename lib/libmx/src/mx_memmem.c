#include "../inc/libmx.h"

void *mx_memmem(const void *big, size_t big_len, const void *little, size_t little_len){
    if(big_len <= 0 || little_len <= 0 || big_len < little_len)
        return NULL;

    unsigned char *str = (unsigned char *)big;
    unsigned char *sub = (unsigned char *)little;

    while(*str){
        if(mx_memcmp(str, sub, little_len - 1) == 0)
            return str;
        str+=1;
    }
    
    return NULL;
}

