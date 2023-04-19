#include "../../inc/client.h"

//Add chat id
char* mx_create_delete_chat_msg_req(const int chat_id, const int message_id){
    cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", DEL_CHAT_MSG_REQ);
    cJSON_AddNumberToObject(req, "chat_id", chat_id);
    cJSON_AddNumberToObject(req, "id", message_id);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}

