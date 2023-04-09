#include "../../inc/client.h"

void mx_on_create_chat_btn_clicked(GtkButton* b, gpointer data) {
	(void)b;

	client_t* client = (client_t*) data;

	gtk_entry_set_text(GTK_ENTRY(client->create_chat_window->chat_name_entry), "");

	gtk_widget_show_all(client->create_chat_window->window);
    gtk_widget_grab_focus (client->create_chat_window->window);
}
