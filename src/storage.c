#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include "error.h"
#include "memory.h"
#include "storage.h"

struct d_storage*
d_storage_new (const char *directory, const char *filename) {
	struct d_storage *s = d_calloc (1, sizeof (struct d_storage));
	char buffer[1024];
	snprintf (buffer, 1024, "%s/%s", directory, filename);

	struct stat file_info;
	if (stat (buffer, &file_info) == -1) {
		s->f = fopen (buffer, "w");
	}
	else {
		s->f = fopen (buffer, "r+");
	}
	if (!s->f) {
		d_bug ("Failed to open file for writing. (%s)", buffer);
	}
	return s;
}

void
d_storage_close (struct d_storage *storage) {
	fclose (storage->f);
	d_free (storage);
}

void
d_storage_write_d (struct d_storage *storage, double *value) {
	fwrite (value, sizeof (double), 1, storage->f);
}

void
d_storage_read_d (struct d_storage *storage, double *value) {
	fread (value, sizeof (double), 1, storage->f);
}

void
d_storage_write_i (struct d_storage *storage, int *value) {
	fwrite (value, sizeof (int), 1, storage->f);
}

void
d_storage_read_i (struct d_storage *storage, int *value) {
	fread (value, sizeof (int), 1, storage->f);
}

void
d_storage_write_s (struct d_storage *storage, const char *value) {
	int length = strlen (value);
	d_storage_write_i (storage, &length);
	fwrite (value, sizeof (char), length, storage->f);
}

char *
d_storage_read_s (struct d_storage *storage) {
	int length;
	d_storage_read_i (storage, &length);
	char *buffer = d_calloc (1, length + 1);
	fread (buffer, sizeof (char), length, storage->f);
	return buffer;
}
