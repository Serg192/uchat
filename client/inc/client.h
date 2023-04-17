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
#include <openssl/sha.h>

#include <cJSON.h>
#include <gtk/gtk.h>

#include <libmx.h>

#include "../../common/common.h"

#define CLIENT_LOG_FILE "client/log/log.txt"
#define AUTH_WIND_XML_PATH "client/res/windows/auth_window.glade"
#define CHAT_WIND_XML_PATH "client/res/windows/chat_window.glade"
#define CREATE_CHAT_WIND_XML_PATH "client/res/windows/create_chat_window.glade"
#define DIALOG_WIND_XML_PATH "client/res/windows/dialog_window.glade"

#define MSG_LOAD_LIMIT 10

typedef struct serv_res_s {
	cJSON* json;
	const char* str_res;
	int type;
}			   serv_res_t;

typedef struct request_s {
	char* req;
	int type;

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
	GtkWidget *show_psw_bnt;

	GtkBuilder *builder;
}			   auth_window_t;

typedef struct create_chat_window_s {
	GtkWidget* window;
	GtkWidget* create_btn;
	GtkWidget* cancel_btn;
	GtkWidget* chat_name_entry;

	GtkBuilder* builder;
}			   create_chat_window_t;

typedef struct dialog_window_s {
	GtkWidget* window;
	GtkWidget* info_lable;
	GtkWidget* ok_btn;
	GtkWidget* cancel_btn;

	GtkBuilder* builder;
}			   dialog_window_t;

typedef struct chat_window_s {
	GtkWidget* window;
	GtkWidget *main_box;
	//Left side(chats list, search, add chat)
	GtkWidget *chats_list_box;		//Not used anywhere
	GtkWidget *chats_list_scrlldwnd;	//Not used anywhere
	GtkWidget *chats_list_view;		//Not used anywhere
	GtkWidget *chats_list_grid;
	GtkWidget *search_entry;	
	GtkWidget* add_chat_btn;
	//Right side(current chat)
	GtkWidget *selected_chat_box;
	GtkWidget *chat_name_label;
	GtkWidget *chat_settings_btn;
	GtkWidget *chat_info_btn;
	GtkWidget *msgs_list_scrlld_wnd;
	GtkWidget *msgs_list_box;
	GtkWidget *entry_edit_stack;
	GtkWidget *selected_msg_close_btn;
	GtkWidget *selected_msg_edit_btn;
	GtkWidget *selected_msg_select_all_btn;
	GtkWidget *message_input_field;
	GtkWidget *send_message_btn;
	
	GtkBuilder *builder;

	int elements_were_attached;

	clock_t search_t;
}			   chat_window_t;

typedef struct client_s {
	int serv;
	SSL* ssl;
	SSL_CTX* ctx;

	/////////////////////////

	queue_t* request_queue;

	////////////////////////
	
	serv_res_t* current_response;

	auth_window_t* a_window;
	chat_window_t* c_window;
	create_chat_window_t* create_chat_window;
	dialog_window_t* d_window;

	bool destroy_cnw;

	int* chat_id_chat_btn_map;
	int join_chat_id;

	bool search_mode;

	int current_chat_id;
	int last_msg_in_chat_id;
	int first_msg_in_chat_id;

}			   client_t;


int mx_get_chat_id_from_btn(GtkWidget* w, client_t* client);

void mx_handle_get_joined_chats(client_t* client);

serv_res_t* mx_get_server_response(SSL* ssl);

auth_window_t* mx_build_auth_window(client_t* client);

chat_window_t* mx_build_chat_window(client_t* client);

create_chat_window_t* mx_build_create_chat_window(client_t* client);

dialog_window_t* mx_build_dialog_window(client_t* client);

void mx_send_req(SSL* ssl, const char* req);

void mx_client_init(client_t* client, const char* host, const int port);

//btn handlers

void mx_on_signup_btn_clicked(GtkButton* b, gpointer data);

void mx_on_login_btn_clicked(GtkButton* b, gpointer data);

void mx_on_chat_btn_clicked(GtkButton* b, gpointer data);

void mx_on_search_changed(GtkWidget *w, gpointer data);

void mx_on_show_psw_btn_clicked(GtkWidget *w, gpointer data);

void mx_on_chat_search_list_clicked(GtkWidget* w, gpointer data);

void mx_on_send_msg_btn_clicked(GtkButton* b, gpointer data);

void mx_on_msg_list_edge_was_reached(GtkScrolledWindow* scrolled_window, GtkPositionType pos, gpointer user_data);

void mx_on_chat_list_clicked(GtkWidget* w, gpointer data);

void mx_add_message_to_list(chat_window_t* window, const char *sender_name, const char *message_text, const char *sending_time, gboolean is_your_message);

void mx_app_on_destroy(GtkWidget *widget, gpointer data);

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
char* mx_create_chat_req(const char* chat_name, int color);

char* mx_create_signup_req(const char* login, const char* password);

char* mx_create_login_req(const char* login, const char* password);

char* mx_create_search_chat_req(const char* pattern);

char* mx_create_join_chat_req(const int chat_id);

char* mx_create_get_chat_participants_req(int chat_id);

char* mx_create_get_joined_chats_req();

char* mx_create_quit_request();

//TEST
void create_new_chat_window(client_t* client);
//


//DATE FORMAT IS YY|MM|DATE
//TIME FORMAT IS HH|MM|SS in 24 hour format
char* mx_create_send_msg_req(const int room_id, const char* context);

char* mx_create_get_chat_msg_req(const int chat_id, const int flag, const int start_id, const int count);

//Response handlers

void mx_handle_auth_err(client_t* client);

//also used when signup success
void mx_handle_auth_success(client_t* client);

void mx_handle_chat_creation(client_t* client);

void mx_handle_msg_update(client_t* client);

//_____Pasword hash function______
char* mx_hash_sha256(const char *password);
//________________________________

char *mx_prepare_str_for_sql(const char *input);


#endif
