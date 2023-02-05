#include "../inc/libmx.h"

//0x00 -> 0x7f one byte   		0xxxxxxx
//0x80 -> 0x7ff two bytes 		110xxxxx 10xxxxxx
//0x800 -> 0xffff tree bytes 	1110xxxx 10xxxxxx 10xxxxxx
//0x10000 -> 0x10ffff 4 bytes   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

void mx_print_unicode(wchar_t c){
	char bytes[4];
	int total;

	if(c < 0x80){
		bytes[0] = (c & 0x7F) | 0x00;
		total = 1;
	} else if(c < 0x800){
		bytes[0] = (c >> 6 & 0x1F )| 0xC0;
		total = 2;
	} else if( c < 0x10000) {
		bytes[0] = (c >> 12 & 0x0F) | 0xE0;
		total = 3; 
	} else {
		bytes[0] = (c >> 18 & 0x7) | 0xF0;
		total = 4;
	}

	int index = 1;
	for(int i = total; i > 1; i--)
		bytes[index++] = ((c >> 6 * (i - 2)) & 0x3F) | 0x80;

	write(1, &bytes, total);
}




