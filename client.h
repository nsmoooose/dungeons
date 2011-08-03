#ifndef __DUNGEONS_CLIENT_H__
#define __DUNGEONS_CLIENT_H__

struct d_client_context {
	int fd;
	int id;
	char *description;

	/* */
};

struct d_client_context* d_client_connect_local (const char *filename);
struct d_client_context* d_client_connect_net (const char *address, int port);
int d_client_process_io (struct d_client_context *context);

#endif
