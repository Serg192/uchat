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

typedef struct serv_res_s {
	cJSON* json;
	const char* str_res;
	int type;
}			   serv_res_t;

typedef struct request_s {
	bool handled;
	char* req;
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

serv_res_t* mx_get_server_response(SSL* ssl);

auth_window_t* mx_build_auth_window();

//btn handlers

void mx_on_signup_btn_clicked(GtkButton* b, gpointer data);

void mx_on_login_btn_clicked(GtkButton* b, gpointer data);

#endif