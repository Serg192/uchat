#include "../../inc/client.h"

//deprecated
/*
void mx_on_search_changed(GtkWidget *w, gpointer data) {
	(void)w;

	client_t* client = (client_t*)data;

	double time_taken = ((double) clock() - client->c_window->search_t) / CLOCKS_PER_SEC;

	char pattern[33];

	sprintf(pattern, "%s", gtk_entry_get_text(w));

	const int pattern_len = mx_strlen(pattern);

	request_t* request = malloc(sizeof(request_t));
	request->req =  pattern_len == 0 ? mx_create_get_joined_chats_req() : mx_create_search_chat_req(pattern);
	
	mx_queue_push(client->request_queue, request);

	client->search_mode = pattern_len != 0;

}*/

void mx_on_search_changed(GtkWidget *w, gpointer data){

	client_t* client = (client_t*)data;

	char pattern[33];
	sprintf(pattern, "%s", gtk_entry_get_text((GtkEntry*)w));

	char* trimmed_str = mx_strtrim(pattern);
	char* prepared_text = mx_prepare_str_for_sql(trimmed_str);

	const int pattern_len = mx_strlen(prepared_text);

	request_t* request = malloc(sizeof(request_t));
	request->req =  pattern_len == 0 ? mx_create_get_joined_chats_req() : mx_create_search_chat_req(prepared_text);
	
	mx_queue_push(client->request_queue, request);

	client->search_mode = pattern_len != 0;

	if(trimmed_str)
		free(trimmed_str);
	if(prepared_text)
		free(prepared_text);
}
