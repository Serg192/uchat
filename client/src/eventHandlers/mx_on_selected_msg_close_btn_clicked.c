#include "../../inc/client.h"

void mx_on_selected_msg_close_btn_clicked(GtkButton* b, gpointer data) {
	(void)b;
	client_t* client = (client_t*)data;
	gtk_list_box_unselect_all(GTK_LIST_BOX(client->c_window->msgs_list_box));
}
