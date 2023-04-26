#include "../inc/client.h"


//for debug only
static void print_json_array(const char *json_str, const char *array_name) {
    // parse the JSON string
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    // get the array from the JSON object
    cJSON *array = cJSON_GetObjectItemCaseSensitive(root, array_name);
    if (array == NULL || !cJSON_IsArray(array)) {
        printf("Error getting array from JSON\n");
        cJSON_Delete(root);
        return;
    }

    // print each element of the array in a separate row
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, array) {
        char *json_string = cJSON_Print(item);
        printf("%s\n", json_string);
        free(json_string);
    }

    // free the JSON object
    cJSON_Delete(root);
}


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
			printf("\n\n************************************\n");
			print_json_array(client->current_response->str_res, "messages");
			mx_handle_msg_update(client);
			printf("\n*****************************************\n");
			break;
		case PARTICIPANTS_RESP:
			mx_handle_create_chat_info_window(client);
			break;
		default:
			break;
	}

	//printf("Deleting json\n");
	cJSON_Delete(response->json);
	//printf("Deleting current_response_str\n");
	free(client->current_response->str_res);
	client->current_response->str_res = NULL;
	//printf("Deleting current_response\n");
	free(client->current_response);
	client->current_response = NULL;

	//printf("Handled\n");
}

void* mx_main_background_loop(void* data){
	printf("Here\n");
	client_t* client = (client_t*)data;

	bool is_running = true;

	clock_t start_time = clock();
	clock_t current_time;

	bool req_was_sent = false;

	while(is_running){
		current_time = clock();

		if(!client->request_queue->empty){
			request_t* request = mx_queue_peek(client->request_queue);
			printf("Sending request => %s\n",request->req);
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
				printf("Response received => %s\n", client->current_response->str_res);
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
}



/*
void mx_main_background_loop(void* data) {

	client_t* client = (client_t*) data;

	bool can_handle_next = true;
	bool is_running = true;

	clock_t start_time = clock(); 
	clock_t time_to_wait_for_response = clock();
    clock_t current_time;

	while(is_running){
		current_time = clock();
		if(!client->request_queue->empty) {
			if(can_handle_next){
				request_t* request = mx_queue_peek(client->request_queue);
				printf("Sending request => %s\n", request->req);
				mx_send_req(client->ssl, request->req);

				if(request->type == QUIT_REQ)
					is_running = false;
				
				free(request->req);
				free(request);
				can_handle_next = false;
				time_to_wait_for_response = current_time;
			} else {
				client->current_response = mx_get_server_response(client->ssl);

				if(client->current_response != NULL) {
					mx_queue_pop(client->request_queue);
					printf("response received => %s\n", client->current_response->str_res);
					handle_response(client);
					can_handle_next = true;
					time_to_wait_for_response = current_time;
				} if((double)(current_time - time_to_wait_for_response) / CLOCKS_PER_SEC >= RESP_TIMEOUT) {
					//Just ignore and send next
					printf("____________________RESPONSE IGNORE_________________\n");
					can_handle_next = true;
					time_to_wait_for_response = current_time;
				}
			}
		} else {
			//push request for to update chat messages
			// 
			if (client->current_chat_id != -1 &&
			    client->last_msg_in_chat_id != -1 &&
			    (double)(current_time - start_time) / CLOCKS_PER_SEC >= 1.0f) {
            	//printf("Should update chat, last msg id is %d\n", client->last_msg_in_chat_id);

            	request_t* request = (request_t*)malloc(sizeof(request_t));

				request->req = mx_create_get_chat_msg_req(client->current_chat_id, MSG_LOAD_BELOW, client->last_msg_in_chat_id, 10000);

				mx_queue_push(client->request_queue, request);

            	start_time = current_time;
        	}

		}
	}

	int ret = SSL_shutdown(client->ssl);
	if (ret == 0) 
    	ret = SSL_shutdown(client->ssl);			
	close(client->serv);
}
*/

