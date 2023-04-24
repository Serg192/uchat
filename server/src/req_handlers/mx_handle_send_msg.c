#include "../../inc/server.h"

/*
static inline void create_msg(const int from_id,
	                          const int room_id,
	                          const int sent_date,
	                          const int sent_time,
	                          const char* context) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Saving msg to database");

	char* sql_req = NULL;

	asprintf(&sql_req, 
		    "INSERT INTO 'message' ('from_id', 'room_id', 'sent_date', 'sent_time', 'context') VALUES ('%d', '%d', '%d', '%d', '%s')",
	         from_id, room_id, sent_date, sent_time, context);

	sqlite3* db = mx_open_db();

	 char* err;
	 if (sqlite3_exec(db, sql_req, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Failed to execute sql: %s", err);
        mx_close_db(db);
        exit(-1);
    }


    free(sql_req);
	mx_close_db(db);

}
*/

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

		//mx_logf(SERV_LOG_FILE, LOG_TRACE, "Date => %d, Time => %d", sending_date, sending_time);

		//TODO: check if received data are valid

		//

		//form sql request

		char* sql_req;
		asprintf(&sql_req, 
		    	"INSERT INTO 'message' ('from_id', 'room_id', 'sending_date', 'sending_time', 'context') VALUES ('%d', '%d', '%d', '%d', '%s')",
	        	 from_id, room_id, sending_date, sending_time, context);


		//create_msg(from_id, room_id, sent_date, sent_time, context);

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
