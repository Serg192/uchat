#include "../../inc/client.h"

char* mx_create_get_joined_chats_req() {
	cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", GET_JOINED_CHATS);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}
