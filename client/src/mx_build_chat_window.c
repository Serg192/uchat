#include "../inc/client.h"

static inline void add_styles_to_chat_window(chat_window_t* window) {
	mx_widget_add_styles(window->window);
	mx_widget_add_styles(window->chats_list_scrlldwnd);
	mx_widget_add_styles(window->chats_list_box);
	mx_widget_add_styles(window->chats_list_grid);
	mx_widget_add_styles(window->add_chat_btn);
	mx_widget_add_styles(window->search_entry);
	mx_widget_add_styles(window->menu_btn); 
	mx_widget_add_styles(window->chat_header_box);
	mx_widget_add_styles(window->chat_name_label);
	mx_widget_add_styles(window->chat_info_btn);
	mx_widget_add_styles(window->msgs_list_box);
	mx_widget_add_styles(window->msgs_list_scrlld_wnd);
	mx_widget_add_styles(window->send_message_btn);
	mx_widget_add_styles(window->entry_edit_stack);
	mx_widget_add_styles(window->selected_msg_edit_box);
	mx_widget_add_styles(window->selected_msg_close_btn);
	mx_widget_add_styles(window->selected_msg_number_label);
	mx_widget_add_styles(window->selected_msg_edit_btn);
	mx_widget_add_styles(window->selected_msg_select_all_btn);
	mx_widget_add_styles(window->selected_msg_delete_btn);
	mx_widget_add_styles(window->selected_msg_forward_btn);
	mx_widget_add_styles(window->message_input_field);
	mx_widget_add_styles(window->msg_input_scrlld_wnd);
}

chat_window_t* mx_build_chat_window(client_t* client) {
	chat_window_t* window = malloc(sizeof(chat_window_t));
	window->search_t = clock();

	window->builder = gtk_builder_new_from_file(CHAT_WIND_XML_PATH);
	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_window"));

	gtk_builder_connect_signals(window->builder, NULL);

	//Left side(chats list, search, add chat)
	window->chats_list_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_box"));
	window->chats_list_scrlldwnd = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_scrlldwnd"));
	window->chats_list_grid = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_grid"));
	window->add_chat_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "add_chat_btn"));
	window->search_entry = GTK_WIDGET(gtk_builder_get_object(window->builder, "search_entry"));
	window->menu_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "menu_btn"));
	window->elements_were_attached = 0;
	
	//Right side(current chat)
	window->selected_chat_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_chat_box"));
	window->chat_header_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_header_box"));
	window->chat_name_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_name_label"));
	window->chat_info_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_info_btn"));
	window->msgs_list_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "msgs_list_box"));
	window->msgs_list_scrlld_wnd = GTK_WIDGET(gtk_builder_get_object(window->builder, "msgs_list_scrlld_wnd"));
	window->send_message_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "send_message_btn"));
	window->entry_edit_stack = GTK_WIDGET(gtk_builder_get_object(window->builder, "entry_edit_stack"));
	window->selected_msg_edit_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_edit_box"));
	window->selected_msg_close_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_close_btn"));
	window->selected_msg_number_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_number_label"));
	window->selected_msg_edit_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_edit_btn"));
	window->selected_msg_select_all_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_select_all_btn"));
	window->selected_msg_delete_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_delete_btn"));
	window->selected_msg_forward_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "selected_msg_forward_btn"));
	window->msg_input_scrlld_wnd = GTK_WIDGET(gtk_builder_get_object(window->builder, "msg_input_scrlld_wnd"));
	window->message_input_field = GTK_WIDGET(gtk_builder_get_object(window->builder, "message_input_field"));

	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(provider, STYLES_FILE_PATH, NULL);
	GtkStyleContext *context = gtk_widget_get_style_context(window->message_input_field);
	gtk_widget_set_name (window->message_input_field, "message_input_field");
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	g_object_unref(provider);

	g_signal_connect(window->add_chat_btn, "released", G_CALLBACK(mx_on_create_chat_btn_clicked), client);
	g_signal_connect(window->search_entry, "search-changed", G_CALLBACK(mx_on_search_changed), client);
	
	g_signal_connect(window->msgs_list_box, "button-press-event", G_CALLBACK(mx_on_button_press_event), client);
	g_signal_connect(window->msgs_list_box, "selected-rows-changed", G_CALLBACK(mx_on_selection_changed), client);
	g_signal_connect(window->msgs_list_scrlld_wnd, "edge-reached", G_CALLBACK(mx_on_msg_list_edge_was_reached), client);
	g_signal_connect(window->selected_msg_close_btn, "released", G_CALLBACK(mx_on_selected_msg_close_btn_clicked), client);
	g_signal_connect(window->selected_msg_select_all_btn, "released", G_CALLBACK(mx_on_selected_msg_select_all_btn_clicked), client);
	g_signal_connect(window->selected_msg_edit_btn, "released", G_CALLBACK(mx_on_edit_btn_clicked), client);
	g_signal_connect(window->selected_msg_delete_btn, "released", G_CALLBACK(mx_on_delete_btn_clicked), client);
	g_signal_connect(window->message_input_field, "key-press-event", G_CALLBACK(mx_enter_key_press), client);
	g_signal_connect(window->send_message_btn, "released", G_CALLBACK(mx_on_send_msg_btn_clicked), client);

	g_signal_connect(window->chat_info_btn, "released", G_CALLBACK(mx_on_chat_info_btn_clicked), client);
	g_signal_connect(window->menu_btn, "released", G_CALLBACK(mx_on_menu_btn_clicked), client);
	g_signal_connect(window->window, "destroy", G_CALLBACK(mx_app_on_destroy), client);
	
	add_styles_to_chat_window(window);

	return window;
}

