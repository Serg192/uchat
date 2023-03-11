#include "../inc/client.h"

static inline void test(client_t* client) {
	
}

static inline void handle_response(client_t* client) {

	serv_res_t* response = client->current_response;
	client->can_send_req = true;

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
		case CHAT_CREATION_SUCCESS_RESP:
			mx_handle_chat_creation(client);
			break;
		case JOINED_CHATS_RESP: 
		//case CHAT_SEARCH_RESP:
			mx_handle_get_joined_chats(client);
			printf("Joined chats resp\n");
			break;
		case CHAT_SEARCH_RESP: 
			printf("Search resp\n");
		//case CHAT_SEARCH_RESP:
			mx_handle_get_joined_chats(client);
			break;
		case ADD_CHAT_MEM_SUCCESS_RESP:
			mx_handle_get_joined_chats(client);
			break;
		default:
			break;
	}

	printf("Deleting json\n");
	cJSON_Delete(response->json);
	printf("Deleting current_response_str\n");
	free(client->current_response->str_res);
	client->current_response->str_res = NULL;
	printf("Deleting current_response\n");
	free(client->current_response);
	client->current_response = NULL;

	printf("Handled\n");
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

