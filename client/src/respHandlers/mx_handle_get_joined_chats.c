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

int mx_get_chat_id_from_btn(GtkWidget* w, client_t* client) {
	GtkWidget* tmp;
	int index = 0;

	while(true) {
		tmp = gtk_grid_get_child_at(client->c_window->chats_list_grid, 1, index);
		if(tmp == NULL || tmp == w)
			break;
		index++;
	}

	return client->chat_id_chat_btn_map[index];
}

void mx_handle_get_joined_chats(client_t* client) {

	//printf("Get joined chats\n");

	cJSON *item = cJSON_GetObjectItem(client->current_response->json,"chats");

	const int chat_count = cJSON_GetArraySize(item);

	//printf("Array len %d\n", chat_count);

	GtkWidget *button;
	GtkWidget *icon;

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

		char *str = mx_strndup(gtk_button_get_label(button), 2);
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

		client->chat_id_chat_btn_map[i] = cJSON_GetObjectItem(subitem, "id")->valueint;

		g_signal_connect(G_OBJECT(icon), "draw", G_CALLBACK(draw_callback), NULL);
		g_signal_connect(button, "released", client->search_mode ? G_CALLBACK(mx_on_chat_search_list_clicked) : G_CALLBACK(mx_on_chat_list_clicked), client);

	}

	gtk_widget_show_all(client->c_window->window);

}
