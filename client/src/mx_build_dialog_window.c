#include "../inc/client.h"

dialog_window_t* mx_build_dialog_window(client_t* client) {
	dialog_window_t* window = (dialog_window_t*)malloc(sizeof(dialog_window_t));

	window->builder = gtk_builder_new_from_file(DIALOG_WIND_XML_PATH);
	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "window"));

	//gtk_widget_set_name(GTK_WIDGET(window->window), "popup_window");
	gtk_window_set_transient_for(GTK_WINDOW(window->window), GTK_WINDOW(client->c_window->window));
	//gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window->window), TRUE);
	//gtk_window_set_position(GTK_WINDOW(window->window), GTK_WIN_POS_CENTER_ALWAYS);
	//gtk_window_set_decorated(GTK_WINDOW(window->window), FALSE);

	g_signal_connect(G_OBJECT(window->window), "focus-out-event", G_CALLBACK(mx_hide_hint_window), NULL);

	window->ok_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "ok_btn"));
	window->cancel_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "cancel_btn"));
	window->info_lable = GTK_WIDGET(gtk_builder_get_object(window->builder, "info_lable"));

	g_signal_connect(window->cancel_btn, "released", G_CALLBACK(mx_hide_hint_window), NULL);

	return window;
}

