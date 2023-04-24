#include "../inc/client.h"


void mx_hide_hint_window(GtkWidget* widget, gpointer data) {
  	 GtkWidget* top_level = gtk_widget_get_toplevel(widget);
   // gtk_widget_destroy(top_level);
  	 gtk_widget_hide(top_level);
}

create_chat_window_t* mx_build_create_chat_window(client_t* client) {
	create_chat_window_t* window = (create_chat_window_t*)malloc(sizeof(create_chat_window_t));

	window->builder = gtk_builder_new_from_file(CREATE_CHAT_WIND_XML_PATH);

	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "create_new_chat_window"));

	//gtk_widget_set_name(GTK_WIDGET(window->window), "popup_window");
	gtk_window_set_transient_for(GTK_WINDOW(window->window), GTK_WINDOW(client->c_window->window));
	//gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window->window), TRUE);
	//gtk_window_set_position(GTK_WINDOW(window->window), GTK_WIN_POS_CENTER_ON_PARENT);
	//gtk_window_set_position(GTK_WINDOW(window->window), GTK_WIN_POS_CENTER_ALWAYS);
	//gtk_window_set_decorated(GTK_WINDOW(window->window), FALSE);

	g_signal_connect(G_OBJECT(window->window), "focus-out-event", G_CALLBACK(mx_hide_hint_window), NULL);

	window->create_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "create_chat_create_btn"));
	window->cancel_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "create_chat_cancel_btn"));
	window->chat_name_entry = GTK_WIDGET(gtk_builder_get_object(window->builder, "new_chat_name"));

	g_signal_connect(G_OBJECT(window->cancel_btn), "released", G_CALLBACK(mx_hide_hint_window), NULL);
	g_signal_connect(G_OBJECT(window->create_btn), "released", G_CALLBACK(mx_on_create_new_chat_btn_clicked), client);

	//gtk_widget_show_all(window->window);
	//gtk_widget_grab_focus (window->window);

	return window;
}
