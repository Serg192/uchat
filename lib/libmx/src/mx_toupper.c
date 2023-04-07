#include "../inc/libmx.h"

int mx_toupper(int c) {
    if (96 < c && c < 123) {
        return c - 32;
    }
    else {
        return c;
    }     
}

