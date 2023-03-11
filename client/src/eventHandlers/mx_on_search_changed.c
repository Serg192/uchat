#include "../../inc/client.h"

void mx_on_search_changed(GtkWidget *w, gpointer data) {
	client_t* client = (client_t*)data;

	if(!client->can_send_req)
			return;

	double time_taken = ((double) clock() - client->c_window->search_t) / CLOCKS_PER_SEC;

	char pattern[33];

	sprintf(pattern, "%s", gtk_entry_get_text(w));

	const int pattern_len = mx_strlen(pattern);

	if(time_taken >= 1.0f) {
		client->c_window->search_t = clock();
	} else if(pattern_len != 0)
		return;

	request_t* request = malloc(sizeof(request_t));
	request->req =  pattern_len == 0 ? mx_create_get_joined_chats_req() : mx_create_search_chat_req(pattern);
	client->current_request = request;

	client->search_mode = pattern_len != 0;
}