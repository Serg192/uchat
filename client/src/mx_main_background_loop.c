#include "../inc/client.h"

static inline void handle_response(client_t* client) {

	serv_res_t* response = client->current_response;

	switch(response->type) {
		case LOGIN_SUCESS_RESP:
			mx_handle_auth_success(client);
			break;
		case LOGIN_ERR_RESP:
			mx_handle_auth_err(client);
			break;
		case SIGNUP_SUCCESS_RESP:
			mx_handle_auth_success(client);
			break;
		case SIGNUP_ERR_RESP:
			mx_handle_auth_err(client);
			break;
		default:
			break;
	}

	cJSON_Delete(response->json);
	free(client->current_response->str_res);
	client->current_response->str_res = NULL;
	free(client->current_response);
	client->current_response = NULL;
	client->can_send_req = true;
}

void mx_main_background_loop(void* data) {

	client_t* client = (client_t*) data;

	while(1) {
		if(client->current_request != NULL) {
			printf("Request != NULL\n");
			mx_send_req(client->ssl, client->current_request->req);
			free(client->current_request->req);
			free(client->current_request);
			client->current_request = NULL;
			client->can_send_req = false;
		} else if(client->current_response == NULL && !client->can_send_req) {
			printf("Getting response\n");
			client->current_response = mx_get_server_response(client->ssl);
			//printf("Here\n");
		} else if(client->current_response != NULL){
			printf("Handling response\n");
			handle_response(client);
		}
	}
}

