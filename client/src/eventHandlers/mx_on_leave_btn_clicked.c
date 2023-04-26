#include "../../inc/client.h"

void mx_on_leave_btn_clicked(GtkButton* b, gpointer data){
    //added to prevent warning
    (void)b;
	client_t* client = (client_t*)data;

	const int chat_to_leave = client->current_chat_id;

	gtk_widget_hide(client->i_window->window);
	gtk_widget_hide(client->c_window->selected_chat_box);
	mx_room_data_clear(client);

	request_t* leave_req = (request_t*)malloc(sizeof(request_t));
	leave_req->req = mx_create_leave_chat_req(chat_to_leave);

	request_t* update_chat_list_req = (request_t*)malloc(sizeof(request_t));
	update_chat_list_req->req = mx_create_get_joined_chats_req();

	

	mx_queue_push(client->request_queue, leave_req);
	mx_queue_push(client->request_queue, update_chat_list_req);
}

