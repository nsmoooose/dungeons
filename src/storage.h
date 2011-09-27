#ifndef __DUNGEONS_STORAGE_H__
#define __DUNGEONS_STORAGE_H__

#include <stdio.h>

#include "list.h"

struct d_storage {
	FILE *f;
};

struct d_storage* d_storage_new (const char *directory,
								 const char *filename);
void d_storage_close (struct d_storage *storage);
void d_storage_write_d (struct d_storage *storage, double *value);
void d_storage_read_d (struct d_storage *storage, double *value);

void d_storage_write_i (struct d_storage *storage, int *value);
void d_storage_read_i (struct d_storage *storage, int *value);

#endif
