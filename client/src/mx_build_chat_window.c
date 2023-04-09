#include "../inc/client.h"

chat_window_t* mx_build_chat_window(client_t* client) {


	chat_window_t* window = malloc(sizeof(chat_window_t));
	window->search_t = clock();

	window->builder = gtk_builder_new_from_file(CHAT_WIND_XML_PATH);

	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_window"));

	g_signal_connect(window->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

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
	window->message_input_field = GTK_WIDGET(gtk_builder_get_object(window->builder, "message_input_field"));

	//gtk_search_entry_set_search_delay(window->search_entry, (guint)400);


	//TEST	 
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
	
	g_signal_connect(window->add_chat_btn, "released", G_CALLBACK(mx_on_create_chat_btn_clicked), client);
	g_signal_connect(window->search_entry, "search-changed", G_CALLBACK(mx_on_search_changed), client);
	//g_signal_connect(window->send_message_btn, "released", G_CALLBACK(add_message_to_list), client->c_window);

	return window;
}

