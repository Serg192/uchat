#include "../../inc/client.h"

static inline void on_join_btn_clicked(GtkWidget* w, gpointer data) {
    //added to prevent warning
    (void)w;
	client_t* client = (client_t*)data;

	request_t* request = (request_t*)malloc(sizeof(request_t));
	request->req = mx_create_join_chat_req(client->join_chat_id);
	mx_queue_push(client->request_queue, request);

	mx_hide_hint_window(client->d_window->window, NULL);
}

void mx_on_chat_search_list_clicked(GtkWidget* w, gpointer data) {
	
	(void)w;
	on_chat_clicked_data_t* c_data = (on_chat_clicked_data_t*)data;
	client_t* client = c_data->client;
	const char* text = c_data->chat_name;

	int *chat_id = (int *)g_object_get_data(G_OBJECT(w), "chat_id");

	client->join_chat_id = GPOINTER_TO_INT(chat_id); //mx_get_chat_id_from_btn(w, client);

	char* title;
    int len = snprintf(NULL, 0, "Join the %s group?", text);
    title = malloc(len + 1);
    sprintf(title, "Join the %s group?", text);

	gtk_label_set_text(GTK_LABEL(client->d_window->info_label), title);

	free(title);

	g_signal_connect(client->d_window->ok_btn, "released", G_CALLBACK(on_join_btn_clicked), client);

	gtk_widget_show_all(client->d_window->window);
	gtk_widget_grab_focus (client->d_window->window);
}
