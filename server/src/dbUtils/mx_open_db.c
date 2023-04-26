#include "../../inc/server.h"

sqlite3* mx_open_db(){

	sqlite3* db;

	if(sqlite3_open(DATABASE_FILE, &db) != SQLITE_OK) {
		mx_log(SERV_LOG_FILE, LOG_ERROR, (char *)sqlite3_errmsg(db));
		exit(-1);
	}

	return db;
}

