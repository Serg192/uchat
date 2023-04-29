#include "../../inc/client.h"

gboolean mx_on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {

	client_t* client = (client_t*)data;

  	GtkListBox *list_box = GTK_LIST_BOX(widget);
  	GtkListBoxRow *row = gtk_list_box_get_row_at_y(list_box, event->y);

  	if (row != NULL && event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY) {
    	gboolean selected = gtk_list_box_row_is_selected(row);

    	if (selected) {
    	  	gtk_list_box_unselect_row(list_box, row);
    	} 
		else {
    	
			gtk_list_box_select_row(list_box, row);
		 	if(client->edit_mode) {
  				GtkTextView* text_view = (GtkTextView*)client->c_window->message_input_field;
	    		GtkTextBuffer* input_txt_buf = gtk_text_view_get_buffer(text_view);
	   			gtk_text_buffer_set_text(input_txt_buf, "", 0);
	   			client->edit_mode = false;
  		  	}
    	}

    	return TRUE;
  	}

  	return FALSE;
}
