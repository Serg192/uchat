#include "../inc/client.h"

static inline void add_styles_to_chat_info_window(chat_info_window_t* window) {
	mx_widget_add_styles(window->window);
	mx_widget_add_styles(window->chat_info_name_label);
	mx_widget_add_styles(window->members_list_label);
	mx_widget_add_styles(window->members_list_scrlld_wnd);
	mx_widget_add_styles(window->members_list_box);
	mx_widget_add_styles(window->leave_chat_btn);
	mx_widget_add_styles(window->cancel_info_btn);
}

chat_info_window_t* mx_build_chat_info_window(client_t* client) {
	chat_info_window_t* window = (chat_info_window_t*)malloc(sizeof(chat_info_window_t));
	window->visible = false;

	window->builder = gtk_builder_new_from_file(CHAT_INFO_WIND_XML_PATH);
	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_info_window"));

	gtk_window_set_transient_for(GTK_WINDOW(window->window), GTK_WINDOW(client->c_window->window));

	window->chat_info_name_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_info_name_label"));
	window->members_list_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "members_list_label"));
	window->members_list_scrlld_wnd = GTK_WIDGET(gtk_builder_get_object(window->builder, "members_list_scrlld_wnd"));
    window->members_list_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "members_list_box"));
    window->leave_chat_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "leave_chat_btn"));
    window->cancel_info_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "cancel_info_btn"));

	g_signal_connect(G_OBJECT(window->cancel_info_btn), "released", G_CALLBACK(mx_on_cancel_info_btn_clicked), client);
    g_signal_connect(G_OBJECT(window->leave_chat_btn), "released", G_CALLBACK(mx_on_leave_btn_clicked), client);
	g_signal_connect(G_OBJECT(window->window), "focus-out-event", G_CALLBACK(mx_hide_hint_window), NULL);

	add_styles_to_chat_info_window(window);

	return window;
}
