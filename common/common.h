#ifndef COMMON_H
#define COMMON_H

typedef enum request_type_e {
	LOGIN_REQ,
	SIGNUP_REQ,
	QUIT_REQ
}            request_type;

typedef enum respond_type_e {
	LOGIN_ERR_RESP,
	SIGNUP_ERR_RESP,
	SIGNUP_SUCCESS_RESP,
	LOGIN_SUCESS_RESP
}			  respond_type;


#endif