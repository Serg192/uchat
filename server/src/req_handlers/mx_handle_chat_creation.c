#include "../../inc/server.h"

//return id of created chat
static inline int create_chat(const char* name, const int room_color) {
	int id;

	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	char* sql_req = sqlite3_mprintf("INSERT INTO 'room' ('name', 'color') VALUES ('%s', '%d')", name, room_color);
	
	char* err;
	 if (sqlite3_exec(db, sql_req, NULL, NULL, &err) != SQLITE_OK) {
        //fprintf(stderr, "Failed to execute sql: %s", err);
        mx_close_db(db);
        exit(-1);
    }

    id = (int)sqlite3_last_insert_rowid(db);

    sqlite3_free(sql_req);
    pthread_mutex_unlock(&db_mutex);
	mx_close_db(db);

	return id;
}

void mx_handle_chat_creation(client_t* client, request_t* req) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Creating new chat");

	const char* room_name = cJSON_GetObjectItem(req->json, "name")->valuestring;
	const int room_color = cJSON_GetObjectItem(req->json, "color")->valueint;

	int room_name_len = mx_strlen(room_name);

	mx_log(SERV_LOG_FILE, LOG_TRACE, (char*)room_name);

	cJSON* response = cJSON_CreateObject();

	if(room_name_len == 0 || room_name_len >= 32) {
		cJSON_AddNumberToObject(response, "rtype", CHAT_CREATION_ERR_RESP);
		cJSON_AddStringToObject(response, "message", "Chat name should have less than 32 characters");
	} else {
		int chat_id = create_chat(room_name, room_color);

		mx_add_room_member(client->user_id, chat_id, TYPE_OWNER);
		//add group member
		
		cJSON_AddNumberToObject(response, "rtype", CHAT_CREATION_SUCCESS_RESP);
	}

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
	free(response_str);
}
