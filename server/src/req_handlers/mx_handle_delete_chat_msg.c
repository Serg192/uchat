#include "../../inc/server.h"

void mx_handle_delete_chat_msg(client_t* client, request_t* req) {


	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling delete message request");

	const int msg_id = cJSON_GetObjectItem(req->json, "id")->valuestring;

	char* sql_req = NULL;

	sprintf(&sql_req, "DELETE FROM 'message' WHERE id = '%d'", msg_id);


	//TODO: check if success
	mx_exec_sql(sql_req);

	cJSON* response = cJSON_CreateObject();

	cJSON_AddNumberToObject(response, "rtype", OK_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	//mx_strdel(response_str);
	free(response_str);

}