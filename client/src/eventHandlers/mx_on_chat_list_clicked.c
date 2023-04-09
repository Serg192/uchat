#include "../../inc/client.h"

//Example of usage mx_create_get_chat_msg_req
//see mx_create_get_chat_msg_req.c file to know more
static inline void test_load_msg(client_t* client){
	request_t* request = (request_t*)malloc(sizeof(request_t));

	request->req = mx_create_get_chat_msg_req(client->current_chat_id, MSG_LOAD_ABOVE, MSG_ID_LAST, 3);

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
	    client->current_chat_id = -1;
	}
	else {
		gtk_label_set_text(client->c_window->chat_name_label, gtk_button_get_label(w));
		gtk_widget_show(client->c_window->selected_chat_box);

		//set current chat id
		client->current_chat_id = mx_get_chat_id_from_btn(w, client);

		//test function
		test_load_msg(client);
	}
}

