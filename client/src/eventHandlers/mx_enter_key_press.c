#include "../../inc/client.h"

gboolean mx_enter_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
	client_t* client = (client_t*)data;
	GtkTextBuffer *buffer;
  	GtkTextIter iter;

	// GtkTextView* text_view = (GtkTextView*)client->c_window->message_input_field;
	// GtkTextIter start, end;
	// gtk_text_buffer_get_bounds(input_txt_buf, &start, &end);
	// char *text = gtk_text_buffer_get_text(input_txt_buf, &start, &end, FALSE);

	if (event->keyval == GDK_KEY_Return) {
		if (event->state & GDK_SHIFT_MASK) {
			buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      		gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));
      		gtk_text_buffer_insert(buffer, &iter, "\n", 1);

			return TRUE;
		}
		else {
			mx_on_send_msg_btn_clicked((GtkButton*)client->c_window->send_message_btn, client);
			return TRUE;
		}
  	}

  	return FALSE;
}
