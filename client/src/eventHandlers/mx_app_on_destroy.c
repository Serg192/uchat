#include "../../inc/client.h"

void mx_app_on_destroy(GtkWidget *widget, gpointer data) {
    client_t* client = (client_t*)data;

	request_t* request = (request_t*)malloc(sizeof(request));
	request->req = mx_create_quit_request();
	request->type = QUIT_REQ;
	mx_queue_push(client->request_queue, request);
    gtk_widget_destroy(widget);
	gtk_main_quit();
}
