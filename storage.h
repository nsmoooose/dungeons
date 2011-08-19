#ifndef __DUNGEONS_STORAGE_H__
#define __DUNGEONS_STORAGE_H__

#include "list.h"

struct d_storage_world_info {
	char *path;
	char *description;
};

struct d_list *d_storage_world_list ();
void d_storage_world_destroy (const char *path);

#endif
