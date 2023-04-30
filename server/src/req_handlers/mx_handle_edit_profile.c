#include "../../inc/server.h"

//"UPDATE user SET username = '%s', password = '%s' WHERE id = %d;", username, password, user_id

void mx_handle_edit_profile(client_t* client, request_t* req){
	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling edit profile");

	const char* new_username = cJSON_GetObjectItem(req->json, "username")->valuestring;
	const char* new_password = cJSON_GetObjectItem(req->json, "password")->valuestring;

	const char* error_msg = "Login is currently in use";

	cJSON* response = cJSON_CreateObject();

	char* sql_req = sqlite3_mprintf("SELECT * from 'user' WHERE username = '%s'", new_username);

	if(!mx_check_if_row_exists(sql_req)){
		sql_req = sqlite3_mprintf("UPDATE user SET username = '%s', password = '%s' WHERE id = '%d'", new_username, new_password, client->user_id);
		mx_exec_sql(sql_req);
		
		cJSON_AddNumberToObject(response, "rtype", PROFILE_EDIT_OK_RESP);
		free(client->username);
		client->username = mx_strdup(new_username);

	} else if (mx_strcmp(client->username, new_username) == 0) {
		sql_req = sqlite3_mprintf("UPDATE user SET  password = '%s' WHERE id = '%d'", new_password, client->user_id);
		mx_exec_sql(sql_req);
		
		cJSON_AddNumberToObject(response, "rtype", PROFILE_EDIT_OK_RESP);
	}else {
		cJSON_AddNumberToObject(response, "rtype", PROFILE_EDIT_ERR_RESP);
		cJSON_AddStringToObject(response, "message", error_msg);
	}


	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
	free(response_str);
}
