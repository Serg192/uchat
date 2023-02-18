#include "../inc/server.h"


static inline void make_daemon() {

	mx_log(SERV_LOG_FILE, LOG_TRACE, "Creating daemon process");

	pid_t pid;
	pid_t sid = 0;

	pid = fork();
	if(pid < 0) {
		mx_log(SERV_LOG_FILE, LOG_ERROR, "fork() failed");
		exit(-1);
	}	

	if(pid > 0) {
		char* log = mx_strjoin("Child pid: ", mx_itoa(pid));
		mx_log(SERV_LOG_FILE, LOG_TRACE, log);
		mx_strdel(&log);

		exit(0);
	}
	umask(0);
	sid = setsid();
	if(sid < 0)
		exit(-1);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

static inline int create_ssocket(int port) {
	struct sockaddr_in address;
	int r_sock = 0;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if((r_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		mx_log(SERV_LOG_FILE, LOG_ERROR, strerror(errno));
		exit(-1);
	}

	if(bind(r_sock, (struct sockaddr*) &address, sizeof(struct sockaddr_in)) == -1) {
		mx_log(SERV_LOG_FILE, LOG_ERROR, strerror(errno));
		exit(-1);
	}

	if(listen(r_sock, 15) == -1) {
		mx_log(SERV_LOG_FILE, LOG_ERROR, strerror(errno));
		exit(-1);
	}

	return r_sock;
}

int main(int argc, char *argv[]) {

	if(argc != 2) {
		mx_printerr("usage: ./uchat_server [port]\n");
		exit(-1);
	}
    
    mx_log(SERV_LOG_FILE, LOG_TRACE, "Starting the server");
	make_daemon();

   	mx_run_serv(create_ssocket(atoi(argv[1])));

	return 0;
}