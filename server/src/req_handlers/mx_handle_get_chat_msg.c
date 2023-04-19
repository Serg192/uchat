#include "../../inc/server.h"

static inline cJSON* build_deleted_msg_array(client_t* client){
	cJSON* deleted_msg_array = cJSON_CreateArray();

	while(!client->deleted_msg_notify_q->empty){
		int deleted_msg_id = (int)mx_queue_peek(client->deleted_msg_notify_q);

		cJSON* array_item = cJSON_CreateObject();
		cJSON_AddNumberToObject(array_item, "msg_id", deleted_msg_id);
		cJSON_AddItemToArray(deleted_msg_array, array_item);
		mx_queue_pop(client->deleted_msg_notify_q);
	}


	return deleted_msg_array;
}

static inline int find_last_msg_id(const int chat_id) {
	int result;
	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	char* sql_req = "SELECT message.id FROM 'message' ORDER BY message.id DESC LIMIT 1";

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

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
    cJSON_AddStringToObject(message, "context", sqlite3_column_text(stmt, 4));
    cJSON_AddStringToObject(message, "username", (own_id == from_id ? "You" : sqlite3_column_text(stmt, 5)));

    return message;
}

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

	char* sql_req = build_sql_req(chat_id, start_id, count, mode);

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }

  //  t_list* list = NULL;

    while(sqlite3_step(stmt) == SQLITE_ROW){
    	const int this_id = sqlite3_column_int64(stmt, 0);

    //	if(mode == MSG_LOAD_BELOW){
    //		mx_push_back(&list, make_message_item(own_id, stmt));
    //	} else {
    		cJSON_AddItemToArray(msg_array, make_message_item(own_id, stmt));
    //	}
    }

    //if(mode == MSG_LOAD_BELOW){
    //	while(list != NULL){
    //		cJSON* tmp_json = (cJSON*)list->data;
    //		cJSON_AddItemToArray(msg_array, tmp_json);
    //		mx_pop_front(&list);
    //	}
    //}


    cJSON_AddItemReferenceToObject(response, "messages", msg_array);

    free(sql_req);
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
	cJSON_AddNumberToObject(response, "rtype", (mode == MSG_LOAD_BELOW ? GET_BELOW_MSG_RESP : GET_ABOVE_MSG_RESP));


	char* response_str = cJSON_PrintUnformatted(response);

	mx_log(SERV_LOG_FILE, LOG_TRACE, response_str);

	mx_send_response(client, response_str);

	cJSON_Delete(response);
}
