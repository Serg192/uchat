#include "../inc/libmx.h"


// Convert hex num from 0 to F to unsigned long
static int hexn_to_int(char n){
    if(n >= '0' && n <= '9')
        return  n - 48;
    else if((n >= 'A' && n <= 'Z') || (n >= 'a' && n <= 'z')){
        if(n >= 'a' && n <= 'f')
            return  10 + n - 97;
        else if(n >= 'A' && n <= 'F')
            return  10 + n - 65;
    }
    return -1;
}


static int strlen(const char *s){
    int len = 0;

    for(int i = 0; s[i] != '\0'; i++)
        len += 1;
    return len;
}

static unsigned long pow(unsigned int num, int p){

    unsigned long res = 1;
    for(int i = 0; i < p; i++)
        res *= num;
    return res;
}

unsigned long mx_hex_to_nbr(const char *hex){
    if(hex == NULL)
        return 0;
    unsigned long result = 0;
    int p = strlen(hex) - 1;
    int index = 0;

    while(hex[index] != '\0'){
        int n = hexn_to_int(hex[index]);
        if(n == -1)
            return 0;
        result += n * pow(16, p);
        index += 1;
        p -= 1;
    }
    return result;
}


