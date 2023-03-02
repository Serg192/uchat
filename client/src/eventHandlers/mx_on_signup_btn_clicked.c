#include "../../inc/client.h"

void mx_on_signup_btn_clicked(GtkButton* b, gpointer data) {
		char username_buffer[33];
		char password_buffer[33];

		client_t* client = (client_t*)data;

		if(!client->can_send_req)
			return;

		sprintf(username_buffer, "%s", gtk_entry_get_text(client->a_window->username_entry));
		sprintf(password_buffer, "%s", gtk_entry_get_text(client->a_window->password_entry));

		//printf("Username = %s, password = %s\n", username_buffer, password_buffer);

		request_t* request = malloc(sizeof(request_t));
		request->req =  mx_create_signup_req(username_buffer, password_buffer);
		client->current_request = request;
}