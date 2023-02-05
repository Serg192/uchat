#include "../inc/libmx.h"

int mx_quicksort(char **arr, int left, int right){
    if(arr == NULL)
        return -1;

    int swaps = 0;
    int l = left;
    int r = right;
    
    if(left < right){
        char* mid = arr[(l + r) / 2];
        while(l < r){
            while(mx_strlen(arr[l]) < mx_strlen(mid))
                l++;
            while(mx_strlen(arr[r]) > mx_strlen(mid))
                r--;

            if(l <= r){
                if(mx_strlen(arr[l]) != mx_strlen(arr[r])){
                    char *t = arr[l];
                    arr[l] = arr[r];
                    arr[r] = t;
                    swaps++;
                }
                l++;
                r--;
            }
        }
        swaps += mx_quicksort(arr, left, r);
        swaps += mx_quicksort(arr, l, right);        
    }

    return swaps;
}


