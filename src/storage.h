#ifndef __DUNGEONS_STORAGE_H__
#define __DUNGEONS_STORAGE_H__

#include "list.h"

struct d_storage_world_info {
	char *path;
	char *description;
};

struct d_storage {
	int i;
};

struct d_list *d_storage_world_list ();
void d_storage_world_destroy (const char *path);

struct d_storage* d_storage_new (const char *directory,
								 const char *filename);
void d_storage_close (struct d_storage *storage);
void d_storage_write_d (struct d_storage *storage, const char *key,
						double *value);
void d_storage_read_d (struct d_storage *storage, const char *key,
					   double *value);

#endif
