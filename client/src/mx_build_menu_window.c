#include "../inc/client.h"

static inline void add_styles_to_menu_window(menu_window_t* window) {
	mx_widget_add_styles(window->window);
	mx_widget_add_styles(window->user_profile_btn);
	mx_widget_add_styles(window->log_out_btn);
	mx_widget_add_styles(window->delete_acc_btn);
	mx_widget_add_styles(window->version_label);
}

menu_window_t* mx_build_menu_window(client_t* client) {
	menu_window_t* window = (menu_window_t*)malloc(sizeof(menu_window_t));

	window->builder = gtk_builder_new_from_file(MENU_WIND_XML_PATH);
	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "menu_window"));

	gtk_window_set_transient_for(GTK_WINDOW(window->window), GTK_WINDOW(client->c_window->window));

	window->user_profile_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "user_profile_btn"));
	window->log_out_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "log_out_btn"));
	window->delete_acc_btn = GTK_WIDGET(gtk_builder_get_object(window->builder, "delete_acc_btn"));
    window->version_label = GTK_WIDGET(gtk_builder_get_object(window->builder, "version_label"));


	g_signal_connect(window->user_profile_btn, "released", G_CALLBACK(mx_on_user_profile_btn_clicked), client);
	g_signal_connect(window->delete_acc_btn, "released", G_CALLBACK(mx_on_del_account_btn_clicked), client);
	g_signal_connect(window->log_out_btn, "released", G_CALLBACK(mx_on_log_out_btn_clicked), client);
	g_signal_connect(G_OBJECT(window->window), "focus-out-event", G_CALLBACK(mx_hide_hint_window), NULL);

	add_styles_to_menu_window(window);

	return window;
}
