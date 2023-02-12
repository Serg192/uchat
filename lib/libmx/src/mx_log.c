#include "../inc/libmx.h"

static inline char* get_str_level(int level) {
	switch(level) {
		case LOG_INFO:
			return "INFO: ";
		case LOG_WARNING:
			return "WARNING: ";
		case LOG_ERROR:
			return "ERROR: ";
		default:
			return "TRACE: ";
	}
}

void mx_log(const char* file, int level, char* text) {

	int fd = open(file, O_CREAT | O_RDWR | O_APPEND, 0600);

    char* str_level = get_str_level(level);

	const int log_len = mx_strlen(str_level) + mx_strlen(text) + mx_strlen(__DATE__) + mx_strlen(__TIME__) + 7;

	char* log = mx_strnew(log_len);

	mx_strcat(log, str_level);
	mx_strcat(log, "[");
	mx_strcat(log, __DATE__);
	mx_strcat(log, " | ");
	mx_strcat(log, __TIME__);
	mx_strcat(log, "] ");
	mx_strcat(log, text);
	mx_strcat(log, "\n");

	write( fd, log, log_len);

    mx_strdel(&log);

    close(fd);
}
