#include <sys/types.h>
#include <sys/socket.h>

#include "server.h"

struct d_server_context*
d_server_start_net (const char *addr, int port) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		return 0;
	}
	return 0;
}

struct d_server_context*
d_server_start_local (const char *filename) {
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) {
		return 0;
	}
	return 0;
}
