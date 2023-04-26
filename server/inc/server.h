#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sqlite3.h>
#include <cJSON.h>

#include <libmx.h>

#include "../../common/common.h"

#define REQ_LEN_BUFFER 128
#define SSL_SERT_FILE "server/cert.pem"
#define SSL_KEY_FILE "server/cert.pem"

#define SERV_LOG_FILE "server/log/log.txt"
#define DATABASE_FILE "server/db/database.db"

pthread_mutex_t db_mutex;

//int Map



//

map_t* client_map;

typedef struct request_s {
	int type;
	char* str_req;
	cJSON* json;
}              request_t;


typedef struct client_s {
	int sock;
	SSL* ssl;
	const char* username;
	int user_id;

	queue_t* deleted_msg_notify_q;
	queue_t* edited_msg_notify_q;
}              client_t;

typedef struct ossl_s {
	SSL* ssl;
	SSL_CTX* ctx;
}                ossl_t;

typedef struct get_msg_params_s {
	int chat_id;
	int mode;
	int start_id;
	int count; 
	int recipient; 
}              get_msg_params_t;

//-----DATE AND TIME SECTION ---------------
typedef struct date_and_time_s {
	int day;
	int month;
	int year;

	int hour;
	int min;
}              date_and_time_t;

typedef struct msg_edit_data_s {
	int id;
	char* text;
}              msg_edit_data_t;


void mx_get_date_time(date_and_time_t* dt);

int mx_format_time(date_and_time_t* dt);

int mx_format_date(date_and_time_t* dt);

//------------------------------------------

void mx_init_openssl(ossl_t* ossl);

void mx_run_serv(int s_sock);

void* mx_client_handler(void* client);

request_t* mx_get_client_req(client_t* client);

void mx_send_response(client_t* client, const char* response);

//database utils

sqlite3* mx_open_db();

void mx_close_db(sqlite3* db);

void mx_init_database();

void mx_exec_sql(char* sql_req);

bool mx_check_if_row_exists(char* sql_req);

bool mx_sql_ex(const char* sql_req);

char* mx_get_str_field_val(const char* table, const char* fieldname, const char* key, const char* val);

void mx_add_room_member(int client_id, int room_id, int perm);


//handlers

void mx_get_chats_array(cJSON* parent_json, const char* search_condition);

void mx_handle_registration(client_t* client, request_t* req);

void mx_handle_logging_in(client_t* client, request_t* req);

void mx_handle_chat_creation(client_t* client, request_t* req);

void mx_handle_add_room_member(client_t* client, request_t* req);

void mx_handle_send_msg(client_t* client, request_t* req);

void mx_handle_chat_search(client_t* client, request_t* req);

void mx_handle_get_chat_participants(client_t* client, request_t* req);

void mx_handle_get_chat_msg(client_t* client, request_t* req);

void mx_handle_delete_chat_msg(client_t* client, request_t* req);

void mx_handle_get_joined_chats(client_t* client, request_t* req);

void mx_handle_msg_edit(client_t* client, request_t* req);

void mx_handle_leave_chat(client_t* client, request_t* req);

void mx_handle_del_account(client_t* client, request_t* req);

void mx_handle_ban(client_t* client, request_t* req);

void mx_handle_perm_change(client_t* client, request_t* req);

int asprintf(char **strp, const char *fmt, ...);

bool mx_check_if_user_is_in_ban(const int chat_id, const int user_id);

#endif

