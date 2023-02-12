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
#include <cJSON.h>

#include <libmx.h>

#include "../../common/common.h"

#define REQ_LEN_BUFFER 128
#define SSL_SERT_FILE "server/cert.pem"
#define SSL_KEY_FILE "server/cert.pem"

#define SERV_LOG_FILE "server/log/log.txt"

typedef struct request_s {
	request_type type;
	char* str_req;
}              request_t;


typedef struct client_s {
	int sock;
	SSL* ssl;
}              client_t;

typedef struct ossl_s {
	SSL* ssl;
	SSL_CTX* ctx;
}                ossl_t;


void mx_init_openssl(ossl_t* ossl);

void mx_run_serv(int s_sock);

void mx_client_handler(void* client);

request_t* mx_get_client_req(client_t* client);

#endif

