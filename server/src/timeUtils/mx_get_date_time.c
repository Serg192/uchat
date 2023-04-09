#include "../../inc/server.h"

void mx_get_date_time(date_and_time_t* dt){
	time_t current_time;
    struct tm *local_time;

    time(&current_time);
    local_time = localtime(&current_time);

    dt->day = local_time->tm_mday;
    dt->month = local_time->tm_mon + 1;
    dt->year = local_time->tm_year + 1900;
    dt->hour = local_time->tm_hour;
    dt->min = local_time->tm_min;
}
