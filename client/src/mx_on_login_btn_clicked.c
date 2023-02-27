#include "../inc/client.h"

void mx_on_login_btn_clicked(GtkButton* b, gpointer data) {
		char username_buffer[33];
		char password_buffer[33];

		auth_window_t* window = (auth_window_t*)data;

		sprintf(username_buffer, "%s", gtk_entry_get_text(window->username_entry));
		sprintf(password_buffer, "%s", gtk_entry_get_text(window->password_entry));

		printf("Username = %s, password = %s\n", username_buffer, password_buffer);
}