#include "../../inc/client.h"

char* mx_create_search_chat_req(const char* pattern) {

	cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", CHAT_SEARCH_REQ);
    cJSON_AddStringToObject(req, "pattern", pattern);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}
