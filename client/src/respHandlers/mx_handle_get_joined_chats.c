#include "../../inc/client.h"

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
	guint width, height;
	GdkRGBA color;
	GtkStyleContext *context;
	
	gdk_rgba_parse(&color, "rgba(255,0,0,1)");

	width = gtk_widget_get_allocated_width(widget);
	height = gtk_widget_get_allocated_height(widget);
	context = gtk_widget_get_style_context(widget);

	gtk_render_background(context, cr, 0, 0, width, height);

	cairo_arc (cr, width / 2.0, height / 2.0,
             	MIN(width, height) / 2.0, 0, 2 * G_PI);

	gdk_cairo_set_source_rgba(cr, &color);

	cairo_fill(cr);

	return FALSE;
}

static inline void update_chat_list(client_t* client) {

	GtkWidget *button;
	GtkWidget *icon;

	gtk_widget_hide(client->c_window->chats_list_grid);

	gtk_grid_remove_column(client->c_window->chats_list_grid, 1);

	for(int i = 0; i < client->chat_container->count; i++) {

		gtk_grid_insert_row(GTK_GRID(client->c_window->chats_list_grid), i);

		button = gtk_button_new_with_label(client->chat_container->chats[i]->name);
		char *str = mx_strndup(client->chat_container->chats[i]->name, 2);
		str[0] = (char)mx_toupper((int)str[0]);
		str[1] = (char)mx_toupper((int)str[1]);
		icon = gtk_label_new(str);

		gtk_widget_set_size_request(icon, 50, 50);
		GdkRGBA icon_text_color;
		gdk_rgba_parse(&icon_text_color, "rgba(255,255,255,1)");
		gtk_widget_override_color(GTK_WIDGET(icon), GTK_STATE_FLAG_NORMAL, &icon_text_color);
		
		gtk_button_set_image(GTK_BUTTON(button), icon);
		gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
		gtk_widget_set_size_request(button, 200, 100);
		gtk_button_set_alignment(GTK_BUTTON(button), 0.0, 0.5);
		gtk_grid_attach(GTK_GRID(client->c_window->chats_list_grid), button, 1, i, 1, 1);


		g_signal_connect(G_OBJECT(icon), "draw", G_CALLBACK(draw_callback), NULL);
		g_signal_connect(button, "released", G_CALLBACK(mx_on_chat_btn_clicked), client/*->chat_container->chats[i]*/);
		mx_strdel(&str);
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
