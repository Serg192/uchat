#include "../../inc/server.h"


static inline void notify_all_members(client_t* client,
	                                  const int chat_id,
	                                  const int msg_id,
	                                  char* text){
	//added to prevent warning
    (void)client;
	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	char* sql_req = NULL;
	asprintf(&sql_req, "SELECT * FROM room_member WHERE room_id = '%d'", chat_id);
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }


    while(sqlite3_step(stmt) == SQLITE_ROW) {
    	int client_id = sqlite3_column_int64(stmt, 0);
    	sqlite3_stmt* inner_stmt;
    	asprintf(&sql_req, "SELECT * FROM user WHERE id = '%d'", client_id);

    	if (sqlite3_prepare_v2(db, sql_req, -1, &inner_stmt, 0) != SQLITE_OK) {
	 		mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)sqlite3_errmsg(db));
        	mx_close_db(db);
        	exit(-1);
    	}

    	while(sqlite3_step(inner_stmt) == SQLITE_ROW) {
    	
    		const int key = sqlite3_column_int64(inner_stmt, 0);
    		
    		client_t* client_to_notify = (client_t*)mx_map_get(client_map, key);

    		if(client_to_notify != NULL){
    			msg_edit_data_t* msg_e = (msg_edit_data_t*)malloc(sizeof(msg_edit_data_t));
    			msg_e->id = msg_id;
    			msg_e->text = mx_strdup(text);
    			mx_queue_push(client_to_notify->edited_msg_notify_q, msg_e);

    			printf("EDITED: notify: id: %d, text:%s\n", msg_id, text);
    		}
    		
    	}

    	sqlite3_finalize(inner_stmt);

    }


    free(sql_req);
    sqlite3_finalize(stmt);

    pthread_mutex_unlock(&db_mutex);
    mx_close_db(db);
}

/*
static inline void notify_all_members(client_t* client,
	                                  const int chat_id, 
	                                  const int msg_id){
	
	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	char* sql_req = NULL;
	asprintf(&sql_req, "SELECT * FROM room_member WHERE room_id = '%d'", chat_id);
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql_req, -1, &stmt, 0) != SQLITE_OK) {
	 	mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        mx_close_db(db);
        exit(-1);
    }


    while(sqlite3_step(stmt) == SQLITE_ROW) {
    	int client_id = sqlite3_column_int64(stmt, 0);
    	sqlite3_stmt* inner_stmt;
    	asprintf(&sql_req, "SELECT * FROM user WHERE id = '%d'", client_id);

    	if (sqlite3_prepare_v2(db, sql_req, -1, &inner_stmt, 0) != SQLITE_OK) {
	 		mx_log(SERV_LOG_FILE, LOG_ERROR, sqlite3_errmsg(db));
        	mx_close_db(db);
        	exit(-1);
    	}

    	while(sqlite3_step(inner_stmt) == SQLITE_ROW) {
    	
    		const int key = sqlite3_column_int64(inner_stmt, 0);
    		
    		client_t* client_to_notify = (client_t*)mx_map_get(client_map, key);

    		if(client_to_notify != NULL){
    			mx_queue_push(client_to_notify->deleted_msg_notify_q, (void*)msg_id);
    		}
    		
    	}

    	sqlite3_finalize(inner_stmt);

    }


    free(sql_req);
    sqlite3_finalize(stmt);

    pthread_mutex_unlock(&db_mutex);
    mx_close_db(db);
}


void mx_handle_delete_chat_msg(client_t* client, request_t* req) {

	

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling delete message request");

	const int msg_id = cJSON_GetObjectItem(req->json, "id")->valueint;
	const int chat_id = cJSON_GetObjectItem(req->json, "chat_id")->valueint;

	char* sql_req = NULL;

	asprintf(&sql_req, "DELETE FROM 'message' WHERE message.id = '%d'", msg_id);


	//TODO: check if success
	mx_exec_sql(sql_req);

	notify_all_members(client, chat_id, msg_id);

	mx_log(SERV_LOG_FILE, LOG_TRACE, "SQL request was executed");
	cJSON* response = cJSON_CreateObject();

	cJSON_AddNumberToObject(response, "rtype", OK_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	//mx_strdel(response_str);
	free(response_str);

}

*/

void mx_handle_msg_edit(client_t* client, request_t* req){
	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling edit message");

	const int chat_id = cJSON_GetObjectItem(req->json, "chat_id")->valueint;
	const int message_id = cJSON_GetObjectItem(req->json, "message_id")->valueint;
	char* new_text = cJSON_GetObjectItem(req->json, "text")->valuestring;


	if(mx_check_if_user_is_in_ban(chat_id, client->user_id)){
		return;
	}

	char* sql_req = NULL;

	asprintf(&sql_req, "UPDATE message SET context = '%s' WHERE id = '%d'", new_text, message_id);

	mx_exec_sql(sql_req);

	notify_all_members(client, chat_id, message_id, new_text);

	mx_log(SERV_LOG_FILE, LOG_TRACE, "SQL request was executed");
	cJSON* response = cJSON_CreateObject();

	cJSON_AddNumberToObject(response, "rtype", OK_RESP);

	char* response_str = cJSON_PrintUnformatted(response);

	mx_send_response(client, response_str);

	cJSON_Delete(response);

	//mx_strdel(response_str);
	free(response_str);

}
