#include "../../inc/client.h"

char* mx_create_send_msg_req(const int room_id,
                             const int sending_date,
                             const int sending_time,
                             const char* context) {
	                                       
    cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", SEND_MSG_REQ);
    cJSON_AddNumberToObject(req, "room_id", room_id);
    cJSON_AddNumberToObject(req, "sending_time", sending_time);
    cJSON_AddNumberToObject(req, "sending_date", sending_date);
    cJSON_AddStringToObject(req, "context", context);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);
    return req_str;
}
