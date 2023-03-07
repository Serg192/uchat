#include "../inc/client.h"

chat_window_t* mx_build_chat_window(client_t* client) {

	chat_window_t* window = malloc(sizeof(chat_window_t));

	window->builder = gtk_builder_new_from_file(CHAT_WIND_XML_PATH);

	window->window = GTK_WIDGET(gtk_builder_get_object(window->builder, "chat_window"));

	g_signal_connect(window->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(window->builder, NULL);

	window->main_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "main_box"));
	window->left_side_box = GTK_WIDGET(gtk_builder_get_object(window->builder, "left_side_box"));
	window->chats_list_scrlldwnd = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_scrlldwnd"));
	window->chats_list_view = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_view"));
	window->chats_list_grid = GTK_WIDGET(gtk_builder_get_object(window->builder, "chats_list_grid"));

	//for test
	FILE *f1;
	if ((f1=fopen(CLIENT_LOG_FILE, "r") )==NULL) {
		printf("Cannot open file.\n");
		exit (1);
	}
	
	GtkWidget *button;
	GtkWidget *image;
	char tmp[1028];
	int row = 0; 
	while (!feof(f1)) {
		if (fgets(tmp, 1024, f1)) {

			tmp[strlen(tmp) - 1] = 0;

			gtk_grid_insert_row(GTK_GRID(window->chats_list_grid), row);
		
			button = gtk_button_new_with_label(tmp);
			image = gtk_image_new_from_file("/home/trubadur/Рабочий стол/uchat/client/src/input-mouse.png");
			gtk_button_set_image(GTK_BUTTON(button), image);
			gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
			gtk_widget_set_size_request(button, 200, 100);
			gtk_button_set_alignment(GTK_BUTTON(button), 0.0, 0.5);
			gtk_grid_attach(GTK_GRID(window->chats_list_grid), button, 1, row, 1, 1);
			//g_signal_connect(button, "clicked", G_CALLBACK(on_row), NULL);
			
			row++;
		}
	}
	
	fclose(f1);
	return window;
}

