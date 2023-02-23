#include "../../inc/server.h"

void mx_handle_add_room_member(client_t* client, request_t* req) {

	int room_id = cJSON_GetObjectItem(req->json, "room_id")->valueint;

	//TODO: handle errors
	mx_add_room_member(client->user_id, room_id);

	cJSON* response = cJSON_CreateObject();

	cJSON_AddNumberToObject(response, "rtype", ADD_CHAT_MEM_SUCCESS_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
	free(response_str);
}
