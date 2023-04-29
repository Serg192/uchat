#include "../../inc/client.h"

void mx_on_selection_changed(GtkListBox *listbox, gpointer data) {
	client_t* client = (client_t*)data;
	

	GtkStack *stack = GTK_STACK(client->c_window->entry_edit_stack);
	GList *selected_rows = gtk_list_box_get_selected_rows(listbox);

	if (selected_rows != NULL) {
    	gtk_stack_set_visible_child_name(stack, "page1");
		
		if (g_list_length(selected_rows) > 1) {
			gtk_widget_hide(client->c_window->selected_msg_edit_btn);
		}
		else {
			gtk_widget_show(client->c_window->selected_msg_edit_btn);
		}
		
		char *selected_count_str = mx_strjoin("Selected: ", mx_itoa(g_list_length(selected_rows)));
		gtk_label_set_text(GTK_LABEL(client->c_window->selected_msg_number_label), selected_count_str);
		free(selected_count_str);
		g_list_free(selected_rows);
  	} else {
    	gtk_stack_set_visible_child_name(stack, "page0");
  	}
}
