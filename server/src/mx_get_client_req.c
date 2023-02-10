#include "../inc/server.h"


static inline int get_req_len(SSL* ssl) {

	char buffer[REQ_LEN_BUFFER];

	int bytes_was_read =  SSL_read(ssl, buffer, REQ_LEN_BUFFER);

	buffer[bytes_was_read] = '\0';

  return atoi(buffer);
}

static inline void read_client_req(request_t* client_req, SSL* ssl, int req_length) {

	char* request = malloc(req_length + 1);
	SSL_read(ssl, request, req_length);
	request[req_length] = '\0';

	client_req->str_req = request;
}

request_t* mx_get_client_req(client_t* client) {


	int req_length = get_req_len(client->ssl);

	if(req_length == 0)
		return NULL;

	request_t* client_req = malloc(sizeof(request_t));
	read_client_req(client_req, client->ssl, req_length);

	printf("Client request: %s\n", client_req->str_req);

	return client_req;
}