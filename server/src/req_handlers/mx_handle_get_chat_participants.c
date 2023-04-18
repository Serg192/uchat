#include "../../inc/server.h"

static inline void build_users_array(int chat_id, cJSON* response){
	cJSON* users_array = cJSON_CreateArray();

	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	char* sql_req = NULL;
	asprintf(&sql_req, "SELECT * FROM room_member WHERE room_id = '%d'", chat_id);
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

    mx_log(SERV_LOG_FILE, LOG_TRACE, "First statement was prepared");


    while(sqlite3_step(stmt) == SQLITE_ROW) {
    	int client_id = sqlite3_column_int64(stmt, 0);
    	mx_log(SERV_LOG_FILE, LOG_TRACE, "Chat user");
    	mx_log(SERV_LOG_FILE, LOG_TRACE, mx_itoa(sqlite3_column_int64(stmt, 0)));
    	sqlite3_stmt* inner_stmt;
    	asprintf(&sql_req, "SELECT * FROM user WHERE id = '%d'", client_id);

    	if (sqlite3_prepare_v2(db, sql_req, -1, &inner_stmt, 0) != SQLITE_OK) {
	 		mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        	mx_close_db(db);
        	exit(-1);
    	}

    	while(sqlite3_step(inner_stmt) == SQLITE_ROW) {
    		cJSON* item = cJSON_CreateObject();
    		cJSON_AddStringToObject(item, "username", sqlite3_column_text(inner_stmt, 1));
    		cJSON_AddNumberToObject(item, "id", sqlite3_column_int64(inner_stmt, 0));

    		mx_log(SERV_LOG_FILE, LOG_TRACE, sqlite3_column_text(inner_stmt, 1));

    		cJSON_AddItemToArray(users_array, item);
    	}

    	sqlite3_finalize(inner_stmt);

    }

    cJSON_AddItemReferenceToObject(response, "users", users_array);

    free(sql_req);
    sqlite3_finalize(stmt);

    pthread_mutex_unlock(&db_mutex);
    mx_close_db(db);
}

void mx_handle_get_chat_participants(client_t* client, request_t* req) {
	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling get chat participants request");

	const int chat_id = cJSON_GetObjectItem(req->json, "room_id")->valueint;

	cJSON* response = cJSON_CreateObject();


	char* sql_req = NULL;
	asprintf(&sql_req, "SELECT * FROM room WHERE id = '%d'", chat_id);

	if(!mx_check_if_row_exists(sql_req)) {
		// There is no nuch room
	} else {
		build_users_array(chat_id, response);
		cJSON_AddNumberToObject(response, "rtype", PARTICIPANTS_RESP);
	}


	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
	free(response_str);
}