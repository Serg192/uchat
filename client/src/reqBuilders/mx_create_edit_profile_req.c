#include "../../inc/client.h"

//Add chat id
char* mx_create_edit_profile_req(const char* new_username, const char* new_password){
    cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", PROFILE_EDIT_REQ);
    cJSON_AddStringToObject(req, "username", new_username);
    cJSON_AddStringToObject(req, "password", new_password);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}

