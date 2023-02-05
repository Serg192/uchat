#include "../inc/libmx.h"

int mx_strncmp(const char *s1, const char *s2, int n){

    int index = 0;

    while(n != 0){
        if(*(s1 + index) != *(s2 + index))
            return *(s1 + index) - *(s2 + index);

        n -= 1;
        index += 1;
    }

    return 0;
}

