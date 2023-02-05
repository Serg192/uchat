#include "../inc/libmx.h"

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd){
    if (buf_size < 0 || fd <= 0) 
        return -2;

    buf_size = 1;
    int bytes_read = 0;
    int result = 0;
    char buf;
    mx_strdel(lineptr);
    if ((bytes_read = read(fd, &buf, buf_size)) > 0){
        *lineptr = mx_strjoin(*lineptr, &buf);
         result += bytes_read;
     }
     else 
         return -1;
        
     while((bytes_read = read(fd, &buf, buf_size)) > 0) {
         if(delim == buf) break;
            result += bytes_read;
            *lineptr = mx_strjoin(*lineptr, &buf);
     }

     return result;
}



