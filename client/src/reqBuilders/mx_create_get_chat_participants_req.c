#include "../../inc/client.h"

char* mx_create_get_chat_participants_req(int chat_id) {
	cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", GET_CHAT_PARTICIPANTS_REQ);
    cJSON_AddNumberToObject(req, "room_id", chat_id);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}
