#include "../../inc/server.h"

static inline char* make_search_pattern(const char* pattern) {
	char* result = mx_strnew(mx_strlen(pattern) + 4);
	mx_strcat(result, "%%");
	mx_strcat(result, pattern);
	mx_strcat(result, "%%");
	return result;
}

void mx_handle_chat_search(client_t* client, request_t* req) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Chat search request");

	const char* pattern = cJSON_GetObjectItem(req->json, "pattern")->valuestring;

	cJSON* response = cJSON_CreateObject();

	char* sql_req = NULL;

	char* s_pattern = make_search_pattern(pattern);
	
	sql_req = sqlite3_mprintf("SELECT * FROM 'room' WHERE room.name LIKE '%s' AND room.id NOT IN (SELECT room_member.room_id FROM 'room_member' WHERE client_id = '%d') ", s_pattern, client->user_id);
	
	mx_get_chats_array(response, sql_req);

	cJSON_AddNumberToObject(response, "rtype", CHAT_SEARCH_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	//mx_strdel(response_str);
	free(response_str);

	free(s_pattern);

	//free(sql_req);
	sqlite3_free(sql_req);
}

