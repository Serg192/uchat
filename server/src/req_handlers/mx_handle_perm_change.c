#include "../../inc/server.h"


void mx_handle_perm_change(client_t* client, request_t* req){
	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling permission change");

	const int chat_id = cJSON_GetObjectItem(req->json, "chat_id")->valueint;
	const int user_id = cJSON_GetObjectItem(req->json, "user_id")->valueint;
	const int permissions = cJSON_GetObjectItem(req->json, "permissions")->valueint;

	char* sql_req = NULL;
	asprintf(&sql_req, "UPDATE room_member SET permissions = '%d' WHERE client_id = '%d' AND room_id = '%d'", permissions, user_id, chat_id);
	mx_exec_sql(sql_req);

	cJSON* response = cJSON_CreateObject();

	cJSON_AddNumberToObject(response, "rtype", OK_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	free(response_str);
	
}
