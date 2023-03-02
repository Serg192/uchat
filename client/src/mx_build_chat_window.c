#include "../inc/client.h"

chat_window_t* mx_build_chat_window(client_t* client) {
	chat_window_t* window = malloc(sizeof(chat_window_t));

	window->builder = gtk_builder_new_from_file(CHAT_WIND_XML_PATH);

	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_window"));

	g_signal_connect(window->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(window->builder, NULL);

	return window;
}
