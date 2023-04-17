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
    	
		g_list_free(selected_rows);
  	} else {
    	gtk_stack_set_visible_child_name(stack, "page0");
  	}
}

gboolean mx_on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
  	GtkListBox *list_box = GTK_LIST_BOX(widget);
  	GtkListBoxRow *row = gtk_list_box_get_row_at_y(list_box, event->y);

  	if (row != NULL && event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY) {
    	gboolean selected = gtk_list_box_row_is_selected(row);

    	if (selected) {
    	  gtk_list_box_unselect_row(list_box, row);
    	} else {
    	  gtk_list_box_select_row(list_box, row);
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
	window->selected_msg_edit_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_edit_btn"));
	window->selected_msg_select_all_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_select_all_btn"));
	window->message_input_field = GTK_WIDGET(gtk_builder_get_object(window->builder, "message_input_field"));

	//just test
	g_signal_connect(window->send_message_btn, "released", G_CALLBACK(mx_on_send_msg_btn_clicked), client);
	//

	//gtk_search_entry_set_search_delay(window->search_entry, (guint)400);


	//TEST	 
	/*
	mx_add_message_to_list(window, "You", "Hello", "20:00", TRUE);
	mx_add_message_to_list(window, "Someone", "", "22:44", FALSE);
	mx_add_message_to_list(window, "Someone", "Create a program that works as a system utility ls . You must implement:•the usage: usage: uls [-l] [file ...]•basic functionality of this command - list of directory contents without flags•processing of file operands for files and directories•the -l  flag, which is one of the most useful flags•the view of extended file attributes and access control lists (ACL)•error handling, as in the original ls . Output uls as the program name instead of ls  where necessary•the multicolumn output format when the option -l  isn't specified","22:45", FALSE);
	mx_add_message_to_list(window, "You", "Create a program that works as a system utility ls . You must implement:•the usage: usage: uls [-l] [file ...]•basic functionality of this command - list of directory contents without flags•processing of file operands for files and directories•the -l  flag, which is one of the most useful flags•the view of extended file attributes and access control lists (ACL)•error handling, as in the original ls . Output uls as the program name instead of ls  where necessary•the multicolumn output format when the option -l  isn't specified","23:20", TRUE);
	mx_add_message_to_list(window, "You", "Hello", "20:00", TRUE);
	mx_add_message_to_list(window, "Someone", "", "22:44", FALSE);
	mx_add_message_to_list(window, "Someone", "Create a program that works as a system utility ls . You must implement:•the usage: usage: uls [-l] [file ...]•basic functionality of this command - list of directory contents without flags•processing of file operands for files and directories•the -l  flag, which is one of the most useful flags•the view of extended file attributes and access control lists (ACL)•error handling, as in the original ls . Output uls as the program name instead of ls  where necessary•the multicolumn output format when the option -l  isn't specified","22:45", FALSE);
	mx_add_message_to_list(window, "You", "Create a program that works as a system utility ls . You must implement:•the usage: usage: uls [-l] [file ...]•basic functionality of this command - list of mx_add_message_to_list contents without flags•processing of file operands for files and directories•the -l  flag, which is one of the most useful flags•the view of extended file attributes and access control lists (ACL)•error handling, as in the original ls . Output uls as the program name instead of ls  where necessary•the multicolumn output format when the option -l  isn't specified","23:20", TRUE);
	mx_add_message_to_list(window, "You", "Hello", "20:00", TRUE);
	mx_add_message_to_list(window, "Someone", "", "22:44", FALSE);
	mx_add_message_to_list(window, "Someone", "Create a program that works as a system utility ls . You must implement:•the usage: usage: uls [-l] [file ...]•basic functionality of this command - list of directory contents without flags•processing of file operands for files and directories•the -l  flag, which is one of the most useful flags•the view of extended file attributes and access control lists (ACL)•error handling, as in the original ls . Output uls as the program name instead of ls  where necessary•the multicolumn output format when the option -l  isn't specified","22:45", FALSE);
	mx_add_message_to_list(window, "You", "Create a program that works as a system utility ls . You must implement:•the usage: usage: uls [-l] [file ...]•basic functionality of this command - list of directory contents without flags•processing of file operands for files and directories•the -l  flag, which is one of the most useful flags•the view of extended file attributes and access control lists (ACL)•error handling, as in the original ls . Output uls as the program name instead of ls  where necessary•the multicolumn output format when the option -l  isn't specified","23:20", TRUE);
	*/
	g_signal_connect(window->add_chat_btn, "released", G_CALLBACK(mx_on_create_chat_btn_clicked), client);
	g_signal_connect(window->search_entry, "search-changed", G_CALLBACK(mx_on_search_changed), client);
	g_signal_connect(window->msgs_list_box, "button-press-event", G_CALLBACK(mx_on_button_press_event), NULL);
	g_signal_connect(window->msgs_list_box, "selected-rows-changed", G_CALLBACK(mx_on_selection_changed), client);
	g_signal_connect(window->selected_msg_close_btn, "released", G_CALLBACK(mx_on_selected_msg_close_btn_clicked), client);
	g_signal_connect(window->selected_msg_select_all_btn, "released", G_CALLBACK(mx_on_selected_msg_select_all_btn_clicked), client);
	
	return window;
}

