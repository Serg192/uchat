#include "../../inc/client.h"

void mx_on_del_account_btn_clicked(GtkWidget* w, gpointer data){
	
	client_t* client = (client_t*)data;

	
	request_t* d_request = (request_t*)malloc(sizeof(request_t));
	d_request->req = mx_create_delete_account_req();
	mx_queue_push(client->request_queue, d_request);

	mx_on_log_out_btn_clicked(w, data);
}


