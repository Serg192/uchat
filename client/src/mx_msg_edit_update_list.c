#include "../inc/client.h"

/*
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
*/

typedef struct edit_msg_s {
	client_t* client;
	int message_id;
	char* text;
}              edit_msg_t;

static inline gboolean edit_msg_in_gtk_loop(gpointer data){
	edit_msg_t* d = (edit_msg_t*)data;
	client_t* client = d->client;
	int message_id = d->message_id; 
	char* text = d->text;

	GtkListBoxRow* row = GTK_LIST_BOX_ROW(mx_map_get(client->message_id_row_map, message_id));

	if(row != NULL){
		message_t *message = (message_t*) g_object_get_data(G_OBJECT(row), "message_data");
		gtk_label_set_text(GTK_LABEL(message->text_label), text);
	}


	free(text);
	free(d);
	return FALSE;
}

void mx_msg_edit_update_list(client_t* client, int message_id, char* text){
	edit_msg_t* data = (edit_msg_t*)malloc(sizeof(edit_msg_t));
	data->client = client;
	data->message_id = message_id;
	data->text = mx_strdup(text);

	g_idle_add(edit_msg_in_gtk_loop, data);
}

