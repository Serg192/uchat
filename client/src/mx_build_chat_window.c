#include "../inc/client.h"

void mx_on_selection_changed(GtkListBox *listbox, gpointer data) {
	client_t* client = (client_t*)data;
	

	GtkStack *stack = client->c_window->entry_edit_stack;
	GList *selected_rows = gtk_list_box_get_selected_rows(listbox);

	if (selected_rows != NULL) {
    	gtk_stack_set_visible_child_name(stack, "page1");
		
		if (g_list_length(selected_rows) > 1) {
			gtk_widget_hide(client->c_window->selected_msg_edit_btn);
		}
		else {
			gtk_widget_show(client->c_window->selected_msg_edit_btn);
		}
		
		char *selected_count_str = mx_strjoin("Selected: ", mx_itoa(g_list_length(selected_rows)));
		gtk_label_set_text(client->c_window->selected_msg_number_label, selected_count_str);
		free(selected_count_str);
		g_list_free(selected_rows);
  	} else {
    	gtk_stack_set_visible_child_name(stack, "page0");
  	}
}

gboolean mx_on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {

	client_t* client = (client_t*)data;

  	GtkListBox *list_box = GTK_LIST_BOX(widget);
  	GtkListBoxRow *row = gtk_list_box_get_row_at_y(list_box, event->y);

  	if (row != NULL && event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY) {
    	gboolean selected = gtk_list_box_row_is_selected(row);

    	if (selected) {
    	  	gtk_list_box_unselect_row(list_box, row);
    	} 
		else {
    	
			gtk_list_box_select_row(list_box, row);
		 	if(client->edit_mode) {
  				GtkTextView* text_view = (GtkTextView*)client->c_window->message_input_field;
	    		GtkTextBuffer* input_txt_buf = gtk_text_view_get_buffer(text_view);
	   			gtk_text_buffer_set_text(input_txt_buf, "", 0);
	   			client->edit_mode = false;
  		  	}
    	}

    	return TRUE;
  	}

  	return FALSE;
}

void mx_on_selected_msg_close_btn_clicked(GtkButton* b, gpointer data) {
	client_t* client = (client_t*)data;
	gtk_list_box_unselect_all(client->c_window->msgs_list_box);
}

void mx_on_selected_msg_select_all_btn_clicked(GtkButton* b, gpointer data) {
	client_t* client = (client_t*)data;
	gtk_list_box_select_all(client->c_window->msgs_list_box);
}

void mx_app_on_destroy(GtkWidget *widget, gpointer data) {
    client_t* client = (client_t*)data;

	request_t* request = (request_t*)malloc(sizeof(request));
	request->req = mx_create_quit_request();
	request->type = QUIT_REQ;
	mx_queue_push(client->request_queue, request);
    gtk_widget_destroy(widget);
	gtk_main_quit();
}



void mx_on_user_info_btn_clicked(GtkWidget* widget, gpointer data) {
	client_t* client = (client_t*)data;

	gint main_window_x, main_window_y;
	gtk_window_get_position(GTK_WINDOW(client->c_window->window), &main_window_x, &main_window_y);
	gtk_window_set_position(GTK_WINDOW(client->u_window->window), GTK_WIN_POS_MOUSE);
	gtk_window_move(GTK_WINDOW(client->u_window->window), main_window_x + 10, main_window_y + 35);

	gtk_widget_show_all(client->u_window->window);
}

chat_window_t* mx_build_chat_window(client_t* client) {


	chat_window_t* window = malloc(sizeof(chat_window_t));
	window->search_t = clock();

	window->builder = gtk_builder_new_from_file(CHAT_WIND_XML_PATH);

	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_window"));

	g_signal_connect(window->window, "destroy", G_CALLBACK(mx_app_on_destroy), client);
	//g_signal_connect(window->window, "delete_event", G_CALLBACK(on_window_close), client);

	gtk_window_set_position(GTK_WINDOW(window->window), GTK_WIN_POS_CENTER_ALWAYS);

	gtk_builder_connect_signals(window->builder, NULL);

	window->main_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "main_box"));
	window->chats_list_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_box"));
	window->chats_list_scrlldwnd = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_scrlldwnd"));
	window->chats_list_view = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_view"));
	window->chats_list_grid = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_grid"));
	window->add_chat_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "add_chat_btn"));
	window->search_entry = GTK_WIDGET(gtk_builder_get_object(window->builder, "search_entry"));
	window->user_info_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "user_info_btn"));
	window->elements_were_attached = 0;
	
	window->selected_chat_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_chat_box"));
	window->chat_name_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_name_label"));
	window->chat_settings_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_settings_btn"));
	window->chat_info_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_info_btn"));
	window->msgs_list_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "msgs_list_box"));
	window->msgs_list_scrlld_wnd = GTK_WIDGET(gtk_builder_get_object(window->builder, "msgs_list_scrlld_wnd"));
	window->send_message_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "send_message_btn"));
	window->entry_edit_stack = GTK_WIDGET(gtk_builder_get_object(window->builder, "entry_edit_stack"));
	window->selected_msg_close_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_close_btn"));
	window->selected_msg_number_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_number_label"));
	window->selected_msg_edit_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_edit_btn"));
	window->selected_msg_select_all_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_select_all_btn"));
	window->selected_msg_delete_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_delete_btn"));
	window->message_input_field = GTK_WIDGET(gtk_builder_get_object(window->builder, "message_input_field"));

	//just test
	g_signal_connect(window->send_message_btn, "released", G_CALLBACK(mx_on_send_msg_btn_clicked), client);
	//

	//gtk_search_entry_set_search_delay(window->search_entry, (guint)400);

	g_signal_connect(window->add_chat_btn, "released", G_CALLBACK(mx_on_create_chat_btn_clicked), client);
	g_signal_connect(window->search_entry, "search-changed", G_CALLBACK(mx_on_search_changed), client);
	g_signal_connect(window->msgs_list_box, "button-press-event", G_CALLBACK(mx_on_button_press_event), client);
	g_signal_connect(window->msgs_list_box, "selected-rows-changed", G_CALLBACK(mx_on_selection_changed), client);
	g_signal_connect(window->selected_msg_close_btn, "released", G_CALLBACK(mx_on_selected_msg_close_btn_clicked), client);
	g_signal_connect(window->selected_msg_select_all_btn, "released", G_CALLBACK(mx_on_selected_msg_select_all_btn_clicked), client);
	g_signal_connect(window->selected_msg_edit_btn, "released", G_CALLBACK(mx_on_edit_btn_clicked), client);

	g_signal_connect(window->msgs_list_scrlld_wnd, "edge-reached", G_CALLBACK(mx_on_msg_list_edge_was_reached), client);
	g_signal_connect(window->selected_msg_delete_btn, "released", G_CALLBACK(mx_on_delete_btn_clicked), client);

	//Doesn't work 
	//g_signal_connect(window->chat_settings_btn, "released", G_CALLBACK(mx_on_leave_btn_clicked), client);
	g_signal_connect(window->chat_settings_btn, "released", G_CALLBACK(mx_on_del_account_btn_clicked), client);
	g_signal_connect(window->chat_info_btn, "released", G_CALLBACK(mx_on_chat_info_btn_clicked), client);
	g_signal_connect(window->user_info_btn, "released", G_CALLBACK(mx_on_user_info_btn_clicked), client);
	
	return window;
}

