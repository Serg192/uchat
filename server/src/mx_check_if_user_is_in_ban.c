#include "../inc/server.h"

bool mx_check_if_user_is_in_ban(const int chat_id, const int user_id){
	
	bool result = false;

	sqlite3* db = mx_open_db();
	pthread_mutex_lock(&db_mutex);

	char* sql_req = sqlite3_mprintf("SELECT banned FROM room_member WHERE client_id = '%d' AND room_id = '%d'", user_id, chat_id);
	
	sqlite3_stmt* answ;

	if (sqlite3_prepare_v2(db, sql_req, -1, &answ, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

    if(sqlite3_step(answ) == SQLITE_ROW){
    	result = sqlite3_column_int(answ, 0) == 1;
    }

    sqlite3_free(sql_req);

    sqlite3_finalize(answ);

	pthread_mutex_unlock(&db_mutex);
	mx_close_db(db);

	return result;

}

