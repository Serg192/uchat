#include "../../inc/server.h"

sqlite3_stmt* mx_prepare_stmt(sqlite3* db, char* sql_req){

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

    sqlite3_free(sql_req);
    return stmt;
}

