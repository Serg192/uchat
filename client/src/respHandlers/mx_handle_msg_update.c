#include "../../inc/client.h"

enum {
	PUSH_FRONT,
	PUSH_BACK
};

static inline void  mx_push_message(client_t* client, message_t* message, int mode) {

	chat_window_t* window = client->c_window;
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
    //Doesn't work
	//gtk_widget_override_background_color(msg_main_box, GTK_STATE_FLAG_NORMAL, "#1ddecb");

	GdkRGBA color;
	gdk_rgba_parse(&color, "#FF0F00");
	gtk_widget_override_background_color(msg_main_box, GTK_STATE_FLAG_NORMAL, &color);

	
	GtkListBoxRow* row = gtk_list_box_row_new();

	g_object_set_data(G_OBJECT(row), "message_data", message);

	mx_map_put(client->message_id_row_map, message->id, row);
	
	gtk_container_add(row, msg_main_box);
	gtk_list_box_insert(GTK_LIST_BOX(window->msgs_list_box), row, (mode == PUSH_BACK ? -1 : 0));

	
	
	GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(window->msgs_list_scrlld_wnd));
   	gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));

   	gtk_widget_show_all(window->msgs_list_box);
}

static inline void del_msg_from_list_box(client_t* client, int message_id){
	//gtk_container_remove(GTK_CONTAINER(client->c_window->msgs_list_box), GTK_WIDGET(row));

	GtkListBoxRow* row = GTK_LIST_BOX_ROW(mx_map_get(client->message_id_row_map, message_id));
	if(row != NULL){
		gtk_container_remove(GTK_CONTAINER(client->c_window->msgs_list_box), GTK_WIDGET(row));
		gtk_widget_destroy(GTK_WIDGET(row));
		mx_map_remove(client->message_id_row_map, message_id);
	}
}

void mx_handle_msg_update(client_t* client){

	//printf("Handling msg update\n");

	cJSON* msg_json = client->current_response->json;
	cJSON* msg_array = cJSON_GetObjectItemCaseSensitive(msg_json, "messages");

    if (msg_array == NULL || !cJSON_IsArray(msg_array)) {
        return;
    }

    int messages_count = cJSON_GetArraySize(msg_array);

  	for(int i = 0; i < messages_count; i++) {
  		 cJSON *message = cJSON_GetArrayItem(msg_array, i);

  		message_t* msg_struct = (message_t*)malloc(sizeof(message_t));

  		msg_struct->id = cJSON_GetObjectItem(message, "id")->valueint;
  		msg_struct->sending_time = cJSON_GetObjectItem(message, "sending_time")->valueint;
  		msg_struct->sending_date = cJSON_GetObjectItem(message, "sending_date")->valueint;
  		msg_struct->username_from = mx_strdup(cJSON_GetObjectItem(message, "username")->valuestring);
  		msg_struct->messages_str = cJSON_GetObjectItem(message, "context")->valuestring;
        
        int mode = client->current_response->type == GET_ABOVE_MSG_RESP ? PUSH_FRONT : PUSH_BACK;

        if(client->last_msg_in_chat_id != msg_struct->id && client->first_msg_in_chat_id != msg_struct->id){
        	mx_push_message(client, msg_struct, mode);
        }

        if(mode == PUSH_BACK){
        	if(client->first_msg_in_chat_id == -1 && i == 0)
        		client->first_msg_in_chat_id = msg_struct->id;
        	client->last_msg_in_chat_id = msg_struct->id;
        }

        if(mode == PUSH_FRONT){
        	if(client->last_msg_in_chat_id == -1 && i == 0)
        		client->last_msg_in_chat_id = msg_struct->id;
        	client->first_msg_in_chat_id = msg_struct->id;
        }

        mx_push_back(&(client->messages_in_current_chat), msg_struct);

  	}

  	if(client->last_msg_in_chat_id == -1)
  		client->last_msg_in_chat_id = MSG_ID_LAST;
  	
  	cJSON* deleted_msg_array = cJSON_GetObjectItemCaseSensitive(msg_json, "deleted");
  	int deleted_msg_count = cJSON_GetArraySize(deleted_msg_array);

  	for(int i = 0; i < deleted_msg_count; i++){
  		cJSON *deleted_msg = cJSON_GetArrayItem(deleted_msg_array, i);
  		del_msg_from_list_box(client, cJSON_GetObjectItem(deleted_msg, "msg_id")->valueint);
  	}
}
