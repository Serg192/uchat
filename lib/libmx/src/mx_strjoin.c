#include "../inc/libmx.h"

char* mx_strjoin(const char *s1, const char *s2){
     char *n_str;

    if(s1 == NULL && s2 == NULL)
        return NULL;
    if(s1 == NULL)
        return mx_strdup(s2);
    if(s2 == NULL)
        return mx_strdup(s1);

    n_str = mx_strnew(mx_strlen(s1) + mx_strlen(s2));
    n_str = mx_strcpy(n_str, s1);
    return mx_strcat(n_str, s2);
}

