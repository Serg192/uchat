#include "../../inc/client.h"

void mx_on_login_btn_clicked(GtkButton* b, gpointer data) {
		(void)b;
		
		char username_buffer[33];
		char password_buffer[33];

		client_t* client = (client_t*)data;

		//sprintf(username_buffer, "%s", gtk_entry_get_text(client->a_window->username_entry));
		//sprintf(password_buffer, "%s", gtk_entry_get_text(client->a_window->password_entry));
		GtkEntry* username_entry = GTK_ENTRY(client->a_window->username_entry);
        GtkEntry* password_entry = GTK_ENTRY(client->a_window->password_entry);
		sprintf(username_buffer, "%s", gtk_entry_get_text(username_entry));
        sprintf(password_buffer, "%s", gtk_entry_get_text(password_entry));


		printf("Username = %s, password = %s, %d\n", username_buffer, password_buffer, mx_strlen(password_buffer));

		if(mx_strlen(password_buffer) < 6) {
			gtk_label_set_text(GTK_LABEL(client->a_window->err_label), (const gchar*)"Password must be 6 - 24 characters long");
			return;
		}

		//For Profile
		//gtk_entry_set_text(GTK_ENTRY(client->u_window->username_entry), gtk_entry_get_text(username_entry)); 
		//gtk_entry_set_text(GTK_ENTRY(client->u_window->password_entry), gtk_entry_get_text(password_entry)); 

		client->login = mx_strdup(gtk_entry_get_text(username_entry));
		client->password = mx_strdup(gtk_entry_get_text(password_entry));

		const char* hash = mx_hash_sha256(password_buffer);
		printf("Hash => %s\n", hash);


		request_t* request = malloc(sizeof(request_t));
		request->req =  mx_create_login_req(username_buffer, hash);
		mx_queue_push(client->request_queue, request);
}

