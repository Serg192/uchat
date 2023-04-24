#include "../../inc/server.h"


void mx_handle_ban(client_t* client, request_t* req){
	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling ban");

	const int chat_id = cJSON_GetObjectItem(req->json, "chat_id")->valueint;
	const int user_id = cJSON_GetObjectItem(req->json, "user_id")->valueint;
	const int banned = cJSON_GetObjectItem(req->json, "banned")->valueint;

	char* sql_req = NULL;

	asprintf(&sql_req, "UPDATE room_member SET banned = '%d' WHERE client_id = '%d' AND room_id = '%d'", banned, user_id, chat_id);
	mx_exec_sql(sql_req);

	if(banned == 1){
		sql_req = NULL;
		asprintf(&sql_req, "UPDATE room_member SET permissions = '%d' WHERE client_id = '%d' AND room_id = '%d'", TYPE_USER, user_id, chat_id);
		mx_exec_sql(sql_req);
	}

	cJSON* response = cJSON_CreateObject();

	cJSON_AddNumberToObject(response, "rtype", OK_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	free(response_str);

}

