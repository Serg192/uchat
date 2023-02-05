#include "../inc/libmx.h"

int mx_count_words(const char *str, char c){
    if(str == NULL)
        return -1;
    int word_count = 0;
    int index = 0;
    bool new_w_found = true;

    while(str[index] != '\0'){

        if(str[index] == c)
            new_w_found = true;
        else if(new_w_found){
            new_w_found = false;
            word_count += 1;
        }
        index += 1;
    }
    return word_count;
}



