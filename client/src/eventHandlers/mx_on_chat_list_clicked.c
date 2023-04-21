#include "../../inc/client.h"

static inline gboolean clear_msg_list(gpointer data){
	client_t* client = (client_t*)data;
	gtk_container_foreach(GTK_CONTAINER(client->c_window->msgs_list_box), (GtkCallback)gtk_widget_destroy, NULL);
	return FALSE;
}

static inline room_data_clear(client_t* client){
	    client->current_chat_id = -1;
	    client->last_msg_in_chat_id = -1;
	    client->first_msg_in_chat_id = -1;

	    g_idle_add(clear_msg_list, client);

	    while(client->messages_in_current_chat){

	    	message_t* msg = (message_t*)(client->messages_in_current_chat->data);

	    	//printf("id %d\n", msg->id);
	    	free(msg->username_from);
	    	free(msg->messages_str);
	    	free(msg);

	    	mx_pop_front(&(client->messages_in_current_chat));
	    }

	    client->messages_in_current_chat = NULL;
}

static inline void push_load_recent_request(client_t* client){
	request_t* request = (request_t*)malloc(sizeof(request_t));

	request->req = mx_create_get_chat_msg_req(client->current_chat_id, MSG_LOAD_ABOVE, MSG_ID_LAST, MSG_LOAD_LIMIT);

	mx_queue_push(client->request_queue, request);
}

void mx_on_chat_list_clicked(GtkWidget* w, gpointer data) {
	(void)w;
	client_t* client = (client_t*)data;

	//gtk_label_set_text(client->c_window->chat_name, (char*)gtk_button_get_label(w));
	
	//client_t* client = (client_t*)data;
	if (mx_strcmp(gtk_label_get_text(client->c_window->chat_name_label), gtk_button_get_label(w)) == 0
		&& (gtk_widget_is_visible(client->c_window->selected_chat_box))) {
		gtk_widget_hide(client->c_window->selected_chat_box);

		//set current chat id
	    
		room_data_clear(client);
	}
	else {

		if(client->current_chat_id != -1) {
			room_data_clear(client);
		}

		gtk_label_set_text(client->c_window->chat_name_label, gtk_button_get_label(w));
		gtk_widget_show(client->c_window->selected_chat_box);

		//set current chat id
		int *chat_id = (int *)g_object_get_data(G_OBJECT(w), "chat_id");
		client->current_chat_id = GPOINTER_TO_INT(chat_id);

		//load recent messages
		push_load_recent_request(client);
	}
}

