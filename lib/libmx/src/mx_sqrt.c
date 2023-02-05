#include "../inc/libmx.h"

int mx_sqrt(int x){
    int p_answ = 1;

    while(p_answ <= x / p_answ){
        if(p_answ * p_answ == x)
            return p_answ;
        p_answ += 1;
    }
    return 0;
}


