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
		case CHAT_CREATION_SUCCESS_RESP:
			mx_handle_chat_creation(client);
			break;
		case JOINED_CHATS_RESP: 
			//mx_handle_get_joined_chats(client);
			mx_handle_update_chat_list(client);
			break;
		case CHAT_SEARCH_RESP: 
			//mx_handle_get_joined_chats(client);
		      mx_handle_update_chat_list(client);
			break;
		case ADD_CHAT_MEM_SUCCESS_RESP:
			//mx_handle_get_joined_chats(client);
			mx_handle_update_chat_list(client);
			break;
		case GET_ABOVE_MSG_RESP:
		case GET_BELOW_MSG_RESP:
			mx_handle_msg_update(client);
			break;
		case PARTICIPANTS_RESP:
			mx_handle_create_chat_info_window(client);
			break;
		case PROFILE_EDIT_ERR_RESP:
			mx_handle_profile_edit_err(client);
			break;
	    case PROFILE_EDIT_OK_RESP:
	    	mx_handle_profile_edit_ok(client);
	    	break;
		default:
			break;
	}

	//printf("Deleting json\n");
	cJSON_Delete(response->json);
	//printf("Deleting current_response_str\n");
	//free(client->current_response->str_res);
	free((void *) client->current_response->str_res);
	client->current_response->str_res = NULL;
	//printf("Deleting current_response\n");
	free(client->current_response);
	client->current_response = NULL;

	//printf("Handled\n");
}

void* mx_main_background_loop(void* data){

	client_t* client = (client_t*)data;

	bool is_running = true;

	clock_t start_time = clock();
	clock_t current_time;

	bool req_was_sent = false;

	while(is_running){
		current_time = clock();

		if(!client->request_queue->empty){
			request_t* request = mx_queue_peek(client->request_queue);
			//printf("Sending request => %s\n",request->req);
			mx_send_req(client->ssl, request->req);

			if(request->type == QUIT_REQ)
				is_running = false;

			free(request->req);
			free(request);
			mx_queue_pop(client->request_queue);
			req_was_sent = true;
		}

		if(req_was_sent){
			//timeout is 3 sec
			client->current_response = mx_get_server_response(client->ssl);
			if(client->current_response != NULL){
				//printf("Response received => %s\n", client->current_response->str_res);
				handle_response(client);
			}
		}
		

		if(client->current_chat_id != -1 &&
			client->last_msg_in_chat_id != -1 &&
			(double)(current_time - start_time) / CLOCKS_PER_SEC >= 1.0f){

			request_t* request = (request_t*)malloc(sizeof(request_t));
			request->req = mx_create_get_chat_msg_req(client->current_chat_id, MSG_LOAD_BELOW, client->last_msg_in_chat_id, 10000);
			mx_queue_push(client->request_queue, request);
			start_time = current_time;
		}
		req_was_sent = false;
	}

	int ret = SSL_shutdown(client->ssl);
	if(ret == 0)
		ret = SSL_shutdown(client->ssl);
	close(client->serv);
	//free other res
	return NULL;
}




