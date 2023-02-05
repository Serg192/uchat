#include "../inc/server.h"


static inline void make_daemon() {
	pid_t pid;
	pid_t sid = 0;

	pid = fork();
	if(pid < 0) {
		mx_printerr("fork() failed\n");
		exit(-1);
	}	

	if(pid > 0) {
		mx_printstr("child pid: ");
		mx_printint(pid);
		mx_printstr("\n");
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
		mx_printerr(strerror(errno));
		mx_printerr("\n");
		exit(-1);
	}

	if(bind(r_sock, (struct sockaddr*) &address, sizeof(struct sockaddr_in)) == -1) {
		mx_printerr(strerror(errno));
		mx_printerr("\n");
		exit(-1);
	}

	if(listen(r_sock, 15) == -1) {
		mx_printerr(strerror(errno));
		mx_printerr("\n");
		exit(-1);
	}

	return r_sock;
}

int main(int argc, char *argv[]) {

	if(argc != 2) {
		mx_printerr("usage: ./uchat_server [port]\n");
		exit(-1);
	}

	//make_daemon();
    
    int s_sock = create_ssocket(atoi(argv[1]));
    
   	struct sockaddr_in c_addr;
   	socklen_t addr_size = sizeof(struct sockaddr_in);
   	int c_sock;

    while(1) {
    	if((c_sock = accept(s_sock, (struct sockaddr*) &c_addr, &addr_size)) != -1) {
    		mx_printstr("new connection\n");
    	} else {

    	}
    }

	return 0;
}