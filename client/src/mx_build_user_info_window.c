#include "../inc/client.h"

static inline void add_styles_to_user_info_window(user_info_window_t* window) {
    mx_widget_add_styles(window->window);
	mx_widget_add_styles(window->profile_label);
	mx_widget_add_styles(window->username_entry);
	mx_widget_add_styles(window->username_label);
	mx_widget_add_styles(window->password_entry);
	mx_widget_add_styles(window->password_label);
    mx_widget_add_styles(window->save_btn);
	mx_widget_add_styles(window->cancel_btn);
	mx_widget_add_styles(window->show_passw_check_btn);
	mx_widget_add_styles(window->information_label);
}

user_info_window_t* mx_build_user_info_window(client_t* client) {
	user_info_window_t* window = (user_info_window_t*)malloc(sizeof(user_info_window_t));

	window->builder = gtk_builder_new_from_file(USER_INFO_WIND_XML_PATH);
	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "user_info_window"));

	gtk_window_set_transient_for(GTK_WINDOW(window->window), GTK_WINDOW(client->c_window->window));

	window->profile_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "profile_label"));
	window->username_entry = GTK_WIDGET(gtk_builder_get_object(window->builder, "username_entry"));
	window->username_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "username_label"));
    window->password_entry = GTK_WIDGET(gtk_builder_get_object(window->builder, "password_entry"));
    window->password_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "password_label"));
	window->save_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "save_btn"));
	window->cancel_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "cancel_btn"));
    window->show_passw_check_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "show_passw_check_btn"));
    window->information_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "information_label"));


    g_signal_connect(window->show_passw_check_btn, "toggled", G_CALLBACK(mx_on_show_psw_btn_clicked), client);
	g_signal_connect(window->save_btn, "released", G_CALLBACK(mx_on_save_profile_changes), client);
	g_signal_connect(window->cancel_btn, "released", G_CALLBACK(mx_hide_hint_window), client);
	g_signal_connect(G_OBJECT(window->window), "focus-out-event", G_CALLBACK(mx_hide_hint_window), NULL);

	add_styles_to_user_info_window(window);

	return window;
}
