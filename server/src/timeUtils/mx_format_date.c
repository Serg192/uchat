#include "../../inc/server.h"

int mx_format_date(date_and_time_t* dt) {
	int day = dt->day;
	int month = dt->month;
	int year = dt->year;

	return day * 1000000 + month * 10000 + year;
}
