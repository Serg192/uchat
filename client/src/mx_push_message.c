#include "../inc/client.h"

static char* format_sending_time(int sending_time, int sending_date) {
	int hour = sending_time / 100;
    int minute = sending_time % 100;
    int year = sending_date % 10000;
    int month = sending_date % 1000000 / 10000;
    int day = sending_date / 1000000;

	char *sending_time_str = (char*)malloc(sizeof(char) * 18);
    snprintf(sending_time_str, 18, "%02d/%02d/%04d  %02d:%02d", day, month, year, hour, minute);

    return sending_time_str;
}

static inline void set_new_adjustment(chat_window_t *window, GtkWidget* row, gdouble old_value, int mode) {
	GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(window->msgs_list_scrlld_wnd));
	gtk_adjustment_set_page_size(adjustment, 0);

	GList *count_rows = gtk_container_get_children(GTK_CONTAINER(window->msgs_list_box));
	int num_rows = g_list_length(count_rows);
	
	gint added_message_height = -1;
	gtk_widget_get_preferred_height(row, NULL, &added_message_height);
	gtk_adjustment_set_upper(adjustment, gtk_adjustment_get_upper(adjustment) + added_message_height);

	if (mode == PUSH_BACK || num_rows <= MSG_LOAD_LIMIT) {
		gtk_adjustment_set_value(adjustment, gtk_adjustment_get_upper(adjustment) + added_message_height);
	}
	else {
		gtk_adjustment_set_value(adjustment, old_value + added_message_height);
	}

	g_list_free(count_rows);
}

static inline gboolean push_message_in_gtk_loop(gpointer data) {
	push_msg_data_t* msg_data = (push_msg_data_t*)data;
	client_t* client = msg_data->client;
	message_t* message = msg_data->message;
	int mode = msg_data->mode;

	chat_window_t* window = client->c_window;
	char *sender_name = message->username_from; 
	char *message_text = message->messages_str;
    char* sending_time = format_sending_time(message->sending_time, message->sending_date);
    
	gboolean is_your_message = mx_strcmp(sender_name, "You") == 0;
    
	GtkWidget* msg_main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_size_request (msg_main_box, 200, 75);
	gtk_widget_set_halign(msg_main_box, is_your_message ? GTK_ALIGN_END : GTK_ALIGN_START);
	gtk_widget_set_name(msg_main_box, is_your_message ? "msg_main_box_you" : "msg_main_box_other");
	mx_widget_add_styles(msg_main_box);

	GtkWidget* msg_sender_name_label = gtk_label_new(sender_name);
	gtk_widget_set_halign(msg_sender_name_label, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(msg_main_box), msg_sender_name_label, FALSE, TRUE, 0);
	gtk_widget_set_name(msg_sender_name_label, "msg_sender_name_label");
	mx_widget_add_styles(msg_sender_name_label);
	
	GtkWidget* msg_text_label = gtk_label_new(message_text);
	gtk_widget_set_halign(msg_text_label, GTK_ALIGN_START);
	gtk_label_set_line_wrap(GTK_LABEL(msg_text_label), TRUE);
	gtk_label_set_line_wrap_mode(GTK_LABEL(msg_text_label), PANGO_WRAP_WORD_CHAR);
	gtk_label_set_max_width_chars(GTK_LABEL(msg_text_label), 75);
	gtk_box_pack_start(GTK_BOX(msg_main_box), msg_text_label, FALSE, TRUE, 0);
	gtk_widget_set_name(msg_text_label, "msg_text_label");
	mx_widget_add_styles(msg_text_label);

	GtkWidget* msg_sending_time_label = gtk_label_new(sending_time);
	gtk_widget_set_halign(msg_sending_time_label, GTK_ALIGN_END);
	gtk_box_pack_start(GTK_BOX(msg_main_box), msg_sending_time_label, FALSE, TRUE, 0);
	gtk_widget_set_name(msg_sending_time_label, "msg_sending_time_label");
	mx_widget_add_styles(msg_sending_time_label);

	
	GtkWidget *row = GTK_WIDGET(gtk_list_box_row_new());
	gtk_widget_set_name(GTK_WIDGET(row), "msg_row");
	mx_widget_add_styles(GTK_WIDGET(row));
    
	message->text_label = msg_text_label;

	g_object_set_data(G_OBJECT(row), "message_data", message);

	mx_map_put(client->message_id_row_map, message->id, row);

	GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(window->msgs_list_scrlld_wnd));
	gdouble old_value = gtk_adjustment_get_value(adjustment);
	
	gtk_container_add(GTK_CONTAINER(row), msg_main_box);
	gtk_list_box_insert(GTK_LIST_BOX(window->msgs_list_box), GTK_WIDGET(row), (mode == PUSH_BACK ? -1 : 0));
    gtk_widget_show_all(window->msgs_list_box);
   
	set_new_adjustment(window, GTK_WIDGET(row), old_value, mode);

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
