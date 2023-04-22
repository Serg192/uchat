#include "../../inc/client.h"

/*
static inline void del_msg_from_list_box(client_t* client, int message_id){
	//gtk_container_remove(GTK_CONTAINER(client->c_window->msgs_list_box), GTK_WIDGET(row));

	GtkListBoxRow* row = GTK_LIST_BOX_ROW(mx_map_get(client->message_id_row_map, message_id));
	if(row != NULL){
		gtk_container_remove(GTK_CONTAINER(client->c_window->msgs_list_box), GTK_WIDGET(row));
		gtk_widget_destroy(GTK_WIDGET(row));
		mx_map_remove(client->message_id_row_map, message_id);
	}
}*/

void mx_handle_msg_update(client_t* client){

	//printf("Handling msg update\n");

	cJSON* msg_json = client->current_response->json;
	cJSON* msg_array = cJSON_GetObjectItemCaseSensitive(msg_json, "messages");

    if (msg_array == NULL || !cJSON_IsArray(msg_array)) {
        return;
    }

    int messages_count = cJSON_GetArraySize(msg_array);

  	for(int i = 0; i < messages_count; i++) {
  		 cJSON *message = cJSON_GetArrayItem(msg_array, i);

  		message_t* msg_struct = (message_t*)malloc(sizeof(message_t));

  		msg_struct->id = cJSON_GetObjectItem(message, "id")->valueint;
  		msg_struct->sending_time = cJSON_GetObjectItem(message, "sending_time")->valueint;
  		msg_struct->sending_date = cJSON_GetObjectItem(message, "sending_date")->valueint;
  		msg_struct->username_from = mx_strdup(cJSON_GetObjectItem(message, "username")->valuestring);
  		msg_struct->messages_str = mx_strdup(cJSON_GetObjectItem(message, "context")->valuestring);
        
        int mode = client->current_response->type == GET_ABOVE_MSG_RESP ? PUSH_FRONT : PUSH_BACK;

        if(client->last_msg_in_chat_id != msg_struct->id && client->first_msg_in_chat_id != msg_struct->id){
        	mx_push_message(client, msg_struct, mode);
        	mx_push_back(&(client->messages_in_current_chat), msg_struct);
        } else {
        	free(msg_struct->messages_str);
        	free(msg_struct->username_from);
        	free(msg_struct);
        	continue;
        }

        if(mode == PUSH_BACK){
        	if(client->first_msg_in_chat_id == -1 && i == 0)
        		client->first_msg_in_chat_id = msg_struct->id;
        	client->last_msg_in_chat_id = msg_struct->id;
        }

        if(mode == PUSH_FRONT){
        	if(client->last_msg_in_chat_id == -1 && i == 0)
        		client->last_msg_in_chat_id = msg_struct->id;
        	client->first_msg_in_chat_id = msg_struct->id;
        }

  	}

  	if(client->last_msg_in_chat_id == -1)
  		client->last_msg_in_chat_id = MSG_ID_LAST;
  	
  	cJSON* deleted_msg_array = cJSON_GetObjectItemCaseSensitive(msg_json, "deleted");
  	int deleted_msg_count = cJSON_GetArraySize(deleted_msg_array);

  	for(int i = 0; i < deleted_msg_count; i++){
  		cJSON *deleted_msg = cJSON_GetArrayItem(deleted_msg_array, i);
  		mx_delete_msg_from_list_box(client, cJSON_GetObjectItem(deleted_msg, "msg_id")->valueint);
  	}

  	cJSON* edited_msg_array = cJSON_GetObjectItemCaseSensitive(msg_json, "edited");
  	int edited_msg_count = cJSON_GetArraySize(edited_msg_array);

  	for(int i = 0; i < edited_msg_count; i++){
  		cJSON* edited_msg = cJSON_GetArrayItem(edited_msg_array, i);
  		mx_msg_edit_update_list(client, cJSON_GetObjectItem(edited_msg, "msg_id")->valueint,
  										cJSON_GetObjectItem(edited_msg, "text")->valuestring);
  	}
}
