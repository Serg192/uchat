#include "../inc/client.h"

static inline void add_styles_to_user_info_window(user_info_window_t* window) {
	mx_widget_add_styles(window->window);
	mx_widget_add_styles(window->username_label);
	mx_widget_add_styles(window->log_out_btn);
	mx_widget_add_styles(window->delete_acc_btn);
	mx_widget_add_styles(window->version_label);
}

user_info_window_t* mx_build_user_info_window(client_t* client) {
	user_info_window_t* window = (user_info_window_t*)malloc(sizeof(user_info_window_t));

	window->builder = gtk_builder_new_from_file(USER_INFO_WIND_XML_PATH);
	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "user_info_window"));

	gtk_window_set_transient_for(GTK_WINDOW(window->window), GTK_WINDOW(client->c_window->window));

	window->username_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "username_label"));
	window->log_out_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "log_out_btn"));
	window->delete_acc_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "delete_acc_btn"));
    window->version_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "version_label"));

    
	g_signal_connect(window->delete_acc_btn, "released", G_CALLBACK(mx_on_del_account_btn_clicked), client);
	g_signal_connect(window->log_out_btn, "released", G_CALLBACK(mx_on_log_out_btn_clicked), client);
	g_signal_connect(G_OBJECT(window->window), "focus-out-event", G_CALLBACK(mx_hide_hint_window), NULL);

	add_styles_to_user_info_window(window);

	return window;
}
