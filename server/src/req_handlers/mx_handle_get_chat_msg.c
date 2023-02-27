#include "../../inc/server.h"

static inline cJSON* make_message_item(sqlite3_stmt* stmt){
	cJSON* message = cJSON_CreateObject();

    cJSON_AddNumberToObject(message, "id", sqlite3_column_int64(stmt, 0));
    cJSON_AddNumberToObject(message, "from_id", sqlite3_column_int64(stmt, 1));
    cJSON_AddNumberToObject(message, "sending_date", sqlite3_column_int64(stmt, 2));
    cJSON_AddNumberToObject(message, "sending_time", sqlite3_column_int64(stmt, 3));
    cJSON_AddStringToObject(message, "context", sqlite3_column_text(stmt, 4));
    cJSON_AddStringToObject(message, "username", sqlite3_column_text(stmt, 5));

    return message;
}

static inline void build_msg_array(int chat_id, cJSON* response) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Building message array");
	cJSON* msg_array = cJSON_CreateArray();

	sqlite3* db = mx_open_db();

	char* sql_req = NULL;

	asprintf(&sql_req, "SELECT message.id, message.from_id, message.sending_date, message.sending_time, message.context, user.username"
					   " FROM 'message' INNER JOIN 'user' ON user.id = message.from_id "
					   "WHERE message.room_id = '%d' ORDER BY message.id ASC", chat_id);

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }


    while(sqlite3_step(stmt) == SQLITE_ROW){
    	cJSON_AddItemToArray(msg_array, make_message_item(stmt));
    }

    cJSON_AddItemReferenceToObject(response, "messages", msg_array);

    free(sql_req);
    sqlite3_finalize(stmt);
    mx_close_db(db);
}

void mx_handle_get_chat_msg(client_t* client, request_t* req){

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Hadling get chat messages");
	
	//flag
	// if flag == 0 send all messages in the chat
	// if flag == -1 send messages that are above the start_id
	// if flag == 1 send messages that are under the start_id
	// if flag == 2 send last messages
	// count - messages to send 

	const int chat_id = cJSON_GetObjectItem(req->json, "room_id")->valueint;
	//const int start_id = cJSON_GetObjectItem(req->json, "start_id")->valueint;
	//const int count = cJSON_GetObjectItem(req->json, "count")->valueint;

	mx_log(SERV_LOG_FILE, LOG_TRACE, "ID");
	mx_log(SERV_LOG_FILE, LOG_TRACE, mx_itoa(chat_id));

	//TODO: handle errors and validate data

	//

	cJSON* response = cJSON_CreateObject();

	build_msg_array(chat_id, response);

	cJSON_AddNumberToObject(response, "rtype", GET_CHAT_MSG_RES);


	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
}
