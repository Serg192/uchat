#include "../../inc/client.h"

static inline gboolean f(gpointer data) {
	client_t* client = (client_t*)data;

	mx_hide_hint_window(client->create_chat_window->window, NULL);
	
	return TRUE;
}

void mx_handle_chat_creation(client_t* client) {
	g_idle_add(f, client);

	request_t* request = malloc(sizeof(request_t));
	request->req =  mx_create_get_joined_chats_req();

	mx_queue_push(client->request_queue, request);
}

