#include "../../inc/client.h"

void mx_on_user_profile_btn_clicked(GtkWidget* widget, gpointer data) {
	(void)widget;
	client_t* client = (client_t*)data;

    // gtk_entry_set_text(GTK_ENTRY(client->u_window->username_entry), "");
    // gtk_entry_set_text(GTK_ENTRY(client->u_window->password_entry), "");

	gtk_widget_show_all(client->u_window->window);
}