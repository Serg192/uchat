#include "../../inc/server.h"

static inline void create_table(sqlite3* db, char* sql_command) {
	char* err;

	if(sqlite3_exec(db, sql_command, NULL, NULL, &err)) {
		mx_log(SERV_LOG_FILE, LOG_ERROR, err);
		sqlite3_close(db);
		exit(-1);
	}
}

void mx_init_database() {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Database initialization");

	sqlite3* db = mx_open_db();

	char* client =  	"CREATE TABLE IF NOT EXISTS 'user' ("
				    	"'id' INTEGER PRIMARY KEY AUTOINCREMENT,"
				    	"'username' VARCHAR(32) NOT NULL,"
				    	"'password' TEXT NOT NULL);";

	char* room =    	"CREATE TABLE IF NOT EXISTS 'room' ("
				    	"'id' INTEGER PRIMARY KEY AUTOINCREMENT,"
						"'color' INTEGER NOT NULL,"
				    	"'name' VARCHAR(32) NOT NULL);";
	
	char* message = 	"CREATE TABLE IF NOT EXISTS 'message' ("
						"'id' INTEGER PRIMARY KEY AUTOINCREMENT,"
						"'from_id' INTEGER NOT NULL,"
						"'room_id' INTEGER NOT NULL,"
						"'sending_date' INTEGER NOT NULL,"
						"'sending_time' INTEGER NOT_NULL,"
						"'context' TEXT NOT NULL);";

	char* room_member = "CREATE TABLE IF NOT EXISTS 'room_member' ("
						"'client_id' INTEGER NOT NULL,"
						"'room_id' INTEGER NOT NULL, "
						"'banned' INTEGER NOT NULL,"
						"'permissions' INTEGER NOT NULL);";

	create_table(db, client);
	create_table(db, room);
	create_table(db, message);
	create_table(db, room_member);

	mx_close_db(db);
}

