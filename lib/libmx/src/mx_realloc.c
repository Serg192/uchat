#include "../inc/libmx.h"

void *mx_realloc(void *ptr, size_t size){
   
    void* new_ptr;
    size_t current_s = malloc_size(ptr);
    //size_t current_s = malloc_usable_size(ptr);
    if(ptr == 0)
      return malloc(size);
    
    new_ptr = malloc(size);

    if(current_s > size)
        mx_memcpy(new_ptr, ptr, size);
    else
        mx_memcpy(new_ptr, ptr, current_s);
    free(ptr);
    return new_ptr;
}

