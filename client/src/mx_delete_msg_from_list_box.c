#include "../inc/client.h"

static inline gboolean del_msg_from_list_box_in_gtk_loop(gpointer data){

	del_msg_from_list_box_data_t* del_data = (del_msg_from_list_box_data_t*)data;
	client_t* client = del_data->client;
	int message_id = del_data->message_id;

	GtkListBoxRow* row = GTK_LIST_BOX_ROW(mx_map_get(client->message_id_row_map, message_id));
	if(row != NULL){
		gtk_container_remove(GTK_CONTAINER(client->c_window->msgs_list_box), GTK_WIDGET(row));
		gtk_widget_destroy(GTK_WIDGET(row));
		mx_map_remove(client->message_id_row_map, message_id);
	}

	free(data);
	return FALSE;
}

void mx_delete_msg_from_list_box(client_t* client, int message_id){
	del_msg_from_list_box_data_t* data = (del_msg_from_list_box_data_t*)malloc(sizeof(del_msg_from_list_box_data_t));
	data->client = client;
	data->message_id = message_id;

	g_idle_add(del_msg_from_list_box_in_gtk_loop, data);
}
