#include "../inc/libmx.h"

char *mx_strnew(const int size){
    char *st;

    if(size < 0)
        return NULL;

    st = (char *)malloc(size + 1);

    for(int i = 0; i <= size; i++)
        st[i] = '\0';

    return st;
}

