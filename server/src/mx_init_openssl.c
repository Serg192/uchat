#include "../inc/server.h"

static inline void load_key(SSL_CTX *ctx) {

	if(SSL_CTX_use_certificate_file(ctx, SSL_SERT_FILE, SSL_FILETYPE_PEM) <= 0) {
		//TODO: better error handling (in a  case of running this server as a daemon process)
		ERR_print_errors_fp(stderr);
		abort();
	}

    if(SSL_CTX_use_PrivateKey_file(ctx, SSL_KEY_FILE, SSL_FILETYPE_PEM) <= 0){
        ERR_print_errors_fp(stderr);
        abort();
    }
  
    if (!SSL_CTX_check_private_key(ctx)){
        fprintf(stderr, "Private key does not match the public certificaten");
        abort();
    }
}

static inline SSL_CTX* init_ctx(void) {
	
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
  
    SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_server_method());
    if (ctx == NULL){
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}


void mx_init_openssl(ossl_t* ossl) {

	SSL_library_init();

	ossl->ctx = init_ctx();
	load_key(ossl->ctx);
}