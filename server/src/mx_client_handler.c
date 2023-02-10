#include "../inc/server.h"


void mx_client_handler(void* client) {
	
	(void*) client;

	bool client_disconnect_req = false;

	//
	printf("Client connected\n");
	//

	while(!client_disconnect_req) {
		request_t* client_req = mx_get_client_req((client_t*)client);

		if(client_req != NULL) {
			//handle request
		}
	}

	pthread_detach(pthread_self());
}