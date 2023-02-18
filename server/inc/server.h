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
}              client_t;

typedef struct ossl_s {
	SSL* ssl;
	SSL_CTX* ctx;
}                ossl_t;


void mx_init_openssl(ossl_t* ossl);

void mx_run_serv(int s_sock);

void* mx_client_handler(void* client);

request_t* mx_get_client_req(client_t* client);

void mx_send_response(client_t* client, const char* response);

//database utils

sqlite3* mx_open_db();

void mx_close_db(sqlite3* db);

void mx_init_database();

bool mx_table_has_user(const char* login);


//handlers

void mx_handle_registration(client_t* client, request_t* req);

void mx_handle_logging_in(client_t* client, request_t* req);

#endif

