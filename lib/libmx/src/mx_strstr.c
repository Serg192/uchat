#include "../inc/libmx.h"

char *mx_strstr(const char *haystack, const char *needle){
    int h_len = mx_strlen(haystack);
    int n_len = mx_strlen(needle);

    if(!haystack || !needle)
        return 0;

    if(n_len == 0)
        return (char*) haystack;

    for(int i = 0; i < h_len; i++){
        if(needle[0] == haystack[i] && mx_strncmp(needle, (char*)haystack + i, n_len) == 0){
            return (char*)haystack + i;
        }
    }

    return 0;
}

