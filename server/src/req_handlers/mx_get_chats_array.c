#include "../../inc/server.h"

void mx_get_chats_array(cJSON* parent_json, const char* search_condition) {
	cJSON* chats_array = cJSON_CreateArray();

	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, search_condition, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }


    while(sqlite3_step(stmt) == SQLITE_ROW) {
    	cJSON* item = cJSON_CreateObject();
    	cJSON_AddStringToObject(item, "name", (const char*)sqlite3_column_text(stmt, 2));
		cJSON_AddNumberToObject(item, "color", sqlite3_column_int64(stmt, 1)); //maybe also (const char*)
    	cJSON_AddNumberToObject(item, "id", sqlite3_column_int64(stmt, 0)); //maybe also (const char*)

    	cJSON_AddItemToArray(chats_array, item);
    }

    cJSON_AddItemReferenceToObject(parent_json, "chats", chats_array);
	
	sqlite3_finalize(stmt);

	pthread_mutex_unlock(&db_mutex);

	mx_close_db(db);
}

