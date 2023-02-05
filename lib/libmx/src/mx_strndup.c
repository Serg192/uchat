#include "../inc/libmx.h"

char* mx_strndup(const char *s1, size_t n){
    size_t len = mx_strlen(s1);
    
    len = n < len ? n : len;

    return mx_strncpy(mx_strnew(len), s1, len);
}

