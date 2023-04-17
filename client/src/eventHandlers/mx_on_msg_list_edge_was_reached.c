#include "../../inc/client.h"

void mx_on_msg_list_edge_was_reached(GtkScrolledWindow* scrolled_window, GtkPositionType pos, gpointer data) {
    if (pos == GTK_POS_TOP) {
    	client_t* client = (client_t*)data;

    	if(client->first_msg_in_chat_id == -1)
    		return;

       	request_t* request = (request_t*)malloc(sizeof(request_t));

		request->req = mx_create_get_chat_msg_req(client->current_chat_id, MSG_LOAD_ABOVE, client->first_msg_in_chat_id, MSG_LOAD_LIMIT);

		mx_queue_push(client->request_queue, request);
    } 
}
