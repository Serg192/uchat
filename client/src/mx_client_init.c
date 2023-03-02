#include "../inc/client.h"

void mx_client_init(client_t* client, const char* host, const int port){
	SSL_library_init();

	client->ctx = mx_init_ctx();
	client->serv = mx_create_connection_with_serv(host, port);

	client->ssl = SSL_new(client->ctx);
    SSL_set_mode(client->ssl, SSL_MODE_ASYNC);
    SSL_set_fd(client->ssl, client->serv);

    if(SSL_connect(client->ssl) == -1) {
        ERR_print_errors_fp(stderr);
    }

    int flags = fcntl(client->serv, F_GETFL,0);
    fcntl(client->serv, F_SETFL, flags | O_NONBLOCK);
    fcntl(client->serv, F_SETFD, O_NONBLOCK);
}
