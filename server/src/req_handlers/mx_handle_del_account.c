#include "../../inc/server.h"

void mx_handle_del_account(client_t* client, request_t* req){

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling delete message request");


	char* sql_req = NULL;

	asprintf(&sql_req, "DELETE FROM user WHERE id = '%d'", client->user_id);


	//TODO: check if success
	mx_exec_sql(sql_req);

	if(client->user_id != -1)
		mx_map_remove(client_map, client->user_id);


	mx_log(SERV_LOG_FILE, LOG_TRACE, "SQL request was executed");
	cJSON* response = cJSON_CreateObject();

	cJSON_AddNumberToObject(response, "rtype", OK_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	//mx_strdel(response_str);
	free(response_str);
}
