#include "../../inc/client.h"

char* mx_create_chat_req(const char* chat_name, int color) {
	cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", CREATE_CHAT_REQ);
    cJSON_AddNumberToObject(req, "color", color);
    cJSON_AddStringToObject(req, "name", chat_name);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}
