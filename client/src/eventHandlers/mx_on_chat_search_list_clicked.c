#include "../../inc/client.h"

static inline void test(GtkWidget* w, gpointer data) {
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
	client_t* client = (client_t*)data;

	int *chat_id = (int *)g_object_get_data(G_OBJECT(w), "chat_id");

	client->join_chat_id = GPOINTER_TO_INT(chat_id); //mx_get_chat_id_from_btn(w, client);

	//const char *text = (char*)gtk_button_get_label(w);
	//const char *text = (char*)gtk_button_get_label(GTK_BUTTON(w));
    const char *text = gtk_button_get_label(GTK_BUTTON(w));

	char* title;
	asprintf(&title, "Join the %s grop?", text);

	//gtk_label_set_text(client->d_window->info_lable, title);
	gtk_label_set_text(GTK_LABEL(client->d_window->info_label), title);

	free(title);

	g_signal_connect(client->d_window->ok_btn, "released", G_CALLBACK(test), client);

	gtk_widget_show_all(client->d_window->window);
	gtk_widget_grab_focus (client->d_window->window);
}
