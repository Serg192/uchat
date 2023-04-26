#include "../inc/client.h"


void mx_send_req(SSL* ssl, const char* req) {

    //printf("Sending request\n");

    int req_ilen = mx_strlen(req);
    char* req_strlen = mx_itoa(req_ilen);

    SSL_write(ssl, req_strlen, mx_strlen(req_strlen));
    SSL_write(ssl, req, mx_strlen(req));
}

