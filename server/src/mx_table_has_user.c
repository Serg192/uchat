#include "../inc/server.h"

bool mx_table_has_user(const char* login) {
	mx_log(SERV_LOG_FILE, LOG_TRACE, "Checking if database has this user");
	bool result;

	sqlite3* db = mx_open_db();

	char* sql_req = NULL;

	asprintf(&sql_req, "SELECT * FROM 'user' WHERE username = '%s'", login);

	sqlite3_stmt* answ;

	if (sqlite3_prepare_v2(db, sql_req, -1, &answ, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

    result = sqlite3_step(answ) == SQLITE_ROW;

    free(sql_req);

    sqlite3_finalize(answ);

	mx_close_db(db);

	mx_log(SERV_LOG_FILE, LOG_TRACE, mx_itoa(result));

	return result;
}