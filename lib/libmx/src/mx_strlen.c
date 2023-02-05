#include "../inc/libmx.h"

int mx_strlen(const char *s){
    int len = 0;

    for(int i = 0; s[i] != '\0'; i++)
        len += 1;

    return len;
}

