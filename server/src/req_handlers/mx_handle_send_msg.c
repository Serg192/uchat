#include "../../inc/server.h"


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

void mx_handle_send_msg(client_t* client, request_t* req) {


	const int from_id = cJSON_GetObjectItem(req->json, "from_id")->valueint;
	const int room_id = cJSON_GetObjectItem(req->json, "room_id")->valueint;
	const int sent_date = cJSON_GetObjectItem(req->json, "sent_date")->valueint;
	const int sent_time = cJSON_GetObjectItem(req->json, "sent_time")->valueint;
	const char* context = cJSON_GetObjectItem(req->json, "context")->valuestring;


	//TODO: check if received data are valid

	//

	create_msg(from_id, room_id, sent_date, sent_time, context);

	


	//30122022| 12:34:33
}
