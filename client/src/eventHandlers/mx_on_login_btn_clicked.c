#include "../../inc/client.h"

void mx_on_login_btn_clicked(GtkButton* b, gpointer data) {
		(void)b;
		
		char username_buffer[33];
		char password_buffer[33];

		client_t* client = (client_t*)data;

		sprintf(username_buffer, "%s", gtk_entry_get_text(GTK_ENTRY(client->a_window->username_entry)));
		sprintf(password_buffer, "%s", gtk_entry_get_text(GTK_ENTRY(client->a_window->password_entry)));
		printf("Username = %s, password = %s, %d\n", username_buffer, password_buffer, mx_strlen(password_buffer));

		if(mx_strlen(password_buffer) < 6) {
			gtk_label_set_text(GTK_LABEL(client->a_window->err_label), (const gchar*)"Password should be at least 6 characters");
			return;
		}

		const char* hash = mx_hash_sha256(password_buffer);
		printf("Hash => %s\n", hash);


		request_t* request = malloc(sizeof(request_t));
		request->req =  mx_create_login_req(username_buffer, hash);
		mx_queue_push(client->request_queue, request);
}
