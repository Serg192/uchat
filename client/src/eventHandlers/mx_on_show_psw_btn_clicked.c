#include "../../inc/client.h"

void mx_on_show_psw_btn_clicked(GtkWidget *w, gpointer data){
    (void)w;
    
    client_t* client = (client_t*)data;
    //gtk_entry_set_visibility(GTK_ENTRY(client->a_window->password_entry), gtk_toggle_button_get_active(client->a_window->show_psw_bnt));
    gtk_entry_set_visibility(GTK_ENTRY(client->a_window->password_entry), gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(client->a_window->show_psw_bnt)));
}

