#include "../inc/client.h"

static char* format_sending_time(int sending_time) {
    char* sending_time_str = (char*)malloc(sizeof(char) * 6);
    int hour = sending_time / 100;
    int minute = sending_time % 100;

    snprintf(sending_time_str, 6, "%02d:%02d", hour, minute);

    return sending_time_str;
}

static inline gboolean push_message_in_gtk_loop(gpointer data){
	push_msg_data_t* msg_data = (push_msg_data_t*)data;
	client_t* client = msg_data->client;
	message_t* message = msg_data->message;
	int mode = msg_data->mode;

	chat_window_t* window = client->c_window;
	char *sender_name = message->username_from; 
	char *message_text = message->messages_str;
    //char *sending_time = mx_itoa(message->sending_time);
    char* sending_time = format_sending_time(message->sending_time);
    //Test with "00:mm" format
    //char* sending_time = format_sending_time(45);
    
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

	free(msg_data);
	free(sending_time);
	return FALSE;
}


void  mx_push_message(client_t* client, message_t* message, int mode) {

	push_msg_data_t* data = (push_msg_data_t*)malloc(sizeof(push_msg_data_t));
	data->client = client;
	data->message = message;
	data->mode = mode;

	g_idle_add(push_message_in_gtk_loop, data);
}
