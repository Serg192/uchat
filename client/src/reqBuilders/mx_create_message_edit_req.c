#include "../../inc/client.h"

char* mx_cteate_message_edit_req(int chat_id, int message_id, char* new_text){
	cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", EDIT_MSG_REQ);
    cJSON_AddNumberToObject(req, "chat_id", chat_id);
    cJSON_AddNumberToObject(req, "message_id", message_id);
    cJSON_AddStringToObject(req, "text", new_text);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}
