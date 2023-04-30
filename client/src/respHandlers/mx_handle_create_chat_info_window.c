#include "../../inc/client.h"



void mx_handle_create_chat_info_window(client_t* client){
	const int own_permissions = cJSON_GetObjectItem(client->current_response->json, "own_permissions")->valueint;

	cJSON* chat_m_array = cJSON_GetObjectItemCaseSensitive(client->current_response->json, "users");
	const int chat_m_array_len = cJSON_GetArraySize(chat_m_array);

	for(int i = 0; i < chat_m_array_len; i++){
		cJSON* user = cJSON_GetArrayItem(chat_m_array, i);

		members_list_entry_t* mi = (members_list_entry_t*)malloc(sizeof(members_list_entry_t));
		mi->client = client;
		mi->this_client_permissions = own_permissions;
		mi->user_to_add = mx_strdup(cJSON_GetObjectItem(user, "username")->valuestring);
		mi->userID_to_add = cJSON_GetObjectItem(user, "id")->valueint;
		mi->banned = cJSON_GetObjectItem(user, "banned")->valueint;
		mi->member_permissions = cJSON_GetObjectItem(user, "permissions")->valueint;

		//printf("MY PERMISSIONS: %d, MEMBER_PERMISSIONS: %d\n", own_permissions, mi->member_permissions);
		mx_user_info_add_user(mi);
	} 

}
