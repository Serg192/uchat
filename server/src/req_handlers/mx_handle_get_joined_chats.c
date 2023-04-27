#include "../../inc/server.h"


void mx_handle_get_joined_chats(client_t* client, request_t* req) {
    //added to prevent warning
    (void)req;
	mx_log(SERV_LOG_FILE, LOG_TRACE, "Get joined chats request");

	
	cJSON* response = cJSON_CreateObject();

	char* sql_req = sqlite3_mprintf("SELECT * FROM 'room' WHERE room.id IN (SELECT room_member.room_id FROM 'room_member' WHERE client_id = '%d')", client->user_id );

	mx_log(SERV_LOG_FILE, LOG_TRACE, "exec sql req");

	mx_get_chats_array(response, sql_req);

	sqlite3_free(sql_req);

	cJSON_AddNumberToObject(response, "rtype", JOINED_CHATS_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	//mx_strdel(response_str);
	free(response_str);
}

