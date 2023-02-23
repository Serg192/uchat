#include "../../inc/server.h"

static inline char* make_search_pattern(const char* pattern) {
	char* result = mx_strnew(mx_strlen(pattern) + 4);
	mx_strcat(result, "%%");
	mx_strcat(result, pattern);
	mx_strcat(result, "%%");
	return result;
}

static inline void make_search(const char* pattern, 
	                           cJSON* parent_json,
	                           client_t* client) {

	cJSON* chats_array = cJSON_CreateArray();
	char* s_pattern = make_search_pattern(pattern);

	sqlite3* db = mx_open_db();

	char* sql_req = NULL;

	asprintf(&sql_req, "SELECT * FROM 'room' WHERE room.name LIKE '%s' AND room.id NOT IN (SELECT room_member.room_id FROM 'room_member' WHERE client_id = '%d') ", s_pattern, client->user_id);
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

    while(sqlite3_step(stmt) == SQLITE_ROW) {
    	cJSON* item = cJSON_CreateObject();
    	cJSON_AddStringToObject(item, "name", sqlite3_column_text(stmt, 1));
    	cJSON_AddNumberToObject(item, "id", sqlite3_column_int64(stmt, 0));

    	cJSON_AddItemToArray(chats_array, item);
    }

    cJSON_AddItemReferenceToObject(parent_json, "chats", chats_array);
  
	free(s_pattern);

	free(sql_req);
	
	sqlite3_finalize(stmt);

	mx_close_db(db);
}


void mx_handle_chat_search(client_t* client, request_t* req) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Chat search request");

	const char* pattern = cJSON_GetObjectItem(req->json, "pattern")->valuestring;

	cJSON* response = cJSON_CreateObject();

	make_search(pattern, response, client);

	cJSON_AddNumberToObject(response, "rtype", CHAT_SEARCH_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	//mx_strdel(response_str);
	free(response_str);
}

