#include "../inc/client.h"


SSL_CTX* mx_init_ctx(void) {
    
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
  
    SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_client_method());
    if (ctx == NULL){
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

