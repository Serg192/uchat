#ifndef COMMON_H
#define COMMON_H

typedef enum request_type_e {
	LOGIN_REQ,
	SIGNUP_REQ,
	CREATE_CHAT_REQ,
	CHAT_SEARCH_REQ,
	JOIN_CHAT_REQ,
	GET_CHAT_PARTICIPANTS_REQ,
	SEND_MSG_REQ,
	EDIT_MSG_REQ,
	GET_CHAT_MSG_REQ,
	DEL_CHAT_MSG_REQ,
	GET_JOINED_CHATS,
	QUIT_REQ
}            request_type;



//Change to OK | ERROR
//because this is stupid  
typedef enum respond_type_e {
	LOGIN_ERR_RESP,
	SIGNUP_ERR_RESP,
	SIGNUP_SUCCESS_RESP,
	LOGIN_SUCESS_RESP,
	CHAT_CREATION_SUCCESS_RESP,
	CHAT_CREATION_ERR_RESP,
	ADD_CHAT_MEM_ERR_RESP,
	ADD_CHAT_MEM_SUCCESS_RESP,
	CHAT_SEARCH_RESP,
	PARTICIPANTS_RESP,
	MSG_SEND_SUCCESS_RESP,
	//GET_CHAT_MSG_RES,
	GET_ABOVE_MSG_RESP,
	GET_BELOW_MSG_RESP,
	JOINED_CHATS_RESP,
	OK_RESP
}			  respond_type;


enum {
	MSG_LOAD_ALL,
	MSG_LOAD_ABOVE,
	MSG_LOAD_BELOW
};

enum {
	MSG_ID_LAST = -2
};

#endif