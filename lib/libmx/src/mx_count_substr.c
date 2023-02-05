#include "../inc/libmx.h"

int mx_count_substr(const char *str, const char *sub){
    if(str == NULL || sub == NULL)
        return -1;
    
    int result = 0;
    int sub_len = mx_strlen(sub);

    if(mx_strlen(str) >= sub_len){
        for(int i = 0; str[i] != '\0'; i++){
            if(mx_strncmp(str + i, sub, sub_len) == 0)
                result += 1;       
        }
    }

    return result;
}


