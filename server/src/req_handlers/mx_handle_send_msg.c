#include "../../inc/server.h"

//DATE FORMAT IS YY|MM|DATE
//TIME FORMAT IS HH|MM|SS in 24 hour format

void mx_handle_send_msg(client_t* client, request_t* req) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling send msg request");

	const int from_id =  client->user_id;//cJSON_GetObjectItem(req->json, "from_id")->valueint;
	const int room_id = cJSON_GetObjectItem(req->json, "room_id")->valueint;


	//check if user is in ban

	if(!mx_check_if_user_is_in_ban(room_id, from_id)){
		//const int sending_date = cJSON_GetObjectItem(req->json, "sending_date")->valueint;
		//const int sending_time = cJSON_GetObjectItem(req->json, "sending_time")->valueint;
		const char* context = cJSON_GetObjectItem(req->json, "context")->valuestring;

		mx_log(SERV_LOG_FILE, LOG_TRACE, "Date and time get");

		date_and_time_t dt;
		mx_get_date_time(&dt);

		const int sending_time = mx_format_time(&dt);
		const int sending_date = mx_format_date(&dt);

		char* sql_req = sqlite3_mprintf("INSERT INTO 'message' ('from_id', 'room_id', 'sending_date', 'sending_time', 'context') VALUES ('%d', '%d', '%d', '%d', '%s')",
	        	 from_id, room_id, sending_date, sending_time, context);

		mx_exec_sql(sql_req);
	}



	

	cJSON* response = cJSON_CreateObject();

	cJSON_AddNumberToObject(response, "rtype", MSG_SEND_SUCCESS_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
	free(response_str);
}
