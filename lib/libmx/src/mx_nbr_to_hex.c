#include "../inc/libmx.h"

static char *strnew(const int size){
    char *st;

    if(size < 0)
        return NULL;

    st = (char*)malloc(size + 1);

    for(int i = 0; i <= size; i++)
        st[i] = '\0';

    return st;
}

static int hex_digit_count(unsigned long n){
    int result = 0;

    while(n > 0){
        n /= 16;
        result += 1;
    }
    return result;
}

static char num_to_hex(int num){
    if(num <= 9)
        return num + 48;
    return 97 + num % 10;
}

char *mx_nbr_to_hex(unsigned long nbr){
    int str_size = hex_digit_count(nbr);
    char intermid_arr[str_size];
    char *res = strnew(str_size);
    int index = 0;

    while(nbr > 0){
        intermid_arr[index++] = num_to_hex(nbr % 16);
        nbr /= 16;
    }

    for(int i = str_size - 1; i >= 0; i--){
        res[str_size -1 - i] = intermid_arr[i];
    }

    return res;
}


