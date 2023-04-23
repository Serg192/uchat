#include "../../inc/server.h"

//"DELETE FROM room_member WHERE client_id = ? AND room_id = ?";
void mx_handle_leave_chat(client_t* client, request_t* req){
	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling leave chat request");

	const int room_id = cJSON_GetObjectItem(req->json, "chat_id")->valueint;
	const int client_id = client->user_id;

	printf("room_id is %d, client_id is %d\n", room_id, client_id);

	char* sql_req = NULL;
	asprintf(&sql_req, "DELETE FROM room_member WHERE client_id = '%d' AND room_id = '%d'", client_id, room_id);

	printf("Executing sql_req\n");
	mx_exec_sql(sql_req);
	printf("Success\n");


	cJSON* response = cJSON_CreateObject();

	cJSON_AddNumberToObject(response, "rtype", OK_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
	free(response_str);
	free(sql_req);
}

