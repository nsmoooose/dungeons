#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"

void
d_bug_impl (const char *file, int line, const char *msg) {
	int my_error = errno;
	char *error = strerror (my_error);
	fprintf (stderr, "%s:%d error: %s ERRNO: %d, %s\n", file, line, msg, my_error, error);
    exit(1);
}
