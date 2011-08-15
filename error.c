#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"

void
d_bug_impl (const char *file, int line, const char *msg, ...) {
	char buffer[1000];
	va_list args;
	va_start (args, msg);
	vsnprintf (buffer, 1000, msg, args);
	va_end (args);

	int my_error = errno;
	char *error = strerror (my_error);
	fprintf (stderr, "%s:%d error: %s ERRNO: %d, %s\n", file, line, buffer, my_error, error);

    exit(1);
}
