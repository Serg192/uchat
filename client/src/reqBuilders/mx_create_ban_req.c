#include "../../inc/client.h"

char* mx_create_ban_req(int chat_id, int user_id, int banned) {
	cJSON* req = cJSON_CreateObject();

	cJSON_AddNumberToObject(req, "chat_id", chat_id);
    cJSON_AddNumberToObject(req, "user_id", user_id);
    cJSON_AddNumberToObject(req, "banned", banned);
    cJSON_AddNumberToObject(req, "rtype", BAN_REQ);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}

