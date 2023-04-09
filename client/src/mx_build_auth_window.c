#include "../inc/client.h"

auth_window_t* mx_build_auth_window(client_t* client) {
	auth_window_t* window = malloc(sizeof(auth_window_t));
	//window->block_buttons = false;

	window->builder = gtk_builder_new_from_file(AUTH_WIND_XML_PATH);

	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "auth_window"));

	gtk_window_set_position(GTK_WINDOW(window->window), GTK_WIN_POS_CENTER_ALWAYS);

	//gtk_window_set_resizable(window->window, FALSE);

	//JUST TEST, add own handler in the future
	g_signal_connect(window->window, "destroy", G_CALLBACK(mx_app_on_destroy), client);

	gtk_builder_connect_signals(window->builder, NULL);

	//

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

	return window;
}
