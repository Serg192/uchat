#include "../../inc/server.h"

bool mx_check_if_row_exists(char* sql_req) {
	
	bool result;

	sqlite3* db = mx_open_db();
	pthread_mutex_lock(&db_mutex);


	sqlite3_stmt* answ;

	if (sqlite3_prepare_v2(db, sql_req, -1, &answ, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

    result = sqlite3_step(answ) == SQLITE_ROW;

    sqlite3_free(sql_req);

    sqlite3_finalize(answ);

	pthread_mutex_unlock(&db_mutex);
	mx_close_db(db);

	return result;
}

