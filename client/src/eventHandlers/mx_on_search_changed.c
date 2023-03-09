#include "../../inc/client.h"

void mx_on_search_changed(GtkWidget *w, gpointer data) {
	client_t* client = (client_t*)data;

	if(!client->can_send_req)
			return;

	char pattern[33];

	sprintf(pattern, "%s", gtk_entry_get_text(w));

	request_t* request = malloc(sizeof(request_t));
	request->req =  mx_strlen(pattern) == 0 ? mx_create_get_joined_chats_req() : mx_create_search_chat_req(pattern);
	client->current_request = request;
}