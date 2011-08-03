#include <time.h>

#include "clock.h"
#include "error.h"

double
d_time_get () {
	struct timespec clock;
	if (clock_gettime (CLOCK_REALTIME, &clock) == -1) {
		d_bug ("Failed to get clock.");
	}
	return clock.tv_sec + ((double)clock.tv_nsec / 1000000000.0);
}
