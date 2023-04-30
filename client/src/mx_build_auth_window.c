#include "../inc/client.h"

static inline void add_styles_to_auth_window(auth_window_t* window) {
	mx_widget_add_styles(window->window);
	mx_widget_add_styles(window->chat_title_label);
	mx_widget_add_styles(window->signup_btn);
	mx_widget_add_styles(window->login_btn);
	mx_widget_add_styles(window->username_entry);
	mx_widget_add_styles(window->password_entry);
	mx_widget_add_styles(window->show_psw_bnt);
	mx_widget_add_styles(window->err_label);
}

auth_window_t* mx_build_auth_window(client_t* client) {
	auth_window_t* window = malloc(sizeof(auth_window_t));

	window->builder = gtk_builder_new_from_file(AUTH_WIND_XML_PATH);
	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "auth_window"));

	gtk_window_set_position(GTK_WINDOW(window->window), GTK_WIN_POS_CENTER_ALWAYS);


	gtk_builder_connect_signals(window->builder, NULL);

    window->chat_title_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_title"));
	window->err_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "error_txt"));
	window->signup_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "signup_btn"));
	window->login_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "login_btn"));
	window->username_entry = GTK_WIDGET(gtk_builder_get_object(window->builder, "username_entry"));
	window->password_entry = GTK_WIDGET(gtk_builder_get_object(window->builder, "password_entry"));
	window->show_psw_bnt = GTK_WIDGET(gtk_builder_get_object(window->builder, "show_passw_check_btn"));

	gtk_entry_set_visibility(GTK_ENTRY(window->password_entry), FALSE);

	g_signal_connect(window->signup_btn, "released", G_CALLBACK(mx_on_signup_btn_clicked), client);
	g_signal_connect(window->login_btn, "released", G_CALLBACK(mx_on_login_btn_clicked), client);
	g_signal_connect(window->show_psw_bnt, "toggled", G_CALLBACK(mx_on_show_psw_btn_clicked), client);
	g_signal_connect(window->window, "destroy", G_CALLBACK(mx_app_on_destroy), client);

	add_styles_to_auth_window(window);

	return window;
}
