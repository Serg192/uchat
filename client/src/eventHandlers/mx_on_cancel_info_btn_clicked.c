#include "../../inc/client.h"

void mx_on_cancel_info_btn_clicked(GtkWidget* widget, gpointer data) {
    (void)widget;
    client_t* client = (client_t*)data;
    gtk_widget_hide(client->i_window->window);
}
