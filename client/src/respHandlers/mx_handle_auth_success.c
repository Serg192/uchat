#include "../../inc/client.h"


static inline gboolean change_windows(gpointer data) {
	client_t* client = (client_t*) data;




////////////////////////
	if(client->can_send_req) {
		request_t* request = malloc(sizeof(request_t));
		request->req =  mx_create_get_joined_chats_req();
		client->current_request = request;
	}

	/////////////////////


    gtk_widget_hide(client->a_window->window);
    gtk_widget_show_all(client->c_window->window);

   // gtk_widget_show_all(client->create_chat_window->window);
   // gtk_widget_grab_focus (client->create_chat_window->window);

    return FALSE;
}

void mx_handle_auth_success(client_t* client) {
	//gtk_widget_hide(client->a_window->window); 

	g_idle_add(change_windows, client);

	//gtk_widget_show(client->a_window->window);
}
