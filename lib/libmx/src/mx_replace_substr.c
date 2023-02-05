#include "../inc/libmx.h"

char* mx_replace_substr(const char *src, const char *sub, const char* replace){
    int s_to_replace = 0;
    int src_len = mx_strlen(src);
    int sub_len = mx_strlen(sub);
    char* str = mx_strnew(src_len);
    int index = 0;
    char* result = NULL;

    for(int i = 0; src[i] != '\0'; i++){
        if(mx_strncmp(src + i, sub, sub_len) == 0){
            s_to_replace += 1;
            str[index++] = 31; 
            i += sub_len - 1;
            continue;
        }
        str[index++] = src[i];
    } 
    result = mx_strnew(src_len - s_to_replace * sub_len + s_to_replace * mx_strlen(replace));
    index = 0;

    for(int i = 0; str[i] != '\0'; i++){      
        if(str[i] == 31){
            for(int j = 0; j < mx_strlen(replace); j++)
                result[index++] = replace[j];
            continue;
        }
        result[index++] = str[i];
    }
    mx_strdel(&str);  
    return result;
}


