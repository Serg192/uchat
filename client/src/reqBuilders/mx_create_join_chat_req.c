#include "../../inc/client.h"

char* mx_create_join_chat_req(const int chat_id) {
	cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", JOIN_CHAT_REQ);
    cJSON_AddNumberToObject(req, "room_id", chat_id);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}

