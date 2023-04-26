#include "../../inc/server.h"


char* mx_get_str_field_val(const char* table, 
	                       const char* fieldname,
	                       const char* key,
	                       const char* val) {

	char* result = NULL;
	char* sql_req = NULL;

	asprintf(&sql_req, "SELECT %s FROM '%s' WHERE %s = '%s'", fieldname, table, key, val);

	sqlite3* db = mx_open_db();
	pthread_mutex_lock(&db_mutex);

	sqlite3_stmt* answ;

	if (sqlite3_prepare_v2(db, sql_req, -1, &answ, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

    if(sqlite3_step(answ) == SQLITE_ROW) {

    	result = mx_strdup((const char*) sqlite3_column_text(answ, 0));
    }


 	free(sql_req);

    sqlite3_finalize(answ);

    pthread_mutex_unlock(&db_mutex);

	mx_close_db(db);

	return result;
}

