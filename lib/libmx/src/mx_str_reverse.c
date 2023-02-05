#include "../inc/libmx.h"

void mx_str_reverse(char *s){
    int left = 0;
    int right = mx_strlen(s) - 1;

    while(right > left){
        mx_swap_char(&s[right], &s[left]);
        right -= 1;
        left += 1;
    }
}


