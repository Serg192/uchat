#include "../../inc/client.h"

void mx_handle_profile_edit_ok(client_t* client){
	(void)client;
	gtk_label_set_text(GTK_LABEL(client->u_window->information_label), "Changes were applied successfully");
	gtk_widget_set_name(client->u_window->information_label, "information_label_ok");

	free(client->login);
	free(client->password);

	client->login = client->tmpLogin;
	client->password = client->tmpPassword;
}

void mx_handle_profile_edit_err(client_t* client){
	cJSON* json = client->current_response->json;

	const char* err_msg = cJSON_GetObjectItem(json, "message")->valuestring;

	gtk_label_set_text(GTK_LABEL(client->u_window->information_label), err_msg);
	gtk_widget_set_name(client->u_window->information_label, "information_label_error");

	free(client->tmpPassword);
	free(client->tmpLogin);
}
