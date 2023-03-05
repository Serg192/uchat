#include "../inc/server.h"

static inline void dispatch(client_t* client, request_t* req) {
	//mx_log(SERV_LOG_FILE, LOG_TRACE, "Request dispatching...");

	switch(req->type) {
				case SIGNUP_REQ:
					mx_handle_registration(client, req);
					break;
				case LOGIN_REQ:
					mx_handle_logging_in(client, req);
					break;
				case CREATE_CHAT_REQ:
					mx_handle_chat_creation(client, req);
					break;
				case CHAT_SEARCH_REQ:
					mx_handle_chat_search(client, req);
					break;
				case JOIN_CHAT_REQ:
					mx_handle_add_room_member(client, req);
					break;
				case GET_CHAT_PARTICIPANTS_REQ:
					mx_handle_get_chat_participants(client, req);
					break;
				case SEND_MSG_REQ:
					mx_handle_send_msg(client, req);
					break;
				case GET_CHAT_MSG_REQ:
					mx_handle_get_chat_msg(client, req);
					break;
				case DEL_CHAT_MSG_REQ:
					mx_handle_delete_chat_msg(client, req);
					break;
				case GET_JOINED_CHATS:
					mx_handle_get_joined_chats(client, req);
					break;
				default:
					break;

	}

}


void* mx_client_handler(void* client) {

	bool client_disconnect_req = false;

	while(!client_disconnect_req) {
		request_t* client_req = mx_get_client_req((client_t*)client);

		if(client_req != NULL) {
			dispatch(client, client_req);
			cJSON_Delete(client_req->json);
			free(client_req->str_req);
			free(client_req);
		}
	}

	pthread_detach(pthread_self());
}