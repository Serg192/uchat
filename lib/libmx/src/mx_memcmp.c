#include "../inc/libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n){
    if(n == 0)
        return 0;

    size_t index;
    const unsigned char *u_s1 = s1;
    const unsigned char *u_s2 = s2;

    for(index = 0; index < n && u_s1[index] == u_s2[index]; index++){
        if(u_s1[index] == '\0' && u_s2[index] == '\0')
            return 0;
    }
    return u_s1[index] - u_s2[index];
}

