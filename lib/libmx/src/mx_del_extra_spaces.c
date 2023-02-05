#include "../inc/libmx.h"

char* mx_del_extra_spaces(const char *str){
    if(str == NULL)
        return NULL;

    char* tmp = mx_strtrim(str);
    char* result = mx_strnew(mx_strlen(tmp));
    int index = 0;
    bool space = false;

    for(int i = 0; tmp[i] != '\0'; i++){
        if(!mx_isspace(tmp[i])){
            result[index++] = tmp[i];
            space = false;
        }else if(!space){
            result[index++] = ' ';
            space = true;
        }    
    }

    mx_strdel(&tmp);
    return result;
}


