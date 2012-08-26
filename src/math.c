#include <math.h>
#include <stdlib.h>
#include "math.h"

int
d_is_power_of_2 (int size) {
    int i, bitcount = 0;
    for (i=0; i<sizeof(int)*8; i++) {
		if (size & (1<<i)) {
			bitcount++;
		}
	}
    if (bitcount == 1) {
		return 1;
	}
    else {
		return 0;
	}
}

int
d_rand (int min, int max) {
	return rand () % (max-min) + min;
}

int
d_aabb3_contains (struct d_aabb3 *aabb, struct d_point3 *pos) {
	if (pos->x >= aabb->p1.x && pos->x <= aabb->p2.x &&
	    pos->y >= aabb->p1.y && pos->y <= aabb->p2.y &&
	    pos->z >= aabb->p1.z && pos->z <= aabb->p2.z) {
		return 1;
	}
	return 0;
}

void
d_aabb3_size (struct d_aabb3 *aabb, struct d_point3 *pos) {
	pos->x = fabs (aabb->p2.x - aabb->p1.x) + 1;
	pos->y = fabs (aabb->p2.y - aabb->p1.y) + 1;
	pos->z = fabs (aabb->p2.z - aabb->p1.z) + 1;
}
