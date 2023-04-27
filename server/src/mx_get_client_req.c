#include "../inc/server.h"

static inline int get_req_len(SSL* ssl) {

    char buffer[REQ_LEN_BUFFER];
    int bytes_was_read = 0;

    while((bytes_was_read = SSL_read(ssl, buffer, REQ_LEN_BUFFER)) <= 0) {
    	if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
            continue;
        }
        return 0;
    }

    return atoi(buffer);
}

static inline void read_client_req(request_t* client_req, SSL* ssl, int req_length) {

	int bytes_were_read = 0;
	char* request = malloc(req_length + 1);

	while(bytes_were_read != req_length) {
		
		int bytes = SSL_read(ssl, request + bytes_were_read, req_length - bytes_were_read);

		if(bytes <= 0) {
			if((errno == EAGAIN) || (errno == EWOULDBLOCK))
				continue;
			else return;
		}

		bytes_were_read += bytes;
	}

	request[req_length] = '\0';

	client_req->str_req = request;
	client_req->json = cJSON_Parse(request);

	if(client_req->json == NULL) {
		mx_log(SERV_LOG_FILE, LOG_ERROR, (char*)cJSON_GetErrorPtr());
		return;
	}

	client_req->type = cJSON_GetObjectItem(client_req->json, "rtype")->valueint;
}

request_t* mx_get_client_req(client_t* client) {

	int req_len = get_req_len(client->ssl);

	if(req_len == 0)
		return NULL;

	request_t* client_req = malloc(sizeof(request_t));
	read_client_req(client_req, client->ssl, req_len);

	if(client_req->json == NULL) {
		free(client_req);
		return NULL;
	}

	char* log = mx_strjoin("Request was received: ", client_req->str_req);
	mx_log(SERV_LOG_FILE, LOG_TRACE, log);
	//mx_log(SERV_LOG_FILE, LOG_TRACE, mx_itoa(client_req));
	mx_strdel(&log);

	return client_req;
}

