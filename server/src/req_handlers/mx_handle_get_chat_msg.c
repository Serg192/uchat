#include "../../inc/server.h"

static inline cJSON* build_deleted_msg_array(client_t* client){
	cJSON* deleted_msg_array = cJSON_CreateArray();

	while(!client->deleted_msg_notify_q->empty){
		int* deleted_msg_id = (int*)mx_queue_peek(client->deleted_msg_notify_q);

		cJSON* array_item = cJSON_CreateObject();
		cJSON_AddNumberToObject(array_item, "msg_id", *deleted_msg_id);
		cJSON_AddItemToArray(deleted_msg_array, array_item);
		mx_queue_pop(client->deleted_msg_notify_q);
		free(deleted_msg_id);
	}


	return deleted_msg_array;
}

static inline cJSON* build_edited_msg_array(client_t* client){
	cJSON* edited_msg_array = cJSON_CreateArray();

	while(!client->edited_msg_notify_q->empty){
		msg_edit_data_t* data = (msg_edit_data_t*)mx_queue_peek(client->edited_msg_notify_q);

		cJSON* array_item = cJSON_CreateObject();
		cJSON_AddNumberToObject(array_item, "msg_id", data->id);
		cJSON_AddStringToObject(array_item, "text", data->text);

		cJSON_AddItemToArray(edited_msg_array, array_item);
		mx_queue_pop(client->edited_msg_notify_q);
		free(data->text);
		free(data);
	}


	return edited_msg_array;
}

static inline int find_last_msg_id(const int chat_id) {
    //added to prevent warning
    (void)chat_id;
	int result = 0;
	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	sqlite3_stmt* stmt = mx_prepare_stmt(db, sqlite3_mprintf("SELECT message.id FROM 'message' ORDER BY message.id DESC LIMIT 1"));

  if(sqlite3_step(stmt) == SQLITE_ROW){
    result = sqlite3_column_int64(stmt, 0);
  }

  sqlite3_finalize(stmt);

  pthread_mutex_unlock(&db_mutex);
  mx_close_db(db);

  return result;
}

static inline cJSON* make_message_item(const int own_id, sqlite3_stmt* stmt){
	cJSON* message = cJSON_CreateObject();

	const int from_id = sqlite3_column_int64(stmt, 1);
    cJSON_AddNumberToObject(message, "id", sqlite3_column_int64(stmt, 0));
    cJSON_AddNumberToObject(message, "from_id", from_id);
    cJSON_AddNumberToObject(message, "sending_date", sqlite3_column_int64(stmt, 2));
    cJSON_AddNumberToObject(message, "sending_time", sqlite3_column_int64(stmt, 3));
    cJSON_AddStringToObject(message, "context", (const char*)sqlite3_column_text(stmt, 4));

    const char* username = (const char*)sqlite3_column_text(stmt, 5);
    username = username == NULL ? "Deleted" : username;
    cJSON_AddStringToObject(message, "username", (own_id == from_id ? "You" : username));

    return message;
}


/*
static inline char* build_sql_req(int chat_id, int start_id, int count, int mode){
	char* sql_req = NULL;

	switch(mode) {
		case MSG_LOAD_ALL:
			asprintf(&sql_req, "SELECT message.id, message.from_id, message.sending_date, message.sending_time, message.context, user.username"
					   		   " FROM 'message' INNER JOIN 'user' ON user.id = message.from_id "
					           "WHERE message.room_id = '%d' ORDER BY message.id ASC", chat_id);
			break;
		case MSG_LOAD_BELOW:
			 asprintf(&sql_req, "SELECT message.id, message.from_id, message.sending_date, message.sending_time, message.context, user.username "
                       		    "FROM 'message' INNER JOIN 'user' ON user.id = message.from_id "
                                "WHERE message.room_id = '%d' AND message.id >= %d ORDER BY message.id ASC LIMIT %d", chat_id, start_id, count);
			break;
		case MSG_LOAD_ABOVE:
			  asprintf(&sql_req, "SELECT message.id, message.from_id, message.sending_date, message.sending_time, message.context, user.username "
                       			 "FROM 'message' INNER JOIN 'user' ON user.id = message.from_id "
                       			 "WHERE message.room_id = '%d' AND message.id <= %d ORDER BY message.id DESC LIMIT %d", chat_id, start_id, count);
			break;
		default:
			mx_log(SERV_LOG_FILE, LOG_ERROR, "Invalid mode specified");
	}

	return sql_req;
}
*/

static inline char* build_sql_req(int chat_id, int start_id, int count, int mode){
	char* sql_req = NULL;

	switch(mode) {
		case MSG_LOAD_ALL:
			sql_req = sqlite3_mprintf("SELECT message.id, message.from_id, message.sending_date, message.sending_time, message.context, user.username"
					   		   " FROM 'message' LEFT JOIN 'user' ON user.id = message.from_id "
					           "WHERE message.room_id = '%d' ORDER BY message.id ASC", chat_id);
			break;
		case MSG_LOAD_BELOW:
			sql_req = sqlite3_mprintf("SELECT message.id, message.from_id, message.sending_date, message.sending_time, message.context, user.username "
                       		    "FROM 'message' LEFT JOIN 'user' ON user.id = message.from_id "
                                "WHERE message.room_id = '%d' AND message.id >= %d ORDER BY message.id ASC LIMIT %d", chat_id, start_id, count);
			break;
		case MSG_LOAD_ABOVE:
			
			sql_req = sqlite3_mprintf("SELECT message.id, message.from_id, message.sending_date, message.sending_time, message.context, user.username "
                       			 "FROM 'message' LEFT JOIN 'user' ON user.id = message.from_id "
                       			 "WHERE message.room_id = '%d' AND message.id <= %d ORDER BY message.id DESC LIMIT %d", chat_id, start_id, count);
			break;
		default:
			mx_log(SERV_LOG_FILE, LOG_ERROR, "Invalid mode specified");
	}

	return sql_req;
}
static inline void build_msg_array(int chat_id,
 								   const int own_id,
 								   const int start_id,
 								   const int mode,
 								   const int count,
 								   cJSON* response) {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Building message array");
	cJSON* msg_array = cJSON_CreateArray();

	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	sqlite3_stmt* stmt = mx_prepare_stmt(db, build_sql_req(chat_id, start_id, count, mode));


  while(sqlite3_step(stmt) == SQLITE_ROW){
    cJSON_AddItemToArray(msg_array, make_message_item(own_id, stmt));  
  }
  
  cJSON_AddItemReferenceToObject(response, "messages", msg_array);
  sqlite3_finalize(stmt);
  pthread_mutex_unlock(&db_mutex);
  mx_close_db(db);
}

void mx_handle_get_chat_msg(client_t* client, request_t* req){

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Hadling get chat messages");
	

	const int chat_id = cJSON_GetObjectItem(req->json, "room_id")->valueint;
	int start_id = cJSON_GetObjectItem(req->json, "start_id")->valueint;
	const int mode = cJSON_GetObjectItem(req->json, "mode")->valueint;
	const int count = cJSON_GetObjectItem(req->json, "count")->valueint;

	if(start_id == MSG_ID_LAST){
		start_id = find_last_msg_id(chat_id);
	}
	//const int start_id = cJSON_GetObjectItem(req->json, "start_id")->valueint;
	//const int count = cJSON_GetObjectItem(req->json, "count")->valueint;

	mx_log(SERV_LOG_FILE, LOG_TRACE, "ID");
	mx_log(SERV_LOG_FILE, LOG_TRACE, mx_itoa(chat_id));

	//TODO: handle errors and validate data

	//

	cJSON* response = cJSON_CreateObject();

	build_msg_array(chat_id, client->user_id, start_id, mode, count, response);

	cJSON_AddItemReferenceToObject(response, "deleted", build_deleted_msg_array(client));
	cJSON_AddItemReferenceToObject(response, "edited", build_edited_msg_array(client));
	cJSON_AddNumberToObject(response, "rtype", (mode == MSG_LOAD_BELOW ? GET_BELOW_MSG_RESP : GET_ABOVE_MSG_RESP));


	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
}
