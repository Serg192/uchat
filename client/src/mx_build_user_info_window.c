#include "../inc/client.h"

user_info_window_t* mx_build_user_info_window(client_t* client) {
	user_info_window_t* window = (user_info_window_t*)malloc(sizeof(user_info_window_t));

	window->builder = gtk_builder_new_from_file(USER_INFO_WIND_XML_PATH);

	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "user_info_window"));

	gtk_window_set_transient_for(GTK_WINDOW(window->window), GTK_WINDOW(client->c_window->window));
	g_signal_connect(G_OBJECT(window->window), "focus-out-event", G_CALLBACK(mx_hide_hint_window), NULL);

	window->username_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "username_label"));
	window->log_out_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "log_out_btn"));
	window->delet_acc_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "delet_acc_btn"));
    window->version_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "version_label"));

	//g_signal_connect(G_OBJECT(window->cancel_info_btn), "released", G_CALLBACK(mx_on_cancel_info_btn_clicked), client);

	return window;
}
