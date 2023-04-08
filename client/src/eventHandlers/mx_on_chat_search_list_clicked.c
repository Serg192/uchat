#include "../../inc/client.h"

static inline void test(GtkWidget* w, gpointer data) {
	client_t* client = (client_t*)data;

	if(!client->can_send_req)
		return;

	request_t* request = (request_t*)malloc(sizeof(request_t));
	request->req = mx_create_join_chat_req(client->join_chat_id);


	client->current_request = request;

	mx_hide_hint_window(client->d_window->window, NULL);
}

void mx_on_chat_search_list_clicked(GtkWidget* w, gpointer data) {
	client_t* client = (client_t*)data;

	client->join_chat_id = client->chat_id_chat_btn_map[mx_get_chat_id_from_btn(w, client)];

	const char *text = (char*)gtk_button_get_label(w);

	char* title;
	asprintf(&title, "Join the %s grop?", text);

	gtk_label_set_text(client->d_window->info_lable, title);

	free(title);

	g_signal_connect(client->d_window->ok_btn, "released", G_CALLBACK(test), client);

	gtk_widget_show_all(client->d_window->window);
	gtk_widget_grab_focus (client->d_window->window);
}