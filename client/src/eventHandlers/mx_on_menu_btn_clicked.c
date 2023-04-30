#include "../../inc/client.h"

void mx_on_menu_btn_clicked(GtkWidget* widget, gpointer data) {
	(void)widget;
	client_t* client = (client_t*)data;

	gint main_window_x, main_window_y;
	gtk_window_get_position(GTK_WINDOW(client->c_window->window), &main_window_x, &main_window_y);
	gtk_window_set_position(GTK_WINDOW(client->m_window->window), GTK_WIN_POS_MOUSE);
	gtk_window_move(GTK_WINDOW(client->m_window->window), main_window_x + 10, main_window_y + 35);

	gtk_widget_show_all(client->m_window->window);
}
