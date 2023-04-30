#include "../../inc/server.h"

static inline bool check_password_for(const char* username, const char* password) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Checking password");

	bool result = false;

	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	sqlite3_stmt* stmt = mx_prepare_stmt(db, sqlite3_mprintf("SELECT * FROM 'user' WHERE username = '%s'", username));

    if(sqlite3_step(stmt) == SQLITE_ROW) {
    	const char* db_pass = mx_strdup((const char*)sqlite3_column_text(stmt, 2));
    	result = mx_strcmp(db_pass, password) == 0;
    	mx_strdel((char**)&db_pass);
    
    }

    sqlite3_finalize(stmt);

    pthread_mutex_unlock(&db_mutex);
    mx_close_db(db);

    return result;
}


void mx_handle_logging_in(client_t* client, request_t* req) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling logging in");

	const char* username = cJSON_GetObjectItem(req->json, "login")->valuestring;
	const char* password = cJSON_GetObjectItem(req->json, "password")->valuestring;
	const char* error_msg = "The username or password is incorrect";

	mx_log(SERV_LOG_FILE, LOG_TRACE, (char*)username);
	mx_log(SERV_LOG_FILE, LOG_TRACE, (char*)password);

	cJSON* response = cJSON_CreateObject();

	char* sql_req = sqlite3_mprintf("SELECT * FROM 'user' WHERE username = '%s'", username);
	//asprintf(&sql_req, "SELECT * FROM 'user' WHERE username = '%s'", username);

	if(!mx_check_if_row_exists(sql_req)) {
		cJSON_AddNumberToObject(response, "rtype", LOGIN_ERR_RESP);
		cJSON_AddStringToObject(response, "message", error_msg);
		mx_log(SERV_LOG_FILE, LOG_TRACE, "There is no such user");
	} else if(!check_password_for(username, password)){
		cJSON_AddNumberToObject(response, "rtype", LOGIN_ERR_RESP);
		cJSON_AddStringToObject(response, "message", error_msg);
		mx_log(SERV_LOG_FILE, LOG_TRACE, "Password is incorrect");
	} else {
		cJSON_AddNumberToObject(response, "rtype", LOGIN_SUCESS_RESP);
		mx_log(SERV_LOG_FILE, LOG_TRACE, "Everything is fine");
		client->username = mx_strdup(username);
		client->user_id = mx_atoi(mx_get_str_field_val("user", "id", "username", username));

		mx_map_put(client_map, client->user_id, client);
	}

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
	free(response_str);
}

