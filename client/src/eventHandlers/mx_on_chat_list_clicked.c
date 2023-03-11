#include "../../inc/client.h"

void mx_on_chat_list_clicked(GtkWidget* w, gpointer data) {
	client_t* client = (client_t*)data;

	gtk_label_set_text(client->c_window->chat_name, (char*)gtk_button_get_label(w));
}