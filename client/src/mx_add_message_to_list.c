#include "../inc/client.h"

void mx_add_message_to_list(chat_window_t* window, const char *sender_name, const char *message_text, const char *sending_time, gboolean is_your_message) {

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
	gtk_widget_show_all(window->msgs_list_box);
	GtkListBoxRow* row = gtk_list_box_row_new();
	gtk_container_add(row, msg_main_box);
	gtk_list_box_insert(GTK_LIST_BOX(window->msgs_list_box), row, -1);
	
	GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(window->msgs_list_scrlld_wnd));
   	gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));
}


