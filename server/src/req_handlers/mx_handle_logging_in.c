#include "../../inc/server.h"

static inline bool check_password_for(const char* username, const char* password) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Checking password");

	bool result;
	char* sql_req = NULL;

	asprintf(&sql_req, "SELECT * FROM 'user' WHERE username = '%s'", username);

	sqlite3* db = mx_open_db();

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

    if(sqlite3_step(stmt) == SQLITE_ROW) {
    	const char* db_pass = mx_strdup((const char*)sqlite3_column_text(stmt, 2));
    	result = mx_strcmp(db_pass, password) == 0;
    	mx_strdel(&db_pass);
    
    }

    free(sql_req);
    sqlite3_finalize(stmt);
    mx_close_db(db);

    return result;
}

void mx_handle_logging_in(client_t* client, request_t* req) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling logging in");

	const char* username = cJSON_GetObjectItem(req->json, "login")->valuestring;
	const char* password = cJSON_GetObjectItem(req->json, "password")->valuestring;
	const char* error_msg = "The username or password is incorrect";

	mx_log(SERV_LOG_FILE, LOG_TRACE, username);
	mx_log(SERV_LOG_FILE, LOG_TRACE, password);

	cJSON* response = cJSON_CreateObject();

	char* sql_req;
	asprintf(&sql_req, "SELECT * FROM 'user' WHERE username = '%s'", username);

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
		client->username = username;
		client->user_id = mx_atoi(mx_get_str_field_val("user", "id", "username", username));
	}

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
	free(response_str);
}