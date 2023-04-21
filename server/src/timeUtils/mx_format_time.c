#include "../../inc/server.h"

int mx_format_time(date_and_time_t* dt){
	int h = dt->hour;
	int m = dt->min;

	return h * 60 + m;
}
