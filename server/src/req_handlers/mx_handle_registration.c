#include "../../inc/server.h"

static inline bool add_user(const char* username, const char* password){
	mx_exec_sql(sqlite3_mprintf("INSERT INTO 'user' ('username', 'password') VALUES ('%s','%s')", username, password));
	return false;
}

void mx_handle_registration(client_t* client, request_t* req) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling registration");

	const char* username = cJSON_GetObjectItem(req->json, "login")->valuestring;
	const char* password = cJSON_GetObjectItem(req->json, "password")->valuestring;

	mx_log(SERV_LOG_FILE, LOG_TRACE, (char*)username);
	mx_log(SERV_LOG_FILE, LOG_TRACE, (char*)password);

	int username_len = mx_strlen(username);

	cJSON* response = cJSON_CreateObject();

	char* sql_req = sqlite3_mprintf("SELECT * FROM user WHERE username = '%s'", username);
	
	if(username_len <= 3 || username_len >= 32) {
		mx_log(SERV_LOG_FILE, LOG_TRACE, "login <= 3 | >= 32");
		cJSON_AddNumberToObject(response, "rtype", SIGNUP_ERR_RESP);
		cJSON_AddStringToObject(response, "message", "Username must be 4 - 31 characters long");
	}
	
	else if(!mx_check_if_row_exists(sql_req)) {
		    mx_log(SERV_LOG_FILE, LOG_TRACE, "add user");
			add_user(username, password);
			cJSON_AddNumberToObject(response, "rtype", SIGNUP_SUCCESS_RESP);
			client->username = mx_strdup(username);
			client->user_id = mx_atoi(mx_get_str_field_val("user", "id", "username", username));

			mx_map_put(client_map, client->user_id, client);


	} else {
		mx_log(SERV_LOG_FILE, LOG_TRACE, "user exist");
		cJSON_AddNumberToObject(response, "rtype", SIGNUP_ERR_RESP);
		cJSON_AddStringToObject(response, "message", "This username already exists");
	}

	char* response_str = cJSON_PrintUnformatted(response);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
}
