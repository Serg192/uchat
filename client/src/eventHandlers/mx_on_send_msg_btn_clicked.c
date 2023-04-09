#include "../../inc/client.h"

void mx_on_send_msg_btn_clicked(GtkButton* b, gpointer data){
	client_t* client = (client_t*)data;

	GtkTextView* text_view = (GtkTextView*)client->c_window->message_input_field;

	GtkTextBuffer* input_txt_buf = gtk_text_view_get_buffer(text_view);

	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(input_txt_buf, &start, &end);
	char *text = gtk_text_buffer_get_text(input_txt_buf, &start, &end, FALSE);


	//If there are any leading spaces, we remove them.
	//This behavior is similar to Telegram's behavior.
	//If the message consists only of spaces without any
	// other characters, we do not send it to the server.
	char* trimmed_str = mx_strtrim(text);

	//Characters like single quotes ('), double quotes ("), backslashes (\),
	//and null characters (\0) can cause problems when using SQL insertion.
	//To avoid these issues, we need to prepare the string before saving it 
	//to the database.
	char* prepared_text = mx_prepare_str_for_sql(trimmed_str);

	//TODO: To complete string validation and preparation, we should add 
	//an additional method that handles user input and does not allow the
	//user to write a message that starts with leading spaces. 
	//(see Telegram)

	if(trimmed_str != NULL && mx_strlen(trimmed_str) != 0){
		request_t* request = (request_t*)malloc(sizeof(request));
		request->req = mx_create_send_msg_req(client->current_chat_id, prepared_text);
		mx_queue_push(client->request_queue, request);
		gtk_text_buffer_set_text(input_txt_buf, "", 0);
	}

	if(text)
		g_free(text);
	if(prepared_text)
		free(prepared_text);
	if(trimmed_str)
		free(trimmed_str);
}
