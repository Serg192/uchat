#include "../inc/client.h"

/*
static inline int get_res_len(SSL* ssl) {

    char buffer[256];
    int bytes_was_read = 0;

    printf("Start\n");

    while((bytes_was_read = SSL_read(ssl, buffer, 256)) <= 0) {
    	if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
            continue;
        }
        return 0;
    }

    printf("End\n");

    return atoi(buffer);
}
*/

static inline int get_res_len(SSL* ssl) {

    char buffer[256];
    int bytes_was_read = 0;

    // Set the read timeout
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    SSL_set_timeout(ssl, 3);

    // Set non-blocking mode
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY | SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);

    while (1) {
        bytes_was_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);

        if (bytes_was_read > 0) {
            return atoi(buffer);
        } else if (bytes_was_read == 0) {
            mx_log(CLIENT_LOG_FILE, LOG_TRACE, "Connection was closed");
            return 0;
        } else {
            // Error occurred
            int err = SSL_get_error(ssl, bytes_was_read);

            if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
                // Check if the timeout has occurred
                fd_set readfds;
                FD_ZERO(&readfds);
                FD_SET(SSL_get_fd(ssl), &readfds);
                int ret = select(SSL_get_fd(ssl) + 1, &readfds, NULL, NULL, &tv);
                if (ret == -1) {
                    mx_log(CLIENT_LOG_FILE, LOG_ERROR, "get_res_len(), select error");
                    return 0;
                } else if (ret == 0) {
                    mx_log(CLIENT_LOG_FILE, LOG_TRACE, "Response timeout");
                    printf("Response timeout\n");
                    return 0;
                }
            } else {
                mx_log(CLIENT_LOG_FILE, LOG_ERROR, "get_res_len(), SSL read failed");
                return 0;
            }
        }
    }
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