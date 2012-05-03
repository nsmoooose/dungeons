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

	/*
	int sfd, cfd;
	struct sockaddr_un my_addr, peer_addr;
	socklen_t peer_addr_size;

	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, MY_SOCK_PATH,
	        sizeof(my_addr.sun_path) - 1);

	if (bind(fd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_un)) == -1)
		handle_error("bind");

	if (listen(fd, LISTEN_BACKLOG) == -1)
		handle_error("listen");

	peer_addr_size = sizeof(struct sockaddr_un);
	cfd = accept(sfd, (struct sockaddr *) &peer_addr,
	             &peer_addr_size);
	if (cfd == -1)
		handle_error("accept");
	*/

	return 0;
}
