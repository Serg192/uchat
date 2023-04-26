#include "../../inc/client.h"

void mx_on_edit_btn_clicked(GtkWidget* w, gpointer data){
    //added to prevent warning
    (void)w;
	client_t* client = (client_t*)data;

	GtkTextView* text_view = (GtkTextView*)client->c_window->message_input_field;

	GtkTextBuffer* input_txt_buf = gtk_text_view_get_buffer(text_view);

	GList* selected_rows = gtk_list_box_get_selected_rows(GTK_LIST_BOX(client->c_window->msgs_list_box));
	for (GList* node = selected_rows; node != NULL; node = node->next) {
    	GtkListBoxRow* row = GTK_LIST_BOX_ROW(node->data);
    	//printf("Entry\n");

    	message_t *message = (message_t*) g_object_get_data(G_OBJECT(row), "message_data");
    	
    	//Can delete only your own messages
    	if(mx_strcmp(message->username_from, "You") == 0) {
    		client->edit_mode = true;
    		client->message_to_edit = message;
    		gtk_text_buffer_set_text(input_txt_buf, message->messages_str, mx_strlen(message->messages_str));

    	}
    	
	}
	g_list_free(selected_rows);
	gtk_list_box_unselect_all(client->c_window->msgs_list_box);
}
