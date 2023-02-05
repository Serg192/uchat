#include "../inc/libmx.h"

static inline int get_file_len(const char *filepath){
    int fd = open(filepath, O_RDONLY);
    int length = 0;
    int bytes_was_read = 0;
    char buff;
    
    if(fd < 0){
        close(fd);
        return 0;
    }
    bytes_was_read = read(fd, &buff, 1);
    while(bytes_was_read > 0){
        length += 1;
        bytes_was_read = read(fd, &buff, 1);
    }

    close(fd);
    return length;
}

char* mx_file_to_str(const char *file){
   int fd = open(file, O_RDONLY);
   char* src = NULL;
   int file_len = get_file_len(file);
    
   if(file_len != 0){
        src = mx_strnew(file_len);
        read(fd, src, file_len);
   }

   close(fd);
   return src;
}


