#include "../inc/libmx.h"

bool mx_isletter(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

