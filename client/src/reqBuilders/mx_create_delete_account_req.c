#include "../../inc/client.h"

char* mx_create_delete_account_req(){
    cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", DEL_ACCOUNT_REQ);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}
