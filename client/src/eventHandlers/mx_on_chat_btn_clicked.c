#include "../../inc/client.h"

void mx_on_chat_btn_clicked(GtkButton* b, gpointer data) {
	//chat_info_t* inf = (chat_info_t*)data;
	client_t* client = (client_t*)data;
	if (mx_strcmp(gtk_label_get_text(client->c_window->chat_name_label), gtk_button_get_label(b)) == 0
		&& (gtk_widget_is_visible(client->c_window->selected_chat_box))) {
		gtk_widget_hide(client->c_window->selected_chat_box);
	}
	else {
		gtk_label_set_text(client->c_window->chat_name_label, gtk_button_get_label(b));
		gtk_widget_show(client->c_window->selected_chat_box);
	}
}

