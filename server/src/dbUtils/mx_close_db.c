#include "../../inc/server.h"

void mx_close_db(sqlite3* db) {
	if (sqlite3_close(db) != SQLITE_OK) {
        mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)sqlite3_errmsg(db));
        exit(-1);
    }
}

