#include "../inc/server.h"

bool mx_check_if_user_is_in_ban(const int chat_id, const int user_id){
	
	bool result = false;

	sqlite3* db = mx_open_db();
	pthread_mutex_lock(&db_mutex);
	
	sqlite3_stmt* answ = mx_prepare_stmt(db, sqlite3_mprintf("SELECT banned FROM room_member WHERE client_id = '%d' AND room_id = '%d'", user_id, chat_id));

    if(sqlite3_step(answ) == SQLITE_ROW){
    	result = sqlite3_column_int(answ, 0) == 1;
    }

    sqlite3_finalize(answ);

	pthread_mutex_unlock(&db_mutex);
	mx_close_db(db);

	return result;

}

