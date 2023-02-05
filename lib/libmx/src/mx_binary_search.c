#include "../inc/libmx.h"

int mx_strcmp(const char *s1, const char *s2);

int mx_binary_search(char **arr, int size, const char *s, int *count){
    int left = 0;
    int right = size - 1;
    int mid = 0;
    *count = 0;

    while(right >= left){
        mid = (left + right) / 2;
        int comp = mx_strcmp(arr[mid], s);
        *count += 1;
        if(comp == 0)
            return mid;
        else if(comp > 0)
            right = mid - 1;
        else
            left = mid + 1;
    }
    *count = 0;
    return -1;
}


