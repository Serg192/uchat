#include "../inc/server.h"

static inline void handle_new_connection(int c_sock, SSL* ssl) {
	pthread_t thread;

	client_t* client = malloc(sizeof(client_t*));
	client->sock = c_sock;
	client->ssl = ssl;

	if(SSL_accept(ssl) == -1) {
		printf("SSL_accept() error\n");
	}

	pthread_create(&thread, NULL, &mx_client_handler, (void*)client);
}


void mx_run_serv(int s_sock) {

	struct sockaddr_in c_addr;
   	socklen_t addr_size = sizeof(struct sockaddr_in);
   	int c_sock;

   	ossl_t ossl;
   	mx_init_openssl(&ossl);

   //	SSL_library_init();
   //	ctx = mx_init_serv_ctx();
   //	mx_load_key(ctx);

   	 while(1) {
    	if((c_sock = accept(s_sock, (struct sockaddr*) &c_addr, &addr_size)) != -1){
    		ossl.ssl = SSL_new(ossl.ctx);
    		SSL_set_fd(ossl.ssl, c_sock);

    		handle_new_connection(c_sock, ossl.ssl);
    	}
    	else {

    	}
    }

    close(s_sock);
}