#include "../inc/client.h"

static inline void add_styles_to_dialog_window(dialog_window_t* window) {
	mx_widget_add_styles(window->window);
	mx_widget_add_styles(window->ok_btn);
	mx_widget_add_styles(window->cancel_btn);
	mx_widget_add_styles(window->info_label);
}

dialog_window_t* mx_build_dialog_window(client_t* client) {
	dialog_window_t* window = (dialog_window_t*)malloc(sizeof(dialog_window_t));

	window->builder = gtk_builder_new_from_file(DIALOG_WIND_XML_PATH);
	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "dialog_window"));

	gtk_window_set_transient_for(GTK_WINDOW(window->window), GTK_WINDOW(client->c_window->window));

	window->ok_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "ok_btn"));
	window->cancel_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "cancel_btn"));
	window->info_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "info_label"));

	g_signal_connect(window->cancel_btn, "released", G_CALLBACK(mx_hide_hint_window), NULL);
	g_signal_connect(G_OBJECT(window->window), "focus-out-event", G_CALLBACK(mx_hide_hint_window), NULL);

	add_styles_to_dialog_window(window);

	return window;
}

