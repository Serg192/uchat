#include "../../inc/client.h"

void mx_on_delete_btn_clicked(GtkWidget* w, gpointer data){
	client_t* client = (client_t*)data;

	GList* selected_rows = gtk_list_box_get_selected_rows(GTK_LIST_BOX(client->c_window->msgs_list_box));
	for (GList* node = selected_rows; node != NULL; node = node->next) {
    	GtkListBoxRow* row = GTK_LIST_BOX_ROW(node->data);
    	//printf("Entry\n");

    	message_t *message = (message_t*) g_object_get_data(G_OBJECT(row), "message_data");
    	
    	//Can delete only your own messages
    	if(mx_strcmp(message->username_from, "You") == 0) {

    		//Create request to delete message
    		//printf("************************************DELETE WITH INDEX %d\n", message->id);
    		//gtk_container_remove(GTK_CONTAINER(client->c_window->msgs_list_box), GTK_WIDGET(row));
    		//gtk_widget_destroy(GTK_WIDGET(row));

    		request_t* request = (request_t*)malloc(sizeof(request_t));

		    request->req = mx_create_delete_chat_msg_req(client->current_chat_id, message->id);

		    printf("[FROM DELETE], request is %s\n", request->req);

		    mx_queue_push(client->request_queue, request);
    	}
    	
	}
	g_list_free(selected_rows);
}

/*

add delete_message_queue for each client in server

int id -> client




#include <gtk/gtk.h>

#define TABLE_SIZE 100

int main(int argc, char *argv[]) {
    // Create a hash table with int keys and GtkWidget values
    GHashTable *table = g_hash_table_new_full(g_int_hash, g_int_equal, g_free, NULL);

    // Insert rows into the hash table with message IDs as keys
    GtkWidget *row1 = gtk_label_new("Message 1");
    g_hash_table_insert(table, GINT_TO_POINTER(1), row1);

    GtkWidget *row2 = gtk_label_new("Message 2");
    g_hash_table_insert(table, GINT_TO_POINTER(2), row2);

    GtkWidget *row3 = gtk_label_new("Message 3");
    g_hash_table_insert(table, GINT_TO_POINTER(3), row3);

    // Look up a row by message ID and remove it from the list box
    int message_id_to_remove = 2;
    GtkWidget *row_to_remove = g_hash_table_lookup(table, GINT_TO_POINTER(message_id_to_remove));
    if (row_to_remove != NULL) {
        gtk_container_remove(GTK_CONTAINER(list_box), row_to_remove);
        g_hash_table_remove(table, GINT_TO_POINTER(message_id_to_remove));
    }

    // Free the hash table and its values (but not its keys)
    g_hash_table_destroy(table);

    return 0;
}


*/

