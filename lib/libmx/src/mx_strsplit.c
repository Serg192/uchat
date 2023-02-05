#include "../inc/libmx.h"

char **mx_strsplit(const char *s, char c){
    if(s == NULL)
        return NULL;
    
    int words_max = mx_count_words(s, c) + 1;
    char** words = (char**)malloc(words_max * sizeof(char*));
    int w_index = 0;

    for(int i = 0; s[i] != '\0'; i++){
        if(s[i] != c){
            int index = i;
            int w_len = 0;
            while(s[index] != '\0'){
                if(s[index] == c)
                    break;
                w_len++;
                index++;
            }
            words[w_index++] = mx_strndup(s + i, w_len);
            i += w_len;
        }
    }

    words[words_max - 1] = NULL;
    return words;
}


