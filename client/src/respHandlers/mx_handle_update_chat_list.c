#include "../../inc/client.h"

typedef struct upd_chat_list_data_s {
	client_t* client;
	cJSON* json;
}              upd_chat_list_data_t;

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
	guint width, height;
	GdkRGBA color;
	GtkStyleContext *context;

	const char* color_names[] = {
    	"red",
    	"green",
    	"blue",
    	"rgb(255, 226, 4)",
    	"orange",
    	"purple",
    	"pink",
    	"gray"
	};

	gdk_rgba_parse(&color, color_names[(int)data]);

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

static inline gboolean update_chat_list_in_gtk_loop(gpointer data){

	printf("Update chat list\n");
	upd_chat_list_data_t* d = (upd_chat_list_data_t*)data;
	client_t* client = d->client;
	
	cJSON* chats_array = cJSON_GetObjectItem(d->json, "chats");
	printf("Get array len\n");
	const int chat_count = cJSON_GetArraySize(chats_array);

	GtkWidget *button;
	GtkWidget *icon;

	printf("Hide\n");

	gtk_widget_hide(client->c_window->chats_list_grid);
	gtk_grid_remove_column(GTK_GRID(client->c_window->chats_list_grid), 1);

	printf("Loop\n");
	for(int i = 0; i < chat_count; i++){
		cJSON* chat_info = cJSON_GetArrayItem(chats_array, i);

		gtk_grid_insert_row(GTK_GRID(client->c_window->chats_list_grid), i);

		button = gtk_button_new_with_label(cJSON_GetObjectItem(chat_info, "name")->valuestring);
		gtk_widget_set_size_request(button, 200, 100);
		gtk_button_set_alignment(GTK_BUTTON(button), 0.0, 0.5);
		gtk_widget_set_name(button, "chat_btn");

		const int color = cJSON_GetObjectItem(chat_info, "color")->valueint;

		g_object_set_data(G_OBJECT(button), "chat_id", GINT_TO_POINTER(cJSON_GetObjectItem(chat_info, "id")->valueint));

		char *str = mx_strndup(gtk_button_get_label(GTK_BUTTON(button)), 2);
		str[0] = (char)mx_toupper((int)str[0]);
		str[1] = (char)mx_toupper((int)str[1]);
		icon = gtk_label_new(str);
		gtk_widget_set_name(icon, "chat_icon");
		gtk_widget_set_margin_end(icon, 10);

		gtk_widget_set_size_request(icon, 60, 60);
		gtk_button_set_image(GTK_BUTTON(button), icon);

		gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
		gtk_grid_attach(GTK_GRID(client->c_window->chats_list_grid), button, 1, i, 1, 1);

		g_signal_connect(G_OBJECT(icon), "draw", G_CALLBACK(draw_callback), GINT_TO_POINTER(color));
		g_signal_connect(button, "released", client->search_mode ? G_CALLBACK(mx_on_chat_search_list_clicked) : G_CALLBACK(mx_on_chat_list_clicked), client);
		mx_widget_add_styles(icon);
		mx_widget_add_styles(button);
	}
	gtk_widget_show_all(client->c_window->chats_list_grid);

	cJSON_Delete(d->json);
	free(d);
	return FALSE;
}


void mx_handle_update_chat_list(client_t* client){
	upd_chat_list_data_t* data = (upd_chat_list_data_t*)malloc(sizeof(upd_chat_list_data_t));
	data->client = client;
	data->json = cJSON_Duplicate(client->current_response->json, 1);

	g_idle_add(update_chat_list_in_gtk_loop, data);
}

