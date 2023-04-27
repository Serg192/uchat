
#include "../../inc/server.h"

static inline bool has_pair(const char* table,
                            const char* field1,
                            const int val1,
                            const char* field2,
                            const int val2) {

	bool result;
	char* sql_req = NULL;

	asprintf(&sql_req, "SELECT 1 FROM %s WHERE %s = '%d' AND %s = '%d'", table, field1, val1, field2, val2);

	sqlite3* db = mx_open_db();
	pthread_mutex_lock(&db_mutex);


	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

    result = sqlite3_step(stmt) == SQLITE_ROW;

    free(sql_req);
    sqlite3_finalize(stmt);

    pthread_mutex_unlock(&db_mutex);

    mx_close_db(db);

    return result;
}

void mx_add_room_member(int client_id, int room_id, int perm) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "mx_add_room_member()");
	
	if(has_pair("room_member", "client_id", client_id, "room_id", room_id))
			return;

	sqlite3* db = mx_open_db();
	pthread_mutex_lock(&db_mutex);


	char* sql_req = NULL;
	asprintf(&sql_req, "INSERT INTO room_member (client_id, room_id, banned, permissions) VALUES ('%d', '%d','%d', '%d')", client_id, room_id, 0, perm);

	char* err;
	 if (sqlite3_exec(db, sql_req, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Failed to execute sql: %s", err);
        mx_close_db(db);
        exit(-1);
    }

    free(sql_req);
    pthread_mutex_unlock(&db_mutex);

	mx_close_db(db);
}

/*

INSERT INTO EmailsRecebidos (De, Assunto, Data)
SELECT @_DE, @_ASSUNTO, @_DATA
WHERE NOT EXISTS (SELECT * FROM EmailsRecebidos 
                WHERE De = @_DE
                AND Assunto = @_ASSUNTO
                AND Data = @_DATA);

*/

