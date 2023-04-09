#include "../../inc/client.h"

char* mx_create_quit_request(){
    cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", QUIT_REQ);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}
