#include <stdio.h>

#include "curses_io.h"
#include "memory.h"

int
main (int argc, char *argv[]) {
	d_curses_run ();

	if (d_allocations != d_frees) {
		fprintf (stderr,
				 "ERROR: Memory leaks detected. Allocations and free doesn't match. "
				 "Allocations: %d, Free: %d\n", d_allocations, d_frees);
		return 1;
	}

	return 0;
}
