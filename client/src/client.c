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
static inline char* test_create_chat_req(const char* chat_name) {
    cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", CREATE_CHAT_REQ);
    cJSON_AddStringToObject(req, "name", chat_name);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}

//TEST METHOD
static inline char* test_create_singup_req(const char* login, const char* password){
    cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", SIGNUP_REQ);
    cJSON_AddStringToObject(req, "login", login);
    cJSON_AddStringToObject(req, "password", password);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}

//TEST METHOD
static inline char* test_create_login_req(const char* login, const char* password) {
    cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", LOGIN_REQ);
    cJSON_AddStringToObject(req, "login", login);
    cJSON_AddStringToObject(req, "password", password);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}

static inline char* test_create_search_chat_req(const char* pattern) {
    cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", CHAT_SEARCH_REQ);
    cJSON_AddStringToObject(req, "pattern", pattern);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}

static inline char* test_create_join_chat_req(const int chat_id) {
    cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", JOIN_CHAT_REQ);
    cJSON_AddNumberToObject(req, "room_id", chat_id);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;

}

static inline char* test_create_get_chat_participants_req(int chat_id) {
    cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", GET_CHAT_PARTICIPANTS_REQ);
    cJSON_AddNumberToObject(req, "room_id", chat_id);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}

//DATE FORMAT IS YY|MM|DATE
//TIME FORMAT IS HH|MM|SS in 24 hour format
static inline char* test_create_send_msg_req(const int room_id, 
                                             const int sending_date,
                                             const int sending_time,
                                             const char* context) {
    cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", SEND_MSG_REQ);
    cJSON_AddNumberToObject(req, "room_id", room_id);
    cJSON_AddNumberToObject(req, "sending_time", sending_time);
    cJSON_AddNumberToObject(req, "sending_date", sending_date);
    cJSON_AddStringToObject(req, "context", context);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}

static inline char* test_create_get_chat_msg_req(const int chat_id,
                                                 const int flag,
                                                 const int start_id,
                                                 const int count){

    cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", GET_CHAT_MSG_REQ);
    cJSON_AddNumberToObject(req, "room_id", chat_id);
    cJSON_AddNumberToObject(req, "flag", flag);
    cJSON_AddNumberToObject(req, "start_id", start_id);
    cJSON_AddStringToObject(req, "count", count);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}

static inline void send_req(SSL* ssl, const char* req) {

    printf("Sending request\n");

    int req_ilen = mx_strlen(req);
    char* req_strlen = mx_itoa(req_ilen);

    SSL_write(ssl, req_strlen, mx_strlen(req_strlen));
    SSL_write(ssl, req, mx_strlen(req));
}

static inline void print_res(serv_res_t* res) {
    char* resp = cJSON_PrintUnformatted(res->json);

    printf("Serv response: %s\n", resp);

    free(resp);
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
    SSL_set_mode(ssl, SSL_MODE_ASYNC);
    SSL_set_fd(ssl, serv);

    if(SSL_connect(ssl) == -1) {
        ERR_print_errors_fp(stderr);
    }

    int flags = fcntl(serv, F_GETFL,0);
    fcntl(serv, F_SETFL, flags | O_NONBLOCK);
    fcntl(serv, F_SETFD, O_NONBLOCK);

    
    bool can_send_next_req = true;
    char login[32], password[32], chat_name[32], search_str[32];

    request_t* client_req = malloc(sizeof(client_req));
    client_req->handled = false;

    clock_t sending_req_time;


    while(1){

        if(can_send_next_req) {

            //Just for test

            int i;

            printf("Choose req type\n");
            scanf("%d", &i);

            char* req;

            switch(i) {
                case 0:
                    printf("Creating SIGNUP_REQ, enter login and password\n");
                    //char* login, password;
                    scanf("%s", login);
                    scanf("%s", password);
                    client_req->req = test_create_singup_req(login, password);
                    send_req(ssl, client_req->req);
                    break;
                case 1:
                    printf("Creating LOGIN_REQ, enter login and password\n");
                    //char* login, password;
                    scanf("%s", login);
                    scanf("%s", password);
                    client_req->req = test_create_login_req(login, password);
                    send_req(ssl, client_req->req);
                    break;

                case 2:
                    printf("Creating new chat, enter chat name\n");
                   // char* chat_name;
                    scanf("%s", chat_name);
                    client_req->req = test_create_chat_req(chat_name);
                    send_req(ssl, client_req->req);
                    break;

                case 3:
                    printf("Find existing chat, enter chat name\n");
                    //char* search_str;
                    scanf("%s", search_str);
                    client_req->req = test_create_search_chat_req(search_str);
                    send_req(ssl, client_req->req);
                    break;

                case 4:

                    printf("Join chat req, enter chat id (received as a result of chat searching)\n");
                    int r_id;
                    scanf("%d", &r_id);
                    client_req->req = test_create_join_chat_req(r_id);
                    send_req(ssl, client_req->req);
                    break;

                case 5:
                    printf("Send get chat participants request, enter chat id\n");
                    int c_id;
                    scanf("%d", &c_id);
                    client_req->req = test_create_get_chat_participants_req(c_id);
                    send_req(ssl, client_req->req);
                    break;
                case 6:
                    printf("Send message, enter chat id and message\n");
                    int room_id;
                    scanf("%d", &room_id);
                    char message[1024];
                    scanf("%s", message);
                    client_req->req = test_create_send_msg_req(room_id, 101, 101, message);
                    send_req(ssl, client_req->req);
                    break;
                case 7:
                    printf("Get chat messages, enter chat_id\n");
                    int chat_i;
                    scanf("%d", &chat_i);
                    client_req->req = test_create_get_chat_msg_req(chat_i, 0, 0, 0);
                    send_req(ssl, client_req->req);
                    break;
                default:
                    break;
            }


            can_send_next_req = false;
            client_req->handled = false;

            sending_req_time =  clock();
        } else {
            serv_res_t* res = mx_get_server_response(ssl);

            if(res != NULL) {
                //handling
                //

                print_res(res);

                can_send_next_req = true;
                client_req->handled = true;
                free(res);

               // usleep(4000);
            } else if(!client_req->handled) {

                clock_t diff = clock() - sending_req_time;
                int time_taken = ((int)diff)/CLOCKS_PER_SEC;

                printf("Taken time: %d\n", time_taken);
                //if server doesn't send respond more that 3 sec resend request
                if(time_taken >= 3) {
                    printf("Resending request\n");
                    send_req(ssl, client_req->req);
                    sending_req_time = clock();
                }
            }
        }
    }

    SSL_free(ssl);
    
    close(serv); 

    SSL_CTX_free(ctx); 

    return 0;
}
