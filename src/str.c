#include <string.h>

#include "memory.h"
#include "str.h"

char *
d_strdup (const char *str) {
	char *s = strdup (str);
	d_allocations++;
	return s;
}

void
d_strfree (char *str) {
	d_free (str);
}

