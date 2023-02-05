#include "../inc/libmx.h"

static void print_int_char_arr(char *digits, int from){
    while(from >= 0){
        mx_printchar(digits[from]);
        from -= 1;
    }
}

static int fill_arr(char* arr, int n){
    int pointer = 0;

    while(n >= 0){
        arr[pointer] = (char)((n % 10) + 48);
        n /= 10;
        pointer += 1;
        if(n == 0)
            break;
    }
    return pointer;
}

void mx_printint(int n){
    char digits_arr[11];
    int arr_pointer = 0;
    bool special_case = false;

    if(n < 0){
        mx_printchar('-');
        if(n == -2147483648){
            special_case = true;
            n += 1;
        }
            n *= -1;
    }
    arr_pointer = fill_arr(digits_arr, n);

   if(special_case)
        digits_arr[0] = '8';

    digits_arr[arr_pointer] = '\0';
    arr_pointer -= 1;  
    print_int_char_arr(digits_arr, arr_pointer);
}


