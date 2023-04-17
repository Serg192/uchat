#include "../../inc/client.h"


static inline void msg_list_clear(client_t* client) {
	 gtk_container_foreach(GTK_CONTAINER(client->c_window->msgs_list_box), (GtkCallback)gtk_widget_destroy, NULL);
}

static inline room_data_clear(client_t* client){
	    client->current_chat_id = -1;
	    client->last_msg_in_chat_id = -1;
	    client->first_msg_in_chat_id = -1;

	    msg_list_clear(client);
}

static inline void push_load_recent_request(client_t* client){
	request_t* request = (request_t*)malloc(sizeof(request_t));

	request->req = mx_create_get_chat_msg_req(client->current_chat_id, MSG_LOAD_ABOVE, MSG_ID_LAST, MSG_LOAD_LIMIT);

	mx_queue_push(client->request_queue, request);
}

void mx_on_chat_list_clicked(GtkWidget* w, gpointer data) {
	(void)w;
	//client_t* client = (client_t*)data;

	//gtk_label_set_text(client->c_window->chat_name, (char*)gtk_button_get_label(w));
	
	client_t* client = (client_t*)data;
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
		client->current_chat_id = mx_get_chat_id_from_btn(w, client);

		//load recent messages
		push_load_recent_request(client);
	}
}

