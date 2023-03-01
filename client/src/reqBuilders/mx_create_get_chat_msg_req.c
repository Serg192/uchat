#include "../../inc/client.h"

char* mx_create_get_chat_msg_req(const int chat_id, 
	                             const int flag,
	                             const int start_id,
	                             const int count) {
	 cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", GET_CHAT_MSG_REQ);
    cJSON_AddNumberToObject(req, "room_id", chat_id);
    cJSON_AddNumberToObject(req, "flag", flag);
    cJSON_AddNumberToObject(req, "start_id", start_id);
    cJSON_AddStringToObject(req, "count", count);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}
