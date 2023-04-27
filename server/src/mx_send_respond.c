#include "../inc/server.h"

void mx_send_response(client_t* client, const char* respond) {

	int res_len = mx_strlen(respond);
	char* str_res_len = mx_itoa(res_len);

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Sending response");
	mx_log(SERV_LOG_FILE, LOG_TRACE, (char*)respond);

	SSL_write(client->ssl, str_res_len, mx_strlen(str_res_len));
	SSL_write(client->ssl, respond, mx_strlen(respond));

}

