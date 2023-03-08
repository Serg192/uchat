#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h> 
#include <malloc.h> 
#include <sys/socket.h>
#include <resolv.h> 
#include <netdb.h>
#include <time.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <cJSON.h>
#include <gtk/gtk.h>

#include <libmx.h>

#include "../../common/common.h"

#define CLIENT_LOG_FILE "client/log/log.txt"
#define AUTH_WIND_XML_PATH "client/res/windows/auth_window.glade"
#define CHAT_WIND_XML_PATH "client/res/windows/chat_window.glade"
#define CREATE_CHAT_WIND_XML_PATH "client/res/windows/create_chat_window.glade"

typedef struct serv_res_s {
	cJSON* json;
	const char* str_res;
	int type;
}			   serv_res_t;

typedef struct request_s {
	char* req;

	//for test purpose
	//bool handled;
}			   request_t;

typedef struct auth_window_s {
	GtkWidget *window;
	GtkWidget *err_label;
	GtkWidget *signup_btn;
	GtkWidget *login_btn;
	GtkWidget *username_entry;
	GtkWidget *password_entry;

	GtkBuilder *builder;
}			   auth_window_t;

typedef struct create_chat_window_s {
	GtkWidget* window;
	GtkWidget* create_btn;
	GtkWidget* cancel_btn;
	GtkWidget* chat_name_entry;

	GtkBuilder* builder;
}			   create_chat_window_t;


typedef struct chat_window_s {
	GtkWidget* window;
	GtkWidget *main_box;
	GtkWidget *left_side_box;
	GtkWidget *chats_list_scrlldwnd;
	GtkWidget *chats_list_view;
	GtkWidget *chats_list_grid;
	
	GtkWidget* add_chat_btn;

	GtkBuilder *builder;
}			   chat_window_t;

typedef struct chat_info_s {
	int id;
	const char* name;
}			   chat_info_t;

typedef struct disp_chats_container_s {
	int count;
	chat_info_t** chats;
}			   disp_chats_container_t;

typedef struct client_s {
	int serv;
	SSL* ssl;
	SSL_CTX* ctx;

	request_t* current_request;
	serv_res_t* current_response;
	bool can_send_req;

	auth_window_t* a_window;
	chat_window_t* c_window;
	create_chat_window_t* create_chat_window;

	bool destroy_cnw;

	disp_chats_container_t* chat_container;

}			   client_t;


void mx_handle_get_joined_chats(client_t* client);

serv_res_t* mx_get_server_response(SSL* ssl);

auth_window_t* mx_build_auth_window(client_t* client);

chat_window_t* mx_build_chat_window(client_t* client);

create_chat_window_t* mx_build_create_chat_window(client_t* client);

void mx_send_req(SSL* ssl, const char* req);

void mx_client_init(client_t* client, const char* host, const int port);

//btn handlers

void mx_on_signup_btn_clicked(GtkButton* b, gpointer data);

void mx_on_login_btn_clicked(GtkButton* b, gpointer data);

void mx_on_chat_btn_clicked(GtkButton* b, gpointer data);

//button inside popup window
void mx_on_create_new_chat_btn_clicked(GtkButton* b, gpointer data);

//create new chat button
void mx_on_create_chat_btn_clicked(GtkButton* b, gpointer data);

//

int mx_create_connection_with_serv(const char* host, int port);

SSL_CTX* mx_init_ctx(void);

void mx_main_background_loop(void* data);

void mx_hide_hint_window(GtkWidget* widget, gpointer data);

//
char* mx_create_chat_req(const char* chat_name);

char* mx_create_signup_req(const char* login, const char* password);

char* mx_create_login_req(const char* login, const char* password);

char* mx_create_search_chat_req(const char* pattern);

char* mx_create_join_chat_req(const int chat_id);

char* mx_create_get_chat_participants_req(int chat_id);

char* mx_create_get_joined_chats_req();

//TEST
void create_new_chat_window(client_t* client);
//


//DATE FORMAT IS YY|MM|DATE
//TIME FORMAT IS HH|MM|SS in 24 hour format
char* mx_create_send_msg_req(const int room_id, const int sending_date, const int sending_time, const char* context);

char* mx_create_get_chat_msg_req(const int chat_id, const int flag, const int start_id, const int count);

//Response handlers

void mx_handle_auth_err(client_t* client);

//also used when signup success
void mx_handle_auth_success(client_t* client);

void mx_handle_chat_creation(client_t* client);


#endif