#include "../inc/client.h"


int mx_create_connection_with_serv(const char* host, int port) {
	struct hostent* ht;
    struct sockaddr_in addr;
    int connection;

    bzero(&addr, sizeof(addr));

    ht = gethostbyname(host);

    if(ht == NULL) {
        perror(host);
        abort();
    }

    connection = socket(PF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //memcpy(&addr.sin_addr, ht->h_addr_list[0], ht->h_length);
    addr.sin_addr.s_addr = *(long *)(ht->h_addr_list[0]);

    if(connect(connection, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        close(connection);
        perror(host);
        abort();
    }

    return connection;
}
