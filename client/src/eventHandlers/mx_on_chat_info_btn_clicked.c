#include "../../inc/client.h"

void mx_on_chat_info_btn_clicked(GtkWidget* widget, gpointer data) {
    //added to prevent warning
    (void)widget;
	client_t* client = (client_t*)data;

	if(client->i_window->visible){
		gtk_widget_hide(client->i_window->window);
		client->i_window->visible = false;
		gtk_container_foreach(GTK_CONTAINER(client->i_window->members_list_box), (GtkCallback)gtk_widget_destroy, NULL);


		 while(client->p_change_clear_list){
	    	perm_change_t* p = (perm_change_t*)(client->p_change_clear_list->data);
	    	free(p);
	    	mx_pop_front(&(client->p_change_clear_list));
	    }

	    client->p_change_clear_list = NULL;

		return;
	}

    gtk_label_set_text(GTK_LABEL(client->i_window->chat_info_name_label), 
    gtk_label_get_text(GTK_LABEL(client->c_window->chat_name_label)));

	request_t* request = (request_t*)malloc(sizeof(request));
	request->req = mx_create_get_chat_participants_req(client->current_chat_id);
	mx_queue_push(client->request_queue, request);

	gtk_widget_show_all(client->i_window->window);
	client->i_window->visible = true;
}
