#include "../inc/server.h"

static inline void handle_new_connection(int c_sock, SSL* ssl) {
	pthread_t thread;

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Allocating memory for new client");

	client_t* client = malloc(sizeof(client_t));
	client->sock = c_sock;
	client->ssl = ssl;
	client->user_id = -1;

	mx_log(SERV_LOG_FILE, LOG_TRACE, "SSL_accept() function");

	if(SSL_accept(client->ssl) == -1) {
		mx_log(SERV_LOG_FILE, LOG_ERROR, "Server SSL_accept() failed");
		mx_log(SERV_LOG_FILE, LOG_ERROR, strerror(errno));
	}

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Setting up flags");

	int flags = fcntl(c_sock, F_GETFL, 0);
	fcntl(c_sock, F_SETFL, flags | O_NONBLOCK);
	fcntl(c_sock, F_SETFD, O_NONBLOCK);

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Everything was configured, starting a new thread");

	pthread_create(&thread, NULL, &mx_client_handler, (void*)client);
}


void mx_run_serv(int s_sock) {

	struct sockaddr_in c_addr;
   	socklen_t addr_size = sizeof(struct sockaddr_in);
   	int c_sock;

   	ossl_t ossl;
   	mx_init_openssl(&ossl);
   	mx_init_database();

   	 while(1) {
   	 	mx_log(SERV_LOG_FILE, LOG_TRACE, "Wainitng for connection ...");
    	if((c_sock = accept(s_sock, (struct sockaddr*) &c_addr, &addr_size)) != -1){

    		const char* ip = inet_ntoa(c_addr.sin_addr);
    		char* log = mx_strjoin("New client was connected: ", ip);
    		mx_log(SERV_LOG_FILE, LOG_INFO, log);
    		mx_strdel(&log);

    		ossl.ssl = SSL_new(ossl.ctx);
    		SSL_set_fd(ossl.ssl, c_sock);

    		mx_log(SERV_LOG_FILE, LOG_TRACE, "Creating client handler");

    		handle_new_connection(c_sock, ossl.ssl);
    	}
    	else {

    	}
    }

    close(s_sock);
}