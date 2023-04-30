#include "../../inc/client.h"

void mx_handle_profile_edit_ok(client_t* client){
	(void)client;
	gtk_label_set_text(GTK_LABEL(client->u_window->error_txt), "Changes were applied successfully");

	free(client->login);
	free(client->password);

	client->login = client->tmpLogin;
	client->password = client->tmpPassword;
}

void mx_handle_profile_edit_err(client_t* client){
	cJSON* json = client->current_response->json;

	const char* err_msg = cJSON_GetObjectItem(json, "message")->valuestring;

	gtk_label_set_text(GTK_LABEL(client->u_window->error_txt), err_msg);

	free(client->tmpPassword);
	free(client->tmpLogin);
}
