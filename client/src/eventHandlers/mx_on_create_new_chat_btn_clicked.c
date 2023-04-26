#include "../../inc/client.h"

void mx_on_create_new_chat_btn_clicked(GtkButton* b, gpointer data) {
	(void)b;

	char chat_name_buffer[33];

	client_t* client = (client_t*)data;

	sprintf(chat_name_buffer, "%s", gtk_entry_get_text(GTK_ENTRY(client->create_chat_window->chat_name_entry)));

	request_t* request = malloc(sizeof(request_t));
	request->req =  mx_create_chat_req(chat_name_buffer, rand() % 8);
	mx_queue_push(client->request_queue, request);
}

