#include "../inc/client.h"

static inline int get_res_len(SSL* ssl) {

    char buffer[256];
    int bytes_was_read = 0;

    //printf("Start\n");

    while((bytes_was_read = SSL_read(ssl, buffer, 256)) <= 0) {
    	if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
            continue;
        }
        return 0;
    }

    //printf("End\n");

    return atoi(buffer);
}

static inline void read_server_res(serv_res_t* res, SSL* ssl, int res_length) {

	int bytes_were_read = 0;
	char* resp = malloc(res_length + 1);

	while(bytes_were_read != res_length) {
		
		int bytes = SSL_read(ssl, resp + bytes_were_read, res_length - bytes_were_read);

		if(bytes <= 0) {
			if((errno == EAGAIN) || (errno == EWOULDBLOCK))
				continue;
			else return;
		}

		bytes_were_read += bytes;
	}

	resp[res_length] = '\0';

	res->json = cJSON_Parse(resp);
	res->str_res = resp;

	if(res->json == NULL) {
		mx_log(CLIENT_LOG_FILE, LOG_ERROR, cJSON_GetErrorPtr());
		return;
	}

	res->type = cJSON_GetObjectItem(res->json, "rtype")->valueint;

}

serv_res_t* mx_get_server_response(SSL* ssl) {

	int res_len = get_res_len(ssl);


	if(res_len == 0)
		return NULL;


	serv_res_t* serv_res = malloc(sizeof(serv_res_t));
	read_server_res(serv_res, ssl, res_len);

	if(serv_res->json == NULL) {
		free(serv_res);
		return NULL;
	}

	char* log = mx_strjoin("Response was received: ", serv_res->str_res);
	mx_log(CLIENT_LOG_FILE, LOG_TRACE, log);
	//mx_log(SERV_LOG_FILE, LOG_TRACE, mx_itoa(client_req));
	mx_strdel(&log);

	return serv_res;
}