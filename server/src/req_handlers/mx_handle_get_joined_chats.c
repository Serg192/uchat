#include "../../inc/server.h"


void mx_handle_get_joined_chats(client_t* client, request_t* req __attribute__((unused))) {
	mx_log(SERV_LOG_FILE, LOG_TRACE, "Get joined chats request");

	printf("Checkpoint one\n");
	
	cJSON* response = cJSON_CreateObject();

	printf("User id : %d\n", client->user_id);

	char* sql_req = NULL;

	asprintf(&sql_req, "SELECT * FROM 'room' WHERE room.id IN (SELECT room_member.room_id FROM 'room_member' WHERE client_id = '%d')", client->user_id );

	printf("Checkpoint 2\n");
	mx_log(SERV_LOG_FILE, LOG_TRACE, "exec sql req");

	mx_get_chats_array(response, sql_req);

	free(sql_req);

	cJSON_AddNumberToObject(response, "rtype", JOINED_CHATS_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	//mx_strdel(response_str);
	free(response_str);
}

