#include "../inc/libmx.h"

static inline int get_num_len(int num){
    int len = 0;
    if(num < 0){
        len ++;
        if(num == -2147483648)
            num += 1;
        num *= -1;
    }

    if(num == 0)
        return len + 1;
    
    while(num > 0){
        len++;
        num /= 10;
    }

    return len;
}

static inline void rev(char *result, int len){
    
    int left = 0;
    int right = len - 1;
    
    if(result[0] == '-'){
        left = 1;
    }

    while(left < right){
        char t = result[left];
        result[left++] = result[right];
        result[right--] = t;
    }
}

char *mx_itoa(int number){
    const int num_len = get_num_len(number);
    char *result = mx_strnew(num_len);

    if(number == 0)
        result = mx_strcpy(result, "0");
    else if(number == -2147483648)
        return mx_strcpy(result, "-2147483648");
    else {
        int index = 0;

        if(number < 0){
            number *= -1;
            result[index++] = '-';
        }

        while(number > 0){
            result[index++] = number % 10 + '0';
            number /= 10;
        }
    }

    rev(result, num_len);
    return result;
}


