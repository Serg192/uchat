#include "../../inc/client.h"

void mx_on_log_out_btn_clicked(GtkWidget* w, gpointer data){
	(void)w;
	client_t* client = (client_t*)data;

	gtk_widget_hide(client->c_window->selected_chat_box);    
    mx_room_data_clear(client);
    gtk_widget_hide(client->c_window->window);

    gtk_entry_set_text(GTK_ENTRY(client->a_window->username_entry), "");
    gtk_entry_set_text(GTK_ENTRY(client->a_window->password_entry), "");

    gtk_label_set_text(GTK_LABEL(client->a_window->err_label), (const gchar*)"");

    gtk_widget_show_all(client->a_window->window);
}
