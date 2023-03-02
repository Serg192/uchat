#include "../../inc/client.h"

static inline gboolean change_windows(gpointer data) {
	client_t* client = (client_t*) data;

    gtk_widget_hide(client->a_window->window);


    chat_window_t* c_window = mx_build_chat_window(client);

    client->c_window = c_window;

    gtk_widget_show(client->c_window->window);

    return FALSE;
}

void mx_handle_auth_success(client_t* client) {
	//gtk_widget_hide(client->a_window->window); 

	g_idle_add(change_windows, client);

	//gtk_widget_show(client->a_window->window);
}
