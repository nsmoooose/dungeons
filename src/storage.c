#include "error.h"
#include "memory.h"
#include "storage.h"

struct d_storage*
d_storage_new (const char *directory, const char *filename) {
	struct d_storage *s = d_calloc (1, sizeof (struct d_storage));
	char buffer[1024];
	snprintf (buffer, 1024, "%s/%s", directory, filename);
	s->f = fopen (buffer, "r+");
	if (!s->f) {
		d_bug ("Failed to open file for writing.");
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

