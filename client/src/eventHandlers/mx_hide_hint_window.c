#include "../../inc/client.h"

void mx_hide_hint_window(GtkWidget* widget, gpointer data) {
	(void)data;
  	 GtkWidget* top_level = gtk_widget_get_toplevel(widget);
  	 gtk_widget_hide(top_level);
}
