#include "../../inc/client.h"

char* mx_create_permissions_change_req(int chat_id, int user_id, int perm) {
	cJSON* req = cJSON_CreateObject();

	cJSON_AddNumberToObject(req, "chat_id", chat_id);
    cJSON_AddNumberToObject(req, "user_id", user_id);
    cJSON_AddNumberToObject(req, "permissions", perm);
    cJSON_AddNumberToObject(req, "rtype", SET_PERM_REQ);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}
