#include "../../inc/client.h"

static inline void update_chat_list(client_t* client) {

	GtkWidget *button;
	GtkWidget *image;

	gtk_widget_hide(client->c_window->chats_list_grid);

	gtk_grid_remove_column(client->c_window->chats_list_grid, 1);

	for(int i = 0; i < client->chat_container->count; i++) {

		gtk_grid_insert_row(GTK_GRID(client->c_window->chats_list_grid), i);

		button = gtk_button_new_with_label(client->chat_container->chats[i]->name);
		image = gtk_image_new_from_file("client/src/input-mouse.png");

		gtk_button_set_image(GTK_BUTTON(button), image);
		gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
		gtk_widget_set_size_request(button, 200, 100);
		gtk_button_set_alignment(GTK_BUTTON(button), 0.0, 0.5);
		gtk_grid_attach(GTK_GRID(client->c_window->chats_list_grid), button, 1, i, 1, 1);


		g_signal_connect(button, "released", G_CALLBACK(mx_on_chat_btn_clicked), client->chat_container->chats[i]);
	}

	client->c_window->elements_were_attached = client->chat_container->count;

	printf("Elements were attached %d\n", client->c_window->elements_were_attached);

	gtk_widget_show_all(client->c_window->window);
}

void mx_handle_get_joined_chats(client_t* client) {

	cJSON *item = cJSON_GetObjectItem(client->current_response->json,"chats");

	const int chat_count = cJSON_GetArraySize(item);

	printf("Array len %d\n", chat_count);

	if(client->chat_container != NULL) {
		for(int i = 0; i < client->chat_container->count; i++)
			free(client->chat_container->chats[i]);
		free(client->chat_container->chats);
		free(client->chat_container);
		client->chat_container = NULL;
	}
	
	client->chat_container = (disp_chats_container_t*) malloc(sizeof(disp_chats_container_t));
	client->chat_container->count = chat_count;
	client->chat_container->chats = (chat_info_t**)malloc(sizeof(chat_info_t*) * chat_count);
	
	for(int i = 0; i < chat_count; i++) {

		cJSON * subitem = cJSON_GetArrayItem(item, i);
		
		chat_info_t* inf = (chat_info_t*)malloc(sizeof(chat_info_t));
		inf->id = cJSON_GetObjectItem(subitem, "id")->valueint;
		inf->name = cJSON_GetObjectItem(subitem, "name")->valuestring;

		client->chat_container->chats[i] = inf;
	}

	update_chat_list(client);
	printf("list updated\n");
}