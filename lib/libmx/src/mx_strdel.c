#include "../inc/libmx.h"

void mx_strdel(char **str){
    free(str[0]);
    str[0] = NULL;
}

