#include "../../inc/client.h"

void mx_on_delete_btn_clicked(GtkWidget* w __attribute__((unused)), gpointer data){
	client_t* client = (client_t*)data;

	GList* selected_rows = gtk_list_box_get_selected_rows(GTK_LIST_BOX(client->c_window->msgs_list_box));
	for (GList* node = selected_rows; node != NULL; node = node->next) {
    	GtkListBoxRow* row = GTK_LIST_BOX_ROW(node->data);
    	//printf("Entry\n");

    	message_t *message = (message_t*) g_object_get_data(G_OBJECT(row), "message_data");
    	
    	//Can delete only your own messages
    	//if(mx_strcmp(message->username_from, "You") == 0) {

    		//Create request to delete message
    		//printf("************************************DELETE WITH INDEX %d\n", message->id);
    		//gtk_container_remove(GTK_CONTAINER(client->c_window->msgs_list_box), GTK_WIDGET(row));
    		//gtk_widget_destroy(GTK_WIDGET(row));

    		request_t* request = (request_t*)malloc(sizeof(request_t));

		    request->req = mx_create_delete_chat_msg_req(client->current_chat_id, message->id);

		    printf("[FROM DELETE], request is %s\n", request->req);

		    mx_queue_push(client->request_queue, request);
    	//}
    	
	}
	g_list_free(selected_rows);
	gtk_list_box_unselect_all(GTK_LIST_BOX(client->c_window->msgs_list_box));
}

