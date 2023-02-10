#include "../inc/client.h"


static inline int create_connection_with_serv(const char* host, int port) {

    struct hostent* ht;
    struct sockaddr_in addr;
    int connection;

    bzero(&addr, sizeof(addr));

    ht = gethostbyname(host);

    if(ht == NULL) {
        perror(host);
        abort();
    }

    connection = socket(PF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //memcpy(&addr.sin_addr, ht->h_addr_list[0], ht->h_length);
    addr.sin_addr.s_addr = *(long *)(ht->h_addr_list[0]);

    if(connect(connection, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        close(connection);
        perror(host);
        abort();
    }

    return connection;
}

static inline SSL_CTX* init_ctx(void) {
    
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
  
    SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_client_method());
    if (ctx == NULL){
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

//TEST METHOD
static inline char* test_create_singup_req(const char* login, const char* password){
    cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", SINGUP_REQ);
    cJSON_AddStringToObject(req, "login", login);
    cJSON_AddStringToObject(req, "password", password);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}

int main(int argc, char* argv[]) {

    if(argc != 3) {
        mx_printerr("usage: ./uchar [ip] [port]\n");
        exit(-1);
    }

    const char* host = argv[1];
    int port = atoi(argv[2]);


    SSL_library_init();

    SSL_CTX* ctx = init_ctx();

    int serv = create_connection_with_serv(host, port);

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, serv);

    if(SSL_connect(ssl) == -1) {
        ERR_print_errors_fp(stderr);
    }


    //TEST 
    //Sending singup request
    char* singup_req = test_create_singup_req("User1", "123456");
    int singup_ilen = strlen(singup_req);
    char* singup_req_len = mx_itoa(singup_ilen);
    
    SSL_write(ssl, singup_req_len, strlen(singup_req_len));
    SSL_write(ssl, singup_req, singup_ilen);

    SSL_free(ssl);
    
    close(serv); 

    SSL_CTX_free(ctx); 

    return 0;
}
