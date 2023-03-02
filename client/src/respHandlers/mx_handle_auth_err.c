#include "../../inc/client.h"

void mx_handle_auth_err(client_t* client) {

	const char* err_msg = cJSON_GetObjectItem(client->current_response->json, "message")->valuestring;
	gtk_label_set_text(GTK_LABEL(client->a_window->err_label), (const gchar*) err_msg);
	printf("Error\n");
}
