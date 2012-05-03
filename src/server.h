#ifndef __DUNGEONS_SERVER_H__
#define __DUNGEONS_SERVER_H__

struct d_server_context {
	int fd;
	/*
	  io_clients
	*/
};

struct d_server_context* d_server_start_net (const char *addr, int port);
struct d_server_context* d_server_start_local (const char *filename);

#endif
