#include "../inc/libmx.h"

bool mx_isspace(char c){
    return (c == ' ' || c == '\t' || c == '\f' || c == '\r' || c == '\n');
}

