#include <stdlib.h>

#include "memory.h"

unsigned int d_allocations = 0;
unsigned int d_frees = 0;

void*
d_calloc (size_t nmemb, size_t size) {
	d_allocations++;
	return calloc (nmemb, size);
}

void
d_free (void *ptr) {
	d_frees++;
	free (ptr);
}
