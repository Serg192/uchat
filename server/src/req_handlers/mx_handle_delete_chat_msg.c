#include "../../inc/server.h"

// all the code should be optimized (DRY)

/*

typedef struct cds_hash_tl_entry_s {
    char* key;
    void* value;
    struct cds_hash_tl_entry_s* next;
}              cds_hash_tl_entry_t;


typedef struct cds_hash_tl_s {
    cds_hash_tl_entry_t** entries;
}              cds_hash_tl_t;

*/
//for test


static inline void notify_all_members(client_t* client,
	                                  const int chat_id, 
	                                  const int msg_id){
	//added to prevent warning
    (void)client;
	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	sqlite3_stmt* stmt = mx_prepare_stmt(db, sqlite3_mprintf("SELECT * FROM room_member WHERE room_id = '%d'", chat_id));

    while(sqlite3_step(stmt) == SQLITE_ROW) {
    	int client_id = sqlite3_column_int64(stmt, 0);
    	sqlite3_stmt* inner_stmt = mx_prepare_stmt(db, sqlite3_mprintf("SELECT * FROM user WHERE id = '%d'", client_id));

    	while(sqlite3_step(inner_stmt) == SQLITE_ROW) {
    	
    		const int key = sqlite3_column_int64(inner_stmt, 0);
    		
    		client_t* client_to_notify = (client_t*)mx_map_get(client_map, key);

    		if(client_to_notify != NULL){
    			int* h_msg_id = (int*)malloc(sizeof(int));
    			*h_msg_id = msg_id;
    			mx_queue_push(client_to_notify->deleted_msg_notify_q, h_msg_id);
    		}
    		
    	}

    	sqlite3_finalize(inner_stmt);

    }

    sqlite3_finalize(stmt);

    pthread_mutex_unlock(&db_mutex);
    mx_close_db(db);
}


static inline bool user_has_permission_to_del_message(const int chat_id, const int message_id, const int user_id){

	bool result = false;

	sqlite3* db = mx_open_db();

	pthread_mutex_lock(&db_mutex);

	char* sql_req = sqlite3_mprintf("SELECT rm1.permissions AS user_permission, m.from_id, rm2.permissions AS from_permission"
		               " FROM message AS m JOIN room_member AS rm1 ON rm1.client_id = '%d' AND rm1.room_id = '%d'"
		               " JOIN room_member AS rm2 ON rm2.client_id = m.from_id AND rm2.room_id = '%d' WHERE m.id = '%d'",
		                user_id, chat_id, chat_id, message_id);
	sqlite3_stmt* stmt = mx_prepare_stmt(db, sql_req);

    bool found = false;
    while(sqlite3_step(stmt) == SQLITE_ROW) {
    	const int this_user_permissions = sqlite3_column_int64(stmt, 0);
    	const int message_from_user_id = sqlite3_column_int64(stmt, 1);
    	const int message_owner_permissions = sqlite3_column_int64(stmt, 2);

    	if(user_id == message_from_user_id || this_user_permissions > message_owner_permissions){
    		result = true;
    	}

    	found = true;
    }

    if(!found){
    	
    	//message from user who left this chat
    	char* sql_req2 = sqlite3_mprintf("SELECT room_member.permissions FROM room_member "
    		                " WHERE room_member.room_id = '%d' AND room_member.client_id = '%d'",
    		                chat_id, user_id);

    	sqlite3_stmt* stmt2 = mx_prepare_stmt(db, sql_req2);

    	while(sqlite3_step(stmt2) == SQLITE_ROW){
    		const int this_user_permissions = sqlite3_column_int64(stmt2, 0);

    		if(this_user_permissions >= TYPE_ADMIN)
    			result = true;
    	}
    
        sqlite3_finalize(stmt2);
    }

    sqlite3_finalize(stmt);

    pthread_mutex_unlock(&db_mutex);
    mx_close_db(db);

    return result;
}

void mx_handle_delete_chat_msg(client_t* client, request_t* req) {
    //added to prevent warning
    (void)client;

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Handling delete message request");

	const int msg_id = cJSON_GetObjectItem(req->json, "id")->valueint;
	const int chat_id = cJSON_GetObjectItem(req->json, "chat_id")->valueint;

	if(mx_check_if_user_is_in_ban(chat_id, client->user_id))
		return;

	char* sql_req = NULL;

	//TODO: check if user has permission to delete this message
	if(!user_has_permission_to_del_message(chat_id, msg_id, client->user_id)){
		return;
	}

	sql_req = sqlite3_mprintf("DELETE FROM 'message' WHERE message.id = '%d'", msg_id);

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

