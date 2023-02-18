#include "../inc/server.h"

void mx_send_response(client_t* client, const char* respond) {

	int res_len = mx_strlen(respond);
	char* str_res_len = mx_itoa(res_len);

	SSL_write(client->ssl, str_res_len, mx_strlen(str_res_len));
	SSL_write(client->ssl, respond, mx_strlen(respond));

	/*
	 int response_len = mx_strlen(response);
    char* len_str = mx_itoa(response_len);
    SSL_write(ssl, len_str, mx_strlen(len_str));
    SSL_write(ssl, response, response_len);
    mx_strdel(&len_str);
	*/
	
}

