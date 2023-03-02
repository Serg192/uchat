#include "../../inc/server.h"

void mx_exec_sql(char* sql_req) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "executing sql sequence");
	mx_log(SERV_LOG_FILE, LOG_TRACE, sql_req);

	sqlite3* db = mx_open_db();

	 char* err;
	 if (sqlite3_exec(db, sql_req, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Failed to execute sql: %s", err);
        mx_close_db(db);
        exit(-1);
    }

    free(sql_req);
	mx_close_db(db);

}
