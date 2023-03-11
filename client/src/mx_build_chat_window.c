#include "../inc/client.h"


chat_window_t* mx_build_chat_window(client_t* client) {


	chat_window_t* window = malloc(sizeof(chat_window_t));

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

	//TEST	 
	GtkWidget *row = GTK_WIDGET(gtk_builder_get_object(window->builder, "msg_main_box"));	
	GtkWidget *msg_sender_name_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "msg_sender_name_label"));
	GtkWidget *msg_sending_time_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "msg_sending_time_label"));
	GtkWidget *msg_text_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "msg_text_label"));
	gtk_label_set_text(msg_sender_name_label, "test_name");
	gtk_label_set_text(msg_sending_time_label, "test_time");
	gtk_label_set_text(msg_text_label, "test_text");
	gtk_list_box_insert(window->msgs_list_box, GTK_WIDGET(row), -1);


	g_signal_connect(window->add_chat_btn, "released", G_CALLBACK(mx_on_create_chat_btn_clicked), client);
	g_signal_connect(window->search_entry, "search-changed", G_CALLBACK(mx_on_search_changed), client);

	return window;
}
