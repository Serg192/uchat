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
#include <sys/select.h> 
#include <time.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sha.h>

#include <cJSON.h>
#include <gtk/gtk.h>

#include <libmx.h>

#include "../../common/common.h"


#include <strings.h> //added for function bzero in mx_create_connection_with_serv.c


#define CLIENT_LOG_FILE "client/log/log.txt"
#define AUTH_WIND_XML_PATH "client/res/windows/auth_window.glade"
#define CHAT_WIND_XML_PATH "client/res/windows/chat_window.glade"
#define CREATE_CHAT_WIND_XML_PATH "client/res/windows/create_chat_window.glade"
#define DIALOG_WIND_XML_PATH "client/res/windows/dialog_window.glade"
#define CHAT_INFO_WIND_XML_PATH "client/res/windows/chat_info_window.glade"
#define USER_INFO_WIND_XML_PATH "client/res/windows/user_info_window.glade"
#define MENU_WIND_XML_PATH "client/res/windows/menu_window.glade"
#define STYLES_FILE_PATH "client/res/styles.css"

#define MSG_LOAD_LIMIT 20
#define RESP_TIMEOUT 3.0f

enum {
	PUSH_FRONT,
	PUSH_BACK
};

typedef struct message_s {
	int id;
	int sending_time;
	int sending_date;
	char* username_from;
	char* messages_str;

	//pointer to gtk label that is used when edit text
	GtkWidget* text_label;

}              message_t;


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
	GtkWidget *window;
	GtkWidget *create_btn;
	GtkWidget *cancel_btn;
	GtkWidget *chat_name_entry;
	GtkWidget *create_chat_label;
	GtkWidget *create_chat_length_label;

	GtkBuilder *builder;
}			   create_chat_window_t;

typedef struct dialog_window_s {
	GtkWidget *window;
	GtkWidget *info_label;
	GtkWidget *ok_btn;
	GtkWidget *cancel_btn;

	GtkBuilder *builder;
}			   dialog_window_t;

typedef struct chat_window_s {
	GtkWidget* window;
	//Left side(chats list, search, add chat)
	GtkWidget *chats_list_box;
	GtkWidget *chats_list_scrlldwnd;
	GtkWidget *chats_list_grid;
	GtkWidget *menu_btn;
	GtkWidget *search_entry;	
	GtkWidget *add_chat_btn;
	//Right side(current chat)
	GtkWidget *selected_chat_box;
	GtkWidget *chat_header_box;
	GtkWidget *chat_name_label;
	GtkWidget *chat_info_btn;
	GtkWidget *msgs_list_scrlld_wnd;
	GtkWidget *msgs_list_box;
	GtkWidget *entry_edit_stack;
	GtkWidget *selected_msg_edit_box;
	GtkWidget *selected_msg_close_btn;
	GtkWidget *selected_msg_number_label;
	GtkWidget *selected_msg_edit_btn;
	GtkWidget *selected_msg_select_all_btn;
	GtkWidget *selected_msg_delete_btn;
	GtkWidget *selected_msg_forward_btn;
	GtkWidget *msg_input_scrlld_wnd;
	GtkWidget *message_input_field;
	GtkWidget *send_message_btn;
	GtkWidget *editing_label;
	GtkWidget *editing_close_btn;
	
	GtkBuilder *builder;

	int elements_were_attached;

	clock_t search_t;
}			   chat_window_t;

typedef struct chat_info_window_s {
	GtkWidget *window;
	GtkWidget *chat_info_name_label;
	GtkWidget *members_list_label;
	GtkWidget *members_list_scrlld_wnd;
	GtkWidget *members_list_box;
	GtkWidget *leave_chat_btn;
	GtkWidget *cancel_info_btn;

	GtkBuilder *builder;

	bool visible;
}			   chat_info_window_t;

typedef struct menu_window_s {
	GtkWidget *window;
	GtkWidget *user_profile_btn;
	GtkWidget *log_out_btn;
	GtkWidget *delete_acc_btn;
	GtkWidget *version_label;

	GtkBuilder *builder;
}			   menu_window_t;

typedef struct user_info_window_s {
	GtkWidget *window;
	GtkWidget *profile_label;
	GtkWidget *username_entry;
	GtkWidget *username_label;
	GtkWidget *password_entry;
	GtkWidget *password_label;
	GtkWidget *save_btn;
	GtkWidget *cancel_btn;
	GtkWidget *show_passw_check_btn;
	GtkWidget *information_label;

	GtkBuilder *builder;
}			   user_info_window_t;

typedef struct client_s {
	char* host;
	int port;
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
	chat_info_window_t* i_window;
	menu_window_t* m_window;
	user_info_window_t* u_window;

	bool destroy_cnw;

	//int* chat_id_chat_btn_map;
	int join_chat_id;

	bool search_mode;

	//Editing
	bool edit_mode;
	message_t* message_to_edit;
	//



	int current_chat_id;
	int last_msg_in_chat_id;
	int first_msg_in_chat_id;

	t_list* messages_in_current_chat;
	t_list* p_change_clear_list;

	map_t* message_id_row_map;

	GtkListBoxRow* edit_row;

	char* login;
	char* password;
	char* tmpLogin;
	char* tmpPassword;

}			   client_t;


typedef struct push_msg_data_s {
	client_t* client;
	message_t* message;
	int mode;
}			   push_msg_data_t;


typedef struct del_msg_from_list_box_data_s {
	client_t* client;
	int message_id;
}              del_msg_from_list_box_data_t;

typedef struct members_list_entry_s {
	client_t* client;
	int this_client_permissions;
	int member_permissions;
	char* user_to_add;
	int userID_to_add;
	int banned;
}              members_list_entry_t;

typedef struct perm_change_s {
	int this_client_permissions;
	int user_permissions;
	GtkWidget* p_label;
	int userID;
}			   perm_change_t;

typedef struct on_chat_clicked_data_s {
	client_t* client;
	const char *chat_name;
}              on_chat_clicked_data_t;

void mx_user_info_add_user(members_list_entry_t* m);

int mx_get_chat_id_from_btn(GtkWidget* w, client_t* client);

//deprecated
void mx_handle_get_joined_chats(client_t* client);

//improved version of mx_handle_get_joined_chats function
void mx_handle_update_chat_list(client_t* client);

serv_res_t* mx_get_server_response(SSL* ssl);

auth_window_t* mx_build_auth_window(client_t* client);

chat_window_t* mx_build_chat_window(client_t* client);

create_chat_window_t* mx_build_create_chat_window(client_t* client);

dialog_window_t* mx_build_dialog_window(client_t* client);

chat_info_window_t* mx_build_chat_info_window(client_t* client);

menu_window_t* mx_build_menu_window(client_t* client);

user_info_window_t* mx_build_user_info_window(client_t* client);

void mx_send_req(SSL* ssl, const char* req);

void mx_client_init(client_t* client, const char* host, const int port);

void mx_msg_edit_update_list(client_t* client, int message_id, char* text);

//btn handlers

void mx_on_signup_btn_clicked(GtkButton* b, gpointer data);

void mx_on_login_btn_clicked(GtkButton* b, gpointer data);

void mx_on_chat_btn_clicked(GtkButton* b, gpointer data);

void mx_on_delete_btn_clicked(GtkWidget* w, gpointer data);

void mx_on_search_changed(GtkWidget *w, gpointer data);

void mx_on_show_psw_btn_clicked(GtkWidget *w, gpointer data);

void mx_on_chat_search_list_clicked(GtkWidget* w, gpointer data);

void mx_on_log_out_btn_clicked(GtkWidget* w, gpointer data);

void mx_on_send_msg_btn_clicked(GtkButton* b, gpointer data);

void mx_on_selected_msg_close_btn_clicked(GtkButton* b, gpointer data);

gboolean mx_enter_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data);

void mx_on_menu_btn_clicked(GtkWidget* widget, gpointer data);

void mx_on_user_profile_btn_clicked(GtkWidget* widget, gpointer data);

void mx_on_cancel_info_btn_clicked(GtkWidget* widget, gpointer data);

gboolean mx_on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data);

void mx_on_selection_changed(GtkListBox *listbox, gpointer data);

void mx_on_selected_msg_select_all_btn_clicked(GtkButton* b, gpointer data);

void mx_on_msg_list_edge_was_reached(GtkScrolledWindow* scrolled_window, GtkPositionType pos, gpointer user_data);

void mx_on_chat_list_clicked(GtkWidget* w, gpointer data);

void mx_on_edit_btn_clicked(GtkWidget* w, gpointer data);

void  mx_push_message(client_t* client, message_t* message, int mode);

void mx_delete_msg_from_list_box(client_t* client, int message_id);

void mx_on_save_profile_changes(GtkWidget* widget, gpointer data);

void mx_app_on_destroy(GtkWidget *widget, gpointer data);

//button inside popup window
void mx_on_create_new_chat_btn_clicked(GtkButton* b, gpointer data);

//create new chat button
void mx_on_create_chat_btn_clicked(GtkButton* b, gpointer data);

void mx_on_leave_btn_clicked(GtkButton* b, gpointer data);

void mx_on_del_account_btn_clicked(GtkWidget* w, gpointer data);

void mx_on_chat_info_btn_clicked(GtkWidget* widget, gpointer data);

void mx_on_ban_btn_clicked(GtkButton* b, gpointer data);

void mx_on_unban_btn_clicked(GtkButton* b, gpointer data);

void mx_on_promote_btn_cliked(GtkButton* b, gpointer data);

void mx_on_demote_btn_clicked(GtkButton* b, gpointer data);

int mx_create_connection_with_serv(const char* host, int port);

SSL_CTX* mx_init_ctx(void);

void* mx_main_background_loop(void* data);

void mx_hide_hint_window(GtkWidget* widget, gpointer data);

//
char* mx_create_ban_req(int chat_id, int user_id, int banned);

char* mx_create_permissions_change_req(int chat_id, int user_id, int perm);

char* mx_create_chat_req(const char* chat_name, int color);

char* mx_create_signup_req(const char* login, const char* password);

char* mx_create_login_req(const char* login, const char* password);

char* mx_create_search_chat_req(const char* pattern);

char* mx_create_join_chat_req(const int chat_id);

char* mx_create_get_chat_participants_req(int chat_id);

char* mx_create_get_joined_chats_req();

char* mx_create_quit_request();

char* mx_cteate_message_edit_req(int chat_id, int message_id, char* new_text);

char* mx_create_delete_chat_msg_req(const int chat_id, const int message_id);

char* mx_create_leave_chat_req(const int chat_id); 

char* mx_create_delete_account_req();

//TEST
void create_new_chat_window(client_t* client);
//


//DATE FORMAT IS YY|MM|DATE
//TIME FORMAT IS HH|MM|SS in 24 hour format
char* mx_create_send_msg_req(const int room_id, const char* context);

char* mx_create_get_chat_msg_req(const int chat_id, const int flag, const int start_id, const int count);

char* mx_create_edit_profile_req(const char* new_username, const char* new_password);

//Response handlers

void mx_handle_auth_err(client_t* client);

void mx_handle_create_chat_info_window(client_t* client);

//also used when signup success
void mx_handle_auth_success(client_t* client);

void mx_handle_chat_creation(client_t* client);

void mx_handle_msg_update(client_t* client);

void mx_handle_profile_edit_ok(client_t* client);

void mx_handle_profile_edit_err(client_t* client);

//_____Pasword hash function______
char* mx_hash_sha256(const char *password);
//________________________________

char *mx_prepare_str_for_sql(const char *input);

void mx_room_data_clear(client_t* client);

void mx_widget_add_styles(GtkWidget *widget);

#endif
