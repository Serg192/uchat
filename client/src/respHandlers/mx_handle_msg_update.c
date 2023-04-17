#include "../../inc/client.h"

enum {
	PUSH_FRONT,
	PUSH_BACK
};

typedef struct message_s {
	int id;
	int sending_time;
	int sending_date;
	char* username_from;
	char* messages_str;

}              message_t;
static inline void  mx_push_message(chat_window_t* window, message_t* message, int mode) {


	char *sender_name = message->username_from; 
	char *message_text = message->messages_str;
	char *sending_time = mx_itoa(message->sending_time);
	gboolean is_your_message = mx_strcmp(sender_name, "You") == 0;
	                                

	GtkWidget* msg_main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_margin_start(msg_main_box, 50);
	gtk_widget_set_margin_end(msg_main_box, 50);
	gtk_widget_set_halign(msg_main_box, is_your_message ? GTK_ALIGN_END : GTK_ALIGN_START);

	GtkWidget* msg_sender_name_label = gtk_label_new(sender_name);
	gtk_widget_set_halign(msg_sender_name_label, GTK_ALIGN_START);
	gtk_widget_set_margin_start(msg_sender_name_label, 5);
	gtk_widget_set_margin_end(msg_sender_name_label, 5);
	gtk_box_pack_start(GTK_BOX(msg_main_box), msg_sender_name_label, FALSE, TRUE, 0);
	
	GtkWidget* msg_text_label = gtk_label_new(message_text);
	gtk_widget_set_halign(msg_text_label, GTK_ALIGN_START);
	gtk_widget_set_margin_start(msg_text_label, 5);
	gtk_widget_set_margin_end(msg_text_label, 25);
	gtk_label_set_line_wrap(GTK_LABEL(msg_text_label), TRUE);
	gtk_label_set_line_wrap_mode(GTK_LABEL(msg_text_label), PANGO_WRAP_WORD_CHAR);
	//gtk_label_set_max_width_chars(GTK_LABEL(msg_text_label), (int)gtk_widget_get_allocated_width(window->selected_chat_box) * 0.25);
	gtk_label_set_max_width_chars(GTK_LABEL(msg_text_label), 80);
	gtk_box_pack_start(GTK_BOX(msg_main_box), msg_text_label, FALSE, TRUE, 5);

	GtkWidget* msg_sending_time_label = gtk_label_new(sending_time);
	gtk_widget_set_halign(msg_sending_time_label, GTK_ALIGN_END);
	gtk_widget_set_margin_start(msg_sending_time_label, 50);
	gtk_widget_set_margin_end(msg_sending_time_label, 5);
	gtk_box_pack_end(GTK_BOX(msg_main_box), msg_sending_time_label, FALSE, TRUE, 0);
    
    //FOR TESTS
	gtk_widget_override_background_color(msg_main_box, GTK_STATE_FLAG_NORMAL, "#1ddecb");
	
	GtkListBoxRow* row = gtk_list_box_row_new();
	gtk_container_add(row, msg_main_box);
	gtk_list_box_insert(GTK_LIST_BOX(window->msgs_list_box), row, (mode == PUSH_BACK ? -1 : 0));

	
	
	GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(window->msgs_list_scrlld_wnd));
   	gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));

   	gtk_widget_show_all(window->msgs_list_box);
}

void mx_handle_msg_update(client_t* client){

	//printf("Handling msg update\n");

	cJSON* msg_json = client->current_response->json;
	cJSON* msg_array = cJSON_GetObjectItemCaseSensitive(msg_json, "messages");

    if (msg_array == NULL || !cJSON_IsArray(msg_array)) {
        return;
    }

    int messages_count = cJSON_GetArraySize(msg_array);

    //printf("Array len is %d\n", messages_count);

  	for(int i = 0; i < messages_count; i++) {
  		 cJSON *message = cJSON_GetArrayItem(msg_array, i);

  		message_t msg_struct;

  		msg_struct.id = cJSON_GetObjectItem(message, "id")->valueint;
  		msg_struct.sending_time = cJSON_GetObjectItem(message, "sending_time")->valueint;
  		msg_struct.sending_date = cJSON_GetObjectItem(message, "sending_date")->valueint;
  		msg_struct.username_from = cJSON_GetObjectItem(message, "username")->valuestring;
  		msg_struct.messages_str = cJSON_GetObjectItem(message, "context")->valuestring;
        
        int mode = client->current_response->type == GET_ABOVE_MSG_RESP ? PUSH_FRONT : PUSH_BACK;

        if(client->last_msg_in_chat_id != msg_struct.id && client->first_msg_in_chat_id != msg_struct.id)
        	mx_push_message(client->c_window, &msg_struct, mode);

        if(mode == PUSH_BACK){
        	if(client->first_msg_in_chat_id == 1 && i == 0)
        		client->first_msg_in_chat_id = msg_struct.id;
        	client->last_msg_in_chat_id = msg_struct.id;
        }

        if(mode == PUSH_FRONT){
        	if(client->last_msg_in_chat_id == -1 && i == 0)
        		client->last_msg_in_chat_id = msg_struct.id;
        	client->first_msg_in_chat_id = msg_struct.id;
        }

  	}

  	//gtk_list_box_scroll_to_row(client->c_window->msgs_list_box, gtk_list_box_get_row_at_index(client->c_window->msgs_list_box, -1), 0.0, FALSE, 0.0, 0.0);
  //	printf("After update last msg is %d\n", client->last_msg_in_chat_id);

    //cJSON_Delete(msg_array);
}
