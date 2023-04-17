#include "../../inc/client.h"

//////////////////////////////////////////////////////////////////////////////////////
// int flag - can be one of these:                                                  //
// MSG_LOAD_ALL     - load all messages from give chat,                             //
//                  in this case start_id and count can be                          //
//                  any value                                                       //
// MSG_LOAD_ABOVE   - load messages with id less equal than given start id          //
//                  all messages in database have their own id                      //
//                  older messages have id always less than id of new               //
//                  messages.                                                       //
// MSG_LOAD_BELOW   - load messages with id greater equal than given start id       //
//__________________________________________________________________________________//
// int start_id     - works only with flag set to MSG_LOAD_ABOVE and MSG_LOAD_BELOW //
//                  can receive integer id or MSG_ID_LAST which sets start_id to be //
//                  the last message in given chat                                  //
//__________________________________________________________________________________//
// int count        - works only with flag set to MSG_LOAD_ABOVE and MSG_LOAD_BELOW //
//                  defines how many messages should be loaded                      //
//////////////////////////////////////////////////////////////////////////////////////
char* mx_create_get_chat_msg_req(const int chat_id, 
	                             const int flag,
	                             const int start_id,
	                             const int count) {
	cJSON* req = cJSON_CreateObject();

    cJSON_AddNumberToObject(req, "rtype", GET_CHAT_MSG_REQ);
    cJSON_AddNumberToObject(req, "room_id", chat_id);
    cJSON_AddNumberToObject(req, "mode", flag);
    cJSON_AddNumberToObject(req, "start_id", start_id);
    cJSON_AddNumberToObject(req, "count", count);

    char* req_str = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    return req_str;
}
