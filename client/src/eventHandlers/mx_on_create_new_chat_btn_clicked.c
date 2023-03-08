#include "../../inc/client.h"

void mx_on_create_new_chat_btn_clicked(GtkButton* b, gpointer data) {

	printf("Create btn clicked\n");

	char chat_name_buffer[33];

	client_t* client = (client_t*)data;

		if(!client->can_send_req)
			return;

//	printf();

	sprintf(chat_name_buffer, "%s", gtk_entry_get_text(client->create_chat_window->chat_name_entry));

	request_t* request = malloc(sizeof(request_t));
	request->req =  mx_create_chat_req(chat_name_buffer);
	client->current_request = request;
}

