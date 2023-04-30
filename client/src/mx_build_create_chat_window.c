#include "../inc/client.h"

static inline void add_styles_to_create_chat_window(create_chat_window_t* window) {
	mx_widget_add_styles(window->window);
	mx_widget_add_styles(window->create_btn);
	mx_widget_add_styles(window->cancel_btn);
	mx_widget_add_styles(window->chat_name_entry);
	mx_widget_add_styles(window->create_chat_label);
	mx_widget_add_styles(window->create_chat_length_label);
}

create_chat_window_t* mx_build_create_chat_window(client_t* client) {
	create_chat_window_t* window = (create_chat_window_t*)malloc(sizeof(create_chat_window_t));

	window->builder = gtk_builder_new_from_file(CREATE_CHAT_WIND_XML_PATH);
	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "create_new_chat_window"));

	gtk_window_set_transient_for(GTK_WINDOW(window->window), GTK_WINDOW(client->c_window->window));

	window->create_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "create_chat_create_btn"));
	window->cancel_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "create_chat_cancel_btn"));
	window->chat_name_entry = GTK_WIDGET(gtk_builder_get_object(window->builder, "new_chat_name"));
	window->create_chat_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "create_chat_label"));
	window->create_chat_length_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "create_chat_length_label"));


	g_signal_connect(G_OBJECT(window->cancel_btn), "released", G_CALLBACK(mx_hide_hint_window), NULL);
	g_signal_connect(G_OBJECT(window->create_btn), "released", G_CALLBACK(mx_on_create_new_chat_btn_clicked), client);
	g_signal_connect(G_OBJECT(window->window), "focus-out-event", G_CALLBACK(mx_hide_hint_window), NULL);

	add_styles_to_create_chat_window(window);

	return window;
}
