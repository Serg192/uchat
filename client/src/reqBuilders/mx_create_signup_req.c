#include "../../inc/client.h"

char* mx_create_signup_req(const char* login, const char* password) {
	cJSON* req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "rtype", SIGNUP_REQ);
    cJSON_AddStringToObject(req, "login", login);
    cJSON_AddStringToObject(req, "password", password);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}
