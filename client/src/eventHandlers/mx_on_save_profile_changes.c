#include "../../inc/client.h"

void mx_on_save_profile_changes(GtkWidget* widget, gpointer data) {
	(void)widget;
	client_t* client = (client_t*)data;

	const char* new_username = gtk_entry_get_text(GTK_ENTRY(client->u_window->username_entry));
	const char* new_password = gtk_entry_get_text(GTK_ENTRY(client->u_window->password_entry));

	if(mx_strcmp(new_username, client->login) == 0 && mx_strcmp(new_password, client->password) == 0){
		gtk_label_set_text(GTK_LABEL(client->u_window->information_label), "");
		return;
	}

	if(mx_strlen(new_username) < 4) {
		gtk_label_set_text(GTK_LABEL(client->u_window->information_label), (const gchar*)"Username must be 4 - 31 characters long");
		gtk_widget_set_name(client->u_window->information_label, "information_label_error");
		return;
	}

	if(mx_strlen(new_password) < 6) {
		gtk_label_set_text(GTK_LABEL(client->u_window->information_label), (const gchar*)"Password must be 6 - 24 characters long");
		gtk_widget_set_name(client->u_window->information_label, "information_label_error");
		return;
	}

	client->tmpLogin = mx_strdup(new_username);
	client->tmpPassword = mx_strdup(new_password);

	char* hash = mx_hash_sha256(new_password);

	request_t* request = (request_t*)malloc(sizeof(request_t));
	request->req = mx_create_edit_profile_req(new_username, hash);
	mx_queue_push(client->request_queue, request);

	free(hash);
}
