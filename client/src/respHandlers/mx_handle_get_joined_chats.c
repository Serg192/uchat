#include "../../inc/client.h"


int mx_get_chat_id_from_btn(GtkWidget* w, client_t* client) {
	GtkWidget* tmp;
	int index = 0;

	while(true) {
		tmp = gtk_grid_get_child_at(client->c_window->chats_list_grid, 1, index);
		if(tmp == NULL || tmp == w)
			break;
		index++;
	}

	return index;
}

void mx_handle_get_joined_chats(client_t* client) {

	cJSON *item = cJSON_GetObjectItem(client->current_response->json,"chats");

	const int chat_count = cJSON_GetArraySize(item);

	printf("Array len %d\n", chat_count);

	GtkWidget *button;
	GtkWidget *image;

	gtk_widget_hide(client->c_window->chats_list_grid);

	gtk_grid_remove_column(client->c_window->chats_list_grid, 1);

	if(client->chat_id_chat_btn_map != NULL) {
		free(client->chat_id_chat_btn_map);
		client->chat_id_chat_btn_map = NULL;
	}

	client->chat_id_chat_btn_map = (int*)malloc(sizeof(int) * chat_count);

	for(int i = 0; i < chat_count; i++) {

		cJSON * subitem = cJSON_GetArrayItem(item, i);
		
		gtk_grid_insert_row(GTK_GRID(client->c_window->chats_list_grid), i);

		button = gtk_button_new_with_label(cJSON_GetObjectItem(subitem, "name")->valuestring);
		image = gtk_image_new_from_file("client/src/input-mouse.png");

		gtk_button_set_image(GTK_BUTTON(button), image);
		gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
		gtk_widget_set_size_request(button, 200, 100);
		gtk_button_set_alignment(GTK_BUTTON(button), 0.0, 0.5);
		gtk_grid_attach(GTK_GRID(client->c_window->chats_list_grid), button, 1, i, 1, 1);

		client->chat_id_chat_btn_map[i] = cJSON_GetObjectItem(subitem, "id")->valueint;

		g_signal_connect(button, "released", client->search_mode ? G_CALLBACK(mx_on_chat_search_list_clicked) : G_CALLBACK(mx_on_chat_list_clicked), client);

	}

	gtk_widget_show_all(client->c_window->window);

	printf("list updated\n");
}
