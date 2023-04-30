#include "../../inc/client.h"

void mx_on_send_msg_btn_clicked(GtkButton* b, gpointer data){
    //added to prevent warning
    (void)b;
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

	const int trimmed_str_len = mx_strlen(trimmed_str);

	request_t* request = NULL;

	if(trimmed_str != NULL && trimmed_str_len != 0){
		request = (request_t*)malloc(sizeof(request));
		bool req_was_created = false;

		if(client->edit_mode){
			if(mx_strcmp(client->message_to_edit->messages_str, trimmed_str) == 0){
				//nothing to do
			} else {
				request->req = mx_cteate_message_edit_req(client->current_chat_id, client->message_to_edit->id, prepared_text);
				req_was_created = true;

				message_t* m = (message_t*)g_object_get_data(G_OBJECT(client->edit_row), "message_data");
				free(m->messages_str);
				m->messages_str = mx_strdup(text);
				g_object_set_data(G_OBJECT(client->edit_row), "message_data", m);
			}
			client->edit_mode = false;
			gtk_widget_hide(client->c_window->editing_close_btn);
			gtk_widget_hide(client->c_window->editing_label);
		} else {
		    request->req = mx_create_send_msg_req(client->current_chat_id, prepared_text);
			req_was_created = true;
		}

		if(!req_was_created){
			free(request);
			request = NULL;
		}


		gtk_text_buffer_set_text(input_txt_buf, "", 0);

	} else if (client->edit_mode && trimmed_str_len == 0) {
		//delete
		request = (request_t*)malloc(sizeof(request));
		request->req = mx_create_delete_chat_msg_req(client->current_chat_id, client->message_to_edit->id);
		client->edit_mode = false;
	}

	if(request != NULL)
		mx_queue_push(client->request_queue, request);

	if(text)
		g_free(text);
	if(prepared_text)
		free(prepared_text);
	if(trimmed_str)
		free(trimmed_str);
}
